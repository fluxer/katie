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

#ifndef QNETWORKACCESSFTPBACKEND_P_H
#define QNETWORKACCESSFTPBACKEND_P_H

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

#include "qnetworkaccessbackend_p.h"
#include "qnetworkaccesscache_p.h"
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "QtNetwork/qftp.h"

#include "QtCore/qpointer.h"

#ifndef QT_NO_FTP

QT_BEGIN_NAMESPACE

class QNetworkAccessFtpIODevice;
class QNetworkAccessCachedFtpConnection;

class QNetworkAccessFtpBackend: public QNetworkAccessBackend
{
    Q_OBJECT
public:
    enum State {
        Idle,
        //Connecting,
        LoggingIn,
        CheckingFeatures,
        Statting,
        Transferring,
        Disconnecting
    };

    QNetworkAccessFtpBackend();
    virtual ~QNetworkAccessFtpBackend();

    virtual void open();
    virtual void closeDownstreamChannel();

    virtual void downstreamReadyWrite();

    enum CacheCleanupMode {
        ReleaseCachedConnection,
        RemoveCachedConnection
    };

    void disconnectFromFtp(CacheCleanupMode mode = ReleaseCachedConnection);

public slots:
    void ftpConnectionReady(QNetworkAccessCache::CacheableObject *object);
    void ftpDone();
    void ftpReadyRead();
    void ftpRawCommandReply(int code, const QString &text);

private:
    friend class QNetworkAccessFtpIODevice;
    QPointer<QNetworkAccessCachedFtpConnection> ftp;
    QIODevice *uploadDevice;
    qint64 totalBytes;
    int helpId, sizeId, mdtmId;
    bool supportsSize, supportsMdtm;
    State state;
};

class QNetworkAccessFtpBackendFactory: public QNetworkAccessBackendFactory
{
public:
    virtual QNetworkAccessBackend *create(QNetworkAccessManager::Operation op,
                                          const QNetworkRequest &request) const;
};

QT_END_NAMESPACE

#endif // QT_NO_FTP

#endif
