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

#include "qnetworkreplydataimpl_p.h"
#include "qdataurl_p.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QMetaObject>

QT_BEGIN_NAMESPACE

QNetworkReplyDataImpl::QNetworkReplyDataImpl(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op)
    : QNetworkReply(*new QNetworkReplyDataImplPrivate(), parent)
{
    Q_D(QNetworkReplyDataImpl);
    setRequest(req);
    setUrl(req.url());
    setOperation(op);
    setFinished(true);
    QNetworkReply::open(QIODevice::ReadOnly);

    QUrl url = req.url();

    // FIXME qDecodeDataUrl should instead be rewritten to have the QByteArray
    // and the mime type as an output parameter and return a bool instead
    QPair<QString, QByteArray> decodeDataUrlResult = qDecodeDataUrl(url);

    if (! decodeDataUrlResult.first.isNull()) {
        QString &mimeType = decodeDataUrlResult.first;
        qint64 size = decodeDataUrlResult.second.size();
        setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
        setHeader(QNetworkRequest::ContentLengthHeader, size);
        QMetaObject::invokeMethod(this, "metaDataChanged", Qt::QueuedConnection);

        d->decodedData.setBuffer(&decodeDataUrlResult.second);
        d->decodedData.open(QIODevice::ReadOnly);

        QMetaObject::invokeMethod(this, "downloadProgress", Qt::QueuedConnection,
                                  Q_ARG(qint64,size), Q_ARG(qint64, size));
        QMetaObject::invokeMethod(this, "readyRead", Qt::QueuedConnection);
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
    } else {
        // something wrong with this URI
        const QString msg = QCoreApplication::translate("QNetworkAccessDataBackend",
                                                        "Invalid URI: %1").arg(QString::fromLatin1(url.toEncoded()));
        setError(QNetworkReply::ProtocolFailure, msg);
        QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,
                                  Q_ARG(QNetworkReply::NetworkError, QNetworkReply::ProtocolFailure));
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
    }
}

qint64 QNetworkReplyDataImpl::size() const
{
    Q_D(const QNetworkReplyDataImpl);
    return d->decodedData.size();
}

qint64 QNetworkReplyDataImpl::bytesAvailable() const
{
    Q_D(const QNetworkReplyDataImpl);
    return QNetworkReply::bytesAvailable() + d->decodedData.bytesAvailable();
}

void QNetworkReplyDataImpl::abort()
{
    QNetworkReply::close();
}

bool QNetworkReplyDataImpl::isSequential() const
{
    return true;
}

/*!
    \internal
*/
qint64 QNetworkReplyDataImpl::readData(char *data, qint64 maxlen)
{
    Q_D(QNetworkReplyDataImpl);

    // TODO idea:
    // Instead of decoding the whole data into new memory, we could decode on demand.
    // Note that this might be tricky to do.

    return d->decodedData.read(data, maxlen);
}

QT_END_NAMESPACE

#include "moc_qnetworkreplydataimpl_p.h"



