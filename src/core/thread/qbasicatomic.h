/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QBASICATOMIC_H
#define QBASICATOMIC_H

#include <QtCore/qglobal.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QBasicAtomicInt
{
public:
#ifdef QT_ARCH_PARISC
    int _q_lock[4];
#endif
#if defined(QT_ARCH_WINDOWS) || defined(QT_ARCH_WINDOWSCE)
    union { // needed for Q_BASIC_ATOMIC_INITIALIZER
        volatile long _q_value;
    };
#else
    volatile int _q_value;
#endif

    // Non-atomic API
    inline bool operator==(int value) const
    {
        return _q_value == value;
    }

    inline bool operator!=(int value) const
    {
        return _q_value != value;
    }

    inline bool operator!() const
    {
        return _q_value == 0;
    }

    inline operator int() const
    {
        return _q_value;
    }

    inline QBasicAtomicInt &operator=(int value)
    {
#ifdef QT_ARCH_PARISC
        this->_q_lock[0] = this->_q_lock[1] = this->_q_lock[2] = this->_q_lock[3] = -1;
#endif
        _q_value = value;
        return *this;
    }

    // Atomic API, implemented in qatomic_XXX.h

    static bool isReferenceCountingNative();
    static bool isReferenceCountingWaitFree();

    bool ref();
    bool deref();

    static bool isTestAndSetNative();
    static bool isTestAndSetWaitFree();

    bool testAndSetRelaxed(int expectedValue, int newValue);
    bool testAndSetAcquire(int expectedValue, int newValue);
    bool testAndSetRelease(int expectedValue, int newValue);
    bool testAndSetOrdered(int expectedValue, int newValue);

    static bool isFetchAndStoreNative();
    static bool isFetchAndStoreWaitFree();

    int fetchAndStoreRelaxed(int newValue);
    int fetchAndStoreAcquire(int newValue);
    int fetchAndStoreRelease(int newValue);
    int fetchAndStoreOrdered(int newValue);

    static bool isFetchAndAddNative();
    static bool isFetchAndAddWaitFree();

    int fetchAndAddRelaxed(int valueToAdd);
    int fetchAndAddAcquire(int valueToAdd);
    int fetchAndAddRelease(int valueToAdd);
    int fetchAndAddOrdered(int valueToAdd);
};

template <typename T>
class QBasicAtomicPointer
{
public:
#ifdef QT_ARCH_PARISC
    int _q_lock[4];
#endif
#if defined(QT_ARCH_WINDOWS) || defined(QT_ARCH_WINDOWSCE)
    union {
        T * volatile _q_value;
#  if !defined(Q_OS_WINCE) && !defined(__i386__) && !defined(_M_IX86)
        qint64
#  else
        long
#  endif
        volatile _q_value_integral;
    };
#else
    T * volatile _q_value;
#endif

    // Non-atomic API
    inline bool operator==(T *value) const
    {
        return _q_value == value;
    }

    inline bool operator!=(T *value) const
    {
        return !operator==(value);
    }

    inline bool operator!() const
    {
        return operator==(0);
    }

    inline operator T *() const
    {
        return _q_value;
    }

    inline T *operator->() const
    {
        return _q_value;
    }

    inline QBasicAtomicPointer<T> &operator=(T *value)
    {
#ifdef QT_ARCH_PARISC
        this->_q_lock[0] = this->_q_lock[1] = this->_q_lock[2] = this->_q_lock[3] = -1;
#endif
        _q_value = value;
        return *this;
    }

    // Atomic API, implemented in qatomic_XXX.h

    static bool isTestAndSetNative();
    static bool isTestAndSetWaitFree();

    bool testAndSetRelaxed(T *expectedValue, T *newValue);
    bool testAndSetAcquire(T *expectedValue, T *newValue);
    bool testAndSetRelease(T *expectedValue, T *newValue);
    bool testAndSetOrdered(T *expectedValue, T *newValue);

    static bool isFetchAndStoreNative();
    static bool isFetchAndStoreWaitFree();

    T *fetchAndStoreRelaxed(T *newValue);
    T *fetchAndStoreAcquire(T *newValue);
    T *fetchAndStoreRelease(T *newValue);
    T *fetchAndStoreOrdered(T *newValue);

    static bool isFetchAndAddNative();
    static bool isFetchAndAddWaitFree();

    T *fetchAndAddRelaxed(qptrdiff valueToAdd);
    T *fetchAndAddAcquire(qptrdiff valueToAdd);
    T *fetchAndAddRelease(qptrdiff valueToAdd);
    T *fetchAndAddOrdered(qptrdiff valueToAdd);
};

#ifdef QT_ARCH_PARISC
#  define Q_BASIC_ATOMIC_INITIALIZER(a) {{-1,-1,-1,-1},(a)}
#elif defined(QT_ARCH_WINDOWS) || defined(QT_ARCH_WINDOWSCE)
#  define Q_BASIC_ATOMIC_INITIALIZER(a) { {(a)} }
#else
#  define Q_BASIC_ATOMIC_INITIALIZER(a) { (a) }
#endif

QT_END_NAMESPACE

#if defined(QT_BOOTSTRAPPED)
#  include <qatomic_bootstrap.h>
#elif defined(QT_ARCH_INTEGRITY)
#  include "QtCore/qatomic_integrity.h"
#elif defined(QT_ARCH_VXWORKS)
#  include "QtCore/qatomic_vxworks.h"
#elif defined(QT_ARCH_ALPHA)
#  include "QtCore/qatomic_alpha.h"
#elif defined(QT_ARCH_ARM)
#  include "QtCore/qatomic_arm.h"
#elif defined(QT_ARCH_ARMV6)
#  include "QtCore/qatomic_armv6.h"
#elif defined(QT_ARCH_AVR32)
#  include "QtCore/qatomic_avr32.h"
#elif defined(QT_ARCH_BFIN)
#  include "QtCore/qatomic_bfin.h"
#elif defined(QT_ARCH_GENERIC)
#  include "QtCore/qatomic_generic.h"
#elif defined(QT_ARCH_I386)
#  include "QtCore/qatomic_i386.h"
#elif defined(QT_ARCH_IA64)
#  include "QtCore/qatomic_ia64.h"
#elif defined(QT_ARCH_M68K)
#  include "QtCore/qatomic_m68k.h"
#elif defined(QT_ARCH_MACOSX)
#  include "QtCore/qatomic_macosx.h"
#elif defined(QT_ARCH_MIPS)
#  include "QtCore/qatomic_mips.h"
#elif defined(QT_ARCH_PARISC)
#  include "QtCore/qatomic_parisc.h"
#elif defined(QT_ARCH_POWERPC)
#  include "QtCore/qatomic_powerpc.h"
#elif defined(QT_ARCH_S390)
#  include "QtCore/qatomic_s390.h"
#elif defined(QT_ARCH_SPARC)
#  include "QtCore/qatomic_sparc.h"
#elif defined(QT_ARCH_WINDOWS)
#  include "QtCore/qatomic_windows.h"
#elif defined(QT_ARCH_WINDOWSCE)
#  include "QtCore/qatomic_windowsce.h"
#elif defined(QT_ARCH_X86_64)
#  include "QtCore/qatomic_x86_64.h"
#elif defined(QT_ARCH_SH)
#  include "QtCore/qatomic_sh.h"
#elif defined(QT_ARCH_SH4A)
#  include "QtCore/qatomic_sh4a.h"
#elif defined(QT_ARCH_NACL)
#  include "QtCore/qatomic_generic.h"
#else
#  error "Qt has not been ported to this architecture"
#endif

QT_END_HEADER

#endif // QBASIC_ATOMIC
