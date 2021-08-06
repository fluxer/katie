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

#ifndef QSHAREDMEMORY_P_H
#define QSHAREDMEMORY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qsharedmemory.h"

#ifdef QT_NO_SHAREDMEMORY
# ifndef QT_NO_SYSTEMSEMAPHORE
namespace QSharedMemoryPrivate
{
    QString makePlatformSafeKey(const QString &key, ,
            const QString &prefix = QLatin1String("/qipc_sharedmemory_"));
}
#endif
#else

#include "qsystemsemaphore.h"
#include "qobject_p.h"

#include <sys/types.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_SYSTEMSEMAPHORE
/*!
  Helper class
  */
class QSharedMemoryLocker
{

public:
    inline QSharedMemoryLocker(QSharedMemory *sharedMemory) : q_sm(sharedMemory)
    {
        Q_ASSERT(q_sm);
    }

    inline ~QSharedMemoryLocker()
    {
        if (q_sm)
            q_sm->unlock();
    }

    inline bool lock()
    {
        if (q_sm && q_sm->lock())
            return true;
        q_sm = nullptr;
        return false;
    }

private:
    QSharedMemory *q_sm;
};
#endif // QT_NO_SYSTEMSEMAPHORE

class Q_AUTOTEST_EXPORT QSharedMemoryPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QSharedMemory)

public:
    QSharedMemoryPrivate();

    void *memory;
    int size;
    QString key;
    QString nativeKey;
    QSharedMemory::SharedMemoryError error;
    QString errorString;
#ifndef QT_NO_SYSTEMSEMAPHORE
    QSystemSemaphore systemSemaphore;
    bool lockedByMe;
#endif

    static QString makePlatformSafeKey(const QString &key,
                   const QString &prefix = QLatin1String("/qipc_sharedmemory_"));
    int handle();
    bool initKey();
    void cleanHandle();
    bool create(int size);
    bool attach(QSharedMemory::AccessMode mode);
    bool detach();

    void setErrorString(const QString &function);

#ifndef QT_NO_SYSTEMSEMAPHORE
    inline bool tryLocker(QSharedMemoryLocker *locker, const QString &function) {
        if (!locker->lock()) {
            errorString = QSharedMemory::tr("%1: unable to lock").arg(function);
            error = QSharedMemory::LockError;
            return false;
        }
        return true;
    }
#endif // QT_NO_SYSTEMSEMAPHORE

private:
    int hand;
};

QT_END_NAMESPACE

#endif // QT_NO_SHAREDMEMORY

#endif // QSHAREDMEMORY_P_H

