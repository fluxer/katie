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

#include "qwindowsurface_p.h"
#include "qwidget.h"
#include "qwidget_p.h"
#include "qapplication_p.h"
#include "qt_x11_p.h"

QT_BEGIN_NAMESPACE

void qt_scrollRectInImage(QImage *img, const QRect &rect, const QPoint &offset)
{
    const QRect imageRect(0, 0, img->width(), img->height());
    const QRect r = rect & imageRect & imageRect.translated(-offset);
    const QPoint p = rect.topLeft() + offset;

    if (r.isEmpty())
        return;

    uchar *mem = img->bits();

    int lineskip = img->bytesPerLine();
    int depth = img->depth() >> 3;

    const uchar *src;
    uchar *dest;

    if (r.top() < p.y()) {
        src = mem + r.bottom() * lineskip + r.left() * depth;
        dest = mem + (p.y() + r.height() - 1) * lineskip + p.x() * depth;
        lineskip = -lineskip;
    } else {
        src = mem + r.top() * lineskip + r.left() * depth;
        dest = mem + p.y() * lineskip + p.x() * depth;
    }

    const int w = r.width();
    int h = r.height();
    const int bytes = w * depth;

    // overlapping segments?
    if (offset.y() == 0 && qAbs(offset.x()) < w) {
        do {
            ::memmove(dest, src, bytes);
            dest += lineskip;
            src += lineskip;
        } while (--h);
    } else {
        do {
            ::memcpy(dest, src, bytes);
            dest += lineskip;
            src += lineskip;
        } while (--h);
    }
}

class QWindowSurfacePrivate
{
public:
    QWindowSurfacePrivate(QWidget *w)
        : window(w), image(nullptr)
    {
    }

    QWidget *window;
    QRect geometry;
    QImage *image;

#ifdef Q_WS_X11
    GC gc;
#ifndef QT_NO_XRENDER
    bool translucentBackground;
#endif
#endif
};  

/*!
    \class QWindowSurface
    \since 4.3
    \internal
    \preliminary

    \brief The QWindowSurface class provides the drawing area for top-level
    windows.
*/

/*!
    Constructs an empty surface for the given top-level \a window.
*/
QWindowSurface::QWindowSurface(QWidget *window)
    : d_ptr(new QWindowSurfacePrivate(window))
{
#ifdef Q_WS_X11
    d_ptr->gc = XCreateGC(qt_x11Data->display, window->handle(), 0, 0);
#ifndef QT_NO_XRENDER
    d_ptr->translucentBackground = (qt_x11Data->use_xrender && window->x11Info().depth() == 32);
#endif
#endif

    window->setWindowSurface(this);
}

/*!
    Destroys this surface.
*/
QWindowSurface::~QWindowSurface()
{
    if (d_ptr->window) {
        d_ptr->window->d_func()->extra->topextra->windowSurface = 0;
    }
#ifdef Q_WS_X11
    XFreeGC(qt_x11Data->display, d_ptr->gc);
#endif
    if (d_ptr->image) {
        delete d_ptr->image;
    }
    delete d_ptr;
}

/*!
    Returns a pointer to the top-level window associated with this
    surface.
*/
QWidget* QWindowSurface::window() const
{
    return d_ptr->window;
}

/*!
    Returns the appropriate paint device.
*/
QPaintDevice *QWindowSurface::paintDevice()
{
    return d_ptr->image;
}

/*!
    This function is called before painting onto the surface begins,
    with the \a region in which the painting will occur.

    \sa endPaint(), paintDevice()
*/
void QWindowSurface::beginPaint(const QRegion &rgn)
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

/*!
    This function is called after painting onto the surface has ended,
    with the \a region in which the painting was performed.

    \sa beginPaint(), paintDevice()
*/
void QWindowSurface::endPaint(const QRegion &)
{
//     QApplication::syncX();
}

/*!
    Flushes the given \a region from the specified \a widget onto the
    screen.

    Note that the \a offset parameter is currently unused.
*/

void QWindowSurface::flush(QWidget *widget, const QRegion &rgn, const QPoint &offset)
{
    // Not ready for painting yet, bail out. This can happen in
    // QWidget::create_sys()
    if (!d_ptr->image || rgn.rectCount() == 0) {
        return;
    }

#ifdef Q_WS_X11
    extern void *qt_getClipRects(const QRegion &r, int &num); // in qpaintengine_x11.cpp
    QPoint wOffset = qt_qwidget_data(widget)->wrect.topLeft();

    if (widget->window() != window()) {
        XFreeGC(qt_x11Data->display, d_ptr->gc);
        d_ptr->gc = XCreateGC(qt_x11Data->display, widget->handle(), 0, 0);
    }

    QRegion wrgn(rgn);
    if (!wOffset.isNull()) {
        wrgn.translate(-wOffset);
    }

    if (wrgn.rectCount() != 1) {
        int num;
        XRectangle *rects = (XRectangle *)qt_getClipRects(wrgn, num);
        XSetClipRectangles(qt_x11Data->display, d_ptr->gc, 0, 0, rects, num, YXBanded);
    }

    QPoint widgetOffset = offset + wOffset;
    QRect clipRect = widget->rect().translated(widgetOffset).intersected(d_ptr->image->rect());

    QRect br = rgn.boundingRect().translated(offset).intersected(clipRect);
    QPoint wpos = br.topLeft() - widgetOffset;

    const int depth = widget->x11Info().depth();
    const int bw = br.width();
    const int bh = br.height();
    QImage image = d_ptr->image->copy(br);
    if (image.depth() != 32) {
        image = image.convertToFormat(QImage::Format_RGB32);
    }
    XImage *xi = XCreateImage(qt_x11Data->display, (Visual *)widget->x11Info().visual(), depth, ZPixmap, 0, 0, bw, bh, 32, 0);
    Q_CHECK_PTR(xi);
    QX11Data::copyQImageToXImage(image, xi);
    XPutImage(qt_x11Data->display, widget->handle(), d_ptr->gc, xi, 0, 0, wpos.x(), wpos.y(), bw, bh);
    XDestroyImage(xi);

    if (wrgn.rectCount() != 1)
        XSetClipMask(qt_x11Data->display, d_ptr->gc, XNone);
#endif // Q_WS_X11
}

/*!
    Sets the currently allocated area to be the given \a rect.

    This function is called whenever area covered by the top-level
    window changes.

    \sa geometry()
*/
void QWindowSurface::setGeometry(const QRect &rect)
{
    d_ptr->geometry = rect;
    if (!d_ptr->image || d_ptr->image->width() < rect.width() || d_ptr->image->height() < rect.height()) {
        QImage::Format format = QImage::Format_ARGB32_Premultiplied;
#if defined(Q_WS_X11) && !defined(QT_NO_XRENDER)
        if (!d_ptr->translucentBackground) {
            format = QImage::systemFormat();
        }
#endif
        int width = window()->width();
        int height = window()->height();
        if (d_ptr->image) {
            width = qMax(d_ptr->image->width(), width);
            height = qMax(d_ptr->image->height(), height);
        }

        if (width == 0 || height == 0) {
            delete d_ptr->image;
            d_ptr->image = nullptr;
            return;
        }

        QImage *oldImage = d_ptr->image;

        d_ptr->image = new QImage(width, height, format);

        delete oldImage;
    }
}

/*!
    Returns the currently allocated area on the screen.
*/
QRect QWindowSurface::geometry() const
{
    return d_ptr->geometry;
}

/*!
    Scrolls the given \a area \a dx pixels to the right and \a dy
    downward; both \a dx and \a dy may be negative.

    Returns true if the area was scrolled successfully; false otherwise.
*/
bool QWindowSurface::scroll(const QRegion &area, int dx, int dy)
{
    if (!d_ptr->image || d_ptr->image->isNull()) {
        return false;
    }

    qt_scrollRectInImage(d_ptr->image, area.boundingRect(), QPoint(dx, dy));

    return true;
}


/*!
  Returns the offset of \a widget in the coordinates of this
  window surface.
 */
QPoint QWindowSurface::offset(const QWidget *widget) const
{
    return widget->mapTo(d_ptr->window, QPoint());
}

/*!
  \fn QRect QWindowSurface::rect(const QWidget *widget) const

  Returns the rectangle for \a widget in the coordinates of this
  window surface.
*/

QT_END_NAMESPACE
