/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#ifndef QGRAPHICSVIEW_P_H
#define QGRAPHICSVIEW_P_H

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

#include "qgraphicsview.h"

#ifndef QT_NO_GRAPHICSVIEW

#include <QtGui/qevent.h>
#include <QtCore/qcoreapplication.h>
#include "qgraphicssceneevent.h"
#include <QtGui/qstyleoption.h>
#include "qabstractscrollarea_p.h"
#include "qapplication_p.h"

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QGraphicsViewPrivate : public QAbstractScrollAreaPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsView)
public:
    QGraphicsViewPrivate();

    void recalculateContentSize();
    void centerView(QGraphicsView::ViewportAnchor anchor);

    QPainter::RenderHints renderHints;

    QGraphicsView::DragMode dragMode;

    bool sceneInteractionAllowed;
    bool hasSceneRect;
    bool connectedToScene;
    bool useLastMouseEvent;
    bool identityMatrix;
    bool dirtyScroll;
    bool keepLastCenterPoint;
    bool transforming;
    bool handScrolling;
    bool mustAllocateStyleOptions;
    bool mustResizeBackgroundPixmap;
    bool fullUpdatePending;
    bool hasUpdateClip;

    QRectF sceneRect;
    void updateLastCenterPoint();

    qint64 horizontalScroll() const;
    qint64 verticalScroll() const;

    QRectF mapRectToScene(const QRect &rect) const;
    QRectF mapRectFromScene(const QRectF &rect) const;

    QRect updateClip;
    QPointF mousePressItemPoint;
    QPointF mousePressScenePoint;
    QPoint mousePressViewPoint;
    QPoint mousePressScreenPoint;
    QPointF lastMouseMoveScenePoint;
    QPoint lastMouseMoveScreenPoint;
    QPoint dirtyScrollOffset;
    Qt::MouseButton mousePressButton;
    QTransform matrix;
    qint64 scrollX, scrollY;
    void updateScroll();

    qreal leftIndent;
    qreal topIndent;

    // Replaying mouse events
    QMouseEvent lastMouseEvent;
    void replayLastMouseEvent();
    void storeMouseEvent(QMouseEvent *event);
    void mouseMoveEventHandler(QMouseEvent *event);

    QPointF lastCenterPoint;
    Qt::Alignment alignment;

    QGraphicsView::ViewportAnchor transformationAnchor;
    QGraphicsView::ViewportAnchor resizeAnchor;
    QGraphicsView::ViewportUpdateMode viewportUpdateMode;
    QGraphicsView::OptimizationFlags optimizationFlags;

    QPointer<QGraphicsScene> scene;
#ifndef QT_NO_RUBBERBAND
    QRect rubberBandRect;
    QRegion rubberBandRegion(const QWidget *widget, const QRect &rect) const;
    bool rubberBanding;
    Qt::ItemSelectionMode rubberBandSelectionMode;
#endif
    int handScrollMotions;

    QGraphicsView::CacheMode cacheMode;

    QVector<QStyleOptionGraphicsItem> styleOptions;
    QStyleOptionGraphicsItem *allocStyleOptionsArray(int numItems);
    void freeStyleOptionsArray(QStyleOptionGraphicsItem *array);

    QBrush backgroundBrush;
    QBrush foregroundBrush;
    QPixmap backgroundPixmap;
    QRegion backgroundPixmapExposed;

#ifndef QT_NO_CURSOR
    QCursor originalCursor;
    bool hasStoredOriginalCursor;
    void _q_setViewportCursor(const QCursor &cursor);
    void _q_unsetViewportCursor();
#endif

    QGraphicsSceneDragDropEvent *lastDragDropEvent;
    void storeDragDropEvent(const QGraphicsSceneDragDropEvent *event);
    void populateSceneDragDropEvent(QGraphicsSceneDragDropEvent *dest,
                                    QDropEvent *source);

    QRect mapToViewRect(const QGraphicsItem *item, const QRectF &rect) const;
    QRegion mapToViewRegion(const QGraphicsItem *item, const QRectF &rect) const;
    QRegion dirtyRegion;
    QRect dirtyBoundingRect;
    void processPendingUpdates();
    inline void updateAll()
    {
        viewport->update();
        fullUpdatePending = true;
        dirtyBoundingRect = QRect();
        dirtyRegion = QRegion();
    }

    inline void dispatchPendingUpdateRequests()
    {
        {
            if (qt_widget_private(viewport)->paintOnScreen())
                QCoreApplication::sendPostedEvents(viewport, QEvent::UpdateRequest);
            QCoreApplication::sendPostedEvents(viewport->window(), QEvent::UpdateRequest);
        }
    }

    void setUpdateClip(QGraphicsItem *);

    inline bool updateRectF(const QRectF &rect)
    {
        if (rect.isEmpty())
            return false;
        if (optimizationFlags & QGraphicsView::DontAdjustForAntialiasing)
            return updateRect(rect.toAlignedRect().adjusted(-1, -1, 1, 1));
        return updateRect(rect.toAlignedRect().adjusted(-2, -2, 2, 2));
    }

    bool updateRect(const QRect &rect);
    bool updateRegion(const QRectF &rect, const QTransform &xform);
    bool updateSceneSlotReimplementedChecked;
    QRegion exposedRegion;

    QList<QGraphicsItem *> findItems(const QRegion &exposedRegion, bool *allItems,
                                     const QTransform &viewTransform) const;

    QPointF mapToScene(const QPointF &point) const;
    QRectF mapToScene(const QRectF &rect) const;
    static void translateTouchEvent(QGraphicsViewPrivate *d, QTouchEvent *touchEvent);
};

QT_END_NAMESPACE

#endif // QT_NO_GRAPHICSVIEW

#endif
