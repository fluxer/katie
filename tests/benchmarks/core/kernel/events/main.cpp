/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtCore>

#include <qtest.h>
#include <qtesteventloop.h>

QT_USE_NAMESPACE

class PingPong : public QObject
{
public:
    void setPeer(QObject *peer);
    void resetCounter() {m_counter = 100;}

protected:
    bool event(QEvent *e);

private:
    QObject *m_peer;
    int m_counter;
};

void PingPong::setPeer(QObject *peer)
{
    m_peer = peer;
    m_counter = 100;
}

bool PingPong::event(QEvent *)
{
    --m_counter;
    if (m_counter > 0) {
        QEvent *e = new QEvent(QEvent::User);
        QCoreApplication::postEvent(m_peer, e);
    } else {
        QTestEventLoop::instance().exitLoop();
    }
    return true;
}

class EventTester : public QObject
{
public:
    int foo(int bar);

protected:
    bool event(QEvent *e);
};

bool EventTester::event(QEvent *e)
{
    if (e->type() == QEvent::User+1)
        return foo(e->type()) != 0;
    return false;
}

int EventTester::foo(int bar)
{
    return bar + 1;
}

class EventsBench : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void noEvent();
    void sendEvent_data();
    void sendEvent();
    void postEvent_data();
    void postEvent();
};

void EventsBench::initTestCase()
{
}

void EventsBench::cleanupTestCase()
{
}

void EventsBench::noEvent()
{
    EventTester tst;
    int val = 0;
    QBENCHMARK {
        val += tst.foo(1);
    }
}

void EventsBench::sendEvent_data()
{
    QTest::addColumn<bool>("filterEvents");
    QTest::newRow("no eventfilter") << false;
    QTest::newRow("eventfilter") << true;
}

void EventsBench::sendEvent()
{
    QFETCH(bool, filterEvents);
    EventTester tst;
    if (filterEvents)
        tst.installEventFilter(this);
    QEvent evt(QEvent::Type(QEvent::User+1));
    QBENCHMARK {
        QCoreApplication::sendEvent(&tst, &evt);
    }
}

void EventsBench::postEvent_data()
{
    QTest::addColumn<bool>("filterEvents");
    // The first time an eventloop is executed, the case runs radically slower at least
    // on some platforms, so test the "no eventfilter" case to get a comparable results
    // with the "eventfilter" case.
    QTest::newRow("first time, no eventfilter") << false;
    QTest::newRow("no eventfilter") << false;
    QTest::newRow("eventfilter") << true;
}

void EventsBench::postEvent()
{
    QFETCH(bool, filterEvents);
    PingPong ping;
    PingPong pong;
    ping.setPeer(&pong);
    pong.setPeer(&ping);
    if (filterEvents) {
        ping.installEventFilter(this);
        pong.installEventFilter(this);
    }

    QBENCHMARK {
        // In case multiple iterations are done, event needs to be created inside the QBENCHMARK,
        // or it gets deleted once first iteration exits and can cause a crash. Similarly,
        // ping and pong need their counters reset.
        QEvent *e = new QEvent(QEvent::User);
        ping.resetCounter();
        pong.resetCounter();
        QCoreApplication::postEvent(&ping, e);
        QTestEventLoop::instance().enterLoop( 61 );
    }
}

QTEST_MAIN(EventsBench)

#include "moc_main.cpp"
