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

#include "qsharednetworksession_p.h"
#include "qbearerengine_p.h"

#ifndef QT_NO_BEARERMANAGEMENT

QT_BEGIN_NAMESPACE

thread_local QSharedNetworkSessionManager* tls = Q_NULLPTR;

inline QSharedNetworkSessionManager* sharedNetworkSessionManager()
{
    if (!tls) {
        tls = new QSharedNetworkSessionManager;
    }
    return tls;
}

static void doDeleteLater(QObject* obj)
{
    obj->deleteLater();
}

QSharedPointer<QNetworkSession> QSharedNetworkSessionManager::getSession(const QNetworkConfiguration &config)
{
    QSharedNetworkSessionManager *m(sharedNetworkSessionManager());
    //if already have a session, return it
    if (m->sessions.contains(config)) {
        QSharedPointer<QNetworkSession> p = m->sessions.value(config).toStrongRef();
        if (!p.isNull())
            return p;
    }
    //otherwise make one
    QSharedPointer<QNetworkSession> session(new QNetworkSession(config), doDeleteLater);
    m->sessions[config] = session;
    return session;
}

void QSharedNetworkSessionManager::setSession(QNetworkConfiguration config, QSharedPointer<QNetworkSession> session)
{
    QSharedNetworkSessionManager *m(sharedNetworkSessionManager());
    m->sessions[config] = session;
}

uint qHash(const QNetworkConfiguration& config)
{
    return ((uint)config.type()) | (((uint)config.bearerType()) << 8) | (((uint)config.purpose()) << 16);
}

QT_END_NAMESPACE

#endif // QT_NO_BEARERMANAGEMENT


