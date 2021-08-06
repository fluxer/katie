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
#include "qmutex.h"
#include "qmutexpool_p.h"
#include "qreadwritelock.h"
#include "qabstracteventdispatcher.h"
#include "qeventloop.h"
#include "qhash.h"
#include "qthread_p.h"
#include "qcoreapplication_p.h"
#include "qscopedpointer.h"

#include <limits.h> // for PTHREAD_STACK_MIN

QT_BEGIN_NAMESPACE

/*
  QThreadData
*/

QThreadData::QThreadData()
    : quitNow(false), canWait(true), isAdopted(false), loopLevel(0),
    thread(nullptr), eventDispatcher(nullptr), _ref(1)
{
    // fprintf(stderr, "QThreadData %p created\n", this);
}

QThreadData::~QThreadData()
{
    Q_ASSERT(_ref == 0);

    delete thread;
    thread = nullptr;

    for (int i = 0; i < postEventList.size(); ++i) {
        const QPostEvent &pe = postEventList.at(i);
        if (pe.event) {
            --pe.receiver->d_func()->postedEvents;
            pe.event->posted = false;
            delete pe.event;
        }
    }

    // fprintf(stderr, "QThreadData %p destroyed\n", this);
}

void QThreadData::ref()
{
#ifndef QT_NO_THREAD
    (void) _ref.ref();
    Q_ASSERT(_ref != 0);
#endif
}

void QThreadData::deref()
{
#ifndef QT_NO_THREAD
    if (!_ref.deref())
        delete this;
#endif
}

/*
  QAdoptedThread
*/

QAdoptedThread::QAdoptedThread(QThreadData *data)
    : QThread(*new QThreadPrivate(data))
{
    // thread should be running and not finished for the lifetime
    // of the application (even if QCoreApplication goes away)
#ifndef QT_NO_THREAD
    d_func()->running = true;
    d_func()->finished = false;
    init();
#endif

    // fprintf(stderr, "new QAdoptedThread = %p\n", this);
}

QAdoptedThread::~QAdoptedThread()
{
    // fprintf(stderr, "~QAdoptedThread = %p\n", this);
}

void QAdoptedThread::run()
{
    // this function should never be called
    qFatal("QAdoptedThread::run(): Internal error, this implementation should never be called.");
}
#ifndef QT_NO_THREAD
/*
  QThreadPrivate
*/

QThreadPrivate::QThreadPrivate(QThreadData *d)
    : QObjectPrivate(), running(false), finished(false), terminated(false),
      isInFinish(false), exited(false), returnCode(-1),
      stackSize(0), priority(QThread::InheritPriority), thread_id(0), data(d)
{
}

QThreadPrivate::~QThreadPrivate()
{
    data->deref();
}

/*!
    \class QThread
    \brief The QThread class provides a platform-independent way to
    manage threads.

    \ingroup thread

    A QThread object manages one thread of control within the
    program. QThreads begin executing in run(). By default, run() starts the
    event loop by calling exec() and runs a Qt event loop inside the thread.

    You can use worker objects by moving them to the thread using
    QObject::moveToThread().

    \snippet doc/src/snippets/code/src_corelib_thread_qthread.cpp worker

    The code inside the Worker's slot would then execute in a
    separate thread. However, you are free to connect the
    Worker's slots to any signal, from any object, in any thread. It
    is safe to connect signals and slots across different threads,
    thanks to a mechanism called \l{Qt::QueuedConnection}{queued
    connections}.

    Another way to make code run in a separate thread, is to subclass QThread
    and reimplement run(). For example:

    \snippet doc/src/snippets/code/src_corelib_thread_qthread.cpp reimpl-run

    In that example, the thread will exit after the run function has returned.
    There will not be any event loop running in the thread unless you call
    exec().

    It is important to remember that a QThread instance \l{QObject#Thread
    Affinity}{lives in} the old thread that instantiated it, not in the
    new thread that calls run(). This means that all of QThread's queued
    slots will execute in the old thread. Thus, a developer who wishes to
    invoke slots in the new thread must use the worker-object approach; new
    slots should not be implemented directly into a subclassed QThread.

    When subclassing QThread, keep in mind that the constructor executes in
    the old thread while run() executes in the new thread. If a member
    variable is accessed from both functions, then the variable is accessed
    from two different threads. Check that it is safe to do so.

    \note Care must be taken when interacting with objects across different
    threads. See \l{Synchronizing Threads} for details.

    \section1 Managing threads

    QThread will notifiy you via a signal
    when the thread is started(), finished(), and terminated(), or
    you can use isFinished() and isRunning() to query the state of
    the thread.

    You can stop the thread by calling exit() or quit(). In extreme
    cases, you may want to forcibly terminate() an executing thread.
    However, doing so is dangerous and discouraged. Please read the
    documentation for terminate() and setTerminationEnabled() for
    detailed information.

    From Qt 4.8 onwards, it is possible to deallocate objects that
    live in a thread that has just ended, by connecting the
    finished() signal to QObject::deleteLater().

    Use wait() to block the calling thread, until the other thread
    has finished execution (or until a specified time has passed).

    The static functions currentThreadId() and currentThread() return
    identifiers for the currently executing thread. The former
    returns a platform specific ID for the thread; the latter returns
    a QThread pointer.

    To choose the name that your thread will be given (as identified
    by the command \c{ps -L} on Linux, for example), you can call
    \l{QObject::setObjectName()}{setObjectName()} before starting the thread.
    If you don't call \l{QObject::setObjectName()}{setObjectName()},
    the name given to your thread will be the class name of the runtime
    type of your thread object (for example, \c "RenderThread" in the case of the
    \l{Mandelbrot Example}, as that is the name of the QThread subclass).
    Note that this is currently not available with release builds on Windows.

    QThread also provides static, platform independent sleep
    functions: sleep(), msleep(), and usleep() allow full second,
    millisecond, and microsecond resolution respectively.

    \note wait() and the sleep() functions should be unnecessary in
    general, since Qt is an event-driven framework. Instead of
    wait(), consider listening for the finished() signal. Instead of
    the sleep() functions, consider using QTimer.

    \sa {Thread Support in Qt}, {Synchronizing Threads}, {Mandelbrot Example},
        {Semaphores Example}, {Wait Conditions Example}
*/

/*!
    \fn Qt::HANDLE QThread::currentThreadId()

    Returns the thread handle of the currently executing thread.

    \warning The handle returned by this function is used for internal
    purposes and should not be used in any application code.
*/

/*!
    \fn int QThread::idealThreadCount()

    Returns the ideal number of threads that can be run on the system. This is done querying
    the number of processor cores, both real and logical, in the system. This function returns -1
    if the number of processor cores could not be detected.
*/

/*!
    \fn void QThread::yieldCurrentThread()

    Yields execution of the current thread to another runnable thread,
    if any. Note that the operating system decides to which thread to
    switch.
*/

/*!
    \fn void QThread::start(Priority priority)

    Begins execution of the thread by calling run(). The
    operating system will schedule the thread according to the \a
    priority parameter. If the thread is already running, this
    function does nothing.

    The effect of the \a priority parameter is dependent on the
    operating system's scheduling policy. In particular, the \a priority
    will be ignored on systems that do not support thread priorities
    (such as on Linux, see http://linux.die.net/man/2/sched_setscheduler
    for more details).

    \sa run(), terminate()
*/

/*!
    \fn void QThread::started()

    This signal is emitted when the thread starts executing.

    \sa finished(), terminated()
*/

/*!
    \fn void QThread::finished()

    This signal is emitted when the thread has finished executing.

    \sa started(), terminated()
*/

/*!
    \fn void QThread::terminated()

    This signal is emitted when the thread is terminated.

    \sa started(), finished()
*/

/*!
    \enum QThread::Priority

    This enum type indicates how the operating system should schedule
    newly created threads.

    \value IdlePriority scheduled only when no other threads are
           running.

    \value LowestPriority scheduled less often than LowPriority.
    \value LowPriority scheduled less often than NormalPriority.

    \value NormalPriority the default priority of the operating
           system.

    \value HighPriority scheduled more often than NormalPriority.
    \value HighestPriority scheduled more often than HighPriority.

    \value TimeCriticalPriority scheduled as often as possible.

    \value InheritPriority use the same priority as the creating
           thread. This is the default.
*/

/*!
    Returns a pointer to a QThread which manages the currently
    executing thread.
*/
QThread *QThread::currentThread()
{
    QThreadData *data = QThreadData::current();
    Q_ASSERT(data);
    return data->thread;
}

/*!
    Constructs a new QThread to manage a new thread. The \a parent
    takes ownership of the QThread. The thread does not begin
    executing until start() is called.

    \sa start()
*/
QThread::QThread(QObject *parent)
    : QObject(*(new QThreadPrivate(new QThreadData())), parent)
{
    Q_D(QThread);
    // fprintf(stderr, "QThreadData %p created for thread %p\n", d->data, this);
    d->data->thread = this;
}

/*! \internal
 */
QThread::QThread(QThreadPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(QThread);
    // fprintf(stderr, "QThreadData %p taken from private data for thread %p\n", d->data, this);
    d->data->thread = this;
}

/*!
    Destroys the QThread.

    Note that deleting a QThread object will not stop the execution
    of the thread it manages. Deleting a running QThread (i.e.
    isFinished() returns false) will probably result in a program
    crash. Wait for the finished() signal before deleting the
    QThread.
*/
QThread::~QThread()
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);
    if (d->isInFinish) {
        locker.unlock();
        wait();
        locker.relock();
    }
    if (Q_UNLIKELY(d->running && !d->finished && !d->data->isAdopted))
        qWarning("QThread: Destroyed while thread is still running");

    d->data->thread = nullptr;
}

/*!
    Returns true if the thread is finished; otherwise returns false.

    \sa isRunning()
*/
bool QThread::isFinished() const
{
    Q_D(const QThread);
    return d->finished || d->isInFinish;
}

/*!
    Returns true if the thread is running; otherwise returns false.

    \sa isFinished()
*/
bool QThread::isRunning() const
{
    Q_D(const QThread);
    return d->running && !d->isInFinish;
}

/*!
    Sets the maximum stack size for the thread to \a stackSize. If \a
    stackSize is greater than zero, the maximum stack size is set to
    \a stackSize bytes, otherwise the maximum stack size is
    automatically determined by the operating system.

    \warning Most operating systems place minimum and maximum limits
    on thread stack sizes. The thread will fail to start if the stack
    size is outside these limits.

    \sa stackSize()
*/
void QThread::setStackSize(uint stackSize)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);
    Q_ASSERT_X(!d->running, "QThread::setStackSize",
               "cannot change stack size while the thread is running");
#ifdef PTHREAD_STACK_MIN
    static long stack_min = sysconf(_SC_THREAD_STACK_MIN);
    if (stack_min == -1)
        stack_min = PTHREAD_STACK_MIN;
    // 0 means default stack size
    if (Q_UNLIKELY(stackSize != 0 && stackSize < stack_min)) {
        qWarning("QThread::setStackSize: %u is less than the minimum %ld", stackSize, stack_min);
        stackSize = stack_min;
    }
#endif
    d->stackSize = stackSize;
}

/*!
    Returns the maximum stack size for the thread (if set with
    setStackSize()); otherwise returns zero.

    \sa setStackSize()
*/
uint QThread::stackSize() const
{
    Q_D(const QThread);
    return d->stackSize;
}

/*!
    Enters the event loop and waits until exit() is called, returning the value
    that was passed to exit(). The value returned is 0 if exit() is called via
    quit().

    This function is meant to be called from within run(). It is necessary to
    call this function to start event handling.

    \sa quit(), exit()
*/
int QThread::exec()
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);
    d->data->quitNow = false;
    if (d->exited) {
        d->exited = false;
        return d->returnCode;
    }
    locker.unlock();

    QEventLoop eventLoop;
    int returnCode = eventLoop.exec();

    locker.relock();
    d->exited = false;
    d->returnCode = -1;
    return returnCode;
}

/*!
    Tells the thread's event loop to exit with a return code.

    After calling this function, the thread leaves the event loop and
    returns from the call to QEventLoop::exec(). The
    QEventLoop::exec() function returns \a returnCode.

    By convention, a \a returnCode of 0 means success, any non-zero value
    indicates an error.

    Note that unlike the C library function of the same name, this
    function \e does return to the caller -- it is event processing
    that stops. 
    
    No QEventLoops will be started anymore in this thread  until 
    QThread::exec() has been called again. If the eventloop in QThread::exec()
    is not running then the next call to QThread::exec() will also return
    immediately.

    \sa quit() QEventLoop
*/
void QThread::exit(int returnCode)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);
    d->exited = true;
    d->returnCode = returnCode;
    d->data->quitNow = true;
    foreach (QEventLoop *eventLoop, d->data->eventLoops) {
        eventLoop->exit(returnCode);
    }
}

/*!
    Tells the thread's event loop to exit with return code 0 (success).
    Equivalent to calling QThread::exit(0).

    This function does nothing if the thread does not have an event
    loop.

    \sa exit() QEventLoop
*/
void QThread::quit()
{
    QThread::exit();
}

/*!
    The starting point for the thread. After calling start(), the
    newly created thread calls this function. The default
    implementation simply calls exec().

    You can reimplement this function to facilitate advanced thread
    management. Returning from this method will end the execution of
    the thread.

    \sa start() wait()
*/
void QThread::run()
{
    (void) QThread::exec();
}

/*!
    \fn bool QThread::finished() const

    Use isFinished() instead.
*/

/*!
    \fn bool QThread::running() const

    Use isRunning() instead.
*/

/*! \fn void QThread::setPriority(Priority priority)
    \since 4.1

    This function sets the \a priority for a running thread. If the
    thread is not running, this function does nothing and returns
    immediately.  Use start() to start a thread with a specific
    priority.

    The \a priority argument can be any value in the \c
    QThread::Priority enum except for \c InheritPriorty.

    The effect of the \a priority parameter is dependent on the
    operating system's scheduling policy. In particular, the \a priority
    will be ignored on systems that do not support thread priorities
    (such as on Linux, see http://linux.die.net/man/2/sched_setscheduler
    for more details).

    \sa Priority priority() start()
*/

/*!
    \since 4.1

    Returns the priority for a running thread.  If the thread is not
    running, this function returns \c InheritPriority.

    \sa Priority setPriority() start()
*/
QThread::Priority QThread::priority() const
{
    Q_D(const QThread);
    return d->priority;
}

/*!
    \fn void QThread::sleep(unsigned long secs)

    Forces the current thread to sleep for \a secs seconds.

    \sa msleep(), usleep()
*/

/*!
    \fn void QThread::msleep(unsigned long msecs)

    Forces the current thread to sleep for \a msecs milliseconds.

    \sa sleep(), usleep()
*/

/*!
    \fn void QThread::usleep(unsigned long usecs)

    Forces the current thread to sleep for \a usecs microseconds.

    \sa sleep(), msleep()
*/

/*!
    \fn void QThread::terminate()

    Terminates the execution of the thread. The thread may or may not
    be terminated immediately, depending on the operating system's
    scheduling policies. Listen for the terminated() signal, or use
    QThread::wait() after terminate(), to be sure.

    When the thread is terminated, all threads waiting for the thread
    to finish will be woken up.

    \warning This function is dangerous and its use is discouraged.
    The thread can be terminated at any point in its code path.
    Threads can be terminated while modifying data. There is no
    chance for the thread to clean up after itself, unlock any held
    mutexes, etc. In short, use this function only if absolutely
    necessary.

    Termination can be explicitly enabled or disabled by calling
    QThread::setTerminationEnabled(). Calling this function while
    termination is disabled results in the termination being
    deferred, until termination is re-enabled. See the documentation
    of QThread::setTerminationEnabled() for more information.

    \sa setTerminationEnabled()
*/

/*!
    \fn bool QThread::wait(unsigned long time)

    Blocks the thread until either of these conditions is met:

    \list
    \o The thread associated with this QThread object has finished
       execution (i.e. when it returns from \l{run()}). This function
       will return true if the thread has finished. It also returns
       true if the thread has not been started yet.
    \o \a time milliseconds has elapsed. If \a time is ULONG_MAX (the
        default), then the wait will never timeout (the thread must
        return from \l{run()}). This function will return false if the
        wait timed out.
    \endlist

    This provides similar functionality to the POSIX \c
    pthread_join() function.

    \sa sleep(), terminate()
*/

/*!
    \fn void QThread::setTerminationEnabled(bool enabled)

    Enables or disables termination of the current thread based on the
    \a enabled parameter. The thread must have been started by
    QThread.

    When \a enabled is false, termination is disabled.  Future calls
    to QThread::terminate() will return immediately without effect.
    Instead, the termination is deferred until termination is enabled.

    When \a enabled is true, termination is enabled.  Future calls to
    QThread::terminate() will terminate the thread normally.  If
    termination has been deferred (i.e. QThread::terminate() was
    called with termination disabled), this function will terminate
    the calling thread \e immediately.  Note that this function will
    not return in this case.

    \sa terminate()
*/

#else // QT_NO_THREAD

QThread::QThread(QObject *parent)
    : QObject(*(new QThreadPrivate), nullptr){
    Q_D(QThread);
    d->data->thread = this;
}

QThread *QThread::currentThread()
{
    return QThreadData::current()->thread;
}

static QThreadData *currentdata = nullptr;

QThreadData* QThreadData::current()
{
    if (!currentdata) {
        QScopedPointer<QThreadData> newdata(new QThreadData());
        newdata->thread = nullptr; // new QAdoptedThread(newdata.data());
        currentdata = newdata.take();
        currentdata->deref();
    }
    return currentdata;
}

/*! \internal
 */
QThread::QThread(QThreadPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    Q_D(QThread);
    // fprintf(stderr, "QThreadData %p taken from private data for thread %p\n", d->data, this);
    d->data->thread = this;
}

QThreadPrivate::QThreadPrivate(QThreadData *d)
    : data(d)
{
}

QThreadPrivate::~QThreadPrivate()
{
    delete data;
}

#endif // QT_NO_THREAD

QT_END_NAMESPACE
