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
#include "qbackingstore_p.h"
#include "qapplication_p.h"

QT_BEGIN_NAMESPACE

class QWindowSurfacePrivate
{
public:
    QWindowSurfacePrivate(QWidget *w)
        : window(w)
    {
    }

    QWidget *window;
    QRect geometry;
    QRegion staticContents;
    QList<QImage*> bufferImages;
};

/*!
    \class QWindowSurface
    \since 4.3
    \internal
    \preliminary
    \ingroup qws qpa

    \brief The QWindowSurface class provides the drawing area for top-level
    windows.
*/


/*!
    \fn void QWindowSurface::beginPaint(const QRegion &region)

    This function is called before painting onto the surface begins,
    with the \a region in which the painting will occur.

    \sa endPaint(), paintDevice()
*/

/*!
    \fn void QWindowSurface::endPaint(const QRegion &region)

    This function is called after painting onto the surface has ended,
    with the \a region in which the painting was performed.

    \sa beginPaint(), paintDevice()
*/

/*!
    \fn void QWindowSurface::flush(QWidget *widget, const QRegion &region,
                                  const QPoint &offset)

    Flushes the given \a region from the specified \a widget onto the
    screen.

    Note that the \a offset parameter is currently unused.
*/

/*!
    \fn QPaintDevice* QWindowSurface::paintDevice()

    Implement this function to return the appropriate paint device.
*/

/*!
    Constructs an empty surface for the given top-level \a window.
*/
QWindowSurface::QWindowSurface(QWidget *window)
    : d_ptr(new QWindowSurfacePrivate(window))
{
    if(window)
        window->setWindowSurface(this);
}

/*!
    Destroys this surface.
*/
QWindowSurface::~QWindowSurface()
{
    if (d_ptr->window)
        d_ptr->window->d_func()->extra->topextra->windowSurface = 0;
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

void QWindowSurface::endPaint(const QRegion &)
{
//     QApplication::syncX();
    qDeleteAll(d_ptr->bufferImages);
    d_ptr->bufferImages.clear();
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
}

/*!
    Returns the currently allocated area on the screen.
*/
QRect QWindowSurface::geometry() const
{
    return d_ptr->geometry;
}

/*!
    \fn QRect QWindowSurface::scroll(const QRegion &area, int dx, int dy)

    Scrolls the given \a area \a dx pixels to the right and \a dy
    downward; both \a dx and \a dy may be negative.

    Returns true if the area was scrolled successfully; false otherwise.
*/

/*!
    Returns a QImage pointer which represents the actual buffer the \a widget
    is drawn into or null if this is unavailable.

    You must call beginPaint() before you call this function and the returned
    pointer is only valid until endPaint() is called.
*/
QImage* QWindowSurface::buffer(const QWidget *widget)
{
    if (widget->window() != window())
        return nullptr;

    QPaintDevice *pdev = paintDevice();
    if (!pdev || pdev->devType() != QInternal::Image)
        return nullptr;

    const QPoint off = offset(widget);
    QImage *img = static_cast<QImage*>(pdev);

    QRect rect(off, widget->size());
    rect &= QRect(QPoint(), img->size());

    if (rect.isEmpty())
        return nullptr;

    img = new QImage(img->copy(rect));
    d_ptr->bufferImages.append(img);

    return img;
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

void QWindowSurface::setStaticContents(const QRegion &region)
{
    d_ptr->staticContents = region;
}

QRegion QWindowSurface::staticContents() const
{
    return d_ptr->staticContents;
}

bool QWindowSurface::hasStaticContents() const
{
    return !d_ptr->staticContents.isEmpty();
}

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

QT_END_NAMESPACE




