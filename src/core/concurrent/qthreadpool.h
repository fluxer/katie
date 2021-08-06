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

#ifndef QTHREADPOOL_H
#define QTHREADPOOL_H

#include <QtCore/qthread.h>
#include <QtCore/qrunnable.h>

#ifndef QT_NO_THREAD

QT_BEGIN_NAMESPACE

class QThreadPoolPrivate;
class Q_CORE_EXPORT QThreadPool : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QThreadPool)
    Q_PROPERTY(int expiryTimeout READ expiryTimeout WRITE setExpiryTimeout)
    Q_PROPERTY(int maxThreadCount READ maxThreadCount WRITE setMaxThreadCount)
    Q_PROPERTY(int activeThreadCount READ activeThreadCount)
    friend class QFutureInterfaceBase;

public:
    QThreadPool(QObject *parent = nullptr);
    ~QThreadPool();

    static QThreadPool *globalInstance();

    void start(QRunnable *runnable, int priority = 0);
    bool tryStart(QRunnable *runnable);

    int expiryTimeout() const;
    void setExpiryTimeout(int expiryTimeout);

    int maxThreadCount() const;
    void setMaxThreadCount(int maxThreadCount);

    int activeThreadCount() const;

    void reserveThread();
    void releaseThread();

    void waitForDone();
    bool waitForDone(int msecs);
};

QT_END_NAMESPACE

#endif // QT_NO_THREAD

#endif
