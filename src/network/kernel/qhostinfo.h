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

#ifndef QHOSTINFO_H
#define QHOSTINFO_H

#include <QtCore/qlist.h>
#include <QtNetwork/qhostaddress.h>


QT_BEGIN_NAMESPACE


class QHostInfoPrivate;

class Q_NETWORK_EXPORT QHostInfo
{
public:
    enum HostInfoError {
        NoError,
        HostNotFound,
        UnknownError
    };

    QHostInfo();
    QHostInfo(const QHostInfo &d);
    QHostInfo &operator=(const QHostInfo &d);
    ~QHostInfo();

    QString hostName() const;
    QList<QHostAddress> addresses() const;

    HostInfoError error() const;

    QString errorString() const;

    static QHostInfo fromName(const QString &name);
    static QString localHostName();
    static QString localDomainName();

private:
    friend class QHostInfoPrivate;
    friend class QAbstractSocket;
    QHostInfoPrivate* d;
};

QT_END_NAMESPACE


#endif // QHOSTINFO_H
