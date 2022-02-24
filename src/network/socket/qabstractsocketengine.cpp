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

#include "qabstractsocketengine_p.h"
#include "qabstracteventdispatcher.h"
#include "qsocketnotifier.h"
#include "qnetworkinterface.h"
#include "qthread_p.h"
#include "qobject_p.h"
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

//#define QABSTRACTSOCKETENGINE_DEBUG

// Common checks
#define Q_CHECK_VALID_SOCKETLAYER(function, returnValue) do { \
    if (!isValid()) { \
        qWarning(""#function" was called on an uninitialized socket device"); \
        return returnValue; \
    } } while (0)

#define Q_CHECK_STATE(function, checkState, returnValue) do { \
    if (d->socketState != (checkState)) { \
        qWarning(""#function" was not called in "#checkState); \
        return (returnValue); \
    } } while (0)

#define Q_CHECK_NOT_STATE(function, checkState, returnValue) do { \
    if (d->socketState == (checkState)) { \
        qWarning(""#function" was called in "#checkState); \
        return (returnValue); \
    } } while (0)

#define Q_CHECK_STATES(function, state1, state2, returnValue) do { \
    if (d->socketState != (state1) && d->socketState != (state2)) { \
        qWarning(""#function" was called" \
                 " not in "#state1" or "#state2); \
        return (returnValue); \
    } } while (0)

#define Q_CHECK_TYPE(function, type, returnValue) do { \
    if (d->socketType != (type)) { \
        qWarning(#function" was called by a" \
                 " socket other than "#type""); \
        return (returnValue); \
    } } while (0)

/*
  \internal
  \class QReadNotifier
  \brief The QReadNotifer class is used to improve performance.

  QReadNotifier is a private class used for performance reasons vs
  connecting to the QSocketNotifier activated() signal.
 */
class QReadNotifier : public QSocketNotifier
{
public:
    QReadNotifier(int fd, QAbstractSocketEngine *parent)
        : QSocketNotifier(fd, QSocketNotifier::Read, parent)
    { engine = parent; }

protected:
    bool event(QEvent *);

    QAbstractSocketEngine *engine;
};

bool QReadNotifier::event(QEvent *e)
{
    if (e->type() == QEvent::SockAct) {
        engine->readNotification();
        return true;
    }
    return QSocketNotifier::event(e);
}

/*
  \internal
  \class QWriteNotifier
  \brief The QWriteNotifer class is used to improve performance.

  QWriteNotifier is a private class used for performance reasons vs
  connecting to the QSocketNotifier activated() signal.
 */
class QWriteNotifier : public QSocketNotifier
{
public:
    QWriteNotifier(int fd, QAbstractSocketEngine *parent)
        : QSocketNotifier(fd, QSocketNotifier::Write, parent) { engine = parent; }

protected:
    bool event(QEvent *);

    QAbstractSocketEngine *engine;
};

bool QWriteNotifier::event(QEvent *e)
{
    if (e->type() == QEvent::SockAct) {
        if (engine->state() == QAbstractSocket::ConnectingState)
            engine->connectionNotification();
        else
            engine->writeNotification();
        return true;
    }
    return QSocketNotifier::event(e);
}

class QExceptionNotifier : public QSocketNotifier
{
public:
    QExceptionNotifier(int fd, QAbstractSocketEngine *parent)
        : QSocketNotifier(fd, QSocketNotifier::Exception, parent) { engine = parent; }

protected:
    bool event(QEvent *);

    QAbstractSocketEngine *engine;
};

bool QExceptionNotifier::event(QEvent *e)
{
    if (e->type() == QEvent::SockAct) {
        if (engine->state() == QAbstractSocket::ConnectingState)
            engine->connectionNotification();
        else
            engine->exceptionNotification();
        return true;
    }
    return QSocketNotifier::event(e);
}

/*! \internal
    Constructs the private class and initializes all data members.
*/
QAbstractSocketEnginePrivate::QAbstractSocketEnginePrivate()
    : socketDescriptor(-1)
    , readNotifier(0)
    , writeNotifier(0)
    , exceptNotifier(0)
    , socketError(QAbstractSocket::UnknownSocketError)
    , hasSetSocketError(false)
    , socketErrorString(QLatin1String(QT_TRANSLATE_NOOP(QSocketLayer, "Unknown error")))
    , socketState(QAbstractSocket::UnconnectedState)
    , socketType(QAbstractSocket::UnknownSocketType)
    , socketProtocol(QAbstractSocket::UnknownNetworkLayerProtocol)
    , localPort(0)
    , peerPort(0)
    , receiver(0)
{
}

/*! \internal
    Destructs the private class.
*/
QAbstractSocketEnginePrivate::~QAbstractSocketEnginePrivate()
{
}


/*!
    Constructs a QAbstractSocketEngine.

    \sa initialize()
*/
QAbstractSocketEngine::QAbstractSocketEngine(QObject *parent)
    : QObject(*new QAbstractSocketEnginePrivate(), parent)
{
}

QAbstractSocketEngine::QAbstractSocketEngine(QAbstractSocketEnginePrivate &dd, QObject* parent)
    : QObject(dd, parent)
{
}

/*!
    Destructs a QAbstractSocketEngine.
*/
QAbstractSocketEngine::~QAbstractSocketEngine()
{
    close();
}


QAbstractSocket::SocketError QAbstractSocketEngine::error() const
{
    return d_func()->socketError;
}

QString QAbstractSocketEngine::errorString() const
{
    return d_func()->socketErrorString;
}

void QAbstractSocketEngine::setError(QAbstractSocket::SocketError error, const QString &errorString)
{
    Q_D(QAbstractSocketEngine);
    d->socketError = error;
    d->socketErrorString = errorString;
}

void QAbstractSocketEngine::setReceiver(QAbstractSocketEngineReceiver *receiver)
{
    d_func()->receiver = receiver;
}

void QAbstractSocketEngine::readNotification()
{
    if (QAbstractSocketEngineReceiver *receiver = d_func()->receiver)
        receiver->readNotification();
}

void QAbstractSocketEngine::writeNotification()
{
    if (QAbstractSocketEngineReceiver *receiver = d_func()->receiver)
        receiver->writeNotification();
}

void QAbstractSocketEngine::exceptionNotification()
{
    if (QAbstractSocketEngineReceiver *receiver = d_func()->receiver)
        receiver->exceptionNotification();
}

/*!
    If there's a connection activity on the socket, process it. Then
    notify our parent if there really was activity.
*/
void QAbstractSocketEngine::connectionNotification()
{
    Q_D(QAbstractSocketEngine);
    Q_ASSERT(state() == QAbstractSocket::ConnectingState);

    connectToHost(d->peerAddress, d->peerPort);
    if (state() != QAbstractSocket::ConnectingState) {
        // we changed states
        if (QAbstractSocketEngineReceiver *receiver = d_func()->receiver)
            receiver->connectionNotification();
    }
}

QAbstractSocket::SocketState QAbstractSocketEngine::state() const
{
    return d_func()->socketState;
}

void QAbstractSocketEngine::setState(QAbstractSocket::SocketState state)
{
    d_func()->socketState = state;
}

QAbstractSocket::SocketType QAbstractSocketEngine::socketType() const
{
    return d_func()->socketType;
}

void QAbstractSocketEngine::setSocketType(QAbstractSocket::SocketType socketType)
{
    d_func()->socketType = socketType;
}

QAbstractSocket::NetworkLayerProtocol QAbstractSocketEngine::protocol() const
{
    return d_func()->socketProtocol;
}

void QAbstractSocketEngine::setProtocol(QAbstractSocket::NetworkLayerProtocol protocol)
{
    d_func()->socketProtocol = protocol;
}

QHostAddress QAbstractSocketEngine::localAddress() const
{
    return d_func()->localAddress;
}

void QAbstractSocketEngine::setLocalAddress(const QHostAddress &address)
{
    d_func()->localAddress = address;
}

quint16 QAbstractSocketEngine::localPort() const
{
    return d_func()->localPort;
}

void QAbstractSocketEngine::setLocalPort(quint16 port)
{
    d_func()->localPort = port;
}

QHostAddress QAbstractSocketEngine::peerAddress() const
{
    return d_func()->peerAddress;
}

void QAbstractSocketEngine::setPeerAddress(const QHostAddress &address)
{
   d_func()->peerAddress = address;
}

quint16 QAbstractSocketEngine::peerPort() const
{
    return d_func()->peerPort;
}

void QAbstractSocketEngine::setPeerPort(quint16 port)
{
    d_func()->peerPort = port;
}

/*!
    Sets the option \a option to the value \a value.
*/
bool QAbstractSocketEngine::setOption(SocketOption option, int value)
{
    Q_D(QAbstractSocketEngine);
    return d->setOption(option, value);
}


/*! \internal

    Sets the error and error string if not set already. The only
    interesting error is the first one that occurred, and not the last
    one.
*/
void QAbstractSocketEnginePrivate::setError(QAbstractSocket::SocketError error, ErrorString errorString)
{
    if (hasSetSocketError) {
        // Only set socket errors once for one engine; expect the
        // socket to recreate its engine after an error. Note: There's
        // one exception: SocketError(11) bypasses this as it's purely
        // a temporary internal error condition.
        // Another exception is the way the waitFor*() functions set
        // an error when a timeout occurs. After the call to setError()
        // they reset the hasSetSocketError to false
        return;
    }
    if (error != QAbstractSocket::SocketError(11))
        hasSetSocketError = true;

    socketError = error;

    switch (errorString) {
    case NonBlockingInitFailedErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Unable to initialize non-blocking socket");
        break;
    case BroadcastingInitFailedErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Unable to initialize broadcast socket");
        break;
    case NoIpV6ErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Attempt to use IPv6 socket on a platform with no IPv6 support");
        break;
    case RemoteHostClosedErrorString:
        socketErrorString = QAbstractSocketEngine::tr("The remote host closed the connection");
        break;
    case TimeOutErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Network operation timed out");
        break;
    case ResourceErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Out of resources");
        break;
    case OperationUnsupportedErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Unsupported socket operation");
        break;
    case ProtocolUnsupportedErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Protocol type not supported");
        break;
    case InvalidSocketErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Invalid socket descriptor");
        break;
    case HostUnreachableErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Host unreachable");
        break;
    case NetworkUnreachableErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Network unreachable");
        break;
    case AccessErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Permission denied");
        break;
    case ConnectionTimeOutErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Connection timed out");
        break;
    case ConnectionRefusedErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Connection refused");
        break;
    case AddressInuseErrorString:
        socketErrorString = QAbstractSocketEngine::tr("The bound address is already in use");
        break;
    case AddressNotAvailableErrorString:
        socketErrorString = QAbstractSocketEngine::tr("The address is not available");
        break;
    case AddressProtectedErrorString:
        socketErrorString = QAbstractSocketEngine::tr("The address is protected");
        break;
    case DatagramTooLargeErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Datagram was too large to send");
        break;
    case SendDatagramErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Unable to send a message");
        break;
    case ReceiveDatagramErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Unable to receive a message");
        break;
    case WriteErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Unable to write");
        break;
    case ReadErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Network error");
        break;
    case PortInuseErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Another socket is already listening on the same port");
        break;
    case NotSocketErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Operation on non-socket");
        break;
    case UnknownSocketErrorString:
        socketErrorString = QAbstractSocketEngine::tr("Unknown error");
        break;
    }
}

/*!
    Initializes a QAbstractSocketEngine by creating a new socket of type \a
    socketType and network layer protocol \a protocol. Returns true on
    success; otherwise returns false.

    If the socket was already initialized, this function closes the
    socket before reeinitializing it.

    The new socket is non-blocking, and for UDP sockets it's also
    broadcast enabled.
*/
bool QAbstractSocketEngine::initialize(QAbstractSocket::SocketType socketType, QAbstractSocket::NetworkLayerProtocol protocol)
{
    Q_D(QAbstractSocketEngine);
    if (isValid())
        close();

#if defined(QT_NO_IPV6)
    if (protocol == QAbstractSocket::IPv6Protocol) {
        d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                    QAbstractSocketEnginePrivate::NoIpV6ErrorString);
        return false;
    }
#endif

    // Create the socket
    if (!d->createNewSocket(socketType, protocol)) {
#if defined (QABSTRACTSOCKETENGINE_DEBUG)
        QString typeStr = QLatin1String("UnknownSocketType");
        if (socketType == QAbstractSocket::TcpSocket) typeStr = QLatin1String("TcpSocket");
        else if (socketType == QAbstractSocket::UdpSocket) typeStr = QLatin1String("UdpSocket");
        QString protocolStr = QLatin1String("UnknownProtocol");
        if (protocol == QAbstractSocket::IPv4Protocol) protocolStr = QLatin1String("IPv4Protocol");
        else if (protocol == QAbstractSocket::IPv6Protocol) protocolStr = QLatin1String("IPv6Protocol");
        qDebug("QAbstractSocketEngine::initialize(type == %s, protocol == %s) failed: %s",
               typeStr.toLatin1().constData(), protocolStr.toLatin1().constData(), d->socketErrorString.toLatin1().constData());
#endif
        return false;
    }

    // Set the broadcasting flag if it's a UDP socket.
    if (socketType == QAbstractSocket::UdpSocket
        && !setOption(BroadcastSocketOption, 1)) {
        d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                    QAbstractSocketEnginePrivate::BroadcastingInitFailedErrorString);
        close();
        return false;
    }


    // Make sure we receive out-of-band data
    if (socketType == QAbstractSocket::TcpSocket
        && !setOption(ReceiveOutOfBandData, 1)) {
        qWarning("QAbstractSocketEngine::initialize unable to inline out-of-band data");
    }

    d->socketType = socketType;
    d->socketProtocol = protocol;
    return true;
}

/*! \overload

    Initializes the socket using \a socketDescriptor instead of
    creating a new one. The socket type and network layer protocol are
    determined automatically. The socket's state is set to \a
    socketState.

    If the socket type is either TCP or UDP, it is made non-blocking.
    UDP sockets are also broadcast enabled.
 */
bool QAbstractSocketEngine::initialize(int socketDescriptor, QAbstractSocket::SocketState socketState)
{
    Q_D(QAbstractSocketEngine);

    if (isValid())
        close();

    d->socketDescriptor = socketDescriptor;

    // determine socket type and protocol
    if (!d->fetchConnectionParameters()) {
#if defined (QABSTRACTSOCKETENGINE_DEBUG)
        qDebug("QAbstractSocketEngine::initialize(socketDescriptor == %i) failed: %s",
               socketDescriptor, d->socketErrorString.toLatin1().constData());
#endif
        d->socketDescriptor = -1;
        return false;
    }

    if (d->socketType != QAbstractSocket::UnknownSocketType) {
        // Make the socket nonblocking.
        if (!setOption(NonBlockingSocketOption, 1)) {
            d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                QAbstractSocketEnginePrivate::NonBlockingInitFailedErrorString);
            close();
            return false;
        }

        // Set the broadcasting flag if it's a UDP socket.
        if (d->socketType == QAbstractSocket::UdpSocket
            && !setOption(BroadcastSocketOption, 1)) {
            d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                QAbstractSocketEnginePrivate::BroadcastingInitFailedErrorString);
            close();
            return false;
        }
    }

    d->socketState = socketState;
    return true;
}

/*!
    Returns true if the socket is valid; otherwise returns false. A
    socket is valid if it has not been successfully initialized, or if
    it has been closed.
*/
bool QAbstractSocketEngine::isValid() const
{
    Q_D(const QAbstractSocketEngine);
    return d->socketDescriptor != -1;
}

/*!
    Returns the native socket descriptor. Any use of this descriptor
    stands the risk of being non-portable.
*/
int QAbstractSocketEngine::socketDescriptor() const
{
    Q_D(const QAbstractSocketEngine);
    return d->socketDescriptor;
}

/*!
    Connects to the IP address and port specified by \a address and \a
    port. If the connection is established, this function returns true
    and the socket enters ConnectedState. Otherwise, false is
    returned.

    If false is returned, state() should be called to see if the
    socket is in ConnectingState. If so, a delayed TCP connection is
    taking place, and connectToHost() must be called again later to
    determine if the connection was established successfully or
    not. The second connection attempt must be made when the socket is
    ready for writing. This state can be determined either by
    connecting a QSocketNotifier to the socket descriptor returned by
    socketDescriptor(), or by calling the blocking function
    waitForWrite().

    Example:
    \snippet doc/src/snippets/code/src_network_socket_qasocketengine.cpp 0

    Otherwise, error() should be called to determine the cause of the
    error.
*/
bool QAbstractSocketEngine::connectToHost(const QHostAddress &address, quint16 port)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::connectToHost(), false);

#if defined (QT_NO_IPV6)
    if (address.protocol() == QAbstractSocket::IPv6Protocol) {
        d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                    QAbstractSocketEnginePrivate::NoIpV6ErrorString);
        return false;
    }
#endif

    Q_CHECK_STATES(QAbstractSocketEngine::connectToHost(),
                   QAbstractSocket::UnconnectedState, QAbstractSocket::ConnectingState, false);

    d->peerAddress = address;
    d->peerPort = port;
    bool connected = d->nativeConnect(address, port);
    if (connected)
        d->fetchConnectionParameters();

    return connected;
}

/*!
    Binds the socket to the address \a address and port \a
    port. Returns true on success; otherwise false is returned. The
    port may be 0, in which case an arbitrary unused port is assigned
    automatically by the operating system.

    Servers call this function to set up the server's address and
    port. TCP servers must in addition call listen() after bind().
*/
bool QAbstractSocketEngine::bind(const QHostAddress &address, quint16 port)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::bind(), false);

#if defined (QT_NO_IPV6)
    if (address.protocol() == QAbstractSocket::IPv6Protocol) {
        d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                    QAbstractSocketEnginePrivate::NoIpV6ErrorString);
        return false;
    }
#endif

    Q_CHECK_STATE(QAbstractSocketEngine::bind(), QAbstractSocket::UnconnectedState, false);

    if (!d->nativeBind(address, port))
        return false;

    d->fetchConnectionParameters();
    return true;
}

/*!
    Prepares a TCP server for accepting incoming connections. This
    function must be called after bind(), and only by TCP sockets.

    After this function has been called, pending client connections
    are detected by checking if the socket is ready for reading. This
    can be done by either creating a QSocketNotifier, passing the
    socket descriptor returned by socketDescriptor(), or by calling
    the blocking function waitForRead().

    Example:
    \snippet doc/src/snippets/code/src_network_socket_qasocketengine.cpp 1

    \sa bind(), accept()
*/
bool QAbstractSocketEngine::listen()
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::listen(), false);
    Q_CHECK_STATE(QAbstractSocketEngine::listen(), QAbstractSocket::BoundState, false);
    Q_CHECK_TYPE(QAbstractSocketEngine::listen(), QAbstractSocket::TcpSocket, false);

    return d->nativeListen();
}

/*!
    Accepts a pending connection from the socket, which must be in
    ListeningState, and returns its socket descriptor. If no pending
    connections are available, -1 is returned.

    \sa bind(), listen()
*/
int QAbstractSocketEngine::accept()
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::accept(), -1);
    Q_CHECK_STATE(QAbstractSocketEngine::accept(), QAbstractSocket::ListeningState, false);
    Q_CHECK_TYPE(QAbstractSocketEngine::accept(), QAbstractSocket::TcpSocket, false);

    return d->nativeAccept();
}

#ifndef QT_NO_NETWORKINTERFACE

/*!
    \since 4.8
*/
bool QAbstractSocketEngine::joinMulticastGroup(const QHostAddress &groupAddress,
                                             const QNetworkInterface &iface)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::joinMulticastGroup(), false);
    Q_CHECK_STATE(QAbstractSocketEngine::joinMulticastGroup(), QAbstractSocket::BoundState, false);
    Q_CHECK_TYPE(QAbstractSocketEngine::joinMulticastGroup(), QAbstractSocket::UdpSocket, false);
    return d->nativeJoinMulticastGroup(groupAddress, iface);
}

/*!
    \since 4.8
*/
bool QAbstractSocketEngine::leaveMulticastGroup(const QHostAddress &groupAddress,
                                              const QNetworkInterface &iface)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::leaveMulticastGroup(), false);
    Q_CHECK_STATE(QAbstractSocketEngine::leaveMulticastGroup(), QAbstractSocket::BoundState, false);
    Q_CHECK_TYPE(QAbstractSocketEngine::leaveMulticastGroup(), QAbstractSocket::UdpSocket, false);
    return d->nativeLeaveMulticastGroup(groupAddress, iface);
}

/*! \since 4.8 */
QNetworkInterface QAbstractSocketEngine::multicastInterface() const
{
    Q_D(const QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::multicastInterface(), QNetworkInterface());
    Q_CHECK_TYPE(QAbstractSocketEngine::multicastInterface(), QAbstractSocket::UdpSocket, QNetworkInterface());
    return d->nativeMulticastInterface();
}

/*! \since 4.8 */
bool QAbstractSocketEngine::setMulticastInterface(const QNetworkInterface &iface)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::setMulticastInterface(), false);
    Q_CHECK_TYPE(QAbstractSocketEngine::setMulticastInterface(), QAbstractSocket::UdpSocket, false);
    return d->nativeSetMulticastInterface(iface);
}

#endif // QT_NO_NETWORKINTERFACE

/*!
    Returns the number of bytes that are currently available for
    reading. On error, -1 is returned.

    For UDP sockets, this function returns the accumulated size of all
    pending datagrams, and it is therefore more useful for UDP sockets
    to call hasPendingDatagrams() and pendingDatagramSize().
*/
qint64 QAbstractSocketEngine::bytesAvailable() const
{
    Q_D(const QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::bytesAvailable(), -1);
    Q_CHECK_NOT_STATE(QAbstractSocketEngine::bytesAvailable(), QAbstractSocket::UnconnectedState, false);

    return d->nativeBytesAvailable();
}

/*!
    Returns true if there is at least one datagram pending. This
    function is only called by UDP sockets, where a datagram can have
    a size of 0. TCP sockets call bytesAvailable().
*/
bool QAbstractSocketEngine::hasPendingDatagrams() const
{
    Q_D(const QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::hasPendingDatagrams(), false);
    Q_CHECK_NOT_STATE(QAbstractSocketEngine::hasPendingDatagrams(), QAbstractSocket::UnconnectedState, false);
    Q_CHECK_TYPE(QAbstractSocketEngine::hasPendingDatagrams(), QAbstractSocket::UdpSocket, false);

    return d->nativeHasPendingDatagrams();
}

/*!
    Returns the size of the pending datagram, or -1 if no datagram is
    pending. A datagram size of 0 is perfectly valid. This function is
    called by UDP sockets before receiveMessage(). For TCP sockets,
    call bytesAvailable().
*/
qint64 QAbstractSocketEngine::pendingDatagramSize() const
{
    Q_D(const QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::pendingDatagramSize(), -1);
    Q_CHECK_TYPE(QAbstractSocketEngine::pendingDatagramSize(), QAbstractSocket::UdpSocket, false);

    return d->nativePendingDatagramSize();
}

/*!
    Reads up to \a maxSize bytes of a datagram from the socket,
    stores it in \a data and returns the number of bytes read. The
    address and port of the sender are stored in \a address and \a
    port. If either of these pointers is 0, the corresponding value is
    discarded.

    To avoid unnecessarily loss of data, call pendingDatagramSize() to
    determine the size of the pending message before reading it. If \a
    maxSize is too small, the rest of the datagram will be lost.

    Returns -1 if an error occurred.

    \sa hasPendingDatagrams()
*/
qint64 QAbstractSocketEngine::readDatagram(char *data, qint64 maxSize, QHostAddress *address,
                                      quint16 *port)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::readDatagram(), -1);
    Q_CHECK_TYPE(QAbstractSocketEngine::readDatagram(), QAbstractSocket::UdpSocket, false);

    return d->nativeReceiveDatagram(data, maxSize, address, port);
}

/*!
    Writes a UDP datagram of size \a size bytes to the socket from
    \a data to the address \a host on port \a port, and returns the
    number of bytes written, or -1 if an error occurred.

    Only one datagram is sent, and if there is too much data to fit
    into a single datagram, the operation will fail and error()
    will return QAbstractSocket::DatagramTooLargeError. Operating systems impose an
    upper limit to the size of a datagram, but this size is different
    on almost all platforms. Sending large datagrams is in general
    disadvised, as even if they are sent successfully, they are likely
    to be fragmented before arriving at their destination.

    Experience has shown that it is in general safe to send datagrams
    no larger than 512 bytes.

    \sa readDatagram()
*/
qint64 QAbstractSocketEngine::writeDatagram(const char *data, qint64 size,
                                       const QHostAddress &host, quint16 port)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::writeDatagram(), -1);
    Q_CHECK_TYPE(QAbstractSocketEngine::writeDatagram(), QAbstractSocket::UdpSocket, -1);
    return d->nativeSendDatagram(data, size, host, port);
}

/*!
    Writes a block of \a size bytes from \a data to the socket.
    Returns the number of bytes written, or -1 if an error occurred.
*/
qint64 QAbstractSocketEngine::write(const char *data, qint64 size)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::write(), -1);
    Q_CHECK_STATE(QAbstractSocketEngine::write(), QAbstractSocket::ConnectedState, -1);
    return d->nativeWrite(data, size);
}


qint64 QAbstractSocketEngine::bytesToWrite() const
{
    return 0;
}

/*!
    Reads up to \a maxSize bytes into \a data from the socket.
    Returns the number of bytes read, or -1 if an error occurred.
*/
qint64 QAbstractSocketEngine::read(char *data, qint64 maxSize)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::read(), -1);
    Q_CHECK_STATES(QAbstractSocketEngine::read(), QAbstractSocket::ConnectedState, QAbstractSocket::BoundState, -1);

    qint64 readBytes = d->nativeRead(data, maxSize);

    // Handle remote close
    if (readBytes == 0 && d->socketType == QAbstractSocket::TcpSocket) {
        d->setError(QAbstractSocket::RemoteHostClosedError,
                    QAbstractSocketEnginePrivate::RemoteHostClosedErrorString);
        close();
        return -1;
    } else if (readBytes == -1) {
        if (!d->hasSetSocketError) {
            d->hasSetSocketError = true;
            d->socketError = QAbstractSocket::NetworkError;
            d->socketErrorString = qt_error_string(errno);
        }
        close();
        return -1;
    }
    return readBytes;
}

/*!
    Closes the socket. In order to use the socket again, initialize()
    must be called.
*/
void QAbstractSocketEngine::close()
{
    Q_D(QAbstractSocketEngine);
    if (d->readNotifier)
        d->readNotifier->setEnabled(false);
    if (d->writeNotifier)
        d->writeNotifier->setEnabled(false);
    if (d->exceptNotifier)
        d->exceptNotifier->setEnabled(false);

    if(d->socketDescriptor != -1) {
        d->nativeClose();
        d->socketDescriptor = -1;
    }
    d->socketState = QAbstractSocket::UnconnectedState;
    d->hasSetSocketError = false;
    d->localPort = 0;
    d->localAddress.clear();
    d->peerPort = 0;
    d->peerAddress.clear();
    if (d->readNotifier) {
        delete d->readNotifier;
        d->readNotifier = 0;
    }
    if (d->writeNotifier) {
        delete d->writeNotifier;
        d->writeNotifier = 0;
    }
    if (d->exceptNotifier) {
        delete d->exceptNotifier;
        d->exceptNotifier = 0;
    }
}

/*!
    Waits for \a msecs milliseconds or until the socket is ready for
    reading. If \a timedOut is not 0 and \a msecs milliseconds have
    passed, the value of \a timedOut is set to true.

    Returns true if data is available for reading; otherwise returns
    false.

    This is a blocking function call; its use is disadvised in a
    single threaded application, as the whole thread will stop
    responding until the function returns. waitForRead() is most
    useful when there is no event loop available. The general approach
    is to create a QSocketNotifier, passing the socket descriptor
    returned by socketDescriptor() to its constructor.
*/
bool QAbstractSocketEngine::waitForRead(int msecs, bool *timedOut)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::waitForRead(), false);
    Q_CHECK_NOT_STATE(QAbstractSocketEngine::waitForRead(),
                      QAbstractSocket::UnconnectedState, false);

    if (timedOut)
        *timedOut = false;

    int ret = d->nativeSelect(msecs, true);
    if (ret == 0) {
        if (timedOut)
            *timedOut = true;
        d->setError(QAbstractSocket::SocketTimeoutError,
            QAbstractSocketEnginePrivate::TimeOutErrorString);
        d->hasSetSocketError = false; // A timeout error is temporary in waitFor functions
        return false;
    } else if (state() == QAbstractSocket::ConnectingState) {
        connectToHost(d->peerAddress, d->peerPort);
    }

    return ret > 0;
}

/*!
    Waits for \a msecs milliseconds or until the socket is ready for
    writing. If \a timedOut is not 0 and \a msecs milliseconds have
    passed, the value of \a timedOut is set to true.

    Returns true if data is available for writing; otherwise returns
    false.

    This is a blocking function call; its use is disadvised in a
    single threaded application, as the whole thread will stop
    responding until the function returns. waitForWrite() is most
    useful when there is no event loop available. The general approach
    is to create a QSocketNotifier, passing the socket descriptor
    returned by socketDescriptor() to its constructor.
*/
bool QAbstractSocketEngine::waitForWrite(int msecs, bool *timedOut)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::waitForWrite(), false);
    Q_CHECK_NOT_STATE(QAbstractSocketEngine::waitForWrite(),
                      QAbstractSocket::UnconnectedState, false);

    if (timedOut)
        *timedOut = false;

    const int ret = d->nativeSelect(msecs, false);
    if (ret == 0) {
        if (timedOut)
            *timedOut = true;
        d->setError(QAbstractSocket::SocketTimeoutError,
                    QAbstractSocketEnginePrivate::TimeOutErrorString);
        d->hasSetSocketError = false; // A timeout error is temporary in waitFor functions
        return false;
    } else if (state() == QAbstractSocket::ConnectingState) {
        connectToHost(d->peerAddress, d->peerPort);
    }

    return ret > 0;
}

bool QAbstractSocketEngine::waitForReadOrWrite(bool *readyToRead, bool *readyToWrite,
                                      bool checkRead, bool checkWrite,
                                      int msecs, bool *timedOut)
{
    Q_D(QAbstractSocketEngine);
    Q_CHECK_VALID_SOCKETLAYER(QAbstractSocketEngine::waitForWrite(), false);
    Q_CHECK_NOT_STATE(QAbstractSocketEngine::waitForReadOrWrite(),
                      QAbstractSocket::UnconnectedState, false);

    const int ret = d->nativeSelect(msecs, checkRead, checkWrite, readyToRead, readyToWrite);
    if (ret == 0) {
        if (timedOut)
            *timedOut = true;
        d->setError(QAbstractSocket::SocketTimeoutError,
                    QAbstractSocketEnginePrivate::TimeOutErrorString);
        d->hasSetSocketError = false; // A timeout error is temporary in waitFor functions
        return false;
    } else if (state() == QAbstractSocket::ConnectingState) {
        connectToHost(d->peerAddress, d->peerPort);
    }

    return ret > 0;
}

/*!
    Returns the value of the option \a socketOption.
*/
int QAbstractSocketEngine::option(SocketOption socketOption) const
{
    Q_D(const QAbstractSocketEngine);
    return d->option(socketOption);
}

bool QAbstractSocketEngine::isReadNotificationEnabled() const
{
    Q_D(const QAbstractSocketEngine);
    return d->readNotifier && d->readNotifier->isEnabled();
}

void QAbstractSocketEngine::setReadNotificationEnabled(bool enable)
{
    Q_D(QAbstractSocketEngine);
    if (d->readNotifier) {
        d->readNotifier->setEnabled(enable);
    } else if (enable && d->threadData->eventDispatcher) {
        d->readNotifier = new QReadNotifier(d->socketDescriptor, this);
        d->readNotifier->setEnabled(true);
    }
}

bool QAbstractSocketEngine::isWriteNotificationEnabled() const
{
    Q_D(const QAbstractSocketEngine);
    return d->writeNotifier && d->writeNotifier->isEnabled();
}

void QAbstractSocketEngine::setWriteNotificationEnabled(bool enable)
{
    Q_D(QAbstractSocketEngine);
    if (d->writeNotifier) {
        d->writeNotifier->setEnabled(enable);
    } else if (enable && d->threadData->eventDispatcher) {
        d->writeNotifier = new QWriteNotifier(d->socketDescriptor, this);
        d->writeNotifier->setEnabled(true);
    }
}

bool QAbstractSocketEngine::isExceptionNotificationEnabled() const
{
    Q_D(const QAbstractSocketEngine);
    return d->exceptNotifier && d->exceptNotifier->isEnabled();
}

void QAbstractSocketEngine::setExceptionNotificationEnabled(bool enable)
{
    Q_D(QAbstractSocketEngine);
    if (d->exceptNotifier) {
        d->exceptNotifier->setEnabled(enable);
    } else if (enable && d->threadData->eventDispatcher) {
        d->exceptNotifier = new QExceptionNotifier(d->socketDescriptor, this);
        d->exceptNotifier->setEnabled(true);
    }
}

QT_END_NAMESPACE

#include "moc_qabstractsocketengine_p.h"
