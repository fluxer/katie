/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2007, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#include "Platform.h"
#include "JSCell.h"

#include "JSFunction.h"
#include "JSString.h"
#include "JSObject.h"

namespace JSC {

bool JSCell::getUInt32(uint32_t&) const
{
    return false;
}

bool JSCell::getString(ExecState* exec, UString&stringValue) const
{
    if (!isString())
        return false;
    stringValue = static_cast<const JSString*>(this)->value(exec);
    return true;
}

UString JSCell::getString(ExecState* exec) const
{
    return isString() ? static_cast<const JSString*>(this)->value(exec) : UString();
}

JSObject* JSCell::getObject()
{
    return isObject() ? asObject(this) : 0;
}

const JSObject* JSCell::getObject() const
{
    return isObject() ? static_cast<const JSObject*>(this) : 0;
}

CallType JSCell::getCallData(CallData&)
{
    return CallTypeNone;
}

ConstructType JSCell::getConstructData(ConstructData&)
{
    return ConstructTypeNone;
}

bool JSCell::getOwnPropertySlot(ExecState* exec, const Identifier& identifier, PropertySlot& slot)
{
    // This is not a general purpose implementation of getOwnPropertySlot.
    // It should only be called by JSValue::get.
    // It calls getPropertySlot, not getOwnPropertySlot.
    JSObject* object = toObject(exec);
    slot.setBase(object);
    if (!object->getPropertySlot(exec, identifier, slot))
        slot.setUndefined();
    return true;
}

bool JSCell::getOwnPropertySlot(ExecState* exec, unsigned identifier, PropertySlot& slot)
{
    // This is not a general purpose implementation of getOwnPropertySlot.
    // It should only be called by JSValue::get.
    // It calls getPropertySlot, not getOwnPropertySlot.
    JSObject* object = toObject(exec);
    slot.setBase(object);
    if (!object->getPropertySlot(exec, identifier, slot))
        slot.setUndefined();
    return true;
}

void JSCell::put(ExecState* exec, const Identifier& identifier, JSValue value, PutPropertySlot& slot)
{
    toObject(exec)->put(exec, identifier, value, slot);
}

void JSCell::put(ExecState* exec, unsigned identifier, JSValue value)
{
    toObject(exec)->put(exec, identifier, value);
}

bool JSCell::deleteProperty(ExecState* exec, const Identifier& identifier)
{
    return toObject(exec)->deleteProperty(exec, identifier);
}

bool JSCell::deleteProperty(ExecState* exec, unsigned identifier)
{
    return toObject(exec)->deleteProperty(exec, identifier);
}

JSObject* JSCell::toThisObject(ExecState* exec) const
{
    return toObject(exec);
}

UString JSCell::toThisString(ExecState* exec) const
{
    return toThisObject(exec)->toString(exec);
}

JSString* JSCell::toThisJSString(ExecState* exec)
{
    return jsString(exec, toThisString(exec));
}

const ClassInfo* JSCell::classInfo() const
{
    return 0;
}

JSValue JSCell::getJSNumber()
{
    return JSValue();
}

bool JSCell::isGetterSetter() const
{
    return false;
}

JSValue JSCell::toPrimitive(ExecState*, PreferredPrimitiveType) const
{
    Q_UNREACHABLE();
    return JSValue();
}

bool JSCell::getPrimitiveNumber(ExecState*, double&, JSValue&)
{
    Q_UNREACHABLE();
    return false;
}

bool JSCell::toBoolean(ExecState*) const
{
    Q_UNREACHABLE();
    return false;
}

double JSCell::toNumber(ExecState*) const
{
    Q_UNREACHABLE();
    return 0;
}

UString JSCell::toString(ExecState*) const
{
    Q_UNREACHABLE();
    return UString();
}

JSObject* JSCell::toObject(ExecState*) const
{
    Q_UNREACHABLE();
    return 0;
}

} // namespace JSC
