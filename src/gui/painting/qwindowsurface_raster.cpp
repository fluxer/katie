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
#include "qpaintdevice.h"
#include "qwidget.h"

#include "qwindowsurface_raster_p.h"
#include "qimage.h"
#include "qwidget_p.h"

#ifdef Q_WS_X11
#include "qpixmap_x11_p.h"
#include "qt_x11_p.h"
#include "qx11info_x11.h"
#endif
#include "qdrawhelper_p.h"

QT_BEGIN_NAMESPACE

class QRasterWindowSurfacePrivate
{
public:
    QImage *image;

#ifdef Q_WS_X11
    GC gc;
#ifndef QT_NO_XRENDER
    bool translucentBackground;
#endif
#endif
};

QRasterWindowSurface::QRasterWindowSurface(QWidget *window)
    : QWindowSurface(window), d_ptr(new QRasterWindowSurfacePrivate)
{
#ifdef Q_WS_X11
    d_ptr->gc = XCreateGC(qt_x11Data->display, window->handle(), 0, 0);
#ifndef QT_NO_XRENDER
    d_ptr->translucentBackground = qt_x11Data->use_xrender
        && window->x11Info().depth() == 32;
#endif
#endif
    d_ptr->image = 0;
}


QRasterWindowSurface::~QRasterWindowSurface()
{
#ifdef Q_WS_X11
    XFreeGC(qt_x11Data->display, d_ptr->gc);
#endif
    if (d_ptr->image)
        delete d_ptr->image;
}

QPaintDevice *QRasterWindowSurface::paintDevice()
{
    return d_ptr->image;
}

void QRasterWindowSurface::beginPaint(const QRegion &rgn)
{
#if defined(Q_WS_X11) && !defined(QT_NO_XRENDER)
    if (!qt_widget_private(window())->isOpaque && window()->testAttribute(Qt::WA_TranslucentBackground)) {
        QPainter p(d_ptr->image);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        foreach (const QRect &r, rgn.rects()) {
            p.fillRect(r, Qt::transparent);
        }
    }
#else
    Q_UNUSED(rgn);
#endif
}

void QRasterWindowSurface::flush(QWidget *widget, const QRegion &rgn, const QPoint &offset)
{
    Q_D(QRasterWindowSurface);

    // Not ready for painting yet, bail out. This can happen in
    // QWidget::create_sys()
    if (!d->image || rgn.rectCount() == 0)
        return;

#ifdef Q_WS_X11
    extern void *qt_getClipRects(const QRegion &r, int &num); // in qpaintengine_x11.cpp
    QPoint wOffset = qt_qwidget_data(widget)->wrect.topLeft();

    if (widget->window() != window()) {
        XFreeGC(qt_x11Data->display, d_ptr->gc);
        d_ptr->gc = XCreateGC(qt_x11Data->display, widget->handle(), 0, 0);
    }

    QRegion wrgn(rgn);
    if (!wOffset.isNull())
        wrgn.translate(-wOffset);

    if (wrgn.rectCount() != 1) {
        int num;
        XRectangle *rects = (XRectangle *)qt_getClipRects(wrgn, num);
        XSetClipRectangles(qt_x11Data->display, d_ptr->gc, 0, 0, rects, num, YXBanded);
    }

    QPoint widgetOffset = offset + wOffset;
    QRect clipRect = widget->rect().translated(widgetOffset).intersected(d_ptr->image->rect());

    QRect br = rgn.boundingRect().translated(offset).intersected(clipRect);
    QPoint wpos = br.topLeft() - widgetOffset;

    int depth = widget->x11Info().depth();
    const QImage *src = d->image;
    if (src->format() != QImage::Format_RGB32 || src->depth() != 32 || depth != 32) {
        Q_ASSERT(src->depth() >= 16);
        QX11PixmapData data(QPixmapData::PixmapType);
        data.xinfo = widget->x11Info();
        data.fromImage(src->copy(br), Qt::NoOpaqueDetection);
        XCopyArea(qt_x11Data->display, data.handle(), widget->handle(), d_ptr->gc, 0 , 0 , br.width(), br.height(), wpos.x(), wpos.y());
    } else {
        // qpaintengine_x11.cpp
        extern void qt_x11_drawImage(const QRect &rect, const QPoint &pos, const QImage *image, Drawable hd, GC gc, Display *dpy, Visual *visual, int depth);
        qt_x11_drawImage(br, wpos, src, widget->handle(), d_ptr->gc, qt_x11Data->display, (Visual *)widget->x11Info().visual(), depth);
    }

    if (wrgn.rectCount() != 1)
        XSetClipMask(qt_x11Data->display, d_ptr->gc, XNone);
#endif // Q_WS_X11
}

void QRasterWindowSurface::setGeometry(const QRect &rect)
{
    QWindowSurface::setGeometry(rect);
    Q_D(QRasterWindowSurface);
    if (d->image == 0 || d->image->width() < rect.width() || d->image->height() < rect.height()) {
#if defined(Q_WS_X11) && !defined(QT_NO_XRENDER)
        if (d_ptr->translucentBackground)
            prepareBuffer(QImage::Format_ARGB32_Premultiplied);
        else
#endif
            prepareBuffer(QImage::systemFormat());
    }
}

// from qwindowsurface.cpp
extern void qt_scrollRectInImage(QImage *img, const QRect &rect, const QPoint &offset);

bool QRasterWindowSurface::scroll(const QRegion &area, int dx, int dy)
{
    Q_D(QRasterWindowSurface);

    if (!d->image || d->image->isNull())
        return false;

    qt_scrollRectInImage(d->image, area.boundingRect(), QPoint(dx, dy));

    return true;
}

void QRasterWindowSurface::prepareBuffer(QImage::Format format)
{
    Q_D(QRasterWindowSurface);

    int width = window()->width();
    int height = window()->height();
    if (d->image) {
        width = qMax(d->image->width(), width);
        height = qMax(d->image->height(), height);
    }

    if (width == 0 || height == 0) {
        delete d->image;
        d->image = 0;
        return;
    }

    QImage *oldImage = d->image;

    d->image = new QImage(width, height, format);

    if (oldImage && hasStaticContents()) {
        const uchar *src = oldImage->constBits();
        uchar *dst = d->image->bits();

        const int srcBytesPerLine = oldImage->bytesPerLine();
        const int dstBytesPerLine = d->image->bytesPerLine();
        const int bytesPerPixel = oldImage->depth() >> 3;

        QRegion staticRegion(staticContents());
        // Make sure we're inside the boundaries of the old image.
        staticRegion &= QRect(0, 0, oldImage->width(), oldImage->height());

        // Copy the static content of the old image into the new one.
        foreach(const QRect &srcRect, staticRegion.rects()) {
            const int bytesOffset = srcRect.x() * bytesPerPixel;
            const int dy = srcRect.y();

            // Adjust src and dst to point to the right offset.
            const uchar *s = src + dy * srcBytesPerLine + bytesOffset;
            uchar *d = dst + dy * dstBytesPerLine + bytesOffset;
            const int numBytes = srcRect.width() * bytesPerPixel;

            int numScanLinesLeft = srcRect.height();
            do {
                ::memcpy(d, s, numBytes);
                d += dstBytesPerLine;
                s += srcBytesPerLine;
            } while (--numScanLinesLeft);
        };
    }

    delete oldImage;
}

QT_END_NAMESPACE




