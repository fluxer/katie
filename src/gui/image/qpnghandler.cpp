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

#include "qpnghandler_p.h"
#include "qiodevice.h"
#include "qvariant.h"
#include "qimage.h"
#include "qimage_p.h"
#include "qdrawhelper_p.h"
#include "qcorecommon_p.h"
#include "qguicommon_p.h"

#include <png.h>
#include <pngconf.h>

QT_BEGIN_NAMESPACE

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#  define QFILLER_ORDER PNG_FILLER_BEFORE
#else
#  define QFILLER_ORDER PNG_FILLER_AFTER
#endif

/*
  All PNG files load to the minimal QImage equivalent.

  All QImage formats output to reasonably efficient PNG equivalents.
  Never to grayscale.
*/

#if defined(Q_C_CALLBACKS)
extern "C" {
#endif

static void qt_png_warning(png_structp /*png_ptr*/, png_const_charp message)
{
    qWarning("libpng warning: %s", message);
}

static void qt_png_read(png_structp png_ptr, png_bytep data, png_size_t length)
{
    QPngHandler *handler = (QPngHandler *)png_get_io_ptr(png_ptr);

    png_size_t nr = handler->device()->read((char*)data, length);
    if (nr != length) {
        png_error(png_ptr, "Read error");
    }
}

static void qt_png_write(png_structp png_ptr, png_bytep data, png_size_t length)
{
    QPngHandler *handler = (QPngHandler *)png_get_io_ptr(png_ptr);

    png_size_t nr = handler->device()->write((char*)data, length);
    if (nr != length) {
        png_error(png_ptr, "Write error");
    }
}

static void qt_png_flush(png_structp /* png_ptr */)
{
}

#if defined(Q_C_CALLBACKS)
}
#endif

QPngHandler::QPngHandler()
    : m_compression(1)
{
}

QPngHandler::~QPngHandler()
{
}

bool QPngHandler::canRead() const
{
    if (QPngHandler::canRead(device())) {
        setFormat("png");
        return true;
    }

    return false;
}

bool QPngHandler::read(QImage *image)
{
    if (!canRead()) {
        return false;
    }

    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    if (!png_ptr) {
        return false;
    }

    png_set_error_fn(png_ptr, 0, 0, qt_png_warning);

    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, 0, 0);
        return false;
    }

    png_info *end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, 0);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return false;
    }

    png_set_read_fn(png_ptr, this, qt_png_read);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width;
    png_uint_32 height;
    int bit_depth;
    int color_type;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);
    png_set_interlace_handling(png_ptr);

    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }

    png_set_expand(png_ptr);

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
    } else if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }

    QImage::Format format = QImage::Format_ARGB32;
    // Only add filler if no alpha, or we can get 5 channel data.
    if (!(color_type & PNG_COLOR_MASK_ALPHA)
        && !png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_filler(png_ptr, 0xff, QFILLER_ORDER);
        // We want 4 bytes, but it isn't an alpha channel
        format = QImage::Format_RGB32;
    }
    *image = QImage(width, height, format);
    if (image->isNull()) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return false;
    }

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    png_set_swap_alpha(png_ptr);
#endif

    png_read_update_info(png_ptr, info_ptr);

    // Qt==ARGB==Big(ARGB)==Little(BGRA)
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    png_set_bgr(png_ptr);
#endif

    uchar *data = image->d->data;
    const int bpl = image->bytesPerLine();
    png_byte **row_pointers = new png_bytep[height];
    for (uint y = 0; y < height; y++) {
        row_pointers[y] = QFAST_SCAN_LINE(data, bpl, y);
    }

    png_read_image(png_ptr, row_pointers);

    image->d->dpmx = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    image->d->dpmy = png_get_y_pixels_per_meter(png_ptr, info_ptr);

    png_read_end(png_ptr, end_info);

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete [] row_pointers;

    return true;
}

bool QPngHandler::write(const QImage &image)
{
    QImage copy = image.convertToFormat(image.hasAlphaChannel() ? QImage::Format_ARGB32 : QImage::Format_RGB32);
    const int height = copy.height();

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,0,0,0);
    if (!png_ptr) {
        return false;
    }

    png_set_error_fn(png_ptr, 0, 0, qt_png_warning);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, 0);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    png_set_write_fn(png_ptr, (void*)this, qt_png_write, qt_png_flush);

    int color_type = 0;
    if (copy.hasAlphaChannel()) {
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    } else {
        color_type = PNG_COLOR_TYPE_RGB;
    }

    png_set_IHDR(png_ptr, info_ptr, copy.width(), height,
                 8, // per channel
                 color_type, 0, 0, 0);       // sets #channels

    png_color_8 sig_bit;
    sig_bit.red = 8;
    sig_bit.green = 8;
    sig_bit.blue = 8;
    sig_bit.alpha = copy.hasAlphaChannel() ? 8 : 0;
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    png_set_compression_level(png_ptr, m_compression);

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    // Swap ARGB to RGBA (normal PNG format) before saving on
    // BigEndian machines
    png_set_swap_alpha(png_ptr);
#elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    // Qt==ARGB==Big(ARGB)==Little(BGRA). But RGB888 is RGB regardless
    png_set_bgr(png_ptr);
#endif

    if (copy.dotsPerMeterX() > 0 || copy.dotsPerMeterY() > 0) {
        png_set_pHYs(png_ptr, info_ptr,
                copy.dotsPerMeterX(), copy.dotsPerMeterY(),
                PNG_RESOLUTION_METER);
    }

    png_write_info(png_ptr, info_ptr);

    png_set_packing(png_ptr);

    if (color_type == PNG_COLOR_TYPE_RGB) {
        png_set_filler(png_ptr, 0, QFILLER_ORDER);
    }

    png_bytep* row_pointers = new png_bytep[height];
    for (int y=0; y<height; y++) {
        row_pointers[y] = (png_bytep)copy.constScanLine(y);
    }
    png_write_image(png_ptr, row_pointers);

    png_write_end(png_ptr, info_ptr);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    delete [] row_pointers;

    return true;
}

bool QPngHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    return (option == QImageIOHandler::CompressionLevel);
}

void QPngHandler::setOption(QImageIOHandler::ImageOption option, const QVariant &value)
{
    if (option == QImageIOHandler::CompressionLevel) {
        const int newlevel = value.toInt();
        if (Q_UNLIKELY(newlevel < 0 || newlevel > 9)) {
            qWarning("QPngHandler::setOption() invalid compression level value");
            m_compression = 1;
        } else {
            m_compression = newlevel;
        }
    }
}

QByteArray QPngHandler::name() const
{
    return "png";
}

bool QPngHandler::canRead(QIODevice *device)
{
    if (Q_UNLIKELY(!device)) {
        qWarning("QPngHandler::canRead() called with no device");
        return false;
    }

    QSTACKARRAY(char, head, 8);
    if (device->peek(head, sizeof(head)) != sizeof(head))
        return false;

    static const uchar pngheader[]
        = { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a };
    return (qstrncmp(head, reinterpret_cast<const char*>(pngheader), 8) == 0);
}

QT_END_NAMESPACE
