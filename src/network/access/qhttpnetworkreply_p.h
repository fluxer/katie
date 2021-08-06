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

#ifndef QHTTPNETWORKREPLY_H
#define QHTTPNETWORKREPLY_H

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
#include <qplatformdefs.h>
#ifndef QT_NO_HTTP

#include <zlib.h>
static const unsigned char gz_magic[2] = {0x1f, 0x8b}; // gzip magic header
// gzip flag byte
#define HEAD_CRC     0x02 // bit 1 set: header CRC present
#define EXTRA_FIELD  0x04 // bit 2 set: extra field present
#define ORIG_NAME    0x08 // bit 3 set: original file name present
#define COMMENT      0x10 // bit 4 set: file comment present
#define RESERVED     0xE0 // bits 5..7: reserved

#include <QtNetwork/qtcpsocket.h>
// it's safe to include these even if SSL support is not enabled
#include <QtNetwork/qsslsocket.h>
#include <QtNetwork/qsslerror.h>

#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include "qbuffer.h"

#include "qobject_p.h"
#include "qhttpnetworkheader_p.h"
#include "qhttpnetworkrequest_p.h"
#include "qauthenticator_p.h"
#include "qbytedata_p.h"

QT_BEGIN_NAMESPACE

class QHttpNetworkConnection;
class QHttpNetworkConnectionChannel;
class QHttpNetworkRequest;
class QHttpNetworkConnectionPrivate;
class QHttpNetworkReplyPrivate;
class Q_AUTOTEST_EXPORT QHttpNetworkReply : public QObject, public QHttpNetworkHeader
{
    Q_OBJECT
public:

    explicit QHttpNetworkReply(const QUrl &url = QUrl(), QObject *parent = nullptr);
    virtual ~QHttpNetworkReply();

    QUrl url() const;
    void setUrl(const QUrl &url);

    int majorVersion() const;
    int minorVersion() const;

    qint64 contentLength() const;
    void setContentLength(qint64 length);

    QList<QPair<QByteArray, QByteArray> > header() const;
    QByteArray headerField(const QByteArray &name, const QByteArray &defaultValue = QByteArray()) const;
    void setHeaderField(const QByteArray &name, const QByteArray &data);
    void parseHeader(const QByteArray &header); // mainly for testing

    QHttpNetworkRequest request() const;
    void setRequest(const QHttpNetworkRequest &request);

    int statusCode() const;
    void setStatusCode(int code);

    QString errorString() const;
    void setErrorString(const QString &error);

    QString reasonPhrase() const;

    qint64 bytesAvailable() const;
    qint64 bytesAvailableNextBlock() const;
    bool readAnyAvailable() const;
    QByteArray readAny();
    QByteArray readAll();
    QByteArray read(qint64 amount);
    qint64 sizeNextBlock();
    void setDownstreamLimited(bool t);
    void setReadBufferSize(qint64 size);

    bool supportsUserProvidedDownloadBuffer();
    void setUserProvidedDownloadBuffer(char*);
    char* userProvidedDownloadBuffer();

    bool isFinished() const;

    bool isPipeliningUsed() const;

    QHttpNetworkConnection* connection();

    QSslConfiguration sslConfiguration() const;
    void setSslConfiguration(const QSslConfiguration &config);
    void ignoreSslErrors();
    void ignoreSslErrors(const QList<QSslError> &errors);

Q_SIGNALS:
    void sslErrors(const QList<QSslError> &errors);

Q_SIGNALS:
    void readyRead();
    void finished();
    void finishedWithError(QNetworkReply::NetworkError errorCode, const QString &detail = QString());
    void headerChanged();
    // FIXME we need to change this to qint64!
    void dataReadProgress(int done, int total);
    void dataSendProgress(qint64 done, qint64 total);
    void cacheCredentials(const QHttpNetworkRequest &request, QAuthenticator *authenticator);
#ifndef QT_NO_NETWORKPROXY
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
#endif
    void authenticationRequired(const QHttpNetworkRequest &request, QAuthenticator *authenticator);
private:
    Q_DECLARE_PRIVATE(QHttpNetworkReply)
    friend class QHttpNetworkConnection;
    friend class QHttpNetworkConnectionPrivate;
    friend class QHttpNetworkConnectionChannel;
};


class QHttpNetworkReplyPrivate : public QObjectPrivate, public QHttpNetworkHeaderPrivate
{
public:
    QHttpNetworkReplyPrivate(const QUrl &newUrl = QUrl());
    ~QHttpNetworkReplyPrivate();
    qint64 readStatus(QAbstractSocket *socket);
    bool parseStatus(const QByteArray &status);
    qint64 readHeader(QAbstractSocket *socket);
    void parseHeader(const QByteArray &header);
    qint64 readBody(QAbstractSocket *socket, QByteDataBuffer *out);
    qint64 readBodyVeryFast(QAbstractSocket *socket, char *b);
    qint64 readBodyFast(QAbstractSocket *socket, QByteDataBuffer *rb);
    bool findChallenge(bool forProxy, QByteArray &challenge) const;
    QAuthenticatorPrivate::Method authenticationMethod(bool isProxy) const;
    void clear();
    void clearHttpLayerInformation();

    qint64 readReplyBodyRaw(QAbstractSocket *in, QByteDataBuffer *out, qint64 size);
    qint64 readReplyBodyChunked(QAbstractSocket *in, QByteDataBuffer *out);
    qint64 getChunkSize(QAbstractSocket *in, qint64 *chunkSize);

    void appendUncompressedReplyData(QByteArray &qba);
    void appendUncompressedReplyData(QByteDataBuffer &data);
    void appendCompressedReplyData(QByteDataBuffer &data);

    bool shouldEmitSignals();
    bool expectContent();
    void eraseData();

    qint64 bytesAvailable() const;
    bool isChunked();
    bool isConnectionCloseEnabled();
    bool isGzipped();
    bool gzipCheckHeader(QByteArray &content, int &pos);
    int gunzipBodyPartially(QByteArray &compressed, QByteArray &inflated);
    void gunzipBodyPartiallyEnd();
    void removeAutoDecompressHeader();

    enum ReplyState {
        NothingDoneState,
        ReadingStatusState,
        ReadingHeaderState,
        ReadingDataState,
        AllDoneState
    } state;

    QHttpNetworkRequest request;
    bool ssl;
    int statusCode;
    int majorVersion;
    int minorVersion;
    QString errorString;
    QString reasonPhrase;
    qint64 bodyLength;
    qint64 contentRead;
    qint64 totalProgress;
    QByteArray fragment; // used for header, status, chunk header etc, not for reply data
    bool chunkedTransferEncoding;
    bool connectionCloseEnabled;
    bool forceConnectionCloseEnabled;
    bool lastChunkRead;
    qint64 currentChunkSize;
    qint64 currentChunkRead;
    qint64 readBufferMaxSize;
    QPointer<QHttpNetworkConnection> connection;
    QPointer<QHttpNetworkConnectionChannel> connectionChannel;
    bool initInflate;
    bool streamEnd;
    z_stream inflateStrm;
    bool autoDecompress;

    QByteDataBuffer responseData; // uncompressed body
    QByteArray compressedData; // compressed body (temporary)
    bool requestIsPrepared;

    bool pipeliningUsed;
    bool downstreamLimited;

    char* userProvidedDownloadBuffer;
};




QT_END_NAMESPACE

//Q_DECLARE_METATYPE(QHttpNetworkReply)

#endif // QT_NO_HTTP


#endif // QHTTPNETWORKREPLY_H
