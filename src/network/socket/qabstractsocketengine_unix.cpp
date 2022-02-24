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

//#define QABSTRACTSOCKETENGINE_DEBUG

#include "qabstractsocketengine_p.h"
#include "qnet_unix_p.h"
#include "qiodevice.h"
#include "qhostaddress.h"
#include "qelapsedtimer.h"
#include "qnetworkinterface.h"
#include "qplatformdefs.h"
#include "qcorecommon_p.h"

#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#ifndef QT_NO_IPV6IFNAME
#include <net/if.h>
#endif

#ifdef Q_OS_SOLARIS
#  include <sys/filio.h> // FIONREAD
#endif

QT_BEGIN_NAMESPACE

/*
    Extracts the port and address from a sockaddr, and stores them in
    \a port and \a addr if they are non-null.
*/
static inline void qt_socket_getPortAndAddress(const struct sockaddr_storage *ss, quint16 *port, QHostAddress *addr)
{
#if !defined(QT_NO_IPV6)
    if (ss->ss_family == AF_INET6) {
        struct sockaddr_in6 *si6 = (struct sockaddr_in6 *)ss;
        Q_IPV6ADDR tmp;
        memcpy(&tmp, &si6->sin6_addr, sizeof(tmp));
        if (addr) {
            addr->setAddress(tmp);
#ifndef QT_NO_IPV6IFNAME
            QSTACKARRAY(char, scopeid, IFNAMSIZ);
            if (::if_indextoname(si6->sin6_scope_id, scopeid)) {
                addr->setScopeId(QString::fromLatin1(scopeid));
            } else
#endif
            addr->setScopeId(QString::number(si6->sin6_scope_id));
        }
        if (port)
            *port = ntohs(si6->sin6_port);
        return;
    }
#endif

    struct sockaddr_in *si4 = (struct sockaddr_in *)ss;
    if (port)
        *port = ntohs(si4->sin_port);
    if (addr) {
        addr->setAddress(ntohl(si4->sin_addr.s_addr));
    }
}

/*! \internal

    Creates and returns a new socket descriptor of type \a socketType
    and \a socketProtocol.  Returns -1 on failure.
*/
bool QAbstractSocketEnginePrivate::createNewSocket(QAbstractSocket::SocketType socketType,
                                         QAbstractSocket::NetworkLayerProtocol socketProtocol)
{
#ifndef QT_NO_IPV6
    int protocol = (socketProtocol == QAbstractSocket::IPv6Protocol) ? AF_INET6 : AF_INET;
#else
    Q_UNUSED(socketProtocol);
    int protocol = AF_INET;
#endif
    int type = (socketType == QAbstractSocket::UdpSocket) ? SOCK_DGRAM : SOCK_STREAM;
#ifdef SOCK_NONBLOCK
    // Linux specific
    type |= SOCK_NONBLOCK;
#endif

    int socket = qt_safe_socket(protocol, type, 0);

    if (socket <= 0) {
        switch (errno) {
        case EPROTONOSUPPORT:
        case EAFNOSUPPORT:
        case EINVAL:
            setError(QAbstractSocket::UnsupportedSocketOperationError, ProtocolUnsupportedErrorString);
            break;
        case ENFILE:
        case EMFILE:
        case ENOBUFS:
        case ENOMEM:
            setError(QAbstractSocket::SocketResourceError, ResourceErrorString);
            break;
        case EACCES:
            setError(QAbstractSocket::SocketAccessError, AccessErrorString);
            break;
        default:
            break;
        }

        return false;
    }

#ifndef SOCK_NONBLOCK
    // Make the socket nonblocking.
    int flags = ::fcntl(socket, F_GETFL, 0);
    if (flags == -1 || ::fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        d->errorOccurred(QAbstractSocket::UnsupportedSocketOperationError, ProtocolUnsupportedErrorString);
        return;
    }
#endif

    socketDescriptor = socket;
    return true;
}

/*
    Returns the value of the socket option \a opt.
*/
int QAbstractSocketEnginePrivate::option(QAbstractSocketEngine::SocketOption opt) const
{
    Q_Q(const QAbstractSocketEngine);
    if (!q->isValid())
        return -1;

    int n = -1;
    int level = SOL_SOCKET; // default

    switch (opt) {
    case QAbstractSocketEngine::NonBlockingSocketOption:
        break;
    case QAbstractSocketEngine::BroadcastSocketOption:
        n = SO_BROADCAST;
        break;
    case QAbstractSocketEngine::AddressReusable:
        n = SO_REUSEADDR;
        break;
    case QAbstractSocketEngine::ReceiveOutOfBandData:
        n = SO_OOBINLINE;
        break;
    case QAbstractSocketEngine::LowDelayOption:
        level = IPPROTO_TCP;
        n = TCP_NODELAY;
        break;
    case QAbstractSocketEngine::KeepAliveOption:
        n = SO_KEEPALIVE;
        break;
    case QAbstractSocketEngine::MulticastTtlOption:
#ifndef QT_NO_IPV6
        if (socketProtocol == QAbstractSocket::IPv6Protocol) {
            level = IPPROTO_IPV6;
            n = IPV6_MULTICAST_HOPS;
            break;
        }
#endif
        level = IPPROTO_IP;
        n = IP_MULTICAST_TTL;
        break;
    case QAbstractSocketEngine::MulticastLoopbackOption:
#ifndef QT_NO_IPV6
        if (socketProtocol == QAbstractSocket::IPv6Protocol) {
            level = IPPROTO_IPV6;
            n = IPV6_MULTICAST_LOOP;
            break;
        }
#endif
        level = IPPROTO_IP;
        n = IP_MULTICAST_LOOP;
        break;
    }

    int v = -1;
    QT_SOCKLEN_T len = sizeof(v);
    if (::getsockopt(socketDescriptor, level, n, (char *) &v, &len) != -1)
        return v;

    return -1;
}


/*
    Sets the socket option \a opt to \a v.
*/
bool QAbstractSocketEnginePrivate::setOption(QAbstractSocketEngine::SocketOption opt, int v)
{
    Q_Q(QAbstractSocketEngine);
    if (!q->isValid())
        return false;

    int n = 0;
    int level = SOL_SOCKET; // default

    switch (opt) {
    case QAbstractSocketEngine::BroadcastSocketOption:
        n = SO_BROADCAST;
        break;
    case QAbstractSocketEngine::NonBlockingSocketOption: {
        // Make the socket nonblocking.
        int flags = ::fcntl(socketDescriptor, F_GETFL, 0);
        if (flags == -1) {
#ifdef QABSTRACTSOCKETENGINE_DEBUG
            perror("QAbstractSocketEnginePrivate::setOption(): fcntl(F_GETFL) failed");
#endif
            return false;
        }
        if (::fcntl(socketDescriptor, F_SETFL, flags | O_NONBLOCK) == -1) {
#ifdef QABSTRACTSOCKETENGINE_DEBUG
            perror("QAbstractSocketEnginePrivate::setOption(): fcntl(F_SETFL) failed");
#endif
            return false;
        }
        return true;
    }
    case QAbstractSocketEngine::AddressReusable:
        n = SO_REUSEADDR;
        break;
    case QAbstractSocketEngine::ReceiveOutOfBandData:
        n = SO_OOBINLINE;
        break;
    case QAbstractSocketEngine::LowDelayOption:
        level = IPPROTO_TCP;
        n = TCP_NODELAY;
        break;
    case QAbstractSocketEngine::KeepAliveOption:
        n = SO_KEEPALIVE;
        break;
    case QAbstractSocketEngine::MulticastTtlOption:
#ifndef QT_NO_IPV6
        if (socketProtocol == QAbstractSocket::IPv6Protocol) {
            level = IPPROTO_IPV6;
            n = IPV6_MULTICAST_HOPS;
            break;
        }
#endif
        level = IPPROTO_IP;
        n = IP_MULTICAST_TTL;
        break;
    case QAbstractSocketEngine::MulticastLoopbackOption:
#ifndef QT_NO_IPV6
        if (socketProtocol == QAbstractSocket::IPv6Protocol) {
            level = IPPROTO_IPV6;
            n = IPV6_MULTICAST_LOOP;
            break;
        }
#endif
        level = IPPROTO_IP;
        n = IP_MULTICAST_LOOP;
        break;
    }

    return ::setsockopt(socketDescriptor, level, n, (char *) &v, sizeof(v)) == 0;
}

bool QAbstractSocketEnginePrivate::nativeConnect(const QHostAddress &addr, quint16 port)
{
#ifdef QABSTRACTSOCKETENGINE_DEBUG
    qDebug("QAbstractSocketEnginePrivate::nativeConnect() : %d ", socketDescriptor);
#endif

    struct sockaddr_in sockAddrIPv4;
    struct sockaddr *sockAddrPtr = 0;
    QT_SOCKLEN_T sockAddrSize = 0;

#if !defined(QT_NO_IPV6)
    struct sockaddr_in6 sockAddrIPv6;

    if (addr.protocol() == QAbstractSocket::IPv6Protocol) {
        memset(&sockAddrIPv6, 0, sizeof(sockAddrIPv6));
        sockAddrIPv6.sin6_family = AF_INET6;
        sockAddrIPv6.sin6_port = htons(port);

        QString scopeid = addr.scopeId();
        bool ok;
        sockAddrIPv6.sin6_scope_id = scopeid.toInt(&ok);
#ifndef QT_NO_IPV6IFNAME
        if (!ok)
            sockAddrIPv6.sin6_scope_id = ::if_nametoindex(scopeid.toLatin1());
#endif
        Q_IPV6ADDR ip6 = addr.toIPv6Address();
        memcpy(&sockAddrIPv6.sin6_addr.s6_addr, &ip6, sizeof(ip6));

        sockAddrSize = sizeof(sockAddrIPv6);
        sockAddrPtr = (struct sockaddr *) &sockAddrIPv6;
    } else
#endif
    if (addr.protocol() == QAbstractSocket::IPv4Protocol) {
        memset(&sockAddrIPv4, 0, sizeof(sockAddrIPv4));
        sockAddrIPv4.sin_family = AF_INET;
        sockAddrIPv4.sin_port = htons(port);
        sockAddrIPv4.sin_addr.s_addr = htonl(addr.toIPv4Address());

        sockAddrSize = sizeof(sockAddrIPv4);
        sockAddrPtr = (struct sockaddr *) &sockAddrIPv4;
    }

    int connectResult = qt_safe_connect(socketDescriptor, sockAddrPtr, sockAddrSize);
    if (connectResult == -1) {
        switch (errno) {
        case EISCONN:
            socketState = QAbstractSocket::ConnectedState;
            break;
        case ECONNREFUSED:
        case EINVAL:
            setError(QAbstractSocket::ConnectionRefusedError, ConnectionRefusedErrorString);
            socketState = QAbstractSocket::UnconnectedState;
            break;
        case ETIMEDOUT:
            setError(QAbstractSocket::NetworkError, ConnectionTimeOutErrorString);
            break;
        case EHOSTUNREACH:
            setError(QAbstractSocket::NetworkError, HostUnreachableErrorString);
            socketState = QAbstractSocket::UnconnectedState;
            break;
        case ENETUNREACH:
            setError(QAbstractSocket::NetworkError, NetworkUnreachableErrorString);
            socketState = QAbstractSocket::UnconnectedState;
            break;
        case EADDRINUSE:
            setError(QAbstractSocket::NetworkError, AddressInuseErrorString);
            break;
        case EINPROGRESS:
        case EALREADY:
            setError(QAbstractSocket::UnfinishedSocketOperationError, ResourceErrorString);
            socketState = QAbstractSocket::ConnectingState;
            break;
        case EAGAIN:
            setError(QAbstractSocket::UnfinishedSocketOperationError, ResourceErrorString);
            break;
        case EACCES:
        case EPERM:
            setError(QAbstractSocket::SocketAccessError, AccessErrorString);
            socketState = QAbstractSocket::UnconnectedState;
            break;
        case EAFNOSUPPORT:
        case EBADF:
        case EFAULT:
        case ENOTSOCK:
            socketState = QAbstractSocket::UnconnectedState;
        default:
            break;
        }

        if (socketState != QAbstractSocket::ConnectedState) {
#if defined (QABSTRACTSOCKETENGINE_DEBUG)
            qDebug("QAbstractSocketEnginePrivate::nativeConnect(%s, %i) == false (%s)",
                   addr.toString().toLatin1().constData(), port,
                   socketState == QAbstractSocket::ConnectingState
                   ? "Connection in progress" : socketErrorString.toLatin1().constData());
#endif
            return false;
        }
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeConnect(%s, %i) == true",
           addr.toString().toLatin1().constData(), port);
#endif

    socketState = QAbstractSocket::ConnectedState;
    return true;
}

bool QAbstractSocketEnginePrivate::nativeBind(const QHostAddress &address, quint16 port)
{
    struct sockaddr_in sockAddrIPv4;
    struct sockaddr *sockAddrPtr = 0;
    QT_SOCKLEN_T sockAddrSize = 0;

#if !defined(QT_NO_IPV6)
    struct sockaddr_in6 sockAddrIPv6;

    if (address.protocol() == QAbstractSocket::IPv6Protocol) {
        memset(&sockAddrIPv6, 0, sizeof(sockAddrIPv6));
        sockAddrIPv6.sin6_family = AF_INET6;
        sockAddrIPv6.sin6_port = htons(port);
#ifndef QT_NO_IPV6IFNAME
        sockAddrIPv6.sin6_scope_id = ::if_nametoindex(address.scopeId().toLatin1().data());
#else
        sockAddrIPv6.sin6_scope_id = address.scopeId().toInt();
#endif
        Q_IPV6ADDR tmp = address.toIPv6Address();
        memcpy(&sockAddrIPv6.sin6_addr.s6_addr, &tmp, sizeof(tmp));
        sockAddrSize = sizeof(sockAddrIPv6);
        sockAddrPtr = (struct sockaddr *) &sockAddrIPv6;
    } else
#endif
    if (address.protocol() == QAbstractSocket::IPv4Protocol) {
        memset(&sockAddrIPv4, 0, sizeof(sockAddrIPv4));
        sockAddrIPv4.sin_family = AF_INET;
        sockAddrIPv4.sin_port = htons(port);
        sockAddrIPv4.sin_addr.s_addr = htonl(address.toIPv4Address());
        sockAddrSize = sizeof(sockAddrIPv4);
        sockAddrPtr = (struct sockaddr *) &sockAddrIPv4;
    }

    int bindResult = QT_SOCKET_BIND(socketDescriptor, sockAddrPtr, sockAddrSize);

    if (bindResult < 0) {
        switch(errno) {
        case EADDRINUSE:
            setError(QAbstractSocket::AddressInUseError, AddressInuseErrorString);
            break;
        case EACCES:
            setError(QAbstractSocket::SocketAccessError, AddressProtectedErrorString);
            break;
        case EINVAL:
            setError(QAbstractSocket::UnsupportedSocketOperationError, OperationUnsupportedErrorString);
            break;
        case EADDRNOTAVAIL:
            setError(QAbstractSocket::SocketAddressNotAvailableError, AddressNotAvailableErrorString);
            break;
        default:
            break;
        }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
        qDebug("QAbstractSocketEnginePrivate::nativeBind(%s, %i) == false (%s)",
               address.toString().toLatin1().constData(), port, socketErrorString.toLatin1().constData());
#endif

        return false;
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeBind(%s, %i) == true",
           address.toString().toLatin1().constData(), port);
#endif
    socketState = QAbstractSocket::BoundState;
    return true;
}

bool QAbstractSocketEnginePrivate::nativeListen()
{
    if (::listen(socketDescriptor, SOMAXCONN) < 0) {
        switch (errno) {
        case EADDRINUSE:
            setError(QAbstractSocket::AddressInUseError,
                     PortInuseErrorString);
            break;
        case EOPNOTSUPP:
            setError(QAbstractSocket::UnsupportedSocketOperationError,
                     OperationUnsupportedErrorString);
            break;
        default:
            break;
        }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
        qDebug("QAbstractSocketEnginePrivate::nativeListen(%i) == false (%s)",
               SOMAXCONN, socketErrorString.toLatin1().constData());
#endif
        return false;
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeListen(%i) == true", SOMAXCONN);
#endif

    socketState = QAbstractSocket::ListeningState;
    return true;
}

int QAbstractSocketEnginePrivate::nativeAccept()
{
    int acceptedDescriptor = qt_safe_accept(socketDescriptor, 0, 0);
    if (acceptedDescriptor == -1) {
        switch (errno) {
        case EOPNOTSUPP:
            setError(QAbstractSocket::UnsupportedSocketOperationError,
                     OperationUnsupportedErrorString);
            break;
        default:
            break;
        }
    }

    return acceptedDescriptor;
}

#ifndef QT_NO_NETWORKINTERFACE

static bool multicastMembershipHelper(QAbstractSocketEnginePrivate *d,
                                      int how6,
                                      int how4,
                                      const QHostAddress &groupAddress,
                                      const QNetworkInterface &interface)
{
    int level = 0;
    int sockOpt = 0;
    void *sockArg;
    QT_SOCKLEN_T sockArgSize;

    ip_mreq mreq4;
#ifndef QT_NO_IPV6
    ipv6_mreq mreq6;

    if (groupAddress.protocol() == QAbstractSocket::IPv6Protocol) {
        level = IPPROTO_IPV6;
        sockOpt = how6;
        sockArg = &mreq6;
        sockArgSize = sizeof(mreq6);
        memset(&mreq6, 0, sizeof(mreq6));
        Q_IPV6ADDR ip6 = groupAddress.toIPv6Address();
        memcpy(&mreq6.ipv6mr_multiaddr, &ip6, sizeof(ip6));
        mreq6.ipv6mr_interface = interface.index();
    } else
#endif
    if (groupAddress.protocol() == QAbstractSocket::IPv4Protocol) {
        level = IPPROTO_IP;
        sockOpt = how4;
        sockArg = &mreq4;
        sockArgSize = sizeof(mreq4);
        memset(&mreq4, 0, sizeof(mreq4));
        mreq4.imr_multiaddr.s_addr = htonl(groupAddress.toIPv4Address());

        if (interface.isValid()) {
            QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
            if (!addressEntries.isEmpty()) {
                QHostAddress firstIP = addressEntries.first().ip();
                mreq4.imr_interface.s_addr = htonl(firstIP.toIPv4Address());
            } else {
                d->setError(QAbstractSocket::NetworkError,
                            QAbstractSocketEnginePrivate::NetworkUnreachableErrorString);
                return false;
            }
        } else {
            mreq4.imr_interface.s_addr = INADDR_ANY;
        }
    } else {
        // unreachable
        d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                    QAbstractSocketEnginePrivate::ProtocolUnsupportedErrorString);
        return false;
    }

    int res = ::setsockopt(d->socketDescriptor, level, sockOpt, sockArg, sockArgSize);
    if (res == -1) {
        switch (errno) {
        case ENOPROTOOPT:
            d->setError(QAbstractSocket::UnsupportedSocketOperationError,
                        QAbstractSocketEnginePrivate::OperationUnsupportedErrorString);
            break;
        case EADDRNOTAVAIL:
            d->setError(QAbstractSocket::SocketAddressNotAvailableError,
                        QAbstractSocketEnginePrivate::AddressNotAvailableErrorString);
            break;
        default:
            d->setError(QAbstractSocket::UnknownSocketError,
                        QAbstractSocketEnginePrivate::UnknownSocketErrorString);
            break;
        }
        return false;
    }
    return true;
}

bool QAbstractSocketEnginePrivate::nativeJoinMulticastGroup(const QHostAddress &groupAddress,
                                                          const QNetworkInterface &interface)
{
    return multicastMembershipHelper(this,
#ifndef QT_NO_IPV6
                                     IPV6_JOIN_GROUP,
#else
                                     0,
#endif
                                     IP_ADD_MEMBERSHIP,
                                     groupAddress,
                                     interface);
}

bool QAbstractSocketEnginePrivate::nativeLeaveMulticastGroup(const QHostAddress &groupAddress,
                                                           const QNetworkInterface &interface)
{
    return multicastMembershipHelper(this,
#ifndef QT_NO_IPV6
                                     IPV6_LEAVE_GROUP,
#else
                                     0,
#endif
                                     IP_DROP_MEMBERSHIP,
                                     groupAddress,
                                     interface);
}

QNetworkInterface QAbstractSocketEnginePrivate::nativeMulticastInterface() const
{
#ifndef QT_NO_IPV6
    if (socketProtocol == QAbstractSocket::IPv6Protocol) {
        uint v;
        QT_SOCKLEN_T sizeofv = sizeof(v);
        if (::getsockopt(socketDescriptor, IPPROTO_IPV6, IPV6_MULTICAST_IF, &v, &sizeofv) == -1)
            return QNetworkInterface();
        return QNetworkInterface::interfaceFromIndex(v);
    }
#endif

    struct in_addr v = { 0 };
    QT_SOCKLEN_T sizeofv = sizeof(v);
    if (::getsockopt(socketDescriptor, IPPROTO_IP, IP_MULTICAST_IF, &v, &sizeofv) == -1)
        return QNetworkInterface();
    if (v.s_addr != 0 && sizeofv >= sizeof(v)) {
        QHostAddress ipv4(ntohl(v.s_addr));
        QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
        for (int i = 0; i < ifaces.count(); ++i) {
            const QNetworkInterface &iface = ifaces.at(i);
            QList<QNetworkAddressEntry> entries = iface.addressEntries();
            for (int j = 0; j < entries.count(); ++j) {
                const QNetworkAddressEntry &entry = entries.at(j);
                if (entry.ip() == ipv4)
                    return iface;
            }
        }
    }
    return QNetworkInterface();
}

bool QAbstractSocketEnginePrivate::nativeSetMulticastInterface(const QNetworkInterface &iface)
{
#ifndef QT_NO_IPV6
    if (socketProtocol == QAbstractSocket::IPv6Protocol) {
        uint v = iface.index();
        return (::setsockopt(socketDescriptor, IPPROTO_IPV6, IPV6_MULTICAST_IF, &v, sizeof(v)) != -1);
    }
#endif

    struct in_addr v;
    if (iface.isValid()) {
        QList<QNetworkAddressEntry> entries = iface.addressEntries();
        for (int i = 0; i < entries.count(); ++i) {
            const QNetworkAddressEntry &entry = entries.at(i);
            const QHostAddress &ip = entry.ip();
            if (ip.protocol() == QAbstractSocket::IPv4Protocol) {
                v.s_addr = htonl(ip.toIPv4Address());
                int r = ::setsockopt(socketDescriptor, IPPROTO_IP, IP_MULTICAST_IF, &v, sizeof(v));
                if (r != -1)
                    return true;
            }
        }
        return false;
    }

    v.s_addr = INADDR_ANY;
    return (::setsockopt(socketDescriptor, IPPROTO_IP, IP_MULTICAST_IF, &v, sizeof(v)) != -1);
}

#endif // QT_NO_NETWORKINTERFACE

qint64 QAbstractSocketEnginePrivate::nativeBytesAvailable() const
{
    int nbytes = 0;
    // gives shorter than true amounts on Unix domain sockets.
    qint64 available = 0;
    if (::ioctl(socketDescriptor, FIONREAD, (char *) &nbytes) >= 0)
        available = (qint64) nbytes;

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeBytesAvailable() == %lli", available);
#endif
    return available;
}

bool QAbstractSocketEnginePrivate::nativeHasPendingDatagrams() const
{
    // Create a sockaddr struct and reset its port number.
    struct sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    QT_SOCKLEN_T sz = sizeof(ss);

    struct sockaddr* sockAddrPtr = (struct sockaddr*) &ss;

    // Peek 0 bytes into the next message. The size of the message may
    // well be 0, so we can't check recvfrom's return value.
    char c;
    ssize_t readBytes = qt_safe_recvfrom(socketDescriptor, &c, 1, MSG_PEEK, sockAddrPtr, &sz);

    // If there's no error, or if our buffer was too small, there must be a
    // pending datagram.
    bool result = (readBytes != -1) || errno == EMSGSIZE;

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeHasPendingDatagrams() == %s",
           result ? "true" : "false");
#endif
    return result;
}

qint64 QAbstractSocketEnginePrivate::nativePendingDatagramSize() const
{
    ssize_t udpMessagePeekBufferSize = QT_BUFFSIZE;
    ssize_t recvResult = -1;

    for (;;) {
        QSTACKARRAY(char, udpMessagePeekBuffer, udpMessagePeekBufferSize);

        recvResult = ::recv(socketDescriptor, udpMessagePeekBuffer,
            udpMessagePeekBufferSize, MSG_PEEK);
        if (recvResult == -1 && errno == EINTR)
            continue;

        if (recvResult != udpMessagePeekBufferSize)
            break;

        udpMessagePeekBufferSize = (udpMessagePeekBufferSize * 2);
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativePendingDatagramSize() == %i", recvResult);
#endif

    return qint64(recvResult);
}

qint64 QAbstractSocketEnginePrivate::nativeReceiveDatagram(char *data, qint64 maxSize,
                                                    QHostAddress *address, quint16 *port)
{
    struct sockaddr_storage ss;
    memset(&ss, 0, sizeof(ss));
    QT_SOCKLEN_T sockAddrSize = sizeof(ss);

    struct sockaddr* sockAddrPtr = (struct sockaddr*) &ss;

    char c;
    ssize_t recvFromResult = qt_safe_recvfrom(socketDescriptor, maxSize ? data : &c,
                                              maxSize ? maxSize : 1, 0, sockAddrPtr, &sockAddrSize);

    if (recvFromResult == -1) {
        setError(QAbstractSocket::NetworkError, ReceiveDatagramErrorString);
    } else if (port || address) {
        qt_socket_getPortAndAddress(&ss, port, address);
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeReceiveDatagram(%p \"%s\", %lli, %s, %i) == %lli",
           data, qt_prettyDebug(data, qMin(recvFromResult, ssize_t(16)), recvFromResult).data(), maxSize,
           address ? address->toString().toLatin1().constData() : "(nil)",
           port ? *port : 0, (qint64) recvFromResult);
#endif

    return qint64(maxSize ? recvFromResult : recvFromResult == -1 ? -1 : 0);
}

qint64 QAbstractSocketEnginePrivate::nativeSendDatagram(const char *data, qint64 len,
                                                   const QHostAddress &host, quint16 port)
{
    struct sockaddr_in sockAddrIPv4;
    struct sockaddr *sockAddrPtr = 0;
    QT_SOCKLEN_T sockAddrSize = 0;

#if !defined(QT_NO_IPV6)
    struct sockaddr_in6 sockAddrIPv6;
    if (host.protocol() == QAbstractSocket::IPv6Protocol) {
        memset(&sockAddrIPv6, 0, sizeof(sockAddrIPv6));
        sockAddrIPv6.sin6_family = AF_INET6;
        sockAddrIPv6.sin6_port = htons(port);

        Q_IPV6ADDR tmp = host.toIPv6Address();
        memcpy(&sockAddrIPv6.sin6_addr.s6_addr, &tmp, sizeof(tmp));
        QString scopeid = host.scopeId();
        bool ok;
        sockAddrIPv6.sin6_scope_id = scopeid.toInt(&ok);
#ifndef QT_NO_IPV6IFNAME
        if (!ok)
            sockAddrIPv6.sin6_scope_id = ::if_nametoindex(scopeid.toLatin1());
#endif
        sockAddrSize = sizeof(sockAddrIPv6);
        sockAddrPtr = (struct sockaddr *)&sockAddrIPv6;
    } else
#endif
    if (host.protocol() == QAbstractSocket::IPv4Protocol) {
        memset(&sockAddrIPv4, 0, sizeof(sockAddrIPv4));
        sockAddrIPv4.sin_family = AF_INET;
        sockAddrIPv4.sin_port = htons(port);
        sockAddrIPv4.sin_addr.s_addr = htonl(host.toIPv4Address());
        sockAddrSize = sizeof(sockAddrIPv4);
        sockAddrPtr = (struct sockaddr *)&sockAddrIPv4;
    }

    ssize_t sentBytes = qt_safe_sendto(socketDescriptor, data, len,
                                       0, sockAddrPtr, sockAddrSize);

    if (sentBytes < 0) {
        switch (errno) {
        case EMSGSIZE:
            setError(QAbstractSocket::DatagramTooLargeError, DatagramTooLargeErrorString);
            break;
        default:
            setError(QAbstractSocket::NetworkError, SendDatagramErrorString);
            break;
        }
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEngine::sendDatagram(%p \"%s\", %lli, \"%s\", %i) == %lli", data,
           qt_prettyDebug(data, qMin<int>(len, 16), len).data(), len, host.toString().toLatin1().constData(),
           port, (qint64) sentBytes);
#endif

    return qint64(sentBytes);
}

bool QAbstractSocketEnginePrivate::fetchConnectionParameters()
{
    localPort = 0;
    localAddress.clear();
    peerPort = 0;
    peerAddress.clear();

    if (socketDescriptor == -1)
        return false;

    struct sockaddr_storage ss;
    QT_SOCKLEN_T sockAddrSize = sizeof(ss);
    memset(&ss, 0, sizeof(ss));

    struct sockaddr* sockAddrPtr = (struct sockaddr*) &ss;

    // Determine local address
    if (::getsockname(socketDescriptor, sockAddrPtr, &sockAddrSize) == 0) {
        qt_socket_getPortAndAddress(&ss, &localPort, &localAddress);

        // Determine protocol family
        switch (ss.ss_family) {
        case AF_INET:
            socketProtocol = QAbstractSocket::IPv4Protocol;
            break;
#if !defined (QT_NO_IPV6)
        case AF_INET6:
            socketProtocol = QAbstractSocket::IPv6Protocol;
            break;
#endif
        default:
            socketProtocol = QAbstractSocket::UnknownNetworkLayerProtocol;
            break;
        }

    } else if (errno == EBADF) {
        setError(QAbstractSocket::UnsupportedSocketOperationError, InvalidSocketErrorString);
        return false;
    }

    // Determine the remote address
    if (::getpeername(socketDescriptor, sockAddrPtr, &sockAddrSize) == 0)
        qt_socket_getPortAndAddress(&ss, &peerPort, &peerAddress);

    // Determine the socket type (UDP/TCP)
    int value = 0;
    QT_SOCKLEN_T valueSize = sizeof(int);
    if (::getsockopt(socketDescriptor, SOL_SOCKET, SO_TYPE, &value, &valueSize) == 0) {
        if (value == SOCK_STREAM)
            socketType = QAbstractSocket::TcpSocket;
        else if (value == SOCK_DGRAM)
            socketType = QAbstractSocket::UdpSocket;
        else
            socketType = QAbstractSocket::UnknownSocketType;
    }
#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    QString socketProtocolStr = "UnknownProtocol";
    if (socketProtocol == QAbstractSocket::IPv4Protocol) socketProtocolStr = "IPv4Protocol";
    else if (socketProtocol == QAbstractSocket::IPv6Protocol) socketProtocolStr = "IPv6Protocol";

    QString socketTypeStr = "UnknownSocketType";
    if (socketType == QAbstractSocket::TcpSocket) socketTypeStr = "TcpSocket";
    else if (socketType == QAbstractSocket::UdpSocket) socketTypeStr = "UdpSocket";

    qDebug("QAbstractSocketEnginePrivate::fetchConnectionParameters() local == %s:%i,"
           " peer == %s:%i, socket == %s - %s",
           localAddress.toString().toLatin1().constData(), localPort,
           peerAddress.toString().toLatin1().constData(), peerPort,socketTypeStr.toLatin1().constData(),
           socketProtocolStr.toLatin1().constData());
#endif
    return true;
}

void QAbstractSocketEnginePrivate::nativeClose()
{
#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEngine::nativeClose()");
#endif

    qt_safe_close(socketDescriptor);
}

qint64 QAbstractSocketEnginePrivate::nativeWrite(const char *data, qint64 len)
{
    Q_Q(QAbstractSocketEngine);

    qint64 writtenBytes = qt_safe_write_nosignal(socketDescriptor, data, len);

    if (writtenBytes < 0) {
        switch (errno) {
        case EPIPE:
        case ECONNRESET:
            writtenBytes = -1;
            setError(QAbstractSocket::RemoteHostClosedError, RemoteHostClosedErrorString);
            q->close();
            break;
        case EAGAIN:
            writtenBytes = 0;
            break;
        case EMSGSIZE:
            setError(QAbstractSocket::DatagramTooLargeError, DatagramTooLargeErrorString);
            break;
        default:
            break;
        }
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeWrite(%p \"%s\", %llu) == %i",
           data, qt_prettyDebug(data, qMin((int) len, 16),
                                (int) len).data(), len, (int) writtenBytes);
#endif

    return writtenBytes;
}
/*
*/
qint64 QAbstractSocketEnginePrivate::nativeRead(char *data, qint64 maxSize)
{
    Q_Q(QAbstractSocketEngine);
    if (!q->isValid()) {
        qWarning("QAbstractSocketEngine::nativeRead: Invalid socket");
        return -1;
    }

    qint64 r = qt_safe_read(socketDescriptor, data, maxSize);

    if (r < 0) {
        r = -1;
        switch (errno) {
#if EWOULDBLOCK-0 && EWOULDBLOCK != EAGAIN
        case EWOULDBLOCK:
#endif
        case EAGAIN:
            // No data was available for reading
            r = -2;
            break;
        case EBADF:
        case EINVAL:
        case EIO:
            //error string is now set in read(), not here in nativeRead()
            break;
        case ECONNRESET:
            r = 0;
            break;
        default:
            break;
        }
    }

#if defined (QABSTRACTSOCKETENGINE_DEBUG)
    qDebug("QAbstractSocketEnginePrivate::nativeRead(%p \"%s\", %llu) == %i",
           data, qt_prettyDebug(data, qMin(r, qint64(16)), r).data(),
           maxSize, r);
#endif

    return r;
}

int QAbstractSocketEnginePrivate::nativeSelect(int timeout, bool selectForRead) const
{
    struct pollfd fds;
    ::memset(&fds, 0, sizeof(struct pollfd));
    fds.fd = socketDescriptor;
    fds.events = (selectForRead ? POLLIN : POLLOUT);
    return qt_safe_poll(&fds, 1, timeout);
}

int QAbstractSocketEnginePrivate::nativeSelect(int timeout, bool checkRead, bool checkWrite,
                       bool *selectForRead, bool *selectForWrite) const
{
    struct pollfd fds;
    ::memset(&fds, 0, sizeof(struct pollfd));
    fds.fd = socketDescriptor;
    fds.events = 0;
    if (checkRead) {
        fds.events |= POLLIN;
    }
    if (checkWrite) {
        fds.events |= POLLOUT;
    }

    int ret = qt_safe_poll(&fds, 1, timeout);
    if (ret > 0) {
        *selectForRead = ((fds.revents & POLLIN) != 0);
        *selectForWrite = ((fds.revents & POLLOUT) != 0);
    }
    return ret;
}

QT_END_NAMESPACE


