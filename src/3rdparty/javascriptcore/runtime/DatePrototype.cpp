/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2008, 2009 Torch Mobile, Inc. All rights reserved.
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *  USA
 *
 */

#include "Platform.h"
#include "DatePrototype.h"
#include "DateConversion.h"
#include "Error.h"
#include "JSString.h"
#include "ObjectPrototype.h"
#include "DateInstance.h"
#include <wtf/Assertions.h>
#include <wtf/DateMath.h>

#include <limits.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <langinfo.h>

using namespace WTF;

namespace JSC {

ASSERT_CLASS_FITS_IN_CELL(DatePrototype);

static JSValue QT_FASTCALL dateProtoFuncGetDate(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetDay(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetFullYear(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetHours(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetMilliSeconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetMinutes(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetMonth(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetSeconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetTime(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetTimezoneOffset(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCDate(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCDay(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCFullYear(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCHours(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCMilliseconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCMinutes(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCMonth(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetUTCSeconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncGetYear(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetDate(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetFullYear(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetHours(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetMilliSeconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetMinutes(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetMonth(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetSeconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetTime(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetUTCDate(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetUTCFullYear(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetUTCHours(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetUTCMilliseconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetUTCMinutes(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetUTCMonth(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetUTCSeconds(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncSetYear(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToDateString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToGMTString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToLocaleDateString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToLocaleString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToLocaleTimeString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToTimeString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToUTCString(ExecState*, JSObject*, JSValue, const ArgList&);
static JSValue QT_FASTCALL dateProtoFuncToISOString(ExecState*, JSObject*, JSValue, const ArgList&);

static JSValue QT_FASTCALL dateProtoFuncToJSON(ExecState*, JSObject*, JSValue, const ArgList&);

}

#include "DatePrototype.lut.h"

namespace JSC {

enum LocaleDateTimeFormat { LocaleDateAndTime, LocaleDate, LocaleTime };

static JSCell* formatLocaleDate(ExecState* exec, const GregorianDateTime& gdt, LocaleDateTimeFormat format)
{
    static const nl_item formats[] = { D_T_FMT, D_FMT, T_FMT };
 
    // Offset year if needed
    struct tm localTM = gdt;
    int year = gdt.year + 1900;
    bool yearNeedsOffset = year < 1900 || year > 2038;
    if (yearNeedsOffset)
        localTM.tm_year = equivalentYearForDST(year) - 1900;
 
    // We do not allow strftime to generate dates with 2-digits years,
    // both to avoid ambiguity, and a crash in strncpy, for years that
    // need offset.
    char* formatString = ::strdup(::nl_langinfo(formats[format]));
    char* yPos = strchr(formatString, 'y');
    if (yPos)
        *yPos = 'Y';

    // Do the formatting
    const int bufsize = 128;
    char timebuffer[bufsize];

    size_t ret = ::strftime(timebuffer, bufsize, formatString, &localTM);
    ::free(formatString);
 
    if (ret == 0)
        return jsEmptyString(exec);
 
    // Copy original into the buffer
    if (yearNeedsOffset && format != LocaleTime) {
        static const int yearLen = 5;   // FIXME will be a problem in the year 10,000
        char yearString[yearLen];
 
        snprintf(yearString, yearLen, "%d", localTM.tm_year + 1900);
        char* yearLocation = strstr(timebuffer, yearString);
        snprintf(yearString, yearLen, "%d", year);
 
        strncpy(yearLocation, yearString, yearLen - 1);
    }
 
    return jsNontrivialString(exec, timebuffer);
}

static JSCell* formatLocaleDate(ExecState* exec, DateInstance* dateObject, double, LocaleDateTimeFormat format, const ArgList&)
{
    const GregorianDateTime* gregorianDateTime = dateObject->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNontrivialString(exec, "Invalid Date");
    return formatLocaleDate(exec, *gregorianDateTime, format);
}

// Converts a list of arguments sent to a Date member function into milliseconds, updating
// ms (representing milliseconds) and t (representing the rest of the date structure) appropriately.
//
// Format of member function: f([hour,] [min,] [sec,] [ms])
static bool fillStructuresUsingTimeArgs(ExecState* exec, const ArgList& args, int maxArgs, double* ms, GregorianDateTime* t)
{
    double milliseconds = 0;
    bool ok = true;
    int idx = 0;
    int numArgs = args.size();
    
    // JS allows extra trailing arguments -- ignore them
    if (numArgs > maxArgs)
        numArgs = maxArgs;

    // hours
    if (maxArgs >= 4 && idx < numArgs) {
        t->hour = 0;
        milliseconds += args.at(idx++).toInt32(exec, ok) * msPerHour;
    }

    // minutes
    if (maxArgs >= 3 && idx < numArgs && ok) {
        t->minute = 0;
        milliseconds += args.at(idx++).toInt32(exec, ok) * msPerMinute;
    }
    
    // seconds
    if (maxArgs >= 2 && idx < numArgs && ok) {
        t->second = 0;
        milliseconds += args.at(idx++).toInt32(exec, ok) * msPerSecond;
    }
    
    if (!ok)
        return false;
        
    // milliseconds
    if (idx < numArgs) {
        double millis = args.at(idx).toNumber(exec);
        ok = std::isfinite(millis);
        milliseconds += millis;
    } else
        milliseconds += *ms;
    
    *ms = milliseconds;
    return ok;
}

// Converts a list of arguments sent to a Date member function into years, months, and milliseconds, updating
// ms (representing milliseconds) and t (representing the rest of the date structure) appropriately.
//
// Format of member function: f([years,] [months,] [days])
static bool fillStructuresUsingDateArgs(ExecState *exec, const ArgList& args, int maxArgs, double *ms, GregorianDateTime *t)
{
    int idx = 0;
    bool ok = true;
    int numArgs = args.size();
  
    // JS allows extra trailing arguments -- ignore them
    if (numArgs > maxArgs)
        numArgs = maxArgs;
  
    // years
    if (maxArgs >= 3 && idx < numArgs)
        t->year = args.at(idx++).toInt32(exec, ok) - 1900;
    
    // months
    if (maxArgs >= 2 && idx < numArgs && ok)   
        t->month = args.at(idx++).toInt32(exec, ok);
    
    // days
    if (idx < numArgs && ok) {   
        t->monthDay = 0;
        *ms += args.at(idx).toInt32(exec, ok) * msPerDay;
    }
    
    return ok;
}

const ClassInfo DatePrototype::info = { "Date", &DateInstance::info, ExecState::dateTable };

/* Source for DatePrototype.lut.h
@begin dateTable
  toString              dateProtoFuncToString                DontEnum|Function       0
  toISOString           dateProtoFuncToISOString             DontEnum|Function       0
  toUTCString           dateProtoFuncToUTCString             DontEnum|Function       0
  toDateString          dateProtoFuncToDateString            DontEnum|Function       0
  toTimeString          dateProtoFuncToTimeString            DontEnum|Function       0
  toLocaleString        dateProtoFuncToLocaleString          DontEnum|Function       0
  toLocaleDateString    dateProtoFuncToLocaleDateString      DontEnum|Function       0
  toLocaleTimeString    dateProtoFuncToLocaleTimeString      DontEnum|Function       0
  valueOf               dateProtoFuncGetTime                 DontEnum|Function       0
  getTime               dateProtoFuncGetTime                 DontEnum|Function       0
  getFullYear           dateProtoFuncGetFullYear             DontEnum|Function       0
  getUTCFullYear        dateProtoFuncGetUTCFullYear          DontEnum|Function       0
  toGMTString           dateProtoFuncToGMTString             DontEnum|Function       0
  getMonth              dateProtoFuncGetMonth                DontEnum|Function       0
  getUTCMonth           dateProtoFuncGetUTCMonth             DontEnum|Function       0
  getDate               dateProtoFuncGetDate                 DontEnum|Function       0
  getUTCDate            dateProtoFuncGetUTCDate              DontEnum|Function       0
  getDay                dateProtoFuncGetDay                  DontEnum|Function       0
  getUTCDay             dateProtoFuncGetUTCDay               DontEnum|Function       0
  getHours              dateProtoFuncGetHours                DontEnum|Function       0
  getUTCHours           dateProtoFuncGetUTCHours             DontEnum|Function       0
  getMinutes            dateProtoFuncGetMinutes              DontEnum|Function       0
  getUTCMinutes         dateProtoFuncGetUTCMinutes           DontEnum|Function       0
  getSeconds            dateProtoFuncGetSeconds              DontEnum|Function       0
  getUTCSeconds         dateProtoFuncGetUTCSeconds           DontEnum|Function       0
  getMilliseconds       dateProtoFuncGetMilliSeconds         DontEnum|Function       0
  getUTCMilliseconds    dateProtoFuncGetUTCMilliseconds      DontEnum|Function       0
  getTimezoneOffset     dateProtoFuncGetTimezoneOffset       DontEnum|Function       0
  setTime               dateProtoFuncSetTime                 DontEnum|Function       1
  setMilliseconds       dateProtoFuncSetMilliSeconds         DontEnum|Function       1
  setUTCMilliseconds    dateProtoFuncSetUTCMilliseconds      DontEnum|Function       1
  setSeconds            dateProtoFuncSetSeconds              DontEnum|Function       2
  setUTCSeconds         dateProtoFuncSetUTCSeconds           DontEnum|Function       2
  setMinutes            dateProtoFuncSetMinutes              DontEnum|Function       3
  setUTCMinutes         dateProtoFuncSetUTCMinutes           DontEnum|Function       3
  setHours              dateProtoFuncSetHours                DontEnum|Function       4
  setUTCHours           dateProtoFuncSetUTCHours             DontEnum|Function       4
  setDate               dateProtoFuncSetDate                 DontEnum|Function       1
  setUTCDate            dateProtoFuncSetUTCDate              DontEnum|Function       1
  setMonth              dateProtoFuncSetMonth                DontEnum|Function       2
  setUTCMonth           dateProtoFuncSetUTCMonth             DontEnum|Function       2
  setFullYear           dateProtoFuncSetFullYear             DontEnum|Function       3
  setUTCFullYear        dateProtoFuncSetUTCFullYear          DontEnum|Function       3
  setYear               dateProtoFuncSetYear                 DontEnum|Function       1
  getYear               dateProtoFuncGetYear                 DontEnum|Function       0
  toJSON                dateProtoFuncToJSON                  DontEnum|Function       0
@end
*/

// ECMA 15.9.4

DatePrototype::DatePrototype(ExecState* exec, NonNullPassRefPtr<Structure> structure)
    : DateInstance(exec, structure)
{
    // The constructor will be added later, after DateConstructor has been built.
}

bool DatePrototype::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticFunctionSlot<JSObject>(exec, ExecState::dateTable(exec), this, propertyName, slot);
}


bool DatePrototype::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor& descriptor)
{
    return getStaticFunctionDescriptor<JSObject>(exec, ExecState::dateTable(exec), this, propertyName, descriptor);
}

// Functions

JSValue QT_FASTCALL dateProtoFuncToString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNontrivialString(exec, "Invalid Date");
    DateConversionBuffer date;
    DateConversionBuffer time;
    formatDate(*gregorianDateTime, date);
    formatTime(*gregorianDateTime, time);
    return jsNontrivialString(exec, makeString(date, " ", time));
}

JSValue QT_FASTCALL dateProtoFuncToUTCString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNontrivialString(exec, "Invalid Date");
    DateConversionBuffer date;
    DateConversionBuffer time;
    formatDateUTCVariant(*gregorianDateTime, date);
    formatTimeUTC(*gregorianDateTime, time);
    return jsNontrivialString(exec, makeString(date, " ", time));
}

JSValue QT_FASTCALL dateProtoFuncToISOString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);
    
    DateInstance* thisDateObj = asDateInstance(thisValue); 
    
    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNontrivialString(exec, "Invalid Date");
    // Maximum amount of space we need in buffer: 6 (max. digits in year) + 2 * 5 (2 characters each for month, day, hour, minute, second) + 4 (. + 3 digits for milliseconds)
    // 6 for formatting and one for null termination = 27.  We add one extra character to allow us to force null termination.
    char buffer[28];
    snprintf(buffer, sizeof(buffer) - 1, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900 + gregorianDateTime->year, gregorianDateTime->month + 1, gregorianDateTime->monthDay, gregorianDateTime->hour, gregorianDateTime->minute, gregorianDateTime->second, static_cast<int>(fmod(thisDateObj->internalNumber(), 1000)));
    buffer[sizeof(buffer) - 1] = 0;
    return jsNontrivialString(exec, buffer);
}

JSValue QT_FASTCALL dateProtoFuncToDateString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNontrivialString(exec, "Invalid Date");
    DateConversionBuffer date;
    formatDate(*gregorianDateTime, date);
    return jsNontrivialString(exec, date);
}

JSValue QT_FASTCALL dateProtoFuncToTimeString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNontrivialString(exec, "Invalid Date");
    DateConversionBuffer time;
    formatTime(*gregorianDateTime, time);
    return jsNontrivialString(exec, time);
}

JSValue QT_FASTCALL dateProtoFuncToLocaleString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 
    return formatLocaleDate(exec, thisDateObj, thisDateObj->internalNumber(), LocaleDateAndTime, args);
}

JSValue QT_FASTCALL dateProtoFuncToLocaleDateString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 
    return formatLocaleDate(exec, thisDateObj, thisDateObj->internalNumber(), LocaleDate, args);
}

JSValue QT_FASTCALL dateProtoFuncToLocaleTimeString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 
    return formatLocaleDate(exec, thisDateObj, thisDateObj->internalNumber(), LocaleTime, args);
}

JSValue QT_FASTCALL dateProtoFuncGetTime(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    return asDateInstance(thisValue)->internalValue(); 
}

JSValue QT_FASTCALL dateProtoFuncGetFullYear(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, 1900 + gregorianDateTime->year);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCFullYear(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, 1900 + gregorianDateTime->year);
}

JSValue QT_FASTCALL dateProtoFuncToGMTString(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNontrivialString(exec, "Invalid Date");
    DateConversionBuffer date;
    DateConversionBuffer time;
    formatDateUTCVariant(*gregorianDateTime, date);
    formatTimeUTC(*gregorianDateTime, time);
    return jsNontrivialString(exec, makeString(date, " ", time));
}

JSValue QT_FASTCALL dateProtoFuncGetMonth(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->month);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCMonth(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->month);
}

JSValue QT_FASTCALL dateProtoFuncGetDate(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->monthDay);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCDate(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->monthDay);
}

JSValue QT_FASTCALL dateProtoFuncGetDay(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->weekDay);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCDay(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->weekDay);
}

JSValue QT_FASTCALL dateProtoFuncGetHours(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->hour);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCHours(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->hour);
}

JSValue QT_FASTCALL dateProtoFuncGetMinutes(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->minute);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCMinutes(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->minute);
}

JSValue QT_FASTCALL dateProtoFuncGetSeconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->second);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCSeconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTimeUTC(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, gregorianDateTime->second);
}

JSValue QT_FASTCALL dateProtoFuncGetMilliSeconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 
    double milli = thisDateObj->internalNumber();
    if (std::isnan(milli))
        return jsNaN(exec);

    double secs = floor(milli / msPerSecond);
    double ms = milli - secs * msPerSecond;
    return jsNumber(exec, ms);
}

JSValue QT_FASTCALL dateProtoFuncGetUTCMilliseconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 
    double milli = thisDateObj->internalNumber();
    if (std::isnan(milli))
        return jsNaN(exec);

    double secs = floor(milli / msPerSecond);
    double ms = milli - secs * msPerSecond;
    return jsNumber(exec, ms);
}

JSValue QT_FASTCALL dateProtoFuncGetTimezoneOffset(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);
    return jsNumber(exec, -gregorianDateTime->utcOffset / minutesPerHour);
}

JSValue QT_FASTCALL dateProtoFuncSetTime(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    double milli = timeClip(args.at(0).toNumber(exec));
    JSValue result = jsNumber(exec, milli);
    thisDateObj->setInternalValue(result);
    return result;
}

static JSValue setNewValueFromTimeArgs(ExecState* exec, JSValue thisValue, const ArgList& args, int numArgsToUse, bool inputIsUTC)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue);
    double milli = thisDateObj->internalNumber();
    
    if (args.isEmpty() || std::isnan(milli)) {
        JSValue result = jsNaN(exec);
        thisDateObj->setInternalValue(result);
        return result;
    }
     
    double secs = floor(milli / msPerSecond);
    double ms = milli - secs * msPerSecond;

    const GregorianDateTime* other = inputIsUTC 
        ? thisDateObj->gregorianDateTimeUTC(exec)
        : thisDateObj->gregorianDateTime(exec);
    if (!other)
        return jsNaN(exec);

    GregorianDateTime gregorianDateTime;
    gregorianDateTime.copyFrom(*other);
    if (!fillStructuresUsingTimeArgs(exec, args, numArgsToUse, &ms, &gregorianDateTime)) {
        JSValue result = jsNaN(exec);
        thisDateObj->setInternalValue(result);
        return result;
    } 
    
    JSValue result = jsNumber(exec, gregorianDateTimeToMS(exec, gregorianDateTime, ms, inputIsUTC));
    thisDateObj->setInternalValue(result);
    return result;
}

static JSValue setNewValueFromDateArgs(ExecState* exec, JSValue thisValue, const ArgList& args, int numArgsToUse, bool inputIsUTC)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue);
    if (args.isEmpty()) {
        JSValue result = jsNaN(exec);
        thisDateObj->setInternalValue(result);
        return result;
    }      
    
    double milli = thisDateObj->internalNumber();
    double ms = 0; 

    GregorianDateTime gregorianDateTime; 
    if (numArgsToUse == 3 && std::isnan(milli)) 
        msToGregorianDateTime(exec, 0, true, gregorianDateTime); 
    else { 
        ms = milli - floor(milli / msPerSecond) * msPerSecond; 
        const GregorianDateTime* other = inputIsUTC 
            ? thisDateObj->gregorianDateTimeUTC(exec)
            : thisDateObj->gregorianDateTime(exec);
        if (!other)
            return jsNaN(exec);
        gregorianDateTime.copyFrom(*other);
    }

    if (!fillStructuresUsingDateArgs(exec, args, numArgsToUse, &ms, &gregorianDateTime)) {
        JSValue result = jsNaN(exec);
        thisDateObj->setInternalValue(result);
        return result;
    } 

    JSValue result = jsNumber(exec, gregorianDateTimeToMS(exec, gregorianDateTime, ms, inputIsUTC));
    thisDateObj->setInternalValue(result);
    return result;
}

JSValue QT_FASTCALL dateProtoFuncSetMilliSeconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = false;
    return setNewValueFromTimeArgs(exec, thisValue, args, 1, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetUTCMilliseconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = true;
    return setNewValueFromTimeArgs(exec, thisValue, args, 1, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetSeconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = false;
    return setNewValueFromTimeArgs(exec, thisValue, args, 2, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetUTCSeconds(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = true;
    return setNewValueFromTimeArgs(exec, thisValue, args, 2, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetMinutes(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = false;
    return setNewValueFromTimeArgs(exec, thisValue, args, 3, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetUTCMinutes(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = true;
    return setNewValueFromTimeArgs(exec, thisValue, args, 3, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetHours(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = false;
    return setNewValueFromTimeArgs(exec, thisValue, args, 4, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetUTCHours(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = true;
    return setNewValueFromTimeArgs(exec, thisValue, args, 4, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetDate(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = false;
    return setNewValueFromDateArgs(exec, thisValue, args, 1, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetUTCDate(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = true;
    return setNewValueFromDateArgs(exec, thisValue, args, 1, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetMonth(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = false;
    return setNewValueFromDateArgs(exec, thisValue, args, 2, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetUTCMonth(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = true;
    return setNewValueFromDateArgs(exec, thisValue, args, 2, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetFullYear(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = false;
    return setNewValueFromDateArgs(exec, thisValue, args, 3, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetUTCFullYear(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    const bool inputIsUTC = true;
    return setNewValueFromDateArgs(exec, thisValue, args, 3, inputIsUTC);
}

JSValue QT_FASTCALL dateProtoFuncSetYear(ExecState* exec, JSObject*, JSValue thisValue, const ArgList& args)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue);     
    if (args.isEmpty()) { 
        JSValue result = jsNaN(exec);
        thisDateObj->setInternalValue(result);
        return result;
    }
    
    double milli = thisDateObj->internalNumber();
    double ms = 0;

    GregorianDateTime gregorianDateTime;
    if (std::isnan(milli))
        // Based on ECMA 262 B.2.5 (setYear)
        // the time must be reset to +0 if it is NaN. 
        msToGregorianDateTime(exec, 0, true, gregorianDateTime);
    else {   
        double secs = floor(milli / msPerSecond);
        ms = milli - secs * msPerSecond;
        if (const GregorianDateTime* other = thisDateObj->gregorianDateTime(exec))
            gregorianDateTime.copyFrom(*other);
    }
    
    bool ok = true;
    int32_t year = args.at(0).toInt32(exec, ok);
    if (!ok) {
        JSValue result = jsNaN(exec);
        thisDateObj->setInternalValue(result);
        return result;
    }
            
    gregorianDateTime.year = (year > 99 || year < 0) ? year - 1900 : year;
    JSValue result = jsNumber(exec, gregorianDateTimeToMS(exec, gregorianDateTime, ms, false));
    thisDateObj->setInternalValue(result);
    return result;
}

JSValue QT_FASTCALL dateProtoFuncGetYear(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    if (!thisValue.inherits(&DateInstance::info))
        return throwError(exec, TypeError);

    DateInstance* thisDateObj = asDateInstance(thisValue); 

    const GregorianDateTime* gregorianDateTime = thisDateObj->gregorianDateTime(exec);
    if (!gregorianDateTime)
        return jsNaN(exec);

    // NOTE: IE returns the full year even in getYear.
    return jsNumber(exec, gregorianDateTime->year);
}

JSValue QT_FASTCALL dateProtoFuncToJSON(ExecState* exec, JSObject*, JSValue thisValue, const ArgList&)
{
    JSObject* object = thisValue.toThisObject(exec);
    if (exec->hadException())
        return jsNull();
    
    JSValue toISOValue = object->get(exec, exec->globalData().propertyNames->toISOString);
    if (exec->hadException())
        return jsNull();

    CallData callData;
    CallType callType = toISOValue.getCallData(callData);
    if (callType == CallTypeNone)
        return throwError(exec, TypeError, "toISOString is not a function");

    JSValue result = call(exec, asObject(toISOValue), callType, callData, object, exec->emptyList());
    if (exec->hadException())
        return jsNull();
    if (result.isObject())
        return throwError(exec, TypeError, "toISOString did not return a primitive value");
    return result;
}

} // namespace JSC
