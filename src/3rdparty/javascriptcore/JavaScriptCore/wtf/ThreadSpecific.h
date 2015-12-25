/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Jian Li <jianli@chromium.org>
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

/* Thread local storage is implemented by using either pthread API or Windows
 * native API. There is subtle semantic discrepancy for the cleanup function
 * implementation as noted below:
 *   @ In pthread implementation, the destructor function will be called
 *     repeatedly if there is still non-NULL value associated with the function.
 *   @ In Windows native implementation, the destructor function will be called
 *     only once.
 * This semantic discrepancy does not impose any problem because nowhere in
 * WebKit the repeated call bahavior is utilized.
 */

#ifndef WTF_ThreadSpecific_h
#define WTF_ThreadSpecific_h

#include <wtf/Noncopyable.h>

#include <QThreadStorage>

namespace WTF {

template<typename T> class ThreadSpecific : public Noncopyable {
public:
    ThreadSpecific();
    T* operator->();
    operator T*();
    T& operator*();
    ~ThreadSpecific();

private:
    
    T* get();
    void set(T*);
    void static destroy(void* ptr);

    struct Data : Noncopyable {
        Data(T* value, ThreadSpecific<T>* owner) : value(value), owner(owner) {}
        ~Data() { owner->destroy(this); }

        T* value;
        ThreadSpecific<T>* owner;
    };

#if ENABLE(SINGLE_THREADED)
    T* m_value;
#else
    QThreadStorage<Data*> m_key;
#endif
};

#if ENABLE(SINGLE_THREADED)
template<typename T>
inline ThreadSpecific<T>::ThreadSpecific()
    : m_value(0)
{
}

template<typename T>
inline ThreadSpecific<T>::~ThreadSpecific()
{
}

template<typename T>
inline T* ThreadSpecific<T>::get()
{
    return m_value;
}

template<typename T>
inline void ThreadSpecific<T>::set(T* ptr)
{
    ASSERT(!get());
    m_value = ptr;
}
#else
template<typename T>
inline ThreadSpecific<T>::ThreadSpecific()
{
}

template<typename T>
inline ThreadSpecific<T>::~ThreadSpecific()
{
    // Does not invoke destructor functions. QThreadStorage will do it
}

template<typename T>
inline T* ThreadSpecific<T>::get()
{
    Data* data = static_cast<Data*>(m_key.localData());
    return data ? data->value : 0;
}

template<typename T>
inline void ThreadSpecific<T>::set(T* ptr)
{
    ASSERT(!get());
    Data* data = new Data(ptr, this);
    m_key.setLocalData(data);
}
#endif

template<typename T>
inline void ThreadSpecific<T>::destroy(void* ptr)
{
#if !ENABLE(SINGLE_THREADED)
    Data* data = static_cast<Data*>(ptr);

    // See comment as above
    if (!data->owner->m_key.hasLocalData())
        data->owner->m_key.setLocalData(data);

    data->value->~T();
    fastFree(data->value);

    delete data;
#endif
}

template<typename T>
inline ThreadSpecific<T>::operator T*()
{
    T* ptr = static_cast<T*>(get());
    if (!ptr) {
        // Set up thread-specific value's memory pointer before invoking constructor, in case any function it calls
        // needs to access the value, to avoid recursion.
        ptr = static_cast<T*>(fastMalloc(sizeof(T)));
        set(ptr);
        new (ptr) T;
    }
    return ptr;
}

template<typename T>
inline T* ThreadSpecific<T>::operator->()
{
    return operator T*();
}

template<typename T>
inline T& ThreadSpecific<T>::operator*()
{
    return *operator T*();
}

}

#endif
