/*
 * Copyright (C) 2008, 2009 Paul Pedriana <ppedriana@ea.com>. All rights reserved.
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

#ifndef FastAllocBase_h
#define FastAllocBase_h

// Provides customizable overrides of fastMalloc/fastFree and operator new/delete
//
// Provided functionality:
//    namespace WTF {
//        class FastAllocBase;
//
//        T*    fastNew<T>();
//        T*    fastNew<T>(arg);
//        T*    fastNew<T>(arg, arg);
//        void  fastDelete(T* p);
//    }
//
// FastDelete assumes that the underlying
//
// Example usage:
//    class Widget : public FastAllocBase { ... };
//
//    char* charPtr = fastNew<char>();
//    fastDelete(charPtr);
//
//    void** voidPtrPtr = fastNew<void*>();
//    fastDelete(voidPtrPtr);
//
//    POD* podPtr = fastNew<POD>();
//    fastDelete(podPtr);
//
//    Object* objectPtr = fastNew<Object>();
//    fastDelete(objectPtr);
//

#include <new>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Assertions.h"
#include "FastMalloc.h"
#include "TypeTraits.h"

namespace WTF {

    class FastAllocBase {
    public:
        // Placement operator new.
        void* operator new(size_t, void* p) { return p; }
        void* operator new[](size_t, void* p) { return p; }

        void* operator new(size_t size)
        {
            return fastMalloc(size);
        }

        void operator delete(void* p)
        {
            fastFree(p);
        }

        void* operator new[](size_t size)
        {
            return fastMalloc(size);
        }

        void operator delete[](void* p)
        {
            fastFree(p);
        }
    };

    // fastNew / fastDelete

    template <typename T>
    inline T* fastNew()
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        return ::new(p) T;
    }

    template <typename T, typename Arg1>
    inline T* fastNew(Arg1 arg1)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        return ::new(p) T(arg1);
    }

    template <typename T, typename Arg1, typename Arg2>
    inline T* fastNew(Arg1 arg1, Arg2 arg2)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        return ::new(p) T(arg1, arg2);
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3>
    inline T* fastNew(Arg1 arg1, Arg2 arg2, Arg3 arg3)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        return ::new(p) T(arg1, arg2, arg3);
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    inline T* fastNew(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        return ::new(p) T(arg1, arg2, arg3, arg4);
    }

    template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    inline T* fastNew(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
    {
        void* p = fastMalloc(sizeof(T));

        if (!p)
            return 0;

        return ::new(p) T(arg1, arg2, arg3, arg4, arg5);
    }

    template <typename T>
    inline void fastDelete(T* p)
    {
        if (!p)
            return;

        p->~T();
        fastFree(p);
    }

} // namespace WTF

using WTF::FastAllocBase;

#endif // FastAllocBase_h
