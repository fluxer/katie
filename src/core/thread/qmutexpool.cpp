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

Q_GLOBAL_STATIC(QRecursiveMutexPool, globalMutexPool)

/*!
    Returns a QMutex from the global mutex pool.
*/
template<>
std::recursive_mutex *QRecursiveMutexPool::globalInstanceGet(const void *address)
{
    QRecursiveMutexPool * const globalInstance = globalMutexPool();
    if (!globalInstance)
        return nullptr;
    return globalInstance->get(address);
}

QT_END_NAMESPACE

#endif // QT_NO_THREAD
