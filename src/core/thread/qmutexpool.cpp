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

#include "qatomic.h"
#include "qmutexpool_p.h"

#ifndef QT_NO_THREAD

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(QMutexPool, globalMutexPool, (QMutex::Recursive))

/*!
    \class QMutexPool
    \brief The QMutexPool class provides a pool of QMutex objects.

    \internal

    \ingroup thread

    QMutexPool is a convenience class that provides access to a fixed
    number of QMutex objects.

    Typical use of a QMutexPool is in situations where it is not
    possible or feasible to use one QMutex for every protected object.
    The mutex pool will return a mutex based on the address of the
    object that needs protection.

    For example, consider this simple class:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutexpool.cpp 0

    Adding a QMutex member to the Number class does not make sense,
    because it is so small. However, in order to ensure that access to
    each Number is protected, you need to use a mutex. In this case, a
    QMutexPool would be ideal.

    Code to calculate the square of a number would then look something
    like this:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutexpool.cpp 1

    This function will safely calculate the square of a number, since
    it uses a mutex from a QMutexPool. The mutex is locked and
    unlocked automatically by the QMutexLocker class. See the
    QMutexLocker documentation for more details.
*/

/*!
    Constructs  a QMutexPool, reserving space for \a size QMutexes. All
    mutexes in the pool are created with \a recursionMode. By default,
    all mutexes are non-recursive.

    The QMutexes are created when needed, and deleted when the
    QMutexPool is destructed.
*/
QMutexPool::QMutexPool(QMutex::RecursionMode recursionMode)
    : recursionMode(recursionMode)
{
    mutexes.fill(Q_NULLPTR);
}

/*!
    Destructs a QMutexPool. All QMutexes that were created by the pool
    are deleted.
*/
QMutexPool::~QMutexPool()
{
    for (int index = 0; index < QMUTEXPOOL_SIZE; ++index) {
        delete mutexes[index];
        mutexes[index] = Q_NULLPTR;
    }
}

/*!
    Returns the global QMutexPool instance.
*/
QMutexPool *QMutexPool::instance()
{
    return globalMutexPool();
}

/*!
    \fn QMutexPool::get(const void *address)
    Returns a QMutex from the pool. QMutexPool uses the value \a address
    to determine which mutex is returned from the pool.
*/

/*! \internal
  create the mutex for the given index
 */
QMutex *QMutexPool::createMutex(int index)
{
    // mutex not created, create one
    QMutex *newMutex = new QMutex(recursionMode);
    if (!mutexes[index].testAndSetOrdered(Q_NULLPTR, newMutex))
        delete newMutex;
    return mutexes[index];
}

/*!
    Returns a QMutex from the global mutex pool.
*/
QMutex *QMutexPool::globalInstanceGet(const void *address)
{
    QMutexPool * const globalInstance = globalMutexPool();
    if (!globalInstance)
        return Q_NULLPTR;
    return globalInstance->get(address);
}

QT_END_NAMESPACE

#endif // QT_NO_THREAD
