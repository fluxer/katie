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

#include "qnetworkaccessbackend_p.h"
#include "qnetworkaccessmanager_p.h"
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "qnetworkreply_p.h"
#include "qhash.h"
#include "qmutex.h"
#include "qnetworkaccesscachebackend_p.h"
#include "qabstractnetworkcache.h"
#include "qhostinfo.h"
#include "qnoncontiguousbytedevice_p.h"

QT_BEGIN_NAMESPACE

class QNetworkAccessBackendFactoryData: public QList<QNetworkAccessBackendFactory *>
{
public:
    QNetworkAccessBackendFactoryData() : QList<QNetworkAccessBackendFactory *>()
    {
        valid.ref();
    }
    ~QNetworkAccessBackendFactoryData()
    {
        std::lock_guard<std::recursive_mutex> locker(mutex); // why do we need to lock?
        valid.deref();
    }

    std::recursive_mutex mutex;
    //this is used to avoid (re)constructing factory data from destructors of other global classes
    static QAtomicInt valid;
};
Q_GLOBAL_STATIC(QNetworkAccessBackendFactoryData, factoryData)
QAtomicInt QNetworkAccessBackendFactoryData::valid = QAtomicInt(0);

QNetworkAccessBackendFactory::QNetworkAccessBackendFactory()
{
    std::lock_guard<std::recursive_mutex> locker(factoryData()->mutex);
    factoryData()->append(this);
}

QNetworkAccessBackendFactory::~QNetworkAccessBackendFactory()
{
    if (QNetworkAccessBackendFactoryData::valid) {
        std::lock_guard<std::recursive_mutex> locker(factoryData()->mutex);
        factoryData()->removeAll(this);
    }
}

QNetworkAccessBackend *QNetworkAccessManagerPrivate::findBackend(QNetworkAccessManager::Operation op,
                                                                 const QNetworkRequest &request)
{
    if (QNetworkAccessBackendFactoryData::valid) {
        std::lock_guard<std::recursive_mutex> locker(factoryData()->mutex);
        QNetworkAccessBackendFactoryData::ConstIterator it = factoryData()->constBegin(),
                                                           end = factoryData()->constEnd();
        while (it != end) {
            QNetworkAccessBackend *backend = (*it)->create(op, request);
            if (backend) {
                backend->manager = this;
                return backend; // found a factory that handled our request
            }
            ++it;
        }
    }
    return 0;
}

QNonContiguousByteDevice* QNetworkAccessBackend::createUploadByteDevice()
{
    if (reply->outgoingDataBuffer)
        uploadByteDevice = QSharedPointer<QNonContiguousByteDevice>(QNonContiguousByteDeviceFactory::create(reply->outgoingDataBuffer));
    else if (reply->outgoingData) {
        uploadByteDevice = QSharedPointer<QNonContiguousByteDevice>(QNonContiguousByteDeviceFactory::create(reply->outgoingData));
    } else {
        return 0;
    }

    bool bufferDisallowed =
            reply->request.attribute(QNetworkRequest::DoNotBufferUploadDataAttribute,
                          QVariant(false)) == QVariant(true);
    if (bufferDisallowed)
        uploadByteDevice->disableReset();

    // We want signal emissions only for normal asynchronous uploads
    if (!isSynchronous())
        connect(uploadByteDevice.data(), SIGNAL(readProgress(qint64,qint64)), this, SLOT(emitReplyUploadProgress(qint64,qint64)));

    return uploadByteDevice.data();
}

// need to have this function since the reply is a private member variable
// and the special backends need to access this.
void QNetworkAccessBackend::emitReplyUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if (reply->isFinished)
        return;
    reply->emitUploadProgress(bytesSent, bytesTotal);
}

QNetworkAccessBackend::QNetworkAccessBackend()
    : manager(0)
    , reply(0)
    , synchronous(false)
{
}

QNetworkAccessBackend::~QNetworkAccessBackend()
{
}

void QNetworkAccessBackend::downstreamReadyWrite()
{
    // do nothing
}

void QNetworkAccessBackend::setDownstreamLimited(bool b)
{
    Q_UNUSED(b);
    // do nothing
}

void QNetworkAccessBackend::setReadBufferSize(qint64 size)
{
    Q_UNUSED(size);
    // do nothing
}

void QNetworkAccessBackend::emitReadBufferFreed(qint64 size)
{
    Q_UNUSED(size);
    // do nothing
}

void QNetworkAccessBackend::copyFinished(QIODevice *)
{
    // do nothing
}

void QNetworkAccessBackend::ignoreSslErrors()
{
    // do nothing
}

void QNetworkAccessBackend::ignoreSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    // do nothing
}

void QNetworkAccessBackend::fetchSslConfiguration(QSslConfiguration &) const
{
    // do nothing
}

void QNetworkAccessBackend::setSslConfiguration(const QSslConfiguration &)
{
    // do nothing
}

QNetworkCacheMetaData QNetworkAccessBackend::fetchCacheMetaData(const QNetworkCacheMetaData &) const
{
    return QNetworkCacheMetaData();
}

QNetworkAccessManager::Operation QNetworkAccessBackend::operation() const
{
    return reply->operation;
}

QNetworkRequest QNetworkAccessBackend::request() const
{
    return reply->request;
}

#ifndef QT_NO_NETWORKPROXY
QList<QNetworkProxy> QNetworkAccessBackend::proxyList() const
{
    return reply->proxyList;
}
#endif

QAbstractNetworkCache *QNetworkAccessBackend::networkCache() const
{
    if (!manager)
        return 0;
    return manager->networkCache;
}

void QNetworkAccessBackend::setCachingEnabled(bool enable)
{
    reply->setCachingEnabled(enable);
}

bool QNetworkAccessBackend::isCachingEnabled() const
{
    return reply->isCachingEnabled();
}

qint64 QNetworkAccessBackend::nextDownstreamBlockSize() const
{
    return reply->nextDownstreamBlockSize();
}

void QNetworkAccessBackend::writeDownstreamData(QByteDataBuffer &list)
{
    reply->appendDownstreamData(list);
}

void QNetworkAccessBackend::writeDownstreamData(QIODevice *data)
{
    reply->appendDownstreamData(data);
}

// not actually appending data, it was already written to the user buffer
void QNetworkAccessBackend::writeDownstreamDataDownloadBuffer(qint64 bytesReceived, qint64 bytesTotal)
{
    reply->appendDownstreamDataDownloadBuffer(bytesReceived, bytesTotal);
}

char* QNetworkAccessBackend::getDownloadBuffer(qint64 size)
{
    return reply->getDownloadBuffer(size);
}

QVariant QNetworkAccessBackend::header(QNetworkRequest::KnownHeaders header) const
{
    return reply->q_func()->header(header);
}

void QNetworkAccessBackend::setHeader(QNetworkRequest::KnownHeaders header, const QVariant &value)
{
    reply->setCookedHeader(header, value);
}

bool QNetworkAccessBackend::hasRawHeader(const QByteArray &headerName) const
{
    return reply->q_func()->hasRawHeader(headerName);
}

QByteArray QNetworkAccessBackend::rawHeader(const QByteArray &headerName) const
{
    return reply->q_func()->rawHeader(headerName);
}

QList<QByteArray> QNetworkAccessBackend::rawHeaderList() const
{
    return reply->q_func()->rawHeaderList();
}

void QNetworkAccessBackend::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    reply->setRawHeader(headerName, headerValue);
}

QVariant QNetworkAccessBackend::attribute(QNetworkRequest::Attribute code) const
{
    return reply->q_func()->attribute(code);
}

void QNetworkAccessBackend::setAttribute(QNetworkRequest::Attribute code, const QVariant &value)
{
    if (value.isValid())
        reply->attributes.insert(code, value);
    else
        reply->attributes.remove(code);
}
QUrl QNetworkAccessBackend::url() const
{
    return reply->url;
}

void QNetworkAccessBackend::setUrl(const QUrl &url)
{
    reply->url = url;
}

void QNetworkAccessBackend::finished()
{
    reply->finished();
}

void QNetworkAccessBackend::error(QNetworkReply::NetworkError code, const QString &errorString)
{
    reply->error(code, errorString);
}

#ifndef QT_NO_NETWORKPROXY
void QNetworkAccessBackend::proxyAuthenticationRequired(const QNetworkProxy &proxy,
                                                        QAuthenticator *authenticator)
{
    manager->proxyAuthenticationRequired(this, proxy, authenticator);
}
#endif

void QNetworkAccessBackend::authenticationRequired(QAuthenticator *authenticator)
{
    manager->authenticationRequired(this, authenticator);
}

void QNetworkAccessBackend::metaDataChanged()
{
    reply->metaDataChanged();
}

void QNetworkAccessBackend::redirectionRequested(const QUrl &target)
{
    reply->redirectionRequested(target);
}

void QNetworkAccessBackend::sslErrors(const QList<QSslError> &errors)
{
    reply->sslErrors(errors);
}

/*!
    Starts the backend.  Returns true if the backend is started.  Returns false if the backend
    could not be started due to an unopened or roaming session.  The caller should recall this
    function once the session has been opened or the roaming process has finished.
*/
bool QNetworkAccessBackend::start()
{
#ifndef QT_NO_NETWORKPROXY
    // the proxy depends only on the url
    reply->proxyList = manager->queryProxy(QNetworkProxyQuery(url()));
#endif

    // now start the request
    open();
    return true;
}

QT_END_NAMESPACE

#include "moc_qnetworkaccessbackend_p.h"


