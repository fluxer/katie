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

#ifndef QGRAPHICSLAYOUTITEM_P_H
#define QGRAPHICSLAYOUTITEM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QSize>
#include <QtGui/QSizePolicy>

QT_BEGIN_NAMESPACE

class QGraphicsLayoutItem;
class Q_AUTOTEST_EXPORT QGraphicsLayoutItemPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsLayoutItem)
public:
    virtual ~QGraphicsLayoutItemPrivate();
    QGraphicsLayoutItemPrivate(QGraphicsLayoutItem *parent, bool isLayout);
    static QGraphicsLayoutItemPrivate *get(QGraphicsLayoutItem *q) { return q->d_func();}
    static const QGraphicsLayoutItemPrivate *get(const QGraphicsLayoutItem *q) { return q->d_func();}

    void init();
    QSizeF *effectiveSizeHints(const QSizeF &constraint) const;
    QGraphicsItem *parentItem() const;
    void ensureUserSizeHints();
    void setSize(Qt::SizeHint which, const QSizeF &size);
    enum SizeComponent { Width, Height };
    void setSizeComponent(Qt::SizeHint which, SizeComponent component, qreal value);

    bool hasHeightForWidth() const;
    bool hasWidthForHeight() const;

    QSizePolicy sizePolicy;
    QGraphicsLayoutItem *parent;

    QSizeF *userSizeHints;
    mutable QSizeF cachedSizeHints[Qt::NSizeHints];
    mutable QSizeF cachedConstraint;
    mutable QSizeF cachedSizeHintsWithConstraints[Qt::NSizeHints];

    mutable bool sizeHintCacheDirty;
    mutable bool sizeHintWithConstraintCacheDirty;
    bool isLayout;
    bool ownedByLayout;

    QGraphicsLayoutItem *q_ptr;
    QRectF geom;
    QGraphicsItem *graphicsItem;
};

QT_END_NAMESPACE

#endif //QGRAPHICSLAYOUTITEM_P_H

