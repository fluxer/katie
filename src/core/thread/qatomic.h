/***********************************************************************
*
* Copyright (c) 2012-2016 Barbara Geller
* Copyright (c) 2012-2016 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software: you can redistribute it and/or 
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CopperSpice.  If not, see 
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef QATOMIC_H
#define QATOMIC_H

#include "QtCore/qglobal.h"

#include <atomic>
#include <cstddef>

#ifndef ATOMIC_INT_LOCK_FREE
#define ATOMIC_INT_LOCK_FREE 0
#endif

#ifndef ATOMIC_POINTER_LOCK_FREE
#define ATOMIC_POINTER_LOCK_FREE 0
#endif

QT_BEGIN_NAMESPACE
QT_BEGIN_HEADER

class Q_CORE_EXPORT QAtomicInt
{
   public:
      inline QAtomicInt() : m_data(0) { 
      }

      inline QAtomicInt(int value) : m_data(value) { 
      }

      inline QAtomicInt(const QAtomicInt &other) {
         int data = other.load();
         store(data);
      }

      inline QAtomicInt &operator=(const QAtomicInt &other) {
         int data = other.load();
         store(data);

         return *this;
      }

      inline QAtomicInt &operator=(int data) {
         store(data);
         return *this;
      }

      int load() const {
         return m_data.load();
      }

      int loadAcquire() {
         return m_data.load(std::memory_order_acquire);
      }

      void store(int newValue) {
         m_data.store(newValue);
      }

      void storeRelease(int newValue) {
         m_data.store(newValue, std::memory_order_release);
      }

      bool ref()  {
         int newValue = ++m_data;
         return newValue != 0;
      }

      bool deref()  {
         int newValue = --m_data;
         return newValue != 0;
      }

      bool testAndSetRelaxed(int expectedValue, int newValue)  {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_relaxed);         
      }

      bool testAndSetAcquire(int expectedValue, int newValue) {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_acquire); 
      }

      bool testAndSetRelease(int expectedValue, int newValue) {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_release); 
      }

      bool testAndSetOrdered(int expectedValue, int newValue) {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_seq_cst); 
      }

      int fetchAndStoreRelaxed(int newValue) {
         return m_data.exchange(newValue, std::memory_order_relaxed);
      }

      int fetchAndStoreAcquire(int newValue) {
         return m_data.exchange(newValue, std::memory_order_acquire);
      }

      int fetchAndStoreRelease(int newValue) {
         return m_data.exchange(newValue, std::memory_order_release);
      }

      int fetchAndStoreOrdered(int newValue) {
         return m_data.exchange(newValue, std::memory_order_seq_cst);
      }

      int fetchAndAddRelaxed(int valueToAdd) {
         return m_data.fetch_add(valueToAdd, std::memory_order_relaxed);
      }

      int fetchAndAddAcquire(int valueToAdd) {
         return m_data.fetch_add(valueToAdd, std::memory_order_acquire);
      }

      int fetchAndAddRelease(int valueToAdd) {
         return m_data.fetch_add(valueToAdd, std::memory_order_release);
      }

      int fetchAndAddOrdered(int valueToAdd) {
         return m_data.fetch_add(valueToAdd, std::memory_order_seq_cst);
      }

      inline operator int() const
        { return load(); }

      static bool isLockFree() {
         return ATOMIC_INT_LOCK_FREE == 2;
      }

   private:
      std::atomic<int> m_data;
};

template <typename T>
class Q_CORE_EXPORT QAtomicPointer
{
   public:
      QAtomicPointer(T *value = 0) : m_data(value) {
      }

      QAtomicPointer(const QAtomicPointer<T> &other) {
         T *data = other.load();
         store(data);
      }

      QAtomicPointer<T> &operator=(const QAtomicPointer<T> &other) {
         T *data = other.load();
         store(data);

         return *this;
      }

      QAtomicPointer<T> &operator=(T *data) {
         store(data);
         return *this;
      }

      T *load() const {
         return m_data.load();
      }

      T *loadAcquire() {
         return m_data.load(std::memory_order_acquire);
      }

      void store(T *newValue) {
         m_data.store(newValue);
      }

      void storeRelease(T *newValue) {
         m_data.store(newValue, std::memory_order_release);
      }

      bool testAndSetRelaxed(T *expectedValue, T *newValue) {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_relaxed);
      }

      bool testAndSetAcquire(T *expectedValue, T *newValue) {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_acquire);
      }

      bool testAndSetRelease(T *expectedValue, T *newValue) {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_release);
      }

      bool testAndSetOrdered(T *expectedValue, T *newValue) {
         return m_data.compare_exchange_strong(expectedValue, newValue, std::memory_order_seq_cst);
      }

      T *fetchAndStoreRelaxed(T *newValue)  {
         return m_data.exchange(newValue, std::memory_order_relaxed);
      }

      T *fetchAndStoreAcquire(T *newValue) {
         return m_data.exchange(newValue, std::memory_order_acquire);
      }

      T *fetchAndStoreRelease(T *newValue) {
         return m_data.exchange(newValue, std::memory_order_release);
      }

      T *fetchAndStoreOrdered(T *newValue) {
         return m_data.exchange(newValue, std::memory_order_seq_cst);
      }

      T *fetchAndAddRelaxed(std::ptrdiff_t valueToAdd) {
         return m_data.fetch_add(valueToAdd, std::memory_order_relaxed);
      }

      T *fetchAndAddAcquire(std::ptrdiff_t valueToAdd) {
         return m_data.fetch_add(valueToAdd, std::memory_order_acquire);
      }

      T *fetchAndAddRelease(std::ptrdiff_t valueToAdd){
         return m_data.fetch_add(valueToAdd, std::memory_order_release);
      }

      T *fetchAndAddOrdered(std::ptrdiff_t valueToAdd){
         return m_data.fetch_add(valueToAdd, std::memory_order_seq_cst);
      }

      inline operator T *() const
        { return load(); }
      inline T *operator->() const
        { return load(); }

      static bool isLockFree() {
         return ATOMIC_POINTER_LOCK_FREE == 2;
      }

   private:
      std::atomic<T *> m_data;
};

/*!
    This is a helper for the assignment operators of implicitly
    shared classes. Your assignment operator should look like this:

    \snippet doc/src/snippets/code/src.corelib.thread.qatomic.h 0
*/
template <typename T>
inline void qAtomicAssign(T *&d, T *x)
{
   if (d == x) {
      return;
   }

   x->ref.ref();
   if (!d->ref.deref()) {
      delete d;
   }

   d = x;
}

/*!
    This is a helper for the detach method of implicitly shared
    classes. Your private class needs a copy constructor which copies
    the members and sets the refcount to 1. After that, your detach
    function should look like this:

    \snippet doc/src/snippets/code/src.corelib.thread.qatomic.h 1
*/
template <typename T>
inline void qAtomicDetach(T *&d)
{
   if (d->ref.load() == 1) {
      return;
   }

   T *x = d;
   d = new T(*d);

   if (!x->ref.deref()) {
      delete x;
   }
}

QT_END_NAMESPACE
QT_END_HEADER

#endif // QATOMIC_H
