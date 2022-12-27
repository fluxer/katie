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

#ifndef QABSTRACTSOCKETENGINE_P_H
#define QABSTRACTSOCKETENGINE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtNetwork/qhostaddress.h"
#include "QtNetwork/qabstractsocket.h"
#include "qobject_p.h"

QT_BEGIN_NAMESPACE

class QAbstractSocketEnginePrivate;
#ifndef QT_NO_NETWORKINTERFACE
class QNetworkInterface;
#endif
class QSocketNotifier;

class QAbstractSocketEngineReceiver {
public:
    virtual ~QAbstractSocketEngineReceiver() {}
    virtual void readNotification() = 0;
    virtual void writeNotification() = 0;
    virtual void connectionNotification() = 0;
};

class Q_AUTOTEST_EXPORT QAbstractSocketEngine : public QObject
{
    Q_OBJECT
public:

    QAbstractSocketEngine(QObject *parent = nullptr);
    ~QAbstractSocketEngine();

    enum SocketOption {
        NonBlockingSocketOption,
        BroadcastSocketOption,
        AddressReusable,
        ReceiveOutOfBandData,
        LowDelayOption,
        KeepAliveOption,
        MulticastTtlOption,
        MulticastLoopbackOption
    };

    bool initialize(QAbstractSocket::SocketType type, QAbstractSocket::NetworkLayerProtocol protocol = QAbstractSocket::IPv4Protocol);

    bool initialize(int socketDescriptor, QAbstractSocket::SocketState socketState = QAbstractSocket::ConnectedState);

    int socketDescriptor() const;

    bool isValid() const;

    bool connectToHost(const QHostAddress &address, quint16 port);
    bool bind(const QHostAddress &address, quint16 port);
    bool listen();
    int accept();
    void close();

    qint64 bytesAvailable() const;

    qint64 read(char *data, qint64 maxlen);
    qint64 write(const char *data, qint64 len);

#ifndef QT_NO_UDPSOCKET
#ifndef QT_NO_NETWORKINTERFACE
    bool joinMulticastGroup(const QHostAddress &groupAddress,
                            const QNetworkInterface &iface);
    bool leaveMulticastGroup(const QHostAddress &groupAddress,
                             const QNetworkInterface &iface);
    QNetworkInterface multicastInterface() const;
    bool setMulticastInterface(const QNetworkInterface &iface);
#endif // QT_NO_NETWORKINTERFACE

    qint64 readDatagram(char *data, qint64 maxlen, QHostAddress *addr = 0,
                        quint16 *port = 0);
    qint64 writeDatagram(const char *data, qint64 len, const QHostAddress &addr,
                         quint16 port);
    bool hasPendingDatagrams() const;
    qint64 pendingDatagramSize() const;
#endif // QT_NO_UDPSOCKET

    qint64 bytesToWrite() const ;

    int option(SocketOption option) const;
    bool setOption(SocketOption option, int value);

    bool waitForRead(int msecs = 30000, bool *timedOut = 0);
    bool waitForWrite(int msecs = 30000, bool *timedOut = 0);
    bool waitForReadOrWrite(bool *readyToRead, bool *readyToWrite,
                            bool checkRead, bool checkWrite,
                            int msecs = 30000, bool *timedOut = 0);

    QAbstractSocket::SocketError error() const;
    QString errorString() const;
    QAbstractSocket::SocketState state() const;
    QAbstractSocket::SocketType socketType() const;
    QAbstractSocket::NetworkLayerProtocol protocol() const;

    QHostAddress localAddress() const;
    quint16 localPort() const;
    QHostAddress peerAddress() const;
    quint16 peerPort() const;

    bool isReadNotificationEnabled() const;
    void setReadNotificationEnabled(bool enable);
    bool isWriteNotificationEnabled() const;
    void setWriteNotificationEnabled(bool enable);

public Q_SLOTS:
    void readNotification();
    void writeNotification();
    void connectionNotification();

public:
    void setReceiver(QAbstractSocketEngineReceiver *receiver);
protected:
    QAbstractSocketEngine(QAbstractSocketEnginePrivate &dd, QObject *parent = nullptr);

    void setError(QAbstractSocket::SocketError error, const QString &errorString);
    void setState(QAbstractSocket::SocketState state);
    void setSocketType(QAbstractSocket::SocketType socketType);
    void setProtocol(QAbstractSocket::NetworkLayerProtocol protocol);
    void setLocalAddress(const QHostAddress &address);
    void setLocalPort(quint16 port);
    void setPeerAddress(const QHostAddress &address);
    void setPeerPort(quint16 port);

private:
    Q_DECLARE_PRIVATE(QAbstractSocketEngine)
    Q_DISABLE_COPY(QAbstractSocketEngine)
};

class QAbstractSocketEnginePrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractSocketEngine)
public:
    QAbstractSocketEnginePrivate();
    ~QAbstractSocketEnginePrivate();

    enum ErrorString {
        NonBlockingInitFailedErrorString,
        BroadcastingInitFailedErrorString,
        NoIpV6ErrorString,
        RemoteHostClosedErrorString,
        TimeOutErrorString,
        ResourceErrorString,
        OperationUnsupportedErrorString,
        ProtocolUnsupportedErrorString,
        InvalidSocketErrorString,
        HostUnreachableErrorString,
        NetworkUnreachableErrorString,
        AccessErrorString,
        ConnectionTimeOutErrorString,
        ConnectionRefusedErrorString,
        AddressInuseErrorString,
        AddressNotAvailableErrorString,
        AddressProtectedErrorString,
        DatagramTooLargeErrorString,
        SendDatagramErrorString,
        ReceiveDatagramErrorString,
        WriteErrorString,
        ReadErrorString,
        PortInuseErrorString,
        NotSocketErrorString,

        UnknownSocketErrorString = -1
    };

    void setError(QAbstractSocket::SocketError error, ErrorString errorString);

    // native functions
    int option(QAbstractSocketEngine::SocketOption option) const;
    bool setOption(QAbstractSocketEngine::SocketOption option, int value);

    bool createNewSocket(QAbstractSocket::SocketType type, QAbstractSocket::NetworkLayerProtocol protocol);

    bool nativeConnect(const QHostAddress &address, quint16 port);
    bool nativeBind(const QHostAddress &address, quint16 port);
    bool nativeListen();
    int nativeAccept();
#if !defined(QT_NO_UDPSOCKET) && !defined(QT_NO_NETWORKINTERFACE)
    bool nativeJoinMulticastGroup(const QHostAddress &groupAddress,
                                  const QNetworkInterface &iface);
    bool nativeLeaveMulticastGroup(const QHostAddress &groupAddress,
                                   const QNetworkInterface &iface);
    QNetworkInterface nativeMulticastInterface() const;
    bool nativeSetMulticastInterface(const QNetworkInterface &iface);
#endif // QT_NO_UDPSOCKET && QT_NO_NETWORKINTERFACE
    qint64 nativeBytesAvailable() const;

    bool nativeHasPendingDatagrams() const;
    qint64 nativePendingDatagramSize() const;
    qint64 nativeReceiveDatagram(char *data, qint64 maxLength,
                                     QHostAddress *address, quint16 *port);
    qint64 nativeSendDatagram(const char *data, qint64 length,
                                  const QHostAddress &host, quint16 port);
    qint64 nativeRead(char *data, qint64 maxLength);
    qint64 nativeWrite(const char *data, qint64 length);
    int nativeSelect(int timeout, bool selectForRead) const;
    int nativeSelect(int timeout, bool checkRead, bool checkWrite,
                     bool *selectForRead, bool *selectForWrite) const;

    void nativeClose();

    bool fetchConnectionParameters();

    int socketDescriptor;
    QSocketNotifier *readNotifier, *writeNotifier;
    QAbstractSocket::SocketError socketError;
    bool hasSetSocketError;
    QString socketErrorString;
    QAbstractSocket::SocketState socketState;
    QAbstractSocket::SocketType socketType;
    QAbstractSocket::NetworkLayerProtocol socketProtocol;
    QHostAddress localAddress;
    quint16 localPort;
    QHostAddress peerAddress;
    quint16 peerPort;
    QAbstractSocketEngineReceiver *receiver;
};

QT_END_NAMESPACE

#endif // QABSTRACTSOCKETENGINE_P_H
