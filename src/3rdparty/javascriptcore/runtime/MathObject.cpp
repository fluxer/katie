/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2007, 2008 Apple Inc. All Rights Reserved.
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
#include "MathObject.h"

#include "ObjectPrototype.h"
#include "Operations.h"
#include <time.h>
#include <wtf/Assertions.h>

namespace JSC {

ASSERT_CLASS_FITS_IN_CELL(MathObject);

static JSValue QT_FASTCALL mathProtoFuncAbs(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncACos(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncASin(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncATan(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncATan2(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncCeil(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncCos(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncExp(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncFloor(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncLog(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncMax(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncMin(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncPow(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncRandom(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncRound(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncSin(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncSqrt(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL mathProtoFuncTan(ExecState*, JSObject*, JSValue, const ArgList&);

}

#include "MathObject.lut.h"

namespace JSC {

static const double piDouble = M_PI;

// ------------------------------ MathObject --------------------------------

const ClassInfo MathObject::info = { "Math", 0, ExecState::mathTable };

/* Source for MathObject.lut.h
@begin mathTable
  abs           mathProtoFuncAbs               DontEnum|Function 1
  acos          mathProtoFuncACos              DontEnum|Function 1
  asin          mathProtoFuncASin              DontEnum|Function 1
  atan          mathProtoFuncATan              DontEnum|Function 1
  atan2         mathProtoFuncATan2             DontEnum|Function 2
  ceil          mathProtoFuncCeil              DontEnum|Function 1
  cos           mathProtoFuncCos               DontEnum|Function 1
  exp           mathProtoFuncExp               DontEnum|Function 1
  floor         mathProtoFuncFloor             DontEnum|Function 1
  log           mathProtoFuncLog               DontEnum|Function 1
  max           mathProtoFuncMax               DontEnum|Function 2
  min           mathProtoFuncMin               DontEnum|Function 2
  pow           mathProtoFuncPow               DontEnum|Function 2
  random        mathProtoFuncRandom            DontEnum|Function 0 
  round         mathProtoFuncRound             DontEnum|Function 1
  sin           mathProtoFuncSin               DontEnum|Function 1
  sqrt          mathProtoFuncSqrt              DontEnum|Function 1
  tan           mathProtoFuncTan               DontEnum|Function 1
@end
*/

MathObject::MathObject(ExecState* exec, NonNullPassRefPtr<Structure> structure)
    : JSObject(structure)
{
    putDirectWithoutTransition(Identifier(exec, "E"), jsNumber(exec, exp(1.0)), DontDelete | DontEnum | ReadOnly);
    putDirectWithoutTransition(Identifier(exec, "LN2"), jsNumber(exec, log(2.0)), DontDelete | DontEnum | ReadOnly);
    putDirectWithoutTransition(Identifier(exec, "LN10"), jsNumber(exec, log(10.0)), DontDelete | DontEnum | ReadOnly);
    putDirectWithoutTransition(Identifier(exec, "LOG2E"), jsNumber(exec, 1.0 / log(2.0)), DontDelete | DontEnum | ReadOnly);
    putDirectWithoutTransition(Identifier(exec, "LOG10E"), jsNumber(exec, 1.0 / log(10.0)), DontDelete | DontEnum | ReadOnly);
    putDirectWithoutTransition(Identifier(exec, "PI"), jsNumber(exec, piDouble), DontDelete | DontEnum | ReadOnly);
    putDirectWithoutTransition(Identifier(exec, "SQRT1_2"), jsNumber(exec, sqrt(0.5)), DontDelete | DontEnum | ReadOnly);
    putDirectWithoutTransition(Identifier(exec, "SQRT2"), jsNumber(exec, sqrt(2.0)), DontDelete | DontEnum | ReadOnly);
}

// ECMA 15.8

bool MathObject::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot &slot)
{
    return getStaticFunctionSlot<JSObject>(exec, ExecState::mathTable(exec), this, propertyName, slot);
}

bool MathObject::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticFunctionDescriptor<JSObject>(exec, ExecState::mathTable(exec), this, propertyName, descriptor);
}

// ------------------------------ Functions --------------------------------

JSValue QT_FASTCALL mathProtoFuncAbs(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsNumber(exec, fabs(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncACos(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, acos(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncASin(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, asin(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncATan(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, atan(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncATan2(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, atan2(args.at(0).toNumber(exec), args.at(1).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncCeil(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsNumber(exec, ceil(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncCos(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, cos(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncExp(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, exp(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncFloor(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsNumber(exec, floor(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncLog(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, log(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncMax(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    unsigned argsCount = args.size();
    double result = -Inf;
    for (unsigned k = 0; k < argsCount; ++k) {
        double val = args.at(k).toNumber(exec);
        if (std::isnan(val)) {
            result = NaN;
            break;
        }
        if (val > result || (val == 0 && result == 0 && !std::signbit(val)))
            result = val;
    }
    return jsNumber(exec, result);
}

JSValue QT_FASTCALL mathProtoFuncMin(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    unsigned argsCount = args.size();
    double result = +Inf;
    for (unsigned k = 0; k < argsCount; ++k) {
        double val = args.at(k).toNumber(exec);
        if (std::isnan(val)) {
            result = NaN;
            break;
        }
        if (val < result || (val == 0 && result == 0 && std::signbit(val)))
            result = val;
    }
    return jsNumber(exec, result);
}

JSValue QT_FASTCALL mathProtoFuncPow(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    // ECMA 15.8.2.1.13

    double arg = args.at(0).toNumber(exec);
    double arg2 = args.at(1).toNumber(exec);

    if (std::isnan(arg2))
        return jsNaN(exec);
    if (std::isinf(arg2) && fabs(arg) == 1)
        return jsNaN(exec);
    return jsNumber(exec, pow(arg, arg2));
}

JSValue QT_FASTCALL mathProtoFuncRandom(ExecState* exec, JSObject*, JSValue, const ArgList&)
{
    return jsDoubleNumber(exec, exec->lexicalGlobalObject()->weakRandomNumber());
}

JSValue QT_FASTCALL mathProtoFuncRound(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    double arg = args.at(0).toNumber(exec);
    if (std::signbit(arg) && arg >= -0.5)
         return jsNumber(exec, -0.0);
    double integer = ceil(arg);
    return jsNumber(exec, integer - (integer - arg > 0.5));
}

JSValue QT_FASTCALL mathProtoFuncSin(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, sin(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncSqrt(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, sqrt(args.at(0).toNumber(exec)));
}

JSValue QT_FASTCALL mathProtoFuncTan(ExecState* exec, JSObject*, JSValue, const ArgList& args)
{
    return jsDoubleNumber(exec, tan(args.at(0).toNumber(exec)));
}

} // namespace JSC
