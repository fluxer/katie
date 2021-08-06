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

#ifndef QUDPSOCKET_H
#define QUDPSOCKET_H

#include <QtNetwork/qabstractsocket.h>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qnetworkinterface.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_UDPSOCKET

class QNetworkInterface;
class QUdpSocketPrivate;

class Q_NETWORK_EXPORT QUdpSocket : public QAbstractSocket
{
    Q_OBJECT
public:
    enum BindFlag {
        DefaultForPlatform = 0x0,
        ShareAddress = 0x1,
        DontShareAddress = 0x2,
        ReuseAddressHint = 0x4
    };
    Q_DECLARE_FLAGS(BindMode, BindFlag)

    explicit QUdpSocket(QObject *parent = nullptr);
    virtual ~QUdpSocket();

    bool bind(const QHostAddress &address, quint16 port, BindMode mode = DefaultForPlatform);
    bool bind(quint16 port = 0, BindMode mode = DefaultForPlatform);

#ifndef QT_NO_NETWORKINTERFACE
    bool joinMulticastGroup(const QHostAddress &groupAddress,
                            const QNetworkInterface &iface = QNetworkInterface());
    bool leaveMulticastGroup(const QHostAddress &groupAddress,
                             const QNetworkInterface &iface = QNetworkInterface());

    QNetworkInterface multicastInterface() const;
    void setMulticastInterface(const QNetworkInterface &iface);
#endif

    bool hasPendingDatagrams() const;
    qint64 pendingDatagramSize() const;
    qint64 readDatagram(char *data, qint64 maxlen, QHostAddress *host = 0, quint16 *port = 0);
    qint64 writeDatagram(const char *data, qint64 len, const QHostAddress &host, quint16 port);
    inline qint64 writeDatagram(const QByteArray &datagram, const QHostAddress &host, quint16 port)
        { return writeDatagram(datagram.constData(), datagram.size(), host, port); }

private:
    Q_DISABLE_COPY(QUdpSocket)
    Q_DECLARE_PRIVATE(QUdpSocket)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QUdpSocket::BindMode)

#endif // QT_NO_UDPSOCKET

QT_END_NAMESPACE


#endif // QUDPSOCKET_H
