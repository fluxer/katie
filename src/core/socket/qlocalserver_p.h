/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QLOCALSERVER_P_H
#define QLOCALSERVER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLocalServer class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QT_NO_LOCALSERVER

#include "qlocalserver.h"
#include "qobject_p.h"
#include "qqueue.h"
#include "qabstractsocketengine_p.h"
#include "qsocketnotifier.h"

QT_BEGIN_NAMESPACE

class QLocalServerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QLocalServer)

public:
    QLocalServerPrivate() :
            listenSocket(-1), socketNotifier(nullptr),
            maxPendingConnections(30), error(QAbstractSocket::UnknownSocketError)
    {
    }

    bool listen(const QString &name);
    static bool removeServer(const QString &name);
    void closeServer();
    void waitForNewConnection(int msec, bool *timedOut);
    void _q_onNewConnection();

    void setError(const QString &function);

    int listenSocket;
    QSocketNotifier *socketNotifier;

    QString serverName;
    QString fullServerName;
    int maxPendingConnections;
    QQueue<QLocalSocket*> pendingConnections;
    QString errorString;
    QAbstractSocket::SocketError error;
};

QT_END_NAMESPACE

#endif // QT_NO_LOCALSERVER

#endif // QLOCALSERVER_P_H

