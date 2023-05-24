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

#include "qimage.h"
#include "qdatastream.h"
#include "qbuffer.h"
#include "qmap.h"
#include "qmatrix.h"
#include "qtransform.h"
#include "qimagereader.h"
#include "qimagewriter.h"
#include "qstringlist.h"
#include "qvariant.h"
#include "qdrawhelper_p.h"
#include "qmemrotate_p.h"
#include "qpixmapdata_p.h"
#include "qhash.h"
#include "qpaintengine_raster_p.h"
#include "qimage_p.h"
#include "qvarlengtharray.h"
#include "qcorecommon_p.h"
#include "qguicommon_p.h"
#include "qx11info_x11.h"

#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

QT_BEGIN_NAMESPACE

#define QIMAGE_SANITYCHECK_MEMORY(image) \
    if (Q_UNLIKELY((image).isNull())) { \
        qWarning("QImage: out of memory, returning null image"); \
        return QImage(); \
    }

static QImage rotated90(const QImage &image) {
    QImage out(image.height(), image.width(), image.format());
    QIMAGE_SANITYCHECK_MEMORY(out);
    if (out.depth() == 1) {
        out.setColorTable(image.colorTable());
    }
    int w = image.width();
    int h = image.height();
    switch (image.format()) {
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        qt_memrotate270(reinterpret_cast<const quint32*>(image.constBits()),
                        w, h, image.bytesPerLine(),
                        reinterpret_cast<quint32*>(out.bits()),
                        out.bytesPerLine());
        break;
    case QImage::Format_RGB16:
        qt_memrotate270(reinterpret_cast<const quint16*>(image.constBits()),
                        w, h, image.bytesPerLine(),
                        reinterpret_cast<quint16*>(out.bits()),
                        out.bytesPerLine());
        break;
    default:
        for (int y=0; y<h; ++y) {
            for (int x=0; x<w; ++x)
                out.setPixel(h-y-1, x, image.pixel(x, y));
        }
        break;
    }
    return out;
}

static QImage rotated180(const QImage &image) {
    return image.mirrored(true, true);
}

static QImage rotated270(const QImage &image) {
    QImage out(image.height(), image.width(), image.format());
    QIMAGE_SANITYCHECK_MEMORY(out);
    if (out.depth() == 1) {
        out.setColorTable(image.colorTable());
    }
    int w = image.width();
    int h = image.height();
    switch (image.format()) {
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        qt_memrotate90(reinterpret_cast<const quint32*>(image.constBits()),
                       w, h, image.bytesPerLine(),
                       reinterpret_cast<quint32*>(out.bits()),
                       out.bytesPerLine());
        break;
    case QImage::Format_RGB16:
        qt_memrotate90(reinterpret_cast<const quint16*>(image.constBits()),
                       w, h, image.bytesPerLine(),
                       reinterpret_cast<quint16*>(out.bits()),
                       out.bytesPerLine());
        break;
    default:
        for (int y=0; y<h; ++y) {
            for (int x=0; x<w; ++x)
                out.setPixel(y, w-x-1, image.pixel(x, y));
        }
        break;
    }
    return out;
}

QAtomicInt qimage_serial_number(1);

QImageData::QImageData()
    : ref(1), width(0), height(0), depth(0), nbytes(0),
    mono0(-1), mono1(-1), data(nullptr),
    format(QImage::Format_ARGB32), bytes_per_line(0),
    ser_no(qimage_serial_number.fetchAndAddRelaxed(1)),
    detach_no(0),
    dpmx(QX11Info::appDpiX() * 100 / qreal(2.54)),
    dpmy(QX11Info::appDpiY() * 100 / qreal(2.54)),
    own_data(true), ro_data(false), has_alpha_clut(false),
    paintEngine(nullptr)
{
}

/*! \fn QImageData * QImageData::create(const QSize &size, QImage::Format format)

    \internal

    Creates a new image data.
    Returns 0 if invalid parameters are give or anything else failed.
*/
QImageData * QImageData::create(const QSize &size, QImage::Format format)
{
    if (!size.isValid()) {
        // invalid parameter(s)
        return nullptr;
    }

    switch (format) {
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        case QImage::Format_RGB16: {
            break;
        }
        default: {
            return nullptr;
        }
    }

    int width = size.width();
    int height = size.height();
    int depth = qt_depthForFormat(format);
    // bytes per scanline (must be multiple of 4)
    const int bytes_per_line = ((width * depth + 31) >> 5) << 2;

    // sanity check
    if (width <= 0 || height <= 0 || bytes_per_line <= 0
        || width >= USHRT_MAX || height >= USHRT_MAX) {
        return nullptr;
    }

    QScopedPointer<QImageData> d(new QImageData);
    if (depth == 1) {
        // QImage::Format_Mono or QImage::Format_MonoLSB
        d->mono0 = qt_blackrgba;
        d->mono1 = qt_whitergba;
    }

    d->width = width;
    d->height = height;
    d->depth = depth;
    d->format = format;
    d->bytes_per_line = bytes_per_line;
    d->nbytes = d->bytes_per_line*height;
    d->data = static_cast<uchar*>(::malloc(d->nbytes));

    if (!d->data) {
        return nullptr;
    }

    return d.take();

}

QImageData::~QImageData()
{
    delete paintEngine;
    if (data && own_data)
        free(data);
    data = 0;
}


bool QImageData::checkForAlphaPixels() const
{
    bool has_alpha_pixels = false;

    switch (format) {
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB: {
            has_alpha_pixels = has_alpha_clut;
            break;
        }
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            const uchar *bits = data;
            for (int y=0; y<height && !has_alpha_pixels; ++y) {
                for (int x=0; x<width; ++x)
                    has_alpha_pixels |= (((const uint *)bits)[x] & 0xff000000) != 0xff000000;
                bits += bytes_per_line;
            }
            break;
        }
        default: {
            break;
        }
    }

    return has_alpha_pixels;
}

/*!
    \class QImage

    \ingroup painting
    \ingroup shared

    \reentrant

    \brief The QImage class provides a hardware-independent image
    representation that allows direct access to the pixel data, and
    can be used as a paint device.

    Qt provides four classes for handling image data: QImage, QPixmap,
    and QBitmap.  QImage is designed and optimized for I/O, and for
    direct pixel access and manipulation, while QPixmap is designed
    and optimized for showing images on screen. QBitmap is only a
    convenience class that inherits QPixmap, ensuring a depth of 1.

    Because QImage is a QPaintDevice subclass, QPainter can be used to
    draw directly onto images.  When using QPainter on a QImage, the
    painting can be performed in another thread than the current GUI
    thread.

    The QImage class supports several image formats described by the
    \l Format enum. These include monochrome, 32-bit and alpha-blended
    images.

    QImage provides a collection of functions that can be used to
    obtain a variety of information about the image. There are also
    several functions that enables transformation of the image.

    QImage objects can be passed around by value since the QImage
    class uses \l{Implicit Data Sharing}{implicit data
    sharing}. QImage objects can also be streamed and compared.

    \note If you would like to load QImage objects in a static build of Qt,
    refer to the \l{How To Create Qt Plugins#Static Plugins}{Plugin HowTo}.

    \tableofcontents

    \section1 Reading and Writing Image Files

    QImage provides several ways of loading an image file: The file
    can be loaded when constructing the QImage object, or by using the
    load() or loadFromData() functions later on. QImage also provides
    the static fromData() function, constructing a QImage from the
    given data.

    Simply call the save() function to save a QImage object.

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

    \section1 Katie Image Format

    The Katie image format is faster, in some cases smaller
    lossless substitute for the PNG format. Its usage should be
    primarily in caching and short-lived transfers such as sending
    QImage trough socket.

    Your code should be prepared to discard any image in that
    format and generate a new image in case loading fails as the
    format (its header and the way image data is stored) may change
    from release to release.

    \section1 Image Information

    QImage provides a collection of functions that can be used to
    obtain a variety of information about the image:

    \table
    \header
    \o \o Available Functions

    \row
    \o Geometry
    \o

    The size(), width(), height(), dotsPerMeterX(), and
    dotsPerMeterY() functions provide information about the image size
    and aspect ratio.

    The rect() function returns the image's enclosing rectangle. The
    valid() function tells if a given pair of coordinates is within
    this rectangle.

    \row
    \o Colors
    \o

    The color of a pixel can be retrieved by passing its coordinates
    to the pixel() function.  The pixel() function returns the color
    as a QRgb value indepedent of the image's format.

    In case of monochrome the colorTable() functions provide
    information about the color components used to store the image
    data: The colorTable() function returns the image's entire color
    table.

    The hasAlphaChannel() function tells if the image's format
    respects the alpha channel, or not. The allGray() and
    isGrayscale() functions tell whether an image's colors are all
    shades of gray.

    See also the \l {QImage#Pixel Manipulation}{Pixel Manipulation}
    and \l {QImage#Image Transformations}{Image Transformations}
    sections.

    \row
    \o Text
    \o

    The text() function returns the image text associated with the
    given text key. An image's text keys can be retrieved using the
    textKeys() function. Use the setText() function to alter an
    image's text.

    \row
    \o Low-level information
    \o

    The depth() function returns the depth of the image. The supported
    depths are 1 (monochrome), 8, 16 and 32 bits. The  bitPlaneCount()
    function tells how many of those bits that are used. For more
    information see the \l {QImage#Image Formats}{Image Formats}
    section.

    The format(), bytesPerLine(), and byteCount() functions provide
    low-level information about the data stored in the image.

    The cacheKey() function returns a number that uniquely
    identifies the contents of this QImage object.
    \endtable

    \section1 Pixel Manipulation

    The functions used to manipulate an image's pixels depend on the
    image format. The reason is that monochrome index-based and use a
    color lookup table, while 32-bit images store ARGB values directly.
    For more information on image formats, see the \l {Image Formats}
    section.

    In case of a 32-bit image, the setPixel() function can be used to
    alter the color of the pixel at the given coordinates to any other
    color specified as an ARGB quadruplet. To make a suitable QRgb
    value, use the qRgb() (adding a default alpha component to the
    given RGB values, i.e. creating an opaque color) or qRgba()
    function. For example:

    \table
    \header
    \o {2,1}32-bit
    \row
    \o \inlineimage qimage-32bit_scaled.png
    \o
    \snippet doc/src/snippets/code/src_gui_image_qimage.cpp 0
    \endtable

    In case of monchrome images, the pixel value is only an index from
    the image's color table. So the setPixel() function can only be
    used to alter the color of the pixel at the given coordinates to a
    predefined color from the image's color table, i.e. it can only
    change the pixel's index value. To alter or add a color to an
    image's color table, use the setColor() function.

    An entry in the color table is an ARGB quadruplet encoded as an
    QRgb value. Use the qRgb() and qRgba() functions to make a
    suitable QRgb value for use with the setColor() function.

    QImage also provide the scanLine() function which returns a
    pointer to the pixel data at the scanline with the given index,
    and the bits() function which returns a pointer to the first pixel
    data (this is equivalent to \c scanLine(0)).

    \section1 Image Formats

    Each pixel stored in a QImage is represented by an integer. The
    size of the integer varies depending on the format. QImage
    supports several image formats described by the \l Format
    enum.

    Monochrome images are stored using 1-bit indexes into a color table
    with at most two colors. There are two different types of
    monochrome images: big endian (MSB first) or little endian (LSB
    first) bit order.

    32-bit images have no color table; instead, each pixel contains an
    QRgb value. There are three different types of 32-bit images
    storing RGB (i.e. 0xffRRGGBB), ARGB and premultiplied ARGB
    values respectively. In the premultiplied format the red, green,
    and blue channels are multiplied by the alpha component divided by
    255.

    An image's format can be retrieved using the format()
    function. Use the convertToFormat() functions to convert an image
    into another format. The allGray() and isGrayscale() functions
    tell whether a color image can safely be converted to a grayscale
    image.

    \section1 Image Transformations

    QImage supports a number of functions for creating a new image
    that is a transformed version of the original: The
    createAlphaMask() function builds and returns a 1-bpp mask from
    the alpha buffer in this image, and the createHeuristicMask()
    function creates and returns a 1-bpp heuristic mask for this
    image. The latter function works by selecting a color from one of
    the corners, then chipping away pixels of that color starting at
    all the edges.

    The mirrored() function returns a mirror of the image in the
    desired direction, the scaled() returns a copy of the image scaled
    to a rectangle of the desired measures, and the rgbSwapped() function
    constructs a BGR image from a RGB image.

    The scaledToWidth() and scaledToHeight() functions return scaled
    copies of the image.

    The transformed() function returns a copy of the image that is
    transformed with the given transformation matrix and
    transformation mode: Internally, the transformation matrix is
    adjusted to compensate for unwanted translation,
    i.e. transformed() returns the smallest image containing all
    transformed points of the original image. The static trueMatrix()
    function returns the actual matrix used for transforming the
    image.

    There are also functions for changing attributes of an image
    in-place:

    \table
    \header \o Function \o Description
    \row
    \o setDotsPerMeterX()
    \o Defines the aspect ratio by setting the number of pixels that fit
    horizontally in a physical meter.
    \row
    \o setDotsPerMeterY()
    \o Defines the aspect ratio by setting the number of pixels that fit
    vertically in a physical meter.
    \row
    \o fill()
    \o Fills the entire image with the given pixel value.
    \row
    \o invertPixels()
    \o Inverts all pixel values in the image using the given InvertMode value.
    \row
    \o setColorTable()
    \o Sets the color table used to translate color indexes. Only
    monochrome formats.

    \endtable

    \sa QImageReader, QImageWriter, QPixmap, QSvgRenderer, {Image Composition Example},
        {Image Viewer Example}, {Scribble Example}, {Pixelator Example}
*/

/*!
    \enum QImage::Endian
    \compat

    This enum type is used to describe the endianness of the CPU and
    graphics hardware. It is provided here for compatibility with earlier versions of Qt.

    Use the \l Format enum instead. The \l Format enum specify the
    endianess for monchrome formats, but for other formats the
    endianess is not relevant.

    \value IgnoreEndian  Endianness does not matter. Useful for some
                         operations that are independent of endianness.
    \value BigEndian     Most significant bit first or network byte order, as on SPARC, PowerPC, and Motorola CPUs.
    \value LittleEndian  Least significant bit first or little endian byte order, as on Intel x86.
*/

/*!
    \enum QImage::InvertMode

    This enum type is used to describe how pixel values should be
    inverted in the invertPixels() function.

    \value InvertRgb    Invert only the RGB values and leave the alpha
                        channel unchanged.

    \value InvertRgba   Invert all channels, including the alpha channel.

    \sa invertPixels()
*/

/*!
    \enum QImage::Format

    The following image formats are available in Qt. Values greater
    than QImage::Format_RGB16 were added in Qt 4.4. See the notes
    after the table.

    \value Format_Invalid   The image is invalid.
    \value Format_Mono      The image is stored using 1-bit per pixel. Bytes are
                            packed with the most significant bit (MSB) first.
    \value Format_MonoLSB   The image is stored using 1-bit per pixel. Bytes are
                            packed with the less significant bit (LSB) first.

    \value Format_RGB32     The image is stored using a 32-bit RGB format (0xffRRGGBB).

    \value Format_ARGB32    The image is stored using a 32-bit ARGB
                            format (0xAARRGGBB).

    \value Format_ARGB32_Premultiplied  The image is stored using a premultiplied 32-bit
                            ARGB format (0xAARRGGBB), i.e. the red,
                            green, and blue channels are multiplied
                            by the alpha component divided by 255. (If RR, GG, or BB
                            has a higher value than the alpha channel, the results are
                            undefined.) Certain operations (such as image composition
                            using alpha blending) are faster using premultiplied ARGB32
                            than with plain ARGB32.

    \value Format_RGB16     The image is stored using a 16-bit RGB format (5-6-5).

    \note Do not render into ARGB32 images using QPainter.  Using
    QImage::Format_ARGB32_Premultiplied is significantly faster.

    \sa format(), convertToFormat()
*/

/*****************************************************************************
  QImage member functions
 *****************************************************************************/

// table to flip bits
static const uchar bitflip[256] = {
    /*
        open OUT, "| fmt";
        for $i (0..255) {
            print OUT (($i >> 7) & 0x01) | (($i >> 5) & 0x02) |
                      (($i >> 3) & 0x04) | (($i >> 1) & 0x08) |
                      (($i << 7) & 0x80) | (($i << 5) & 0x40) |
                      (($i << 3) & 0x20) | (($i << 1) & 0x10), ", ";
        }
        close OUT;
    */
    0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240,
    8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248,
    4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244,
    12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252,
    2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
    10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250,
    6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246,
    14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254,
    1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241,
    9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
    5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245,
    13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253,
    3, 131, 67, 195, 35, 163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243,
    11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251,
    7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
    15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255
};

/*!
    Constructs a null image.

    \sa isNull()
*/

QImage::QImage()
    : QPaintDevice(),
    d(nullptr)
{
}

/*!
    Constructs an image with the given \a width, \a height and \a
    format.

    A \l{isNull()}{null} image will be returned if memory cannot be allocated.

    \warning This will create a QImage with uninitialized data. Call
    fill() to fill the image with an appropriate pixel value before
    drawing onto it with QPainter.
*/
QImage::QImage(int width, int height, Format format)
    : QPaintDevice(),
    d(QImageData::create(QSize(width, height), format))
{
}

/*!
    Constructs an image with the given \a size and \a format.

    A \l{isNull()}{null} image is returned if memory cannot be allocated.

    \warning This will create a QImage with uninitialized data. Call
    fill() to fill the image with an appropriate pixel value before
    drawing onto it with QPainter.
*/
QImage::QImage(const QSize &size, Format format)
    : QPaintDevice(),
    d(QImageData::create(size, format))
{
}



QImageData *QImageData::create(uchar *data, int width, int height,  int bpl, QImage::Format format, bool readOnly)
{
    switch (format) {
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        case QImage::Format_RGB16: {
            break;
        }
        default: {
            return nullptr;
        }
    }

    const int depth = qt_depthForFormat(format);
    const int min_bytes_per_line = (width * depth + 7)/8;

    if (bpl <= 0) {
        bpl = ((width * depth + 31)/32) * 4;
    }

    if (Q_UNLIKELY(width <= 0 || height <= 0 || !data
        || INT_MAX/sizeof(uchar *) < uint(height)
        || INT_MAX/uint(depth) < uint(width)
        || bpl <= 0
        || bpl < min_bytes_per_line
        || INT_MAX/uint(bpl) < uint(height))) {
        return nullptr; // invalid parameter(s)
    }

    QScopedPointer<QImageData> d(new QImageData);

    d->own_data = false;
    d->ro_data = readOnly;
    d->data = data;
    d->width = width;
    d->height = height;
    d->depth = depth;
    d->format = format;

    d->bytes_per_line = bpl;
    d->nbytes = d->bytes_per_line * height;

    return d.take();
}

/*!
    Constructs an image with the given \a width, \a height and \a
    format, that uses an existing memory buffer, \a data. The \a width
    and \a height must be specified in pixels, \a data must be 32-bit aligned,
    and each scanline of data in the image must also be 32-bit aligned.

    The buffer must remain valid throughout the life of the
    QImage. The image does not delete the buffer at destruction.
*/
QImage::QImage(uchar* data, int width, int height, Format format)
    : QPaintDevice(),
    d(QImageData::create(data, width, height, 0, format, false))
{
}

/*!
    Constructs an image with the given \a width, \a height and \a
    format, that uses an existing read-only memory buffer, \a
    data. The \a width and \a height must be specified in pixels, \a
    data must be 32-bit aligned, and each scanline of data in the
    image must also be 32-bit aligned.

    The buffer must remain valid throughout the life of the QImage and
    all copies that have not been modified or otherwise detached from
    the original buffer. The image does not delete the buffer at
    destruction.

    Unlike the similar QImage constructor that takes a non-const data buffer,
    this version will never alter the contents of the buffer.  For example,
    calling QImage::bits() will return a deep copy of the image, rather than
    the buffer passed to the constructor.  This allows for the efficiency of
    constructing a QImage from raw data, without the possibility of the raw
    data being changed.
*/
QImage::QImage(const uchar* data, int width, int height, Format format)
    : QPaintDevice(),
    d(QImageData::create(const_cast<uchar*>(data), width, height, 0, format, true))
{
}

/*!
    Constructs an image with the given \a width, \a height and \a
    format, that uses an existing memory buffer, \a data. The \a width
    and \a height must be specified in pixels. \a bytesPerLine
    specifies the number of bytes per line (stride).

    The buffer must remain valid throughout the life of the
    QImage. The image does not delete the buffer at destruction.
*/
QImage::QImage(uchar *data, int width, int height, int bytesPerLine, Format format)
    : QPaintDevice(),
    d(QImageData::create(data, width, height, bytesPerLine, format, false))
{
}


/*!
    Constructs an image with the given \a width, \a height and \a
    format, that uses an existing memory buffer, \a data. The \a width
    and \a height must be specified in pixels. \a bytesPerLine
    specifies the number of bytes per line (stride).

    The buffer must remain valid throughout the life of the
    QImage. The image does not delete the buffer at destruction.

    Unlike the similar QImage constructor that takes a non-const data buffer,
    this version will never alter the contents of the buffer.  For example,
    calling QImage::bits() will return a deep copy of the image, rather than
    the buffer passed to the constructor.  This allows for the efficiency of
    constructing a QImage from raw data, without the possibility of the raw
    data being changed.
*/

QImage::QImage(const uchar *data, int width, int height, int bytesPerLine, Format format)
    : QPaintDevice(),
    d(QImageData::create(const_cast<uchar*>(data), width, height, bytesPerLine, format, true))
{
}

/*!
    Constructs an image and tries to load the image from the file with
    the given \a fileName.

    The loader attempts to read the image using the specified \a
    format. If the \a format is not specified (which is the default),
    the loader probes the file for a header to guess the file format.

    If the loading of the image failed, this object is a null image.

    \sa isNull(), {QImage#Reading and Writing Image Files}{Reading and Writing Image Files}
*/

QImage::QImage(const QString &fileName, const char *format)
    : QPaintDevice(),
    d(nullptr)
{
    load(fileName, format);
}

/*!
    Constructs an image and tries to load the image from the file with
    the given \a fileName.

    The loader attempts to read the image using the specified \a
    format. If the \a format is not specified (which is the default),
    the loader probes the file for a header to guess the file format.

    If the loading of the image failed, this object is a null image.

    You can disable this constructor by defining \c
    QT_NO_CAST_FROM_ASCII when you compile your applications. This can
    be useful, for example, if you want to ensure that all
    user-visible strings go through QObject::tr().

    \sa QString::fromAscii(), isNull(), {QImage#Reading and Writing
    Image Files}{Reading and Writing Image Files}
*/
#ifndef QT_NO_CAST_FROM_ASCII
QImage::QImage(const char *fileName, const char *format)
    : QPaintDevice(),
    d(nullptr)
{
    load(QString::fromAscii(fileName), format);
}
#endif

/*!
    \fn QImage::QImage(const QByteArray &data)

    Use the static fromData() function instead.

    \oldcode
        QByteArray data;
        ...
        QImage image(data);
    \newcode
        QByteArray data;
        ...
        QImage image = QImage::fromData(data);
    \endcode
*/


/*!
    Constructs a shallow copy of the given \a image.

    For more information about shallow copies, see the \l {Implicit
    Data Sharing} documentation.

    \sa copy()
*/

QImage::QImage(const QImage &image)
    : QPaintDevice(),
    d(nullptr)
{
    if (image.paintingActive()) {
        operator=(image.copy());
    } else {
        d = image.d;
        if (d)
            d->ref.ref();
    }
}


/*!
    Destroys the image and cleans up.
*/

QImage::~QImage()
{
    if (d && !d->ref.deref())
        delete d;
}

/*!
    Assigns a shallow copy of the given \a image to this image and
    returns a reference to this image.

    For more information about shallow copies, see the \l {Implicit
    Data Sharing} documentation.

    \sa copy(), QImage()
*/

QImage &QImage::operator=(const QImage &image)
{
    if (image.paintingActive()) {
        operator=(image.copy());
    } else {
        qAtomicAssign(d, image.d);
    }
    return *this;
}

/*!
    \fn void QImage::swap(QImage &other)
    \since 4.8

    Swaps image \a other with this image. This operation is very
    fast and never fails.
*/

/*!
  \internal
*/
int QImage::devType() const
{
    return QInternal::Image;
}

/*!
   Returns the image as a QVariant.
*/
QImage::operator QVariant() const
{
    return QVariant(QVariant::Image, this);
}

/*!
    \internal

    If multiple images share common data, this image makes a copy of
    the data and detaches itself from the sharing mechanism, making
    sure that this image is the only one referring to the data.

    Nothing is done if there is just a single reference.

    \sa copy(), {Implicit Data Sharing}
*/
void QImage::detach()
{
    if (d) {
        if (d->ref != 1 || d->ro_data)
            *this = copy();

        if (d)
            ++d->detach_no;
    }
}


/*!
    \fn QImage QImage::copy(int x, int y, int width, int height) const
    \overload

    The returned image is copied from the position (\a x, \a y) in
    this image, and will always have the given \a width and \a height.
    In areas beyond this image, pixels are set to 0.

*/

/*!
    \fn QImage QImage::copy(const QRect& rectangle) const

    Returns a sub-area of the image as a new image.

    The returned image is copied from the position (\a
    {rectangle}.x(), \a{rectangle}.y()) in this image, and will always
    have the size of the given \a rectangle.

    In areas beyond this image, pixels are set to 0. For 32-bit RGB
    images, this means black; for 32-bit ARGB images, this means
    transparent black; for 1-bit images, this means Qt::color0.

    If the given \a rectangle is a null rectangle the entire image is
    copied.

    \sa QImage()
*/
QImage QImage::copy(const QRect& r) const
{
    if (!d)
        return QImage();

    if (r.isNull()) {
        QImage image(d->width, d->height, d->format);
        if (image.isNull())
            return image;

        // Qt for Embedded Linux can create images with non-default bpl
        // make sure we don't crash.
        if (image.d->nbytes != d->nbytes) {
            const int bpl = qMin(bytesPerLine(), image.bytesPerLine());
            uchar* bits = image.bits();
            for (int i = 0; i < height(); i++)
                ::memcpy(QFAST_SCAN_LINE(bits, bpl, i), constScanLine(i), bpl);
        } else {
            // using bits() to detach
            ::memcpy(image.bits(), d->data, d->nbytes);
        }
        image.d->mono0 = d->mono0;
        image.d->mono1 = d->mono1;
        image.d->dpmx = d->dpmx;
        image.d->dpmy = d->dpmy;
        image.d->has_alpha_clut = d->has_alpha_clut;
        return image;
    }

    int x = r.x();
    int y = r.y();
    int w = r.width();
    int h = r.height();

    int dx = 0;
    int dy = 0;
    if (w <= 0 || h <= 0)
        return QImage();

    QImage image(w, h, d->format);
    if (image.isNull())
        return image;

    if (x < 0 || y < 0 || x + w > d->width || y + h > d->height) {
        // bitBlt will not cover entire image - clear it.
        image.fill(0);
        if (x < 0) {
            dx = -x;
            x = 0;
        }
        if (y < 0) {
            dy = -y;
            y = 0;
        }
    }

    int pixels_to_copy = qMax(w - dx, 0);
    if (x > d->width)
        pixels_to_copy = 0;
    else if (pixels_to_copy > d->width - x)
        pixels_to_copy = d->width - x;
    int lines_to_copy = qMax(h - dy, 0);
    if (y > d->height)
        lines_to_copy = 0;
    else if (lines_to_copy > d->height - y)
        lines_to_copy = d->height - y;

    bool byteAligned = true;
    if (d->format == QImage::Format_Mono || d->format == QImage::Format_MonoLSB)
        byteAligned = !(dx & 7) && !(x & 7) && !(pixels_to_copy & 7);

    if (byteAligned) {
        const uchar *src = d->data + ((x * d->depth) >> 3) + y * d->bytes_per_line;
        uchar *dest = image.d->data + ((dx * d->depth) >> 3) + dy * image.d->bytes_per_line;
        const int bytes_to_copy = (pixels_to_copy * d->depth) >> 3;
        for (int i = 0; i < lines_to_copy; ++i) {
            memcpy(dest, src, bytes_to_copy);
            src += d->bytes_per_line;
            dest += image.d->bytes_per_line;
        }
    } else if (d->format == QImage::Format_Mono) {
        const uchar *src = d->data + y * d->bytes_per_line;
        uchar *dest = image.d->data + dy * image.d->bytes_per_line;
        for (int i = 0; i < lines_to_copy; ++i) {
            for (int j = 0; j < pixels_to_copy; ++j) {
                if (src[(x + j) >> 3] & (0x80 >> ((x + j) & 7)))
                    dest[(dx + j) >> 3] |= (0x80 >> ((dx + j) & 7));
                else
                    dest[(dx + j) >> 3] &= ~(0x80 >> ((dx + j) & 7));
            }
            src += d->bytes_per_line;
            dest += image.d->bytes_per_line;
        }
    } else { // QImage::Format_MonoLSB
        Q_ASSERT(d->format == QImage::Format_MonoLSB);
        const uchar *src = d->data + y * d->bytes_per_line;
        uchar *dest = image.d->data + dy * image.d->bytes_per_line;
        for (int i = 0; i < lines_to_copy; ++i) {
            for (int j = 0; j < pixels_to_copy; ++j) {
                if (src[(x + j) >> 3] & (0x1 << ((x + j) & 7)))
                    dest[(dx + j) >> 3] |= (0x1 << ((dx + j) & 7));
                else
                    dest[(dx + j) >> 3] &= ~(0x1 << ((dx + j) & 7));
            }
            src += d->bytes_per_line;
            dest += image.d->bytes_per_line;
        }
    }

    image.d->mono0 = d->mono0;
    image.d->mono1 = d->mono1;
    image.d->dpmx = d->dpmx;
    image.d->dpmy = d->dpmy;
    image.d->has_alpha_clut = d->has_alpha_clut;
    return image;
}


/*!
    \fn bool QImage::isNull() const

    Returns true if it is a null image, otherwise returns false.

    A null image has all parameters set to zero and no allocated data.
*/
bool QImage::isNull() const
{
    return !d;
}

/*!
    \fn int QImage::width() const

    Returns the width of the image.

    \sa {QImage#Image Information}{Image Information}
*/
int QImage::width() const
{
    return d ? d->width : 0;
}

/*!
    \fn int QImage::height() const

    Returns the height of the image.

    \sa {QImage#Image Information}{Image Information}
*/
int QImage::height() const
{
    return d ? d->height : 0;
}

/*!
    \fn QSize QImage::size() const

    Returns the size of the image, i.e. its width() and height().

    \sa {QImage#Image Information}{Image Information}
*/
QSize QImage::size() const
{
    return d ? QSize(d->width, d->height) : QSize(0, 0);
}

/*!
    \fn QRect QImage::rect() const

    Returns the enclosing rectangle (0, 0, width(), height()) of the
    image.

    \sa {QImage#Image Information}{Image Information}
*/
QRect QImage::rect() const
{
    return d ? QRect(0, 0, d->width, d->height) : QRect();
}

/*!
    Returns the depth of the image.

    The image depth is the number of bits used to store a single
    pixel, also called bits per pixel (bpp).

    The supported depths are 1, 16 and 32.

    \sa bitPlaneCount(), convertToFormat(), {QImage#Image Formats}{Image Formats},
    {QImage#Image Information}{Image Information}

*/
int QImage::depth() const
{
    return d ? d->depth : 0;
}

/*!
    Sets the color table used to translate color indexes to QRgb
    values, to the specified \a colors.

    When the image is used, the color table must be large enough to
    have entries for all the pixel/index values present in the image,
    otherwise the results are undefined.

    \sa colorTable(), setColor(), {QImage#Image Transformations}{Image
    Transformations}
*/
void QImage::setColorTable(const QVector<QRgb> &colors)
{
    if (Q_UNLIKELY(colors.size() != 2)) {
        qWarning("QImage::setColorTable: Color table should have two colors");
        return;
    }
    if (!d || (d->mono0 == colors.at(0) && d->mono1 == colors.at(1)))
        return;
    detach();

    d->mono0 = colors.at(0);
    d->mono1 = colors.at(1);
    d->has_alpha_clut = false;
    if (qAlpha(d->mono0) != 255 || qAlpha(d->mono1) != 255) {
        d->has_alpha_clut = true;
    }
}

/*!
    Returns a list of the colors contained in the image's color table,
    or an empty list if the image does not have a color table

    \sa setColorTable(), color()
*/
QVector<QRgb> QImage::colorTable() const
{
    QVector<QRgb> result;
    if (!d || d->depth != 1)
        return result;
    result.append(d->mono0);
    result.append(d->mono1);
    return result;
}

/*!
    \since 4.6
    Returns the number of bytes occupied by the image data.

    \sa bytesPerLine(), bits(), {QImage#Image Information}{Image
    Information}
*/
int QImage::byteCount() const
{
    return d ? d->nbytes : 0;
}

/*!
    Returns the number of bytes per image scanline.

    This is equivalent to byteCount() / height().

    \sa scanLine()
*/
int QImage::bytesPerLine() const
{
    return (d && d->height) ? d->nbytes / d->height : 0;
}


/*!
    Returns the color in the color table at index \a i. The first
    color is at index 0.

    The colors in an image's color table are specified as ARGB
    quadruplets (QRgb). Use the qAlpha(), qRed(), qGreen(), and
    qBlue() functions to get the color value components.

    \sa setColor(), {QImage#Pixel Manipulation}{Pixel
    Manipulation}
*/
QRgb QImage::color(int i) const
{
    QRgb result(uint(-1));
    if (!d || d->depth != 1)
        return result;
    switch (i) {
        case 0: {
            result = d->mono0;
            break;
        }
        case 1: {
            result = d->mono1;
            break;
        }
    }
    return result;
}

/*!
    \fn void QImage::setColor(int index, QRgb colorValue)

    Sets the color at the given \a index in the color table, to the
    given to \a colorValue. The color value is an ARGB quadruplet.

    \sa color(), setColorTable(), {QImage#Pixel Manipulation}{Pixel
    Manipulation}
*/
void QImage::setColor(int i, QRgb c)
{
    if (!d)
        return;
    if (Q_UNLIKELY(i < 0 || d->depth > 1 || i > 1)) {
        qWarning("QImage::setColor: Index out of bound %d", i);
        return;
    }
    detach();

    switch (i) {
        case 0: {
            d->mono0 = c;
            break;
        }
        case 1: {
            d->mono1 = c;
            break;
        }
    }
    d->has_alpha_clut |= (qAlpha(c) != 255);
}

/*!
    Returns a pointer to the pixel data at the scanline with index \a
    i. The first scanline is at index 0.

    The scanline data is aligned on a 32-bit boundary.

    \warning If you are accessing 32-bpp image data, cast the returned
    pointer to \c{QRgb*} (QRgb has a 32-bit size) and use it to
    read/write the pixel value. You cannot use the \c{uchar*} pointer
    directly, because the pixel format depends on the byte order on
    the underlying platform. Use qRed(), qGreen(), qBlue(), and
    qAlpha() to access the pixels.

    \sa bytesPerLine(), bits(), {QImage#Pixel Manipulation}{Pixel
    Manipulation}, constScanLine()
*/
uchar *QImage::scanLine(int i)
{
    if (!d)
        return 0;

    detach();

    return d->data + i * d->bytes_per_line;
}

/*!
    \overload
*/
const uchar *QImage::scanLine(int i) const
{
    if (!d)
        return 0;

    Q_ASSERT(i >= 0 && i < height());
    return d->data + i * d->bytes_per_line;
}


/*!
    Returns a pointer to the pixel data at the scanline with index \a
    i. The first scanline is at index 0.

    The scanline data is aligned on a 32-bit boundary.

    Note that QImage uses \l{Implicit Data Sharing} {implicit data
    sharing}, but this function does \e not perform a deep copy of the
    shared pixel data, because the returned data is const.

    \sa scanLine(), constBits()
    \since 4.7
*/
const uchar *QImage::constScanLine(int i) const
{
    if (!d)
        return 0;

    Q_ASSERT(i >= 0 && i < height());
    return d->data + i * d->bytes_per_line;
}

/*!
    Returns a pointer to the first pixel data. This is equivalent to
    scanLine(0).

    Note that QImage uses \l{Implicit Data Sharing} {implicit data
    sharing}. This function performs a deep copy of the shared pixel
    data, thus ensuring that this QImage is the only one using the
    current return value.

    \sa scanLine(), byteCount(), constBits()
*/
uchar *QImage::bits()
{
    if (!d)
        return 0;
    detach();

    return d->data;
}

/*!
    \overload

    Note that QImage uses \l{Implicit Data Sharing} {implicit data
    sharing}, but this function does \e not perform a deep copy of the
    shared pixel data, because the returned data is const.
*/
const uchar *QImage::bits() const
{
    return d ? d->data : 0;
}


/*!
    Returns a pointer to the first pixel data.

    Note that QImage uses \l{Implicit Data Sharing} {implicit data
    sharing}, but this function does \e not perform a deep copy of the
    shared pixel data, because the returned data is const.

    \sa bits(), constScanLine()
    \since 4.7
*/
const uchar *QImage::constBits() const
{
    return d ? d->data : 0;
}

/*!
    \fn void QImage::reset()

    Resets all image parameters and deallocates the image data.

    Assign a null image instead.

    \oldcode
        QImage image;
        image.reset();
    \newcode
        QImage image;
        image = QImage();
    \endcode
*/

/*!
    \fn void QImage::fill(uint pixelValue)

    Fills the entire image with the given \a pixelValue.

    If the depth of this image is 1, only the lowest bit is used. If
    you say fill(0), fill(2), etc., the image is filled with 0s. If
    you say fill(1), fill(3), etc., the image is filled with 1s. If
     the depth is 16 the lowest 16 bits are used.

    Note: QImage::pixel() returns the color of the pixel at the given
    coordinates while QColor::pixel() returns the pixel value of the
    underlying window system (essentially an index value), so normally
    you will want to use QImage::pixel() to use a color from an
    existing image or QColor::rgb() to use a specific color.

    \sa depth(), {QImage#Image Transformations}{Image Transformations}
*/

void QImage::fill(uint pixel)
{
    if (!d)
        return;

    detach();

    if (d->depth == 1) {
        int w = d->width;
        if (d->depth == 1) {
            if (pixel & 1)
                pixel = 0xffffffff;
            else
                pixel = 0;
            w = (w + 7) / 8;
        } else {
            pixel &= 0xff;
        }
        qt_rectfill<quint8>(d->data, pixel, 0, 0, w, d->height, d->bytes_per_line);
        return;
    } else if (d->depth == 16) {
        qt_rectfill<quint16>(
            reinterpret_cast<quint16*>(d->data), qt_colorConvert<quint16, quint32>(pixel, 0),
            0, 0, d->width, d->height, d->bytes_per_line
        );
        return;
    }

    if (d->format == QImage::Format_RGB32)
        pixel |= 0xff000000;

    qt_rectfill<quint32>(
        reinterpret_cast<quint32*>(d->data), pixel,
        0, 0, d->width, d->height, d->bytes_per_line
    );
}


/*!
    \fn void QImage::fill(Qt::GlobalColor color)
    \overload
    \since 4.8

    Fills the image with the given \a color, described as a standard global
    color.
 */

void QImage::fill(Qt::GlobalColor color)
{
    fill(QColor(color));
}



/*!
    \fn void QImage::fill(const QColor &color)

    \overload

    Fills the entire image with the given \a color.

    If the depth of the image is 1, the image will be filled with 1 if
    \a color equals Qt::color1; it will otherwise be filled with 0.

    \since 4.8
*/

void QImage::fill(const QColor &color)
{
    if (!d)
        return;
    detach();

    if (d->depth == 32) {
        uint pixel = color.rgba();
        if (d->format == QImage::Format_ARGB32_Premultiplied)
            pixel = PREMUL(pixel);
        fill(pixel);

    } else if (d->depth == 16 && d->format == QImage::Format_RGB16) {
        fill((uint) color.rgba());

    } else if (d->depth == 1) {
        if (color == Qt::color1)
            fill((uint) 1);
        else
            fill((uint) 0);

    } else {
        Q_ASSERT_X(false, "QImage::fill", "internal error");
    }
}

/*!
    Inverts all pixel values in the image.

    The given invert \a mode only have a meaning when the image's
    depth is 32. The default \a mode is InvertRgb, which leaves the
    alpha channel unchanged. If the \a mode is InvertRgba, the alpha
    bits are also inverted.

    \sa {QImage#Image Transformations}{Image Transformations}
*/

void QImage::invertPixels(InvertMode mode)
{
    if (!d)
        return;

    detach();

    if (depth() != 32) {
        // number of used bytes pr line
        int bpl = (d->width * d->depth + 7) / 8;
        int pad = d->bytes_per_line - bpl;
        uchar *sl = d->data;
        for (int y=0; y<d->height; ++y) {
            for (int x=0; x<bpl; ++x)
                *sl++ ^= 0xff;
            sl += pad;
        }
    } else {
        quint32 *p = (quint32*)d->data;
        quint32 *end = (quint32*)(d->data + d->nbytes);
        uint xorbits = (mode == InvertRgba) ? 0xffffffff : 0x00ffffff;
        while (p < end)
            *p++ ^= xorbits;
    }
}

/*!
    \fn void QImage::invertPixels(bool invertAlpha)

    Use the invertPixels() function that takes a QImage::InvertMode
    parameter instead.
*/

/*!
    Returns the format of the image.

    \sa {QImage#Image Formats}{Image Formats}
*/
QImage::Format QImage::format() const
{
    return d ? d->format : QImage::Format_Invalid;
}

QImage::Format QImage::systemFormat()
{
#if defined(Q_WS_X11)
    if (QX11Info::appDepth() == 16)
        return QImage::Format_RGB16;
#endif
    return QImage::Format_RGB32;
}


/*****************************************************************************
  Internal routines for converting image depth.
 *****************************************************************************/

typedef void (QT_FASTCALL *Image_Converter)(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags);

static void QT_FASTCALL swap_bit_order(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    Q_ASSERT(src->format == QImage::Format_Mono || src->format == QImage::Format_MonoLSB);
    Q_ASSERT(dest->format == QImage::Format_Mono || dest->format == QImage::Format_MonoLSB);
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);
    Q_ASSERT(src->nbytes == dest->nbytes);
    Q_ASSERT(src->bytes_per_line == dest->bytes_per_line);

    dest->mono0 = src->mono0;
    dest->mono1 = src->mono1;

    const int src_bpl = (src->width + 7) / 8;
    const int src_pad = src->bytes_per_line - src_bpl;
    const int dest_bpl = (dest->width + 7) / 8;
    const int dest_pad = dest->bytes_per_line - dest_bpl;
    const uchar *src_data = src->data;
    uchar *dest_data = dest->data;
    for (int i = 0; i < src->height; ++i) {
        for (int x = 0; x < src_bpl; ++x) {
            *dest_data = bitflip[*src_data];
            ++src_data;
            ++dest_data;
        }
        src_data += src_pad;
        dest_data += dest_pad;
    }
}

static void QT_FASTCALL mask_alpha_converter(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);

    const int src_pad = (src->bytes_per_line >> 2) - src->width;
    const int dest_pad = (dest->bytes_per_line >> 2) - dest->width;
    const uint *src_data = (const uint *)src->data;
    uint *dest_data = (uint *)dest->data;

    for (int i = 0; i < src->height; ++i) {
        const uint *end = src_data + src->width;
        while (src_data < end) {
            *dest_data = *src_data | 0xff000000;
            ++src_data;
            ++dest_data;
        }
        src_data += src_pad;
        dest_data += dest_pad;
    }
}

static QVector<QRgb> fix_color_table(const QVector<QRgb> &ctbl, QImage::Format format)
{
    if (format == QImage::Format_RGB32) {
        // check if the color table has alpha
        QVector<QRgb> colorTable(ctbl.size());
        QRgb* colorTableData = colorTable.data();
        for (int i = 0; i < ctbl.size(); ++i) {
            if (qAlpha(colorTableData[i]) != 255) {
                colorTableData[i] = ctbl.at(i) | 0xff000000;
            } else {
                colorTableData[i] = ctbl.at(i);
            }
        }
        return colorTable;
    } else if (format == QImage::Format_ARGB32_Premultiplied) {
        // check if the color table has alpha
        QVector<QRgb> colorTable(ctbl.size());
        QRgb* colorTableData = colorTable.data();
        for (int i = 0; i < ctbl.size(); ++i) {
            colorTableData[i] = PREMUL(ctbl.at(i));
        }
        return colorTable;
    }
    return ctbl;
}

//
// dither_to_1:  Uses selected dithering algorithm.
//

static void dither_to_Mono(QImageData *dst, const QImageData *src,
                           Qt::ImageConversionFlags flags, bool fromalpha)
{
    Q_ASSERT(src->width == dst->width);
    Q_ASSERT(src->height == dst->height);
    Q_ASSERT(src->depth == 32);
    Q_ASSERT(dst->format == QImage::Format_Mono || dst->format == QImage::Format_MonoLSB);

    dst->mono0 = QRgb(0xffffffff);
    dst->mono1 = QRgb(0xff000000);

    enum { Threshold, Ordered, Diffuse } dithermode;

    if (fromalpha) {
        if ((flags & Qt::AlphaDither_Mask) == Qt::DiffuseAlphaDither)
            dithermode = Diffuse;
        else if ((flags & Qt::AlphaDither_Mask) == Qt::OrderedAlphaDither)
            dithermode = Ordered;
        else
            dithermode = Threshold;
    } else {
        if ((flags & Qt::Dither_Mask) == Qt::ThresholdDither)
            dithermode = Threshold;
        else if ((flags & Qt::Dither_Mask) == Qt::OrderedDither)
            dithermode = Ordered;
        else
            dithermode = Diffuse;
    }

    uchar *dst_data = dst->data;
    const uchar *src_data = src->data;

    switch (dithermode) {
        case Diffuse: {
            QVarLengthArray<int> lineBuffer(src->width * 2);
            int *line1 = lineBuffer.data();
            int *line2 = lineBuffer.data() + src->width;
            int bmwidth = (src->width+7)/8;

            int *b1, *b2;
            int wbytes = src->width * (src->depth/8);
            const uchar *p = src->data;
            const uchar *end = p + wbytes;
            b2 = line2;
            // 32 bit image
            if (fromalpha) {
                while (p < end) {
                    *b2++ = 255 - (*(uint*)p >> 24);
                    p += 4;
                }
            } else {
                while (p < end) {
                    *b2++ = qGray(*(uint*)p);
                    p += 4;
                }
            }
            for (int y=0; y<src->height; y++) {                        // for each scan line...
                int *tmp = line1; line1 = line2; line2 = tmp;
                bool not_last_line = y < src->height - 1;
                if (not_last_line) {                // calc. grayvals for next line
                    p = src->data + (y+1)*src->bytes_per_line;
                    end = p + wbytes;
                    b2 = line2;
                    // 24 bit image
                    if (fromalpha) {
                        while (p < end) {
                            *b2++ = 255 - (*(uint*)p >> 24);
                            p += 4;
                        }
                    } else {
                        while (p < end) {
                            *b2++ = qGray(*(uint*)p);
                            p += 4;
                        }
                    }
                }

                int err;
                uchar *p = dst->data + y*dst->bytes_per_line;
                memset(p, 0, bmwidth);
                b1 = line1;
                b2 = line2;
                int bit = 7;
                for (int x=1; x<=src->width; x++) {
                    if (*b1 < 128) {                // black pixel
                        err = *b1++;
                        *p |= 1 << bit;
                    } else {                        // white pixel
                        err = *b1++ - 255;
                    }
                    if (bit == 0) {
                        p++;
                        bit = 7;
                    } else {
                        bit--;
                    }
                    if (x < src->width)
                        *b1 += (err*7)>>4;                // spread error to right pixel
                    if (not_last_line) {
                        b2[0] += (err*5)>>4;        // pixel below
                        if (x > 1)
                            b2[-1] += (err*3)>>4;        // pixel below left
                        if (x < src->width)
                            b2[1] += err>>4;        // pixel below right
                    }
                    b2++;
                }
            }
            break;
        }
        case Ordered: {
            memset(dst->data, 0, dst->nbytes);
            for (int i=0; i<src->height; i++) {
                const uint *p = (const uint *)src_data;
                const uint *end = p + src->width;
                uchar *m = dst_data;
                int bit = 7;
                int j = 0;
                if (fromalpha) {
                    while (p < end) {
                        if ((*p++ >> 24) >= qt_bayer_matrix[j++&15][i&15])
                            *m |= 1 << bit;
                        if (bit == 0) {
                            m++;
                            bit = 7;
                        } else {
                            bit--;
                        }
                    }
                } else {
                    while (p < end) {
                        if ((uint)qGray(*p++) < qt_bayer_matrix[j++&15][i&15])
                            *m |= 1 << bit;
                        if (bit == 0) {
                            m++;
                            bit = 7;
                        } else {
                            bit--;
                        }
                    }
                }
                dst_data += dst->bytes_per_line;
                src_data += src->bytes_per_line;
            }
            break;
        }
        default: {
            // Threshold:
            memset(dst->data, 0, dst->nbytes);
            for (int i=0; i<src->height; i++) {
                const uint *p = (const uint *)src_data;
                const uint *end = p + src->width;
                uchar *m = dst_data;
                int bit = 7;
                if (fromalpha) {
                    while (p < end) {
                        if ((*p++ >> 24) >= 128)
                            *m |= 1 << bit;        // Set mask "on"
                        if (bit == 0) {
                            m++;
                            bit = 7;
                        } else {
                            bit--;
                        }
                    }
                } else {
                    while (p < end) {
                        if (qGray(*p++) < 128)
                            *m |= 1 << bit;        // Set pixel "black"
                        if (bit == 0) {
                            m++;
                            bit = 7;
                        } else {
                            bit--;
                        }
                    }
                }
                dst_data += dst->bytes_per_line;
                src_data += src->bytes_per_line;
            }
        }
    }

    if (dst->format == QImage::Format_MonoLSB) {
        // need to swap bit order
        uchar *sl = dst->data;
        int bpl = (dst->width + 7) * dst->depth / 8;
        int pad = dst->bytes_per_line - bpl;
        for (int y=0; y<dst->height; ++y) {
            for (int x=0; x<bpl; ++x) {
                *sl = bitflip[*sl];
                ++sl;
            }
            sl += pad;
        }
    }
}

static void QT_FASTCALL convert_X_to_Mono(QImageData *dst, const QImageData *src, Qt::ImageConversionFlags flags)
{
    dither_to_Mono(dst, src, flags, false);
}

static void QT_FASTCALL convert_Mono_to_X32(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    Q_ASSERT(src->format == QImage::Format_Mono || src->format == QImage::Format_MonoLSB);
    Q_ASSERT(dest->format == QImage::Format_RGB32
             || dest->format == QImage::Format_ARGB32
             || dest->format == QImage::Format_ARGB32_Premultiplied);
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);

    const QVector<QRgb> srcColorTable = { src->mono0, src->mono1 };
    QVector<QRgb> colorTable = fix_color_table(srcColorTable, dest->format);

    // Default to black / white colors
    if (colorTable.size() < 2) {
        if (colorTable.size() == 0)
            colorTable << 0xff000000;
        colorTable << 0xffffffff;
    }

    const uchar *src_data = src->data;
    uchar *dest_data = dest->data;
    if (src->format == QImage::Format_Mono) {
        for (int y = 0; y < dest->height; y++) {
            uint *p = (uint *)dest_data;
            for (int x = 0; x < dest->width; x++)
                *p++ = colorTable.at((src_data[x>>3] >> (7 - (x & 7))) & 1);

            src_data += src->bytes_per_line;
            dest_data += dest->bytes_per_line;
        }
    } else {
        for (int y = 0; y < dest->height; y++) {
            uint *p = (uint *)dest_data;
            for (int x = 0; x < dest->width; x++)
                *p++ = colorTable.at((src_data[x>>3] >> (x & 7)) & 1);

            src_data += src->bytes_per_line;
            dest_data += dest->bytes_per_line;
        }
    }
}

static void QT_FASTCALL convert_RGB16_to_RGB32(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    uchar *d = dest->data;
    const uchar *s = src->data;
    for (int i = 0; i < src->height; ++i) {
        qt_memconvert<quint32,quint16>((quint32*)d, (const quint16*)s, src->width);
        d += dest->bytes_per_line;
        s += src->bytes_per_line;
    }
}

static void QT_FASTCALL convert_RGB16_to_Mono(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags flags)
{
    Q_ASSERT(src->format == QImage::Format_RGB16);
    Q_ASSERT(dest->format == QImage::Format_Mono || dest->format == QImage::Format_MonoLSB);
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);
    QScopedPointer<QImageData> tmp(QImageData::create(QSize(src->width, src->height), QImage::Format_ARGB32));
    convert_RGB16_to_RGB32(tmp.data(), src, flags);
    dither_to_Mono(dest, tmp.data(), flags, false);
}

static void QT_FASTCALL convert_RGB32_to_RGB16(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    uchar *d = dest->data;
    const uchar *s = src->data;
    for (int i = 0; i < src->height; ++i) {
        qt_memconvert<quint16,quint32>((quint16*)d, (const quint32*)s, src->width);
        d += dest->bytes_per_line;
        s += src->bytes_per_line;
    }
}

static void QT_FASTCALL convert_ARGB_to_ARGB_PM(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    Q_ASSERT(src->format == QImage::Format_ARGB32);
    Q_ASSERT(dest->format == QImage::Format_ARGB32_Premultiplied);
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);

    const int src_pad = (src->bytes_per_line >> 2) - src->width;
    const int dest_pad = (dest->bytes_per_line >> 2) - dest->width;
    const QRgb *src_data = (const QRgb *) src->data;
    QRgb *dest_data = (QRgb *) dest->data;

    for (int i = 0; i < src->height; ++i) {
        const QRgb *end = src_data + src->width;
        while (src_data < end) {
            *dest_data = PREMUL(*src_data);
            ++src_data;
            ++dest_data;
        }
        src_data += src_pad;
        dest_data += dest_pad;
    }
}

static void QT_FASTCALL convert_ARGB_PM_to_ARGB(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    Q_ASSERT(src->format == QImage::Format_ARGB32_Premultiplied);
    Q_ASSERT(dest->format == QImage::Format_ARGB32);
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);

    const int src_pad = (src->bytes_per_line >> 2) - src->width;
    const int dest_pad = (dest->bytes_per_line >> 2) - dest->width;
    const QRgb *src_data = (const QRgb *) src->data;
    QRgb *dest_data = (QRgb *) dest->data;

    for (int i = 0; i < src->height; ++i) {
        const QRgb *end = src_data + src->width;
        while (src_data < end) {
            *dest_data = INV_PREMUL(*src_data);
            ++src_data;
            ++dest_data;
        }
        src_data += src_pad;
        dest_data += dest_pad;
    }
}

static void QT_FASTCALL convert_ARGB_PM_to_RGB(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags)
{
    Q_ASSERT(src->format == QImage::Format_ARGB32_Premultiplied);
    Q_ASSERT(dest->format == QImage::Format_RGB32);
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);

    const int src_pad = (src->bytes_per_line >> 2) - src->width;
    const int dest_pad = (dest->bytes_per_line >> 2) - dest->width;
    const QRgb *src_data = (const QRgb *) src->data;
    QRgb *dest_data = (QRgb *) dest->data;

    for (int i = 0; i < src->height; ++i) {
        const QRgb *end = src_data + src->width;
        while (src_data < end) {
            *dest_data = 0xff000000 | INV_PREMUL(*src_data);
            ++src_data;
            ++dest_data;
        }
        src_data += src_pad;
        dest_data += dest_pad;
    }
}

static void QT_FASTCALL convert_ARGB_to_RGB16(QImageData *dest, const QImageData *src, Qt::ImageConversionFlags flags)
{
    Q_ASSERT(src->format == QImage::Format_ARGB32_Premultiplied);
    Q_ASSERT(dest->format == QImage::Format_RGB16);
    Q_ASSERT(src->width == dest->width);
    Q_ASSERT(src->height == dest->height);
    QScopedPointer<QImageData> tmp(QImageData::create(QSize(src->width, src->height), QImage::Format_ARGB32));
    convert_ARGB_PM_to_ARGB(tmp.data(), src, flags);
    convert_RGB32_to_RGB16(dest, tmp.data(), flags);
}

static void QT_FASTCALL convert_ARGB_PM_to_Mono(QImageData *dst, const QImageData *src, Qt::ImageConversionFlags flags)
{
    QScopedPointer<QImageData> tmp(QImageData::create(QSize(src->width, src->height), QImage::Format_ARGB32));
    convert_ARGB_PM_to_ARGB(tmp.data(), src, flags);
    dither_to_Mono(dst, tmp.data(), flags, false);
}

/*
    QImage::Format_Invalid,
    QImage::Format_Mono,
    QImage::Format_MonoLSB,
    QImage::Format_RGB32,
    QImage::Format_ARGB32,
    QImage::Format_ARGB32_Premultiplied,
    QImage::Format_RGB16,
*/


// first index source, second dest
static const Image_Converter converter_map[QImage::NImageFormats][QImage::NImageFormats] =
{
    {
        0, 0, 0, 0, 0, 0, 0
    },
    {
        0,
        0,
        swap_bit_order,
        convert_Mono_to_X32,
        convert_Mono_to_X32,
        convert_Mono_to_X32,
        0
    }, // QImage::Format_Mono

    {
        0,
        swap_bit_order,
        0,
        convert_Mono_to_X32,
        convert_Mono_to_X32,
        convert_Mono_to_X32,
        0
    }, // QImage::Format_MonoLSB

    {
        0,
        convert_X_to_Mono,
        convert_X_to_Mono,
        0,
        mask_alpha_converter,
        mask_alpha_converter,
        convert_RGB32_to_RGB16
    }, // QImage::Format_RGB32

    {
        0,
        convert_X_to_Mono,
        convert_X_to_Mono,
        mask_alpha_converter,
        0,
        convert_ARGB_to_ARGB_PM,
        convert_RGB32_to_RGB16
    }, // QImage::Format_ARGB32

    {
        0,
        convert_ARGB_PM_to_Mono,
        convert_ARGB_PM_to_Mono,
        convert_ARGB_PM_to_RGB,
        convert_ARGB_PM_to_ARGB,
        0,
        convert_ARGB_to_RGB16
    },  // QImage::Format_ARGB32_Premultiplied

    {
        0,
        convert_RGB16_to_Mono,
        convert_RGB16_to_Mono,
        convert_RGB16_to_RGB32,
        convert_RGB16_to_RGB32,
        convert_RGB16_to_RGB32,
        0
    }, // QImage::Format_RGB16
};

/*!
    Returns a copy of the image in the given \a format.

    The specified image conversion \a flags control how the image data
    is handled during the conversion process.

    \sa {QImage#Image Format}{Image Format}
*/
QImage QImage::convertToFormat(Format format, Qt::ImageConversionFlags flags) const
{
    if (!d || d->format == format)
        return *this;

    if (format == QImage::Format_Invalid || d->format == QImage::Format_Invalid)
        return QImage();

    const Image_Converter converter = converter_map[d->format][format];
    Q_ASSERT(converter);
    QImage image(d->width, d->height, format);
    QIMAGE_SANITYCHECK_MEMORY(image);

    image.d->dpmx = dotsPerMeterX();
    image.d->dpmy = dotsPerMeterY();

    converter(image.d, d, flags);
    return image;
}

/*!
    \fn bool QImage::valid(const QPoint &pos) const

    Returns true if \a pos is a valid coordinate pair within the
    image; otherwise returns false.

    \sa rect(), QRect::contains()
*/

/*!
    \overload

    Returns true if QPoint(\a x, \a y) is a valid coordinate pair
    within the image; otherwise returns false.
*/
bool QImage::valid(int x, int y) const
{
    return d
        && x >= 0 && x < d->width
        && y >= 0 && y < d->height;
}

/*!
    \fn QRgb QImage::pixel(const QPoint &position) const

    Returns the color of the pixel at the given \a position.

    If the \a position is not valid, the results are undefined.

    \warning This function is expensive when used for massive pixel
    manipulations.

    \sa setPixel(), valid(), {QImage#Pixel Manipulation}{Pixel
    Manipulation}
*/

/*!
    \overload

    Returns the color of the pixel at coordinates (\a x, \a y).
*/
QRgb QImage::pixel(int x, int y) const
{
    if (Q_UNLIKELY(!d || x < 0 || x >= d->width || y < 0 || y >= d->height)) {
        qWarning("QImage::pixel: coordinate (%d,%d) out of range", x, y);
        return 12345;
    }
    const uchar *s = constScanLine(y);
    switch(d->format) {
        case QImage::Format_Mono: {
            const int index = ((*(s + (x >> 3)) >> (7- (x & 7))) & 1);
            if (index == 0)
                return d->mono0;
            return d->mono1;
        }
        case QImage::Format_MonoLSB: {
            const int index = ((*(s + (x >> 3)) >> (x & 7)) & 1);
            if (index == 0)
                return d->mono0;
            return d->mono1;
        }
        case QImage::Format_RGB16: {
            return qt_colorConvert<quint32, quint16>(reinterpret_cast<const quint16*>(s)[x], 0);
        }
        default: {
            return ((QRgb*)s)[x];
        }
    }
}


/*!
    \fn void QImage::setPixel(const QPoint &position, QRgb rgb)

    Sets the pixel index or color at the given \a position to \a rgb.

    If \a position is not a valid coordinate pair in the image, the
    result is undefined.

    If the image's format is monochrome, the given \a rgb value must
    be one of the colors in the image's color table.

    \warning This function is expensive due to the call of the internal
    \c{detach()} function called within; if performance is a concern, we
    recommend the use of \l{QImage::}{scanLine()} to access pixel data
    directly.

    \sa pixel(), {QImage#Pixel Manipulation}{Pixel Manipulation}
*/

/*!
    \overload

    Sets the pixel index or color at (\a x, \a y) to \a rgb.
*/
void QImage::setPixel(int x, int y, QRgb rgb)
{
    if (Q_UNLIKELY(!d || x < 0 || x >= width() || y < 0 || y >= height())) {
        qWarning("QImage::setPixel: coordinate (%d,%d) out of range", x, y);
        return;
    }
    // detach is called from within scanLine
    uchar * s = scanLine(y);
    if (Q_UNLIKELY(!s)) {
        qWarning("setPixel: Out of memory");
        return;
    }
    switch(d->format) {
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB: {
            if (Q_UNLIKELY(rgb != d->mono0 && rgb != d->mono1)) {
                qWarning("QImage::setPixel: Invalid color %d", rgb);
            } else if (format() == Format_MonoLSB) {
                if (rgb == d->mono0)
                    *(s + (x >> 3)) &= ~(1 << (x & 7));
                else
                    *(s + (x >> 3)) |= (1 << (x & 7));
            } else {
                if (rgb == d->mono0)
                    *(s + (x >> 3)) &= ~(1 << (7-(x & 7)));
                else
                    *(s + (x >> 3)) |= (1 << (7-(x & 7)));
            }
            break;
        }
        case QImage::Format_RGB32: {
            //make sure alpha is 255, we depend on it in qdrawhelper for cases
            // when image is set as a texture pattern on a qbrush
            ((uint *)s)[x] = (rgb | 0xff000000);
            break;
        }
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            ((uint *)s)[x] = rgb;
            break;
        }
        case QImage::Format_RGB16: {
            ((quint16 *)s)[x] = qt_colorConvert<quint16, quint32>(quint32(rgb), 0);
            break;
        }
        case QImage::Format_Invalid:
        case QImage::NImageFormats: {
            Q_ASSERT(false);
        }
    }
}

/*!
    Returns true if all the colors in the image are shades of gray
    (i.e. their red, green and blue components are equal); otherwise
    false.

    Note that this function is slow for images without color table.

    \sa isGrayscale()
*/
bool QImage::allGray() const
{
    if (!d)
        return true;

    if (d->depth == 32) {
        int p = width()*height();
        const QRgb* b = (const QRgb*)constBits();
        while (p--)
            if (!qIsGray(*b++))
                return false;
    } else if (d->depth == 16) {
        int p = width()*height();
        const ushort* b = (const ushort *)constBits();
        while (p--)
            if (!qIsGray(qt_colorConvert<quint32, quint16>(*b++, 0)))
                return false;
    } else {
        if (!qIsGray(d->mono0))
            return false;
        if (!qIsGray(d->mono1))
            return false;
    }
    return true;
}

/*!
    For 32-bit images, this function is equivalent to allGray()
    otherwise returns false.

    \sa allGray(), {QImage#Image Formats}{Image Formats}
*/
bool QImage::isGrayscale() const
{
    if (!d)
        return false;

    switch (depth()) {
        case 32:
        case 16: {
            return allGray();
        }
    }
    return false;
}


/*!
    \fn QImage QImage::smoothScale(int width, int height, Qt::AspectRatioMode mode) const

    Use scaled() instead.

    \oldcode
        QImage image;
        image.smoothScale(width, height, mode);
    \newcode
        QImage image;
        image.scaled(width, height, mode, Qt::SmoothTransformation);
    \endcode
*/

/*!
    \fn QImage QImage::smoothScale(const QSize &size, Qt::AspectRatioMode mode) const
    \overload

    Use scaled() instead.

    \oldcode
        QImage image;
        image.smoothScale(size, mode);
    \newcode
        QImage image;
        image.scaled(size, mode, Qt::SmoothTransformation);
    \endcode
*/

/*!
    \fn QImage QImage::scaled(int width, int height, Qt::AspectRatioMode aspectRatioMode,
                             Qt::TransformationMode transformMode) const
    \overload

    Returns a copy of the image scaled to a rectangle with the given
    \a width and \a height according to the given \a aspectRatioMode
    and \a transformMode.

    If either the \a width or the \a height is zero or negative, this
    function returns a null image.
*/

/*!
    \fn QImage QImage::scaled(const QSize &size, Qt::AspectRatioMode aspectRatioMode,
                             Qt::TransformationMode transformMode) const

    Returns a copy of the image scaled to a rectangle defined by the
    given \a size according to the given \a aspectRatioMode and \a
    transformMode.

    \image qimage-scaling.png

    \list
    \i If \a aspectRatioMode is Qt::IgnoreAspectRatio, the image
       is scaled to \a size.
    \i If \a aspectRatioMode is Qt::KeepAspectRatio, the image is
       scaled to a rectangle as large as possible inside \a size, preserving the aspect ratio.
    \i If \a aspectRatioMode is Qt::KeepAspectRatioByExpanding,
       the image is scaled to a rectangle as small as possible
       outside \a size, preserving the aspect ratio.
    \endlist

    If the given \a size is empty, this function returns a null image.

    \sa isNull(), {QImage#Image Transformations}{Image
    Transformations}
*/
QImage QImage::scaled(const QSize& s, Qt::AspectRatioMode aspectMode, Qt::TransformationMode mode) const
{
    if (Q_UNLIKELY(!d)) {
        qWarning("QImage::scaled: Image is a null image");
        return QImage();
    }
    if (s.isEmpty())
        return QImage();

    QSize newSize = size();
    newSize.scale(s, aspectMode);
    newSize.rwidth() = qMax(newSize.width(), 1);
    newSize.rheight() = qMax(newSize.height(), 1);
    if (newSize == size())
        return *this;

    QTransform wm = QTransform::fromScale((qreal)newSize.width() / width(), (qreal)newSize.height() / height());
    return transformed(wm, mode);
}

/*!
    \fn QImage QImage::scaledToWidth(int width, Qt::TransformationMode mode) const

    Returns a scaled copy of the image. The returned image is scaled
    to the given \a width using the specified transformation \a
    mode.

    This function automatically calculates the height of the image so
    that its aspect ratio is preserved.

    If the given \a width is 0 or negative, a null image is returned.

    \sa {QImage#Image Transformations}{Image Transformations}
*/
QImage QImage::scaledToWidth(int w, Qt::TransformationMode mode) const
{
    if (Q_UNLIKELY(!d)) {
        qWarning("QImage::scaleWidth: Image is a null image");
        return QImage();
    }
    if (w <= 0)
        return QImage();

    qreal factor = (qreal) w / width();
    QTransform wm = QTransform::fromScale(factor, factor);
    return transformed(wm, mode);
}

/*!
    \fn QImage QImage::scaledToHeight(int height, Qt::TransformationMode mode) const

    Returns a scaled copy of the image. The returned image is scaled
    to the given \a height using the specified transformation \a
    mode.

    This function automatically calculates the width of the image so that
    the ratio of the image is preserved.

    If the given \a height is 0 or negative, a null image is returned.

    \sa {QImage#Image Transformations}{Image Transformations}
*/
QImage QImage::scaledToHeight(int h, Qt::TransformationMode mode) const
{
    if (Q_UNLIKELY(!d)) {
        qWarning("QImage::scaleHeight: Image is a null image");
        return QImage();
    }
    if (h <= 0)
        return QImage();

    qreal factor = (qreal) h / height();
    QTransform wm = QTransform::fromScale(factor, factor);
    return transformed(wm, mode);
}


/*!
    \fn QMatrix QImage::trueMatrix(const QMatrix &matrix, int width, int height)

    Returns the actual matrix used for transforming an image with the
    given \a width, \a height and \a matrix.

    When transforming an image using the transformed() function, the
    transformation matrix is internally adjusted to compensate for
    unwanted translation, i.e. transformed() returns the smallest
    image containing all transformed points of the original image.
    This function returns the modified matrix, which maps points
    correctly from the original image into the new image.

    \sa transformed(), {QImage#Image Transformations}{Image
    Transformations}
*/
QMatrix QImage::trueMatrix(const QMatrix &matrix, int w, int h)
{
    return trueMatrix(QTransform(matrix), w, h).toAffine();
}

/*!
    Returns a copy of the image that is transformed using the given
    transformation \a matrix and transformation \a mode.

    The transformation \a matrix is internally adjusted to compensate
    for unwanted translation; i.e. the image produced is the smallest
    image that contains all the transformed points of the original
    image. Use the trueMatrix() function to retrieve the actual matrix
    used for transforming an image.

    \sa trueMatrix(), {QImage#Image Transformations}{Image
    Transformations}
*/
QImage QImage::transformed(const QMatrix &matrix, Qt::TransformationMode mode) const
{
    return transformed(QTransform(matrix), mode);
}

/*!
    Builds and returns a 1-bpp mask from the alpha buffer in this
    image. Returns a null image if the image's format is
    QImage::Format_RGB32.

    The \a flags argument is a bitwise-OR of the
    Qt::ImageConversionFlags, and controls the conversion
    process. Passing 0 for flags sets all the default options.

    The returned image has little-endian bit order (i.e. the image's
    format is QImage::Format_MonoLSB), which you can convert to
    big-endian (QImage::Format_Mono) using the convertToFormat()
    function.

    \sa createHeuristicMask(), {QImage#Image Transformations}{Image
    Transformations}
*/
QImage QImage::createAlphaMask(Qt::ImageConversionFlags flags) const
{
    if (!d || d->format == QImage::Format_RGB32 || d->format == QImage::Format_RGB16)
        return QImage();

    QImage source(*this);
    if (d->depth != 32) {
        source = source.convertToFormat(QImage::Format_ARGB32);
    }

    QImage mask(d->width, d->height, QImage::Format_MonoLSB);
    if (!mask.isNull()) {
        dither_to_Mono(mask.d, source.d, flags, true);
    }
    return mask;
}

#ifndef QT_NO_IMAGE_HEURISTIC_MASK
/*!
    Creates and returns a 1-bpp heuristic mask for this image.

    The function works by selecting a color from one of the corners,
    then chipping away pixels of that color starting at all the edges.
    The four corners vote for which color is to be masked away. In
    case of a draw (this generally means that this function is not
    applicable to the image), the result is arbitrary.

    The returned image has little-endian bit order (i.e. the image's
    format is QImage::Format_MonoLSB), which you can convert to
    big-endian (QImage::Format_Mono) using the convertToFormat()
    function.

    If \a clipTight is true (the default) the mask is just large
    enough to cover the pixels; otherwise, the mask is larger than the
    data pixels.

    Note that this function disregards the alpha buffer.

    \sa createAlphaMask(), {QImage#Image Transformations}{Image
    Transformations}
*/

QImage QImage::createHeuristicMask(bool clipTight) const
{
    if (!d)
        return QImage();

    if (d->depth != 32) {
        QImage img32 = convertToFormat(QImage::Format_RGB32);
        return img32.createHeuristicMask(clipTight);
    }

#define PIX(x,y)  (*((const QRgb*)constScanLine(y)+x) & 0x00ffffff)

    int w = width();
    int h = height();
    QImage m(w, h, QImage::Format_MonoLSB);
    QIMAGE_SANITYCHECK_MEMORY(m);
    m.setColorTable(monoColorTable());
    m.fill(0xff);

    QRgb background = PIX(0,0);
    if (background != PIX(w-1,0) &&
         background != PIX(0,h-1) &&
         background != PIX(w-1,h-1)) {
        background = PIX(w-1,0);
        if (background != PIX(w-1,h-1) &&
             background != PIX(0,h-1) &&
             PIX(0,h-1) == PIX(w-1,h-1)) {
            background = PIX(w-1,h-1);
        }
    }

    int x,y;
    bool done = false;
    uchar *ypp, *ypc, *ypn;
    while(!done) {
        done = true;
        ypn = m.scanLine(0);
        ypc = 0;
        for (y = 0; y < h; y++) {
            ypp = ypc;
            ypc = ypn;
            ypn = (y == h-1) ? 0 : m.scanLine(y+1);
            const QRgb *p = (const QRgb *)constScanLine(y);
            for (x = 0; x < w; x++) {
                // slowness here - it's possible to do six of these tests
                // together in one go. oh well.
                if ((x == 0 || y == 0 || x == w-1 || y == h-1 ||
                       !(*(ypc + ((x-1) >> 3)) & (1 << ((x-1) & 7))) ||
                       !(*(ypc + ((x+1) >> 3)) & (1 << ((x+1) & 7))) ||
                       !(*(ypp + (x     >> 3)) & (1 << (x     & 7))) ||
                       !(*(ypn + (x     >> 3)) & (1 << (x     & 7)))) &&
                     (       (*(ypc + (x     >> 3)) & (1 << (x     & 7)))) &&
                     ((*p & 0x00ffffff) == background)) {
                    done = false;
                    *(ypc + (x >> 3)) &= ~(1 << (x & 7));
                }
                p++;
            }
        }
    }

    if (!clipTight) {
        ypn = m.scanLine(0);
        ypc = 0;
        for (y = 0; y < h; y++) {
            ypp = ypc;
            ypc = ypn;
            ypn = (y == h-1) ? 0 : m.scanLine(y+1);
            const QRgb *p = (const QRgb *)constScanLine(y);
            for (x = 0; x < w; x++) {
                if ((*p & 0x00ffffff) != background) {
                    if (x > 0)
                        *(ypc + ((x-1) >> 3)) |= (1 << ((x-1) & 7));
                    if (x < w-1)
                        *(ypc + ((x+1) >> 3)) |= (1 << ((x+1) & 7));
                    if (y > 0)
                        *(ypp + (x >> 3)) |= (1 << (x & 7));
                    if (y < h-1)
                        *(ypn + (x >> 3)) |= (1 << (x & 7));
                }
                p++;
            }
        }
    }

#undef PIX

    return m;
}
#endif //QT_NO_IMAGE_HEURISTIC_MASK

/*!
    Creates and returns a mask for this image based on the given \a
    color value. If the \a mode is MaskInColor (the default value),
    all pixels matching \a color will be opaque pixels in the mask. If
    \a mode is MaskOutColor, all pixels matching the given color will
    be transparent.

    \sa createAlphaMask(), createHeuristicMask()
*/

QImage QImage::createMaskFromColor(QRgb color, Qt::MaskMode mode) const
{
    if (!d)
        return QImage();
    QImage maskImage(size(), QImage::Format_MonoLSB);
    QIMAGE_SANITYCHECK_MEMORY(maskImage);
    maskImage.fill(0);
    uchar *s = maskImage.bits();
    const int bpl = maskImage.bytesPerLine();

    if (depth() == 32) {
        for (int h = 0; h < d->height; h++) {
            const uint *sl = (const uint *) constScanLine(h);
            for (int w = 0; w < d->width; w++) {
                if (sl[w] == color)
                    *(s + (w >> 3)) |= (1 << (w & 7));
            }
            s += bpl;
        }
    } else {
        for (int h = 0; h < d->height; h++) {
            for (int w = 0; w < d->width; w++) {
                if ((uint) pixel(w, h) == color)
                    *(s + (w >> 3)) |= (1 << (w & 7));
            }
            s += bpl;
        }
    }
    if (mode == Qt::MaskOutColor)
        maskImage.invertPixels();
    return maskImage;
}


/*!
    \fn QImage QImage::mirror(bool horizontal, bool vertical) const

    Use mirrored() instead.
*/

/*!
    Returns a mirror of the image, mirrored in the horizontal and/or
    the vertical direction depending on whether \a horizontal and \a
    vertical are set to true or false.

    Note that the original image is not changed.

    \sa {QImage#Image Transformations}{Image Transformations}
*/
QImage QImage::mirrored(bool horizontal, bool vertical) const
{
    if (!d)
        return QImage();

    if ((d->width <= 1 && d->height <= 1) || (!horizontal && !vertical))
        return *this;

    QTransform transform;
    if (horizontal) {
        transform.rotate(-180.0, Qt::YAxis);
    }
    if (vertical) {
        transform.rotate(-180.0, Qt::XAxis);
    }

    return transformed(transform);
}

/*!
    \fn QImage QImage::swapRGB() const

    Use rgbSwapped() instead.

    \omit
    Returns a QImage in which the values of the red and blue
    components of all pixels have been swapped, effectively converting
    an RGB image to an BGR image. The original QImage is not changed.
    \endomit
*/

/*!
    Returns a QImage in which the values of the red and blue
    components of all pixels have been swapped, effectively converting
    an RGB image to an BGR image.

    The original QImage is not changed.

    \sa {QImage#Image Transformations}{Image Transformations}
*/
QImage QImage::rgbSwapped() const
{
    if (isNull())
        return *this;
    QImage res;
    switch (d->format) {
        case QImage::Format_Invalid:
        case QImage::NImageFormats: {
            Q_ASSERT(false);
            break;
        }
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB: {
            res = copy();
            QIMAGE_SANITYCHECK_MEMORY(res);
            const QRgb c0 = res.d->mono0;
            res.d->mono0 = QRgb(((c0 << 16) & 0xff0000) | ((c0 >> 16) & 0xff) | (c0 & 0xff00ff00));
            const QRgb c1 = res.d->mono1;
            res.d->mono1 = QRgb(((c1 << 16) & 0xff0000) | ((c1 >> 16) & 0xff) | (c1 & 0xff00ff00));
            break;
        }
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            res = QImage(d->width, d->height, d->format);
            QIMAGE_SANITYCHECK_MEMORY(res);
            uchar* resbits = res.bits();
            const int bpl = res.bytesPerLine();
            for (int i = 0; i < d->height; i++) {
                uint *q = (uint*)QFAST_SCAN_LINE(resbits, bpl, i);
                const uint *p = (const uint*)constScanLine(i);
                const uint *end = p + d->width;
                while (p < end) {
                    *q = ((*p << 16) & 0xff0000) | ((*p >> 16) & 0xff) | (*p & 0xff00ff00);
                    p++;
                    q++;
                }
            }
            break;
        }
        case QImage::Format_RGB16: {
            res = QImage(d->width, d->height, d->format);
            QIMAGE_SANITYCHECK_MEMORY(res);
            uchar* resbits = res.bits();
            const int bpl = res.bytesPerLine();
            for (int i = 0; i < d->height; i++) {
                ushort *q = (ushort*)QFAST_SCAN_LINE(resbits, bpl, i);
                const ushort *p = (const ushort*)constScanLine(i);
                const ushort *end = p + d->width;
                while (p < end) {
                    *q = ((*p << 11) & 0xf800) | ((*p >> 11) & 0x1f) | (*p & 0x07e0);
                    p++;
                    q++;
                }
            }
            break;
        }
    }
    return res;
}

/*!
    Loads an image from the file with the given \a fileName. Returns true if
    the image was successfully loaded; otherwise returns false.

    The loader attempts to read the image using the specified \a format, e.g.,
    PNG or XPM. If \a format is not specified (which is the default), the
    loader probes the file for a header to guess the file format.

    \sa {QImage#Reading and Writing Image Files}{Reading and Writing Image Files}
*/

bool QImage::load(const QString &fileName, const char* format)
{
    if (fileName.isEmpty())
        return false;

    QImage image = QImageReader(fileName, format).read();
    if (!image.isNull()) {
        operator=(image);
        return true;
    }
    return false;
}

/*!
    \overload

    This function reads a QImage from the given \a device. This can,
    for example, be used to load an image directly into a QByteArray.
*/

bool QImage::load(QIODevice* device, const char* format)
{
    QImage image = QImageReader(device, format).read();
    if(!image.isNull()) {
        operator=(image);
        return true;
    }
    return false;
}

/*!
    \fn bool QImage::loadFromData(const char *data, int len, const char *format)

    Loads an image from the first \a len bytes of the given binary \a
    data. Returns true if the image was successfully loaded; otherwise
    returns false.

    The loader attempts to read the image using the specified \a format, e.g.,
    PNG or XPM. If \a format is not specified (which is the default), the
    loader probes the file for a header to guess the file format.

    \sa {QImage#Reading and Writing Image Files}{Reading and Writing Image Files}
*/

bool QImage::loadFromData(const char *data, int len, const char *format)
{
    QBuffer b;
    b.setData(data, len);
    b.open(QIODevice::ReadOnly);
    return QImageReader(&b, format).read(this);
}

/*!
    \fn bool QImage::loadFromData(const QByteArray &data, const char *format)

    \overload

    Loads an image from the given QByteArray \a data.
*/

/*!
    \fn QImage QImage::fromData(const char *data, int size, const char *format)

    Constructs a QImage from the first \a size bytes of the given
    binary \a data. The loader attempts to read the image using the
    specified \a format. If \a format is not specified (which is the default),
    the loader probes the file for a header to guess the file format.
    binary \a data. The loader attempts to read the image, either using the
    optional image \a format specified or by determining the image format from
    the data.

    If \a format is not specified (which is the default), the loader probes the
    file for a header to determine the file format. If \a format is specified,
    it must be one of the values returned by QImageReader::supportedImageFormats().

    If the loading of the image fails, the image returned will be a null image.

    \sa load(), save(), {QImage#Reading and Writing Image Files}{Reading and Writing Image Files}
 */

QImage QImage::fromData(const char *data, int size, const char *format)
{
    QBuffer b;
    b.setData(data, size);
    b.open(QIODevice::ReadOnly);
    return QImageReader(&b, format).read();
}

/*!
    \fn QImage QImage::fromData(const QByteArray &data, const char *format)

    \overload

    Loads an image from the given QByteArray \a data.
*/

/*!
    Saves the image to the file with the given \a fileName, using the
    given image file \a format and \a quality factor. If \a format is
    0, QImage will attempt to guess the format by looking at \a fileName's
    suffix.

    The \a quality factor must be in the range 0 to 100 or -1. Specify
    0 to obtain small files, 100 for large and -1 (the default) to use
    the default settings.

    Returns true if the image was successfully saved; otherwise
    returns false.

    \sa {QImage#Reading and Writing Image Files}{Reading and Writing
    Image Files}
*/
bool QImage::save(const QString &fileName, const char *format, int quality) const
{
    if (isNull())
        return false;
    QImageWriter writer(fileName, format);
    return d->doImageIO(this, &writer, quality);
}

/*!
    \overload

    This function writes a QImage to the given \a device.

    This can, for example, be used to save an image directly into a
    QByteArray:

    \snippet doc/src/snippets/image/image.cpp 0
*/

bool QImage::save(QIODevice* device, const char* format, int quality) const
{
    if (isNull())
        return false;                                // nothing to save
    QImageWriter writer(device, format);
    return d->doImageIO(this, &writer, quality);
}

/* \internal
*/

bool QImageData::doImageIO(const QImage *image, QImageWriter *writer, int quality) const
{
    if (Q_UNLIKELY(quality > 100  || quality < -1))
        qWarning("QImage::save: Quality out of range [-1, 100]");
    if (quality >= 0)
        writer->setQuality(qMin(quality,100));
    return writer->write(*image);
}

/*****************************************************************************
  QImage stream functions
 *****************************************************************************/
#if !defined(QT_NO_DATASTREAM)
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QImage &image)
    \relates QImage

    Writes the given \a image to the given \a stream. Note that
    writing the stream to a file will not produce a valid image file.

    \sa QImage::save(), {Serializing Qt Data Types}
*/
QDataStream &operator<<(QDataStream &s, const QImage &image)
{
    const bool alphaclut = (image.d ? image.d->has_alpha_clut : false);
    const bool monoimage = (image.d ? (image.d->depth == 1) : false);
    s << (qint8) image.format();
    s << (qint16) image.width();
    s << (qint16) image.height();
    s << (qint32) image.dotsPerMeterX();
    s << (qint32) image.dotsPerMeterY();
    s << (qint32) image.byteCount();
    if (monoimage) {
        s << (qint64)image.d->mono0;
        s << (qint64)image.d->mono1;
    } else {
        s << (qint64)-1;
        s << (qint64)-1;
    }
    s << (bool)alphaclut;
    s.writeRawData(reinterpret_cast<const char*>(image.constBits()), image.byteCount());
    return s;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QImage &image)
    \relates QImage

    Reads an image from the given \a stream and stores it in the given
    \a image.

    \sa QImage::load(), {Serializing Qt Data Types}
*/
QDataStream &operator>>(QDataStream &s, QImage &image)
{
    qint8 format;
    qint16 width;
    qint16 height;
    qint32 dotsperx;
    qint32 dotspery;
    qint32 bytecount;
    qint64 mono0;
    qint64 mono1;
    bool alphaclut;
    s >> format;
    s >> width;
    s >> height;
    s >> dotsperx;
    s >> dotspery;
    s >> bytecount;
    s >> mono0;
    s >> mono1;
    s >> alphaclut;

    image = QImage(width, height, static_cast<QImage::Format>(format));
    if (image.isNull()) {
        // could be a default-constructed image
        return s;
    }

    const qint64 result = s.readRawData(reinterpret_cast<char*>(image.d->data), bytecount);
    if (Q_UNLIKELY(result != bytecount)) {
        image = QImage();
        s.setStatus(QDataStream::ReadPastEnd);
        return s;
    }

    image.d->has_alpha_clut = alphaclut;
    image.d->mono0 = mono0;
    image.d->mono1 = mono1;
    image.d->dpmx = dotsperx;
    image.d->dpmy = dotspery;

    return s;
}
#endif // QT_NO_DATASTREAM

/*!
    \fn bool QImage::operator==(const QImage & image) const

    Returns true if this image and the given \a image have the same
    contents; otherwise returns false.

    The comparison can be slow, unless there is some obvious
    difference (e.g. different size or format), in which case the
    function will return quickly.

    \sa operator=()
*/
bool QImage::operator==(const QImage & i) const
{
    // same object, or shared?
    if (i.d == d)
        return true;
    if (!i.d || !d)
        return false;

    // obviously different stuff?
    if (i.d->height != d->height || i.d->width != d->width) {
        return false;
    }

    if (i.d->format == d->format && i.d->nbytes == d->nbytes
        && d->format >= QImage::Format_RGB32 && d->format <= QImage::Format_RGB16) {
        return (memcmp(constBits(), i.constBits(), d->nbytes) == 0);
    }

    for (int h = 0; h < d->height; h++) {
        for (int w = 0; w < d->width; w++) {
            if (pixel(w, h) != i.pixel(w, h)) {
                return false;
            }
        }
    }
    return true;
}

/*!
    \fn bool QImage::operator!=(const QImage & image) const

    Returns true if this image and the given \a image have different
    contents; otherwise returns false.

    The comparison can be slow, unless there is some obvious
    difference, such as different widths, in which case the function
    will return quickly.

    \sa operator=()
*/
bool QImage::operator!=(const QImage & i) const
{
    return !(*this == i);
}

/*!
    Returns the number of pixels that fit horizontally in a physical
    meter. Together with dotsPerMeterY(), this number defines the
    intended scale and aspect ratio of the image.

    \sa setDotsPerMeterX(), {QImage#Image Information}{Image
    Information}
*/
int QImage::dotsPerMeterX() const
{
    return d ? qRound(d->dpmx) : 0;
}

/*!
    Returns the number of pixels that fit vertically in a physical
    meter. Together with dotsPerMeterX(), this number defines the
    intended scale and aspect ratio of the image.

    \sa setDotsPerMeterY(), {QImage#Image Information}{Image
    Information}
*/
int QImage::dotsPerMeterY() const
{
    return d ? qRound(d->dpmy) : 0;
}

/*!
    Sets the number of pixels that fit horizontally in a physical
    meter, to \a x.

    Together with dotsPerMeterY(), this number defines the intended
    scale and aspect ratio of the image, and determines the scale
    at which QPainter will draw graphics on the image. It does not
    change the scale or aspect ratio of the image when it is rendered
    on other paint devices.

    \sa dotsPerMeterX(), {QImage#Image Information}{Image Information}
*/
void QImage::setDotsPerMeterX(int x)
{
    if (!d || !x)
        return;
    detach();

    d->dpmx = x;
}

/*!
    Sets the number of pixels that fit vertically in a physical meter,
    to \a y.

    Together with dotsPerMeterX(), this number defines the intended
    scale and aspect ratio of the image, and determines the scale
    at which QPainter will draw graphics on the image. It does not
    change the scale or aspect ratio of the image when it is rendered
    on other paint devices.

    \sa dotsPerMeterY(), {QImage#Image Information}{Image Information}
*/
void QImage::setDotsPerMeterY(int y)
{
    if (!d || !y)
        return;
    detach();

    d->dpmy = y;
}

/*
    Sets the image bits to the \a pixmap contents and returns a
    reference to the image.

    If the image shares data with other images, it will first
    dereference the shared data.

    Makes a call to QPixmap::convertToImage().
*/

/*!
    \internal

    Used by QPainter to retrieve a paint engine for the image.
*/

QPaintEngine *QImage::paintEngine() const
{
    if (!d)
        return 0;

    if (!d->paintEngine) {
        d->paintEngine = new QRasterPaintEngine(const_cast<QImage *>(this));
    }

    return d->paintEngine;
}


/*!
    \internal

    Returns the size for the specified \a metric on the device.
*/
int QImage::metric(PaintDeviceMetric metric) const
{
    if (!d)
        return 0;

    switch (metric) {
    case PdmWidth:
        return d->width;

    case PdmHeight:
        return d->height;

    case PdmWidthMM:
        return qRound(d->width * 1000 / d->dpmx);

    case PdmHeightMM:
        return qRound(d->height * 1000 / d->dpmy);

    case PdmNumColors:
        if (d->depth == 1)
            return 2;
        return 0;

    case PdmDepth:
        return d->depth;

    case PdmDpiX:
        return qRound(d->dpmx * 0.0254);

    case PdmDpiY:
        return qRound(d->dpmy * 0.0254);

    case PdmPhysicalDpiX:
        return qRound(d->dpmx * 0.0254);

    case PdmPhysicalDpiY:
        return qRound(d->dpmy * 0.0254);

    default:
        qWarning("QImage::metric(): Unhandled metric type %d", metric);
    }
    return 0;
}

/*!
    \fn QImage QImage::xForm(const QMatrix &matrix) const

    Use transformed() instead.

    \oldcode
        QImage image;
        ...
        image.xForm(matrix);
    \newcode
        QImage image;
        ...
        image.transformed(matrix);
    \endcode
*/

/*!
    Returns a number that identifies the contents of this QImage
    object. Distinct QImage objects can only have the same key if they
    refer to the same contents.

    The key will change when the image is altered.
*/
qint64 QImage::cacheKey() const
{
    if (!d)
        return 0;
    return (((qint64) d->ser_no) << 32) | ((qint64) d->detach_no);
}

/*!
    \obsolete
    Sets the alpha channel of this image to the given \a alphaChannel.

    If \a alphaChannel is an 8 bit grayscale image, the intensity values are
    written into this buffer directly. Otherwise, \a alphaChannel is converted
    to 32 bit and the intensity of the RGB pixel values is used.

    Note that the image will be converted to the Format_ARGB32_Premultiplied
    format if the function succeeds.

    Use one of the composition modes in QPainter::CompositionMode instead.

    \warning This function is expensive.

    \sa alphaChannel(), {QImage#Image Transformations}{Image
    Transformations}, {QImage#Image Formats}{Image Formats}
*/

void QImage::setAlphaChannel(const QImage &alphaChannel)
{
    if (!d)
        return;

    int w = d->width;
    int h = d->height;

    if (Q_UNLIKELY(w != alphaChannel.d->width || h != alphaChannel.d->height)) {
        qWarning("QImage::setAlphaChannel: "
                 "Alpha channel must have same dimensions as the target image");
        return;
    }

    if (Q_UNLIKELY(d->paintEngine && d->paintEngine->isActive())) {
        qWarning("QImage::setAlphaChannel: "
                 "Unable to set alpha channel while image is being painted on");
        return;
    }

    if (d->format == QImage::Format_ARGB32_Premultiplied)
        detach();
    else
        *this = convertToFormat(QImage::Format_ARGB32_Premultiplied);

    if (isNull())
        return;

    const QImage sourceImage = alphaChannel.convertToFormat(QImage::Format_RGB32);
    if (Q_UNLIKELY(sourceImage.isNull())) {
        qWarning("QImage::setAlphaChannel: out of memory");
        return;
    }
    const uchar *src_data = sourceImage.d->data;
    const uchar *dest_data = d->data;
    for (int y=0; y<h; ++y) {
        const QRgb *src = (const QRgb *) src_data;
        QRgb *dest = (QRgb *) dest_data;
        for (int x=0; x<w; ++x) {
            int alpha = qGray(*src);
            int destAlpha = qt_div_255(alpha * qAlpha(*dest));
            *dest = ((destAlpha << 24)
                        | (qt_div_255(qRed(*dest) * alpha) << 16)
                        | (qt_div_255(qGreen(*dest) * alpha) << 8)
                        | (qt_div_255(qBlue(*dest) * alpha)));
            ++dest;
            ++src;
        }
        src_data += sourceImage.d->bytes_per_line;
        dest_data += d->bytes_per_line;
    }
}


/*!
    \obsolete

    Returns the alpha channel of the image as a new ARGB32 QImage in which
    each pixel's red, green, and blue values are given the alpha value of the
    original image. The color depth of the returned image is 32-bit.

    You can see an example of use of this function in QPixmap's
    \l{QPixmap::}{alphaChannel()}, which works in the same way as
    this function on QPixmaps.

    Most usecases for this function can be replaced with QPainter and
    using composition modes.

    \warning This is an expensive function.

    \sa setAlphaChannel(), hasAlphaChannel(),
    {QPixmap#Pixmap Information}{Pixmap},
    {QImage#Image Transformations}{Image Transformations}
*/

QImage QImage::alphaChannel() const
{
    if (!d)
        return QImage();

    int w = d->width;
    int h = d->height;

    QImage image(w, h, QImage::Format_ARGB32);
    QIMAGE_SANITYCHECK_MEMORY(image);

    if (!hasAlphaChannel()) {
        image.fill(255);
        return image;
    }

    QImage alpha32 = *this;
    if (d->format != QImage::Format_ARGB32 && d->format != QImage::Format_ARGB32_Premultiplied)
        alpha32 = convertToFormat(Format_ARGB32);
    QIMAGE_SANITYCHECK_MEMORY(alpha32);

    const uchar *src_data = alpha32.d->data;
    uchar *dest_data = image.d->data;
    for (int y=0; y<h; ++y) {
        const QRgb *src = (const QRgb *) src_data;
        uchar *dest = dest_data;
        for (int x=0; x<w; ++x) {
            *dest = qAlpha(*src);
            ++dest;
            ++src;
        }
        src_data += alpha32.d->bytes_per_line;
        dest_data += image.d->bytes_per_line;
    }

    return image;
}

/*!
    Returns true if the image has a format that respects the alpha
    channel, otherwise returns false.

    \sa {QImage#Image Information}{Image Information}
*/
bool QImage::hasAlphaChannel() const
{
    return d && (d->format == QImage::Format_ARGB32_Premultiplied
                 || d->format == QImage::Format_ARGB32
                 || (d->has_alpha_clut && (d->format == QImage::Format_Mono
                                           || d->format == QImage::Format_MonoLSB)));
}


/*!
    \since 4.7
    Returns the number of bit planes in the image.

    The number of bit planes is the number of bits of color and
    transparency information for each pixel. This is different from
    (i.e. smaller than) the depth when the image format contains
    unused bits.

    \sa depth(), format(), {QImage#Image Formats}{Image Formats}
*/
int QImage::bitPlaneCount() const
{
    if (!d)
        return 0;
    switch (d->format) {
        case QImage::Format_Invalid:
            return 0;
        case QImage::Format_RGB32:
            return 24;
        default:
            return qt_depthForFormat(d->format);
    }
    Q_UNREACHABLE();
}



/*!
    \fn QImage QImage::copy(const QRect &rect, Qt::ImageConversionFlags flags) const
    \compat

    Use copy() instead.
*/

/*!
    \fn QImage QImage::copy(int x, int y, int w, int h, Qt::ImageConversionFlags flags) const
    \compat

    Use copy() instead.
*/

/*!
    \fn QImage QImage::scaleWidth(int w) const
    \compat

    Use scaledToWidth() instead.
*/

/*!
    \fn QImage QImage::scaleHeight(int h) const
    \compat

    Use scaledToHeight() instead.
*/

/*!
    Returns a copy of the image that is transformed using the given
    transformation \a matrix and transformation \a mode.

    The transformation \a matrix is internally adjusted to compensate
    for unwanted translation; i.e. the image produced is the smallest
    image that contains all the transformed points of the original
    image. Use the trueMatrix() function to retrieve the actual matrix
    used for transforming an image.

    Unlike the other overload, this function can be used to perform perspective
    transformations on images.

    \sa trueMatrix(), {QImage#Image Transformations}{Image
    Transformations}
*/

QImage QImage::transformed(const QTransform &matrix, Qt::TransformationMode mode) const
{
    if (!d)
        return QImage();

    // source image data
    int ws = width();
    int hs = height();

    // compute size of target image
    QTransform mat = trueMatrix(matrix, ws, hs);

    if (mat.type() == QTransform::TxNone) {
        // identity matrix
        return *this;
    }

    if (mat.type() == QTransform::TxRotate) {
        if (mat.m11() == 0 && mat.m12() == 1.0 && mat.m21() == -1. && mat.m22() == 0)
            return rotated90(*this);
        else if (mat.m11() == 0 && mat.m12() == -1.0 && mat.m21() == 1.0 && mat.m22() == 0 )
            return rotated270(*this);
        else if (mat.m11() == -1.0 && mat.m12() == 0 && mat.m21() == 0 && mat.m22() == -1.0)
            return rotated180(*this);
    }

    // target image data
    int wd;
    int hd;
    bool complex_xform = false;
    if (mat.type() == QTransform::TxScale) {
        if (mode == Qt::FastTransformation) {
            hd = qRound(qAbs(mat.m22()) * hs);
            wd = qRound(qAbs(mat.m11()) * ws);
        } else {
            hd = int(qAbs(mat.m22()) * hs + 0.9999);
            wd = int(qAbs(mat.m11()) * ws + 0.9999);
        }
    } else {
        QPolygonF a(QRectF(0, 0, ws, hs));
        a = mat.map(a);
        QRect r = a.boundingRect().toAlignedRect();
        wd = r.width();
        hd = r.height();
        complex_xform = true;
    }

    if (wd == 0 || hd == 0)
        return QImage();

    QImage::Format target_format = d->format;

    if (complex_xform || mode == Qt::SmoothTransformation) {
        if (d->format < QImage::Format_RGB32 || !hasAlphaChannel()) {
            target_format = QImage::Format_ARGB32_Premultiplied;
        }
    }

    QImage dImage(wd, hd, target_format);
    QIMAGE_SANITYCHECK_MEMORY(dImage);

    if (target_format == QImage::Format_MonoLSB
        || target_format == QImage::Format_Mono) {
        dImage.d->mono0 = d->mono0;
        dImage.d->mono1 = d->mono1;
        dImage.d->has_alpha_clut = d->has_alpha_clut | complex_xform;
    }

    dImage.d->dpmx = dotsPerMeterX();
    dImage.d->dpmy = dotsPerMeterY();

    // initizialize the data
    memset(dImage.d->data, 0x00, dImage.byteCount());

    QPainter p(&dImage);
    if (mode == Qt::SmoothTransformation) {
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
    }
    p.setTransform(mat);
    p.drawImage(QPoint(0, 0), *this);
    return dImage;
}

/*!
    \fn QTransform QImage::trueMatrix(const QTransform &matrix, int width, int height)

    Returns the actual matrix used for transforming an image with the
    given \a width, \a height and \a matrix.

    When transforming an image using the transformed() function, the
    transformation matrix is internally adjusted to compensate for
    unwanted translation, i.e. transformed() returns the smallest
    image containing all transformed points of the original image.
    This function returns the modified matrix, which maps points
    correctly from the original image into the new image.

    Unlike the other overload, this function creates transformation
    matrices that can be used to perform perspective
    transformations on images.

    \sa transformed(), {QImage#Image Transformations}{Image
    Transformations}
*/

QTransform QImage::trueMatrix(const QTransform &matrix, int w, int h)
{
    const QRectF rect(0, 0, w, h);
    const QRect mapped = matrix.mapRect(rect).toAlignedRect();
    const QPoint delta = mapped.topLeft();
    return matrix * QTransform().translate(-delta.x(), -delta.y());
}

QT_END_NAMESPACE
