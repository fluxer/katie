/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef JSZombie_h
#define JSZombie_h

#include "JSCell.h"

#if ENABLE(JSC_ZOMBIES)
namespace JSC {

class JSZombie : public JSCell {
public:
    JSZombie(const ClassInfo* oldInfo, Structure* structure)
        : JSCell(structure)
        , m_oldInfo(oldInfo)
    {
    }
    virtual bool isZombie() const { return true; }
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static Structure* leakedZombieStructure();

    virtual bool isGetterSetter() const { Q_UNREACHABLE(); return false; }
    virtual bool isAPIValueWrapper() const { Q_UNREACHABLE(); return false; }
    virtual bool isPropertyNameIterator() const { Q_UNREACHABLE(); return false; }
    virtual CallType getCallData(CallData&) { Q_UNREACHABLE(); return CallTypeNone; }
    virtual ConstructType getConstructData(ConstructData&) { Q_UNREACHABLE(); return ConstructTypeNone; }
    virtual bool getUInt32(uint32_t&) const { Q_UNREACHABLE(); return false; }
    virtual JSValue toPrimitive(ExecState*, PreferredPrimitiveType) const { Q_UNREACHABLE(); return jsNull(); }
    virtual bool getPrimitiveNumber(ExecState*, double&, JSValue&) { Q_UNREACHABLE(); return false; }
    virtual bool toBoolean(ExecState*) const { Q_UNREACHABLE(); return false; }
    virtual double toNumber(ExecState*) const { Q_UNREACHABLE(); return 0.0; }
    virtual UString toString(ExecState*) const { Q_UNREACHABLE(); return ""; }
    virtual JSObject* toObject(ExecState*) const { Q_UNREACHABLE(); return 0; }
    virtual void markChildren(MarkStack&) { Q_UNREACHABLE(); }
    virtual void put(ExecState*, const Identifier&, JSValue, PutPropertySlot&) { Q_UNREACHABLE(); }
    virtual void put(ExecState*, unsigned, JSValue) { Q_UNREACHABLE(); }
    virtual bool deleteProperty(ExecState*, const Identifier&) { Q_UNREACHABLE(); return false; }
    virtual bool deleteProperty(ExecState*, unsigned) { Q_UNREACHABLE(); return false; }
    virtual JSObject* toThisObject(ExecState*) const { Q_UNREACHABLE(); return 0; }
    virtual UString toThisString(ExecState*) const { Q_UNREACHABLE(); return ""; }
    virtual JSString* toThisJSString(ExecState*) { Q_UNREACHABLE(); return 0; }
    virtual JSValue getJSNumber() { Q_UNREACHABLE(); return jsNull(); }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&) { Q_UNREACHABLE(); return false; }
    virtual bool getOwnPropertySlot(ExecState*, unsigned, PropertySlot&) { Q_UNREACHABLE(); return false; }
    
    static const ClassInfo s_info;
private:
    const ClassInfo* m_oldInfo;
};

}

#endif // ENABLE(JSC_ZOMBIES)

#endif // JSZombie_h
