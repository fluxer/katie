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

#ifndef QTHREADPOOL_P_H
#define QTHREADPOOL_P_H

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
//

#include "QtCore/qmutex.h"
#include "QtCore/qwaitcondition.h"
#include "QtCore/qset.h"
#include "QtCore/qqueue.h"
#include "qobject_p.h"

#ifndef QT_NO_THREAD

QT_BEGIN_NAMESPACE

class QThreadPoolThread;
class QThreadPoolPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QThreadPool)
    friend class QThreadPoolThread;

public:
    QThreadPoolPrivate();

    bool tryStart(QRunnable *task);
    void enqueueTask(QRunnable *task, int priority = 0);
    int activeThreadCount() const;

    void tryToStartMoreThreads();
    bool tooManyThreadsActive() const;

    void startThread(QRunnable *runnable);
    void reset();
    bool waitForDone(int msecs = -1);
    void stealRunnable(QRunnable *);

    QMutex mutex;
    QSet<QThreadPoolThread *> allThreads;
    QQueue<QThreadPoolThread *> waitingThreads;
    QQueue<QThreadPoolThread *> expiredThreads;
    QList<QPair<QRunnable *, int> > queue;
    QWaitCondition noActiveThreads;

    bool isExiting;
    int expiryTimeout;
    int maxThreadCount;
    int reservedThreads;
    int activeThreads;
};

QT_END_NAMESPACE

#endif // QT_NO_THREAD
#endif
