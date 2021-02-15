/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#ifndef QSTYLE_P_H
#define QSTYLE_P_H

#include "qobject_p.h"
#include <QtGui/qstyle.h>

QT_BEGIN_NAMESPACE

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of qstyle_*.cpp.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

// Private class

class QStyle;

class QStylePrivate: public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QStyle)
public:
    inline QStylePrivate()
        : proxyStyle(0) {}
    QStyle *proxyStyle;
};


#define BEGIN_STYLE_PIXMAPCACHE(a) \
    QRect rect = option->rect; \
    QPixmap internalPixmapCache; \
    QImage imageCache; \
    QPainter *p = painter; \
    QString unique = QStyleHelper::uniqueName((a), option, option->rect.size()); \
    QTransform::TransformationType devTxType = painter->deviceTransform().type(); \
    QTransform::TransformationType worldTxType = painter->worldTransform().type(); \
    bool doPixmapCache = (devTxType <= QTransform::TxTranslate && worldTxType <= QTransform::TxTranslate); \
    if (doPixmapCache && QPixmapCache::find(unique, internalPixmapCache)) { \
        painter->drawPixmap(option->rect.topLeft(), internalPixmapCache); \
    } else { \
        if (doPixmapCache) { \
            rect.setRect(0, 0, option->rect.width(), option->rect.height()); \
            imageCache = QImage(option->rect.size(), QImage::Format_ARGB32_Premultiplied); \
            imageCache.fill(0); \
            p = new QPainter(&imageCache); \
        }



#define END_STYLE_PIXMAPCACHE \
        if (doPixmapCache) { \
            p->end(); \
            delete p; \
            internalPixmapCache = QPixmap::fromImage(imageCache); \
            painter->drawPixmap(option->rect.topLeft(), internalPixmapCache); \
            QPixmapCache::insert(unique, internalPixmapCache); \
        } \
    }

QT_END_NAMESPACE

#endif //QSTYLE_P_H
