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

#ifndef QREADWRITELOCK_P_H
#define QREADWRITELOCK_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the implementation.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qhash.h>

#ifndef QT_NO_THREAD

QT_BEGIN_NAMESPACE

struct QReadWriteLockPrivate
{
    QReadWriteLockPrivate()
        : accessCount(0),
        waitingReaders(0),
        waitingWriters(0)
    { }

    QMutex mutex;
    QWaitCondition readerWait;
    QWaitCondition writerWait;

    int accessCount;
    int waitingReaders;
    int waitingWriters;
};

QT_END_NAMESPACE

#endif // QT_NO_THREAD

#endif // QREADWRITELOCK_P_H
