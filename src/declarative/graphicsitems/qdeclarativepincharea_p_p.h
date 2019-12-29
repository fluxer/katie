/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEPINCHAREA_P_H
#define QDECLARATIVEPINCHAREA_P_H

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

#include "qdatetime.h"
#include "qbasictimer.h"
#include "qevent.h"
#include "qgraphicssceneevent.h"
#include "qdeclarativeitem_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativePinch;
class QDeclarativePinchAreaPrivate : public QDeclarativeItemPrivate
{
    Q_DECLARE_PUBLIC(QDeclarativePinchArea)
public:
    QDeclarativePinchAreaPrivate()
      : absorb(true), stealMouse(false), inPinch(false)
      , pinchRejected(false), pinchActivated(false), touchEventsActive(false)
      , pinch(0), pinchStartDist(0), pinchStartScale(1.0)
      , pinchLastScale(1.0), pinchStartRotation(0.0), pinchStartAngle(0.0)
      , pinchLastAngle(0.0), pinchRotation(0.0)
    {
    }

    ~QDeclarativePinchAreaPrivate();

    void init()
    {
        Q_Q(QDeclarativePinchArea);
        q->setAcceptedMouseButtons(Qt::LeftButton);
        q->setAcceptTouchEvents(true);
        q->setFiltersChildEvents(true);
    }

    bool absorb : 1;
    bool stealMouse : 1;
    bool inPinch : 1;
    bool pinchRejected : 1;
    bool pinchActivated : 1;
    bool touchEventsActive : 1;
    QDeclarativePinch *pinch;
    QPointF sceneStartPoint1;
    QPointF sceneStartPoint2;
    QPointF lastPoint1;
    QPointF lastPoint2;
    qreal pinchStartDist;
    qreal pinchStartScale;
    qreal pinchLastScale;
    qreal pinchStartRotation;
    qreal pinchStartAngle;
    qreal pinchLastAngle;
    qreal pinchRotation;
    QPointF sceneStartCenter;
    QPointF pinchStartCenter;
    QPointF sceneLastCenter;
    QPointF pinchStartPos;
    QList<QTouchEvent::TouchPoint> touchPoints;
    int id1;
};

QT_END_NAMESPACE

#endif // QDECLARATIVEPINCHAREA_P_H
