/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEIMAGEBASE_P_H
#define QDECLARATIVEIMAGEBASE_P_H

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

#include "qdeclarativeimplicitsizeitem_p_p.h"
#include "qdeclarativepixmapcache_p.h"

#include <QtCore/QPointer>

QT_BEGIN_NAMESPACE

class QNetworkReply;
class QDeclarativeImageBasePrivate : public QDeclarativeImplicitSizeItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeImageBase)

public:
    QDeclarativeImageBasePrivate()
      : status(QDeclarativeImageBase::Null),
        progress(0.0),
        explicitSourceSize(false),
        async(false),
        cache(true),
        mirror(false)
    {
        QGraphicsItemPrivate::flags = QGraphicsItemPrivate::flags & ~QGraphicsItem::ItemHasNoContents;
    }

    QDeclarativePixmap pix;
    QDeclarativeImageBase::Status status;
    QUrl url;
    qreal progress;
    QSize sourcesize;
    bool explicitSourceSize : 1;
    bool async : 1;
    bool cache : 1;
    bool mirror: 1;
};

QT_END_NAMESPACE

#endif
