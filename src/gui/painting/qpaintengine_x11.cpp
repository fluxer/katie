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

#include "qdebug.h"
#include "qwidget_p.h"
#include "qpaintengine_x11_p.h"
#include "qt_x11_p.h"

QT_BEGIN_NAMESPACE

extern Qt::HANDLE qt_x11Handle(const QPaintDevice *pd);
extern const QX11Info *qt_x11Info(const QPaintDevice *pd);

// hack, so we don't have to make QRegion::clipRectangles() public or include
// X11 headers in qregion.h
Q_GUI_EXPORT void *qt_getClipRects(const QRegion &r, int &num)
{
    return r.clipRectangles(num);
}

void QX11PaintEnginePrivate::init()
{
    hd = 0;
#ifndef QT_NO_XRENDER
    picture = 0;
#endif
    xinfo = 0;
    gc = 0;
}

static QPaintEngine::PaintEngineFeatures qt_decide_features()
{
    QPaintEngine::PaintEngineFeatures features;

    if (qt_x11Data->use_xrender) {
        features |= QPaintEngine::Antialiasing;
    }

    return features;
}

QX11PaintEngine::QX11PaintEngine()
    : QPaintEngine(*(new QX11PaintEnginePrivate), qt_decide_features())
{
    d_func()->init();
}

QX11PaintEngine::QX11PaintEngine(QX11PaintEnginePrivate &dptr)
    : QPaintEngine(dptr, qt_decide_features())
{
    d_func()->init();
}

QX11PaintEngine::~QX11PaintEngine()
{
}

bool QX11PaintEngine::begin(QPaintDevice *pdev)
{
    Q_D(QX11PaintEngine);
    d->xinfo = qt_x11Info(pdev);
    QWidget *w = d->pdev->devType() == QInternal::Widget ? static_cast<QWidget *>(d->pdev) : 0;
    const bool isAlienWidget = w && !w->internalWinId() && w->testAttribute(Qt::WA_WState_Created);
#ifndef QT_NO_XRENDER
    if (w) {
        if (isAlienWidget)
            d->picture = (::Picture)w->nativeParentWidget()->x11PictureHandle();
        else
            d->picture = (::Picture)w->x11PictureHandle();
    }
#else
    d->picture = 0;
#endif
    d->hd = !isAlienWidget ? qt_x11Handle(pdev) : qt_x11Handle(w->nativeParentWidget());

    Q_ASSERT(d->xinfo != 0);

    d->gc = XCreateGC(d->xinfo->display(), d->hd, 0, 0);

    if (isAlienWidget) {
        // Set system clip for alien widgets painting outside the paint event.
        // This is not a problem with native windows since the windowing system
        // will handle the clip.
        QWidgetPrivate *wd = w->d_func();
        QRegion widgetClip(wd->clipRect());
        wd->clipToEffectiveMask(widgetClip);
        wd->subtractOpaqueSiblings(widgetClip);
        widgetClip.translate(w->mapTo(w->nativeParentWidget(), QPoint()));
        setSystemClip(widgetClip);
    }

    setDirty(QPaintEngine::DirtyClipRegion);
    setDirty(QPaintEngine::DirtyPen);
    setDirty(QPaintEngine::DirtyBrush);
    setDirty(QPaintEngine::DirtyBackground);

    return true;
}

bool QX11PaintEngine::end()
{
    Q_D(QX11PaintEngine);

#if !defined(QT_NO_XRENDER)
    if (d->picture) {
        // reset clipping/subwindow mode on our render picture
        XRenderPictureAttributes attrs;
        attrs.subwindow_mode = ClipByChildren;
        attrs.clip_mask = XNone;
        XRenderChangePicture(
            d->xinfo->display(), d->picture,
            CPClipMask | CPSubwindowMode,
            &attrs
        );
    }
#endif

    if (d->gc && d->pdev->painters < 2) {
        XFreeGC(d->xinfo->display(), d->gc);
        d->gc = 0;
    }

    // Restore system clip for alien widgets painting outside the paint event.
    if (d->pdev->devType() == QInternal::Widget && !static_cast<QWidget *>(d->pdev)->internalWinId())
        setSystemClip(QRegion());

    return true;
}

void QX11PaintEngine::updateState(const QPaintEngineState &state)
{
    Q_D(QX11PaintEngine);
    if (state.state() & QPaintEngine::DirtyHints) {
#if !defined(QT_NO_XRENDER)
        if (qt_x11Data->use_xrender && d->picture) {
            XRenderPictureAttributes attrs;
            attrs.poly_edge = (state.renderHints() & QPainter::Antialiasing) ? PolyEdgeSmooth : PolyEdgeSharp;
            XRenderChangePicture(d->xinfo->display(), d->picture, CPPolyEdge, &attrs);
        }
#endif
    }
}

void QX11PaintEngine::drawPolygon(const QPointF *polygonPoints, int pointCount, PolygonDrawMode mode)
{
    Q_D(QX11PaintEngine);
    QPainterPath path(polygonPoints[0]);
    for (int i = 1; i < pointCount; ++i)
        path.lineTo(polygonPoints[i]);
    if (mode == PolylineMode) {
        QBrush oldBrush = painter()->brush();
        painter()->setBrush(Qt::NoBrush);
        path.setFillRule(Qt::WindingFill);
        drawPath(path);
        painter()->setBrush(oldBrush);
    } else {
        path.setFillRule(mode == OddEvenMode ? Qt::OddEvenFill : Qt::WindingFill);
        path.closeSubpath();
        drawPath(path);
    }
}

void QX11PaintEngine::drawPath(const QPainterPath &path)
{
    Q_D(QX11PaintEngine);
    if (path.isEmpty())
        return;

    // NOTE: the user of this painter (QWidget) uses backing store so optimizing drawing here is
    // not important, it is used only when WA_PaintOnScreen is set and it is mostly painting of
    // QPixmap
    const QRect pathbounds = path.boundingRect().toRect();
    QImage pathimage(pathbounds.size(), QImage::Format_ARGB32_Premultiplied);
    pathimage.fill(Qt::transparent);

    QPainter pathpainter(&pathimage);
    pathpainter.setPen(painter()->pen());
    pathpainter.setBrush(painter()->brush());
    pathpainter.setBrushOrigin(painter()->brushOrigin());
    pathpainter.setBackground(painter()->background());
    pathpainter.setBackgroundMode(painter()->backgroundMode());
    pathpainter.setFont(painter()->font());
    pathpainter.setOpacity(painter()->opacity());
    pathpainter.setClipping(painter()->hasClipping());
    pathpainter.setClipRegion(painter()->clipRegion());
    pathpainter.setClipPath(painter()->clipPath());
    pathpainter.setTransform(painter()->transform());
    pathpainter.setWorldTransform(painter()->worldTransform());
    pathpainter.setWorldMatrixEnabled(painter()->worldMatrixEnabled());
    pathpainter.setViewTransformEnabled(painter()->viewTransformEnabled());
    pathpainter.setViewport(painter()->viewport());
    pathpainter.setRenderHints(painter()->renderHints());
    pathpainter.setLayoutDirection(painter()->layoutDirection());
    pathpainter.drawPath(path);
    pathpainter.end();

    drawImage(QRectF(), pathimage, pathimage.rect(), Qt::AutoColor);
}

void QX11PaintEngine::drawPixmap(const QRectF &r, const QPixmap &px, const QRectF &sr)
{
    drawImage(r, px.toImage(), sr);
}

void QX11PaintEngine::drawImage(const QRectF &r, const QImage &pm, const QRectF &sr, Qt::ImageConversionFlags flags)
{
    Q_D(QX11PaintEngine);
    if (pm.isNull()) {
        return;
    }
    XImage *ximage = XCreateImage(
        d->xinfo->display(), (Visual *)d->xinfo->visual(),
        d->xinfo->depth(),
        (pm.depth() == 1) ? XYPixmap : ZPixmap,
        0, 0,
        pm.width(), pm.height(),
        pm.depth(), 0
    );
    Q_CHECK_PTR(ximage);
    if (Q_UNLIKELY(!ximage)) {
        qWarning("QX11PaintEngine::drawImage: Could not create X11 image");
        return;
    }
    bool freedata = false;
    QX11Data::copyQImageToXImage(pm, ximage, &freedata);
    const QRect srr = sr.toRect();
    const QRect rr = r.toRect();
    XPutImage(
        d->xinfo->display(), d->hd, d->gc, ximage,
        srr.x(), srr.y(),
        rr.x(), rr.y(),
        pm.width(), pm.height()
    );
    QX11Data::destroyXImage(ximage, freedata);
}

QT_END_NAMESPACE
