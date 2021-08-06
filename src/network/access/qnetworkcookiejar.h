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

#ifndef QNETWORKCOOKIEJAR_H
#define QNETWORKCOOKIEJAR_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

// ### Qt5 remove this include
#include <QtNetwork/QNetworkCookie>


QT_BEGIN_NAMESPACE


class QNetworkCookieJarPrivate;
class Q_NETWORK_EXPORT QNetworkCookieJar: public QObject
{
    Q_OBJECT
public:
    QNetworkCookieJar(QObject *parent = nullptr);
    virtual ~QNetworkCookieJar();

    virtual QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);

protected:
    QList<QNetworkCookie> allCookies() const;
    void setAllCookies(const QList<QNetworkCookie> &cookieList);

private:
    Q_DECLARE_PRIVATE(QNetworkCookieJar)
    Q_DISABLE_COPY(QNetworkCookieJar)
};

QT_END_NAMESPACE


#endif
