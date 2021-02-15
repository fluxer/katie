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
    enum ClassId { RasterClass, X11Class };

    QPixmapData(PixelType pixelType, ClassId classId);
    virtual ~QPixmapData();

    virtual QPixmapData *createCompatiblePixmapData() const;

    virtual void resize(int width, int height) = 0;
    virtual void fromImage(const QImage &image,
                           Qt::ImageConversionFlags flags) = 0;
    virtual void fromImageReader(QImageReader *imageReader,
                                 Qt::ImageConversionFlags flags);

    virtual bool fromFile(const QString &filename, const char *format,
                          Qt::ImageConversionFlags flags);
    virtual bool fromData(const uchar *buffer, uint len, const char *format,
                          Qt::ImageConversionFlags flags);

    virtual void copy(const QPixmapData *data, const QRect &rect);
    virtual bool scroll(int dx, int dy, const QRect &rect);

    virtual int metric(QPaintDevice::PaintDeviceMetric metric) const = 0;
    virtual void fill(const QColor &color) = 0;
    virtual QBitmap mask() const;
    virtual void setMask(const QBitmap &mask);
    virtual bool hasAlphaChannel() const = 0;
    virtual QPixmap transformed(const QTransform &matrix,
                                Qt::TransformationMode mode) const;
    virtual void setAlphaChannel(const QPixmap &alphaChannel);
    virtual QPixmap alphaChannel() const;
    virtual QImage toImage() const = 0;
    virtual QImage toImage(const QRect &rect) const;
    virtual QPaintEngine* paintEngine() const = 0;

    inline PixelType pixelType() const { return type; }
    inline ClassId classId() const { return id; }

    virtual QImage* buffer();

    inline int width() const { return w; }
    inline int height() const { return h; }
    inline int depth() const { return d; }
    inline bool isNull() const { return is_null; }
    inline qint64 cacheKey() const {
        return ((static_cast<qint64>(id) << 56)
                | (static_cast<qint64>(ser_no) << 32)
                | (static_cast<qint64>(detach_no)));
    }


    static QPixmapData *create(int w, int h, PixelType type);

protected:
    void setSerialNumber(int serNo);
    int w;
    int h;
    int d;
    bool is_null;

private:
    friend class QPixmap;
    friend class QX11PixmapData;
    friend class QExplicitlySharedDataPointer<QPixmapData>;

    QAtomicInt ref;
    int detach_no;

    PixelType type;
    ClassId id;
    int ser_no;
};

#define QT_XFORM_TYPE_MSBFIRST 0
#define QT_XFORM_TYPE_LSBFIRST 1
extern bool qt_xForm_helper(const QTransform&, int, int, int, uchar*, int, int, int, const uchar*, int, int, int);

QT_END_NAMESPACE

#endif // QPIXMAPDATA_P_H
