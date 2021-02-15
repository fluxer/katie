/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#include <QString>
#include <QHostInfo>
#include <QLocalServer>

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
        if (supportsIPv6()) {
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
        QLocalServer server;
        return server.listen("::1");
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
