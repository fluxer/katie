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

#ifndef QLOCALSOCKET_P_H
#define QLOCALSOCKET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLocalSocket class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QT_NO_LOCALSOCKET

#include "qlocalsocket.h"
#include "qiodevice_p.h"
#include "qtimer.h"
#include "qabstractsocketengine_p.h"
#include "qtcpsocket.h"
#include "qsocketnotifier.h"

QT_BEGIN_NAMESPACE

class QLocalSocketPrivate : public QIODevicePrivate
{
    Q_DECLARE_PUBLIC(QLocalSocket)

public:
    QLocalSocketPrivate();
    void init();

    QTcpSocket unixSocket;
    QString generateErrorString(QLocalSocket::LocalSocketError, const QString &function) const;
    void errorOccurred(QLocalSocket::LocalSocketError, const QString &function);
    void _q_stateChanged(QAbstractSocket::SocketState newState);
    void _q_error(QAbstractSocket::SocketError newError);
    void _q_connectToSocket();
    void _q_abortConnectionAttempt();
    void cancelDelayedConnect();
    QSocketNotifier *delayConnect;
    QTimer *connectTimer;
    int connectingSocket;
    QString connectingName;
    QIODevice::OpenMode connectingOpenMode;

    QString serverName;
    QString fullServerName;
    QLocalSocket::LocalSocketState state;
};

QT_END_NAMESPACE

#endif // QT_NO_LOCALSOCKET

#endif // QLOCALSOCKET_P_H

