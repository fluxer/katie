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

#include "qpainter.h"
#include "qpixmap.h"
#include "qpixmapfilter_p.h"
#include "qapplication_p.h"
#include "qpaintengineex_p.h"
#include "qpaintengine_raster_p.h"
#include "qmath.h"
#include "qmemrotate_p.h"
#include "qdrawhelper_p.h"
#include "qdebug.h"

#ifndef QT_NO_GRAPHICSEFFECT
QT_BEGIN_NAMESPACE

class QPixmapFilterPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QPixmapFilter)
public:
    QPixmapFilter::FilterType type;
};

/*!
    \class QPixmapFilter
    \since 4.5
    \ingroup painting

    \brief The QPixmapFilter class provides the basic functionality for
    pixmap filter classes. Pixmap filter can be for example colorize or blur.

    QPixmapFilter is the base class for every pixmap filter. QPixmapFilter is
    an abstract class and cannot itself be instantiated. It provides a standard
    interface for filter processing.

    \internal
*/

/*!
    \enum QPixmapFilter::FilterType

    \internal

    This enum describes the types of filter that can be applied to pixmaps.

    \value ColorizeFilter     A filter that is used to change the overall color
                              of an image. See QPixmapColorizeFilter for more
                              information.
    \value DropShadowFilter   A filter that is used to add a drop shadow to an
                              image. See QPixmapDropShadowFilter for more
                              information.
    \value BlurFilter         A filter that is used to blur an image using
                              a simple blur radius. See QPixmapBlurFilter
                              for more information.
*/


/*!
    Constructs a default QPixmapFilter with the given \a type.

    This constructor should be used when subclassing QPixmapFilter to
    create custom user filters.

    \internal
*/
QPixmapFilter::QPixmapFilter(FilterType type, QObject *parent)
    : QObject(*new QPixmapFilterPrivate, parent)
{
    d_func()->type = type;
}



/*!
   \internal
*/
QPixmapFilter::QPixmapFilter(QPixmapFilterPrivate&d, QPixmapFilter::FilterType type, QObject *parent)
    : QObject(d, parent)
{
    d_func()->type = type;
}


/*!
    Destroys the pixmap filter.

    \internal
*/
QPixmapFilter::~QPixmapFilter()
{
}

/*!
    Returns the type of the filter. All standard pixmap filter classes
    are associated with a unique value.

    \internal
*/
QPixmapFilter::FilterType QPixmapFilter::type() const
{
    Q_D(const QPixmapFilter);
    return d->type;
}

/*!
    Returns the bounding rectangle that is affected by the pixmap
    filter if the filter is applied to the specified \a rect.

    \internal
*/
QRectF QPixmapFilter::boundingRectFor(const QRectF &rect) const
{
    return rect;
}

/*!
    \fn void QPixmapFilter::draw(QPainter *painter, const QPointF &p, const QPixmap &src, const QRectF& srcRect) const

    Uses \a painter to draw filtered result of \a src at the point
    specified by \a p. If \a srcRect is specified the it will
    be used as a source rectangle to only draw a part of the source.

    draw() will affect the area which boundingRectFor() returns.

    \internal
*/

/*!
    \class QPixmapBlurFilter
    \since 4.6
    \ingroup multimedia

    \brief The QPixmapBlurFilter class provides blur filtering
    for pixmaps.

    QPixmapBlurFilter implements a blur pixmap filter,
    which is applied when \l{QPixmapFilter::}{draw()} is called.

    The filter lets you specialize the radius of the blur as well
    as hints as to whether to prefer performance or quality.

    By default, the blur effect is produced by applying an exponential
    filter generated from the specified blurRadius().  Paint engines
    may override this with a custom blur that is faster on the
    underlying hardware.

    \sa {Pixmap Filters Example}, QPixmapDropShadowFilter

    \internal
*/

class QPixmapBlurFilterPrivate : public QPixmapFilterPrivate
{
public:
    QPixmapBlurFilterPrivate() : radius(5), hints(QGraphicsBlurEffect::PerformanceHint) {}

    qreal radius;
    QGraphicsBlurEffect::BlurHints hints;
};


/*!
    Constructs a pixmap blur filter.

    \internal
*/
QPixmapBlurFilter::QPixmapBlurFilter(QObject *parent)
    : QPixmapFilter(*new QPixmapBlurFilterPrivate, BlurFilter, parent)
{
}

/*!
    Destructor of pixmap blur filter.

    \internal
*/
QPixmapBlurFilter::~QPixmapBlurFilter()
{
}

/*!
    Sets the radius of the blur filter. Higher radius produces increased blurriness.

    \internal
*/
void QPixmapBlurFilter::setRadius(qreal radius)
{
    Q_D(QPixmapBlurFilter);
    d->radius = radius;
}

/*!
    Gets the radius of the blur filter.

    \internal
*/
qreal QPixmapBlurFilter::radius() const
{
    Q_D(const QPixmapBlurFilter);
    return d->radius;
}

/*!
    Setting the blur hints to PerformanceHint causes the implementation
    to trade off visual quality to blur the image faster.  Setting the
    blur hints to QualityHint causes the implementation to improve
    visual quality at the expense of speed.

    AnimationHint causes the implementation to optimize for animating
    the blur radius, possibly by caching blurred versions of the source
    pixmap.

    The implementation is free to ignore this value if it only has a single
    blur algorithm.

    \internal
*/
void QPixmapBlurFilter::setBlurHints(QGraphicsBlurEffect::BlurHints hints)
{
    Q_D(QPixmapBlurFilter);
    d->hints = hints;
}

/*!
    Gets the blur hints of the blur filter.

    \internal
*/
QGraphicsBlurEffect::BlurHints QPixmapBlurFilter::blurHints() const
{
    Q_D(const QPixmapBlurFilter);
    return d->hints;
}

const qreal radiusScale = qreal(2.5);

/*!
    \internal
*/
QRectF QPixmapBlurFilter::boundingRectFor(const QRectF &rect) const
{
    Q_D(const QPixmapBlurFilter);
    const qreal delta = radiusScale * d->radius + 1;
    return rect.adjusted(-delta, -delta, delta, delta);
}

static inline int qt_static_shift(int shift, int value)
{
    if (shift == 0)
        return value;
    else if (shift > 0)
        return value << (uint(shift) & 0x1f);
    else
        return value >> (uint(-shift) & 0x1f);
}


static const int aprec = 12;
static const int zprec = 10;

static inline void qt_blurinner(uchar *bptr, int &zR, int &zG, int &zB, int &zA, int alpha)
{
    QRgb *pixel = (QRgb *)bptr;

#define Z_MASK (0xff << zprec)
    const int A_zprec = qt_static_shift(zprec - 24, *pixel) & Z_MASK;
    const int R_zprec = qt_static_shift(zprec - 16, *pixel) & Z_MASK;
    const int G_zprec = qt_static_shift(zprec - 8, *pixel)  & Z_MASK;
    const int B_zprec = qt_static_shift(zprec, *pixel)      & Z_MASK;
#undef Z_MASK

    const int zR_zprec = zR >> aprec;
    const int zG_zprec = zG >> aprec;
    const int zB_zprec = zB >> aprec;
    const int zA_zprec = zA >> aprec;

    zR += alpha * (R_zprec - zR_zprec);
    zG += alpha * (G_zprec - zG_zprec);
    zB += alpha * (B_zprec - zB_zprec);
    zA += alpha * (A_zprec - zA_zprec);

#define ZA_MASK (0xff << (zprec + aprec))
    *pixel =
        qt_static_shift(24 - zprec - aprec, zA & ZA_MASK)
        | qt_static_shift(16 - zprec - aprec, zR & ZA_MASK)
        | qt_static_shift(8 - zprec - aprec, zG & ZA_MASK)
        | qt_static_shift(-zprec - aprec, zB & ZA_MASK);
#undef ZA_MASK
}

static inline void qt_blurinner_alphaOnly(uchar *bptr, int &z, int alpha)
{
    const int A_zprec = int(*(bptr)) << zprec;
    const int z_zprec = z >> aprec;
    z += alpha * (A_zprec - z_zprec);
    *(bptr) = z >> (zprec + aprec);
}

static inline void qt_blurrow(QImage & im, int line, int alpha, bool alphaOnly)
{
    uchar *bptr = im.scanLine(line);

    int zR = 0, zG = 0, zB = 0, zA = 0;

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    if (alphaOnly && im.format() != QImage::Format_Indexed8)
        bptr += 3;
#endif

    const int stride = im.depth() >> 3;
    const int im_width = im.width();
    for (int index = 0; index < im_width; ++index) {
        if (alphaOnly)
            qt_blurinner_alphaOnly(bptr, zA, alpha);
        else
            qt_blurinner(bptr, zR, zG, zB, zA, alpha);
        bptr += stride;
    }

    bptr -= stride;

    for (int index = im_width - 2; index >= 0; --index) {
        bptr -= stride;
        if (alphaOnly)
            qt_blurinner_alphaOnly(bptr, zA, alpha);
        else
            qt_blurinner(bptr, zR, zG, zB, zA, alpha);
    }
}

/*
*  expblur(QImage &img, int radius)
*
*  Based on exponential blur algorithm by Jani Huhtanen
*
*  In-place blur of image 'img' with kernel
*  of approximate radius 'radius'.
*
*  Blurs with two sided exponential impulse
*  response.
*
*  aprec = precision of alpha parameter
*  in fixed-point format 0.aprec
*
*  zprec = precision of state parameters
*  zR,zG,zB and zA in fp format 8.zprec
*/
static void expblur(QImage &img, qreal radius, bool improvedQuality, bool alphaOnly)
{
    // halve the radius if we're using two passes
    if (improvedQuality)
        radius *= qreal(0.5);

    Q_ASSERT(img.format() == QImage::Format_ARGB32_Premultiplied
             || img.format() == QImage::Format_RGB32);

    // choose the alpha such that pixels at radius distance from a fully
    // saturated pixel will have an alpha component of no greater than
    // the cutOffIntensity
    const qreal cutOffIntensity = 2;
    int alpha = radius <= qreal(1e-5)
        ? ((1 << aprec)-1)
        : qRound((1<<aprec)*(1 - qPow(cutOffIntensity * (1 / qreal(255)), 1 / radius)));

    int img_height = img.height();
    for (int row = 0; row < img_height; ++row) {
        for (int i = 0; i <= int(improvedQuality); ++i)
            qt_blurrow(img, row, alpha, alphaOnly);
    }

    QImage temp(img.height(), img.width(), img.format());
    qt_memrotate270(reinterpret_cast<const quint32*>(img.constBits()),
                    img.width(), img.height(), img.bytesPerLine(),
                    reinterpret_cast<quint32*>(temp.bits()),
                    temp.bytesPerLine());

    img_height = temp.height();
    for (int row = 0; row < img_height; ++row) {
        for (int i = 0; i <= int(improvedQuality); ++i)
            qt_blurrow(temp, row, alpha, alphaOnly);
    }

    qt_memrotate90(reinterpret_cast<const quint32*>(temp.constBits()),
                   temp.width(), temp.height(), temp.bytesPerLine(),
                   reinterpret_cast<quint32*>(img.bits()),
                   img.bytesPerLine());
}
#define AVG(a,b)  ( ((((a)^(b)) & 0xfefefefeUL) >> 1) + ((a)&(b)) )
#define AVG16(a,b)  ( ((((a)^(b)) & 0xf7deUL) >> 1) + ((a)&(b)) )

static QImage qt_halfScaled(const QImage &source)
{
    if (source.width() < 2 || source.height() < 2)
        return QImage();

    QImage srcImage = source;

    if (source.format() == QImage::Format_Indexed8) {
        // assumes grayscale
        QImage dest(source.width() / 2, source.height() / 2, srcImage.format());

        const uchar *src = srcImage.constBits();
        int sx = srcImage.bytesPerLine();
        int sx2 = sx << 1;

        uchar *dst = dest.bits();
        int dx = dest.bytesPerLine();
        int ww = dest.width();
        int hh = dest.height();

        for (int y = hh; y; --y, dst += dx, src += sx2) {
            const uchar *p1 = src;
            const uchar *p2 = src + sx;
            uchar *q = dst;
            for (int x = ww; x; --x, ++q, p1 += 2, p2 += 2)
                *q = ((int(p1[0]) + int(p1[1]) + int(p2[0]) + int(p2[1])) + 2) >> 2;
        }

        return dest;
    } else if (source.format() != QImage::Format_ARGB32_Premultiplied
               && source.format() != QImage::Format_RGB32) {
        srcImage = source.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    QImage dest(source.width() / 2, source.height() / 2, srcImage.format());

    const quint32 *src = reinterpret_cast<const quint32*>(srcImage.constBits());
    int sx = srcImage.bytesPerLine() >> 2;
    int sx2 = sx << 1;

    quint32 *dst = reinterpret_cast<quint32*>(dest.bits());
    int dx = dest.bytesPerLine() >> 2;
    int ww = dest.width();
    int hh = dest.height();

    for (int y = hh; y; --y, dst += dx, src += sx2) {
        const quint32 *p1 = src;
        const quint32 *p2 = src + sx;
        quint32 *q = dst;
        for (int x = ww; x; --x, q++, p1 += 2, p2 += 2)
            *q = AVG(AVG(p1[0], p1[1]), AVG(p2[0], p2[1]));
    }

    return dest;
}

static void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly)
{
    if (blurImage.format() != QImage::Format_ARGB32_Premultiplied
        && blurImage.format() != QImage::Format_RGB32)
    {
        blurImage = blurImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    qreal scale = 1;
    if (radius >= 4 && blurImage.width() >= 2 && blurImage.height() >= 2) {
        blurImage = qt_halfScaled(blurImage);
        scale = 2;
        radius *= qreal(0.5);
    }

    expblur(blurImage, radius, quality, alphaOnly);

    Q_ASSERT(p);
    p->scale(scale, scale);
    p->drawImage(QRect(0, 0, blurImage.width(), blurImage.height()), blurImage);
}

Q_GUI_EXPORT bool qt_scaleForTransform(const QTransform &transform, qreal *scale);

/*!
    \internal
*/
void QPixmapBlurFilter::draw(QPainter *painter, const QPointF &p, const QPixmap &src, const QRectF &rect) const
{
    Q_D(const QPixmapBlurFilter);
    if (!painter->isActive())
        return;

    if (src.isNull())
        return;

    QRectF srcRect = rect;
    if (srcRect.isNull())
        srcRect = src.rect();

    if (d->radius <= 1) {
        painter->drawPixmap(srcRect.translated(p), src, srcRect);
        return;
    }

    qreal scaledRadius = radiusScale * d->radius;
    qreal scale;
    if (qt_scaleForTransform(painter->transform(), &scale))
        scaledRadius /= scale;

    QImage srcImage;
    if (srcRect == src.rect()) {
        srcImage = src.toImage();
    } else {
        QRect rect = srcRect.toAlignedRect().intersected(src.rect());
        srcImage = src.copy(rect).toImage();
    }

    QTransform transform = painter->worldTransform();
    painter->translate(p);
    qt_blurImage(painter, srcImage, scaledRadius, (d->hints & QGraphicsBlurEffect::QualityHint), false);
    painter->setWorldTransform(transform);
}

// grayscales the image to dest (could be same). If rect isn't defined
// destination image size is used to determine the dimension of grayscaling
// process.
Q_GUI_EXPORT void qt_grayscale(const QImage &image, QImage &dest)
{
    QRect srcRect =  image.rect();
    if (srcRect.isNull()) {
        srcRect = dest.rect();
    }
    QRect destRect = srcRect;
    if (&image != &dest) {
        destRect.moveTo(QPoint(0, 0));
    }

    unsigned int *outData = (unsigned int *)dest.bits();

    if (dest.size() == image.size() && image.rect() == srcRect) {
        const unsigned int *data = (const unsigned int *)image.constBits();
        // a bit faster loop for grayscaling everything
        int pixels = dest.width() * dest.height();
        for (int i = 0; i < pixels; ++i) {
            int val = qGray(data[i]);
            outData[i] = qRgba(val, val, val, qAlpha(data[i]));
        }
    } else {
        int yd = destRect.top();
        for (int y = srcRect.top(); y <= srcRect.bottom() && y < image.height(); y++) {
            const unsigned int *data = (const unsigned int*)image.constScanLine(y);
            outData = (unsigned int*)dest.scanLine(yd++);
            int xd = destRect.left();
            for (int x = srcRect.left(); x <= srcRect.right() && x < image.width(); x++) {
                int val = qGray(data[x]);
                outData[xd++] = qRgba(val, val, val, qAlpha(data[x]));
            }
        }
    }
}

/*!
    \class QPixmapColorizeFilter
    \since 4.5
    \ingroup painting

    \brief The QPixmapColorizeFilter class provides colorizing
    filtering for pixmaps.

    A colorize filter gives the pixmap a tint of its color(). The
    filter first grayscales the pixmap and then converts those to
    colorized values using QPainter::CompositionMode_Screen with the
    chosen color. The alpha-channel is not changed.

    Example:
    \snippet doc/src/snippets/code/src_gui_image_qpixmapfilter.cpp 0

    \sa QPainter::CompositionMode

    \internal
*/
class QPixmapColorizeFilterPrivate : public QPixmapFilterPrivate
{
    Q_DECLARE_PUBLIC(QPixmapColorizeFilter)
public:
    QColor color;
    qreal strength;
    bool opaque;
    bool alphaBlend;
};

/*!
    Constructs an pixmap colorize filter.

    Default color value for colorizing is QColor(0, 0, 192).

    \internal
*/
QPixmapColorizeFilter::QPixmapColorizeFilter(QObject *parent)
    : QPixmapFilter(*new QPixmapColorizeFilterPrivate, ColorizeFilter, parent)
{
    Q_D(QPixmapColorizeFilter);
    d->color = QColor(0, 0, 192);
    d->strength = qreal(1);
    d->opaque = true;
    d->alphaBlend = false;
}

/*!
    Gets the color of the colorize filter.

    \internal
*/
QColor QPixmapColorizeFilter::color() const
{
    Q_D(const QPixmapColorizeFilter);
    return d->color;
}

/*!
    Sets the color of the colorize filter to the \a color specified.

    \internal
*/
void QPixmapColorizeFilter::setColor(const QColor &color)
{
    Q_D(QPixmapColorizeFilter);
    d->color = color;
}

/*!
    Gets the strength of the colorize filter, 1.0 means full colorized while
    0.0 equals to no filtering at all.

    \internal
*/
qreal QPixmapColorizeFilter::strength() const
{
    Q_D(const QPixmapColorizeFilter);
    return d->strength;
}

/*!
    Sets the strength of the colorize filter to \a strength.

    \internal
*/
void QPixmapColorizeFilter::setStrength(qreal strength)
{
    Q_D(QPixmapColorizeFilter);
    d->strength = qBound(qreal(0), strength, qreal(1));
    d->opaque = !qFuzzyIsNull(d->strength);
    d->alphaBlend = !qFuzzyIsNull(d->strength - 1);
}

/*!
    \internal
*/
void QPixmapColorizeFilter::draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect) const
{
    Q_D(const QPixmapColorizeFilter);

    if (src.isNull())
        return;

    // raster implementation
    if (!d->opaque) {
        painter->drawPixmap(dest, src, srcRect);
        return;
    }

    QImage srcImage;
    QImage destImage;

    if (srcRect.isNull()) {
        srcImage = src.toImage();
        srcImage = srcImage.convertToFormat(srcImage.hasAlphaChannel() ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32);
        destImage = QImage(srcImage.size(), srcImage.format());
    } else {
        QRect rect = srcRect.toAlignedRect().intersected(src.rect());

        srcImage = src.copy(rect).toImage();
        srcImage = srcImage.convertToFormat(srcImage.hasAlphaChannel() ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32);
        destImage = QImage(rect.size(), srcImage.format());
    }

    // do colorizing
    QPainter destPainter(&destImage);
    qt_grayscale(srcImage, destImage);
    destPainter.setCompositionMode(QPainter::CompositionMode_Screen);
    destPainter.fillRect(srcImage.rect(), d->color);
    destPainter.end();

    if (d->alphaBlend) {
        // alpha blending srcImage and destImage
        QImage buffer = srcImage;
        QPainter bufPainter(&buffer);
        bufPainter.setOpacity(d->strength);
        bufPainter.drawImage(0, 0, destImage);
        bufPainter.end();
        destImage = buffer;
    }

    if (srcImage.hasAlphaChannel())
        destImage.setAlphaChannel(srcImage.alphaChannel());

    painter->drawImage(dest, destImage);
}

class QPixmapDropShadowFilterPrivate : public QPixmapFilterPrivate
{
public:
    QPixmapDropShadowFilterPrivate()
        : offset(8, 8), color(63, 63, 63, 180), radius(1) {}

    QPointF offset;
    QColor color;
    qreal radius;
};

/*!
    \class QPixmapDropShadowFilter
    \since 4.5
    \ingroup painting

    \brief The QPixmapDropShadowFilter class is a convenience class
    for drawing pixmaps with drop shadows.

    The drop shadow is produced by taking a copy of the source pixmap
    and applying a color to the copy using a
    QPainter::CompositionMode_DestinationIn operation. The original
    pixmap is drawn on top.

    The QPixmapDropShadowFilter class provides some customization
    options to specify how the drop shadow should appear. The color of
    the drop shadow can be modified using the setColor() function, the
    drop shadow offset can be modified using the setOffset() function,
    and the blur radius of the drop shadow can be changed through the
    setBlurRadius() function.

    By default, the drop shadow is a dark gray shadow, blurred with a
    radius of 1 at an offset of 8 pixels towards the lower right.

    Example:
    \snippet doc/src/snippets/code/src_gui_image_qpixmapfilter.cpp 2

    \sa QPixmapColorizeFilter

    \internal
 */

/*!
    Constructs drop shadow filter.

    \internal
*/
QPixmapDropShadowFilter::QPixmapDropShadowFilter(QObject *parent)
    : QPixmapFilter(*new QPixmapDropShadowFilterPrivate, DropShadowFilter, parent)
{
}

/*!
    Destroys drop shadow filter.

    \internal
*/
QPixmapDropShadowFilter::~QPixmapDropShadowFilter()
{
}

/*!
    Returns the radius in pixels of the blur on the drop shadow.

    A smaller radius results in a sharper shadow.

    \sa color(), offset()

    \internal
*/
qreal QPixmapDropShadowFilter::blurRadius() const
{
    Q_D(const QPixmapDropShadowFilter);
    return d->radius;
}

/*!
    Sets the radius in pixels of the blur on the drop shadow to the \a radius specified.

    Using a smaller radius results in a sharper shadow.

    \sa setColor(), setOffset()

    \internal
*/
void QPixmapDropShadowFilter::setBlurRadius(qreal radius)
{
    Q_D(QPixmapDropShadowFilter);
    d->radius = radius;
}

/*!
    Returns the color of the drop shadow.

    \sa blurRadius(), offset()

    \internal
*/
QColor QPixmapDropShadowFilter::color() const
{
    Q_D(const QPixmapDropShadowFilter);
    return d->color;
}

/*!
    Sets the color of the drop shadow to the \a color specified.

    \sa setBlurRadius(), setOffset()

    \internal
*/
void QPixmapDropShadowFilter::setColor(const QColor &color)
{
    Q_D(QPixmapDropShadowFilter);
    d->color = color;
}

/*!
    Returns the shadow offset in pixels.

    \sa blurRadius(), color()

    \internal
*/
QPointF QPixmapDropShadowFilter::offset() const
{
    Q_D(const QPixmapDropShadowFilter);
    return d->offset;
}

/*!
    Sets the shadow offset in pixels to the \a offset specified.

    \sa setBlurRadius(), setColor()

    \internal
*/
void QPixmapDropShadowFilter::setOffset(const QPointF &offset)
{
    Q_D(QPixmapDropShadowFilter);
    d->offset = offset;
}

/*!
    \fn void QPixmapDropShadowFilter::setOffset(qreal dx, qreal dy)
    \overload

    Sets the shadow offset in pixels to be the displacement specified by the
    horizontal \a dx and vertical \a dy coordinates.

    \sa setBlurRadius(), setColor()

    \internal
*/

/*!
    \internal
 */
QRectF QPixmapDropShadowFilter::boundingRectFor(const QRectF &rect) const
{
    Q_D(const QPixmapDropShadowFilter);
    return rect.united(rect.translated(d->offset).adjusted(-d->radius, -d->radius, d->radius, d->radius));
}

/*!
    \internal
 */
void QPixmapDropShadowFilter::draw(QPainter *p,
                                   const QPointF &pos,
                                   const QPixmap &px,
                                   const QRectF &src) const
{
    Q_D(const QPixmapDropShadowFilter);

    if (px.isNull())
        return;

    QImage tmp(px.size(), QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    QPainter tmpPainter(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(d->offset, px);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, d->radius, false, true);
    blurPainter.end();

    // blacken the image...
    QPainter blackenPainter(&blurred);
    blackenPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    blackenPainter.fillRect(blurred.rect(), d->color);
    blackenPainter.end();

    // draw the blurred drop shadow...
    p->drawImage(pos, blurred);

    // Draw the actual pixmap...
    p->drawPixmap(pos, px, src);
}

QT_END_NAMESPACE

#include "moc_qpixmapfilter_p.h"

#endif //QT_NO_GRAPHICSEFFECT
