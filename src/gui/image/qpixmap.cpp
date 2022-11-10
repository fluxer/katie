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

#include "qpixmap.h"
#include "qpixmapdata_p.h"
#include "qbitmap.h"
#include "qimage.h"
#include "qwidget.h"
#include "qpainter.h"
#include "qdatastream.h"
#include "qapplication.h"
#include "qwidget_p.h"
#include "qevent.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qpixmapcache.h"
#include "qdatetime.h"
#include "qimagereader.h"
#include "qimagewriter.h"
#include "qpaintengine.h"
#include "qstylehelper_p.h"
#include "qguicommon_p.h"

#if defined(Q_WS_X11)
# include "qx11info_x11.h"
# include "qt_x11_p.h"
#endif

QT_BEGIN_NAMESPACE

void QPixmap::init(int w, int h, int type)
{
    if ((w > 0 && h > 0) || type == QPixmapData::BitmapType)
        data = new QPixmapData(w, h, static_cast<QPixmapData::PixelType>(type));
    else
        data = 0;
}

/*!
    Constructs a null pixmap.

    \sa isNull()
*/

QPixmap::QPixmap()
    : QPaintDevice()
{
    init(0, 0, QPixmapData::PixmapType);
}

/*!
    Constructs a pixmap with the given \a width and \a height. If
    either \a width or \a height is zero, a null pixmap is
    constructed.

    \warning This will create a QPixmap with uninitialized data. Call
    fill() to fill the pixmap with an appropriate color before drawing
    onto it with QPainter.

    \sa isNull()
*/

QPixmap::QPixmap(int w, int h)
    : QPaintDevice()
{
    init(w, h, QPixmapData::PixmapType);
}

/*!
    \overload

    Constructs a pixmap of the given \a size.

    \warning This will create a QPixmap with uninitialized data. Call
    fill() to fill the pixmap with an appropriate color before drawing
    onto it with QPainter.
*/

QPixmap::QPixmap(const QSize &size)
    : QPaintDevice()
{
    init(size.width(), size.height(), QPixmapData::PixmapType);
}

/*!
  \internal
*/
QPixmap::QPixmap(const QSize &s, int type)
{
    init(s.width(), s.height(), static_cast<QPixmapData::PixelType>(type));
}

/*!
    \internal
*/
QPixmap::QPixmap(QPixmapData *d)
    : QPaintDevice(), data(d)
{
}

/*!
    Constructs a pixmap from the file with the given \a fileName. If the
    file does not exist or is of an unknown format, the pixmap becomes a
    null pixmap.

    The loader attempts to read the pixmap using the specified \a
    format. If the \a format is not specified (which is the default),
    the loader probes the file for a header to guess the file format.

    The file name can either refer to an actual file on disk or to
    one of the application's embedded resources. See the
    \l{resources.html}{Resource System} overview for details on how
    to embed images and other resource files in the application's
    executable.

    If the image needs to be modified to fit in a lower-resolution
    result (e.g. converting from 32-bit to monochrome), use the \a
    flags to control the conversion.

    The \a fileName, \a format and \a flags parameters are
    passed on to load(). This means that the data in \a fileName is
    not compiled into the binary. If \a fileName contains a relative
    path (e.g. the filename only) the relevant file must be found
    relative to the runtime working directory.

    \sa {QPixmap#Reading and Writing Image Files}{Reading and Writing
    Image Files}
*/

QPixmap::QPixmap(const QString& fileName, const char *format, Qt::ImageConversionFlags flags)
    : QPaintDevice()
{
    init(0, 0, QPixmapData::PixmapType);
    load(fileName, format, flags);
}

/*!
    Constructs a pixmap that is a copy of the given \a pixmap.

    \sa copy()
*/

QPixmap::QPixmap(const QPixmap &pixmap)
    : QPaintDevice()
{
    if (pixmap.paintingActive()) {                // make a deep copy
        operator=(pixmap.copy());
    } else {
        data = pixmap.data;
    }
}

/*!
    Destroys the pixmap.
*/

QPixmap::~QPixmap()
{
    Q_ASSERT(!data || data->ref >= 1); // Catch if ref-counting changes again
}

/*!
  \internal
*/
int QPixmap::devType() const
{
    return QInternal::Pixmap;
}

/*!
    \fn QPixmap QPixmap::copy(int x, int y, int width, int height) const
    \overload

    Returns a deep copy of the subset of the pixmap that is specified
    by the rectangle QRect( \a x, \a y, \a width, \a height).
*/

/*!
    Returns a deep copy of the subset of the pixmap that is specified
    by the given \a rect. For more information on deep copies,
    see the \l {Implicit Data Sharing} documentation.

    If the given \a rect is empty, the whole image is copied.

    \sa operator=(), QPixmap(), {QPixmap#Pixmap
    Transformations}{Pixmap Transformations}
*/
QPixmap QPixmap::copy(const QRect &rect) const
{
    if (isNull())
        return QPixmap();

    QRect r(0, 0, width(), height());
    if (!rect.isEmpty())
        r = r.intersected(rect);

    QPixmapData *d = data->createCompatiblePixmapData();
    d->copy(data.data(), r);
    return QPixmap(d);
}

/*!
    \fn QPixmap::scroll(int dx, int dy, int x, int y, int width, int height, QRegion *exposed)
    \since 4.6

    This convenience function is equivalent to calling QPixmap::scroll(\a dx,
    \a dy, QRect(\a x, \a y, \a width, \a height), \a exposed).

    \sa QWidget::scroll(), QGraphicsItem::scroll()
*/

/*!
    \since 4.6

    Scrolls the area \a rect of this pixmap by (\a dx, \a dy). The exposed
    region is left unchanged. You can optionally pass a pointer to an empty
    QRegion to get the region that is \a exposed by the scroll operation.

    \snippet doc/src/snippets/code/src_gui_image_qpixmap.cpp 2

    You cannot scroll while there is an active painter on the pixmap.

    \sa QWidget::scroll(), QGraphicsItem::scroll()
*/
void QPixmap::scroll(int dx, int dy, const QRect &rect, QRegion *exposed)
{
    if (isNull() || (dx == 0 && dy == 0))
        return;
    QRect dest = rect & this->rect();
    QRect src = dest.translated(-dx, -dy) & dest;
    if (src.isEmpty()) {
        if (exposed)
            *exposed += dest;
        return;
    }

    detach();

    data->scroll(dx, dy, src);

    if (exposed) {
        *exposed += dest;
        *exposed -= src.translated(dx, dy);
    }
}

/*!
    Assigns the given \a pixmap to this pixmap and returns a reference
    to this pixmap.

    \sa copy(), QPixmap()
*/

QPixmap &QPixmap::operator=(const QPixmap &pixmap)
{
    if (Q_UNLIKELY(paintingActive())) {
        qWarning("QPixmap::operator=: Cannot assign to pixmap during painting");
        return *this;
    }
    if (pixmap.paintingActive()) {                // make a deep copy
        *this = pixmap.copy();
    } else {
        data = pixmap.data;
    }
    return *this;
}

/*!
    \fn void QPixmap::swap(QPixmap &other)
    \since 4.8

    Swaps pixmap \a other with this pixmap. This operation is very
    fast and never fails.
*/

/*!
   Returns the pixmap as a QVariant.
*/
QPixmap::operator QVariant() const
{
    return QVariant(QVariant::Pixmap, this);
}

/*!
    \fn bool QPixmap::operator!() const

    Returns true if this is a null pixmap; otherwise returns false.

    \sa isNull()
*/

/*!
    \fn QPixmap::operator QImage() const

    Returns the pixmap as a QImage.

    Use the toImage() function instead.
*/

/*!
    Converts the pixmap to a QImage. Returns a null image if the
    conversion fails.

    If the pixmap has 1-bit depth, the returned image will also be 1
    bit deep. Images with more bits will be returned in a format
    closely represents the underlying system. Usually this will be
    QImage::Format_ARGB32_Premultiplied for pixmaps with an alpha and
    QImage::Format_RGB32 or QImage::Format_RGB16 for pixmaps without
    alpha.

    Note that for the moment, alpha masks on monochrome images are
    ignored.

    \sa fromImage(), {QImage#Image Formats}{Image Formats}
*/
QImage QPixmap::toImage() const
{
    if (isNull())
        return QImage();

    return data->toImage();
}

/*!
    \fn QMatrix QPixmap::trueMatrix(const QTransform &matrix, int width, int height)

    Returns the actual matrix used for transforming a pixmap with the
    given \a width, \a height and \a matrix.

    When transforming a pixmap using the transformed() function, the
    transformation matrix is internally adjusted to compensate for
    unwanted translation, i.e. transformed() returns the smallest
    pixmap containing all transformed points of the original
    pixmap. This function returns the modified matrix, which maps
    points correctly from the original pixmap into the new pixmap.

    \sa transformed(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}
*/
QTransform QPixmap::trueMatrix(const QTransform &m, int w, int h)
{
    return QImage::trueMatrix(m, w, h);
}

/*!
  \overload

  This convenience function loads the matrix \a m into a
  QTransform and calls the overloaded function with the
  QTransform and the width \a w and the height \a h.
 */
QMatrix QPixmap::trueMatrix(const QMatrix &m, int w, int h)
{
    return trueMatrix(QTransform(m), w, h).toAffine();
}


/*!
    Returns true if this is a QBitmap; otherwise returns false.
*/
bool QPixmap::isQBitmap() const
{
    return data->type == QPixmapData::BitmapType;
}

/*!
    Returns true if this is a null pixmap; otherwise returns false.

    A null pixmap has zero width, zero height and no contents. You
    cannot draw in a null pixmap.
*/
bool QPixmap::isNull() const
{
    return !data || data->isNull();
}

/*!
    Returns the width of the pixmap.

    \sa size(), {QPixmap#Pixmap Information}{Pixmap Information}
*/
int QPixmap::width() const
{
    return data ? data->width() : 0;
}

/*!
    Returns the height of the pixmap.

    \sa size(), {QPixmap#Pixmap Information}{Pixmap Information}
*/
int QPixmap::height() const
{
    return data ? data->height() : 0;
}

/*!
    Returns the size of the pixmap.

    \sa width(), height(), {QPixmap#Pixmap Information}{Pixmap
    Information}
*/
QSize QPixmap::size() const
{
    return data ? QSize(data->width(), data->height()) : QSize(0, 0);
}

/*!
    Returns the pixmap's enclosing rectangle.

    \sa {QPixmap#Pixmap Information}{Pixmap Information}
*/
QRect QPixmap::rect() const
{
    return data ? QRect(0, 0, data->width(), data->height()) : QRect();
}

/*!
    Returns the depth of the pixmap.

    The pixmap depth is also called bits per pixel (bpp) or bit planes
    of a pixmap. A null pixmap has depth 0.

    \sa defaultDepth(), {QPixmap#Pixmap Information}{Pixmap
    Information}
*/
int QPixmap::depth() const
{
    return data ? data->depth() : 0;
}

/*!
    Sets a mask bitmap.

    This function merges the \a mask with the pixmap's alpha channel. A pixel
    value of 1 on the mask means the pixmap's pixel is unchanged; a value of 0
    means the pixel is transparent. The mask must have the same size as this
    pixmap.

    Setting a null mask resets the mask, leaving the previously transparent
    pixels black. The effect of this function is undefined when the pixmap is
    being painted on.

    \warning This is potentially an expensive operation.

    \sa mask(), {QPixmap#Pixmap Transformations}{Pixmap Transformations},
    QBitmap
*/
void QPixmap::setMask(const QBitmap &mask)
{
    if (Q_UNLIKELY(paintingActive())) {
        qWarning("QPixmap::setMask: Cannot set mask while pixmap is being painted on");
        return;
    }

    if (Q_UNLIKELY(!mask.isNull() && mask.size() != size())) {
        qWarning("QPixmap::setMask() mask size differs from pixmap size");
        return;
    }

    if (isNull())
        return;

    if (static_cast<const QPixmap &>(mask).data == data) // trying to selfmask
       return;

    detach();
    data->setMask(mask);
}

#ifndef QT_NO_IMAGE_HEURISTIC_MASK
/*!
    Creates and returns a heuristic mask for this pixmap.

    The function works by selecting a color from one of the corners
    and then chipping away pixels of that color, starting at all the
    edges.  If \a clipTight is true (the default) the mask is just
    large enough to cover the pixels; otherwise, the mask is larger
    than the data pixels.

    The mask may not be perfect but it should be reasonable, so you
    can do things such as the following:

    \snippet doc/src/snippets/code/src_gui_image_qpixmap.cpp 1

    This function is slow because it involves converting to/from a
    QImage, and non-trivial computations.

    \sa QImage::createHeuristicMask(), createMaskFromColor()
*/
QBitmap QPixmap::createHeuristicMask(bool clipTight) const
{
    return QBitmap::fromImage(toImage().createHeuristicMask(clipTight));
}
#endif

/*!
    Creates and returns a mask for this pixmap based on the given \a
    maskColor. If the \a mode is Qt::MaskInColor, all pixels matching the
    maskColor will be transparent. If \a mode is Qt::MaskOutColor, all pixels
    matching the maskColor will be opaque.

    This function is slow because it involves converting to/from a
    QImage.

    \sa createHeuristicMask(), QImage::createMaskFromColor()
*/
QBitmap QPixmap::createMaskFromColor(const QColor &maskColor, Qt::MaskMode mode) const
{
    QImage image = toImage().convertToFormat(QImage::Format_ARGB32);
    return QBitmap::fromImage(image.createMaskFromColor(maskColor.rgba(), mode));
}

/*!
    Loads a pixmap from the file with the given \a fileName. Returns
    true if the pixmap was successfully loaded; otherwise returns
    false.

    The loader attempts to read the pixmap using the specified \a
    format. If the \a format is not specified (which is the default),
    the loader probes the file for a header to guess the file format.

    The file name can either refer to an actual file on disk or to one
    of the application's embedded resources. See the
    \l{resources.html}{Resource System} overview for details on how to
    embed pixmaps and other resource files in the application's
    executable.

    If the data needs to be modified to fit in a lower-resolution
    result (e.g. converting from 32-bit to monochrome), use the
    \a flags to control the conversion.

    Note that QPixmaps are automatically added to the QPixmapCache
    when loaded from a file; the key used is internal and can not
    be acquired.

    \sa loadFromData(), {QPixmap#Reading and Writing Image
    Files}{Reading and Writing Image Files}
*/
bool QPixmap::load(const QString &fileName, const char *format, Qt::ImageConversionFlags flags)
{
    if (fileName.isEmpty())
        return false;

    QFileInfo info(fileName);
    QString key = QLatin1String("qt_pixmap")
                  + info.absoluteFilePath()
                  + HexString<uint>(info.lastModified().toTime_t())
                  + HexString<quint64>(info.size())
                  + HexString<uint>(data ? data->pixelType() : QPixmapData::PixmapType);

    // Note: If no extension is provided, we try to match the
    // file against known plugin extensions
    if (!info.completeSuffix().isEmpty() && !info.exists())
        return false;

    if (QPixmapCache::find(key, *this))
        return true;

    QScopedPointer<QPixmapData> tmp(new QPixmapData(data ? data->type : QPixmapData::PixmapType));
    if (tmp->fromFile(fileName, format, flags)) {
        data = tmp.take();
        QPixmapCache::insert(key, *this);
        return true;
    }

    return false;
}

/*!
    Loads a pixmap from the \a len first bytes of the given binary \a
    data.  Returns true if the pixmap was loaded successfully;
    otherwise returns false.

    The loader attempts to read the pixmap using the specified \a
    format. If the \a format is not specified (which is the default),
    the loader probes the file for a header to guess the file format.

    If the data needs to be modified to fit in a lower-resolution
    result (e.g. converting from 32-bit to monochrome), use the
    \a flags to control the conversion.

    \sa load(), {QPixmap#Reading and Writing Image Files}{Reading and
    Writing Image Files}
*/
bool QPixmap::loadFromData(const uchar *buf, uint len, const char *format, Qt::ImageConversionFlags flags)
{
    if (len == 0 || buf == 0)
        return false;

    if (!data)
        data = new QPixmapData(QPixmapData::PixmapType);

    return data->fromData(buf, len, format, flags);
}

/*!
    \fn bool QPixmap::loadFromData(const QByteArray &data, const char *format, Qt::ImageConversionFlags flags)

    \overload

    Loads a pixmap from the binary \a data using the specified \a
    format and conversion \a flags.
*/

/*!
    Saves the pixmap to the file with the given \a fileName using the
    specified image file \a format and \a quality factor. Returns true
    if successful; otherwise returns false.

    The \a quality factor must be in the range [0,100] or -1. Specify
    0 to obtain small compressed files, 100 for large uncompressed
    files, and -1 to use the default settings.

    If \a format is 0, an image format will be chosen from \a fileName's
    suffix.

    \sa {QPixmap#Reading and Writing Image Files}{Reading and Writing
    Image Files}
*/
bool QPixmap::save(const QString &fileName, const char *format, int quality) const
{
    if (isNull())
        return false;                                // nothing to save
    QImageWriter writer(fileName, format);
    return doImageIO(&writer, quality);
}

/*!
    \overload

    This function writes a QPixmap to the given \a device using the
    specified image file \a format and \a quality factor. This can be
    used, for example, to save a pixmap directly into a QByteArray:

    \snippet doc/src/snippets/image/image.cpp 1
*/
bool QPixmap::save(QIODevice* device, const char* format, int quality) const
{
    if (isNull())
        return false;                                // nothing to save
    QImageWriter writer(device, format);
    return doImageIO(&writer, quality);
}

/*! \internal
*/
bool QPixmap::doImageIO(QImageWriter *writer, int quality) const
{
    if (Q_UNLIKELY(quality > 100 || quality < -1))
        qWarning("QPixmap::save: quality out of range [-1,100]");
    if (quality >= 0)
        writer->setQuality(qMin(quality,100));
    return writer->write(toImage());
}


// The implementation (and documentation) of
// QPixmap::fill(const QWidget *, const QPoint &)
// is in qwidget.cpp

/*!
    \fn void QPixmap::fill(const QWidget *widget, int x, int y)
    \overload

    Fills the pixmap with the \a widget's background color or pixmap.
    The given point, (\a x, \a y), defines an offset in widget
    coordinates to which the pixmap's top-left pixel will be mapped
    to.
*/

/*!
    Fills the pixmap with the given \a color.

    The effect of this function is undefined when the pixmap is
    being painted on.

    \sa {QPixmap#Pixmap Transformations}{Pixmap Transformations}
*/
void QPixmap::fill(const QColor &color)
{
    if (isNull())
        return;

    // Some people are probably already calling fill while a painter is active, so to not break
    // their programs, only print a warning and return when the fill operation could cause a crash.
    if (Q_UNLIKELY(paintingActive() && (color.alpha() != 255) && !hasAlphaChannel())) {
        qWarning("QPixmap::fill: Cannot fill while pixmap is being painted on");
        return;
    }

    detach();
    data->fill(color);
}

/*!
    Returns a number that identifies this QPixmap. Distinct QPixmap
    objects can only have the same cache key if they refer to the same
    contents.

    The cacheKey() will change when the pixmap is altered.
*/
qint64 QPixmap::cacheKey() const
{
    if (isNull())
        return 0;

    Q_ASSERT(data);
    return data->cacheKey();
}

static void sendResizeEvents(QWidget *target)
{
    QResizeEvent e(target->size(), QSize());
    QApplication::sendEvent(target, &e);

    const QObjectList children = target->children();
    for (int i = 0; i < children.size(); ++i) {
        QWidget *child = static_cast<QWidget*>(children.at(i));
        if (child->isWidgetType() && !child->isWindow() && child->testAttribute(Qt::WA_PendingResizeEvent))
            sendResizeEvents(child);
    }
}

/*!
    Creates a pixmap and paints the given \a widget, restricted by the
    given \a rectangle, in it. If the \a widget has any children, then
    they are also painted in the appropriate positions.

    If no rectangle is specified (the default) the entire widget is
    painted.

    If \a widget is 0, the specified rectangle doesn't overlap the
    widget's rectangle, or an error occurs, the function will return a
    null QPixmap.  If the rectangle is a superset of the given \a
    widget, the areas outside the \a widget are covered with the
    widget's background.

    This function actually asks \a widget to paint itself (and its
    children to paint themselves) by calling paintEvent() with painter
    redirection turned on. But QPixmap also provides the grabWindow()
    function which is a bit faster by grabbing pixels directly off the
    screen. In addition, if there are overlaying windows,
    grabWindow(), unlike grabWidget(), will see them.

    \warning Do not grab a widget from its QWidget::paintEvent().
    However, it is safe to grab a widget from another widget's
    \l {QWidget::}{paintEvent()}.

    \sa grabWindow()
*/
QPixmap QPixmap::grabWidget(QWidget * widget, const QRect &rect)
{
    if (!widget)
        return QPixmap();

    if (widget->testAttribute(Qt::WA_PendingResizeEvent) || !widget->testAttribute(Qt::WA_WState_Created))
        sendResizeEvents(widget);

    widget->d_func()->prepareToRender(QRegion(),
        QWidget::DrawWindowBackground | QWidget::DrawChildren | QWidget::IgnoreMask);

    QRect r(rect);
    if (r.width() < 0)
        r.setWidth(widget->width() - rect.x());
    if (r.height() < 0)
        r.setHeight(widget->height() - rect.y());

    if (!r.intersects(widget->rect()))
        return QPixmap();

    QPixmap res(r.size());
    if (!qt_widget_private(widget)->isOpaque)
        res.fill(Qt::transparent);

    widget->d_func()->render(&res, QPoint(), r, QWidget::DrawWindowBackground
                             | QWidget::DrawChildren | QWidget::IgnoreMask, true);
    return res;
}

/*!
    \fn QPixmap QPixmap::grabWidget(QWidget *widget, int x, int y, int
    width, int height)

    \overload

    Creates a pixmap and paints the given \a widget, restricted by
    QRect(\a x, \a y, \a width, \a height), in it.

    \warning Do not grab a widget from its QWidget::paintEvent().
    However, it is safe to grab a widget from another widget's
    \l {QWidget::}{paintEvent()}.
*/

/*****************************************************************************
  QPixmap stream functions
 *****************************************************************************/
#if !defined(QT_NO_DATASTREAM)
/*!
    \relates QPixmap

    Writes the given \a pixmap to the given \a stream as a PNG
    image. Note that writing the stream to a file will not produce a
    valid image file.

    \sa QPixmap::save(), {Serializing Qt Data Types}
*/

QDataStream &operator<<(QDataStream &stream, const QPixmap &pixmap)
{
    return stream << pixmap.toImage();
}

/*!
    \relates QPixmap

    Reads an image from the given \a stream into the given \a pixmap.

    \sa QPixmap::load(), {Serializing Qt Data Types}
*/

QDataStream &operator>>(QDataStream &stream, QPixmap &pixmap)
{
    QImage image;
    stream >> image;

    if (image.isNull()) {
        pixmap = QPixmap();
    } else if (image.depth() == 1) {
        pixmap = QBitmap::fromImage(image);
    } else {
        pixmap = QPixmap::fromImage(image);
    }
    return stream;
}

#endif // QT_NO_DATASTREAM

/*!
    \fn QPixmap QPixmap::scaled(int width, int height,
    Qt::AspectRatioMode aspectRatioMode, Qt::TransformationMode
    transformMode) const

    \overload

    Returns a copy of the pixmap scaled to a rectangle with the given
    \a width and \a height according to the given \a aspectRatioMode and
    \a transformMode.

    If either the \a width or the \a height is zero or negative, this
    function returns a null pixmap.
*/

/*!
    \fn QPixmap QPixmap::scaled(const QSize &size, Qt::AspectRatioMode
    aspectRatioMode, Qt::TransformationMode transformMode) const

    Scales the pixmap to the given \a size, using the aspect ratio and
    transformation modes specified by \a aspectRatioMode and \a
    transformMode.

    \image qimage-scaling.png

    \list
    \i If \a aspectRatioMode is Qt::IgnoreAspectRatio, the pixmap
       is scaled to \a size.
    \i If \a aspectRatioMode is Qt::KeepAspectRatio, the pixmap is
       scaled to a rectangle as large as possible inside \a size, preserving the aspect ratio.
    \i If \a aspectRatioMode is Qt::KeepAspectRatioByExpanding,
       the pixmap is scaled to a rectangle as small as possible
       outside \a size, preserving the aspect ratio.
    \endlist

    If the given \a size is empty, this function returns a null
    pixmap.


    In some cases it can be more beneficial to draw the pixmap to a
    painter with a scale set rather than scaling the pixmap. This is
    the case when the scale factor changes rapidly.

    \sa isNull(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}

*/
QPixmap QPixmap::scaled(const QSize& s, Qt::AspectRatioMode aspectMode, Qt::TransformationMode mode) const
{
    if (Q_UNLIKELY(isNull())) {
        qWarning("QPixmap::scaled: Pixmap is a null pixmap");
        return QPixmap();
    }
    if (s.isEmpty())
        return QPixmap();

    QSize newSize = size();
    newSize.scale(s, aspectMode);
    newSize.rwidth() = qMax(newSize.width(), 1);
    newSize.rheight() = qMax(newSize.height(), 1);
    if (newSize == size())
        return *this;

    QTransform wm = QTransform::fromScale((qreal)newSize.width() / width(),
                                          (qreal)newSize.height() / height());
    return transformed(wm, mode);
}

/*!
    \fn QPixmap QPixmap::scaledToWidth(int width, Qt::TransformationMode
    mode) const

    Returns a scaled copy of the image. The returned image is scaled
    to the given \a width using the specified transformation \a mode.
    The height of the pixmap is automatically calculated so that the
    aspect ratio of the pixmap is preserved.

    If \a width is 0 or negative, a null pixmap is returned.

    \sa isNull(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}
*/
QPixmap QPixmap::scaledToWidth(int w, Qt::TransformationMode mode) const
{
    if (Q_UNLIKELY(isNull())) {
        qWarning("QPixmap::scaleWidth: Pixmap is a null pixmap");
        return copy();
    }
    if (w <= 0)
        return QPixmap();

    qreal factor = (qreal) w / width();
    QTransform wm = QTransform::fromScale(factor, factor);
    return transformed(wm, mode);
}

/*!
    \fn QPixmap QPixmap::scaledToHeight(int height,
    Qt::TransformationMode mode) const

    Returns a scaled copy of the image. The returned image is scaled
    to the given \a height using the specified transformation \a mode.
    The width of the pixmap is automatically calculated so that the
    aspect ratio of the pixmap is preserved.

    If \a height is 0 or negative, a null pixmap is returned.

    \sa isNull(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}
*/
QPixmap QPixmap::scaledToHeight(int h, Qt::TransformationMode mode) const
{
    if (Q_UNLIKELY(isNull())) {
        qWarning("QPixmap::scaleHeight: Pixmap is a null pixmap");
        return copy();
    }
    if (h <= 0)
        return QPixmap();

    qreal factor = (qreal) h / height();
    QTransform wm = QTransform::fromScale(factor, factor);
    return transformed(wm, mode);
}

/*!
    Returns a copy of the pixmap that is transformed using the given
    transformation \a transform and transformation \a mode. The original
    pixmap is not changed.

    The transformation \a transform is internally adjusted to compensate
    for unwanted translation; i.e. the pixmap produced is the smallest
    pixmap that contains all the transformed points of the original
    pixmap. Use the trueMatrix() function to retrieve the actual
    matrix used for transforming the pixmap.

    This function is slow because it involves transformation to a
    QImage, non-trivial computations and a transformation back to a
    QPixmap.

    \sa trueMatrix(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}
*/
QPixmap QPixmap::transformed(const QTransform &transform,
                             Qt::TransformationMode mode) const
{
    if (isNull() || transform.type() <= QTransform::TxTranslate)
        return *this;

    return data->transformed(transform, mode);
}

/*!
  \overload

  This convenience function loads the \a matrix into a
  QTransform and calls the overloaded function.
 */
QPixmap QPixmap::transformed(const QMatrix &matrix, Qt::TransformationMode mode) const
{
    return transformed(QTransform(matrix), mode);
}

/*!
    \class QPixmap

    \brief The QPixmap class is an off-screen image representation
    that can be used as a paint device.

    \ingroup painting
    \ingroup shared


    Qt provides four classes for handling image data: QImage, QPixmap,
    and QBitmap. QImage is designed and optimized for I/O, and for direct
    pixel access and manipulation, while QPixmap is designed and
    optimized for showing images on screen. QBitmap is only a convenience
    class that inherits QPixmap, ensuring a depth of 1. The isQBitmap()
    function returns true if a QPixmap object is really a bitmap,
    otherwise returns false.

    A QPixmap can easily be displayed on the screen using QLabel or
    one of QAbstractButton's subclasses (such as QPushButton and
    QToolButton). QLabel has a pixmap property, whereas
    QAbstractButton has an icon property.

    In addition to the ordinary constructors, a QPixmap can be
    constructed using the static grabWidget() and grabWindow()
    functions which creates a QPixmap and paints the given widget, or
    window, into it.

    QPixmap objects can be passed around by value since the QPixmap
    class uses implicit data sharing. For more information, see the \l
    {Implicit Data Sharing} documentation. QPixmap objects can also be
    streamed.

    Note that the pixel data in a pixmap is internal and is managed by
    the underlying window system. Because QPixmap is a QPaintDevice
    subclass, QPainter can be used to draw directly onto pixmaps.
    Pixels can only be accessed through QPainter functions or by
    converting the QPixmap to a QImage. However, the fill() function
    is available for initializing the entire pixmap with a given color.

    There are functions to convert between QImage and
    QPixmap. Typically, the QImage class is used to load an image
    file, optionally manipulating the image data, before the QImage
    object is converted into a QPixmap to be shown on
    screen. Alternatively, if no manipulation is desired, the image
    file can be loaded directly into a QPixmap.

    QPixmap provides a collection of functions that can be used to
    obtain a variety of information about the pixmap. In addition,
    there are several functions that enables transformation of the
    pixmap.

    \tableofcontents

    \section1 Reading and Writing Image Files

    QPixmap provides several ways of reading an image file: The file
    can be loaded when constructing the QPixmap object, or by using
    the load() or loadFromData() functions later on. When loading an
    image, the file name can either refer to an actual file on disk or
    to one of the application's embedded resources. See \l{The Qt
    Resource System} overview for details on how to embed images and
    other resource files in the application's executable.

    Simply call the save() function to save a QPixmap object.

    The complete list of supported file formats are available through
    the QImageReader::supportedImageFormats() and
    QImageWriter::supportedImageFormats() functions. New file formats
    can be added as plugins. By default, Katie supports the following
    formats:

    \table
    \header \o Format \o Description                      \o Katie's support
    \row    \o PNG    \o Portable Network Graphics        \o Read/write
    \row    \o KAT    \o Katie Image                      \o Read/write
    \row    \o PBM    \o Portable Bitmap                  \o Read
    \row    \o PPM    \o Portable Pixmap                  \o Read/write
    \row    \o XPM    \o X11 Pixmap                       \o Read
    \endtable

    \section1 Pixmap Information

    QPixmap provides a collection of functions that can be used to
    obtain a variety of information about the pixmap:

    \table
    \header
    \o \o Available Functions
    \row
    \o Geometry
    \o
    The size(), width() and height() functions provide information
    about the pixmap's size. The rect() function returns the image's
    enclosing rectangle.

    \row
    \o Alpha component
    \o

    The hasAlphaChannel() returns true if the pixmap has a format that
    respects the alpha channel, otherwise returns false. The hasAlpha(),
    setMask() and mask() functions are legacy and should not be used.
    They are potentially very slow.

    The createHeuristicMask() function creates and returns a 1-bpp
    heuristic mask (i.e. a QBitmap) for this pixmap. It works by
    selecting a color from one of the corners and then chipping away
    pixels of that color, starting at all the edges. The
    createMaskFromColor() function creates and returns a mask (i.e. a
    QBitmap) for the pixmap based on a given color.

    \row
    \o Low-level information
    \o

    The depth() function returns the depth of the pixmap. The
    defaultDepth() function returns the default depth, i.e. the depth
    used by the application on the given screen.

    The cacheKey() function returns a number that uniquely
    identifies the contents of the QPixmap object.

    \endtable

    \section1 Pixmap Conversion

    A QPixmap object can be converted into a QImage using the
    toImage() function. Likewise, a QImage can be converted into a
    QPixmap using the fromImage(). If this is too expensive an
    operation, you can use QBitmap::fromImage() instead.

    \section1 Pixmap Transformations

    QPixmap supports a number of functions for creating a new pixmap
    that is a transformed version of the original:

    The scaled(), scaledToWidth() and scaledToHeight() functions
    return scaled copies of the pixmap, while the copy() function
    creates a QPixmap that is a plain copy of the original one.

    The transformed() function returns a copy of the pixmap that is
    transformed with the given transformation matrix and
    transformation mode: Internally, the transformation matrix is
    adjusted to compensate for unwanted translation,
    i.e. transformed() returns the smallest pixmap containing all
    transformed points of the original pixmap. The static trueMatrix()
    function returns the actual matrix used for transforming the
    pixmap.

    \sa QBitmap, QImage, QImageReader, QImageWriter
*/

/*!
    Returns true if this pixmap has an alpha channel, \e or has a
    mask, otherwise returns false.

    \sa hasAlphaChannel(), mask()
*/
bool QPixmap::hasAlpha() const
{
    return data && data->hasAlphaChannel();
}

/*!
    Returns true if the pixmap has a format that respects the alpha
    channel, otherwise returns false.

    \sa hasAlpha()
*/
bool QPixmap::hasAlphaChannel() const
{
    return data && data->hasAlphaChannel();
}

/*!
    \internal
*/
int QPixmap::metric(PaintDeviceMetric metric) const
{
    return data ? data->metric(metric) : 0;
}

/*!
    Sets the alpha channel of this pixmap to the given \a alphaChannel
    by converting the \a alphaChannel into 32 bit and using the
    intensity of the RGB pixel values.

    The effect of this function is undefined when the pixmap is being
    painted on.

    \warning This is potentially an expensive operation. Most usecases
    for this function are covered by QPainter and compositionModes
    which will normally execute faster.

    \sa alphaChannel(), {QPixmap#Pixmap Transformations}{Pixmap
    Transformations}
*/
void QPixmap::setAlphaChannel(const QPixmap &alphaChannel)
{
    if (alphaChannel.isNull())
        return;

    if (Q_UNLIKELY(paintingActive())) {
        qWarning("QPixmap::setAlphaChannel: "
                 "Cannot set alpha channel while pixmap is being painted on");
        return;
    }

    if (Q_UNLIKELY(width() != alphaChannel.width() && height() != alphaChannel.height())) {
        qWarning("QPixmap::setAlphaChannel: "
                 "The pixmap and the alpha channel pixmap must have the same size");
        return;
    }

    detach();
    data->setAlphaChannel(alphaChannel);
}

/*!
    Returns the alpha channel of the pixmap as a new grayscale QPixmap in which
    each pixel's red, green, and blue values are given the alpha value of the
    original pixmap.

    You can use this function while debugging
    to get a visible image of the alpha channel. If the pixmap doesn't have an
    alpha channel, i.e., the alpha channel's value for all pixels equals
    0xff), a null pixmap is returned. You can check this with the \c isNull()
    function.

    We show an example:

    \snippet doc/src/snippets/alphachannel.cpp 0

    \image alphachannelimage.png The pixmap and channelImage QPixmaps

    \warning This is an expensive operation. The alpha channel of the
    pixmap is extracted dynamically from the pixeldata. Most usecases of this
    function are covered by QPainter and compositionModes which will normally
    execute faster.

    \sa setAlphaChannel(), {QPixmap#Pixmap Information}{Pixmap
    Information}
*/
QPixmap QPixmap::alphaChannel() const
{
    return data ? data->alphaChannel() : QPixmap();
}

/*!
    \internal
*/
QPaintEngine *QPixmap::paintEngine() const
{
    return data ? data->paintEngine() : nullptr;
}

/*!
    \fn QBitmap QPixmap::mask() const

    Extracts a bitmap mask from the pixmap's alpha channel.

    \warning This is potentially an expensive operation. The mask of
    the pixmap is extracted dynamically from the pixeldata.

    \sa setMask(), {QPixmap#Pixmap Information}{Pixmap Information}
*/
QBitmap QPixmap::mask() const
{
    return data ? data->mask() : QBitmap();
}

/*!
    Detaches the pixmap from shared pixmap data.

    A pixmap is automatically detached by Katie whenever its contents
    are about to change. This is done in almost all QPixmap member
    functions that modify the pixmap (fill(), fromImage(),
    load(), etc.), and in QPainter::begin() on a pixmap.

    The detach() function returns immediately if there is just a
    single reference or if the pixmap has not been initialized yet.
*/
void QPixmap::detach()
{
    if (!data)
        return;

    data->image.detach();

    if (data->ref != 1) {
        *this = copy();
    }
    ++data->detach_no;
}

/*!
    Converts the given \a image to a pixmap using the specified \a
    flags to control the conversion.  The \a flags argument is a
    bitwise-OR of the \l{Qt::ImageConversionFlags}. Passing 0 for \a
    flags sets all the default options.

    In case of monochrome images, the image is first converted to a
    32-bit pixmap and then filled with the colors in the color table.
    If this is too expensive an operation, you can
    use QBitmap::fromImage() instead.

    \sa fromImageReader(), toImage(), {QPixmap#Pixmap Conversion}{Pixmap Conversion}
*/
QPixmap QPixmap::fromImage(const QImage &image, Qt::ImageConversionFlags flags)
{
    if (image.isNull())
        return QPixmap();

    QScopedPointer<QPixmapData> data(new QPixmapData(QPixmapData::PixmapType));
    data->fromImage(image, flags);
    return QPixmap(data.take());
}

/*!
    Create a QPixmap from an image read directly from an \a imageReader.
    The \a flags argument is a bitwise-OR of the \l{Qt::ImageConversionFlags}.
    Passing 0 for \a flags sets all the default options.

    On some systems, reading an image directly to QPixmap can use less memory than
    reading a QImage to convert it to QPixmap.

    \sa fromImage(), toImage(), {QPixmap#Pixmap Conversion}{Pixmap Conversion}
*/
QPixmap QPixmap::fromImageReader(QImageReader *imageReader, Qt::ImageConversionFlags flags)
{
    QScopedPointer<QPixmapData> data(new QPixmapData(QPixmapData::PixmapType));
    data->fromImageReader(imageReader, flags);
    return QPixmap(data.take());
}

/*!
  \internal
*/
QPixmapData* QPixmap::pixmapData() const
{
    if (data)
        return data.data();

    return nullptr;
}

/*!
    Creates and returns a pixmap constructed by grabbing the contents
    of the given \a window restricted by QRect(\a x, \a y, \a width,
    \a height).

    The arguments (\a{x}, \a{y}) specify the offset in the window,
    whereas (\a{width}, \a{height}) specify the area to be copied.  If
    \a width is negative, the function copies everything to the right
    border of the window. If \a height is negative, the function
    copies everything to the bottom of the window.

    The window system identifier (\c WId) can be retrieved using the
    QWidget::winId() function. The rationale for using a window
    identifier and not a QWidget, is to enable grabbing of windows
    that are not part of the application, window system frames, and so
    on.

    The grabWindow() function grabs pixels from the screen, not from
    the window, i.e. if there is another window partially or entirely
    over the one you grab, you get pixels from the overlying window,
    too. The mouse cursor is generally not grabbed.

    Note on X11 that if the given \a window doesn't have the same depth
    as the root window, and another window partially or entirely
    obscures the one you grab, you will \e not get pixels from the
    overlying window.  The contents of the obscured areas in the
    pixmap will be undefined and uninitialized.

    \warning In general, grabbing an area outside the screen is not
    safe. This depends on the underlying window system.

    \sa grabWidget(), {Screenshot Example}
*/
QPixmap QPixmap::grabWindow(WId window, int x, int y, int w, int h)
{
    if (w == 0 || h == 0)
        return QPixmap();

    Window unusedroot;
    int unusedx;
    int unusedy;
    uint width;
    uint height;
    uint unusedborderwidth;
    uint depth;
    XGetGeometry(
        qt_x11Data->display,
        window,
        &unusedroot, &unusedx, &unusedy,
        &width, &height,
        &unusedborderwidth,
        &depth
    );

    if (w < 0)
        w = width - x;
    if (h < 0)
        h = height - y;

    XImage *ximage = XGetImage(
        qt_x11Data->display, window,
        x, y,
        w, h,
        AllPlanes, (depth == 1) ? XYPixmap : ZPixmap
    );
    if (Q_UNLIKELY(!ximage)) {
        return QPixmap();
    }
    QImage::Format format = QImage::systemFormat();
    if (depth == 1 && ximage->bitmap_bit_order == LSBFirst) {
        format = QImage::Format_MonoLSB;
    } else if (depth == 1) {
        format = QImage::Format_Mono;
    } else if (depth == 24) {
        format = QImage::Format_RGB32;
    } else if (depth == 32 && qt_x11Data->use_xrender) {
        format = QImage::Format_ARGB32_Premultiplied;
    }
    QImage image(w, h, format);
    if (image.depth() == 1) {
        image.setColorTable(monoColorTable());
    }
    QX11Data::copyXImageToQImage(ximage, image);
    XDestroyImage(ximage);
    return QPixmap::fromImage(image);
}

#if defined(Q_WS_X11)
/*!
    \since 4.5

    Creates a QPixmap from the native X11 Pixmap handle \a pixmap.

    \warning This function is X11 specific; using it is non-portable.
*/
QPixmap QPixmap::fromX11Pixmap(Qt::HANDLE pixmap)
{
    // to X Pixmap and Window are just "Drawable"
    return QPixmap::grabWindow(pixmap);
}

/*!
    \since 4.12

    Returns X11 Pixmap handle of the pixmap.

    QPixmap does \e not take ownership of the pixmap handle, it
    has to be deleted by the user.

    \warning This function is only available on X11.
*/
Qt::HANDLE QPixmap::toX11Pixmap() const
{
    if (isNull()) {
        return 0;
    }
    Qt::HANDLE handle = XCreatePixmap(
        qt_x11Data->display, QX11Info::appRootWindow(),
        width(), height(),
        32
    );
    if (!handle) {
        return 0;
    }
    QImage image = toImage();
    if (image.depth() != 32) {
        image = image.convertToFormat(QImage::Format_RGB32);
    }
    XImage* ximage = XCreateImage(
        qt_x11Data->display, (Visual*)QX11Info::appVisual(),
        image.depth(),
        ZPixmap,
        0, NULL, // offset and data
        image.width(), image.height(),
        32,
        0 // bytes per line
    );
    if (Q_UNLIKELY(!ximage)) {
        XFreePixmap(qt_x11Data->display, handle);
        return 0;
    }
    bool freedata = false;
    QX11Data::copyQImageToXImage(image, ximage, &freedata);
    GC xgc = XCreateGC(
        qt_x11Data->display, handle,
        0, 0 // value mask and values
    );
    XPutImage(
        qt_x11Data->display, handle, xgc, ximage,
        0, 0, 0, 0, // source x and y, destination x and y
        ximage->width, ximage->height
    );
    XFreeGC(qt_x11Data->display, xgc);
    QX11Data::destroyXImage(ximage, freedata);
    return handle;
}
#endif // Q_WS_X11

QT_END_NAMESPACE
