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

#ifndef QNETWORKACCESSDEBUGPIPEBACKEND_P_H
#define QNETWORKACCESSDEBUGPIPEBACKEND_P_H

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
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "qtcpsocket.h"

QT_BEGIN_NAMESPACE

#ifdef QT_BUILD_INTERNAL

class QNetworkAccessDebugPipeBackend: public QNetworkAccessBackend
{
    Q_OBJECT
public:
    QNetworkAccessDebugPipeBackend();
    virtual ~QNetworkAccessDebugPipeBackend();

    virtual void open();
    virtual void closeDownstreamChannel();

    virtual void downstreamReadyWrite();

protected:
    void pushFromSocketToDownstream();
    void pushFromUpstreamToSocket();
    void possiblyFinish();
    QNonContiguousByteDevice *uploadByteDevice;

private slots:
    void uploadReadyReadSlot();
    void socketReadyRead();
    void socketBytesWritten(qint64 bytes);
    void socketError();
    void socketDisconnected();
    void socketConnected();

private:
    QTcpSocket socket;
    bool bareProtocol;
    bool hasUploadFinished;
    bool hasDownloadFinished;
    bool hasEverythingFinished;

    qint64 bytesDownloaded;
    qint64 bytesUploaded;
};

class QNetworkAccessDebugPipeBackendFactory: public QNetworkAccessBackendFactory
{
public:
    virtual QNetworkAccessBackend *create(QNetworkAccessManager::Operation op,
                                          const QNetworkRequest &request) const;
};

#endif  // QT_BUILD_INTERNAL

QT_END_NAMESPACE

#endif
