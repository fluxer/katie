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

#ifndef QMUTEXPOOL_P_H
#define QMUTEXPOOL_P_H

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

#include "QtCore/qatomic.h"
#include "QtCore/qmutex.h"

#include <array>

#ifndef QT_NO_THREAD

#define QMUTEXPOOL_SIZE 131

QT_BEGIN_NAMESPACE

class Q_CORE_EXPORT QMutexPool
{
public:
    explicit QMutexPool(QMutex::RecursionMode recursionMode = QMutex::NonRecursive);
    ~QMutexPool();

    inline QMutex *get(const void *address) {
        int index = std::intptr_t(address) % mutexes.size();
        QMutex *m = mutexes[index];
        if (m)
            return m;
        return createMutex(index);
    }

    static QMutex *globalInstanceGet(const void *address);

private:
    QMutex *createMutex(int index);
    std::array<QAtomicPointer<QMutex>, QMUTEXPOOL_SIZE> mutexes;
    const QMutex::RecursionMode recursionMode;

    Q_DISABLE_COPY(QMutexPool)
};

QT_END_NAMESPACE

#endif // QT_NO_THREAD

#endif // QMUTEXPOOL_P_H
