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

#ifndef QABSTRACTSOCKET_P_H
#define QABSTRACTSOCKET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QAbstractSocket class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtNetwork/qabstractsocket.h"
#include "QtCore/qbytearray.h"
#include "QtCore/qlist.h"
#include "QtCore/qtimer.h"
#include "qringbuffer_p.h"
#include "qiodevice_p.h"
#include "qabstractsocketengine_p.h"
#include "qnetworkproxy.h"

QT_BEGIN_NAMESPACE

class QHostInfo;

class QAbstractSocketPrivate : public QIODevicePrivate, public QAbstractSocketEngineReceiver
{
    Q_DECLARE_PUBLIC(QAbstractSocket)
public:
    QAbstractSocketPrivate();
    virtual ~QAbstractSocketPrivate();

    // from QAbstractSocketEngineReceiver
    inline void readNotification() { canReadNotification(); }
    inline void writeNotification() { canWriteNotification(); }
    inline void exceptionNotification() {}
    void connectionNotification();
#ifndef QT_NO_NETWORKPROXY
    inline void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator) {
        Q_Q(QAbstractSocket);
        q->proxyAuthenticationRequired(proxy, authenticator);
    }
#endif

    bool canReadNotification();
    bool canWriteNotification();

    // slots
    void _q_connectToNextAddress();
    void _q_startConnecting(const QHostInfo &hostInfo);
    void _q_testConnection();
    void _q_abortConnectionAttempt();
    void _q_forceDisconnect();

    bool readSocketNotifierCalled;
    bool readSocketNotifierState;
    bool readSocketNotifierStateSet;

    bool emittedReadyRead;
    bool emittedBytesWritten;

    bool abortCalled;
    bool closeCalled;
    bool pendingClose;

    QString hostName;
    quint16 port;
    QHostAddress host;
    QList<QHostAddress> addresses;

    quint16 localPort;
    quint16 peerPort;
    QHostAddress localAddress;
    QHostAddress peerAddress;
    QString peerName;

    QAbstractSocketEngine *socketEngine;
    int cachedSocketDescriptor;

#ifndef QT_NO_NETWORKPROXY
    QNetworkProxy proxy;
    QNetworkProxy proxyInUse;
    void resolveProxy(const QString &hostName, quint16 port);
#else
    inline void resolveProxy(const QString &, quint16) { }
#endif
    inline void resolveProxy(quint16 port) { resolveProxy(QString(), port); }

    void resetSocketLayer();
    bool flush();

    bool initSocketLayer(QAbstractSocket::NetworkLayerProtocol protocol);
    void startConnectingByName(const QString &host);
    void fetchConnectionParameters();
    void setupSocketNotifiers();
    bool readFromSocket();

    qint64 readBufferMaxSize;
    QRingBuffer readBuffer;
    QRingBuffer writeBuffer;

    bool isBuffered;
    int blockingTimeout;

    QTimer *connectTimer;
    QTimer *disconnectTimer;

    int hostLookupId;

    QAbstractSocket::SocketType socketType;
    QAbstractSocket::SocketState state;

    QAbstractSocket::SocketError socketError;

    bool prePauseReadSocketNotifierState;
    bool prePauseWriteSocketNotifierState;
    bool prePauseExceptionSocketNotifierState;
    static void pauseSocketNotifiers(QAbstractSocket*);
    static void resumeSocketNotifiers(QAbstractSocket*);
    static QAbstractSocketEngine* getSocketEngine(QAbstractSocket*);
};

QT_END_NAMESPACE

#endif // QABSTRACTSOCKET_P_H
