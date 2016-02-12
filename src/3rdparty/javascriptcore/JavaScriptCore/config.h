/*
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include <wtf/Platform.h>

#if !defined(QT_BUILD_SCRIPT_LIB) && OS(WINDOWS) && !defined(BUILDING_WX__) && !COMPILER(GCC)
#if defined(BUILDING_JavaScriptCore) || defined(BUILDING_WTF)
#define JS_EXPORTDATA __declspec(dllexport)
#else
#define JS_EXPORTDATA __declspec(dllimport)
#endif
#define JS_EXPORTCLASS JS_EXPORTDATA
#else
#define JS_EXPORTDATA
#define JS_EXPORTCLASS
#endif

#if OS(WINDOWS)

// If we don't define these, they get defined in windef.h. 
// We want to use std::min and std::max
#define max max
#define min min

#if !COMPILER(MSVC7) && !OS(WINCE)
// We need to define this before the first #include of stdlib.h or it won't contain rand_s.
#ifndef _CRT_RAND_S
#define _CRT_RAND_S
#endif
#endif

#endif

#if OS(FREEBSD) || OS(OPENBSD)
#define HAVE_PTHREAD_NP_H 1
#endif

/* FIXME: if all platforms have these, do they really need #defines? */
#define HAVE_STDINT_H 1

#ifdef __cplusplus
#undef new
#undef delete
#include <wtf/FastMalloc.h>
#endif
