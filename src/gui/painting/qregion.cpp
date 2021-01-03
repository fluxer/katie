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

#include "qregion.h"
#include "qpainterpath.h"
#include "qpolygon.h"
#include "qdatastream.h"
#include "qvariant.h"
#include "qdebug.h"
#include "qimage.h"
#include "qbitmap.h"
#include "qt_x11_p.h"

#include <limits.h>

QT_BEGIN_NAMESPACE

/*!
    \class QRegion
    \brief The QRegion class specifies a clip region for a painter.

    \ingroup painting
    \ingroup shared

    QRegion is used with QPainter::setClipRegion() to limit the paint
    area to what needs to be painted. There is also a QWidget::repaint()
    function that takes a QRegion parameter. QRegion is the best tool for
    minimizing the amount of screen area to be updated by a repaint.

    This class is not suitable for constructing shapes for rendering, especially
    as outlines. Use QPainterPath to create paths and shapes for use with
    QPainter.

    QRegion is an \l{implicitly shared} class.

    \section1 Creating and Using Regions

    A region can be created from a rectangle, an ellipse, a polygon or
    a bitmap. Complex regions may be created by combining simple
    regions using united(), intersected(), subtracted(), or xored() (exclusive
    or). You can move a region using translate().

    You can test whether a region isEmpty() or if it
    contains() a QPoint or QRect. The bounding rectangle can be found
    with boundingRect().

    The function rects() gives a decomposition of the region into
    rectangles.

    Example of using complex regions:
    \snippet doc/src/snippets/code/src_gui_painting_qregion.cpp 0

    \sa QPainter::setClipRegion(), QPainter::setClipRect(), QPainterPath
*/


/*!
    \enum QRegion::RegionType

    Specifies the shape of the region to be created.

    \value Rectangle  the region covers the entire rectangle.
    \value Ellipse  the region is an ellipse inside the rectangle.
*/

/*!
    \fn void QRegion::translate(const QPoint &point)

    \overload

    Translates the region \a{point}\e{.x()} along the x axis and
    \a{point}\e{.y()} along the y axis, relative to the current
    position. Positive values move the region to the right and down.

    Translates to the given \a point.
*/

/*!
    \fn Handle QRegion::handle() const

    Returns a platform-specific region handle. The \c Handle type is
    \c HRGN on Windows, \c Region on X11, and \c RgnHandle on Mac OS
    X. On \l{Qt for Embedded Linux} it is \c {void *}.

    \warning This function is not portable.
*/

/*****************************************************************************
  QRegion member functions
 *****************************************************************************/

/*!
    \fn QRegion::QRegion()

    Constructs an empty region.

    \sa isEmpty()
*/

/*!
    \fn QRegion::QRegion(const QRect &r, RegionType t)
    \overload

    Create a region based on the rectange \a r with region type \a t.

    If the rectangle is invalid a null region will be created.

    \sa QRegion::RegionType
*/

/*!
    \fn QRegion::QRegion(const QPolygon &a, Qt::FillRule fillRule)

    Constructs a polygon region from the point array \a a with the fill rule
    specified by \a fillRule.

    If \a fillRule is \l{Qt::WindingFill}, the polygon region is defined
    using the winding algorithm; if it is \l{Qt::OddEvenFill}, the odd-even fill
    algorithm is used.

    \warning This constructor can be used to create complex regions that will
    slow down painting when used.
*/

/*!
    \fn QRegion::QRegion(const QRegion &r)

    Constructs a new region which is equal to region \a r.
*/

/*!
    \fn QRegion::QRegion(const QBitmap &bm)

    Constructs a region from the bitmap \a bm.

    The resulting region consists of the pixels in bitmap \a bm that
    are Qt::color1, as if each pixel was a 1 by 1 rectangle.

    This constructor may create complex regions that will slow down
    painting when used. Note that drawing masked pixmaps can be done
    much faster using QPixmap::setMask().
*/


QRegion::QRegionData QRegion::shared_empty = {
    QAtomicInt(1), XCreateRegion(), Q_NULLPTR
};

static XRectangle qt_QRectToXRectangle(const QRect &rect)
{
    XRectangle xr;
    xr.x = qMax(SHRT_MIN, rect.x());
    xr.y = qMax(SHRT_MIN, rect.y());
    xr.width = qMin((int)USHRT_MAX, rect.width());
    xr.height = qMin((int)USHRT_MAX, rect.height());
    return xr;
}

static XPoint* qt_QPolygonToXPoints(const QPolygon &polygon)
{
    XPoint* xpoints =  static_cast<XPoint*>(::malloc(polygon.size() * sizeof(XPoint)));
    const QPoint* qpoints = polygon.data();
    for (int i = 0; i < polygon.size(); i++) {
        XPoint xp;
        xp.x = qMax(SHRT_MIN, qpoints[i].x());
        xp.y = qMax(SHRT_MIN, qpoints[i].y());
        xpoints[i] = xp;
    }
    return xpoints;
}

static Region qt_bitmapToRegion(const QBitmap& bitmap)
{
    QImage image = bitmap.toImage();

    Region region = XCreateRegion();

    XRectangle xr;

#define AddSpan \
        { \
            xr.x = qMax(SHRT_MIN, prev1); \
            xr.y = qMax(SHRT_MIN, y); \
            xr.width = qMin((int)USHRT_MAX, x - 1); \
            xr.height = qMin((int)USHRT_MAX, y); \
            XUnionRectWithRegion(&xr, region, region); \
        }

    const uchar zero = 0;
    bool little = image.format() == QImage::Format_MonoLSB;

    int x, y;
    for (y = 0; y < image.height(); ++y) {
        const uchar *line = image.scanLine(y);
        int w = image.width();
        uchar all = zero;
        int prev1 = -1;
        for (x = 0; x < w;) {
            uchar byte = line[x / 8];
            if (x > w - 8 || byte!=all) {
                if (little) {
                    for (int b = 8; b > 0 && x < w; --b) {
                        if (!(byte & 0x01) == !all) {
                            // More of the same
                        } else {
                            // A change.
                            if (all!=zero) {
                                AddSpan
                                all = zero;
                            } else {
                                prev1 = x;
                                all = ~zero;
                            }
                        }
                        byte >>= 1;
                        ++x;
                    }
                } else {
                    for (int b = 8; b > 0 && x < w; --b) {
                        if (!(byte & 0x80) == !all) {
                            // More of the same
                        } else {
                            // A change.
                            if (all != zero) {
                                AddSpan
                                all = zero;
                            } else {
                                prev1 = x;
                                all = ~zero;
                            }
                        }
                        byte <<= 1;
                        ++x;
                    }
                }
            } else {
                x += 8;
            }
        }
        if (all != zero) {
            AddSpan
        }
    }
#undef AddSpan

    return region;
}

/*! \internal
    Returns true if \a rect is guaranteed to be fully contained in \a region.
    A false return value does not guarantee the opposite.
*/
bool qt_region_strictContains(const QRegion &region, const QRect &rect)
{
    if (region.isEmpty() || !rect.isValid())
        return false;

    const QRect r1 = region.boundingRect();
    return (rect.left() >= r1.left() && rect.right() <= r1.right()
            && rect.top() >= r1.top() && rect.bottom() <= r1.bottom());
}

inline bool rect_intersects(const QRect &r1, const QRect &r2)
{
    return (r1.right() >= r2.left() && r1.left() <= r2.right() &&
            r1.bottom() >= r2.top() && r1.top() <= r2.bottom());
}

Q_AUTOTEST_EXPORT QPainterPath qt_regionToPath(const QRegion &region)
{
    QPainterPath result;
    result.addRect(region.boundingRect());
    return result;
}

/*!
    Constructs a rectangular or elliptic region.

    If \a t is \c Rectangle, the region is the filled rectangle (\a x,
    \a y, \a w, \a h). If \a t is \c Ellipse, the region is the filled
    ellipse with center at (\a x + \a w / 2, \a y + \a h / 2) and size
    (\a w ,\a h).
*/
QRegion::QRegion(int x, int y, int w, int h, RegionType t)
{
    QRegion tmp(QRect(x, y, w, h), t);
    tmp.d->ref.ref();
    d = tmp.d;
}


/*!
    \fn QRegion::~QRegion()
    \internal

    Destroys the region.
*/

void QRegion::detach()
{
    if (d->ref != 1)
        *this = copy();
    else if (d->xrectangles) {
        ::free(d->xrectangles);
        d->xrectangles = Q_NULLPTR;
    }
}

/*****************************************************************************
  QRegion stream functions
 *****************************************************************************/

/*!
    \fn QRegion &QRegion::operator=(const QRegion &r)

    Assigns \a r to this region and returns a reference to the region.
*/

/*!
    \fn void QRegion::swap(QRegion &other)
    \since 4.8

    Swaps region \a other with this region. This operation is very
    fast and never fails.
*/

/*!
    \relates QRegion

    Writes the region \a r to the stream \a s and returns a reference
    to the stream.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/
#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &s, const QRegion &r)
{
    s << r.rects();
    return s;
}

/*!
    \relates QRegion

    Reads a region from the stream \a s into \a r and returns a
    reference to the stream.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/

QDataStream &operator>>(QDataStream &s, QRegion &r)
{
    QVector<QRect> rects;
    s >> rects;
    foreach (const QRect &rect, rects) {
        r = r.united(rect);
    }
    return s;
}
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug s, const QRegion &r)
{
    QVector<QRect> rects = r.rects();
    s.nospace() << "QRegion(size=" << rects.size() << "), "
                << "bounds = " << r.boundingRect() << '\n';
    for (int i=0; i<rects.size(); ++i)
        s << "- " << i << rects.at(i) << '\n';
    return s;
}
#endif


// These are not inline - they can be implemented better on some platforms
//  (eg. Windows at least provides 3-variable operations).  For now, simple.


/*!
    Applies the united() function to this region and \a r. \c r1|r2 is
    equivalent to \c r1.united(r2).

    \sa united(), operator+()
*/
const QRegion QRegion::operator|(const QRegion &r) const
{
    return united(r);
}

/*!
    Applies the united() function to this region and \a r. \c r1+r2 is
    equivalent to \c r1.united(r2).

    \sa united(), operator|()
*/
const QRegion QRegion::operator+(const QRegion &r) const
{
    return united(r);
}

/*!
   \overload
   \since 4.4
 */
const QRegion QRegion::operator+(const QRect &r) const
{
    return united(r);
}

/*!
    Applies the intersected() function to this region and \a r. \c r1&r2
    is equivalent to \c r1.intersected(r2).

    \sa intersected()
*/
const QRegion QRegion::operator&(const QRegion &r) const
{
    return intersected(r);
}

/*!
   \overload
   \since 4.4
 */
const QRegion QRegion::operator&(const QRect &r) const
{
    return intersected(r);
}

/*!
    Applies the subtracted() function to this region and \a r. \c r1-r2
    is equivalent to \c r1.subtracted(r2).

    \sa subtracted()
*/
const QRegion QRegion::operator-(const QRegion &r) const
{
    return subtracted(r);
}

/*!
    Applies the xored() function to this region and \a r. \c r1^r2 is
    equivalent to \c r1.xored(r2).

    \sa xored()
*/
const QRegion QRegion::operator^(const QRegion &r) const
{
    return xored(r);
}

/*!
    Applies the united() function to this region and \a r and assigns
    the result to this region. \c r1|=r2 is equivalent to \c
    {r1 = r1.united(r2)}.

    \sa united()
*/
QRegion& QRegion::operator|=(const QRegion &r)
{
    return *this = *this | r;
}

/*!
    \fn QRegion& QRegion::operator+=(const QRect &rect)

    Returns a region that is the union of this region with the specified \a rect.

    \sa united()
*/
/*!
    \fn QRegion& QRegion::operator+=(const QRegion &r)

    Applies the united() function to this region and \a r and assigns
    the result to this region. \c r1+=r2 is equivalent to \c
    {r1 = r1.united(r2)}.

    \sa intersected()
*/

/*!
  \fn QRegion& QRegion::operator&=(const QRegion &r)

  Applies the intersected() function to this region and \a r and
  assigns the result to this region. \c r1&=r2 is equivalent to \c
  r1 = r1.intersected(r2).

  \sa intersected()
*/
QRegion& QRegion::operator&=(const QRegion &r)
{
    return *this = *this & r;
}

/*!
   \overload
   \since 4.4
 */
QRegion& QRegion::operator&=(const QRect &r)
{
    return *this = *this & r;
}

/*!
  \fn QRegion& QRegion::operator-=(const QRegion &r)

  Applies the subtracted() function to this region and \a r and
  assigns the result to this region. \c r1-=r2 is equivalent to \c
  {r1 = r1.subtracted(r2)}.

  \sa subtracted()
*/
QRegion& QRegion::operator-=(const QRegion &r)
{
    return *this = *this - r;
}

/*!
    Applies the xored() function to this region and \a r and
    assigns the result to this region. \c r1^=r2 is equivalent to \c
    {r1 = r1.xored(r2)}.

    \sa xored()
*/
QRegion& QRegion::operator^=(const QRegion &r)
{
    return *this = *this ^ r;
}

/*!
    \fn bool QRegion::operator!=(const QRegion &other) const

    Returns true if this region is different from the \a other region;
    otherwise returns false.
*/

/*!
   Returns the region as a QVariant
*/
QRegion::operator QVariant() const
{
    return QVariant(QVariant::Region, this);
}

/*!
    \fn bool QRegion::operator==(const QRegion &r) const

    Returns true if the region is equal to \a r; otherwise returns
    false.
*/

/*!
    \fn bool QRegion::isNull() const

    Use isEmpty() instead.
*/


/*!
    \fn void QRegion::translate(int dx, int dy)

    Translates (moves) the region \a dx along the X axis and \a dy
    along the Y axis.
*/

/*!
    \fn QRegion QRegion::translated(const QPoint &p) const
    \overload
    \since 4.1

    Returns a copy of the regtion that is translated \a{p}\e{.x()}
    along the x axis and \a{p}\e{.y()} along the y axis, relative to
    the current position.  Positive values move the rectangle to the
    right and down.

    \sa translate()
*/

/*!
    \since 4.1

    Returns a copy of the region that is translated \a dx along the
    x axis and \a dy along the y axis, relative to the current
    position. Positive values move the region to the right and
    down.

    \sa translate()
*/

QRegion QRegion::translated(int dx, int dy) const
{
    QRegion result(*this);
    result.translate(dx, dy);
    return result;
}

/*!
    \since 4.2

    Returns true if this region intersects with \a region, otherwise
    returns false.
*/
bool QRegion::intersects(const QRegion &region) const
{
    if (isEmpty() || region.isEmpty())
        return false;

    return rect_intersects(boundingRect(), region.boundingRect());
}

/*!
    \fn bool QRegion::intersects(const QRect &rect) const
    \since 4.2

    Returns true if this region intersects with \a rect, otherwise
    returns false.
*/

/*!
    \fn int QRegion::rectCount() const
    \since 4.6

    Returns the number of rectangles that will be returned in rects().
*/

/*!
    \fn bool QRegion::isEmpty() const

    Returns true if the region is empty; otherwise returns false. An
    empty region is a region that contains no points.

    Example:
    \snippet doc/src/snippets/code/src_gui_painting_qregion_unix.cpp 0
*/

/*!
    \fn bool QRegion::contains(const QPoint &p) const

    Returns true if the region contains the point \a p; otherwise
    returns false.
*/

/*!
    \fn bool QRegion::contains(const QRect &r) const
    \overload

    Returns true if the region overlaps the rectangle \a r; otherwise
    returns false.
*/

/*!
    \fn QRegion QRegion::unite(const QRegion &r) const
    \obsolete

    Use united(\a r) instead.
*/

/*!
    \fn QRegion QRegion::unite(const QRect &rect) const
    \since 4.4
    \obsolete

    Use united(\a rect) instead.
*/

/*!
    \fn QRegion QRegion::united(const QRect &rect) const
    \since 4.4

    Returns a region which is the union of this region and the given \a rect.

    \sa intersected(), subtracted(), xored()
*/

/*!
    \fn QRegion QRegion::united(const QRegion &r) const
    \since 4.2

    Returns a region which is the union of this region and \a r.

    \img runion.png Region Union

    The figure shows the union of two elliptical regions.

    \sa intersected(), subtracted(), xored()
*/

/*!
    \fn QRegion QRegion::intersect(const QRegion &r) const
    \obsolete

    Use intersected(\a r) instead.
*/

/*!
    \fn QRegion QRegion::intersect(const QRect &rect) const
    \since 4.4
    \obsolete

    Use intersected(\a rect) instead.
*/

/*!
    \fn QRegion QRegion::intersected(const QRect &rect) const
    \since 4.4

    Returns a region which is the intersection of this region and the given \a rect.

    \sa subtracted(), united(), xored()
*/

/*!
    \fn QRegion QRegion::intersected(const QRegion &r) const
    \since 4.2

    Returns a region which is the intersection of this region and \a r.

    \img rintersect.png Region Intersection

    The figure shows the intersection of two elliptical regions.

    \sa subtracted(), united(), xored()
*/

/*!
    \fn QRegion QRegion::subtract(const QRegion &r) const
    \obsolete

    Use subtracted(\a r) instead.
*/

/*!
    \fn QRegion QRegion::subtracted(const QRegion &r) const
    \since 4.2

    Returns a region which is \a r subtracted from this region.

    \img rsubtract.png Region Subtraction

    The figure shows the result when the ellipse on the right is
    subtracted from the ellipse on the left (\c {left - right}).

    \sa intersected(), united(), xored()
*/

/*!
    \fn QRegion QRegion::eor(const QRegion &r) const
    \obsolete

    Use xored(\a r) instead.
*/

/*!
    \fn QRegion QRegion::xored(const QRegion &r) const
    \since 4.2

    Returns a region which is the exclusive or (XOR) of this region
    and \a r.

    \img rxor.png Region XORed

    The figure shows the exclusive or of two elliptical regions.

    \sa intersected(), united(), subtracted()
*/

/*!
    \fn QRect QRegion::boundingRect() const

    Returns the bounding rectangle of this region. An empty region
    gives a rectangle that is QRect::isNull().
*/

/*!
    \fn QVector<QRect> QRegion::rects() const

    Returns an array of non-overlapping rectangles that make up the
    region.

    The union of all the rectangles is equal to the original region.
*/

/*!
    \fn void QRegion::setRects(const QRect *rects, int number)

    Sets the region using the array of rectangles specified by \a rects and
    \a number.
    The rectangles \e must be optimally Y-X sorted and follow these restrictions:

    \list
    \o The rectangles must not intersect.
    \o All rectangles with a given top coordinate must have the same height.
    \o No two rectangles may abut horizontally (they should be combined
       into a single wider rectangle in that case).
    \o The rectangles must be sorted in ascending order, with Y as the major
       sort key and X as the minor sort key.
    \endlist
    \omit
    Only some platforms have these restrictions (Qt for Embedded Linux, X11 and Mac OS X).
    \endomit
*/

QRegion::QRegion()
    : d(&shared_empty)
{
    d->ref.ref();
}

QRegion::QRegion(const QRect &r, RegionType t)
{
    if (r.isEmpty()) {
        d = &shared_empty;
        d->ref.ref();
    } else {
        d = new QRegionData;
        d->ref = 1;
        if (t == Rectangle) {
            d->rgn = XCreateRegion();
            XRectangle xr = qt_QRectToXRectangle(r);
            XUnionRectWithRegion(&xr, d->rgn, d->rgn);
        } else if (t == Ellipse) {
            QPainterPath path;
            path.addEllipse(r.x(), r.y(), r.width(), r.height());
            QPolygon a = path.toSubpathPolygons().at(0).toPolygon();
            XPoint* xpoints = qt_QPolygonToXPoints(a);
            Region rgn = XPolygonRegion(xpoints, a.size(), EvenOddRule);
            ::free(xpoints);
            d->rgn = rgn;
        }
        d->xrectangles = Q_NULLPTR;
    }
}

QRegion::QRegion(const QPolygon &a, Qt::FillRule fillRule)
{
    if (a.isEmpty()) {
        d = &shared_empty;
        d->ref.ref();
    } else {
        XPoint* xpoints = qt_QPolygonToXPoints(a);
        Region rgn = XPolygonRegion(xpoints, a.size(), fillRule == Qt::WindingFill ? WindingRule : EvenOddRule);
        ::free(xpoints);
        d = new QRegionData;
        d->ref = 1;
        d->rgn = rgn;
        d->xrectangles = Q_NULLPTR;
    }
}

QRegion::QRegion(const QRegion &r)
{
    d = r.d;
    d->ref.ref();
}

QRegion::QRegion(const QBitmap &bm)
{
    if (bm.isNull()) {
        d = &shared_empty;
        d->ref.ref();
    } else {
        d = new QRegionData;
        d->ref = 1;
        d->rgn = qt_bitmapToRegion(bm);
        d->xrectangles = Q_NULLPTR;
    }
}

void QRegion::cleanUp(QRegion::QRegionData *x)
{
    if (x->rgn)
        XDestroyRegion(x->rgn);
    if (x->xrectangles)
        ::free(x->xrectangles);
    delete x;
}

QRegion::~QRegion()
{
    if (!d->ref.deref())
        cleanUp(d);
}


QRegion &QRegion::operator=(const QRegion &r)
{
    r.d->ref.ref();
    if (!d->ref.deref())
        cleanUp(d);
    d = r.d;
    return *this;
}

/*!
    \internal
*/
QRegion QRegion::copy() const
{
    QRegion r;
    QScopedPointer<QRegionData> x(new QRegionData);
    x->ref = 1;
    x->rgn = XCreateRegion();
    XUnionRegion(d->rgn, x->rgn, x->rgn);
    x->xrectangles = Q_NULLPTR;
    if (!r.d->ref.deref())
        cleanUp(r.d);
    r.d = x.take();
    return r;
}

bool QRegion::isEmpty() const
{
    return d == &shared_empty || XEmptyRegion(d->rgn);
}

bool QRegion::contains(const QPoint &p) const
{
    return XPointInRegion(d->rgn, p.x(), p.y());
}

bool QRegion::contains(const QRect &r) const
{
    return XRectInRegion(d->rgn, r.left(), r.top(), r.width(), r.height()) != RectangleOut;
}

void QRegion::translate(int dx, int dy)
{
    if ((dx == 0 && dy == 0) || isEmpty())
        return;

    detach();
    XOffsetRegion(d->rgn, dx, dy);
}

QRegion QRegion::unite(const QRegion &r) const
{
    if (d == r.d || r.isEmpty()  || XEqualRegion(d->rgn, r.d->rgn))
        return *this;

    QRegion result;
    result.detach();
    XUnionRegion(d->rgn, r.d->rgn, result.d->rgn);
    return result;
}

QRegion& QRegion::operator+=(const QRegion &r)
{
    if (d == r.d || r.isEmpty()  || XEqualRegion(d->rgn, r.d->rgn))
        return *this;

    detach();
    XUnionRegion(d->rgn, r.d->rgn, d->rgn);
    return *this;
}

QRegion QRegion::unite(const QRect &r) const
{
    if (r.isEmpty())
        return *this;

    QRegion result;
    result.detach();
    XRectangle xr = qt_QRectToXRectangle(r);
    XUnionRectWithRegion(&xr, d->rgn, result.d->rgn);
    return result;
}

QRegion& QRegion::operator+=(const QRect &r)
{
    if (r.isEmpty())
        return *this;

    detach();
    XRectangle xr = qt_QRectToXRectangle(r);
    XUnionRectWithRegion(&xr, d->rgn, d->rgn);
    return *this;
}

QRegion QRegion::intersect(const QRegion &r) const
{
    if (d == r.d || XEqualRegion(d->rgn, r.d->rgn))
        return *this;

    QRegion result;
    result.detach();
    XIntersectRegion(d->rgn, r.d->rgn, result.d->rgn);
    return result;
}

QRegion QRegion::intersect(const QRect &r) const
{
    return intersect(QRegion(r));
}

QRegion QRegion::subtract(const QRegion &r) const
{
    if (d == r.d || XEqualRegion(d->rgn, r.d->rgn))
        return QRegion();

    QRegion result;
    result.detach();
    XSubtractRegion(d->rgn, r.d->rgn, result.d->rgn);
    return result;
}

QRegion QRegion::eor(const QRegion &r) const
{
    if (d == r.d || XEqualRegion(d->rgn, r.d->rgn)) {
        return QRegion();
    }

    QRegion result;
    result.detach();
    XXorRegion(d->rgn, r.d->rgn, result.d->rgn);
    return result;
}

QRect QRegion::boundingRect() const
{
    if (isEmpty())
        return QRect();

    XRectangle xr;
    XClipBox(d->rgn, &xr);
    return QRect(xr.x, xr.y, xr.width, xr.height);
}

QVector<QRect> QRegion::rects() const
{
    QVector<QRect> result;
    if (isEmpty()) {
        return result;
    }

    result.append(boundingRect());
    return result;
}

void QRegion::setRects(const QRect *rects, int num)
{
    *this = QRegion();
    if (!rects || num < 1)
        return;

    detach();
    for (int i = 0; i < num; i++) {
        XRectangle xr = qt_QRectToXRectangle(rects[i]);
        XUnionRectWithRegion(&xr, d->rgn, d->rgn);
    }
}

int QRegion::rectCount() const
{
    if (isEmpty()) {
        return 0;
    }

    return 1;
}


bool QRegion::operator==(const QRegion &r) const
{
    if (d == r.d)
        return true;

    return XEqualRegion(d->rgn, r.d->rgn);
}

bool QRegion::intersects(const QRect &rect) const
{
    if (rect.isNull())
        return false;

    return rect_intersects(boundingRect(), rect.normalized());
}

void *QRegion::clipRectangles(int &num) const
{
    if (!d->xrectangles && !(d == &shared_empty || isEmpty())) {
        XRectangle *xr = static_cast<XRectangle*>(::malloc(1 * sizeof(XRectangle)));
        d->xrectangles = xr;
        XClipBox(d->rgn, xr);
    }
    if (d == &shared_empty || isEmpty())
        num = 0;
    else
        num = 1;
    return d->xrectangles;
}

QT_END_NAMESPACE
