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

#include "qabstracteventdispatcher.h"
#include "qabstracteventdispatcher_p.h"

#include "qthread.h"
#include "qthread_p.h"
#include "qcoreapplication_p.h"

#include <bitset>
#include <mutex>

QT_BEGIN_NAMESPACE

// 2048 pre-allocated should be more than enough for application, IDs rearely go beyond single
// digit. could implement fallback to int-sized set if limit is reached
enum { QTimerMax = 2048 };

class QTimersSet : public std::bitset<QTimerMax>
{
public:
    void unsetBit(const int bit);
    int freeBit();
private:
    std::mutex m_mutex;
};

void QTimersSet::unsetBit(const int bit)
{
    std::lock_guard<std::mutex> locker(m_mutex);
    this->set(bit, false);
}

int QTimersSet::freeBit()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    // 0 is invalid timer ID
    for (qint16 i = 1; i < QTimerMax; i++) {
        if (!this->test(i)) {
            this->set(i, true);
            return i;
        }
    }
    return 0;
}

static QTimersSet timerIds;

void QAbstractEventDispatcherPrivate::init()
{
    Q_Q(QAbstractEventDispatcher);
    if (Q_UNLIKELY(threadData->eventDispatcher)) {
        qWarning("QAbstractEventDispatcher: An event dispatcher has already been created for this thread");
    } else {
        threadData->eventDispatcher = q;
    }
}

// Timer IDs are implemented using a free-list
int QAbstractEventDispatcherPrivate::allocateTimerId()
{
    const int id = timerIds.freeBit();
    if (Q_UNLIKELY(id == 0)) {
        qFatal("QAbstractEventDispatcher: no free timer ID");
    }
    return id;
}

// Releasing a timer ID requires only a bit flip to mark it as unused
void QAbstractEventDispatcherPrivate::releaseTimerId(int timerId)
{
    Q_ASSERT_X(timerIds.test(timerId), "QAbstractEventDispatcher::releaseTimerId",
               "Internal error: timer ID not found");
    timerIds.unsetBit(timerId);
}

/*!
    \class QAbstractEventDispatcher
    \brief The QAbstractEventDispatcher class provides an interface to manage Qt's event queue.

    \ingroup events

    An event dispatcher receives events from the window system and other
    sources. It then sends them to the QCoreApplication or QApplication
    instance for processing and delivery. QAbstractEventDispatcher provides
    fine-grained control over event delivery.

    For simple control of event processing use
    QCoreApplication::processEvents().

    For finer control of the application's event loop, call
    instance() and call functions on the QAbstractEventDispatcher
    object that is returned. If you want to use your own instance of
    QAbstractEventDispatcher or of a QAbstractEventDispatcher
    subclass, you must create your instance \e before you create the
    QApplication object.

    The main event loop is started by calling
    QCoreApplication::exec(), and stopped by calling
    QCoreApplication::exit(). Local event loops can be created using
    QEventLoop.

    Programs that perform long operations can call processEvents()
    with a bitwise OR combination of various QEventLoop::ProcessEventsFlag
    values to control which events should be delivered.

    QAbstractEventDispatcher also allows the integration of an
    external event loop with the Qt event loop. For example, the
    \l{Motif Extension}
    includes a reimplementation of QAbstractEventDispatcher that merges Qt and
    Motif events together.

    \sa QEventLoop, QCoreApplication
*/

/*!
    Constructs a new event dispatcher with the given \a parent.
*/
QAbstractEventDispatcher::QAbstractEventDispatcher(QObject *parent)
    : QObject(*new QAbstractEventDispatcherPrivate, parent)
{
    Q_D(QAbstractEventDispatcher);
    d->init();
}

/*!
    \internal
*/
QAbstractEventDispatcher::QAbstractEventDispatcher(QAbstractEventDispatcherPrivate &dd,
                                                   QObject *parent)
    : QObject(dd, parent)
{
    Q_D(QAbstractEventDispatcher);
    d->init();
}

/*!
    Destroys the event dispatcher.
*/
QAbstractEventDispatcher::~QAbstractEventDispatcher()
{ }

/*!
    Returns a pointer to the event dispatcher object for the specified
    \a thread. If \a thread is zero, the current thread is used. If no
    event dispatcher exists for the specified thread, this function
    returns 0.

    \bold{Note:} If Qt is built without thread support, the \a thread
    argument is ignored.
 */
QAbstractEventDispatcher *QAbstractEventDispatcher::instance(QThread *thread)
{
    QThreadData *data = thread ? QThreadData::get2(thread) : QThreadData::current();
    return data->eventDispatcher;
}

/*!
    \fn bool QAbstractEventDispatcher::processEvents(QEventLoop::ProcessEventsFlags flags)

    Processes pending events that match \a flags until there are no
    more events to process. Returns true if an event was processed;
    otherwise returns false.

    This function is especially useful if you have a long running
    operation and want to show its progress without allowing user
    input; i.e. by using the QEventLoop::ExcludeUserInputEvents flag.

    If the QEventLoop::WaitForMoreEvents flag is set in \a flags, the
    behavior of this function is as follows:

    \list

    \i If events are available, this function returns after processing
    them.

    \i If no events are available, this function will wait until more
    are available and return after processing newly available events.

    \endlist

    If the QEventLoop::WaitForMoreEvents flag is not set in \a flags,
    and no events are available, this function will return
    immediately.

    \bold{Note:} This function does not process events continuously; it
    returns after all available events are processed.

    \sa hasPendingEvents()
*/

/*! \fn bool QAbstractEventDispatcher::hasPendingEvents()

    Returns true if there is an event waiting; otherwise returns
    false.
*/

/*!
    \fn void QAbstractEventDispatcher::registerSocketNotifier(QSocketNotifier *notifier)

    Registers \a notifier with the event loop. Subclasses must
    implement this method to tie a socket notifier into another
    event loop.
*/

/*! \fn void QAbstractEventDispatcher::unregisterSocketNotifier(QSocketNotifier *notifier)

    Unregisters \a notifier from the event dispatcher. Subclasses must
    reimplement this method to tie a socket notifier into another
    event loop. Reimplementations must call the base
    implementation.
*/

/*!
    \fn int QAbstractEventDispatcher::registerTimer(int interval, QObject *object)

    Registers a timer with the specified \a interval for the given \a object.
*/
int QAbstractEventDispatcher::registerTimer(int interval, QObject *object)
{
    int id = QAbstractEventDispatcherPrivate::allocateTimerId();
    registerTimer(id, interval, object);
    return id;
}

/*!
    \fn void QAbstractEventDispatcher::registerTimer(int timerId, int interval, QObject *object)

    Register a timer with the specified \a timerId and \a interval for
    the given \a object.
*/

/*!
    \fn bool QAbstractEventDispatcher::unregisterTimer(int timerId)

    Unregisters the timer with the given \a timerId.
    Returns true if successful; otherwise returns false.

    \sa registerTimer(), unregisterTimers()
*/

/*!
    \fn bool QAbstractEventDispatcher::unregisterTimers(QObject *object)

    Unregisters all the timers associated with the given \a object.
    Returns true if all timers were successful removed; otherwise returns false.

    \sa unregisterTimer(), registeredTimers()
*/

/*!
    \fn QList<TimerInfo> QAbstractEventDispatcher::registeredTimers(QObject *object) const

    Returns a list of registered timers for \a object. The timer ID
    is the first member in each pair; the interval is the second.
*/

/*! \fn void QAbstractEventDispatcher::wakeUp()
    \threadsafe

    Wakes up the event loop.

    \sa awake()
*/

/*!
    \fn void QAbstractEventDispatcher::interrupt()

    Interrupts event dispatching; i.e. the event dispatcher will
    return from processEvents() as soon as possible.
*/

/*! \fn void QAbstractEventDispatcher::flush()

    Flushes the event queue. This normally returns almost
    immediately. Does nothing on platforms other than X11.
*/

/*!
    \typedef QAbstractEventDispatcher::TimerInfo

    Typedef for QPair<int, int>. The first component of
    the pair is the timer ID; the second component is
    the interval.

    \sa registeredTimers()
*/

/*!
    \typedef QAbstractEventDispatcher::EventFilter

    Typedef for a function with the signature

    \snippet doc/src/snippets/code/src_corelib_kernel_qabstracteventdispatcher.cpp 0

    Note that the type of the \a message is platform dependent. The
    following table shows the \a {message}'s type on UNIX. You can
    do a static cast to these types.

    \table
        \header
            \o Platform
            \o type
        \row
            \o X11
            \o XEvent
    \endtable

    

    \sa setEventFilter(), filterEvent()
*/

/*!
    Replaces the event filter function for this
    QAbstractEventDispatcher with \a filter and returns the replaced
    event filter function. Only the current event filter function is
    called. If you want to use both filter functions, save the
    replaced EventFilter in a place where yours can call it.

    The event filter function set here is called for all messages
    taken from the system event loop before the event is dispatched to
    the respective target, including the messages not meant for Qt
    objects.

    The event filter function should return true if the message should
    be filtered, (i.e. stopped). It should return false to allow
    processing the message to continue.

    By default, no event filter function is set (i.e., this function
    returns a null EventFilter the first time it is called).
*/
QAbstractEventDispatcher::EventFilter QAbstractEventDispatcher::setEventFilter(EventFilter filter)
{
    Q_D(QAbstractEventDispatcher);
    EventFilter oldFilter = d->event_filter;
    d->event_filter = filter;
    return oldFilter;
}

/*!
    Sends \a message through the event filter that was set by
    setEventFilter().  If no event filter has been set, this function
    returns false; otherwise, this function returns the result of the
    event filter function.

    Subclasses of QAbstractEventDispatcher \e must call this function
    for \e all messages received from the system to ensure
    compatibility with any extensions that may be used in the
    application.

    Note that the type of \a message is platform dependent. See 
    QAbstractEventDispatcher::EventFilter for details.

    \sa setEventFilter()
*/
bool QAbstractEventDispatcher::filterEvent(void *message)
{
    Q_D(QAbstractEventDispatcher);
    if (d->event_filter) {
        // Raise the loopLevel so that deleteLater() calls in or triggered
        // by event_filter() will be processed from the main event loop.
        QScopedLoopLevelCounter loopLevelCounter(d->threadData);
        return d->event_filter(message);
    }
    return false;
}

/*! \fn void QAbstractEventDispatcher::awake()

    This signal is emitted after the event loop returns from a
    function that could block.

    \sa wakeUp() aboutToBlock()
*/

/*! \fn void QAbstractEventDispatcher::aboutToBlock()

    This signal is emitted before the event loop calls a function that
    could block.

    \sa awake()
*/

QT_END_NAMESPACE

#include "moc_qabstracteventdispatcher.h"

