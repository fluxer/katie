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

#include <qatomic.h>
#include <qcoreapplication.h>
#include <qdatetime.h>
#include <qmutex.h>
#include <qthread.h>
#include <qwaitcondition.h>
#include <qsemaphore.h>

//TESTED_CLASS=
//TESTED_FILES=

class tst_QMutex : public QObject
{
    Q_OBJECT

public:
    tst_QMutex();
    virtual ~tst_QMutex();

private slots:
    void tryLock();
    void lock_unlock_locked_tryLock();
    void stressTest();
    void tryLockRace();
    void qtbug16115_trylock();
};

static const int iterations = 100;

tst_QMutex::tst_QMutex()

{
}

tst_QMutex::~tst_QMutex()
{
}

QAtomicInt lockCount(0);
QMutex normalMutex;
QSemaphore testsTurn;
QSemaphore threadsTurn;

void tst_QMutex::tryLock()
{
    class Thread : public QThread
    {
    public:
        void run()
        {
            testsTurn.release();

            threadsTurn.acquire();
            QVERIFY(!normalMutex.tryLock());
            testsTurn.release();

            threadsTurn.acquire();
            QVERIFY(normalMutex.tryLock());
            QVERIFY(lockCount.testAndSetRelaxed(0, 1));
            QVERIFY(!normalMutex.tryLock());
            QVERIFY(lockCount.testAndSetRelaxed(1, 0));
            normalMutex.unlock();
            testsTurn.release();

            threadsTurn.acquire();
            QElapsedTimer timer;
            timer.start();
            QVERIFY(!normalMutex.tryLock(1000));
            QVERIFY(timer.elapsed() >= 1000);
            testsTurn.release();

            threadsTurn.acquire();
            timer.start();
            QVERIFY(normalMutex.tryLock(1000));
            QVERIFY(timer.elapsed() <= 1000);
            QVERIFY(lockCount.testAndSetRelaxed(0, 1));
            timer.start();
            QVERIFY(!normalMutex.tryLock(1000));
            QVERIFY(timer.elapsed() >= 1000);
            QVERIFY(lockCount.testAndSetRelaxed(1, 0));
            normalMutex.unlock();
            testsTurn.release();

            threadsTurn.acquire();
            QVERIFY(!normalMutex.tryLock(0));
            testsTurn.release();

            threadsTurn.acquire();
            timer.start();
            QVERIFY(normalMutex.tryLock(0));
            QVERIFY(timer.elapsed() < 1000);
            QVERIFY(lockCount.testAndSetRelaxed(0, 1));
            QVERIFY(!normalMutex.tryLock(0));
            QVERIFY(lockCount.testAndSetRelaxed(1, 0));
            normalMutex.unlock();
            testsTurn.release();

            threadsTurn.acquire();
        }
    };

    Thread thread;
    thread.start();

    // thread can't acquire lock
    testsTurn.acquire();
    normalMutex.lock();
    QVERIFY(lockCount.testAndSetRelaxed(0, 1));
    threadsTurn.release();

    // thread can acquire lock
    testsTurn.acquire();
    QVERIFY(lockCount.testAndSetRelaxed(1, 0));
    normalMutex.unlock();
    threadsTurn.release();

    // thread can't acquire lock, timeout = 1000
    testsTurn.acquire();
    normalMutex.lock();
    QVERIFY(lockCount.testAndSetRelaxed(0, 1));
    threadsTurn.release();

    // thread can acquire lock, timeout = 1000
    testsTurn.acquire();
    QVERIFY(lockCount.testAndSetRelaxed(1, 0));
    normalMutex.unlock();
    threadsTurn.release();

    // thread can't acquire lock, timeout = 0
    testsTurn.acquire();
    normalMutex.lock();
    QVERIFY(lockCount.testAndSetRelaxed(0, 1));
    threadsTurn.release();

    // thread can acquire lock, timeout = 0
    testsTurn.acquire();
    QVERIFY(lockCount.testAndSetRelaxed(1, 0));
    normalMutex.unlock();
    threadsTurn.release();

    // wait for thread to finish
    testsTurn.acquire();
    threadsTurn.release();
    thread.wait();
}

class mutex_Thread : public QThread
{
public:
    QMutex mutex;
    QWaitCondition cond;

    QMutex &test_mutex;

    inline mutex_Thread(QMutex &m) : test_mutex(m) { }

    void run()
    {
	test_mutex.lock();

	mutex.lock();
	for (int i = 0; i < iterations; ++i) {
	    cond.wakeOne();
	    cond.wait(&mutex);
	}
	mutex.unlock();

    	test_mutex.unlock();
    }
};

void tst_QMutex::lock_unlock_locked_tryLock()
{
    // normal mutex
    QMutex mutex;
    mutex_Thread thread(mutex);

    for (int i = 0; i < iterations; ++i) {
	// normal mutex
	QVERIFY(mutex.tryLock());
	mutex.unlock();

	thread.mutex.lock();
	thread.start();

	for (int j = 0; j < iterations; ++j) {
	    QVERIFY(thread.cond.wait(&thread.mutex, 10000));
	    QVERIFY(!mutex.tryLock());

	    thread.cond.wakeOne();
	}

	thread.mutex.unlock();

	QVERIFY(thread.wait(10000));
	QVERIFY(mutex.tryLock());

	mutex.unlock();
    }
}

enum { one_minute = 60 * 1000, threadCount = 10 };

class StressTestThread : public QThread
{
    QElapsedTimer t;
public:
    static QAtomicInt lockCount;
    static QAtomicInt sentinel;
    static QMutex mutex;
    static int errorCount;
    void start()
    {
        t.start();
        QThread::start();
    }
    void run()
    {
        while (t.elapsed() < one_minute) {
            mutex.lock();
            if (sentinel.ref()) ++errorCount;
            if (!sentinel.deref()) ++errorCount;
            lockCount.ref();
            mutex.unlock();
            if (mutex.tryLock()) {
                if (sentinel.ref()) ++errorCount;
                if (!sentinel.deref()) ++errorCount;
                lockCount.ref();
                mutex.unlock();
            }
        }
    }
};
QMutex StressTestThread::mutex;
QAtomicInt StressTestThread::lockCount = QAtomicInt(0);
QAtomicInt StressTestThread::sentinel = QAtomicInt(-1);
int StressTestThread::errorCount = 0;

void tst_QMutex::stressTest()
{
    StressTestThread threads[threadCount];
    for (int i = 0; i < threadCount; ++i)
        threads[i].start();
    QVERIFY(threads[0].wait(one_minute + 10000));
    for (int i = 1; i < threadCount; ++i)
        QVERIFY(threads[i].wait(10000));
    QCOMPARE(StressTestThread::errorCount, 0);
    qDebug("locked %d times", int(StressTestThread::lockCount));
}

class TryLockRaceThread : public QThread
{
public:
    static QMutex mutex;

    void run()
    {
        QElapsedTimer t;
        t.start();
        do {
            if (mutex.tryLock())
                mutex.unlock();
        } while (t.elapsed() < 20000);
    }
};
QMutex TryLockRaceThread::mutex;

void tst_QMutex::tryLockRace()
{
    // mutex not in use, should be able to lock it
    QVERIFY(TryLockRaceThread::mutex.tryLock());
    TryLockRaceThread::mutex.unlock();

    // try to break tryLock
    TryLockRaceThread thread[threadCount];
    for (int i = 0; i < threadCount; ++i)
        thread[i].start();
    for (int i = 0; i < threadCount; ++i)
        QVERIFY(thread[i].wait());

    // mutex not in use, should be able to lock it
    QVERIFY(TryLockRaceThread::mutex.tryLock());
    TryLockRaceThread::mutex.unlock();
}

// Variable that will be protected by the mutex. Volatile so that the
// the optimiser doesn't mess with it based on the increment-then-decrement
// usage pattern.
static volatile int qtbug16115_trylock_counter;
// Counter for how many times the protected variable has an incorrect value.
static int qtbug16115_failure_count = 0;

void tst_QMutex::qtbug16115_trylock()
{
    //Used to deadlock on unix
    struct TrylockThread : QThread {
        TrylockThread(QMutex &mut) : mut(mut) {}
        QMutex &mut;
        void run() {
            for (int i = 0; i < 1000000; ++i) {
                if (mut.tryLock(0)) {
                    if ((++qtbug16115_trylock_counter) != 1)
                        ++qtbug16115_failure_count;
                    if ((--qtbug16115_trylock_counter) != 0)
                        ++qtbug16115_failure_count;
                    mut.unlock();
                }
            }
        }
    };
    QMutex mut;
    TrylockThread t1(mut);
    TrylockThread t2(mut);
    TrylockThread t3(mut);
    t1.start();
    t2.start();
    t3.start();

    for (int i = 0; i < 1000000; ++i) {
        mut.lock();
        if ((++qtbug16115_trylock_counter) != 1)
            ++qtbug16115_failure_count;
        if ((--qtbug16115_trylock_counter) != 0)
            ++qtbug16115_failure_count;
        mut.unlock();
    }
    t1.wait();
    t2.wait();
    t3.wait();
    QCOMPARE(qtbug16115_failure_count, 0);
}

QTEST_MAIN(tst_QMutex)

#include "moc_tst_qmutex.cpp"
