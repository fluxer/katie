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

#include "qpixmapdata_p.h"
#include "qbitmap.h"
#include "qimagereader.h"
#include "qapplication_p.h"
#include "qimage_p.h"
#include "qx11info_x11.h"
#include "qdrawhelper_p.h"
#include "qguicommon_p.h"

QT_BEGIN_NAMESPACE

QPixmapData *QPixmapData::create(int w, int h, PixelType type)
{
    QPixmapData *data = new QPixmapData(type);
    data->resize(w, h);
    return data;
}


QPixmapData::QPixmapData(PixelType pixelType)
    : ref(0),
      detach_no(0),
      type(pixelType),
      ser_no(0)
{
}

QPixmapData::~QPixmapData()
{
}

QPixmapData *QPixmapData::createCompatiblePixmapData() const
{
    return new QPixmapData(pixelType());
}

static QImage makeBitmapCompliantIfNeeded(QPixmapData *d, const QImage &image, Qt::ImageConversionFlags flags)
{
    if (d->pixelType() == QPixmapData::BitmapType) {
        QImage img = image.convertToFormat(QImage::Format_MonoLSB, flags);

        // make sure image.color(0) == Qt::color0 (white)
        // and image.color(1) == Qt::color1 (black)
        if (img.color(0) == qt_blackrgb && img.color(1) == qt_whitergb) {
            img.invertPixels();
            img.setColor(0, qt_whitergb);
            img.setColor(1, qt_blackrgb);
        }
        return img;
    }

    return image;
}

void QPixmapData::fromImageReader(QImageReader *imageReader,
                                  Qt::ImageConversionFlags flags)
{
    QImage image = imageReader->read();
    if (image.isNull())
        return;

    fromImage(image, flags);
}

bool QPixmapData::fromFile(const QString &fileName, const char *format,
                           Qt::ImageConversionFlags flags)
{
    QImage image = QImageReader(fileName, format).read();
    if (image.isNull())
        return false;
    fromImage(makeBitmapCompliantIfNeeded(this, image, flags), flags);
    return !isNull();
}

QBitmap QPixmapData::mask() const
{
    if (!hasAlphaChannel())
        return QBitmap();

    const QImage img = toImage();
    const QImage image = (img.depth() < 32 ? img.convertToFormat(QImage::Format_ARGB32_Premultiplied) : img);
    const int w = image.width();
    const int h = image.height();

    QImage mask(w, h, QImage::Format_MonoLSB);
    if (mask.isNull()) // allocation failed
        return QBitmap();

    mask.setColorTable(monoColorTable());

    const int bpl = mask.bytesPerLine();
    uchar *dest = mask.bits();
    for (int y = 0; y < h; ++y) {
        const QRgb *src = reinterpret_cast<const QRgb*>(image.constScanLine(y));
        uchar *tscan = QFAST_SCAN_LINE(dest, bpl, y);
        ::memset(tscan, 0, bpl);
        for (int x = 0; x < w; ++x) {
            if (qAlpha(*src) > 0)
                tscan[x >> 3] |= qt_pixmap_bit_mask[x & 7];
            ++src;
        }
    }

    return QBitmap::fromImage(mask);
}

void QPixmapData::setMask(const QBitmap &mask)
{
    if (mask.isNull()) {
        if (image.depth() != 1) { // hw: ????
            image = image.convertToFormat(QImage::Format_RGB32);
        }
        return;
    }

    const int w = image.width();
    const int h = image.height();
    if (image.depth() == 1) {
        QImage result(image.size(), image.format());
        const QImage imageMask = mask.toImage().convertToFormat(result.format());
        const int bpl = result.bytesPerLine();
        uchar *dest = result.bits();
        for (int y = 0; y < h; ++y) {
            const uchar *mscan = imageMask.constScanLine(y);
            uchar *tscan = QFAST_SCAN_LINE(dest, bpl, y);
            for (int i = 0; i < bpl; ++i)
                tscan[i] &= mscan[i];
        }
        image = result;
    } else {
        const QImage imageMask = mask.toImage().convertToFormat(QImage::Format_MonoLSB);
        QImage result = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        const int bpl = result.bytesPerLine();
        uchar *dest = result.bits();
        for (int y = 0; y < h; ++y) {
            const uchar *mscan = imageMask.constScanLine(y);
            QRgb *tscan = reinterpret_cast<QRgb*>(QFAST_SCAN_LINE(dest, bpl, y));
            for (int x = 0; x < w; ++x) {
                if (!(mscan[x>>3] & qt_pixmap_bit_mask[x&7]))
                    tscan[x] = 0;
            }
        }
        image = result;
    }
}

QPixmap QPixmapData::transformed(const QTransform &matrix,
                                 Qt::TransformationMode mode) const
{
    return QPixmap::fromImage(toImage().transformed(matrix, mode));
}

void QPixmapData::setAlphaChannel(const QPixmap &alphaChannel)
{
    image.setAlphaChannel(alphaChannel.toImage());
}

QPixmap QPixmapData::alphaChannel() const
{
    return QPixmap::fromImage(toImage().alphaChannel());
}

bool QPixmapData::hasAlphaChannel() const
{
    return image.hasAlphaChannel();
}

void QPixmapData::setSerialNumber(int serNo)
{
    ser_no = serNo;
}

QImage QPixmapData::toImage() const
{
    if (image.paintingActive())
        return image.copy();

    return image;
}

QImage QPixmapData::toImage(const QRect &rect) const
{
    if (rect.isNull())
        return image;

    return image.copy(rect);
}

QPaintEngine* QPixmapData::paintEngine() const
{
    return image.paintEngine();
}

int QPixmapData::metric(QPaintDevice::PaintDeviceMetric metric) const
{
    if (isNull())
        return 0;

    // override the image dpi with the screen dpi when rendering to a pixmap
    switch (metric) {
    case QPaintDevice::PdmWidth:
        return width();
    case QPaintDevice::PdmHeight:
        return height();
    case QPaintDevice::PdmWidthMM:
        return qRound(width() * 25.4 / QX11Info::appDpiX());
    case QPaintDevice::PdmHeightMM:
        return qRound(height() * 25.4 / QX11Info::appDpiY());
    case QPaintDevice::PdmNumColors:
        if (depth() == 1)
            return 2;
        return 0;
    case QPaintDevice::PdmDepth:
        return depth();
    case QPaintDevice::PdmDpiX: // fall-through
    case QPaintDevice::PdmPhysicalDpiX:
        return QX11Info::appDpiX();
    case QPaintDevice::PdmDpiY: // fall-through
    case QPaintDevice::PdmPhysicalDpiY:
        return QX11Info::appDpiY();
    }

    qWarning("QPixmapData::metric(): Unhandled metric type %d", metric);
    return 0;
}

QImage* QPixmapData::buffer()
{
    return &image;
}

void QPixmapData::fill(const QColor &color)
{
    if (color.alpha() != 255 && !image.hasAlphaChannel()) {
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    image.fill(color);
}

void QPixmapData::copy(const QPixmapData *data, const QRect &rect)
{
    fromImage(data->toImage(rect), Qt::NoOpaqueDetection);
}

// from qwindowsurface.cpp
extern void qt_scrollRectInImage(QImage *img, const QRect &rect, const QPoint &offset);

void QPixmapData::scroll(int dx, int dy, const QRect &rect)
{
    if (!image.isNull())
        qt_scrollRectInImage(&image, rect, QPoint(dx, dy));
}

void QPixmapData::resize(int width, int height)
{
    QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    if (pixelType() == BitmapType) {
        format = QImage::Format_MonoLSB;
    }

    image = QImage(width, height, format);

    if (pixelType() == BitmapType && !image.isNull()) {
        image.setColorTable(monoColorTable());
    }

    setSerialNumber(image.cacheKey() >> 32);
}

bool QPixmapData::fromData(const uchar *buffer, uint len, const char *format,
                           Qt::ImageConversionFlags flags)
{
    QImage image = QImage::fromData(reinterpret_cast<const char *>(buffer), len, format);
    if (image.isNull())
        return false;

    fromImage(image, flags);
    return !isNull();
}

void QPixmapData::fromImage(const QImage &sourceImage,
                            Qt::ImageConversionFlags flags)
{
    QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    if (pixelType() == BitmapType) {
        format = QImage::Format_MonoLSB;
    }

    image = sourceImage.convertToFormat(format);

    setSerialNumber(image.cacheKey()  >> 32);
}

QT_END_NAMESPACE
