/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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
#ifndef QT_NO_XSHM
    bool needsSync;
#endif
#ifndef QT_NO_XRENDER
    bool translucentBackground;
#endif
#endif
    bool inSetGeometry;
};

QRasterWindowSurface::QRasterWindowSurface(QWidget *window, bool setDefaultSurface)
    : QWindowSurface(window, setDefaultSurface), d_ptr(new QRasterWindowSurfacePrivate)
{
#ifdef Q_WS_X11
    d_ptr->gc = XCreateGC(qt_x11Data->display, window->handle(), 0, 0);
#ifndef QT_NO_XRENDER
    d_ptr->translucentBackground = qt_x11Data->use_xrender
        && window->x11Info().depth() == 32;
#endif
#ifndef QT_NO_XSHM
    d_ptr->needsSync = false;
#endif
#endif
    d_ptr->image = 0;
    d_ptr->inSetGeometry = false;
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

#if defined(Q_WS_X11) && !defined(QT_NO_XSHM)
void QRasterWindowSurface::syncX()
{
    // delay writing to the backbuffer until we know for sure X is done reading from it
    if (d_ptr->needsSync) {
        XSync(qt_x11Data->display, false);
        d_ptr->needsSync = false;
    }
}
#endif

void QRasterWindowSurface::beginPaint(const QRegion &rgn)
{
#if defined(Q_WS_X11) && !defined(QT_NO_XSHM)
    syncX();
#endif

#if defined(Q_WS_X11) && !defined(QT_NO_XRENDER)
    if (!qt_widget_private(window())->isOpaque && window()->testAttribute(Qt::WA_TranslucentBackground)) {
        QPainter p(d_ptr->image);
        p.setCompositionMode(QPainter::CompositionMode_Source);
        foreach (const QRect r, rgn.rects()) {
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
    if (src->format() != QImage::Format_RGB32 || (depth != 24 && depth != 32) || qt_x11Data->bppForDepth.value(depth) != 32) {
        Q_ASSERT(src->depth() >= 16);
        const QImage sub_src(src->scanLine(br.y()) + br.x() * (uint(src->depth()) / 8),
                                br.width(), br.height(), src->bytesPerLine(), src->format());
        QX11PixmapData *data = new QX11PixmapData(QPixmapData::PixmapType);
        data->xinfo = widget->x11Info();
        data->fromImage(sub_src, Qt::NoOpaqueDetection);
        QPixmap pm = QPixmap(data);
        XCopyArea(qt_x11Data->display, pm.handle(), widget->handle(), d_ptr->gc, 0 , 0 , br.width(), br.height(), wpos.x(), wpos.y());
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
    d->inSetGeometry = true;
    if (d->image == 0 || d->image->width() < rect.width() || d->image->height() < rect.height()) {
#if defined(Q_WS_X11) && !defined(QT_NO_XRENDER)
        if (d_ptr->translucentBackground)
            prepareBuffer(QImage::Format_ARGB32_Premultiplied);
        else
#endif
            prepareBuffer(QImage::systemFormat());
    }
    d->inSetGeometry = false;
}

// from qwindowsurface.cpp
extern void qt_scrollRectInImage(QImage *img, const QRect &rect, const QPoint &offset);

bool QRasterWindowSurface::scroll(const QRegion &area, int dx, int dy)
{
    Q_D(QRasterWindowSurface);

    if (!d->image || d->image->isNull())
        return false;

#if defined(Q_WS_X11) && !defined(QT_NO_XSHM)
    syncX();
#endif

    qt_scrollRectInImage(d->image, area.boundingRect(), QPoint(dx, dy));

    return true;
}

QWindowSurface::WindowSurfaceFeatures QRasterWindowSurface::features() const
{
    return QWindowSurface::AllFeatures;
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

    if (oldImage && d->inSetGeometry && hasStaticContents()) {
        const uchar *src = oldImage->bits();
        uchar *dst = d->image->bits();

        const int srcBytesPerLine = oldImage->bytesPerLine();
        const int dstBytesPerLine = d->image->bytesPerLine();
        const int bytesPerPixel = oldImage->depth() >> 3;

        QRegion staticRegion(staticContents());
        // Make sure we're inside the boundaries of the old image.
        staticRegion &= QRect(0, 0, oldImage->width(), oldImage->height());
        const QVector<QRect> &rects = staticRegion.rects();
        const QRect *srcRect = rects.constData();

        // Copy the static content of the old image into the new one.
        int numRectsLeft = rects.size();
        do {
            const int bytesOffset = srcRect->x() * bytesPerPixel;
            const int dy = srcRect->y();

            // Adjust src and dst to point to the right offset.
            const uchar *s = src + dy * srcBytesPerLine + bytesOffset;
            uchar *d = dst + dy * dstBytesPerLine + bytesOffset;
            const int numBytes = srcRect->width() * bytesPerPixel;

            int numScanLinesLeft = srcRect->height();
            do {
                ::memcpy(d, s, numBytes);
                d += dstBytesPerLine;
                s += srcBytesPerLine;
            } while (--numScanLinesLeft);

            ++srcRect;
        } while (--numRectsLeft);
    }

    delete oldImage;
}

QT_END_NAMESPACE




