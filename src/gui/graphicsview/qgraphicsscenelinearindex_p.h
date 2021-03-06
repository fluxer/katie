/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QGRAPHICSSCENELINEARINDEX_H
#define QGRAPHICSSCENELINEARINDEX_H

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

#include <QtCore/qglobal.h>

#ifndef QT_NO_GRAPHICSVIEW

#include <QtCore/qrect.h>
#include <QtCore/qlist.h>
#include <QtGui/qgraphicsitem.h>
#include "qgraphicssceneindex_p.h"


QT_BEGIN_NAMESPACE


class Q_AUTOTEST_EXPORT QGraphicsSceneLinearIndex : public QGraphicsSceneIndex
{
    Q_OBJECT

public:
    QGraphicsSceneLinearIndex(QGraphicsScene *scene = 0) : QGraphicsSceneIndex(scene)
    { }

    QList<QGraphicsItem *> items(Qt::SortOrder order = Qt::DescendingOrder) const
    { Q_UNUSED(order); return m_items; }

    virtual QList<QGraphicsItem *> estimateItems(const QRectF &rect, Qt::SortOrder order) const
    {
        Q_UNUSED(rect);
        Q_UNUSED(order);
        return m_items;
    }

protected :
    virtual void clear()
    { m_items.clear(); }

    virtual void addItem(QGraphicsItem *item)
    { m_items << item; }

    virtual void removeItem(QGraphicsItem *item)
    { m_items.removeOne(item); }

private:
    QList<QGraphicsItem*> m_items;
};

QT_END_NAMESPACE


#endif // QT_NO_GRAPHICSVIEW

#endif // QGRAPHICSSCENELINEARINDEX_H
