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

#ifndef QTCPSOCKET_H
#define QTCPSOCKET_H

#include <QtNetwork/qabstractsocket.h>
#include <QtCore/qvariant.h>


QT_BEGIN_NAMESPACE


class QTcpSocketPrivate;

class Q_NETWORK_EXPORT QTcpSocket : public QAbstractSocket
{
    Q_OBJECT
public:
    explicit QTcpSocket(QObject *parent = nullptr);
    virtual ~QTcpSocket();

protected:
    QTcpSocket(QTcpSocketPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(QTcpSocket)
    Q_DECLARE_PRIVATE(QTcpSocket)
};

QT_END_NAMESPACE


#endif // QTCPSOCKET_H
