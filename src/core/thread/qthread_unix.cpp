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

#include "qthread.h"

#include "qcoreapplication.h"
#include "qthread_p.h"
#include "qdebug.h"
#include "qeventdispatcher_unix_p.h"

#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#if defined(QT_HAVE_PRCTL)
#include <sys/prctl.h>
#endif

#if defined(Q_OS_LINUX) && !defined(SCHED_IDLE)
// from linux/sched.h
# define SCHED_IDLE    5
#endif

#if !defined(Q_OS_OPENBSD) && defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
#define QT_HAS_THREAD_PRIORITY_SCHEDULING
#endif

QT_BEGIN_NAMESPACE

#ifndef QT_NO_THREAD
static thread_local QThreadData *currentThreadData = nullptr;

static pthread_once_t current_thread_data_once = PTHREAD_ONCE_INIT;
static pthread_key_t current_thread_data_key;

static void destroy_current_thread_data(void *p)
{

    // POSIX says the value in our key is set to zero before calling
    // this destructor function, so we need to set it back to the
    // right value...
    pthread_setspecific(current_thread_data_key, p);
    QThreadData *data = static_cast<QThreadData *>(p);
    if (data->isAdopted) {
        QThread *thread = data->thread;
        Q_ASSERT(thread);
        QThreadPrivate *thread_p = static_cast<QThreadPrivate *>(QObjectPrivate::get(thread));
        Q_ASSERT(!thread_p->finished);
        thread_p->finish(thread);
    }
    data->deref();

    // ... but we must reset it to zero before returning so we aren't
    // called again (POSIX allows implementations to call destructor
    // functions repeatedly until all values are zero)
    pthread_setspecific(current_thread_data_key, nullptr);
}

static void create_current_thread_data_key()
{
    pthread_key_create(&current_thread_data_key, destroy_current_thread_data);
}

static void destroy_current_thread_data_key()
{
    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    pthread_key_delete(current_thread_data_key);

    // Reset current_thread_data_once in case we end up recreating
    // the thread-data in the rare case of QObject construction
    // after destroying the QThreadData.
    pthread_once_t pthread_once_init = PTHREAD_ONCE_INIT;
    current_thread_data_once = pthread_once_init;
}
Q_DESTRUCTOR_FUNCTION(destroy_current_thread_data_key)


// Utility function for setting thread specific data.
static void set_thread_data(QThreadData *data)
{
    currentThreadData = data;
    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    pthread_setspecific(current_thread_data_key, data);
}

QThreadData *QThreadData::current()
{
    QThreadData *data = currentThreadData;
    if (!data) {
        data = new QThreadData();
        QT_TRY {
            set_thread_data(data);
            data->thread = new QAdoptedThread(data);
        } QT_CATCH(...) {
            currentThreadData = nullptr;
            pthread_setspecific(current_thread_data_key, nullptr);
            data->deref();
            data = nullptr;
            QT_RETHROW;
        }
        data->deref();

        data->isAdopted = true;
    }
    return data;
}


void QAdoptedThread::init()
{
    Q_D(QThread);
    d->thread_id = pthread_self();
}
#endif // QT_NO_THREAD

/*
   QThreadPrivate
*/
void QThreadPrivate::createEventDispatcher(QThreadData *data)
{
    QMutexLocker l(&data->postEventList.mutex);
    data->eventDispatcher = new QEventDispatcherUNIX;
}

#ifndef QT_NO_THREAD
void *QThreadPrivate::start(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
    pthread_cleanup_push(QThreadPrivate::finish, arg);

    QThread *thr = reinterpret_cast<QThread *>(arg);
    QThreadData *data = QThreadData::get2(thr);

    thr->setPriority(thr->d_func()->priority);

    set_thread_data(data);

    data->ref();
    {
        QMutexLocker locker(&thr->d_func()->mutex);
        data->quitNow = thr->d_func()->exited;
    }

    // ### TODO: allow the user to create a custom event dispatcher
    createEventDispatcher(data);

#if defined(QT_HAVE_PRCTL) || defined(QT_HAVE_PTHREAD_SETNAME_NP)
    // sets the name of the current thread.
    QString objectName = thr->objectName();

    if (Q_LIKELY(objectName.isEmpty()))
        objectName = thr->metaObject()->className();
#if defined(QT_HAVE_PRCTL)
    ::prctl(PR_SET_NAME, (unsigned long)objectName.toLocal8Bit().constData(), 0, 0, 0);
#elif defined(QT_HAVE_PTHREAD_SETNAME_NP) && defined(Q_OS_NETBSD)
    pthread_setname_np(thr->d_func()->thread_id, objectName.toLocal8Bit().constData(), (char*)"%s");
#elif defined(QT_HAVE_PTHREAD_SETNAME_NP)
    pthread_setname_np(thr->d_func()->thread_id, objectName.toLocal8Bit().constData());
#endif
#endif

    emit thr->started();
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    pthread_testcancel();
    thr->run();

    pthread_cleanup_pop(1);

    return nullptr;
}

void QThreadPrivate::finish(void *arg)
{
    QThread *thr = reinterpret_cast<QThread *>(arg);
    QThreadPrivate *d = thr->d_func();

    QMutexLocker locker(&d->mutex);

    d->isInFinish = true;
    d->priority = QThread::InheritPriority;
    bool terminated = d->terminated;
    locker.unlock();
    if (terminated)
        emit thr->terminated();
    emit thr->finished();
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    locker.relock();
    d->terminated = false;

    QAbstractEventDispatcher *eventDispatcher = d->data->eventDispatcher;
    if (eventDispatcher) {
        d->data->eventDispatcher = nullptr;
        locker.unlock();
        delete eventDispatcher;
        locker.relock();
    }

    d->thread_id = 0;
    d->running = false;
    d->finished = true;

    d->isInFinish = false;
    d->thread_done.wakeAll();
}




/**************************************************************************
 ** QThread
 *************************************************************************/

Qt::HANDLE QThread::currentThreadId()
{
    return Qt::HANDLE(pthread_self());
}

int QThread::idealThreadCount()
{
#ifdef _SC_NPROCESSORS_ONLN
    return sysconf(_SC_NPROCESSORS_ONLN);
#else
    return 1;
#endif
}

void QThread::yieldCurrentThread()
{
    sched_yield();
}

void QThread::sleep(unsigned long secs)
{
    struct timespec ts = { time_t(secs), 0 };
    ::nanosleep(&ts, NULL);
}

void QThread::msleep(unsigned long msecs)
{
    struct timespec ts = { time_t(msecs) / 1000, long(msecs % 1000) * 1000000 };
    ::nanosleep(&ts, NULL);
}

void QThread::usleep(unsigned long usecs)
{
    struct timespec ts = { time_t(usecs) / 1000000, long(usecs % 1000000) * 1000 };
    ::nanosleep(&ts, NULL);
}

#ifdef QT_HAS_THREAD_PRIORITY_SCHEDULING
// Does some magic and calculate the Unix scheduler priorities
// sched_policy is IN/OUT: it must be set to a valid policy before calling this function
// sched_priority is OUT only
static bool calculateUnixPriority(int priority, int *sched_policy, int *sched_priority)
{
#ifdef SCHED_IDLE
    if (priority == QThread::IdlePriority) {
        *sched_policy = SCHED_IDLE;
        *sched_priority = 0;
        return true;
    }
    const int lowestPriority = QThread::LowestPriority;
#else
    const int lowestPriority = QThread::IdlePriority;
#endif
    const int highestPriority = QThread::TimeCriticalPriority;

    const int prio_min = sched_get_priority_min(*sched_policy);
    const int prio_max = sched_get_priority_max(*sched_policy);
    if (prio_min == -1 || prio_max == -1)
        return false;

    int prio;
    // crudely scale our priority enum values to the prio_min/prio_max
    prio = ((priority - lowestPriority) * (prio_max - prio_min) / highestPriority) + prio_min;
    prio = qMax(prio_min, qMin(prio_max, prio));

    *sched_priority = prio;
    return true;
}
#endif

void QThread::start(Priority priority)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);

    if (d->isInFinish)
        d->thread_done.wait(locker.mutex());

    if (d->running)
        return;

    d->running = true;
    d->finished = false;
    d->terminated = false;
    d->returnCode = 0;
    d->exited = false;
    d->priority = priority;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (d->stackSize > 0) {
        int code = pthread_attr_setstacksize(&attr, d->stackSize);
        if (Q_UNLIKELY(code)) {
            qWarning("QThread::start: Thread stack size error: %s",
                     qPrintable(qt_error_string(code)));

            // we failed to set the stacksize, and as the documentation states,
            // the thread will fail to run...
            d->running = false;
            d->finished = false;
            return;
        }
    }

    int code = pthread_create(&d->thread_id, &attr, QThreadPrivate::start, this);

    pthread_attr_destroy(&attr);

    if (Q_UNLIKELY(code)) {
        qWarning("QThread::start: Thread creation error: %s", qPrintable(qt_error_string(code)));

        d->running = false;
        d->finished = false;
        d->thread_id = 0;
    }
}

void QThread::terminate()
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);

    if (!d->thread_id)
        return;

    int code = pthread_cancel(d->thread_id);
    if (Q_UNLIKELY(code)) {
        qWarning("QThread::terminate: Thread termination error: %s",
                 qPrintable(qt_error_string(code)));
    } else {
        d->terminated = true;
    }
}

bool QThread::wait(unsigned long time)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);

    if (Q_UNLIKELY(pthread_equal(d->thread_id, pthread_self()))) {
        qWarning("QThread::wait: Thread tried to wait on itself");
        return false;
    }

    if (d->finished)
        return true;

    while (d->running) {
        if (!d->thread_done.wait(locker.mutex(), time))
            return false;
    }
    return true;
}

void QThread::setTerminationEnabled(bool enabled)
{
    Q_ASSERT_X(currentThread() != nullptr, "QThread::setTerminationEnabled()",
               "Current thread was not started with QThread.");

    if (enabled) {
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
        pthread_testcancel();
    } else {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);
    }
}

void QThread::setPriority(Priority priority)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);
    if (Q_UNLIKELY(!d->running)) {
        qWarning("QThread::setPriority: Cannot set priority, thread is not running");
        return;
    }

#ifdef QT_HAS_THREAD_PRIORITY_SCHEDULING
    int sched_policy;
    sched_param param;

    if (Q_UNLIKELY(pthread_getschedparam(d->thread_id, &sched_policy, &param) != 0)) {
        // failed to get the scheduling policy, don't bother setting
        // the priority
        qWarning("QThread::setPriority: Cannot get scheduler parameters");
        return;
    }

    int prio;
    if (Q_UNLIKELY(!calculateUnixPriority(priority, &sched_policy, &prio))) {
        // failed to get the scheduling parameters, don't
        // bother setting the priority
        qWarning("QThread::setPriority: Cannot determine scheduler priority range");
        return;
    }

    param.sched_priority = prio;
    int status = pthread_setschedparam(d->thread_id, sched_policy, &param);

# ifdef SCHED_IDLE
    // were we trying to set to idle priority and failed?
    if (status == -1 && sched_policy == SCHED_IDLE && errno == EINVAL) {
        // reset to lowest priority possible
        pthread_getschedparam(d->thread_id, &sched_policy, &param);
        param.sched_priority = sched_get_priority_min(sched_policy);
        pthread_setschedparam(d->thread_id, sched_policy, &param);
    }
# else
    Q_UNUSED(status);
# endif // SCHED_IDLE
#endif

    d->priority = priority;
}

#include "moc_qthread.h"

#endif // QT_NO_THREAD

QT_END_NAMESPACE

