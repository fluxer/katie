/*
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Eric Seidel <eric@webkit.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "Platform.h"
#include "Collector.h"

#include "ArgList.h"
#include "CallFrame.h"
#include "CodeBlock.h"
#include "CollectorHeapIterator.h"
#include "Interpreter.h"
#include "JSArray.h"
#include "JSGlobalObject.h"
#include "JSONObject.h"
#include "JSString.h"
#include "JSValue.h"
#include "JSZombie.h"
#include "MarkStack.h"
#include "Nodes.h"
#include <wtf/FastMalloc.h>
#include <wtf/HashCountedSet.h>

#include <algorithm>
#include <limits.h>
#include <setjmp.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <pthread.h>

#define COLLECT_ON_EVERY_ALLOCATION 0

namespace JSC {

// tunable parameters

const size_t GROWTH_FACTOR = 2;
const size_t LOW_WATER_FACTOR = 4;
const size_t ALLOCATIONS_PER_COLLECTION = 3600;
// This value has to be a macro to be used in max() without introducing
// a PIC branch in Mach-O binaries, see <rdar://problem/5971391>.
#define MIN_ARRAY_SIZE (static_cast<size_t>(14))

Heap::Heap(JSGlobalData* globalData)
    : m_markListSet(0)
    , m_globalData(globalData)
{
    Q_ASSERT(globalData);
    memset(&m_heap, 0, sizeof(CollectorHeap));
    allocateBlock();
}

Heap::~Heap()
{
    // The destroy function must already have been called, so assert this.
    Q_ASSERT(!m_globalData);
}

void Heap::destroy()
{
    if (!m_globalData)
        return;

    Q_ASSERT(!m_globalData->dynamicGlobalObject);
    Q_ASSERT(!isBusy());
    
    // The global object is not GC protected at this point, so sweeping may delete it
    // (and thus the global data) before other objects that may use the global data.
    RefPtr<JSGlobalData> protect(m_globalData);

    delete m_markListSet;
    m_markListSet = 0;

    freeBlocks();

    m_globalData = 0;
}

NEVER_INLINE CollectorBlock* Heap::allocateBlock()
{
#if defined(QT_HAVE_POSIX_MEMALIGN)
    void* address;
    if (::posix_memalign(&address, BLOCK_SIZE, BLOCK_SIZE) != 0)
        CRASH();
#else
    static size_t pagesize = ::getpagesize();

    size_t extra = 0;
    if (BLOCK_SIZE > pagesize)
        extra = BLOCK_SIZE - pagesize;

    void* mmapResult = QT_MMAP(NULL, BLOCK_SIZE + extra, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    uintptr_t address = reinterpret_cast<uintptr_t>(mmapResult);

    size_t adjust = 0;
    if ((address & BLOCK_OFFSET_MASK) != 0)
        adjust = BLOCK_SIZE - (address & BLOCK_OFFSET_MASK);

    if (adjust > 0)
        ::munmap(reinterpret_cast<char*>(address), adjust);

    if (adjust < extra)
        ::munmap(reinterpret_cast<char*>(address + adjust + BLOCK_SIZE), extra - adjust);

    address += adjust;
#endif

    // Initialize block.

    CollectorBlock* block = reinterpret_cast<CollectorBlock*>(address);
    block->heap = this;
    clearMarkBits(block);

    Structure* dummyMarkableCellStructure = m_globalData->dummyMarkableCellStructure.get();
    for (size_t i = 0; i < HeapConstants::cellsPerBlock; ++i)
        new (block->cells + i) JSCell(dummyMarkableCellStructure);
    
    // Add block to blocks vector.

    size_t numBlocks = m_heap.numBlocks;
    if (m_heap.usedBlocks == numBlocks) {
        static const size_t maxNumBlocks = ULONG_MAX / sizeof(CollectorBlock*) / GROWTH_FACTOR;
        if (numBlocks > maxNumBlocks)
            CRASH();
        numBlocks = std::max(MIN_ARRAY_SIZE, numBlocks * GROWTH_FACTOR);
        m_heap.numBlocks = numBlocks;
        m_heap.blocks = static_cast<CollectorBlock**>(fastRealloc(m_heap.blocks, numBlocks * sizeof(CollectorBlock*)));
    }
    m_heap.blocks[m_heap.usedBlocks++] = block;

    return block;
}

NEVER_INLINE void Heap::freeBlock(size_t block)
{
    m_heap.didShrink = true;

    ObjectIterator it(m_heap, block);
    ObjectIterator end(m_heap, block + 1);
    for ( ; it != end; ++it)
        (*it)->~JSCell();
    freeBlockPtr(m_heap.blocks[block]);

    // swap with the last block so we compact as we go
    m_heap.blocks[block] = m_heap.blocks[m_heap.usedBlocks - 1];
    m_heap.usedBlocks--;

    if (m_heap.numBlocks > MIN_ARRAY_SIZE && m_heap.usedBlocks < m_heap.numBlocks / LOW_WATER_FACTOR) {
        m_heap.numBlocks = m_heap.numBlocks / GROWTH_FACTOR; 
        m_heap.blocks = static_cast<CollectorBlock**>(fastRealloc(m_heap.blocks, m_heap.numBlocks * sizeof(CollectorBlock*)));
    }
}

NEVER_INLINE void Heap::freeBlockPtr(CollectorBlock* block)
{
#if defined(QT_HAVE_POSIX_MEMALIGN)
    ::free(block);
#else
    ::munmap(reinterpret_cast<char*>(block), BLOCK_SIZE);
#endif
}

void Heap::freeBlocks()
{
    ProtectCountSet protectedValuesCopy = m_protectedValues;

    clearMarkBits();
    ProtectCountSet::iterator protectedValuesEnd = protectedValuesCopy.end();
    for (ProtectCountSet::iterator it = protectedValuesCopy.begin(); it != protectedValuesEnd; ++it)
        markCell(it->first);

    m_heap.nextCell = 0;
    m_heap.nextBlock = 0;
    DeadObjectIterator it(m_heap, m_heap.nextBlock, m_heap.nextCell);
    DeadObjectIterator end(m_heap, m_heap.usedBlocks);
    for ( ; it != end; ++it)
        (*it)->~JSCell();

    Q_ASSERT(!protectedObjectCount());

    protectedValuesEnd = protectedValuesCopy.end();
    for (ProtectCountSet::iterator it = protectedValuesCopy.begin(); it != protectedValuesEnd; ++it)
        it->first->~JSCell();

    for (size_t block = 0; block < m_heap.usedBlocks; ++block)
        freeBlockPtr(m_heap.blocks[block]);

    fastFree(m_heap.blocks);

    memset(&m_heap, 0, sizeof(CollectorHeap));
}

void Heap::recordExtraCost(size_t cost)
{
    // Our frequency of garbage collection tries to balance memory use against speed
    // by collecting based on the number of newly created values. However, for values
    // that hold on to a great deal of memory that's not in the form of other JS values,
    // that is not good enough - in some cases a lot of those objects can pile up and
    // use crazy amounts of memory without a GC happening. So we track these extra
    // memory costs. Only unusually large objects are noted, and we only keep track
    // of this extra cost until the next GC. In garbage collected languages, most values
    // are either very short lived temporaries, or have extremely long lifetimes. So
    // if a large value survives one garbage collection, there is not much point to
    // collecting more frequently as long as it stays alive.

    if (m_heap.extraCost > maxExtraCost && m_heap.extraCost > m_heap.usedBlocks * BLOCK_SIZE / 2) {
        // If the last iteration through the heap deallocated blocks, we need
        // to clean up remaining garbage before marking. Otherwise, the conservative
        // marking mechanism might follow a pointer to unmapped memory.
        if (m_heap.didShrink)
            sweep();
        reset();
    }
    m_heap.extraCost += cost;
}

void* Heap::allocate(size_t s)
{
    typedef HeapConstants::Block Block;
    typedef HeapConstants::Cell Cell;

    Q_UNUSED(s);
    Q_ASSERT(s <= HeapConstants::cellSize);

    Q_ASSERT(m_heap.operationInProgress == NoOperation);

#if COLLECT_ON_EVERY_ALLOCATION
    collectAllGarbage();
    Q_ASSERT(m_heap.operationInProgress == NoOperation);
#endif

allocate:

    // Fast case: find the next garbage cell and recycle it.

    do {
        Q_ASSERT(m_heap.nextBlock < m_heap.usedBlocks);
        Block* block = reinterpret_cast<Block*>(m_heap.blocks[m_heap.nextBlock]);
        do {
            Q_ASSERT(m_heap.nextCell < HeapConstants::cellsPerBlock);
            if (!block->marked.get(m_heap.nextCell)) { // Always false for the last cell in the block
                Cell* cell = block->cells + m_heap.nextCell;

                m_heap.operationInProgress = Allocation;
                JSCell* imp = reinterpret_cast<JSCell*>(cell);
                imp->~JSCell();
                m_heap.operationInProgress = NoOperation;

                ++m_heap.nextCell;
                return cell;
            }
        } while (++m_heap.nextCell != HeapConstants::cellsPerBlock);
        m_heap.nextCell = 0;
    } while (++m_heap.nextBlock != m_heap.usedBlocks);

    // Slow case: reached the end of the heap. Mark live objects and start over.

    reset();
    goto allocate;
}

void Heap::resizeBlocks()
{
    m_heap.didShrink = false;

    size_t usedCellCount = markedCells();
    size_t minCellCount = usedCellCount + std::max(ALLOCATIONS_PER_COLLECTION, usedCellCount);
    size_t minBlockCount = (minCellCount + HeapConstants::cellsPerBlock - 1) / HeapConstants::cellsPerBlock;

    size_t maxCellCount = 1.25f * minCellCount;
    size_t maxBlockCount = (maxCellCount + HeapConstants::cellsPerBlock - 1) / HeapConstants::cellsPerBlock;

    if (m_heap.usedBlocks < minBlockCount)
        growBlocks(minBlockCount);
    else if (m_heap.usedBlocks > maxBlockCount)
        shrinkBlocks(maxBlockCount);
}

void Heap::growBlocks(size_t neededBlocks)
{
    Q_ASSERT(m_heap.usedBlocks < neededBlocks);
    while (m_heap.usedBlocks < neededBlocks)
        allocateBlock();
}

void Heap::shrinkBlocks(size_t neededBlocks)
{
    Q_ASSERT(m_heap.usedBlocks > neededBlocks);
    
    // Clear the always-on last bit, so isEmpty() isn't fooled by it.
    for (size_t i = 0; i < m_heap.usedBlocks; ++i)
        m_heap.blocks[i]->marked.clear(HeapConstants::cellsPerBlock - 1);

    for (size_t i = 0; i != m_heap.usedBlocks && m_heap.usedBlocks != neededBlocks; ) {
        if (m_heap.blocks[i]->marked.isEmpty()) {
            freeBlock(i);
        } else
            ++i;
    }

    // Reset the always-on last bit.
    for (size_t i = 0; i < m_heap.usedBlocks; ++i)
        m_heap.blocks[i]->marked.set(HeapConstants::cellsPerBlock - 1);
}

inline bool isPointerAligned(void* p)
{
    return (((intptr_t)(p) & (sizeof(char*) - 1)) == 0);
}

// Cell size needs to be a power of two for isPossibleCell to be valid.
COMPILE_ASSERT(sizeof(CollectorCell) % 2 == 0, Collector_cell_size_is_power_of_two);

static inline bool isCellAligned(void *p)
{
    return (((intptr_t)(p) & CELL_MASK) == 0);
}

static inline bool isPossibleCell(void* p)
{
    return isCellAligned(p) && p;
}

void Heap::markConservatively(MarkStack& markStack, void* start, void* end)
{
    if (start > end) {
        void* tmp = start;
        start = end;
        end = tmp;
    }

    Q_ASSERT((static_cast<char*>(end) - static_cast<char*>(start)) < 0x1000000);
    Q_ASSERT(isPointerAligned(start));
    Q_ASSERT(isPointerAligned(end));

    char** p = static_cast<char**>(start);
    char** e = static_cast<char**>(end);

    CollectorBlock** blocks = m_heap.blocks;
    while (p != e) {
        char* x = *p++;
        if (isPossibleCell(x)) {
            size_t usedBlocks;
            uintptr_t xAsBits = reinterpret_cast<uintptr_t>(x);
            xAsBits &= CELL_ALIGN_MASK;

            uintptr_t offset = xAsBits & BLOCK_OFFSET_MASK;
            const size_t lastCellOffset = sizeof(CollectorCell) * (CELLS_PER_BLOCK - 1);
            if (offset > lastCellOffset)
                continue;

            CollectorBlock* blockAddr = reinterpret_cast<CollectorBlock*>(xAsBits - offset);
            usedBlocks = m_heap.usedBlocks;
            for (size_t block = 0; block < usedBlocks; block++) {
                if (blocks[block] != blockAddr)
                    continue;
                markStack.append(reinterpret_cast<JSCell*>(xAsBits));
                markStack.drain();
            }
        }
    }
}

void NEVER_INLINE Heap::markCurrentThreadConservativelyInternal(MarkStack& markStack)
{
    // pointer size must be pointer aligned
#if defined(PTHREAD_STACK_MIN) && defined(_SC_THREAD_ATTR_STACKSIZE)
    static long stackPointerSize = sysconf(_SC_THREAD_ATTR_STACKSIZE) * QT_POINTER_SIZE;
    if (stackPointerSize == -1)
        stackPointerSize = PTHREAD_STACK_MIN * QT_POINTER_SIZE;

    char stackPointer[stackPointerSize];
    char* stackBase = stackPointer + stackPointerSize;
#else
    enum { stackPointerSize = USHRT_MAX * QT_POINTER_SIZE};
    static thread_local char stackPointer[stackPointerSize];
    static thread_local char* stackBase = stackPointer + stackPointerSize;
#endif
    markConservatively(markStack, stackPointer, stackBase);
}

#define REGISTER_BUFFER_ALIGNMENT Q_DECL_ALIGN(QT_POINTER_SIZE)

void Heap::markCurrentThreadConservatively(MarkStack& markStack)
{
    // setjmp forces volatile registers onto the stack
    jmp_buf registers REGISTER_BUFFER_ALIGNMENT;
    setjmp(registers);

    markCurrentThreadConservativelyInternal(markStack);
}

void Heap::protect(JSValue k)
{
    Q_ASSERT(k);
    Q_ASSERT(!m_globalData->isSharedInstance);

    if (!k.isCell())
        return;

    m_protectedValues.add(k.asCell());
}

void Heap::unprotect(JSValue k)
{
    Q_ASSERT(k);
    Q_ASSERT(!m_globalData->isSharedInstance);

    if (!k.isCell())
        return;

    m_protectedValues.remove(k.asCell());
}

void Heap::markProtectedObjects(MarkStack& markStack)
{
    ProtectCountSet::iterator end = m_protectedValues.end();
    for (ProtectCountSet::iterator it = m_protectedValues.begin(); it != end; ++it) {
        markStack.append(it->first);
        markStack.drain();
    }
}

void Heap::clearMarkBits()
{
    for (size_t i = 0; i < m_heap.usedBlocks; ++i)
        clearMarkBits(m_heap.blocks[i]);
}

void Heap::clearMarkBits(CollectorBlock* block)
{
    // allocate assumes that the last cell in every block is marked.
    block->marked.clearAll();
    block->marked.set(HeapConstants::cellsPerBlock - 1);
}

size_t Heap::markedCells(size_t startBlock, size_t startCell) const
{
    Q_ASSERT(startBlock <= m_heap.usedBlocks);
    Q_ASSERT(startCell < HeapConstants::cellsPerBlock);

    if (startBlock >= m_heap.usedBlocks)
        return 0;

    size_t result = 0;
    result += m_heap.blocks[startBlock]->marked.count(startCell);
    for (size_t i = startBlock + 1; i < m_heap.usedBlocks; ++i)
        result += m_heap.blocks[i]->marked.count();

    return result;
}

void Heap::sweep()
{
    Q_ASSERT(m_heap.operationInProgress == NoOperation);
    if (m_heap.operationInProgress != NoOperation)
        CRASH();
    m_heap.operationInProgress = Collection;
    
#if !ENABLE(JSC_ZOMBIES)
    Structure* dummyMarkableCellStructure = m_globalData->dummyMarkableCellStructure.get();
#endif

    DeadObjectIterator it(m_heap, m_heap.nextBlock, m_heap.nextCell);
    DeadObjectIterator end(m_heap, m_heap.usedBlocks);
    for ( ; it != end; ++it) {
        JSCell* cell = *it;
#if ENABLE(JSC_ZOMBIES)
        if (!cell->isZombie()) {
            const ClassInfo* info = cell->classInfo();
            cell->~JSCell();
            new (cell) JSZombie(info, JSZombie::leakedZombieStructure());
            Heap::markCell(cell);
        }
#else
        cell->~JSCell();
        // Callers of sweep assume it's safe to mark any cell in the heap.
        new (cell) JSCell(dummyMarkableCellStructure);
#endif
    }

    m_heap.operationInProgress = NoOperation;
}

void Heap::markRoots()
{
    Q_ASSERT(m_heap.operationInProgress == NoOperation);
    if (m_heap.operationInProgress != NoOperation)
        CRASH();

    m_heap.operationInProgress = Collection;

    MarkStack& markStack = m_globalData->markStack;

    // Reset mark bits.
    clearMarkBits();

    // Mark stack roots.
    markCurrentThreadConservatively(markStack);
    m_globalData->interpreter->registerFile().markCallFrames(markStack, this);

    // Mark explicitly registered roots.
    markProtectedObjects(markStack);

    // Mark misc. other roots.
    if (m_markListSet && m_markListSet->size())
        MarkedArgumentBuffer::markLists(markStack, *m_markListSet);
    if (m_globalData->exception)
        markStack.append(m_globalData->exception);
    m_globalData->smallStrings.markChildren(markStack);
    if (m_globalData->functionCodeBlockBeingReparsed)
        m_globalData->functionCodeBlockBeingReparsed->markAggregate(markStack);
    if (m_globalData->firstStringifierToMark)
        JSONObject::markStringifiers(markStack, m_globalData->firstStringifierToMark);

    if (m_globalData->clientData)
        m_globalData->clientData->mark(markStack);

    markStack.drain();
    markStack.compact();

    m_heap.operationInProgress = NoOperation;
}

size_t Heap::objectCount() const
{
    return m_heap.nextBlock * HeapConstants::cellsPerBlock // allocated full blocks
           + m_heap.nextCell // allocated cells in current block
           + markedCells(m_heap.nextBlock, m_heap.nextCell) // marked cells in remainder of m_heap
           - m_heap.usedBlocks; // 1 cell per block is a dummy sentinel
}

void Heap::addToStatistics(Heap::Statistics& statistics) const
{
    statistics.size += m_heap.usedBlocks * BLOCK_SIZE;
    statistics.free += m_heap.usedBlocks * BLOCK_SIZE - (objectCount() * HeapConstants::cellSize);
}

Heap::Statistics Heap::statistics() const
{
    Statistics statistics = { 0, 0 };
    addToStatistics(statistics);
    return statistics;
}

size_t Heap::globalObjectCount()
{
    size_t count = 0;
    if (JSGlobalObject* head = m_globalData->head) {
        JSGlobalObject* o = head;
        do {
            ++count;
            o = o->next();
        } while (o != head);
    }
    return count;
}

size_t Heap::protectedGlobalObjectCount()
{
    size_t count = 0;
    if (JSGlobalObject* head = m_globalData->head) {
        JSGlobalObject* o = head;
        do {
            if (m_protectedValues.contains(o))
                ++count;
            o = o->next();
        } while (o != head);
    }

    return count;
}

size_t Heap::protectedObjectCount()
{
    return m_protectedValues.size();
}

static const char* typeName(JSCell* cell)
{
    if (cell->isString())
        return "string";
    if (cell->isGetterSetter())
        return "gettersetter";
    if (cell->isAPIValueWrapper())
        return "value wrapper";
    if (cell->isPropertyNameIterator())
        return "for-in iterator";
    Q_ASSERT(cell->isObject());
    const ClassInfo* info = cell->classInfo();
    return info ? info->className : "Object";
}

HashCountedSet<const char*>* Heap::protectedObjectTypeCounts()
{
    HashCountedSet<const char*>* counts = new HashCountedSet<const char*>;

    ProtectCountSet::iterator end = m_protectedValues.end();
    for (ProtectCountSet::iterator it = m_protectedValues.begin(); it != end; ++it)
        counts->add(typeName(it->first));

    return counts;
}

bool Heap::isBusy()
{
    return m_heap.operationInProgress != NoOperation;
}

void Heap::reset()
{
    markRoots();

    m_heap.nextCell = 0;
    m_heap.nextBlock = 0;
    m_heap.nextNumber = 0;
    m_heap.extraCost = 0;
#if ENABLE(JSC_ZOMBIES)
    sweep();
#endif
    resizeBlocks();
}

void Heap::collectAllGarbage()
{
    // If the last iteration through the heap deallocated blocks, we need
    // to clean up remaining garbage before marking. Otherwise, the conservative
    // marking mechanism might follow a pointer to unmapped memory.
    if (m_heap.didShrink)
        sweep();

    markRoots();

    m_heap.nextCell = 0;
    m_heap.nextBlock = 0;
    m_heap.nextNumber = 0;
    m_heap.extraCost = 0;
    sweep();
    resizeBlocks();
}

LiveObjectIterator Heap::primaryHeapBegin()
{
    return LiveObjectIterator(m_heap, 0);
}

LiveObjectIterator Heap::primaryHeapEnd()
{
    return LiveObjectIterator(m_heap, m_heap.usedBlocks);
}

} // namespace JSC
