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

#ifndef QNETWORKDISKCACHE_P_H
#define QNETWORKDISKCACHE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qabstractnetworkcache_p.h"

#include "qfile.h"

#ifndef QT_NO_NETWORKDISKCACHE

QT_BEGIN_NAMESPACE

class QCacheItem
{
public:
    QNetworkCacheMetaData metaData;

    void write(QFile *device) const;
    bool read(QFile *device);
};

class QNetworkDiskCachePrivate : public QAbstractNetworkCachePrivate
{
public:
    QNetworkDiskCachePrivate()
        : QAbstractNetworkCachePrivate()
        , maximumCacheSize(1024 * 1024 * 50)
        , currentCacheSize(0)
        {}

    QString cacheFileName(const QUrl &url) const;
    qint64 cacheSize() const;

    QString cacheDirectory;
    qint64 maximumCacheSize;
    qint64 currentCacheSize;

    Q_DECLARE_PUBLIC(QNetworkDiskCache)
};

QT_END_NAMESPACE

#endif // QT_NO_NETWORKDISKCACHE

#endif // QNETWORKDISKCACHE_P_H
