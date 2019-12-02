/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QString>
#include <QHostInfo>
#include <QTcpServer>

#if !defined(QT_NO_GETADDRINFO)
#  include <sys/types.h>
# if defined(Q_OS_UNIX)
#  include <sys/socket.h>
# endif
#  include <netdb.h>
#endif

class QtNetworkSettings
{
public:

    static QString serverLocalName()
    {
        return QString::fromLatin1("w3");
    }
    static QString serverDomainName()
    {
        return QString::fromLatin1("org");
    }
    static QString serverName()
    {
        return serverLocalName() + QLatin1Char('.') + serverDomainName();
    }
    static QString wildcardServerName()
    {
        return QLatin1String("qt-test-server.wildcard.dev.") + serverDomainName();
    }

    static QHostAddress serverIP()
    {
        return QHostInfo::fromName(serverName()).addresses().first();
    }

    static QString serverIPs()
    {
        if (supportsIPv6() && systemSupportsIPv6()) {
            return QLatin1String("128.30.52.100 2603:400A:FFFF:804:801E:34:0:64");
        }
        return QLatin1String("128.30.52.100");
    }

    static bool compareReplyIMAP(QByteArray const& actual)
    {
        // Server greeting may contain capability, version and server name
        // But spec only requires "* OK" and "\r\n"
        // Match against a prefix and postfix that covers all Cyrus versions
        if (actual.startsWith("* OK ") && actual.endsWith("server ready\r\n")) {
            return true;
        }

        return false;
    }

    static bool compareReplyIMAPSSL(QByteArray const& actual)
    {
        return compareReplyIMAP(actual);
    }

    static bool compareReplyFtp(QByteArray const& actual)
    {
        QList<QByteArray> expected;

        // A few different vsFTPd versions.
        // Feel free to add more as needed
        expected << QByteArray( "220 (vsFTPd 2.0.5)\r\n221 Goodbye.\r\n" );
        expected << QByteArray( "220 (vsFTPd 2.2.2)\r\n221 Goodbye.\r\n" );

        Q_FOREACH (QByteArray const& ba, expected) {
            if (ba == actual) {
                return true;
            }
        }

        return false;
    }

    static bool supportsIPv6() {
        QTcpServer server;
        if (server.listen(QHostAddress("::1"))) {
            return true;
        }
        return false;
    }

    static bool systemSupportsIPv6() {
#if !defined(QT_NO_GETADDRINFO)
        // check if the system getaddrinfo can do IPv6 lookups
        struct addrinfo hint, *result = 0;
        memset(&hint, 0, sizeof hint);
        hint.ai_family = AF_UNSPEC;
# ifdef AI_ADDRCONFIG
        hint.ai_flags = AI_ADDRCONFIG;
# endif

        int res = getaddrinfo("::1", "80", &hint, &result);
        if (res == 0) {
            // this test worked
            freeaddrinfo(result);
            res = getaddrinfo("aaaa-single.test.qt-project.org", "80", &hint, &result);
            if (res == 0 && result != 0 && result->ai_family != AF_INET) {
                freeaddrinfo(result);
                return true;
            }
        }
#endif // QT_NO_GETADDRINFO
        return false;
    }

};

// ### remove, only Symbian needed that
#define Q_SET_DEFAULT_IAP

class QtNetworkSettingsInitializerCode {
public:
    QtNetworkSettingsInitializerCode() {
        QHostInfo testServerResult = QHostInfo::fromName(QtNetworkSettings::serverName());
        if (testServerResult.error() != QHostInfo::NoError) {
            qWarning() << "Could not lookup" << QtNetworkSettings::serverName();
            qWarning() << "Please configure the test environment!";
            qWarning() << "See /etc/hosts or network-settings.h";
            qFatal("Exiting");
        }
    }
};
QtNetworkSettingsInitializerCode qtNetworkSettingsInitializer;
