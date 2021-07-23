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

#ifndef QREADWRITELOCK_H
#define QREADWRITELOCK_H

#include <QtCore/qglobal.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_THREAD

struct QReadWriteLockPrivate;

class Q_CORE_EXPORT QReadWriteLock
{
public:
    QReadWriteLock();
    ~QReadWriteLock();

    void lockForRead();
    bool tryLockForRead();
    bool tryLockForRead(int timeout);

    void lockForWrite();
    bool tryLockForWrite();
    bool tryLockForWrite(int timeout);

    void unlock();

private:
    Q_DISABLE_COPY(QReadWriteLock)
    QReadWriteLockPrivate *d;

    friend class QWaitCondition;
};

class Q_CORE_EXPORT QReadLocker
{
public:
    inline QReadLocker(QReadWriteLock *readWriteLock);

    inline ~QReadLocker()
    { unlock(); }

    inline void unlock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(1u)) {
                q_val &= ~quintptr(1u);
                readWriteLock()->unlock();
            }
        }
    }

    inline void relock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(0u)) {
                readWriteLock()->lockForRead();
                q_val |= quintptr(1u);
            }
        }
    }

    inline QReadWriteLock *readWriteLock() const
    { return reinterpret_cast<QReadWriteLock *>(q_val & ~quintptr(1u)); }

private:
    Q_DISABLE_COPY(QReadLocker)
    quintptr q_val;
};

inline QReadLocker::QReadLocker(QReadWriteLock *areadWriteLock)
    : q_val(reinterpret_cast<quintptr>(areadWriteLock))
{
    Q_ASSERT_X((q_val & quintptr(1u)) == quintptr(0),
               "QReadLocker", "QReadWriteLock pointer is misaligned");
    relock();
}

class Q_CORE_EXPORT QWriteLocker
{
public:
    inline QWriteLocker(QReadWriteLock *readWriteLock);

    inline ~QWriteLocker()
    { unlock(); }

    inline void unlock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(1u)) {
                q_val &= ~quintptr(1u);
                readWriteLock()->unlock();
            }
        }
    }

    inline void relock()
    {
        if (q_val) {
            if ((q_val & quintptr(1u)) == quintptr(0u)) {
                readWriteLock()->lockForWrite();
                q_val |= quintptr(1u);
            }
        }
    }

    inline QReadWriteLock *readWriteLock() const
    { return reinterpret_cast<QReadWriteLock *>(q_val & ~quintptr(1u)); }


private:
    Q_DISABLE_COPY(QWriteLocker)
    quintptr q_val;
};

inline QWriteLocker::QWriteLocker(QReadWriteLock *areadWriteLock)
    : q_val(reinterpret_cast<quintptr>(areadWriteLock))
{
    Q_ASSERT_X((q_val & quintptr(1u)) == quintptr(0),
               "QWriteLocker", "QReadWriteLock pointer is misaligned");
    relock();
}

#else // QT_NO_THREAD

class Q_CORE_EXPORT QReadWriteLock
{
public:
    inline QReadWriteLock() { }
    inline ~QReadWriteLock() { }

    static inline void lockForRead() { }
    static inline bool tryLockForRead() { return true; }
    static inline bool tryLockForRead(int timeout) { Q_UNUSED(timeout); return true; }

    static inline void lockForWrite() { }
    static inline bool tryLockForWrite() { return true; }
    static inline bool tryLockForWrite(int timeout) { Q_UNUSED(timeout); return true; }

    static inline void unlock() { }

private:
    Q_DISABLE_COPY(QReadWriteLock)
};

class Q_CORE_EXPORT QReadLocker
{
public:
    inline QReadLocker(QReadWriteLock *) { }
    inline ~QReadLocker() { }

    static inline void unlock() { }
    static inline void relock() { }
    static inline QReadWriteLock *readWriteLock() { return 0; }

private:
    Q_DISABLE_COPY(QReadLocker)
};

class Q_CORE_EXPORT QWriteLocker
{
public:
    inline explicit QWriteLocker(QReadWriteLock *) { }
    inline ~QWriteLocker() { }

    static inline void unlock() { }
    static inline void relock() { }
    static inline QReadWriteLock *readWriteLock() { return 0; }

private:
    Q_DISABLE_COPY(QWriteLocker)
};

#endif // QT_NO_THREAD

QT_END_NAMESPACE


#endif // QREADWRITELOCK_H
