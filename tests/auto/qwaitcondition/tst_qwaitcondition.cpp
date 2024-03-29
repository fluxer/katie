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


#include <QtTest/QtTest>

#include <qcoreapplication.h>
#include <qmutex.h>
#include <qthread.h>
#include <qwaitcondition.h>

#define COND_WAIT_TIME 1


//TESTED_CLASS=
//TESTED_FILES=

class tst_QWaitCondition : public QObject
{
    Q_OBJECT

public:
    tst_QWaitCondition();

private slots:
    void wait_QMutex();
    void wakeOne();
    void wakeAll();
    void wait_RaceCondition();
};

static const int iterations = 10;

// Note: some tests rely on ThreadCount being multiple of 2
#if defined(Q_OS_SOLARIS) || (defined(Q_OS_LINUX) && defined(QT_ARCH_ARM))
static const int ThreadCount = 4;
#else
static const int ThreadCount = 10;
#endif

tst_QWaitCondition::tst_QWaitCondition()

{
}

class wait_QMutex_Thread_1 : public QThread
{
public:
    QMutex mutex;
    QWaitCondition cond;

    inline wait_QMutex_Thread_1()
    { }

    void run()
    {
    mutex.lock();
    cond.wakeOne();
    cond.wait(&mutex);
    mutex.unlock();
    }
};

class wait_QMutex_Thread_2 : public QThread
{
public:
    QWaitCondition started;

    QMutex *mutex;
    QWaitCondition *cond;

    inline wait_QMutex_Thread_2()
    : mutex(0), cond(0)
    { }

    void run()
    {
    mutex->lock();
    started.wakeOne();
    cond->wait(mutex);
    mutex->unlock();
    }
};

void tst_QWaitCondition::wait_QMutex()
{
    int x;
    for (int i = 0; i < iterations; ++i) {
    {
        QMutex mutex;
        QWaitCondition cond;

        mutex.lock();

        cond.wakeOne();
        QVERIFY(!cond.wait(&mutex, 1));

        cond.wakeAll();
        QVERIFY(!cond.wait(&mutex, 1));

        mutex.unlock();
    }

    {
        // test multiple threads waiting on separate wait conditions
        wait_QMutex_Thread_1 thread[ThreadCount];

        for (x = 0; x < ThreadCount; ++x) {
        thread[x].mutex.lock();
        thread[x].start();
        // wait for thread to start
        QVERIFY(thread[x].cond.wait(&thread[x].mutex, 1000));
        thread[x].mutex.unlock();
        }

        for (x = 0; x < ThreadCount; ++x) {
        QVERIFY(thread[x].isRunning());
        QVERIFY(!thread[x].isFinished());
        }

        for (x = 0; x < ThreadCount; ++x) {
        thread[x].mutex.lock();
        thread[x].cond.wakeOne();
        thread[x].mutex.unlock();
        }

        for (x = 0; x < ThreadCount; ++x) {
        QVERIFY(thread[x].wait(1000));
        }
    }

    {
        // test multiple threads waiting on a wait condition
        QMutex mutex;
        QWaitCondition cond1, cond2;
        wait_QMutex_Thread_2 thread[ThreadCount];

        mutex.lock();
        for (x = 0; x < ThreadCount; ++x) {
        thread[x].mutex = &mutex;
        thread[x].cond = (x < ThreadCount / 2) ? &cond1 : &cond2;
        thread[x].start();
        // wait for thread to start
        QVERIFY(thread[x].started.wait(&mutex, 1000));
        }
        mutex.unlock();

        for (x = 0; x < ThreadCount; ++x) {
        QVERIFY(thread[x].isRunning());
        QVERIFY(!thread[x].isFinished());
        }

        mutex.lock();
        cond1.wakeAll();
        cond2.wakeAll();
        mutex.unlock();

        for (x = 0; x < ThreadCount; ++x) {
        QVERIFY(thread[x].wait(1000));
        }
    }
    }
}

class wake_Thread : public QThread
{
public:
    static int count;

    QWaitCondition started;
    QWaitCondition dummy;

    QMutex *mutex;
    QWaitCondition *cond;

    inline wake_Thread()
    : mutex(0), cond(0)
    { }

    static inline void sleep(ulong s)
    { QThread::sleep(s); }

    void run()
    {
    mutex->lock();
    ++count;
        dummy.wakeOne(); // this wakeup should be lost
           started.wakeOne();
        dummy.wakeAll(); // this one too
    cond->wait(mutex);
        --count;
    mutex->unlock();
    }
};

int wake_Thread::count = 0;

void tst_QWaitCondition::wakeOne()
{
    int x;
    // wake up threads, one at a time
    for (int i = 0; i < iterations; ++i) {
    QMutex mutex;
    QWaitCondition cond;

    // QMutex
    wake_Thread thread[ThreadCount];
    bool thread_exited[ThreadCount];

    mutex.lock();
    for (x = 0; x < ThreadCount; ++x) {
        thread[x].mutex = &mutex;
        thread[x].cond = &cond;
        thread_exited[x] = false;
        thread[x].start();
        // wait for thread to start
        QVERIFY(thread[x].started.wait(&mutex, 1000));
        // make sure wakeups are not queued... if nothing is
        // waiting at the time of the wakeup, nothing happens
        QVERIFY(!thread[x].dummy.wait(&mutex, 1));
    }
    mutex.unlock();

    QCOMPARE(wake_Thread::count, ThreadCount);

    // wake up threads one at a time
    for (x = 0; x < ThreadCount; ++x) {
        mutex.lock();
        cond.wakeOne();
        QVERIFY(!cond.wait(&mutex, COND_WAIT_TIME));
        QVERIFY(!thread[x].dummy.wait(&mutex, 1));
        mutex.unlock();

        int exited = 0;
        for (int y = 0; y < ThreadCount; ++y) {
            if (thread_exited[y])
                        continue;
            if (thread[y].wait(exited > 0 ? 3 : 1000)) {
                thread_exited[y] = true;
                ++exited;
            }
        }

        QCOMPARE(exited, 1);
        QCOMPARE(wake_Thread::count, ThreadCount - (x + 1));
    }

    QCOMPARE(wake_Thread::count, 0);
    }

    // wake up threads, two at a time
    for (int i = 0; i < iterations; ++i) {
    QMutex mutex;
    QWaitCondition cond;

        // QMutex
    wake_Thread thread[ThreadCount];
    bool thread_exited[ThreadCount];

    mutex.lock();
    for (x = 0; x < ThreadCount; ++x) {
        thread[x].mutex = &mutex;
        thread[x].cond = &cond;
        thread_exited[x] = false;
        thread[x].start();
        // wait for thread to start
        QVERIFY(thread[x].started.wait(&mutex, 1000));
        // make sure wakeups are not queued... if nothing is
        // waiting at the time of the wakeup, nothing happens
        QVERIFY(!thread[x].dummy.wait(&mutex, 1));
    }
    mutex.unlock();

    QCOMPARE(wake_Thread::count, ThreadCount);

    // wake up threads one at a time
    for (x = 0; x < ThreadCount; x += 2) {
        mutex.lock();
        cond.wakeOne();
        cond.wakeOne();
        QVERIFY(!cond.wait(&mutex, COND_WAIT_TIME));
        QVERIFY(!thread[x].dummy.wait(&mutex, 1));
        QVERIFY(!thread[x + 1].dummy.wait(&mutex, 1));
        mutex.unlock();

        int exited = 0;
        for (int y = 0; y < ThreadCount; ++y) {
            if (thread_exited[y])
                        continue;
            if (thread[y].wait(exited > 0 ? 3 : 1000)) {
                thread_exited[y] = true;
                ++exited;
            }
        }

        QCOMPARE(exited, 2);
        QCOMPARE(wake_Thread::count, ThreadCount - (x + 2));
    }

    QCOMPARE(wake_Thread::count, 0);
    }
}

void tst_QWaitCondition::wakeAll()
{
    int x;
    for (int i = 0; i < iterations; ++i) {
    QMutex mutex;
    QWaitCondition cond;

    // QMutex
    wake_Thread thread[ThreadCount];

    mutex.lock();
    for (x = 0; x < ThreadCount; ++x) {
        thread[x].mutex = &mutex;
        thread[x].cond = &cond;
        thread[x].start();
        // wait for thread to start
        QVERIFY(thread[x].started.wait(&mutex, 1000));
    }
    mutex.unlock();

    QCOMPARE(wake_Thread::count, ThreadCount);

    // wake up all threads at once
    mutex.lock();
    cond.wakeAll();
    QVERIFY(!cond.wait(&mutex, COND_WAIT_TIME));
    mutex.unlock();

    int exited = 0;
    for (x = 0; x < ThreadCount; ++x) {
        if (thread[x].wait(1000))
        ++exited;
    }

    QCOMPARE(exited, ThreadCount);
    QCOMPARE(wake_Thread::count, 0);
    }
}

class wait_RaceConditionThread : public QThread
{
public:
    wait_RaceConditionThread(QMutex *mutex, QWaitCondition *startup, QWaitCondition *waitCondition,
                             ulong timeout = ULONG_MAX)
        : timeout(timeout), returnValue(false), ready(false),
          mutex(mutex), startup(startup), waitCondition(waitCondition) {}

    unsigned long timeout;
    bool returnValue;

    bool ready;

    QMutex *mutex;
    QWaitCondition *startup;
    QWaitCondition *waitCondition;

    void run() {
        mutex->lock();

        ready = true;
        startup->wakeOne();

        returnValue = waitCondition->wait(mutex, timeout);

        mutex->unlock();
    }
};

void tst_QWaitCondition::wait_RaceCondition()
{
    {
        QMutex mutex;
        QWaitCondition startup;
        QWaitCondition waitCondition;

        wait_RaceConditionThread timeoutThread(&mutex, &startup, &waitCondition, 1000),
            waitingThread1(&mutex, &startup, &waitCondition);

        timeoutThread.start();
        waitingThread1.start();
        mutex.lock();

        // wait for the threads to start up
        while (!timeoutThread.ready
               || !waitingThread1.ready) {
            startup.wait(&mutex);
        }

        QTest::qWait(2000);

        waitCondition.wakeOne();

        mutex.unlock();

        QVERIFY(timeoutThread.wait(5000));
        QVERIFY(!timeoutThread.returnValue);
        QVERIFY(waitingThread1.wait(5000));
        QVERIFY(waitingThread1.returnValue);
    }
}

QTEST_MAIN(tst_QWaitCondition)

#include "moc_tst_qwaitcondition.cpp"
