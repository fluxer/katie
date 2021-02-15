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

#ifndef QDECLARATIVERECT_P_H
#define QDECLARATIVERECT_P_H

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

#include "qdeclarativeitem_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeGradient;
class QDeclarativeRectangle;
class QDeclarativeRectanglePrivate : public QDeclarativeItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativeRectangle)

public:
    QDeclarativeRectanglePrivate() :
    color(Qt::white), gradient(0), pen(0), radius(0), paintmargin(0)
    {
        QGraphicsItemPrivate::flags = QGraphicsItemPrivate::flags & ~QGraphicsItem::ItemHasNoContents;
    }

    ~QDeclarativeRectanglePrivate()
    {
        delete pen;
    }

    QColor color;
    QDeclarativeGradient *gradient;
    QDeclarativePen *pen;
    qreal radius;
    qreal paintmargin;
    QPixmap rectImage;
    static int doUpdateSlotIdx;

    QDeclarativePen *getPen() {
        if (!pen) {
            Q_Q(QDeclarativeRectangle);
            pen = new QDeclarativePen;
            static int penChangedSignalIdx = -1;
            if (penChangedSignalIdx < 0)
                penChangedSignalIdx = QDeclarativePen::staticMetaObject.indexOfSignal("penChanged()");
            if (doUpdateSlotIdx < 0)
                doUpdateSlotIdx = QDeclarativeRectangle::staticMetaObject.indexOfSlot("doUpdate()");
            QMetaObject::connect(pen, penChangedSignalIdx, q, doUpdateSlotIdx);
        }
        return pen;
    }

    void setPaintMargin(qreal margin)
    {
        Q_Q(QDeclarativeRectangle);
        if (margin == paintmargin)
            return;
        q->prepareGeometryChange();
        paintmargin = margin;
    }
};

QT_END_NAMESPACE

#endif // QDECLARATIVERECT_P_H
