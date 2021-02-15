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

#ifndef QHTTPNETWORKHEADER_H
#define QHTTPNETWORKHEADER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//
#ifndef QT_NO_HTTP

#include "qshareddata.h"
#include "qurl.h"

QT_BEGIN_NAMESPACE

class Q_AUTOTEST_EXPORT QHttpNetworkHeader
{
public:
    virtual ~QHttpNetworkHeader() {};
    virtual QUrl url() const = 0;
    virtual void setUrl(const QUrl &url) = 0;

    virtual int majorVersion() const = 0;
    virtual int minorVersion() const = 0;

    virtual qint64 contentLength() const = 0;
    virtual void setContentLength(qint64 length) = 0;

    virtual QList<QPair<QByteArray, QByteArray> > header() const = 0;
    virtual QByteArray headerField(const QByteArray &name, const QByteArray &defaultValue = QByteArray()) const = 0;
    virtual void setHeaderField(const QByteArray &name, const QByteArray &data) = 0;
};

class QHttpNetworkHeaderPrivate : public QSharedData
{
public:
    QUrl url;
    QList<QPair<QByteArray, QByteArray> > fields;

    QHttpNetworkHeaderPrivate(const QUrl &newUrl = QUrl());
    QHttpNetworkHeaderPrivate(const QHttpNetworkHeaderPrivate &other);
    qint64 contentLength() const;
    void setContentLength(qint64 length);

    QByteArray headerField(const QByteArray &name, const QByteArray &defaultValue = QByteArray()) const;
    QList<QByteArray> headerFieldValues(const QByteArray &name) const;
    void setHeaderField(const QByteArray &name, const QByteArray &data);
    bool operator==(const QHttpNetworkHeaderPrivate &other) const;

};


QT_END_NAMESPACE


#endif // QT_NO_HTTP


#endif // QHTTPNETWORKHEADER_H






