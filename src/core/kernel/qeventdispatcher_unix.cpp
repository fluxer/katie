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

#include "qplatformdefs.h"
#include "qcoreapplication.h"
#include "qpair.h"
#include "qsocketnotifier.h"
#include "qthread.h"
#include "qelapsedtimer.h"
#include "qeventdispatcher_unix_p.h"
#include "qthread_p.h"
#include "qdebug.h"
#include "qcoreapplication_p.h"
#include "qcore_unix_p.h"
#include "qcorecommon_p.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_DEBUG
static const long maxOpenFiles = sysconf(_SC_OPEN_MAX);
#endif

QEventDispatcherUNIXPrivate::QEventDispatcherUNIXPrivate()
    : interrupt(false)
{
    // initialize the common parts of the event loop
    if (qt_safe_pipe(thread_pipe, O_NONBLOCK) == -1) {
        perror("QEventDispatcherUNIXPrivate(): Unable to create thread pipe");
        qFatal("QEventDispatcherUNIXPrivate(): Can not continue without a thread pipe");
    }
}

QEventDispatcherUNIXPrivate::~QEventDispatcherUNIXPrivate()
{
    // cleanup the common parts of the event loop
    qt_safe_close(thread_pipe[0]);
    qt_safe_close(thread_pipe[1]);

    // cleanup timers
    qDeleteAll(timerList);
}

int QEventDispatcherUNIXPrivate::doSelect(QEventLoop::ProcessEventsFlags flags, int timeout)
{
    Q_Q(QEventDispatcherUNIX);

    // needed in QEventDispatcherUNIX::select()
    timerList.updateCurrentTime();

    QEventDispatcherPollSet pollset;
    pollset.reserve(sn_select.size() + 1);

    struct pollfd tpfds;
    ::memset(&tpfds, 0, sizeof(struct pollfd));
    tpfds.fd = thread_pipe[0];
    tpfds.events = POLLIN;
    pollset.append(tpfds);

    if (! (flags & QEventLoop::ExcludeSocketNotifiers)) {
        foreach (const QSocketNotifier *sn, sn_select) {

            struct pollfd fds;
            ::memset(&fds, 0, sizeof(struct pollfd));
            fds.fd = sn->socket();
            switch (sn->type()) {
                case QSocketNotifier::Read: {
                    fds.events = POLLIN;
                    break;
                }
                case QSocketNotifier::Write: {
                    fds.events = POLLOUT;
                    break;
                }
                case QSocketNotifier::Exception: {
                    fds.events = 0;
                    break;
                }
            }
            pollset.append(fds);
        }
    }

    int nsel = q->select(pollset, timeout);

    if (nsel == -1) {
        QEventDispatcherPollSet::iterator it = (pollset.begin() + 1);
        while (it != pollset.end()) {
            struct pollfd p = (*it);
            if ((p.revents & POLLNVAL) != 0) {
                QSocketNotifier *sn = nullptr;
                foreach (QSocketNotifier *snit, sn_select) {
                    if (snit->socket() == p.fd) {
                        sn = snit;
                        break;
                    }
                }
                Q_ASSERT_X(sn != nullptr, "QSocketNotifier", "Internal error");
                // disable the invalid socket notifier
                static const char *sockTypeString[] = { "Read", "Write", "Exception" };
                qWarning("QSocketNotifier: Invalid socket %d and type '%s', disabling...",
                         sn->socket(), sockTypeString[sn->type()]);
                sn->setEnabled(false);
                it = pollset.erase(it);
                sn_select.removeOne(sn);
                continue;
            }
            it++;
        }
    }

    int nevents = 0;
    if (nsel > 0 && (tpfds.revents & POLLIN) != 0) {
        // some other thread woke us up... consume the data on the thread pipe so that
        // select doesn't immediately return next time
        QSTACKARRAY(char, c, 16);
        while (qt_safe_read(thread_pipe[0], c, sizeof(c)) > 0)
            ;

        if (!wakeUps.testAndSetRelease(1, 0)) {
            // hopefully, this is dead code
            qWarning("QEventDispatcherUNIX: internal error, wakeUps.testAndSetRelease(1, 0) failed!");
        }
        nevents++;
    }

    // if select says data is ready on any socket, then set the socket notifier to pending
    for (int i = 1; i < pollset.size(); i++) {
        struct pollfd p = pollset.at(i);
        QSocketNotifier *sn = sn_select.at(i - 1);
        int checkforevents = 0;
        switch (sn->type()) {
            case QSocketNotifier::Read: {
                checkforevents = POLLIN;
                break;
            }
            case QSocketNotifier::Write: {
                checkforevents = POLLOUT;
                break;
            }
            case QSocketNotifier::Exception: {
                checkforevents = POLLERR;
                break;
            }
        }
        if ((p.revents & checkforevents) != 0) {
            q->setSocketNotifierPending(sn);
        }
    }
    return (nevents + q->activateSocketNotifiers());
}

/*
 * Internal functions for manipulating timer data structures.  The
 * timerBitVec array is used for keeping track of timer identifiers.
 */

QTimerInfoList::QTimerInfoList()
    : QList<QTimerInfo*>(),
    firstTimerInfo(nullptr)
{
    if (Q_LIKELY(QElapsedTimer::isMonotonic())) {
        // detected monotonic timers
        previousTime.tv_sec = previousTime.tv_usec = 0;
        previousTicks = 0;
        ticksPerSecond = 0;
        msPerTick = 0;
    } else {
        // not using monotonic timers, initialize the timeChanged() machinery
        previousTime = qt_gettime();

        struct tms unused;
        previousTicks = ::times(&unused);

        ticksPerSecond = sysconf(_SC_CLK_TCK);
        msPerTick = 1000/ticksPerSecond;
    }
}

timeval QTimerInfoList::updateCurrentTime()
{
    currentTime = qt_gettime();
    return currentTime;
}

timeval qAbsTimeval(const timeval &t)
{
    timeval tmp = t;
    if (tmp.tv_sec < 0) {
        tmp.tv_sec = -tmp.tv_sec - 1;
        tmp.tv_usec -= 1000000;
    }
    if (tmp.tv_sec == 0 && tmp.tv_usec < 0) {
        tmp.tv_usec = -tmp.tv_usec;
    }
    return normalizedTimeval(tmp);
}

/*
  Returns true if the real time clock has changed by more than 10%
  relative to the processor time since the last time this function was
  called. This presumably means that the system time has been changed.

  If /a delta is nonzero, delta is set to our best guess at how much the system clock was changed.
*/
bool QTimerInfoList::timeChanged(timeval *delta)
{
    struct tms unused;
    clock_t currentTicks = times(&unused);

    clock_t elapsedTicks = currentTicks - previousTicks;
    timeval elapsedTime = currentTime - previousTime;

    timeval elapsedTimeTicks;
    elapsedTimeTicks.tv_sec = elapsedTicks / ticksPerSecond;
    elapsedTimeTicks.tv_usec = (((elapsedTicks * 1000) / ticksPerSecond) % 1000) * 1000;

    timeval dummy;
    if (!delta)
        delta = &dummy;
    *delta = elapsedTime - elapsedTimeTicks;

    previousTicks = currentTicks;
    previousTime = currentTime;

    // If tick drift is more than 10% off compared to realtime, we assume that the clock has
    // been set. Of course, we have to allow for the tick granularity as well.
    timeval tickGranularity;
    tickGranularity.tv_sec = 0;
    tickGranularity.tv_usec = msPerTick * 1000;
    return elapsedTimeTicks < ((qAbsTimeval(*delta) - tickGranularity) * 10);
}

void QTimerInfoList::repairTimersIfNeeded()
{
    if (Q_LIKELY(QElapsedTimer::isMonotonic()))
        return;
    timeval delta;
    if (timeChanged(&delta))
        timerRepair(delta);
}

/*
  insert timer info into list
*/
void QTimerInfoList::timerInsert(QTimerInfo *ti)
{
    int index = size();
    while (index--) {
        const QTimerInfo * const t = at(index);
        if (!(ti->timeout < t->timeout))
            break;
    }
    insert(index+1, ti);
}

/*
  repair broken timer
*/
void QTimerInfoList::timerRepair(const timeval &diff)
{
    // repair all timers
    for (int i = 0; i < size(); ++i) {
        QTimerInfo *t = at(i);
        t->timeout = t->timeout + diff;
    }
}

/*
  Returns the time to wait for the next timer, or null if no timers
  are waiting.
*/
bool QTimerInfoList::timerWait(timeval &tm)
{
    timeval currentTime = updateCurrentTime();
    repairTimersIfNeeded();

    // Find first waiting timer not already active
    QTimerInfo *t = nullptr;
    for (QTimerInfoList::const_iterator it = constBegin(); it != constEnd(); ++it) {
        if (!(*it)->activateRef) {
            t = *it;
            break;
        }
    }

    if (!t)
        return false;

    if (currentTime < t->timeout) {
        // time to wait
        tm = t->timeout - currentTime;
    } else {
        // no time to wait
        tm.tv_sec  = 0;
        tm.tv_usec = 0;
    }

    return true;
}

void QTimerInfoList::registerTimer(int timerId, int interval, QObject *object)
{
    QTimerInfo *t = new QTimerInfo;
    t->id = timerId;
    t->interval.tv_sec  = interval / 1000;
    t->interval.tv_usec = (interval % 1000) * 1000;
    t->timeout = updateCurrentTime() + t->interval;
    t->obj = object;
    t->activateRef = nullptr;

    timerInsert(t);
}

bool QTimerInfoList::unregisterTimer(int timerId)
{
    // set timer inactive
    for (int i = 0; i < count(); ++i) {
        QTimerInfo *t = at(i);
        if (t->id == timerId) {
            // found it
            removeAt(i);
            if (t == firstTimerInfo)
                firstTimerInfo = nullptr;
            if (t->activateRef)
                *(t->activateRef) = nullptr;

            // release the timer id
            if (!QObjectPrivate::get(t->obj)->inThreadChangeEvent)
                QAbstractEventDispatcherPrivate::releaseTimerId(timerId);

            delete t;
            return true;
        }
    }
    // id not found
    return false;
}

bool QTimerInfoList::unregisterTimers(QObject *object)
{
    if (isEmpty())
        return false;
    for (int i = 0; i < count(); ++i) {
        QTimerInfo *t = at(i);
        if (t->obj == object) {
            // object found
            removeAt(i);
            if (t == firstTimerInfo)
                firstTimerInfo = nullptr;
            if (t->activateRef)
                *(t->activateRef) = nullptr;

            // release the timer id
            if (!QObjectPrivate::get(t->obj)->inThreadChangeEvent)
                QAbstractEventDispatcherPrivate::releaseTimerId(t->id);

            delete t;
            // move back one so that we don't skip the new current item
            --i;
        }
    }
    return true;
}

QList<QPair<int, int> > QTimerInfoList::registeredTimers(QObject *object) const
{
    QList<QPair<int, int> > list;
    for (int i = 0; i < count(); ++i) {
        const QTimerInfo * const t = at(i);
        if (t->obj == object)
            list << QPair<int, int>(t->id, t->interval.tv_sec * 1000 + t->interval.tv_usec / 1000);
    }
    return list;
}

/*
    Activate pending timers, returning how many where activated.
*/
int QTimerInfoList::activateTimers()
{
    if (isEmpty())
        return 0; // nothing to do

    int n_act = 0, maxCount = 0;
    firstTimerInfo = nullptr;

    timeval currentTime = updateCurrentTime();
    repairTimersIfNeeded();


    // Find out how many timer have expired
    for (QTimerInfoList::const_iterator it = constBegin(); it != constEnd(); ++it) {
        if (currentTime < (*it)->timeout)
            break;
        maxCount++;
    }

    //fire the timers.
    while (maxCount--) {
        if (isEmpty())
            break;

        QTimerInfo *currentTimerInfo = first();
        if (currentTime < currentTimerInfo->timeout)
            break; // no timer has expired

        if (!firstTimerInfo) {
            firstTimerInfo = currentTimerInfo;
        } else if (firstTimerInfo == currentTimerInfo) {
            // avoid sending the same timer multiple times
            break;
        } else if (currentTimerInfo->interval <  firstTimerInfo->interval
                   || currentTimerInfo->interval == firstTimerInfo->interval) {
            firstTimerInfo = currentTimerInfo;
        }

        // remove from list
        removeFirst();

        // determine next timeout time
        currentTimerInfo->timeout += currentTimerInfo->interval;
        if (currentTimerInfo->timeout < currentTime)
            currentTimerInfo->timeout = currentTime + currentTimerInfo->interval;

        // reinsert timer
        timerInsert(currentTimerInfo);
        if (currentTimerInfo->interval.tv_usec > 0 || currentTimerInfo->interval.tv_sec > 0)
            n_act++;

        if (!currentTimerInfo->activateRef) {
            // send event, but don't allow it to recurse
            currentTimerInfo->activateRef = &currentTimerInfo;

            QTimerEvent e(currentTimerInfo->id);
            QCoreApplication::sendEvent(currentTimerInfo->obj, &e);

            if (currentTimerInfo)
                currentTimerInfo->activateRef = nullptr;
        }
    }

    firstTimerInfo = nullptr;
    return n_act;
}

QEventDispatcherUNIX::QEventDispatcherUNIX(QObject *parent)
    : QAbstractEventDispatcher(*new QEventDispatcherUNIXPrivate, parent)
{
}

QEventDispatcherUNIX::QEventDispatcherUNIX(QEventDispatcherUNIXPrivate &dd, QObject *parent)
    : QAbstractEventDispatcher(dd, parent)
{
}

QEventDispatcherUNIX::~QEventDispatcherUNIX()
{
    Q_D(QEventDispatcherUNIX);
    d->threadData->eventDispatcher = nullptr;
}

int QEventDispatcherUNIX::select(QEventDispatcherPollSet &pollset, int timeout)
{
    return qt_safe_poll(pollset.data(), pollset.size(), timeout);
}

/*!
    \internal
*/
void QEventDispatcherUNIX::registerTimer(int timerId, int interval, QObject *obj)
{
#ifndef QT_NO_DEBUG
    if (timerId < 1 || interval < 0 || !obj) {
        qWarning("QEventDispatcherUNIX::registerTimer: invalid arguments");
        return;
    } else if (obj->thread() != thread() || thread() != QThread::currentThread()) {
        qWarning("QObject::startTimer: timers cannot be started from another thread");
        return;
    }
#endif

    Q_D(QEventDispatcherUNIX);
    d->timerList.registerTimer(timerId, interval, obj);
}

/*!
    \internal
*/
bool QEventDispatcherUNIX::unregisterTimer(int timerId)
{
#ifndef QT_NO_DEBUG
    if (timerId < 1) {
        qWarning("QEventDispatcherUNIX::unregisterTimer: invalid argument");
        return false;
    } else if (thread() != QThread::currentThread()) {
        qWarning("QObject::killTimer: timers cannot be stopped from another thread");
        return false;
    }
#endif

    Q_D(QEventDispatcherUNIX);
    return d->timerList.unregisterTimer(timerId);
}

/*!
    \internal
*/
bool QEventDispatcherUNIX::unregisterTimers(QObject *object)
{
#ifndef QT_NO_DEBUG
    if (!object) {
        qWarning("QEventDispatcherUNIX::unregisterTimers: invalid argument");
        return false;
    } else if (object->thread() != thread() || thread() != QThread::currentThread()) {
        qWarning("QObject::killTimers: timers cannot be stopped from another thread");
        return false;
    }
#endif

    Q_D(QEventDispatcherUNIX);
    return d->timerList.unregisterTimers(object);
}

QList<QEventDispatcherUNIX::TimerInfo>
QEventDispatcherUNIX::registeredTimers(QObject *object) const
{
    if (!object) {
        qWarning("QEventDispatcherUNIX:registeredTimers: invalid argument");
        return QList<TimerInfo>();
    }

    Q_D(const QEventDispatcherUNIX);
    return d->timerList.registeredTimers(object);
}

/*****************************************************************************
 QEventDispatcher implementations for UNIX
 *****************************************************************************/

void QEventDispatcherUNIX::registerSocketNotifier(QSocketNotifier *notifier)
{
    Q_ASSERT(notifier);
#ifndef QT_NO_DEBUG
    int sockfd = notifier->socket();
    if (Q_UNLIKELY(sockfd < 0 || sockfd >= maxOpenFiles)) {
        qWarning("QSocketNotifier: Internal error");
        return;
    } else if (Q_UNLIKELY(notifier->thread() != thread() || thread() != QThread::currentThread())) {
        qWarning("QSocketNotifier: socket notifiers cannot be enabled from another thread");
        return;
    }
#endif

    Q_D(QEventDispatcherUNIX);
    d->sn_select.append(notifier);
}

void QEventDispatcherUNIX::unregisterSocketNotifier(QSocketNotifier *notifier)
{
    Q_ASSERT(notifier);
#ifndef QT_NO_DEBUG
    int sockfd = notifier->socket();
    if (Q_UNLIKELY(sockfd < 0 || sockfd >= maxOpenFiles)) {
        qWarning("QSocketNotifier: Internal error");
        return;
    } else if (Q_UNLIKELY(notifier->thread() != thread() || thread() != QThread::currentThread())) {
        qWarning("QSocketNotifier: socket notifiers cannot be disabled from another thread");
        return;
    }
#endif

    Q_D(QEventDispatcherUNIX);
    d->sn_select.removeOne(notifier);
    d->sn_pending.removeOne(notifier);
}

void QEventDispatcherUNIX::setSocketNotifierPending(QSocketNotifier *notifier)
{
    Q_ASSERT(notifier);
#ifndef QT_NO_DEBUG
    int sockfd = notifier->socket();
    if (Q_UNLIKELY(sockfd < 0 || sockfd >= maxOpenFiles)) {
        qWarning("QSocketNotifier: Internal error");
        return;
    }
    Q_ASSERT(notifier->thread() == thread() && thread() == QThread::currentThread());
#endif

    Q_D(QEventDispatcherUNIX);
    d->sn_pending.append(notifier);
}

int QEventDispatcherUNIX::activateTimers()
{
    Q_ASSERT(thread() == QThread::currentThread());
    Q_D(QEventDispatcherUNIX);
    return d->timerList.activateTimers();
}

int QEventDispatcherUNIX::activateSocketNotifiers()
{
    Q_D(QEventDispatcherUNIX);
    if (d->sn_pending.isEmpty())
        return 0;

    // activate entries
    int n_act = 0;
    QEvent event(QEvent::SockAct);
    while (!d->sn_pending.isEmpty()) {
        QSocketNotifier *sn = d->sn_pending.takeFirst();
        QCoreApplication::sendEvent(sn, &event);
        n_act++;
    }
    return n_act;
}

// return true if we handled events, false otherwise
bool QEventDispatcherUNIX::processEvents(QEventLoop::ProcessEventsFlags flags)
{
    Q_D(QEventDispatcherUNIX);
    d->interrupt = false;

    // we are awake, broadcast it
    emit awake();
    QCoreApplicationPrivate::sendPostedEvents(0, 0, d->threadData);

    const bool canWait = (d->threadData->canWaitLocked()
                          && !d->interrupt
                          && (flags & QEventLoop::WaitForMoreEvents));

    if (canWait)
        emit aboutToBlock();

    if (!d->interrupt) {
        // return the maximum time we can wait for an event.
        timeval wait_tm = { 0l, 0l };
        timeval *tm = &wait_tm;
        if (d->timerList.timerWait(wait_tm)) {
            tm = &wait_tm;
        }

        if (!canWait) {
            // no time to wait
            tm->tv_sec  = 0l;
            tm->tv_usec = 0l;
        }
 
        int nevents = d->doSelect(flags, (tm->tv_sec * 1000) + (tm->tv_usec / 1000));

        // activate timers
        nevents += activateTimers();

        return (nevents > 0);
    }

    return false;
}

bool QEventDispatcherUNIX::hasPendingEvents()
{
    QThreadData *currentThreadData = QThreadData::current();
    return currentThreadData->postEventList.size() - currentThreadData->postEventList.startOffset;
}

void QEventDispatcherUNIX::wakeUp()
{
    Q_D(QEventDispatcherUNIX);
    if (d->wakeUps.testAndSetAcquire(0, 1)) {
        char c = 0;
        qt_safe_write( d->thread_pipe[1], &c, 1);
    }
}

void QEventDispatcherUNIX::interrupt()
{
    Q_D(QEventDispatcherUNIX);
    d->interrupt = true;
    wakeUp();
}

void QEventDispatcherUNIX::flush()
{ }

QT_END_NAMESPACE

#include "moc_qeventdispatcher_unix_p.h"
