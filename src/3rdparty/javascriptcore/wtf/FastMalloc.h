/*
 *  Copyright (C) 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef WTF_FastMalloc_h
#define WTF_FastMalloc_h

#include "Platform.h"
#include <stdlib.h>
#include <new>

namespace WTF {

    // These functions call CRASH() if an allocation fails.
    void* fastMalloc(size_t);
    void* fastZeroedMalloc(size_t);
    void* fastRealloc(void*, size_t);

    inline void* tryFastMalloc(size_t n) { return ::malloc(n); };
    inline void* tryFastRealloc(void* p, size_t n) { return ::realloc(p, n); };

    inline void fastFree(void* p) { ::free(p); };
} // namespace WTF

using WTF::fastMalloc;
using WTF::fastZeroedMalloc;
using WTF::fastRealloc;
using WTF::tryFastMalloc;
using WTF::tryFastRealloc;
using WTF::fastFree;

#endif /* WTF_FastMalloc_h */
