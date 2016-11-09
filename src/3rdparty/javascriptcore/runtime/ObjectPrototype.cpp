/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2008 Apple Inc. All rights reserved.
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
#include "ObjectPrototype.h"

#include "Error.h"
#include "JSFunction.h"
#include "JSString.h"
#include "PrototypeFunction.h"

namespace JSC {

ASSERT_CLASS_FITS_IN_CELL(ObjectPrototype);

static JSValue QT_FASTCALL objectProtoFuncValueOf(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncHasOwnProperty(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncIsPrototypeOf(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncDefineGetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncDefineSetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncLookupGetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncLookupSetter(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncPropertyIsEnumerable(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL objectProtoFuncToLocaleString(ExecState*, JSObject*, JSValue, const ArgList&);

ObjectPrototype::ObjectPrototype(ExecState* exec, NonNullPassRefPtr<Structure> stucture, Structure* prototypeFunctionStructure)
    : JSObject(stucture)
    , m_hasNoPropertiesWithUInt32Names(true)
{
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 0, exec->propertyNames().toString, objectProtoFuncToString), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 0, exec->propertyNames().toLocaleString, objectProtoFuncToLocaleString), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 0, exec->propertyNames().valueOf, objectProtoFuncValueOf), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().hasOwnProperty, objectProtoFuncHasOwnProperty), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().propertyIsEnumerable, objectProtoFuncPropertyIsEnumerable), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().isPrototypeOf, objectProtoFuncIsPrototypeOf), DontEnum);

    // Mozilla extensions
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 2, exec->propertyNames().__defineGetter__, objectProtoFuncDefineGetter), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 2, exec->propertyNames().__defineSetter__, objectProtoFuncDefineSetter), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().__lookupGetter__, objectProtoFuncLookupGetter), DontEnum);
    putDirectFunctionWithoutTransition(exec, new (exec) NativeFunctionWrapper(exec, prototypeFunctionStructure, 1, exec->propertyNames().__lookupSetter__, objectProtoFuncLookupSetter), DontEnum);
}

void ObjectPrototype::put(ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    JSObject::put(exec, propertyName, value, slot);

    if (m_hasNoPropertiesWithUInt32Names) {
        bool isUInt32;
        propertyName.toStrictUInt32(&isUInt32);
        m_hasNoPropertiesWithUInt32Names = !isUInt32;
    }
}

bool ObjectPrototype::getOwnPropertySlot(ExecState* exec, unsigned propertyName, PropertySlot& slot)
{
    if (m_hasNoPropertiesWithUInt32Names)
        return false;
    return JSObject::getOwnPropertySlot(exec, propertyName, slot);
}

// ------------------------------ Functions --------------------------------

// ECMA 15.2.4.2, 15.2.4.4, 15.2.4.5, 15.2.4.7

JSValue QT_FASTCALL objectProtoFuncValueOf(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    return thisValue.toThisObject(exec);
}

JSValue QT_FASTCALL objectProtoFuncHasOwnProperty(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return jsBoolean(thisValue.toThisObject(exec)->hasOwnProperty(exec, Identifier(exec, args.at(0).toString(exec))));
}

JSValue QT_FASTCALL objectProtoFuncIsPrototypeOf(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    JSObject* thisObj = thisValue.toThisObject(exec);

    if (!args.at(0).isObject())
        return jsBoolean(false);

    JSValue v = asObject(args.at(0))->prototype();

    while (true) {
        if (!v.isObject())
            return jsBoolean(false);
        if (v == thisObj)
            return jsBoolean(true);
        v = asObject(v)->prototype();
    }
}

JSValue QT_FASTCALL objectProtoFuncDefineGetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    CallData callData;
    if (args.at(1).getCallData(callData) == CallTypeNone)
        return throwError(exec, SyntaxError, "invalid getter usage");
    thisValue.toThisObject(exec)->defineGetter(exec, Identifier(exec, args.at(0).toString(exec)), asObject(args.at(1)));
    return jsUndefined();
}

JSValue QT_FASTCALL objectProtoFuncDefineSetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    CallData callData;
    if (args.at(1).getCallData(callData) == CallTypeNone)
        return throwError(exec, SyntaxError, "invalid setter usage");
    thisValue.toThisObject(exec)->defineSetter(exec, Identifier(exec, args.at(0).toString(exec)), asObject(args.at(1)));
    return jsUndefined();
}

JSValue QT_FASTCALL objectProtoFuncLookupGetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return thisValue.toThisObject(exec)->lookupGetter(exec, Identifier(exec, args.at(0).toString(exec)));
}

JSValue QT_FASTCALL objectProtoFuncLookupSetter(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return thisValue.toThisObject(exec)->lookupSetter(exec, Identifier(exec, args.at(0).toString(exec)));
}

JSValue QT_FASTCALL objectProtoFuncPropertyIsEnumerable(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    return jsBoolean(thisValue.toThisObject(exec)->propertyIsEnumerable(exec, Identifier(exec, args.at(0).toString(exec))));
}

JSValue QT_FASTCALL objectProtoFuncToLocaleString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    return thisValue.toThisJSString(exec);
}

JSValue QT_FASTCALL objectProtoFuncToString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    return jsNontrivialString(exec, makeString("[object ", thisValue.toThisObject(exec)->className(), "]"));
}

} // namespace JSC
