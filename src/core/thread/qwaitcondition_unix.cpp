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

#include "qwaitcondition.h"
#include "qmutex.h"
#include "qreadwritelock.h"
#include "qreadwritelock_p.h"

#include <condition_variable>
#include <chrono>

#ifndef QT_NO_THREAD

QT_BEGIN_NAMESPACE

class QWaitConditionPrivate {
public:
    std::mutex mutex;
    std::condition_variable cond;
};


QWaitCondition::QWaitCondition()
    : d(new QWaitConditionPrivate)
{
}


QWaitCondition::~QWaitCondition()
{
    delete d;
}

void QWaitCondition::wakeOne()
{
    std::lock_guard<std::mutex> lock(d->mutex);
    d->cond.notify_one();
}

void QWaitCondition::wakeAll()
{
    std::lock_guard<std::mutex> lock(d->mutex);
    d->cond.notify_all();
}

bool QWaitCondition::wait(QMutex *mutex, unsigned long time)
{
    if (!mutex) {
        return false;
    }

    std::unique_lock<std::mutex> lock(d->mutex);
    mutex->unlock();

    std::cv_status returnValue;
    if (time != ULONG_MAX) {
        returnValue = d->cond.wait_for(lock, std::chrono::milliseconds(time));
    } else {
        returnValue = std::cv_status::no_timeout;
        d->cond.wait(lock);
    }
    lock.unlock();

    mutex->lock();

    return returnValue == std::cv_status::no_timeout;
}

bool QWaitCondition::wait(QReadWriteLock *readWriteLock, unsigned long time)
{
    if (!readWriteLock || readWriteLock->d->accessCount == 0)
        return false;
    if (Q_UNLIKELY(readWriteLock->d->accessCount < -1)) {
        qWarning("QWaitCondition: cannot wait on QReadWriteLocks with recursive lockForWrite()");
        return false;
    }

    std::unique_lock<std::mutex> lock(d->mutex);
    int previousAccessCount = readWriteLock->d->accessCount;
    readWriteLock->unlock();

    std::cv_status returnValue;
    if (time != ULONG_MAX) {
        returnValue = d->cond.wait_for(lock, std::chrono::milliseconds(time));
    } else {
        returnValue = std::cv_status::no_timeout;
        d->cond.wait(lock);
    }
    lock.unlock();

    if (previousAccessCount < 0)
        readWriteLock->lockForWrite();
    else
        readWriteLock->lockForRead();

    return returnValue == std::cv_status::no_timeout;
}

QT_END_NAMESPACE

#endif // QT_NO_THREAD
