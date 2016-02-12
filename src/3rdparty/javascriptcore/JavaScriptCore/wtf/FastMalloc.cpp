// Copyright (c) 2005, 2007, Google Inc.
// All rights reserved.
// Copyright (C) 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// ---
// Author: Sanjay Ghemawat <opensource@google.com>
//
// A malloc that uses a per-thread cache to satisfy small malloc requests.
// (The time for malloc/free of a small object drops from 300 ns to 50 ns.)
//
// See doc/tcmalloc.html for a high-level
// description of how this malloc works.
//
// SYNCHRONIZATION
//  1. The thread-specific lists are accessed without acquiring any locks.
//     This is safe because each such list is only accessed by one thread.
//  2. We have a lock per central free-list, and hold it while manipulating
//     the central free list for a particular size.
//  3. The central page allocator is protected by "pageheap_lock".
//  4. The pagemap (which maps from page-number to descriptor),
//     can be read without holding any locks, and written while holding
//     the "pageheap_lock".
//  5. To improve performance, a subset of the information one can get
//     from the pagemap is cached in a data structure, pagemap_cache_,
//     that atomically reads and writes its entries.  This cache can be
//     read and written without locking.
//
//     This multi-threaded access to the pagemap is safe for fairly
//     subtle reasons.  We basically assume that when an object X is
//     allocated by thread A and deallocated by thread B, there must
//     have been appropriate synchronization in the handoff of object
//     X from thread A to thread B.  The same logic applies to pagemap_cache_.
//
// THE PAGEID-TO-SIZECLASS CACHE
// Hot PageID-to-sizeclass mappings are held by pagemap_cache_.  If this cache
// returns 0 for a particular PageID then that means "no information," not that
// the sizeclass is 0.  The cache may have stale information for pages that do
// not hold the beginning of any free()'able object.  Staleness is eliminated
// in Populate() for pages with sizeclass > 0 objects, and in do_malloc() and
// do_memalign() for all other relevant pages.
//
// TODO: Bias reclamation to larger addresses
// TODO: implement mallinfo/mallopt
// TODO: Better testing
//
// 9/28/2003 (new page-level allocator replaces ptmalloc2):
// * malloc/free of small objects goes from ~300 ns to ~50 ns.
// * allocation of a reasonably complicated struct
//   goes from about 1100 ns to about 300 ns.

#include "config.h"
#include "FastMalloc.h"

#include "Assertions.h"
#include <limits>
#if ENABLE(JSC_MULTIPLE_THREADS)
#include <pthread.h>
#endif

// Use a background thread to periodically scavenge memory to release back to the system
// https://bugs.webkit.org/show_bug.cgi?id=27900: don't turn this on for Tiger until we have figured out why it caused a crash.
#if defined(BUILDING_ON_TIGER)
#define USE_BACKGROUND_THREAD_TO_SCAVENGE_MEMORY 0
#else
#define USE_BACKGROUND_THREAD_TO_SCAVENGE_MEMORY 1
#endif

#if defined(__HP_aCC)
// HP'a aCC compiler has broken for scoping
# define for if(0){}else for
#endif

#ifndef NDEBUG
namespace WTF {

#if ENABLE(JSC_MULTIPLE_THREADS)
static pthread_key_t isForbiddenKey;
static pthread_once_t isForbiddenKeyOnce = PTHREAD_ONCE_INIT;
static void initializeIsForbiddenKey()
{
  pthread_key_create(&isForbiddenKey, 0);
}

#if !ASSERT_DISABLED
static bool isForbidden()
{
    pthread_once(&isForbiddenKeyOnce, initializeIsForbiddenKey);
    return !!pthread_getspecific(isForbiddenKey);
}
#endif

void fastMallocForbid()
{
    pthread_once(&isForbiddenKeyOnce, initializeIsForbiddenKey);
    pthread_setspecific(isForbiddenKey, &isForbiddenKey);
}

void fastMallocAllow()
{
    pthread_once(&isForbiddenKeyOnce, initializeIsForbiddenKey);
    pthread_setspecific(isForbiddenKey, 0);
}

#else

static bool staticIsForbidden;
static bool isForbidden()
{
    return staticIsForbidden;
}

void fastMallocForbid()
{
    staticIsForbidden = true;
}

void fastMallocAllow()
{
    staticIsForbidden = false;
}
#endif // ENABLE(JSC_MULTIPLE_THREADS)

} // namespace WTF
#endif // NDEBUG

#include <string.h>

namespace WTF {

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)

namespace Internal {

void fastMallocMatchFailed(void*)
{
    CRASH();
}

} // namespace Internal

#endif

void* fastZeroedMalloc(size_t n) 
{
    void* result = fastMalloc(n);
    memset(result, 0, n);
    return result;
}

char* fastStrDup(const char* src)
{
    int len = strlen(src) + 1;
    char* dup = static_cast<char*>(fastMalloc(len));

    if (dup)
        memcpy(dup, src, len);

    return dup;
}
    
TryMallocReturnValue tryFastZeroedMalloc(size_t n) 
{
    void* result;
    if (!tryFastMalloc(n).getValue(result))
        return 0;
    memset(result, 0, n);
    return result;
}

} // namespace WTF

namespace WTF {

TryMallocReturnValue tryFastMalloc(size_t n) 
{
    ASSERT(!isForbidden());

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
    if (std::numeric_limits<size_t>::max() - sizeof(AllocAlignmentInteger) <= n)  // If overflow would occur...
        return 0;

    void* result = malloc(n + sizeof(AllocAlignmentInteger));
    if (!result)
        return 0;

    *static_cast<AllocAlignmentInteger*>(result) = Internal::AllocTypeMalloc;
    result = static_cast<AllocAlignmentInteger*>(result) + 1;

    return result;
#else
    return malloc(n);
#endif
}

void* fastMalloc(size_t n) 
{
    ASSERT(!isForbidden());

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
    TryMallocReturnValue returnValue = tryFastMalloc(n);
    void* result;
    returnValue.getValue(result);
#else
    void* result = malloc(n);
#endif

    if (!result)
        CRASH();
    return result;
}

TryMallocReturnValue tryFastCalloc(size_t n_elements, size_t element_size)
{
    ASSERT(!isForbidden());

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
    size_t totalBytes = n_elements * element_size;
    if (n_elements > 1 && element_size && (totalBytes / element_size) != n_elements || (std::numeric_limits<size_t>::max() - sizeof(AllocAlignmentInteger) <= totalBytes))
        return 0;

    totalBytes += sizeof(AllocAlignmentInteger);
    void* result = malloc(totalBytes);
    if (!result)
        return 0;

    memset(result, 0, totalBytes);
    *static_cast<AllocAlignmentInteger*>(result) = Internal::AllocTypeMalloc;
    result = static_cast<AllocAlignmentInteger*>(result) + 1;
    return result;
#else
    return calloc(n_elements, element_size);
#endif
}

void* fastCalloc(size_t n_elements, size_t element_size)
{
    ASSERT(!isForbidden());

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
    TryMallocReturnValue returnValue = tryFastCalloc(n_elements, element_size);
    void* result;
    returnValue.getValue(result);
#else
    void* result = calloc(n_elements, element_size);
#endif

    if (!result)
        CRASH();
    return result;
}

void fastFree(void* p)
{
    ASSERT(!isForbidden());

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
    if (!p)
        return;

    AllocAlignmentInteger* header = Internal::fastMallocMatchValidationValue(p);
    if (*header != Internal::AllocTypeMalloc)
        Internal::fastMallocMatchFailed(p);
    free(header);
#else
    free(p);
#endif
}

TryMallocReturnValue tryFastRealloc(void* p, size_t n)
{
    ASSERT(!isForbidden());

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
    if (p) {
        if (std::numeric_limits<size_t>::max() - sizeof(AllocAlignmentInteger) <= n)  // If overflow would occur...
            return 0;
        AllocAlignmentInteger* header = Internal::fastMallocMatchValidationValue(p);
        if (*header != Internal::AllocTypeMalloc)
            Internal::fastMallocMatchFailed(p);
        void* result = realloc(header, n + sizeof(AllocAlignmentInteger));
        if (!result)
            return 0;

        // This should not be needed because the value is already there:
        // *static_cast<AllocAlignmentInteger*>(result) = Internal::AllocTypeMalloc;
        result = static_cast<AllocAlignmentInteger*>(result) + 1;
        return result;
    } else {
        return fastMalloc(n);
    }
#else
    return realloc(p, n);
#endif
}

void* fastRealloc(void* p, size_t n)
{
    ASSERT(!isForbidden());

#if ENABLE(FAST_MALLOC_MATCH_VALIDATION)
    TryMallocReturnValue returnValue = tryFastRealloc(p, n);
    void* result;
    returnValue.getValue(result);
#else
    void* result = realloc(p, n);
#endif

    if (!result)
        CRASH();
    return result;
}

FastMallocStatistics fastMallocStatistics()
{
    FastMallocStatistics statistics = { 0, 0, 0, 0 };
    return statistics;
}

} // namespace WTF

#if OS(DARWIN)
// This symbol is present in the JavaScriptCore exports file even when FastMalloc is disabled.
// It will never be used in this case, so it's type and value are less interesting than its presence.
extern "C" const int jscore_fastmalloc_introspection = 0;
#endif
