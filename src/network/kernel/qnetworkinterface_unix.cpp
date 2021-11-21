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

#include "qset.h"
#include "qnetworkinterface.h"
#include "qnetworkinterface_p.h"
#include "qalgorithms.h"
#include "qnet_unix_p.h"
#include "qplatformdefs.h"
#include "qcorecommon_p.h"

#ifndef QT_NO_NETWORKINTERFACE

#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef Q_OS_SOLARIS
#  include <sys/sockio.h>
#endif

#ifdef QT_HAVE_GETIFADDRS
#  include <ifaddrs.h>
#else
#  include <sys/ioctl.h>
#endif

#ifndef SIOCGIFBRDADDR
#  define SIOCGIFBRDADDR 0x8919
#endif

QT_BEGIN_NAMESPACE

static QHostAddress addressFromSockaddr(sockaddr *sa)
{
    QHostAddress address;
    if (!sa)
        return address;

    if (sa->sa_family == AF_INET)
        address.setAddress(htonl(((sockaddr_in *)sa)->sin_addr.s_addr));
#ifndef QT_NO_IPV6
    else if (sa->sa_family == AF_INET6) {
        address.setAddress(((sockaddr_in6 *)sa)->sin6_addr.s6_addr);
        int scope = ((sockaddr_in6 *)sa)->sin6_scope_id;
        if (scope) {
#ifndef QT_NO_IPV6IFNAME
            QSTACKARRAY(char, scopeid, IFNAMSIZ);
            if (::if_indextoname(scope, scopeid)) {
                address.setScopeId(QString::fromLatin1(scopeid));
            } else
#endif
                address.setScopeId(QString::number(scope));
        }
    }
#endif
    return address;

}

static QNetworkInterface::InterfaceFlags convertFlags(uint rawFlags)
{
    QNetworkInterface::InterfaceFlags flags = 0;
    flags |= (rawFlags & IFF_UP) ? QNetworkInterface::IsUp : QNetworkInterface::InterfaceFlag(0);
    flags |= (rawFlags & IFF_RUNNING) ? QNetworkInterface::IsRunning : QNetworkInterface::InterfaceFlag(0);
    flags |= (rawFlags & IFF_BROADCAST) ? QNetworkInterface::CanBroadcast : QNetworkInterface::InterfaceFlag(0);
    flags |= (rawFlags & IFF_LOOPBACK) ? QNetworkInterface::IsLoopBack : QNetworkInterface::InterfaceFlag(0);
    flags |= (rawFlags & IFF_POINTOPOINT) ? QNetworkInterface::IsPointToPoint : QNetworkInterface::InterfaceFlag(0);
    flags |= (rawFlags & IFF_MULTICAST) ? QNetworkInterface::CanMulticast : QNetworkInterface::InterfaceFlag(0);
    return flags;
}

#ifdef QT_HAVE_GETIFADDRS

static QList<QNetworkInterfacePrivate *> createInterfaces(ifaddrs *rawList)
{
    QList<QNetworkInterfacePrivate *> interfaces;

    // make sure there's one entry for each interface
    for (ifaddrs *ptr = rawList; ptr; ptr = ptr->ifa_next) {
        // Get the interface index
        int ifindex = ::if_nametoindex(ptr->ifa_name);

        QList<QNetworkInterfacePrivate *>::Iterator if_it = interfaces.begin();
        for ( ; if_it != interfaces.end(); ++if_it)
            if ((*if_it)->index == ifindex)
                // this one has been added already
                break;

        if (if_it == interfaces.end()) {
            // none found, create
            QNetworkInterfacePrivate *iface = new QNetworkInterfacePrivate;
            interfaces << iface;

            iface->index = ifindex;
            iface->name = QString::fromLatin1(ptr->ifa_name);
            iface->flags = convertFlags(ptr->ifa_flags);
        }
    }

    return interfaces;
}

static QList<QNetworkInterfacePrivate *> interfaceListing()
{
    QList<QNetworkInterfacePrivate *> interfaces;

    int socket = qt_safe_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (socket == -1)
        return interfaces;      // error

    ifaddrs *interfaceListing;
    if (::getifaddrs(&interfaceListing) == -1) {
        // error
        qt_safe_close(socket);
        return interfaces;
    }

    interfaces = createInterfaces(interfaceListing);
    for (ifaddrs *ptr = interfaceListing; ptr; ptr = ptr->ifa_next) {
        // Get the interface index
        int ifindex = ::if_nametoindex(ptr->ifa_name);
        QNetworkInterfacePrivate *iface = 0;
        QList<QNetworkInterfacePrivate *>::Iterator if_it = interfaces.begin();
        for ( ; if_it != interfaces.end(); ++if_it)
            if ((*if_it)->index == ifindex) {
                // found this interface already
                iface = *if_it;
                break;
            }
        if (!iface) {
            // skip all non-IP interfaces
            continue;
        }

        QNetworkAddressEntry entry;
        entry.setIp(addressFromSockaddr(ptr->ifa_addr));
        if (entry.ip().isNull())
            // could not parse the address
            continue;

        entry.setNetmask(addressFromSockaddr(ptr->ifa_netmask));
        if (iface->flags & QNetworkInterface::CanBroadcast)
            entry.setBroadcast(addressFromSockaddr(ptr->ifa_broadaddr));

        iface->addressEntries << entry;
    }

    ::freeifaddrs(interfaceListing);
    qt_safe_close(socket);
    return interfaces;
}

#else // QT_HAVE_GETIFADDRS

static const int STORAGEBUFFER_GROWTH = 256;

static QSet<QByteArray> interfaceNames(int socket)
{
    QSet<QByteArray> result;
#ifdef QT_NO_IPV6IFNAME
    QByteArray storageBuffer;
    struct ifconf interfaceList;

    forever {
        // grow the storage buffer
        storageBuffer.resize(storageBuffer.size() + STORAGEBUFFER_GROWTH);
        interfaceList.ifc_buf = storageBuffer.data();
        interfaceList.ifc_len = storageBuffer.size();

        // get the interface list
        if (::ioctl(socket, SIOCGIFCONF, &interfaceList) >= 0) {
            if (int(interfaceList.ifc_len + sizeof(ifreq) + 64) < storageBuffer.size()) {
                // if the buffer was big enough, break
                storageBuffer.resize(interfaceList.ifc_len);
                break;
            }
        } else {
            // internal error
            return result;
        }
        if (storageBuffer.size() > 100000) {
            // out of space
            return result;
        }
    }

    int interfaceCount = interfaceList.ifc_len / sizeof(ifreq);
    for (int i = 0; i < interfaceCount; ++i) {
        QByteArray name = QByteArray(interfaceList.ifc_req[i].ifr_name);
        if (!name.isEmpty())
            result << name;
    }

    return result;
#else
    Q_UNUSED(socket);

    // use if_nameindex
    struct if_nameindex *interfaceList = ::if_nameindex();
    for (struct if_nameindex *ptr = interfaceList; ptr && ptr->if_name; ++ptr)
        result << ptr->if_name;

    ::if_freenameindex(interfaceList);
    return result;
#endif
}

static QNetworkInterfacePrivate *findInterface(int socket, QList<QNetworkInterfacePrivate *> &interfaces,
                                               struct ifreq &req)
{
    int ifindex = 0;

#ifndef QT_NO_IPV6IFNAME
    // Get the interface index
    ifindex = ::if_nametoindex(req.ifr_name);

    // find the interface data
    foreach (QNetworkInterfacePrivate *it, interfaces) {
        if (it->index == ifindex) {
            // existing interface
            return it;
        }
    }
#else
    // Search by name
    foreach (QNetworkInterfacePrivate *it, interfaces) {
        if (it->name == QString::fromLatin1(req.ifr_name)) {
            // existing interface
            return it;
        }
    }
#endif

    // new interface, create data:
    QNetworkInterfacePrivate *iface = new QNetworkInterfacePrivate;
    iface->index = ifindex;
    interfaces << iface;

#ifdef SIOCGIFNAME // not defined on Solaris
    // Get the canonical name
    QByteArray oldName = req.ifr_name;
    if (::ioctl(socket, SIOCGIFNAME, &req) >= 0) {
        iface->name = QString::fromLatin1(req.ifr_name);

        // reset the name:
        ::memcpy(req.ifr_name, oldName.constData(), qMin<int>(oldName.length() + 1, sizeof(req.ifr_name) - 1));
    } else
#endif
    {
        // use this name anyways
        iface->name = QString::fromLatin1(req.ifr_name);
    }

    // Get interface flags
    if (::ioctl(socket, SIOCGIFFLAGS, &req) >= 0) {
        iface->flags = convertFlags(req.ifr_flags);
    }

    // Get the HW address
    if (::ioctl(socket, SIOCGIFHWADDR, &req) >= 0) {
        uchar *addr = (uchar *)req.ifr_addr.sa_data;
        iface->hardwareAddress = iface->makeHwAddress(6, addr);
    }

    return iface;
}

static QList<QNetworkInterfacePrivate *> interfaceListing()
{
    QList<QNetworkInterfacePrivate *> interfaces;

    int socket = qt_safe_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (socket == -1)
        return interfaces;      // error

    foreach (const QByteArray &it, interfaceNames(socket)) {
        struct ifreq req;
        ::memset(&req, 0, sizeof(ifreq));
        ::memcpy(req.ifr_name, it.constData(), qMin<int>(it.length() + 1, sizeof(req.ifr_name) - 1));

        QNetworkInterfacePrivate *iface = findInterface(socket, interfaces, req);

        // Get the interface broadcast address
        QNetworkAddressEntry entry;
        if (iface->flags & QNetworkInterface::CanBroadcast) {
            if (::ioctl(socket, SIOCGIFBRDADDR, &req) >= 0) {
                sockaddr *sa = &req.ifr_addr;
                if (sa->sa_family == AF_INET)
                    entry.setBroadcast(addressFromSockaddr(sa));
            }
        }

        // Get the address of the interface
        if (::ioctl(socket, SIOCGIFADDR, &req) >= 0) {
            sockaddr *sa = &req.ifr_addr;
            entry.setIp(addressFromSockaddr(sa));
        }

        // Get the interface netmask
        if (::ioctl(socket, SIOCGIFNETMASK, &req) >= 0) {
            sockaddr *sa = &req.ifr_addr;
            entry.setNetmask(addressFromSockaddr(sa));
        }

        iface->addressEntries << entry;
    }

    qt_safe_close(socket);
    return interfaces;
}

#endif // QT_HAVE_GETIFADDRS

QList<QNetworkInterfacePrivate *> QNetworkInterfaceManager::scan()
{
    return interfaceListing();
}

QT_END_NAMESPACE

#endif // QT_NO_NETWORKINTERFACE


