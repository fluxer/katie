/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtTest module of the Katie Toolkit.
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

#ifndef QTESTEVENTLOOP_H
#define QTESTEVENTLOOP_H

#include <QtTest/qtest_global.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qeventloop.h>
#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>


QT_BEGIN_NAMESPACE

class Q_TEST_EXPORT QTestEventLoop : public QObject
{
    Q_OBJECT

public:
    inline QTestEventLoop(QObject *aParent = 0)
        : QObject(aParent), _timeout(false), timerId(-1) {}
    inline void enterLoop(int secs);


    inline void changeInterval(int secs)
    { killTimer(timerId); timerId = startTimer(secs * 1000); }

    inline bool timeout() const
    { return _timeout; }

    inline static QTestEventLoop &instance()
    {
        static QPointer<QTestEventLoop> testLoop;
        if (testLoop.isNull())
            testLoop = new QTestEventLoop(QCoreApplication::instance());
        return *static_cast<QTestEventLoop *>(testLoop);
    }

public Q_SLOTS:
    inline void exitLoop();

protected:
    inline void timerEvent(QTimerEvent *e);

private:
    bool _timeout;
    int timerId;

    QEventLoop loop;
};

inline void QTestEventLoop::enterLoop(int secs)
{
    Q_ASSERT(!loop.isRunning());

    _timeout = false;
    timerId = startTimer(secs * 1000);

    loop.exec();
}

inline void QTestEventLoop::exitLoop()
{
    if (timerId != -1)
        killTimer(timerId);

    timerId = -1;

    loop.exit();
}

inline void QTestEventLoop::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerId) {
        _timeout = true;
        exitLoop();
    }
}

QT_END_NAMESPACE


#endif
