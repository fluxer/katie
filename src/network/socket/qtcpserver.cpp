/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtNetwork module of the Katie Toolkit.
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

//#define QTCPSERVER_DEBUG

/*! \class QTcpServer

    \brief The QTcpServer class provides a TCP-based server.

    \reentrant
    \ingroup network
    \inmodule QtNetwork

    This class makes it possible to accept incoming TCP connections.
    You can specify the port or have QTcpServer pick one
    automatically. You can listen on a specific address or on all the
    machine's addresses.

    Call listen() to have the server listen for incoming connections.
    The newConnection() signal is then emitted each time a client
    connects to the server.

    Call nextPendingConnection() to accept the pending connection as
    a connected QTcpSocket. The function returns a pointer to a
    QTcpSocket in QAbstractSocket::ConnectedState that you can use for
    communicating with the client.

    If an error occurs, serverError() returns the type of error, and
    errorString() can be called to get a human readable description of
    what happened.

    When listening for connections, the address and port on which the
    server is listening are available as serverAddress() and
    serverPort().

    Calling close() makes QTcpServer stop listening for incoming
    connections.

    Although QTcpServer is mostly designed for use with an event
    loop, it's possible to use it without one. In that case, you must
    use waitForNewConnection(), which blocks until either a
    connection is available or a timeout expires.

    Platform security capabilities are added via the
    \l{qmake-variable-reference.html#target-capability}{TARGET.CAPABILITY}
    qmake variable.

    \sa QTcpSocket, {Fortune Server Example}, {Threaded Fortune Server Example},
        {Loopback Example}, {Torrent Example}
*/

/*! \fn void QTcpServer::newConnection()

    This signal is emitted every time a new connection is available.

    \sa hasPendingConnections(), nextPendingConnection()
*/

#include "qobject_p.h"
#include "qalgorithms.h"
#include "qhostaddress.h"
#include "qpointer.h"
#include "qabstractsocketengine_p.h"
#include "qtcpserver.h"
#include "qtcpsocket.h"
#include "qstdcontainers_p.h"

QT_BEGIN_NAMESPACE

#define Q_CHECK_SOCKETENGINE(returnValue) do { \
    if (!d->socketEngine) { \
        return returnValue; \
    } } while (0)

class QTcpServerPrivate : public QObjectPrivate, public QAbstractSocketEngineReceiver
{
    Q_DECLARE_PUBLIC(QTcpServer)
public:
    QTcpServerPrivate();
    ~QTcpServerPrivate();

    QStdVector<QTcpSocket *> pendingConnections;

    QAbstractSocketEngine *socketEngine;

    int maxConnections;

    // from QAbstractSocketEngineReceiver
    void readNotification();
    inline void writeNotification() {}
    inline void connectionNotification() {}
};

/*! \internal
*/
QTcpServerPrivate::QTcpServerPrivate()
    : socketEngine(nullptr),
    maxConnections(30)
{
}

/*! \internal
*/
QTcpServerPrivate::~QTcpServerPrivate()
{
}

/*! \internal
*/
void QTcpServerPrivate::readNotification()
{
    Q_Q(QTcpServer);
    for (;;) {
        if (pendingConnections.count() >= maxConnections) {
#if defined (QTCPSERVER_DEBUG)
            qDebug("QTcpServerPrivate::_q_processIncomingConnection() too many connections");
#endif
            if (socketEngine->isReadNotificationEnabled())
                socketEngine->setReadNotificationEnabled(false);
            return;
        }

        int descriptor = socketEngine->accept();
        if (descriptor == -1)
            break;
#if defined (QTCPSERVER_DEBUG)
        qDebug("QTcpServerPrivate::_q_processIncomingConnection() accepted socket %i", descriptor);
#endif
        q->incomingConnection(descriptor);

        QPointer<QTcpServer> that = q;
        emit q->newConnection();
        if (!that || !q->isListening())
            return;
    }
}

/*!
    Constructs a QTcpServer object.

    \a parent is passed to the QObject constructor.

    \sa listen(), setSocketDescriptor()
*/
QTcpServer::QTcpServer(QObject *parent)
    : QObject(*new QTcpServerPrivate, parent)
{
}

/*!
    Destroys the QTcpServer object. If the server is listening for
    connections, the socket is automatically closed.

    Any client \l{QTcpSocket}s that are still connected must either
    disconnect or be reparented before the server is deleted.

    \sa close()
*/
QTcpServer::~QTcpServer()
{
    close();
}

/*!
    Tells the server to listen for incoming connections on address \a
    address and port \a port. If \a port is 0, a port is chosen
    automatically. If \a address is QHostAddress::Any, the server
    will listen on all network interfaces.

    Returns true on success; otherwise returns false.

    \sa isListening()
*/
bool QTcpServer::listen(const QHostAddress &address, quint16 port)
{
    Q_D(QTcpServer);
    if (Q_UNLIKELY(isListening())) {
        qWarning("QTcpServer::listen() called when already listening");
        return false;
    }

    delete d->socketEngine;
    d->socketEngine = new QAbstractSocketEngine(this);
    if (!d->socketEngine->initialize(QAbstractSocket::TcpSocket, address.protocol())) {
        return false;
    }

    // Under Unix, we want to be able to bind to the port, even if a socket on
    // the same address-port is in TIME_WAIT. Under Windows this is possible
    // anyway -- furthermore, the meaning of reusable on Windows is different:
    // it means that you can use the same address-port for multiple listening
    // sockets.
    // Don't abort though if we can't set that option. For example the socks
    // engine doesn't support that option, but that shouldn't prevent us from
    // trying to bind/listen.
    d->socketEngine->setOption(QAbstractSocketEngine::AddressReusable, 1);

    if (!d->socketEngine->bind(address, port)) {
        return false;
    }

    if (!d->socketEngine->listen()) {
        return false;
    }

    d->socketEngine->setReceiver(d);
    d->socketEngine->setReadNotificationEnabled(true);

#if defined (QTCPSERVER_DEBUG)
    qDebug("QTcpServer::listen(%i, \"%s\") == true (listening on port %i)", port,
           address.toString().toLatin1().constData(), d->socketEngine->localPort());
#endif
    return true;
}

/*!
    Returns true if the server is currently listening for incoming
    connections; otherwise returns false.

    \sa listen()
*/
bool QTcpServer::isListening() const
{
    Q_D(const QTcpServer);
    Q_CHECK_SOCKETENGINE(false);
    return d->socketEngine->state() == QAbstractSocket::ListeningState;
}

/*!
    Closes the server. The server will no longer listen for incoming
    connections.

    \sa listen()
*/
void QTcpServer::close()
{
    Q_D(QTcpServer);

    qDeleteAll(d->pendingConnections);
    d->pendingConnections.clear();

    if (d->socketEngine) {
        d->socketEngine->close();
        d->socketEngine->deleteLater();
        d->socketEngine = 0;
    }
}

/*!
    Returns the native socket descriptor the server uses to listen
    for incoming instructions, or -1 if the server is not listening.

    \sa setSocketDescriptor(), isListening()
*/
int QTcpServer::socketDescriptor() const
{
    Q_D(const QTcpServer);
    Q_CHECK_SOCKETENGINE(-1);
    return d->socketEngine->socketDescriptor();
}

/*!
    Sets the socket descriptor this server should use when listening
    for incoming connections to \a socketDescriptor. Returns true if
    the socket is set successfully; otherwise returns false.

    The socket is assumed to be in listening state.

    \sa socketDescriptor(), isListening()
*/
bool QTcpServer::setSocketDescriptor(int socketDescriptor)
{
    Q_D(QTcpServer);
    if (Q_UNLIKELY(isListening())) {
        qWarning("QTcpServer::setSocketDescriptor() called when already listening");
        return false;
    }

    delete d->socketEngine;
    d->socketEngine = new QAbstractSocketEngine(this);
    if (!d->socketEngine->initialize(socketDescriptor, QAbstractSocket::ListeningState)) {
#if defined (QTCPSERVER_DEBUG)
        qDebug("QTcpServer::setSocketDescriptor(%i) failed (%s)", socketDescriptor,
               d->socketEngine->errorString().toLatin1().constData());
#endif
        return false;
    }

    d->socketEngine->setReceiver(d);
    d->socketEngine->setReadNotificationEnabled(true);

#if defined (QTCPSERVER_DEBUG)
    qDebug("QTcpServer::setSocketDescriptor(%i) succeeded.", socketDescriptor);
#endif
    return true;
}

/*!
    Returns the server's port if the server is listening for
    connections; otherwise returns 0.

    \sa serverAddress(), listen()
*/
quint16 QTcpServer::serverPort() const
{
    Q_D(const QTcpServer);
    Q_CHECK_SOCKETENGINE(0);
    return d->socketEngine->localPort();
}

/*!
    Returns the server's address if the server is listening for
    connections; otherwise returns QHostAddress::Null.

    \sa serverPort(), listen()
*/
QHostAddress QTcpServer::serverAddress() const
{
    Q_D(const QTcpServer);
    Q_CHECK_SOCKETENGINE(QHostAddress(QHostAddress::Null));
    return d->socketEngine->localAddress();
}

/*!
    Waits for at most \a msec milliseconds or until an incoming
    connection is available. Returns true if a connection is
    available; otherwise returns false. If the operation timed out
    and \a timedOut is not 0, *\a timedOut will be set to true.

    This is a blocking function call. Its use is disadvised in a
    single-threaded GUI application, since the whole application will
    stop responding until the function returns.
    waitForNewConnection() is mostly useful when there is no event
    loop available.

    The non-blocking alternative is to connect to the newConnection()
    signal.

    If msec is -1, this function will not time out.

    \sa hasPendingConnections(), nextPendingConnection()
*/
bool QTcpServer::waitForNewConnection(int msec, bool *timedOut)
{
    Q_D(QTcpServer);
    if (!isListening()) {
        return false;
    }

    if (!d->socketEngine->waitForRead(msec, timedOut)) {
        return false;
    }

    if (timedOut && *timedOut)
        return false;

    d->readNotification();

    return true;
}

/*!
    Returns true if the server has a pending connection; otherwise
    returns false.

    \sa nextPendingConnection(), setMaxPendingConnections()
*/
bool QTcpServer::hasPendingConnections() const
{
    return !d_func()->pendingConnections.isEmpty();
}

/*!
    Returns the next pending connection as a connected QTcpSocket
    object.

    The socket is created as a child of the server, which means that
    it is automatically deleted when the QTcpServer object is
    destroyed. It is still a good idea to delete the object
    explicitly when you are done with it, to avoid wasting memory.

    0 is returned if this function is called when there are no pending
    connections.

    \note The returned QTcpSocket object cannot be used from another
    thread. If you want to use an incoming connection from another thread,
    you need to override incomingConnection().

    \sa hasPendingConnections()
*/
QTcpSocket *QTcpServer::nextPendingConnection()
{
    Q_D(QTcpServer);
    if (d->pendingConnections.isEmpty())
        return 0;

    if (!d->socketEngine->isReadNotificationEnabled())
        d->socketEngine->setReadNotificationEnabled(true);

    return d->pendingConnections.takeFirst();
}

/*!
    This virtual function is called by QTcpServer when a new
    connection is available. The \a socketDescriptor argument is the
    native socket descriptor for the accepted connection.

    The base implementation creates a QTcpSocket, sets the socket
    descriptor and then stores the QTcpSocket in an internal list of
    pending connections. Finally newConnection() is emitted.

    Reimplement this function to alter the server's behavior when a
    connection is available.

    \note If you want to handle an incoming connection as a new QTcpSocket
    object in another thread you have to pass the socketDescriptor
    to the other thread and create the QTcpSocket object there and
    use its setSocketDescriptor() method.

    \sa newConnection(), nextPendingConnection(), addPendingConnection()
*/
void QTcpServer::incomingConnection(int socketDescriptor)
{
#if defined (QTCPSERVER_DEBUG)
    qDebug("QTcpServer::incomingConnection(%i)", socketDescriptor);
#endif

    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    addPendingConnection(socket);
}

/*!
    This function is called by QTcpServer::incomingConnection()
    to add the \a socket to the list of pending incoming connections.

    \note Don't forget to call this member from reimplemented
    incomingConnection() if you do not want to break the
    Pending Connections mechanism.

    \sa incomingConnection()
    \since 4.7
*/
void QTcpServer::addPendingConnection(QTcpSocket* socket)
{
    d_func()->pendingConnections.append(socket);
}

/*!
    Sets the maximum number of pending accepted connections to \a
    numConnections. QTcpServer will accept no more than \a
    numConnections incoming connections before
    nextPendingConnection() is called. By default, the limit is 30
    pending connections.

    Clients may still able to connect after the server has reached
    its maximum number of pending connections (i.e., QTcpSocket can
    still emit the connected() signal). QTcpServer will stop
    accepting the new connections, but the operating system may
    still keep them in queue.

    \sa maxPendingConnections(), hasPendingConnections()
*/
void QTcpServer::setMaxPendingConnections(int numConnections)
{
    d_func()->maxConnections = numConnections;
}

/*!
    Returns the maximum number of pending accepted connections. The
    default is 30.

    \sa setMaxPendingConnections(), hasPendingConnections()
*/
int QTcpServer::maxPendingConnections() const
{
    return d_func()->maxConnections;
}

/*!
    Returns an error code for the last error that occurred.

    \sa errorString()
*/
QAbstractSocket::SocketError QTcpServer::serverError() const
{
    Q_D(const QTcpServer);
    Q_CHECK_SOCKETENGINE(QAbstractSocket::UnknownSocketError);
    return d_func()->socketEngine->error();
}

/*!
    Returns a human readable description of the last error that
    occurred.

    \sa serverError()
*/
QString QTcpServer::errorString() const
{
    Q_D(const QTcpServer);
    Q_CHECK_SOCKETENGINE(tr("Unknown error"));
    return d->socketEngine->errorString();
}

QT_END_NAMESPACE

#include "moc_qtcpserver.h"
