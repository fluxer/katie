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
#include <QtCore/qscopedpointer.h>
#include <QtNetwork/qhostaddress.h>


QT_BEGIN_NAMESPACE


class QObject;
class QHostInfoPrivate;

class Q_NETWORK_EXPORT QHostInfo
{
public:
    enum HostInfoError {
        NoError,
        HostNotFound,
        UnknownError
    };

    QHostInfo(int lookupId = -1);
    QHostInfo(const QHostInfo &d);
    QHostInfo &operator=(const QHostInfo &d);
    ~QHostInfo();

    QString hostName() const;
    void setHostName(const QString &name);

    QList<QHostAddress> addresses() const;
    void setAddresses(const QList<QHostAddress> &addresses);

    HostInfoError error() const;
    void setError(HostInfoError error);

    QString errorString() const;
    void setErrorString(const QString &errorString);

    void setLookupId(int id);
    int lookupId() const;

    static int lookupHost(const QString &name, QObject *receiver, const char *member);
    static void abortHostLookup(int lookupId);

    static QHostInfo fromName(const QString &name);
    static QString localHostName();
    static QString localDomainName();

private:
    QScopedPointer<QHostInfoPrivate> d;
};

QT_END_NAMESPACE


#endif // QHOSTINFO_H
