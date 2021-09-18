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

#ifndef QNET_UNIX_P_H
#define QNET_UNIX_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of Qt code on Unix. This header file may change from version to
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "qcore_unix_p.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// for inet_addr
#include <netdb.h>
#include <arpa/inet.h>

QT_BEGIN_NAMESPACE

static inline int qt_safe_socket(int domain, int type, int protocol)
{
#if defined(SOCK_CLOEXEC)
    // since Linux 2.6.27
    return ::socket(domain, type | SOCK_CLOEXEC, protocol);
#else
    int fd = ::socket(domain, type, protocol);
    if (fd != -1)
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);
    return fd;
#endif
}

static inline int qt_safe_accept(int s, struct sockaddr *addr, QT_SOCKLEN_T *addrlen)
{
#if defined(QT_HAVE_ACCEPT4) && defined(SOCK_CLOEXEC)
    return ::accept4(s, addr, addrlen, SOCK_CLOEXEC);
#elif defined(QT_HAVE_PACCEPT) && defined(SOCK_CLOEXEC)
    return ::paccept(s, addr, addrlen, NULL, SOCK_CLOEXEC);
#else
    int fd = ::accept(s, addr, addrlen);
    if (fd != -1)
        ::fcntl(fd, F_SETFD, FD_CLOEXEC);
    return fd;
#endif
}

static inline int qt_safe_connect(int sockfd, const struct sockaddr *addr, QT_SOCKLEN_T addrlen)
{
    int ret;
    Q_EINTR_LOOP(ret, QT_SOCKET_CONNECT(sockfd, addr, addrlen));
    return ret;
}

#if defined(socket)
# undef socket
#endif
#if defined(accept)
# undef accept
#endif

// VxWorks' headers do not specify any const modifiers
static inline int qt_safe_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *to, QT_SOCKLEN_T tolen)
{
#ifdef MSG_NOSIGNAL
    flags |= MSG_NOSIGNAL;
#else
    qt_ignore_sigpipe();
#endif

    int ret;
    Q_EINTR_LOOP(ret, ::sendto(sockfd, buf, len, flags, to, tolen));
    return ret;
}

QT_END_NAMESPACE

#endif // QNET_UNIX_P_H
