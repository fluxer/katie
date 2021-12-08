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

#ifndef QNETWORKREPLYDATAIMPL_H
#define QNETWORKREPLYDATAIMPL_H

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

#include "qnetworkreply.h"
#include "qnetworkreply_p.h"
#include "qnetworkaccessmanager.h"
#include <QBuffer>

QT_BEGIN_NAMESPACE


class QNetworkReplyDataImplPrivate;
class QNetworkReplyDataImpl: public QNetworkReply
{
    Q_OBJECT
public:
    QNetworkReplyDataImpl(QObject *parent, const QNetworkRequest &req, const QNetworkAccessManager::Operation op);

    // reimplemented from QIODevice
    qint64 size() const final;
    qint64 bytesAvailable() const final;

    // reimplemented from QNetworkReply
    void abort() final;
    bool isSequential() const final;
    qint64 readData(char *data, qint64 maxlen) final;

    Q_DECLARE_PRIVATE(QNetworkReplyDataImpl)
};

class QNetworkReplyDataImplPrivate: public QNetworkReplyPrivate
{
public:
    QBuffer decodedData;

    Q_DECLARE_PUBLIC(QNetworkReplyDataImpl)
};

QT_END_NAMESPACE

#endif // QNETWORKREPLYDATAIMPL_H
