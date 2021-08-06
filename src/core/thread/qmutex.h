/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMUTEX_H
#define QMUTEX_H

#include <QtCore/qglobal.h>

#include <mutex>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_THREAD

class Q_CORE_EXPORT QMutex
{
public:
    QMutex() { }
    ~QMutex() { }

    inline void lock() {
        mutex.lock();
    }

    inline bool tryLock() {
        return mutex.try_lock();
    }

    inline bool tryLock(int timeout) {
        return mutex.try_lock_for(std::chrono::milliseconds(timeout));
    }

    inline void unlock() {
        mutex.unlock();
    }

private:
    Q_DISABLE_COPY(QMutex)

    std::timed_mutex mutex;
};

class Q_CORE_EXPORT QMutexLocker
{
public:
    inline explicit QMutexLocker(QMutex *m)
    {
        Q_ASSERT_X((reinterpret_cast<quintptr>(m) & quintptr(1u)) == quintptr(0),
                   "QMutexLocker", "QMutex pointer is misaligned");
        if (m) {
            m->lock();
            val = reinterpret_cast<quintptr>(m) | quintptr(1u);
        } else {
            val = 0;
        }
    }
    inline ~QMutexLocker() { unlock(); }

    inline void unlock()
    {
        if ((val & quintptr(1u)) == quintptr(1u)) {
            val &= ~quintptr(1u);
            mutex()->unlock();
        }
    }

    inline void relock()
    {
        if (val) {
            if ((val & quintptr(1u)) == quintptr(0u)) {
                mutex()->lock();
                val |= quintptr(1u);
            }
        }
    }

    inline QMutex *mutex() const
    {
        return reinterpret_cast<QMutex *>(val & ~quintptr(1u));
    }

private:
    Q_DISABLE_COPY(QMutexLocker)

    quintptr val;
};


#else // QT_NO_THREAD


class Q_CORE_EXPORT QMutex
{
public:
    inline QMutex() { }
    inline ~QMutex() { }

    static inline void lock() { }
    static inline bool tryLock(int timeout = 0) { Q_UNUSED(timeout); return true; }
    static inline void unlock() { }

private:
    Q_DISABLE_COPY(QMutex)
};

class Q_CORE_EXPORT QMutexLocker
{
public:
    inline explicit QMutexLocker(QMutex *) { }
    inline ~QMutexLocker() { }

    static inline void unlock() { }
    static void relock() { }
    static inline QMutex *mutex() { return nullptr; }

private:
    Q_DISABLE_COPY(QMutexLocker)
};

#endif // QT_NO_THREAD

QT_END_NAMESPACE


#endif // QMUTEX_H
