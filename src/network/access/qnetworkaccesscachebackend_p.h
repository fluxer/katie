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

#ifndef QNETWORKACCESSCACHEBACKEND_P_H
#define QNETWORKACCESSCACHEBACKEND_P_H

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

QT_BEGIN_NAMESPACE

class QNetworkAccessCacheBackend : public QNetworkAccessBackend
{

public:
    QNetworkAccessCacheBackend();
    ~QNetworkAccessCacheBackend();

    void open();
    void closeDownstreamChannel();
    void closeUpstreamChannel();

    void upstreamReadyRead();
    void downstreamReadyWrite();

private:
    bool sendCacheContents();
    QIODevice *device;

};

QT_END_NAMESPACE

#endif // QNETWORKACCESSCACHEBACKEND_P_H
