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

#include "qsocketnotifier.h"
#include "qplatformdefs.h"
#include "qabstracteventdispatcher.h"
#include "qcoreapplication.h"

#include "qobject_p.h"
#include "qthread_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QSocketNotifier
    \brief The QSocketNotifier class provides support for monitoring
    activity on a file descriptor.

    \ingroup network
    \ingroup io

    The QSocketNotifier makes it possible to integrate Qt's event
    loop with other event loops based on file descriptors. For
    example, the \l{CORBA Framework} uses it to process CORBA
    events.  File descriptor action is detected in Qt's main event
    loop (QCoreApplication::exec()).

    \target write notifiers

    Once you have opened a device using a low-level (usually
    platform-specific) API, you can create a socket notifier to
    monitor the file descriptor. The socket notifier is enabled by
    default, i.e. it emits the activated() signal whenever a socket
    event corresponding to its type occurs. Connect the activated()
    signal to the slot you want to be called when an event
    corresponding to your socket notifier's type occurs.

    There are three types of socket notifiers: read, write, and
    exception. The type is described by the \l Type enum, and must be
    specified when constructing the socket notifier. After
    construction it can be determined using the type() function. Note
    that if you need to monitor both reads and writes for the same
    file descriptor, you must create two socket notifiers. Note also
    that it is not possible to install two socket notifiers of the
    same type (\l Read, \l Write, \l Exception) on the same socket.

    The setEnabled() function allows you to disable as well as enable
    the socket notifier. It is generally advisable to explicitly
    enable or disable the socket notifier, especially for write
    notifiers. A disabled notifier ignores socket events (the same
    effect as not creating the socket notifier). Use the isEnabled()
    function to determine the notifier's current status.

    Finally, you can use the socket() function to retrieve the
    socket identifier.  Although the class is called QSocketNotifier,
    it is normally used for other types of devices than sockets.
    QTcpSocket and QUdpSocket provide notification through signals, so
    there is normally no need to use a QSocketNotifier on them.

    \sa QFile, QProcess, QTcpSocket, QUdpSocket
*/

/*!
    \enum QSocketNotifier::Type

    This enum describes the various types of events that a socket
    notifier can recognize. The type must be specified when
    constructing the socket notifier.

    Note that if you need to monitor both reads and writes for the
    same file descriptor, you must create two socket notifiers. Note
    also that it is not possible to install two socket notifiers of
    the same type (Read, Write, Exception) on the same socket.

    \value Read      There is data to be read.
    \value Write      Data can be written.
    \value Exception  An exception has occurred. We recommend against using this.

    \sa QSocketNotifier(), type()
*/

/*!
    Constructs a socket notifier with the given \a parent. It enables
    the \a socket, and watches for events of the given \a type.

    It is generally advisable to explicitly enable or disable the
    socket notifier, especially for write notifiers.

    \sa setEnabled(), isEnabled()
*/

QSocketNotifier::QSocketNotifier(int socket, Type type, QObject *parent)
    : QObject(parent)
{
    sockfd = socket;
    sntype = type;
    snenabled = true;

    Q_D(QObject);
    if (Q_UNLIKELY(socket < 0))
        qWarning("QSocketNotifier: Invalid socket specified");
    else if (Q_UNLIKELY(!d->threadData->eventDispatcher))
        qWarning("QSocketNotifier: Can only be used with threads started with QThread");
    else
        d->threadData->eventDispatcher->registerSocketNotifier(this);
}

/*!
    Destroys this socket notifier.
*/

QSocketNotifier::~QSocketNotifier()
{
    setEnabled(false);
}


/*!
    \fn void QSocketNotifier::activated(int socket)

    This signal is emitted whenever the socket notifier is enabled and
    a socket event corresponding to its \l {Type}{type} occurs.

    The socket identifier is passed in the \a socket parameter.

    \sa type(), socket()
*/


/*!
    \fn int QSocketNotifier::socket() const

    Returns the socket identifier specified to the constructor.

    \sa type()
*/

/*!
    \fn Type QSocketNotifier::type() const

    Returns the socket event type specified to the constructor.

    \sa socket()
*/


/*!
    \fn bool QSocketNotifier::isEnabled() const

    Returns true if the notifier is enabled; otherwise returns false.

    \sa setEnabled()
*/

/*!
    If \a enable is true, the notifier is enabled; otherwise the notifier
    is disabled.

    The notifier is enabled by default, i.e. it emits the activated()
    signal whenever a socket event corresponding to its
    \l{type()}{type} occurs. If it is disabled, it ignores socket
    events (the same effect as not creating the socket notifier).

    Write notifiers should normally be disabled immediately after the
    activated() signal has been emitted

    \sa isEnabled(), activated()
*/

void QSocketNotifier::setEnabled(bool enable)
{
    if (sockfd < 0)
        return;
    if (snenabled == enable)                        // no change
        return;
    snenabled = enable;

    Q_D(QObject);
    if (!d->threadData->eventDispatcher) // perhaps application/thread is shutting down
        return;
    if (snenabled)
        d->threadData->eventDispatcher->registerSocketNotifier(this);
    else
        d->threadData->eventDispatcher->unregisterSocketNotifier(this);
}


/*!\reimp
*/
bool QSocketNotifier::event(QEvent *e)
{
    if (e->type() == QEvent::ThreadChange) {
        if (snenabled) {
            QMetaObject::invokeMethod(this, "setEnabled", Qt::QueuedConnection,
                                      Q_ARG(bool, snenabled));
            setEnabled(false);
        }
        return true;
    } else if (e->type() == QEvent::SockAct) {
        emit activated(sockfd);
        return true;
    }

    return QObject::event(e);
}

#include "moc_qsocketnotifier.h"

QT_END_NAMESPACE
