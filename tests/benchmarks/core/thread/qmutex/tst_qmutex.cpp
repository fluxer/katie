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
#include <pthread.h>

typedef pthread_mutex_t NativeMutexType;
void NativeMutexInitialize(NativeMutexType *mutex)
{
    pthread_mutex_init(mutex, NULL);
}
void NativeMutexDestroy(NativeMutexType *mutex)
{
    pthread_mutex_destroy(mutex);
}
void NativeMutexLock(NativeMutexType *mutex)
{
    pthread_mutex_lock(mutex);
}
void NativeMutexUnlock(NativeMutexType *mutex)
{
    pthread_mutex_unlock(mutex);
}

QT_USE_NAMESPACE

//TESTED_FILES=

class tst_QMutex : public QObject
{
    Q_OBJECT

    int threadCount;

public:
    // barriers for the contended tests
    static QSemaphore semaphore1, semaphore2, semaphore3, semaphore4;

    tst_QMutex()
    {
        // at least 2 threads, even on single cpu/core machines
        threadCount = qMax(2, QThread::idealThreadCount());
        qDebug("thread count: %d", threadCount);
    }

private slots:
    void noThread_data();
    void noThread();

    void constructionNative();
    void uncontendedNative();
    void constructionQMutex();
    void uncontendedQMutex();
    void uncontendedQMutexLocker();

    void contendedNative_data();
    void contendedQMutex_data() { contendedNative_data(); }
    void contendedQMutexLocker_data() { contendedNative_data(); }

    void contendedNative();
    void contendedQMutex();
    void contendedQMutexLocker();
};

QSemaphore tst_QMutex::semaphore1;
QSemaphore tst_QMutex::semaphore2;
QSemaphore tst_QMutex::semaphore3;
QSemaphore tst_QMutex::semaphore4;

void tst_QMutex::noThread_data()
{
    QTest::addColumn<int>("t");

    QTest::newRow("noLock") << 1;
    QTest::newRow("QMutex") << 2;
    QTest::newRow("QMutexLocker") << 3;
}

void tst_QMutex::noThread()
{
    volatile int count = 0;
    const int N = 5000000;
    QMutex mtx;

    QFETCH(int, t);
    switch(t) {
        case 1:
            QBENCHMARK {
                count = 0;
                for (int i = 0; i < N; i++) {
                    count++;
                }
            }
            break;
        case 2:
            QBENCHMARK {
                count = 0;
                for (int i = 0; i < N; i++) {
                    mtx.lock();
                    count++;
                    mtx.unlock();
                }
            }
            break;
        case 3:
            QBENCHMARK {
                count = 0;
                for (int i = 0; i < N; i++) {
                    QMutexLocker locker(&mtx);
                    count++;
                }
            }
            break;
    }
    QCOMPARE(int(count), N);
}

void tst_QMutex::constructionNative()
{
    QBENCHMARK {
        NativeMutexType mutex;
        NativeMutexInitialize(&mutex);
        NativeMutexDestroy(&mutex);
    }
}

void tst_QMutex::uncontendedNative()
{
    NativeMutexType mutex;
    NativeMutexInitialize(&mutex);
    QBENCHMARK {
        NativeMutexLock(&mutex);
        NativeMutexUnlock(&mutex);
    }
    NativeMutexDestroy(&mutex);
}

void tst_QMutex::constructionQMutex()
{
    QBENCHMARK {
        QMutex mutex;
        Q_UNUSED(mutex);
    }
}

void tst_QMutex::uncontendedQMutex()
{
    QMutex mutex;
    QBENCHMARK {
        mutex.lock();
        mutex.unlock();
    }
}

void tst_QMutex::uncontendedQMutexLocker()
{
    QMutex mutex;
    QBENCHMARK {
        QMutexLocker locker(&mutex);
    }
}

void tst_QMutex::contendedNative_data()
{
    QTest::addColumn<int>("iterations");
    QTest::addColumn<int>("msleepDuration");
    QTest::addColumn<bool>("use2mutexes");

    QTest::newRow("baseline")               <<    0 <<  -1 << false;

    QTest::newRow("no msleep, 1 mutex")     << 1000 <<  -1 << false;
    QTest::newRow("no msleep, 2 mutexes")   << 1000 <<  -1 << true;
    QTest::newRow("msleep(0), 1 mutex")     << 1000 <<   0 << false;
    QTest::newRow("msleep(0), 2 mutexes")   << 1000 <<   0 << true;
    QTest::newRow("msleep(1), 1 mutex")     <<   10 <<   1 << false;
    QTest::newRow("msleep(1), 2 mutexes")   <<   10 <<   1 << true;
    QTest::newRow("msleep(2), 1 mutex")     <<   10 <<   2 << false;
    QTest::newRow("msleep(2), 2 mutexes")   <<   10 <<   2 << true;
    QTest::newRow("msleep(10), 1 mutex")    <<   10 <<  10 << false;
    QTest::newRow("msleep(10), 2 mutexes")  <<   10 <<  10 << true;
}

class NativeMutexThread : public QThread
{
    NativeMutexType *mutex1, *mutex2;
    int iterations, msleepDuration;
    bool use2mutexes;
public:
    bool done;
    NativeMutexThread(NativeMutexType *mutex1, NativeMutexType *mutex2, int iterations, int msleepDuration, bool use2mutexes)
        : mutex1(mutex1), mutex2(mutex2), iterations(iterations), msleepDuration(msleepDuration), use2mutexes(use2mutexes), done(false)
    { }
    void run() {
        forever {
            tst_QMutex::semaphore1.release();
            tst_QMutex::semaphore2.acquire();
            if (done)
                break;
            for (int i = 0; i < iterations; ++i) {
                NativeMutexLock(mutex1);
                if (use2mutexes)
                    NativeMutexLock(mutex2);
                if (msleepDuration >= 0)
                    QThread::msleep(msleepDuration);
                if (use2mutexes)
                    NativeMutexUnlock(mutex2);
                NativeMutexUnlock(mutex1);

                QThread::yieldCurrentThread();
            }
            tst_QMutex::semaphore3.release();
            tst_QMutex::semaphore4.acquire();
        }
    }
};

void tst_QMutex::contendedNative()
{
    QFETCH(int, iterations);
    QFETCH(int, msleepDuration);
    QFETCH(bool, use2mutexes);

    NativeMutexType mutex1, mutex2;
    NativeMutexInitialize(&mutex1);
    NativeMutexInitialize(&mutex2);

    QVector<NativeMutexThread *> threads(threadCount);
    for (int i = 0; i < threads.count(); ++i) {
        threads[i] = new NativeMutexThread(&mutex1, &mutex2, iterations, msleepDuration, use2mutexes);
        threads[i]->start();
    }

    QBENCHMARK {
        semaphore1.acquire(threadCount);
        semaphore2.release(threadCount);
        semaphore3.acquire(threadCount);
        semaphore4.release(threadCount);
    }

    for (int i = 0; i < threads.count(); ++i)
        threads[i]->done = true;
    semaphore1.acquire(threadCount);
    semaphore2.release(threadCount);
    for (int i = 0; i < threads.count(); ++i)
        threads[i]->wait();
    qDeleteAll(threads);

    NativeMutexDestroy(&mutex1);
    NativeMutexDestroy(&mutex2);
}

class QMutexThread : public QThread
{
    QMutex *mutex1, *mutex2;
    int iterations, msleepDuration;
    bool use2mutexes;
public:
    bool done;
    QMutexThread(QMutex *mutex1, QMutex *mutex2, int iterations, int msleepDuration, bool use2mutexes)
        : mutex1(mutex1), mutex2(mutex2), iterations(iterations), msleepDuration(msleepDuration), use2mutexes(use2mutexes), done(false)
    { }
    void run() {
        forever {
            tst_QMutex::semaphore1.release();
            tst_QMutex::semaphore2.acquire();
            if (done)
                break;
            for (int i = 0; i < iterations; ++i) {
                mutex1->lock();
                if (use2mutexes)
                    mutex2->lock();
                if (msleepDuration >= 0)
                    QThread::msleep(msleepDuration);
                if (use2mutexes)
                    mutex2->unlock();
                mutex1->unlock();

                QThread::yieldCurrentThread();
            }
            tst_QMutex::semaphore3.release();
            tst_QMutex::semaphore4.acquire();
        }
    }
};

void tst_QMutex::contendedQMutex()
{
    QFETCH(int, iterations);
    QFETCH(int, msleepDuration);
    QFETCH(bool, use2mutexes);

    QMutex mutex1, mutex2;

    QVector<QMutexThread *> threads(threadCount);
    for (int i = 0; i < threads.count(); ++i) {
        threads[i] = new QMutexThread(&mutex1, &mutex2, iterations, msleepDuration, use2mutexes);
        threads[i]->start();
    }

    QBENCHMARK {
        semaphore1.acquire(threadCount);
        semaphore2.release(threadCount);
        semaphore3.acquire(threadCount);
        semaphore4.release(threadCount);
    }

    for (int i = 0; i < threads.count(); ++i)
        threads[i]->done = true;
    semaphore1.acquire(threadCount);
    semaphore2.release(threadCount);
    for (int i = 0; i < threads.count(); ++i)
        threads[i]->wait();
    qDeleteAll(threads);
}

class QMutexLockerThread : public QThread
{
    QMutex *mutex1, *mutex2;
    int iterations, msleepDuration;
    bool use2mutexes;
public:
    bool done;
    QMutexLockerThread(QMutex *mutex1, QMutex *mutex2, int iterations, int msleepDuration, bool use2mutexes)
        : mutex1(mutex1), mutex2(mutex2), iterations(iterations), msleepDuration(msleepDuration), use2mutexes(use2mutexes), done(false)
    { }
    void run() {
        forever {
            tst_QMutex::semaphore1.release();
            tst_QMutex::semaphore2.acquire();
            if (done)
                break;
            for (int i = 0; i < iterations; ++i) {
                {
                    QMutexLocker locker1(mutex1);
                    QMutexLocker locker2(use2mutexes ? mutex2 : 0);
                    if (msleepDuration >= 0)
                        QThread::msleep(msleepDuration);
                }

                QThread::yieldCurrentThread();
            }
            tst_QMutex::semaphore3.release();
            tst_QMutex::semaphore4.acquire();
        }
    }
};

void tst_QMutex::contendedQMutexLocker()
{
    QFETCH(int, iterations);
    QFETCH(int, msleepDuration);
    QFETCH(bool, use2mutexes);

    QMutex mutex1, mutex2;

    QVector<QMutexLockerThread *> threads(threadCount);
    for (int i = 0; i < threads.count(); ++i) {
        threads[i] = new QMutexLockerThread(&mutex1, &mutex2, iterations, msleepDuration, use2mutexes);
        threads[i]->start();
    }

    QBENCHMARK {
        semaphore1.acquire(threadCount);
        semaphore2.release(threadCount);
        semaphore3.acquire(threadCount);
        semaphore4.release(threadCount);
    }

    for (int i = 0; i < threads.count(); ++i)
        threads[i]->done = true;
    semaphore1.acquire(threadCount);
    semaphore2.release(threadCount);
    for (int i = 0; i < threads.count(); ++i)
        threads[i]->wait();
    qDeleteAll(threads);
}

QTEST_MAIN(tst_QMutex)

#include "moc_tst_qmutex.cpp"
