/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/QtCore>
#include <QtTest/QtTest>

#include <math.h>

QT_USE_NAMESPACE

class tst_QWaitCondition : public QObject
{
    Q_OBJECT

public:
    tst_QWaitCondition()
    {
    }

private slots:
    void oscillate_data();
    void oscillate();

    void thrash_data();
    void thrash();

public:
    static QWaitCondition local, remote;
    enum Turn {LocalTurn, RemoteTurn};
    static Turn turn;
};

QWaitCondition tst_QWaitCondition::local;
QWaitCondition tst_QWaitCondition::remote;
tst_QWaitCondition::Turn tst_QWaitCondition::turn = tst_QWaitCondition::LocalTurn;

class OscillateThread : public QThread
{
public:
    bool m_done;
    unsigned long m_timeout;
    bool m_wakeOne;
    int count;

    OscillateThread(unsigned long timeout, bool wakeOne)
    : m_done(false), m_timeout(timeout), m_wakeOne(wakeOne)
        {}
    void run()
    {
        QMutex mtx;
        count = 0;

        forever {
            if (m_done)
                break;
            mtx.lock();
            while (tst_QWaitCondition::turn == tst_QWaitCondition::LocalTurn)
                tst_QWaitCondition::remote.wait(&mtx, m_timeout);
            mtx.unlock();
            tst_QWaitCondition::turn = tst_QWaitCondition::LocalTurn;
            if (m_wakeOne)
                tst_QWaitCondition::local.wakeOne();
            else
                tst_QWaitCondition::local.wakeAll();
            count++;
        }
    }
};

void tst_QWaitCondition::oscillate_data()
{
    QTest::addColumn<unsigned long>("timeout");
    QTest::addColumn<bool>("wakeOne");

    QTest::newRow("mutex, timeout, one") << 1000ul << true;
    QTest::newRow("mutex, timeout, all") << 1000ul << false;
    QTest::newRow("mutex, forever, one") << ULONG_MAX << true;
    QTest::newRow("mutex, forever, all") << ULONG_MAX << false;
}

void tst_QWaitCondition::oscillate()
{
    QMutex mtx;

    QFETCH(unsigned long, timeout);
    QFETCH(bool, wakeOne);

    turn = LocalTurn;
    OscillateThread thrd(timeout, wakeOne);
    thrd.start();

    QBENCHMARK {
        mtx.lock();
        turn = RemoteTurn;
        if (wakeOne)
            remote.wakeOne();
        else
            remote.wakeAll();
        while (turn == RemoteTurn)
            local.wait(&mtx, timeout);
        mtx.unlock();
    }

    thrd.m_done = true;
    remote.wakeAll();
    thrd.wait();

    QCOMPARE(0, 0);
}

void tst_QWaitCondition::thrash_data()
{
    oscillate_data();
}

void tst_QWaitCondition::thrash()
{
    QMutex mtx;
    mtx.lock();

    QFETCH(unsigned long, timeout);
    QFETCH(bool, wakeOne);

    turn = LocalTurn;
    OscillateThread thrd(timeout, wakeOne);
    thrd.start();
    local.wait(&mtx, 1000ul);
    mtx.unlock();

    QBENCHMARK {
        turn = RemoteTurn;
        if (wakeOne)
            remote.wakeOne();
        else
            remote.wakeAll();
    }

    thrd.m_done = true;
    turn = RemoteTurn;
    remote.wakeAll();
    thrd.wait();

    QCOMPARE(0, 0);
}

QTEST_MAIN(tst_QWaitCondition)

#include "moc_tst_qwaitcondition.cpp"
