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

#ifndef QLOCALSERVER_H
#define QLOCALSERVER_H

#include <QtNetwork/qabstractsocket.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_LOCALSERVER

class QLocalSocket;
class QLocalServerPrivate;

class Q_NETWORK_EXPORT QLocalServer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QLocalServer)

Q_SIGNALS:
    void newConnection();

public:
    QLocalServer(QObject *parent = nullptr);
    ~QLocalServer();

    void close();
    QString errorString() const;
    virtual bool hasPendingConnections() const;
    bool isListening() const;
    bool listen(const QString &name);
    int maxPendingConnections() const;
    virtual QLocalSocket *nextPendingConnection();
    QString serverName() const;
    QString fullServerName() const;
    static bool removeServer(const QString &name);
    QAbstractSocket::SocketError serverError() const;
    void setMaxPendingConnections(int numConnections);
    bool waitForNewConnection(int msec = 0, bool *timedOut = 0);

protected:
    virtual void incomingConnection(int socketDescriptor);

private:
    Q_DISABLE_COPY(QLocalServer)
    Q_PRIVATE_SLOT(d_func(), void _q_onNewConnection())
};

#endif // QT_NO_LOCALSERVER

QT_END_NAMESPACE


#endif // QLOCALSERVER_H

