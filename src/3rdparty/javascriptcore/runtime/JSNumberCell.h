/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2004, 2005, 2007, 2008 Apple Inc. All rights reserved.
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

#ifndef JSNumberCell_h
#define JSNumberCell_h

#include "CallFrame.h"
#include "JSCell.h"
#include "JSImmediate.h"
#include "Collector.h"
#include "UString.h"
#include <stddef.h> // for size_t

namespace JSC {

    const double NaN = NAN;
    const double Inf = INFINITY;

#if USE(JSVALUE64)
    ALWAYS_INLINE JSValue::JSValue(EncodeAsDoubleTag, ExecState*, double d)
    {
        *this = JSImmediate::fromNumberOutsideIntegerRange(d);
    }

    inline JSValue::JSValue(ExecState*, double d)
    {
        JSValue v = JSImmediate::from(d);
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, int i)
    {
        JSValue v = JSImmediate::from(i);
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, unsigned i)
    {
        JSValue v = JSImmediate::from(i);
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, long i)
    {
        JSValue v = JSImmediate::from(i);
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, unsigned long i)
    {
        JSValue v = JSImmediate::from(i);
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, long long i)
    {
        JSValue v = JSImmediate::from(static_cast<double>(i));
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(ExecState*, unsigned long long i)
    {
        JSValue v = JSImmediate::from(static_cast<double>(i));
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(JSGlobalData*, double d)
    {
        JSValue v = JSImmediate::from(d);
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(JSGlobalData*, int i)
    {
        JSValue v = JSImmediate::from(i);
        Q_ASSERT(v);
        *this = v;
    }

    inline JSValue::JSValue(JSGlobalData*, unsigned i)
    {
        JSValue v = JSImmediate::from(i);
        Q_ASSERT(v);
        *this = v;
    }

    inline bool JSValue::isDouble() const
    {
        return JSImmediate::isDouble(asValue());
    }

    inline double JSValue::asDouble() const
    {
        return JSImmediate::doubleValue(asValue());
    }

    inline bool JSValue::isNumber() const
    {
        return JSImmediate::isNumber(asValue());
    }

    inline double JSValue::uncheckedGetNumber() const
    {
        Q_ASSERT(isNumber());
        return JSImmediate::toDouble(asValue());
    }

    inline JSValue::JSValue(ExecState*, char i)
    {
        Q_ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue::JSValue(ExecState*, unsigned char i)
    {
        Q_ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue::JSValue(ExecState*, short i)
    {
        Q_ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue::JSValue(ExecState*, unsigned short i)
    {
        Q_ASSERT(JSImmediate::from(i));
        *this = JSImmediate::from(i);
    }

    inline JSValue jsNaN(ExecState* exec)
    {
        return jsNumber(exec, NaN);
    }

    inline JSValue jsNaN(JSGlobalData* globalData)
    {
        return jsNumber(globalData, NaN);
    }

    // --- JSValue inlines ----------------------------

    ALWAYS_INLINE JSValue JSValue::toJSNumber(ExecState* exec) const
    {
        return isNumber() ? asValue() : jsNumber(exec, this->toNumber(exec));
    }

    inline bool JSValue::getNumber(double &result) const
    {
        if (isInt32())
            result = asInt32();
        else if (Q_LIKELY(isDouble()))
            result = asDouble();
        else {
            Q_ASSERT(!isNumber());
            return false;
        }
        return true;
    }
#endif // USE(JSVALUE64)

} // namespace JSC

#endif // JSNumberCell_h
