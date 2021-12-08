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

#include "qlocalserver.h"
#include "qlocalserver_p.h"
#include "qlocalsocket.h"
#include "qlocalsocket_p.h"
#include "qnet_unix_p.h"

#ifndef QT_NO_LOCALSERVER

#include <sys/socket.h>
#include <sys/un.h>

#include "qdebug.h"
#include "qdir.h"
#include "qdatetime.h"

QT_BEGIN_NAMESPACE

bool QLocalServerPrivate::removeServer(const QString &name)
{
    QString fileName;
    if (name.startsWith(QLatin1Char('/'))) {
        fileName = name;
    } else {
        fileName = QDir::tempPath() + QLatin1Char('/') + name;
    }
    const QStatInfo statinfo(fileName);
    if (statinfo.isFile())
        return QFile::remove(fileName);
    return true;
}

bool QLocalServerPrivate::listen(const QString &requestedServerName)
{
    Q_Q(QLocalServer);

    // determine the full server path
    if (requestedServerName.startsWith(QLatin1Char('/'))) {
        fullServerName = requestedServerName;
    } else {
        fullServerName = QDir::tempPath() + QLatin1Char('/') + requestedServerName;
    }
    serverName = requestedServerName;

    // create the unix socket
    listenSocket = qt_safe_socket(PF_UNIX, SOCK_STREAM, 0);
    if (-1 == listenSocket) {
        setError(QLatin1String("QLocalServer::listen"));
        closeServer();
        return false;
    }

    // Construct the unix address
    struct ::sockaddr_un addr;
    addr.sun_family = PF_UNIX;
    QByteArray latinFullServerName = fullServerName.toLatin1();
    if (sizeof(addr.sun_path) < (uint)latinFullServerName.size() + 1) {
        setError(QLatin1String("QLocalServer::listen"));
        closeServer();
        return false;
    }
    ::memcpy(addr.sun_path, latinFullServerName.constData(),
             latinFullServerName.size() + 1);


    // bind
    if(-1 == QT_SOCKET_BIND(listenSocket, (sockaddr *)&addr, sizeof(sockaddr_un))) {
        setError(QLatin1String("QLocalServer::listen"));
        // if address is in use already, just close the socket, but do not delete the file
        if(errno == EADDRINUSE)
            qt_safe_close(listenSocket);
        // otherwise, close the socket and delete the file
        else
            closeServer();
        listenSocket = -1;
        return false;
    }

    // listen for connections
    if (-1 == ::listen(listenSocket, SOMAXCONN)) {
        setError(QLatin1String("QLocalServer::listen"));
        closeServer();
        listenSocket = -1;
        if (error != QAbstractSocket::AddressInUseError)
            QFile::remove(fullServerName);
        return false;
    }
    Q_ASSERT(!socketNotifier);
    socketNotifier = new QSocketNotifier(listenSocket,
                                         QSocketNotifier::Read, q);
    q->connect(socketNotifier, SIGNAL(activated(int)),
               q, SLOT(_q_onNewConnection()));
    socketNotifier->setEnabled(maxPendingConnections > 0);
    return true;
}

/*!
    \internal

    \sa QLocalServer::closeServer()
 */
void QLocalServerPrivate::closeServer()
{
    if (socketNotifier) {
        socketNotifier->setEnabled(false); // Otherwise, closed socket is checked before deleter runs
        socketNotifier->deleteLater();
        socketNotifier = 0;
    }

    if (-1 != listenSocket)
        qt_safe_close(listenSocket);
    listenSocket = -1;

    if (!fullServerName.isEmpty())
        QFile::remove(fullServerName);
}

/*!
    \internal

    We have received a notification that we can read on the listen socket.
    Accept the new socket.
 */
void QLocalServerPrivate::_q_onNewConnection()
{
    Q_Q(QLocalServer);
    if (-1 == listenSocket)
        return;

    ::sockaddr_un addr;
    QT_SOCKLEN_T length = sizeof(sockaddr_un);
    int connectedSocket = qt_safe_accept(listenSocket, (sockaddr *)&addr, &length);
    if(-1 == connectedSocket) {
        setError(QLatin1String("QLocalSocket::activated"));
        closeServer();
    } else {
        socketNotifier->setEnabled(pendingConnections.size()
                                   <= maxPendingConnections);
        q->incomingConnection(connectedSocket);
    }
}

void QLocalServerPrivate::waitForNewConnection(int msec, bool *timedOut)
{
    struct pollfd fds;
    ::memset(&fds, 0, sizeof(struct pollfd));
    fds.fd = listenSocket;
    fds.events = POLLIN;
    int result = qt_safe_poll(&fds, 1, msec);
    if (result == -1) {
        setError(QLatin1String("QLocalServer::waitForNewConnection"));
        closeServer();
    }
    if (result > 0)
        _q_onNewConnection();
    if (timedOut)
        *timedOut = (result == 0);
}

void QLocalServerPrivate::setError(const QString &function)
{
    if (EAGAIN == errno)
        return;

    switch (errno) {
    case EACCES:
        errorString = QLocalServer::tr("%1: Permission denied").arg(function);
        error = QAbstractSocket::SocketAccessError;
        break;
    case ELOOP:
    case ENOENT:
    case ENAMETOOLONG:
    case EROFS:
    case ENOTDIR:
        errorString = QLocalServer::tr("%1: Name error").arg(function);
        error = QAbstractSocket::HostNotFoundError;
        break;
    case EADDRINUSE:
        errorString = QLocalServer::tr("%1: Address in use").arg(function);
        error = QAbstractSocket::AddressInUseError;
        break;

    default:
        errorString = QLocalServer::tr("%1: Unknown error %2")
                      .arg(function).arg(errno);
        error = QAbstractSocket::UnknownSocketError;
#if defined QLOCALSERVER_DEBUG
        qWarning() << errorString << "fullServerName:" << fullServerName;
#endif
    }
}

QT_END_NAMESPACE

#endif // QT_NO_LOCALSERVER


