/****************************************************************************
**
** Copyright (C) 2009 David Faure <dfaure@kdab.net>
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

#ifndef QNETWORKPROXY_P_H
#define QNETWORKPROXY_P_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QT_NO_NETWORKPROXY

QT_BEGIN_NAMESPACE

class QSystemConfigurationProxyFactory : public QNetworkProxyFactory
{
public:
    QSystemConfigurationProxyFactory() : QNetworkProxyFactory() {}

    virtual QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery& query)
    {
        QList<QNetworkProxy> proxies = QNetworkProxyFactory::systemProxyForQuery(query);

        // Make sure NoProxy is in the list, so that QTcpServer can work:
        // it searches for the first proxy that can has the ListeningCapability capability
        // if none have (as is the case with HTTP proxies), it fails to bind.
        // NoProxy allows it to fallback to the 'no proxy' case and bind.
        proxies.append(QNetworkProxy::NoProxy);

        return proxies;
    }
};

QT_END_NAMESPACE

#endif // QT_NO_NETWORKINTERFACE

#endif

