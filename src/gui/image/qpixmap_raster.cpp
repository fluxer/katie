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
#include "qfont_p.h"
#include "qpixmap_raster_p.h"
#include "qimage_p.h"
#include "qpaintengine.h"
#include "qbitmap.h"
#include "qimage.h"
#include "qbuffer.h"
#include "qimagereader.h"
#include "qwidget_p.h"
#include "qdrawhelper_p.h"
#include "qguicommon_p.h"

QT_BEGIN_NAMESPACE

QRasterPixmapData::QRasterPixmapData(PixelType type)
    : QPixmapData(type, RasterClass)
{
}

QRasterPixmapData::~QRasterPixmapData()
{
}

QPixmapData *QRasterPixmapData::createCompatiblePixmapData() const
{
    return new QRasterPixmapData(pixelType());
}

void QRasterPixmapData::resize(int width, int height)
{
    QImage::Format format;
    if (pixelType() == BitmapType)
        format = QImage::Format_MonoLSB;
    else
        format = QImage::systemFormat();

    image = QImage(width, height, format);
    w = width;
    h = height;
    d = image.depth();
    is_null = (w <= 0 || h <= 0);

    if (pixelType() == BitmapType && !image.isNull()) {
        image.setColorTable(monoColorTable());
    }

    setSerialNumber(image.cacheKey() >> 32);
}

bool QRasterPixmapData::fromData(const uchar *buffer, uint len, const char *format,
                      Qt::ImageConversionFlags flags)
{
    QBuffer b;
    b.setData(reinterpret_cast<const char *>(buffer), len);
    b.open(QIODevice::ReadOnly);
    QImage image = QImageReader(&b, format).read();
    if (image.isNull())
        return false;

    fromImage(image, flags);
    return !isNull();
}

void QRasterPixmapData::fromImage(const QImage &sourceImage,
                                  Qt::ImageConversionFlags flags)
{
    QImage::Format format;
    if (pixelType() == BitmapType) {
        format = QImage::Format_MonoLSB;
    } else {
        if (sourceImage.depth() == 1) {
            format = sourceImage.hasAlphaChannel()
                    ? QImage::Format_ARGB32_Premultiplied
                    : QImage::Format_RGB32;
        } else {
            QImage::Format opaqueFormat = QImage::systemFormat();
            QImage::Format alphaFormat = QImage::Format_ARGB32_Premultiplied;

            if (!sourceImage.hasAlphaChannel()) {
                format = opaqueFormat;
            } else if ((flags & Qt::NoOpaqueDetection) == 0
                       && !sourceImage.d->checkForAlphaPixels())
            {
                format = opaqueFormat;
            } else {
                format = alphaFormat;
            }
        }
    }

    image = sourceImage.convertToFormat(format);

    if (image.d) {
        w = image.d->width;
        h = image.d->height;
        d = image.d->depth;
    } else {
        w = h = d = 0;
    }
    is_null = (w <= 0 || h <= 0);

    setSerialNumber(image.cacheKey()  >> 32);
}

void QRasterPixmapData::fromImageReader(QImageReader *imageReader,
                                        Qt::ImageConversionFlags flags)
{
    QImage image = imageReader->read();
    if (image.isNull())
        return;

    fromImage(image, flags);
}

// from qwindowsurface.cpp
extern void qt_scrollRectInImage(QImage *img, const QRect &rect, const QPoint &offset);

void QRasterPixmapData::copy(const QPixmapData *data, const QRect &rect)
{
    fromImage(data->toImage(rect), Qt::NoOpaqueDetection);
}

bool QRasterPixmapData::scroll(int dx, int dy, const QRect &rect)
{
    if (!image.isNull())
        qt_scrollRectInImage(&image, rect, QPoint(dx, dy));
    return true;
}

void QRasterPixmapData::fill(const QColor &color)
{
    if (color.alpha() != 255 && !image.hasAlphaChannel()) {
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    image.fill(color);
}

void QRasterPixmapData::setMask(const QBitmap &mask)
{
    if (mask.size().isEmpty()) {
        if (image.depth() != 1) { // hw: ????
            image = image.convertToFormat(QImage::Format_RGB32);
        }
    } else {
        const int w = image.width();
        const int h = image.height();

        switch (image.depth()) {
        case 1: {
            const QImage imageMask = mask.toImage().convertToFormat(image.format());
            const int bpl = image.bytesPerLine();
            uchar *dest = image.bits();
            for (int y = 0; y < h; ++y) {
                const uchar *mscan = imageMask.constScanLine(y);
                uchar *tscan = QFAST_SCAN_LINE(dest, bpl, y);
                for (int i = 0; i < bpl; ++i)
                    tscan[i] &= mscan[i];
            }
            break;
        }
        default: {
            const QImage imageMask = mask.toImage().convertToFormat(QImage::Format_MonoLSB);
            image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            const int bpl = image.bytesPerLine();
            uchar *dest = image.bits();
            for (int y = 0; y < h; ++y) {
                const uchar *mscan = imageMask.constScanLine(y);
                QRgb *tscan = reinterpret_cast<QRgb*>(QFAST_SCAN_LINE(dest, bpl, y));
                for (int x = 0; x < w; ++x) {
                    if (!(mscan[x>>3] & qt_pixmap_bit_mask[x&7]))
                        tscan[x] = 0;
                }
            }
            break;
        }
        }
    }
}

bool QRasterPixmapData::hasAlphaChannel() const
{
    return image.hasAlphaChannel();
}

QImage QRasterPixmapData::toImage() const
{
    if (!image.isNull()) {
        const QImageData *data = image.d;
        if (data->paintEngine && data->paintEngine->isActive()
            && data->paintEngine->paintDevice() == &image)
        {
            return image.copy();
        }
    }

    return image;
}

QImage QRasterPixmapData::toImage(const QRect &rect) const
{
    if (rect.isNull())
        return image;

    return image.copy(rect);
}

void QRasterPixmapData::setAlphaChannel(const QPixmap &alphaChannel)
{
    image.setAlphaChannel(alphaChannel.toImage());
}

QPaintEngine* QRasterPixmapData::paintEngine() const
{
    return image.paintEngine();
}

int QRasterPixmapData::metric(QPaintDevice::PaintDeviceMetric metric) const
{
    if (!image.d)
        return 0;

    // override the image dpi with the screen dpi when rendering to a pixmap
    switch (metric) {
    case QPaintDevice::PdmWidth:
        return w;
    case QPaintDevice::PdmHeight:
        return h;
    case QPaintDevice::PdmWidthMM:
        return qRound(image.d->width * 25.4 / QX11Info::appDpiX());
    case QPaintDevice::PdmHeightMM:
        return qRound(image.d->height * 25.4 / QX11Info::appDpiY());
    case QPaintDevice::PdmNumColors:
        return image.d->colortable.size();
    case QPaintDevice::PdmDepth:
        return depth();
    case QPaintDevice::PdmDpiX: // fall-through
    case QPaintDevice::PdmPhysicalDpiX:
        return QX11Info::appDpiX();
    case QPaintDevice::PdmDpiY: // fall-through
    case QPaintDevice::PdmPhysicalDpiY:
        return QX11Info::appDpiY();
    }

    qWarning("QRasterPixmapData::metric(): Unhandled metric type %d", metric);
    return 0;
}

QImage* QRasterPixmapData::buffer()
{
    return &image;
}

QT_END_NAMESPACE




