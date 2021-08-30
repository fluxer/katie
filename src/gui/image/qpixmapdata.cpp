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
#include "qbuffer.h"
#include "qbitmap.h"
#include "qimagereader.h"
#include "qpixmap_raster_p.h"
#include "qapplication_p.h"
#include "qdrawhelper_p.h"
#include "qguicommon_p.h"

QT_BEGIN_NAMESPACE

QPixmapData *QPixmapData::create(int w, int h, PixelType type)
{
    QPixmapData *data = new QRasterPixmapData(type);
    data->resize(w, h);
    return data;
}


QPixmapData::QPixmapData(PixelType pixelType, ClassId objectId)
    : w(0),
      h(0),
      d(0),
      is_null(true),
      ref(0),
      detach_no(0),
      type(pixelType),
      id(objectId),
      ser_no(0)
{
}

QPixmapData::~QPixmapData()
{
}

QPixmapData *QPixmapData::createCompatiblePixmapData() const
{
    return new QRasterPixmapData(pixelType());
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
    fromImage(imageReader->read(), flags);
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

bool QPixmapData::fromData(const uchar *buf, uint len, const char *format, Qt::ImageConversionFlags flags)
{
    QBuffer b;
    b.setData(reinterpret_cast<const char *>(buf), len);
    b.open(QIODevice::ReadOnly);
    QImage image = QImageReader(&b, format).read();
    fromImage(makeBitmapCompliantIfNeeded(this, image, flags), flags);
    return !isNull();
}

void QPixmapData::copy(const QPixmapData *data, const QRect &rect)
{
    fromImage(data->toImage(rect), Qt::NoOpaqueDetection);
}

bool QPixmapData::scroll(int dx, int dy, const QRect &rect)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);
    Q_UNUSED(rect);
    return false;
}

void QPixmapData::setMask(const QBitmap &mask)
{
    if (mask.size().isEmpty()) {
        if (depth() != 1)
            fromImage(toImage().convertToFormat(QImage::Format_RGB32),
                      Qt::AutoColor);
    } else {
        QImage image = toImage();
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
        fromImage(image, Qt::AutoColor);
    }
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

QPixmap QPixmapData::transformed(const QTransform &matrix,
                                 Qt::TransformationMode mode) const
{
    return QPixmap::fromImage(toImage().transformed(matrix, mode));
}

void QPixmapData::setAlphaChannel(const QPixmap &alphaChannel)
{
    QImage image = toImage();
    image.setAlphaChannel(alphaChannel.toImage());
    fromImage(image, Qt::AutoColor);
}

QPixmap QPixmapData::alphaChannel() const
{
    return QPixmap::fromImage(toImage().alphaChannel());
}

void QPixmapData::setSerialNumber(int serNo)
{
    ser_no = serNo;
}

QImage QPixmapData::toImage(const QRect &rect) const
{
    return toImage().copy(rect);
}

QImage* QPixmapData::buffer()
{
    return 0;
}


QT_END_NAMESPACE




