/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#ifndef QBEARERENGINE_IMPL_H
#define QBEARERENGINE_IMPL_H

#include "qbearerengine_p.h"

#ifndef QT_NO_BEARERMANAGEMENT

QT_BEGIN_NAMESPACE

class QBearerEngineImpl : public QBearerEngine
{
    Q_OBJECT

public:
    enum ConnectionError {
        InterfaceLookupError = 0,
        ConnectError,
        OperationNotSupported,
        DisconnectionError,
    };

    QBearerEngineImpl(QObject *parent = nullptr) : QBearerEngine(parent) {}
    ~QBearerEngineImpl() {}

    virtual void connectToId(const QString &id) = 0;
    virtual void disconnectFromId(const QString &id) = 0;

    virtual QString getInterfaceFromId(const QString &id) = 0;

    virtual QNetworkSession::State sessionStateForId(const QString &id) = 0;

    virtual quint64 bytesWritten(const QString &) { return Q_UINT64_C(0); }
    virtual quint64 bytesReceived(const QString &) { return Q_UINT64_C(0); }
    virtual quint64 startTime(const QString &) { return Q_UINT64_C(0); }

Q_SIGNALS:
    void connectionError(const QString &id, QBearerEngineImpl::ConnectionError error);
};

QT_END_NAMESPACE

#endif // QT_NO_BEARERMANAGEMENT

#endif // QBEARERENGINE_IMPL_H
