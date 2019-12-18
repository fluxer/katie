/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Platform.h"
#include "JSGlobalData.h"

#include "ArgList.h"
#include "Collector.h"
#include "CommonIdentifiers.h"
#include "FunctionConstructor.h"
#include "GetterSetter.h"
#include "Interpreter.h"
#include "JSActivation.h"
#include "JSAPIValueWrapper.h"
#include "JSArray.h"
#include "JSByteArray.h"
#include "JSClassRef.h"
#include "JSFunction.h"
#include "JSNotAnObject.h"
#include "JSPropertyNameIterator.h"
#include "JSStaticScopeObject.h"
#include "Lexer.h"
#include "Lookup.h"
#include "Nodes.h"
#include "Parser.h"

using namespace WTF;

namespace JSC {

extern const HashTable arrayTable;
extern const HashTable jsonTable;
extern const HashTable dateTable;
extern const HashTable mathTable;
extern const HashTable numberTable;
extern const HashTable regExpTable;
extern const HashTable regExpConstructorTable;
extern const HashTable stringTable;

void* JSGlobalData::jsArrayVPtr;
void* JSGlobalData::jsByteArrayVPtr;
void* JSGlobalData::jsStringVPtr;
void* JSGlobalData::jsFunctionVPtr;

void JSGlobalData::storeVPtrs()
{
    CollectorCell cell;
    void* storage = &cell;

    COMPILE_ASSERT(sizeof(JSArray) <= sizeof(CollectorCell), sizeof_JSArray_must_be_less_than_CollectorCell);
    JSCell* jsArray = new (storage) JSArray(JSArray::createStructure(jsNull()));
    JSGlobalData::jsArrayVPtr = jsArray->vptr();
    jsArray->~JSCell();

    COMPILE_ASSERT(sizeof(JSByteArray) <= sizeof(CollectorCell), sizeof_JSByteArray_must_be_less_than_CollectorCell);
    JSCell* jsByteArray = new (storage) JSByteArray(JSByteArray::VPtrStealingHack);
    JSGlobalData::jsByteArrayVPtr = jsByteArray->vptr();
    jsByteArray->~JSCell();

    COMPILE_ASSERT(sizeof(JSString) <= sizeof(CollectorCell), sizeof_JSString_must_be_less_than_CollectorCell);
    JSCell* jsString = new (storage) JSString(JSString::VPtrStealingHack);
    JSGlobalData::jsStringVPtr = jsString->vptr();
    jsString->~JSCell();

    COMPILE_ASSERT(sizeof(JSFunction) <= sizeof(CollectorCell), sizeof_JSFunction_must_be_less_than_CollectorCell);
    JSCell* jsFunction = new (storage) JSFunction(JSFunction::createStructure(jsNull()));
    JSGlobalData::jsFunctionVPtr = jsFunction->vptr();
    jsFunction->~JSCell();
}

JSGlobalData::JSGlobalData(bool isShared)
    : isSharedInstance(isShared)
    , clientData(0)
    , arrayTable(fastNew<HashTable>(JSC::arrayTable))
    , dateTable(fastNew<HashTable>(JSC::dateTable))
    , jsonTable(fastNew<HashTable>(JSC::jsonTable))
    , mathTable(fastNew<HashTable>(JSC::mathTable))
    , numberTable(fastNew<HashTable>(JSC::numberTable))
    , regExpTable(fastNew<HashTable>(JSC::regExpTable))
    , regExpConstructorTable(fastNew<HashTable>(JSC::regExpConstructorTable))
    , stringTable(fastNew<HashTable>(JSC::stringTable))
    , activationStructure(JSActivation::createStructure(jsNull()))
    , interruptedExecutionErrorStructure(JSObject::createStructure(jsNull()))
    , staticScopeStructure(JSStaticScopeObject::createStructure(jsNull()))
    , stringStructure(JSString::createStructure(jsNull()))
    , notAnObjectErrorStubStructure(JSNotAnObjectErrorStub::createStructure(jsNull()))
    , notAnObjectStructure(JSNotAnObject::createStructure(jsNull()))
    , propertyNameIteratorStructure(JSPropertyNameIterator::createStructure(jsNull()))
    , getterSetterStructure(GetterSetter::createStructure(jsNull()))
    , apiWrapperStructure(JSAPIValueWrapper::createStructure(jsNull()))
    , dummyMarkableCellStructure(JSCell::createDummyStructure())
#if USE(JSVALUE32)
    , numberStructure(JSNumberCell::createStructure(jsNull()))
#endif
    , identifierTable(createIdentifierTable())
    , propertyNames(new CommonIdentifiers(this))
    , emptyList(new MarkedArgumentBuffer)
    , lexer(new Lexer(this))
    , parser(new Parser)
    , interpreter(new Interpreter)
    , timeoutChecker(new TimeoutChecker)
    , heap(this)
    , initializingLazyNumericCompareFunction(false)
    , head(0)
    , dynamicGlobalObject(0)
    , functionCodeBlockBeingReparsed(0)
    , firstStringifierToMark(0)
    , markStack(jsArrayVPtr)
{
}

JSGlobalData::~JSGlobalData()
{
    // By the time this is destroyed, heap.destroy() must already have been called.

    delete interpreter;
#ifndef QT_NO_DEBUG
    // Zeroing out to make the behavior more predictable when someone attempts to use a deleted instance.
    interpreter = 0;
#endif

    arrayTable->deleteTable();
    dateTable->deleteTable();
    jsonTable->deleteTable();
    mathTable->deleteTable();
    numberTable->deleteTable();
    regExpTable->deleteTable();
    regExpConstructorTable->deleteTable();
    stringTable->deleteTable();

    fastDelete(const_cast<HashTable*>(arrayTable));
    fastDelete(const_cast<HashTable*>(dateTable));
    fastDelete(const_cast<HashTable*>(jsonTable));
    fastDelete(const_cast<HashTable*>(mathTable));
    fastDelete(const_cast<HashTable*>(numberTable));
    fastDelete(const_cast<HashTable*>(regExpTable));
    fastDelete(const_cast<HashTable*>(regExpConstructorTable));
    fastDelete(const_cast<HashTable*>(stringTable));

    delete parser;
    delete lexer;
    delete timeoutChecker;

    deleteAllValues(opaqueJSClassData);

    delete emptyList;

    delete propertyNames;
    deleteIdentifierTable(identifierTable);

    delete clientData;
}

PassRefPtr<JSGlobalData> JSGlobalData::createNonDefault()
{
    return adoptRef(new JSGlobalData(false));
}

PassRefPtr<JSGlobalData> JSGlobalData::create()
{
    JSGlobalData* globalData = new JSGlobalData(false);
    setDefaultIdentifierTable(globalData->identifierTable);
    setCurrentIdentifierTable(globalData->identifierTable);
    return adoptRef(globalData);
}

bool JSGlobalData::sharedInstanceExists()
{
    return sharedInstanceInternal();
}

JSGlobalData& JSGlobalData::sharedInstance()
{
    JSGlobalData*& instance = sharedInstanceInternal();
    if (!instance) {
        instance = new JSGlobalData(true);
    }
    return *instance;
}

JSGlobalData*& JSGlobalData::sharedInstanceInternal()
{
    static JSGlobalData* sharedInstance;
    return sharedInstance;
}

// FIXME: We can also detect forms like v1 < v2 ? -1 : 0, reverse comparison, etc.
const Vector<Instruction>& JSGlobalData::numericCompareFunction(ExecState* exec)
{
    if (!lazyNumericCompareFunction.size() && !initializingLazyNumericCompareFunction) {
        initializingLazyNumericCompareFunction = true;
        RefPtr<FunctionExecutable> function = FunctionExecutable::fromGlobalCode(Identifier(exec, "numericCompare"), exec, 0, makeSource(UString("(function (v1, v2) { return v1 - v2; })")), 0, 0);
        lazyNumericCompareFunction = function->bytecode(exec, exec->scopeChain()).instructions();
        initializingLazyNumericCompareFunction = false;
    }

    return lazyNumericCompareFunction;
}

JSGlobalData::ClientData::~ClientData()
{
}

void JSGlobalData::resetDateCache()
{
    localTimeOffsetCache.reset();
    cachedDateString = UString();
    dateInstanceCache.reset();
}

void JSGlobalData::startSampling()
{
    interpreter->startSampling();
}

void JSGlobalData::stopSampling()
{
    interpreter->stopSampling();
}

void JSGlobalData::dumpSampleData(ExecState* exec)
{
    interpreter->dumpSampleData(exec);
}

} // namespace JSC
