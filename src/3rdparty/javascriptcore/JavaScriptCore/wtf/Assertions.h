/*
 * Copyright (C) 2003, 2006, 2007 Apple Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef WTF_Assertions_h
#define WTF_Assertions_h

/*
   no namespaces because this file has to be includable from C and Objective-C

   Note, this file uses many GCC extensions, but it should be compatible with
   C, Objective C, C++, and Objective C++.

   For non-debug builds, everything is disabled by default.
   Defining any of the symbols explicitly prevents this from having any effect.
   
   MSVC7 note: variadic macro support was added in MSVC8, so for now we disable
   those macros in MSVC7. For more info, see the MSDN document on variadic 
   macros here:
   
   http://msdn2.microsoft.com/en-us/library/ms177415(VS.80).aspx
*/

#include "Platform.h"

#if COMPILER(MSVC)
#include <stddef.h>
#else
#include <inttypes.h>
#endif

#ifdef NDEBUG
#define ASSERTIONS_DISABLED_DEFAULT 1
#else
#define ASSERTIONS_DISABLED_DEFAULT 0
#endif

#if COMPILER(MSVC7) || COMPILER(WINSCW)
#define HAVE_VARIADIC_MACRO 0
#else
#define HAVE_VARIADIC_MACRO 1
#endif

#ifndef ASSERT_DISABLED
#define ASSERT_DISABLED ASSERTIONS_DISABLED_DEFAULT
#endif

#ifndef ASSERT_MSG_DISABLED
#if HAVE(VARIADIC_MACRO)
#define ASSERT_MSG_DISABLED ASSERTIONS_DISABLED_DEFAULT
#else
#define ASSERT_MSG_DISABLED 1
#endif
#endif

#ifndef ASSERT_ARG_DISABLED
#define ASSERT_ARG_DISABLED ASSERTIONS_DISABLED_DEFAULT
#endif

/* CRASH -- gets us into the debugger or the crash reporter -- signals are ignored by the crash reporter so we must do better */

#ifndef CRASH
#define CRASH() do { \
    *(int *)(uintptr_t)0xbbadbeef = 0; \
    ((void(*)())0)(); /* More reliable, but doesn't say BBADBEEF */ \
} while(false)
#endif

/* ASSERT, ASSERT_NOT_REACHED, ASSERT_UNUSED */

#if OS(WINCE) && !PLATFORM(TORCHMOBILE)
/* FIXME: We include this here only to avoid a conflict with the ASSERT macro. */
#include <windows.h>
#undef min
#undef max
#undef ERROR
#endif

#if OS(WINDOWS)
/* FIXME: Change to use something other than ASSERT to avoid this conflict with the underlying platform */
#undef ASSERT
#endif

#if ASSERT_DISABLED

#define ASSERT(assertion) ((void)0)
#define ASSERT_NOT_REACHED() ((void)0)
#define ASSERT_UNUSED(variable, assertion) ((void)variable)

#else

#define ASSERT(assertion) do \
    if (!(assertion)) { \
        WTFReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #assertion); \
        CRASH(); \
    } \
while (0)

#define ASSERT_NOT_REACHED() do { \
    WTFReportAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, 0); \
    CRASH(); \
} while (0)

#define ASSERT_UNUSED(variable, assertion) ASSERT(assertion)

#endif

/* ASSERT_WITH_MESSAGE */

#if COMPILER(MSVC7)
#define ASSERT_WITH_MESSAGE(assertion) ((void)0)
#elif COMPILER(WINSCW)
#define ASSERT_WITH_MESSAGE(assertion, arg...) ((void)0)
#elif ASSERT_MSG_DISABLED
#define ASSERT_WITH_MESSAGE(assertion, ...) ((void)0)
#else
#define ASSERT_WITH_MESSAGE(assertion, ...) do \
    if (!(assertion)) { \
        WTFReportAssertionFailureWithMessage(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #assertion, __VA_ARGS__); \
        CRASH(); \
    } \
while (0)
#endif
                        
                        
/* ASSERT_ARG */

#if ASSERT_ARG_DISABLED

#define ASSERT_ARG(argName, assertion) ((void)0)

#else

#define ASSERT_ARG(argName, assertion) do \
    if (!(assertion)) { \
        WTFReportArgumentAssertionFailure(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, #argName, #assertion); \
        CRASH(); \
    } \
while (0)

#endif

/* COMPILE_ASSERT */
#ifndef COMPILE_ASSERT
#if COMPILER_SUPPORTS(C_STATIC_ASSERT)
/* Unlike static_assert below, this also works in plain C code. */
#define COMPILE_ASSERT(exp, name) _Static_assert((exp), #name)
#elif COMPILER_SUPPORTS(CXX_STATIC_ASSERT)
#define COMPILE_ASSERT(exp, name) static_assert((exp), #name)
#else
#define COMPILE_ASSERT(exp, name) typedef int dummy##name [(exp) ? 1 : -1]
#endif
#endif

#endif /* WTF_Assertions_h */
