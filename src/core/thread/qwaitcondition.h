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

#ifndef QWAITCONDITION_H
#define QWAITCONDITION_H

#include <QtCore/qglobal.h>

#include <limits.h>


QT_BEGIN_NAMESPACE

#ifndef QT_NO_THREAD

class QWaitConditionPrivate;
class QMutex;
class QReadWriteLock;

class Q_CORE_EXPORT QWaitCondition
{
public:
    QWaitCondition();
    ~QWaitCondition();

    bool wait(QMutex *mutex, unsigned long time = ULONG_MAX);
    bool wait(QReadWriteLock *readWriteLock, unsigned long time = ULONG_MAX);

    void wakeOne();
    void wakeAll();

private:
    Q_DISABLE_COPY(QWaitCondition)

    QWaitConditionPrivate * d;
};

#else

class QMutex;
class QReadWriteLock;

class Q_CORE_EXPORT QWaitCondition
{
public:
    QWaitCondition() {}
    ~QWaitCondition() {}

    bool wait(QMutex *mutex, unsigned long time = ULONG_MAX)
    {
        Q_UNUSED(mutex);
        Q_UNUSED(time);
        return true;
    }

    bool wait(QReadWriteLock *readWriteLock, unsigned long time = ULONG_MAX)
    {
        Q_UNUSED(readWriteLock);
        Q_UNUSED(time);
        return true;
    }

    void wakeOne() {}
    void wakeAll() {}
};

#endif // QT_NO_THREAD

QT_END_NAMESPACE


#endif // QWAITCONDITION_H
