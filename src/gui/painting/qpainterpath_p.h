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

#ifndef QPAINTERPATH_P_H
#define QPAINTERPATH_P_H

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

#include "QtGui/qpainterpath.h"
#include "QtCore/qlist.h"
#include "QtCore/qvarlengtharray.h"
#include "qstroker_p.h"

QT_BEGIN_NAMESPACE

class QPainterPathStrokerPrivate
{
public:
    QPainterPathStrokerPrivate();

    QStroker stroker;
    QVector<qfixed> dashPattern;
    qreal dashOffset;
};

class QPainterPathPrivate
{
public:
    QPainterPathPrivate() :
        ref(1),
        cStart(0),
        fillRule(Qt::OddEvenFill),
        require_moveTo(false),
        dirtyBounds(false),
        dirtyControlBounds(false),
        convex(false)
    {
    }

    QPainterPathPrivate(const QPainterPathPrivate &other) :
        ref(1), elements(other.elements),
        cStart(other.cStart), fillRule(other.fillRule),
        bounds(other.bounds),
        controlBounds(other.controlBounds),
        require_moveTo(false),
        dirtyBounds(other.dirtyBounds),
        dirtyControlBounds(other.dirtyControlBounds),
        convex(other.convex)
    {
    }

    inline bool isClosed() const;
    inline void close();
    inline void maybeMoveTo();

    QAtomicInt ref;
    QVector<QPainterPath::Element> elements;

    int cStart;
    Qt::FillRule fillRule;

    QRectF bounds;
    QRectF controlBounds;

    bool require_moveTo;
    bool dirtyBounds;
    bool dirtyControlBounds;
    bool convex;
};

void Q_GUI_EXPORT qt_find_ellipse_coords(const QRectF &r, qreal angle, qreal length,
                                         QPointF* startPoint, QPointF *endPoint);

inline bool QPainterPathPrivate::isClosed() const
{
    const QPainterPath::Element &first = elements.at(cStart);
    const QPainterPath::Element &last = elements.last();
    return first.x == last.x && first.y == last.y;
}

inline void QPainterPathPrivate::close()
{
    Q_ASSERT(ref == 1);
    require_moveTo = true;
    const QPainterPath::Element &first = elements.at(cStart);
    QPainterPath::Element &last = elements.last();
    if (first.x != last.x || first.y != last.y) {
        if (qFuzzyCompare(first.x, last.x) && qFuzzyCompare(first.y, last.y)) {
            last.x = first.x;
            last.y = first.y;
        } else {
            QPainterPath::Element e = { first.x, first.y, QPainterPath::LineToElement };
            elements << e;
        }
    }
}

inline void QPainterPathPrivate::maybeMoveTo()
{
    if (require_moveTo) {
        QPainterPath::Element e = elements.last();
        e.type = QPainterPath::MoveToElement;
        elements.append(e);
        require_moveTo = false;
    }
}

// This value is used to determine the length of control point vectors
// when approximating arc segments as curves. The factor is multiplied
// with the radius of the circle.
#define KAPPA qreal(0.5522847498)

QT_END_NAMESPACE

#endif // QPAINTERPATH_P_H
