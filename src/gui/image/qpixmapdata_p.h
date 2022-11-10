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

#ifndef QPIXMAPDATA_P_H
#define QPIXMAPDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGui/qpixmap.h>
#include <QtCore/qatomic.h>


QT_BEGIN_NAMESPACE

class QImageReader;

class Q_GUI_EXPORT QPixmapData
{
public:
    enum PixelType {
        // WARNING: Do not change the first two
        // Must match QPixmap::Type
        PixmapType, BitmapType
    };

    QPixmapData(PixelType pixelType);
    QPixmapData(int w, int h, PixelType type);
    ~QPixmapData();

    void fromImage(const QImage &image,
                   Qt::ImageConversionFlags flags);
    void fromImageReader(QImageReader *imageReader,
                         Qt::ImageConversionFlags flags);

    bool fromFile(const QString &filename, const char *format,
                  Qt::ImageConversionFlags flags);
    bool fromData(const uchar *buffer, uint len, const char *format,
                  Qt::ImageConversionFlags flags);

    void copy(const QPixmapData *data, const QRect &rect);
    void scroll(int dx, int dy, const QRect &rect);

    int metric(QPaintDevice::PaintDeviceMetric metric) const;
    void fill(const QColor &color);
    QBitmap mask() const;
    void setMask(const QBitmap &mask);
    bool hasAlphaChannel() const;
    void setAlphaChannel(const QPixmap &alphaChannel);
    QPixmap transformed(const QTransform &matrix,
                        Qt::TransformationMode mode) const;
    QPixmap alphaChannel() const;
    QImage toImage() const;
    QImage toImage(const QRect &rect) const;
    QPaintEngine* paintEngine() const;

    inline PixelType pixelType() const { return type; }

    QImage* buffer();

    inline int width() const { return image.width(); }
    inline int height() const { return image.height(); }
    inline int depth() const { return image.depth(); }
    inline bool isNull() const { return image.isNull(); }
    inline qint64 cacheKey() const {
        return ((static_cast<qint64>(ser_no) << 32)
                | (static_cast<qint64>(detach_no)));
    }

protected:
    void setSerialNumber(int serNo);

private:
    friend class QPixmap;
    friend class QRasterPaintEngine;
    friend class QExplicitlySharedDataPointer<QPixmapData>;

    QAtomicInt ref;
    QImage image;
    int detach_no;
    int ser_no;
    PixelType type;
};

QT_END_NAMESPACE

#endif // QPIXMAPDATA_P_H
