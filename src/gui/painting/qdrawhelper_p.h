/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#ifndef QDRAWHELPER_P_H
#define QDRAWHELPER_P_H

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

#include "QtGui/qcolor.h"
#include "QtGui/qpainter.h"
#include "QtGui/qimage.h"
#include "qrasterdefs_p.h"
#include "qmath.h"

QT_BEGIN_NAMESPACE

/*******************************************************************************
 * QSpan
 *
 * duplicate definition of FT_Span
 */
typedef QT_FT_Span QSpan;

struct QSpanData;
class QRasterBuffer;
class QClipData;
class QRasterPaintEngine;

typedef QT_FT_SpanFunc ProcessSpans;
typedef void (*BitmapBlitFunc)(QRasterBuffer *rasterBuffer,
                               int x, int y, quint32 color,
                               const uchar *bitmap,
                               int mapWidth, int mapHeight, int mapStride);

typedef void (*AlphamapBlitFunc)(QRasterBuffer *rasterBuffer,
                                 int x, int y, quint32 color,
                                 const uchar *bitmap,
                                 int mapWidth, int mapHeight, int mapStride,
                                 const QClipData *clip);

typedef void (*AlphaRGBBlitFunc)(QRasterBuffer *rasterBuffer,
                                 int x, int y, quint32 color,
                                 const uint *rgbmask,
                                 int mapWidth, int mapHeight, int mapStride,
                                 const QClipData *clip);

typedef void (*RectFillFunc)(QRasterBuffer *rasterBuffer,
                             int x, int y, int width, int height,
                             quint32 color);

struct DrawHelper {
    ProcessSpans blendColor;
    ProcessSpans blendGradient;
    BitmapBlitFunc bitmapBlit;
    AlphamapBlitFunc alphamapBlit;
    AlphaRGBBlitFunc alphaRGBBlit;
    RectFillFunc fillRect;
};

extern DrawHelper qDrawHelper[QImage::NImageFormats];

void qBlendTexture(int count, const QSpan *spans, void *userData);

typedef void (QT_FASTCALL *CompositionFunction)(uint *dest, const uint *src, int length, uint const_alpha);
typedef void (QT_FASTCALL *CompositionFunctionSolid)(uint *dest, int length, uint color, uint const_alpha);

struct LinearGradientValues
{
    qreal dx;
    qreal dy;
    qreal l;
    qreal off;
};

struct RadialGradientValues
{
    qreal dx;
    qreal dy;
    qreal dr;
    qreal sqrfr;
    qreal a;
    qreal inv2a;
    bool extended;
};

struct Operator;
typedef uint* (QT_FASTCALL *DestFetchProc)(uint *buffer, const QRasterBuffer *rasterBuffer, int x, int y, int length);
typedef void (QT_FASTCALL *DestStoreProc)(QRasterBuffer *rasterBuffer, int x, int y, const uint *buffer, int length);
typedef const uint* (QT_FASTCALL *SourceFetchProc)(uint *buffer, const Operator *o, const QSpanData *data, int y, int x, int length);

struct Operator
{
    QPainter::CompositionMode mode;
    DestFetchProc dest_fetch;
    DestStoreProc dest_store;
    SourceFetchProc src_fetch;
    CompositionFunctionSolid funcSolid;
    CompositionFunction func;
    union {
        LinearGradientValues linear;
        RadialGradientValues radial;
    };
};

struct QSolidData
{
    uint color;
};

struct QLinearGradientData
{
    struct {
        qreal x;
        qreal y;
    } origin;
    struct {
        qreal x;
        qreal y;
    } end;
};

struct QRadialGradientData
{
    struct {
        qreal x;
        qreal y;
        qreal radius;
    } center;
    struct {
        qreal x;
        qreal y;
        qreal radius;
    } focal;
};

struct QConicalGradientData
{
    struct {
        qreal x;
        qreal y;
    } center;
    qreal angle;
};

struct QGradientData
{
    QGradient::Spread spread;

    union {
        QLinearGradientData linear;
        QRadialGradientData radial;
        QConicalGradientData conical;
    };

#define GRADIENT_STOPTABLE_SIZE 1024
#define GRADIENT_STOPTABLE_SIZE_SHIFT 10

    uint* colorTable; //[GRADIENT_STOPTABLE_SIZE];

    bool alphaColor;
};

struct QTextureData
{
    const uchar *imageData;
    const uchar *scanLine(int y) const { return imageData + y*bytesPerLine; }

    int width;
    int height;
    // clip rect
    int x1;
    int y1;
    int x2;
    int y2;
    int bytesPerLine;
    QImage::Format format;
    const QVector<QRgb> *colorTable;
    bool hasAlpha;
    enum Type {
        Plain,
        Tiled
    };
    Type type;
    int const_alpha;
};

struct QSpanData
{
    QSpanData() : tempImage(Q_NULLPTR) {}
    ~QSpanData() { delete tempImage; }

    QRasterBuffer *rasterBuffer;
    ProcessSpans blend;
    ProcessSpans unclipped_blend;
    BitmapBlitFunc bitmapBlit;
    AlphamapBlitFunc alphamapBlit;
    AlphaRGBBlitFunc alphaRGBBlit;
    RectFillFunc fillRect;
    qreal m11, m12, m13, m21, m22, m23, m33, dx, dy;   // inverse xform matrix
    const QClipData *clip;
    enum Type {
        None,
        Solid,
        LinearGradient,
        RadialGradient,
        ConicalGradient,
        Texture
    } type;
    QTransform::TransformationType txop;
    bool bilinear;
    QImage *tempImage;
    union {
        QSolidData solid;
        QGradientData gradient;
        QTextureData texture;
    };

    void init(QRasterBuffer *rb, const QRasterPaintEngine *pe);
    void setup(const QBrush &brush, int alpha, QPainter::CompositionMode compositionMode);
    void setupMatrix(const QTransform &matrix, bool bilinear);
    void initTexture(const QImage *image, int alpha, QTextureData::Type = QTextureData::Plain, const QRect &sourceRect = QRect());
    void adjustSpanMethods();
};

#if QT_POINTER_SIZE == 8 // 64-bit versions
static inline uint INTERPOLATE_PIXEL_256(uint x, uint a, uint y, uint b) {
    quint64 t = (((quint64(x)) | ((quint64(x)) << 24)) & 0x00ff00ff00ff00ff) * a;
    t += (((quint64(y)) | ((quint64(y)) << 24)) & 0x00ff00ff00ff00ff) * b;
    t >>= 8;
    t &= 0x00ff00ff00ff00ff;
    return (uint(t)) | (uint(t >> 24));
}

static inline uint BYTE_MUL(uint x, uint a) {
    quint64 t = (((quint64(x)) | ((quint64(x)) << 24)) & 0x00ff00ff00ff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff00ff00ff) + 0x80008000800080) >> 8;
    t &= 0x00ff00ff00ff00ff;
    return (uint(t)) | (uint(t >> 24));
}

static inline uint PREMUL(uint x) {
    uint a = x >> 24;
    quint64 t = (((quint64(x)) | ((quint64(x)) << 24)) & 0x00ff00ff00ff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff00ff00ff) + 0x80008000800080) >> 8;
    t &= 0x000000ff00ff00ff;
    return (uint(t)) | (uint(t >> 24)) | (a << 24);
}

#else // 32-bit versions

static inline uint INTERPOLATE_PIXEL_256(uint x, uint a, uint y, uint b) {
    uint t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
    t >>= 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
    x &= 0xff00ff00;
    x |= t;
    return x;
}

static inline uint BYTE_MUL(uint x, uint a) {
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return x;
}

static inline uint PREMUL(uint x) {
    uint a = x >> 24;
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff) * a;
    x = (x + ((x >> 8) & 0xff) + 0x80);
    x &= 0xff00;
    x |= t | (a << 24);
    return x;
}
#endif // QT_POINTER_SIZE

#define INV_PREMUL(p)                                   \
    (qAlpha(p) == 0 ? 0 :                               \
    ((qAlpha(p) << 24)                                  \
     | (((255*qRed(p))/ qAlpha(p)) << 16)               \
     | (((255*qGreen(p)) / qAlpha(p))  << 8)            \
     | ((255*qBlue(p)) / qAlpha(p))))

#define ARGB_COMBINE_ALPHA(argb, alpha) \
    ((((argb >> 24) * alpha) >> 8) << 24) | (argb & 0x00ffffff)

class quint32p
{
public:
    inline quint32p(quint32 v) : data(PREMUL(v)) {}

    inline operator quint32() const { return data; }
    inline operator quint16() const { return data; }

    static inline quint32p fromRawData(quint32 v)
    {
        quint32p p;
        p.data = v;
        return p;
    }

private:
    quint32p() {}
    quint32 data;
};

class qrgb565;

class qargb8565
{
public:
    static inline bool hasAlpha() { return true; }

    inline qargb8565() {}
    inline qargb8565(quint32 v);
    inline explicit qargb8565(quint32p v);
    inline qargb8565(const qargb8565 &v);
    inline qargb8565(const qrgb565 &v);

    inline operator quint32() const;
    inline operator quint16() const;

    inline qargb8565 operator+(qargb8565 v) const;
    inline bool operator==(const qargb8565 &v) const;

    inline quint32 rawValue() const;

private:
    friend class qrgb565;
    quint8 data[3];
};

class qrgb565
{
public:
    static inline bool hasAlpha() { return false; }

    qrgb565(int v = 0) : data(v) {}

    inline explicit qrgb565(quint32p v);
    inline explicit qrgb565(quint32 v);
    inline explicit qrgb565(const qargb8565 &v);

    inline operator quint32() const;
    inline operator quint16() const;

    inline qrgb565 operator+(qrgb565 v) const;

    static inline quint8 alpha(quint8 a) { return (a + 1) >> 3; }
    static inline quint8 ialpha(quint8 a) { return 0x20 - alpha(a); }

    inline bool operator==(const qrgb565 &v) const;
    inline quint16 rawValue() const { return data; }

private:
    friend class qargb8565;
    quint16 data;
};

qargb8565::qargb8565(quint32 v)
{
    *this = qargb8565(quint32p(v));
}

qargb8565::qargb8565(quint32p v)
{
    data[0] = qAlpha(v);
    const int r = qRed(v);
    const int g = qGreen(v);
    const int b = qBlue(v);
    data[1] = ((g << 3) & 0xe0) | (b >> 3);
    data[2] = (r & 0xf8) | (g >> 5);
}

qargb8565::qargb8565(const qargb8565 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2];
}

qargb8565::qargb8565(const qrgb565 &v)
{
    data[0] = 0xff;
    data[1] = v.data & 0xff;
    data[2] = v.data >> 8;
}

qargb8565::operator quint32() const
{
    const quint16 rgb = (data[2] << 8) | data[1];
    const int a = data[0];
    const int r = (rgb & 0xf800);
    const int g = (rgb & 0x07e0);
    const int b = (rgb & 0x001f);
    const int tr = qMin(a, (r >> 8) | (r >> 13));
    const int tg = qMin(a, (g >> 3) | (g >> 9));
    const int tb = qMin(a, (b << 3) | (b >> 2));
    return qRgba(tr, tg, tb, data[0]);
}

qargb8565::operator quint16() const
{
    return (data[2] << 8) | data[1];
}

qargb8565 qargb8565::operator+(qargb8565 v) const
{
    qargb8565 t;
    t.data[0] = data[0] + v.data[0];
    const quint16 rgb =  ((data[2] + v.data[2]) << 8)
                         + (data[1] + v.data[1]);
    t.data[1] = rgb & 0xff;
    t.data[2] = rgb >> 8;
    return t;
}

bool qargb8565::operator==(const qargb8565 &v) const
{
    return data[0] == v.data[0]
        && data[1] == v.data[1]
        && data[2] == v.data[2];
}

quint32 qargb8565::rawValue() const
{
    return (data[2] << 16) | (data[1] << 8) | data[0];
}

qrgb565::qrgb565(quint32p v)
{
    *this = qrgb565(quint32(v));
}

qrgb565::qrgb565(quint32 v)
{
    const int r = qRed(v) << 8;
    const int g = qGreen(v) << 3;
    const int b = qBlue(v) >> 3;

    data = (r & 0xf800) | (g & 0x07e0)| (b & 0x001f);
}

qrgb565::qrgb565(const qargb8565 &v)
{
    data = (v.data[2] << 8) | v.data[1];
}

qrgb565::operator quint32() const
{
    const int r = (data & 0xf800);
    const int g = (data & 0x07e0);
    const int b = (data & 0x001f);
    const int tr = (r >> 8) | (r >> 13);
    const int tg = (g >> 3) | (g >> 9);
    const int tb = (b << 3) | (b >> 2);
    return qRgb(tr, tg, tb);
}

qrgb565::operator quint16() const
{
    return data;
}

qrgb565 qrgb565::operator+(qrgb565 v) const
{
    qrgb565 t;
    t.data = data + v.data;
    return t;
}

bool qrgb565::operator==(const qrgb565 &v) const
{
    return data == v.data;
}

class qrgb555;

class qargb8555
{
public:
    static inline bool hasAlpha() { return true; }

    qargb8555() {}
    inline qargb8555(quint32 v);
    inline explicit qargb8555(quint32p v);
    inline qargb8555(const qargb8555 &v);
    inline qargb8555(const qrgb555 &v);

    inline operator quint32() const;

    inline qargb8555 operator+(qargb8555 v) const;

    inline bool operator==(const qargb8555 &v) const;

private:
    friend class qrgb555;
    quint8 data[3];
};

class qrgb555
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb555(int v = 0) : data(v) {}

    inline explicit qrgb555(quint32p v) { *this = qrgb555(quint32(v)); }

    inline explicit qrgb555(quint32 v)
    {
        const int r = qRed(v) << 7;
        const int g = qGreen(v) << 2;
        const int b = qBlue(v) >> 3;

        data = (r & 0x7c00) | (g & 0x03e0) | (b & 0x001f);
    }

    inline explicit qrgb555(quint16 v)
    {
        data = ((v >> 1) & (0x7c00 | 0x03e0)) |
               (v & 0x001f);
    }

    inline explicit qrgb555(const qargb8555 &v);

    inline operator quint32() const
    {
        const int r = (data & 0x7c00);
        const int g = (data & 0x03e0);
        const int b = (data & 0x001f);
        const int tr = (r >> 7) | (r >> 12);
        const int tg = (g >> 2) | (g >> 7);
        const int tb = (b << 3) | (b >> 2);

        return qRgb(tr, tg, tb);
    }

    inline operator quint16() const
    {
        const int r = ((data & 0x7c00) << 1) & 0xf800;
        const int g = (((data & 0x03e0) << 1) | ((data >> 4) & 0x0020)) & 0x07e0;
        const int b = (data & 0x001f);

        return r | g | b;
    }

    inline qrgb555 operator+(qrgb555 v) const;

    inline bool operator==(const qrgb555 &v) const { return v.data == data; }
    inline bool operator!=(const qrgb555 &v) const { return v.data != data; }

private:
    friend class qargb8555;
    quint16 data;
};

qrgb555::qrgb555(const qargb8555 &v)
{
    data = (v.data[2] << 8) | v.data[1];
}

qrgb555 qrgb555::operator+(qrgb555 v) const
{
    qrgb555 t;
    t.data = data + v.data;
    return t;
}

qargb8555::qargb8555(quint32 v)
{
    v = quint32p(v);
    data[0] = qAlpha(v);
    const int r = qRed(v);
    const int g = qGreen(v);
    const int b = qBlue(v);
    data[1] = ((g << 2) & 0xe0) | (b >> 3);
    data[2] = ((r >> 1) & 0x7c) | (g >> 6);

}

qargb8555::qargb8555(quint32p v)
{
    data[0] = qAlpha(v);
    const int r = qRed(v);
    const int g = qGreen(v);
    const int b = qBlue(v);
    data[1] = ((g << 2) & 0xe0) | (b >> 3);
    data[2] = ((r >> 1) & 0x7c) | (g >> 6);
}

qargb8555::qargb8555(const qargb8555 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2];
}

qargb8555::qargb8555(const qrgb555 &v)
{
    data[0] = 0xff;
    data[1] = v.data & 0xff;
    data[2] = v.data >> 8;
}

qargb8555::operator quint32() const
{
    const quint16 rgb = (data[2] << 8) | data[1];
    const int r = (rgb & 0x7c00);
    const int g = (rgb & 0x03e0);
    const int b = (rgb & 0x001f);
    const int tr = (r >> 7) | (r >> 12);
    const int tg = (g >> 2) | (g >> 7);
    const int tb = (b << 3) | (b >> 2);

    return qRgba(tr, tg, tb, data[0]);
}

bool qargb8555::operator==(const qargb8555 &v) const
{
    return data[0] == v.data[0]
        && data[1] == v.data[1]
        && data[2] == v.data[2];
}

qargb8555 qargb8555::operator+(qargb8555 v) const
{
    qargb8555 t;
    t.data[0] = data[0] + v.data[0];
    const quint16 rgb =  ((data[2] + v.data[2]) << 8)
                         + (data[1] + v.data[1]);
    t.data[1] = rgb & 0xff;
    t.data[2] = rgb >> 8;
    return t;
}

class qrgb666;

class qargb6666
{
public:
    static inline bool hasAlpha() { return true; }

    inline qargb6666() {}
    inline qargb6666(quint32 v) { *this = qargb6666(quint32p(v)); }
    inline explicit qargb6666(quint32p v);
    inline qargb6666(const qargb6666 &v);
    inline qargb6666(const qrgb666 &v);

    inline operator quint32 () const;

    inline qargb6666 operator+(qargb6666 v) const;
    inline bool operator==(const qargb6666 &v) const;

private:
    friend class qrgb666;
    quint8 data[3];
};

class qrgb666
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb666() {}
    inline qrgb666(quint32 v);
    inline qrgb666(const qargb6666 &v);

    inline operator quint32 () const;

    inline qrgb666 operator+(qrgb666 v) const;

    inline bool operator==(const qrgb666 &v) const;
    inline bool operator!=(const qrgb666 &v) const { return !(*this == v); }

private:
    friend class qargb6666;
    quint8 data[3];
};

qrgb666::qrgb666(quint32 v)
{
    const uchar b = qBlue(v);
    const uchar g = qGreen(v);
    const uchar r = qRed(v);
    const uint p = (b >> 2) | ((g >> 2) << 6) | ((r >> 2) << 12);
    data[0] = qBlue(p);
    data[1] = qGreen(p);
    data[2] = qRed(p);
}

qrgb666::qrgb666(const qargb6666 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2] & 0x03;
}

qrgb666::operator quint32 () const
{
    const uchar r = (data[2] << 6) | ((data[1] & 0xf0) >> 2) | (data[2] & 0x3);
    const uchar g = (data[1] << 4) | ((data[0] & 0xc0) >> 4) | ((data[1] & 0x0f) >> 2);
    const uchar b = (data[0] << 2) | ((data[0] & 0x3f) >> 4);
    return qRgb(r, g, b);
}

qrgb666 qrgb666::operator+(qrgb666 v) const
{
    const quint32 x1 = (data[2] << 16) | (data[1] << 8) | data[0];
    const quint32 x2 = (v.data[2] << 16) | (v.data[1] << 8) | v.data[0];
    const quint32 t = x1 + x2;
    qrgb666 r;
    r.data[0] = t & 0xff;
    r.data[1] = (t >> 8) & 0xff;
    r.data[2] = (t >> 16) & 0xff;
    return r;
}

bool qrgb666::operator==(const qrgb666 &v) const
{
    return (data[0] == v.data[0] &&
            data[1] == v.data[1] &&
            data[2] == v.data[2]);
}

qargb6666::qargb6666(quint32p v)
{
    const quint8 b = qBlue(v) >> 2;
    const quint8 g = qGreen(v) >> 2;
    const quint8 r = qRed(v) >> 2;
    const quint8 a = qAlpha(v) >> 2;
    const uint p = (a << 18) | (r << 12) | (g << 6) | b;
    data[0] = qBlue(p);
    data[1] = qGreen(p);
    data[2] = qRed(p);
}

qargb6666::qargb6666(const qargb6666 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = v.data[2];
}

qargb6666::qargb6666(const qrgb666 &v)
{
    data[0] = v.data[0];
    data[1] = v.data[1];
    data[2] = (v.data[2] | 0xfc);
}

qargb6666::operator quint32 () const
{
    const quint8 r = (data[2] << 6) | ((data[1] & 0xf0) >> 2) | (data[2] & 0x3);
    const quint8 g = (data[1] << 4) | ((data[0] & 0xc0) >> 4) | ((data[1] & 0x0f) >> 2);
    const quint8 b = (data[0] << 2) | ((data[0] & 0x3f) >> 4);
    const quint8 a = (data[2] & 0xfc) | (data[2] >> 6);
    return qRgba(r, g, b, a);
}

qargb6666 qargb6666::operator+(qargb6666 v) const
{
    const quint32 x1 = (data[2] << 16) | (data[1] << 8) | data[0];
    const quint32 x2 = (v.data[2] << 16) | (v.data[1] << 8) | v.data[0];
    const quint32 t = x1 + x2;
    qargb6666 r;
    r.data[0] = t & 0xff;
    r.data[1] = (t >> 8) & 0xff;
    r.data[2] = (t >> 16) & 0xff;
    return r;
}

bool qargb6666::operator==(const qargb6666 &v) const
{
    return data[0] == v.data[0]
        && data[1] == v.data[1]
        && data[2] == v.data[2];
}

class qrgb888
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb888() {}
    inline qrgb888(quint32 v);

    inline operator quint32() const;

    inline qrgb888 operator+(qrgb888 v) const;
    inline bool operator==(qrgb888 v) const;

private:
    uchar data[3];
};

qrgb888::qrgb888(quint32 v)
{
    data[0] = qRed(v);
    data[1] = qGreen(v);
    data[2] = qBlue(v);
}

qrgb888::operator quint32() const
{
    return qRgb(data[0], data[1], data[2]);
}

qrgb888 qrgb888::operator+(qrgb888 v) const
{
    qrgb888 t = *this;
    t.data[0] += v.data[0];
    t.data[1] += v.data[1];
    t.data[2] += v.data[2];
    return t;
}

bool qrgb888::operator==(qrgb888 v) const
{
    return (data[0] == v.data[0] &&
            data[1] == v.data[1] &&
            data[2] == v.data[2]);
}

// hw: endianess??
class quint24
{
public:
    inline quint24(quint32 v)
    {
        data[0] = qBlue(v);
        data[1] = qGreen(v);
        data[2] = qRed(v);
    }

    inline operator quint32 () const
    {
        return qRgb(data[2], data[1], data[0]);
    }

    inline bool operator==(const quint24 &v) const
    {
        return data[0] == v.data[0]
            && data[1] == v.data[1]
            && data[2] == v.data[2];
    }

private:
    uchar data[3];
};

class qrgb444;

class qargb4444
{
public:
    static inline bool hasAlpha() { return true; }

    inline qargb4444() {}
    inline qargb4444(quint32 v) { *this = qargb4444(quint32p(v)); }
    inline explicit qargb4444(quint32p v);
    inline qargb4444(const qrgb444 &v);

    inline operator quint32() const;
    inline operator quint8() const;

    inline qargb4444 operator+(qargb4444 v) const;

    inline bool operator==(const qargb4444 &v) const { return data == v.data; }

private:
    friend class qrgb444;
    quint16 data;
};

class qrgb444
{
public:
    static inline bool hasAlpha() { return false; }

    inline qrgb444() {}
    inline qrgb444(quint32 v);
    inline explicit qrgb444(qargb4444 v);

    inline operator quint32() const;
    inline operator quint8() const;

    inline qrgb444 operator+(qrgb444 v) const;

    inline bool operator==(const qrgb444 &v) const { return data == v.data; }
    inline bool operator!=(const qrgb444 &v) const { return data != v.data; }

private:
    friend class qargb4444;
    quint16 data;
};


qargb4444::qargb4444(quint32p color)
{
    quint32 v = color;
    v &= 0xf0f0f0f0;
    const int a = qAlpha(v) << 8;
    const int r = qRed(v) << 4;
    const int g = qGreen(v);
    const int b = qBlue(v) >> 4;

    data = a | r | g | b;
}

qargb4444::qargb4444(const qrgb444 &v)
{
    data = v.data | 0xf000;
}

qargb4444::operator quint32() const
{
    const int a = (data & 0xf000);
    const int r = (data & 0x0f00);
    const int g = (data & 0x00f0);
    const int b = (data & 0x000f);
    const int ta = (a >> 8) | (a >> 12);
    const int tr = (r >> 4) | (r >> 8);
    const int tg = g | (g >> 4);
    const int tb = (b << 4) | b;

    return qRgba(tr, tg, tb, ta);
}

qargb4444::operator quint8() const
{
    // hw: optimize!
    return operator quint32();
}

qargb4444 qargb4444::operator+(qargb4444 v) const
{
    qargb4444 t;
    t.data = data + v.data;
    return t;
}

qrgb444::qrgb444(quint32 v)
{
    v &= 0xf0f0f0f0;
    const int r = qRed(v) << 4;
    const int g = qGreen(v);
    const int b = qBlue(v) >> 4;

    data = r | g | b;
}

qrgb444::qrgb444(qargb4444 v)
{
    data = v.data & 0x0fff;
}

qrgb444::operator quint32() const
{
    const int r = (data & 0x0f00);
    const int g = (data & 0x00f0);
    const int b = (data & 0x000f);
    const int tr = (r >> 4) | (r >> 8);
    const int tg = g | (g >> 4);
    const int tb = (b << 4) | b;

    return qRgb(tr, tg, tb);
}

qrgb444::operator quint8() const
{
    // hw: optimize!
    return operator quint32();
}

qrgb444 qrgb444::operator+(qrgb444 v) const
{
    qrgb444 t;
    t.data = data + v.data;
    return t;
}


template <class DST, class SRC>
inline DST qt_colorConvert(const SRC color, const DST dummy)
{
    Q_UNUSED(dummy);
    return DST(color);
}

template <>
inline quint32 qt_colorConvert(const quint16 color, const quint32 dummy)
{
    Q_UNUSED(dummy);
    const int r = (color & 0xf800);
    const int g = (color & 0x07e0);
    const int b = (color & 0x001f);
    const int tr = (r >> 8) | (r >> 13);
    const int tg = (g >> 3) | (g >> 9);
    const int tb = (b << 3) | (b >> 2);

    return qRgb(tr, tg, tb);
}

template <>
inline quint16 qt_colorConvert(const quint32 color, const quint16 dummy)
{
    Q_UNUSED(dummy);
    const int r = qRed(color) << 8;
    const int g = qGreen(color) << 3;
    const int b = qBlue(color) >> 3;

    return (r & 0xf800) | (g & 0x07e0)| (b & 0x001f);
}

template <class T>
inline void qt_memfill(T *dest, const T color, int count)
{
    while (count--)
        *dest++ = color;
}

template <class DST, class SRC>
inline void qt_memfill(DST *dest, const SRC color, int count)
{
    const DST c = qt_colorConvert<DST, SRC>(color, 0);
    while (count--)
        *dest++ = c;
}

template <class T>
inline void qt_rectfill(T *dest, const T value,
                        int x, int y, int width, int height, int stride)
{
    char *d = reinterpret_cast<char*>(dest + x) + y * stride;
    if (uint(stride) == (width * sizeof(T))) {
        qt_memfill(reinterpret_cast<T*>(d), value, width * height);
    } else {
        for (int j = 0; j < height; ++j) {
            dest = reinterpret_cast<T*>(d);
            qt_memfill(dest, value, width);
            d += stride;
        }
    }
}

template <class DST, class SRC>
inline void qt_memconvert(DST *dest, const SRC *src, int count)
{
    if (sizeof(DST) == 1) {
        while (count) {
            int n = 1;
            const SRC color = *src++;
            const DST dstColor = qt_colorConvert<DST, SRC>(color, 0);
            while (--count && (*src == color || dstColor == qt_colorConvert<DST, SRC>(*src, 0))) {
                ++n;
                ++src;
            }
            qt_memfill(dest, dstColor, n);
            dest += n;
        }
    } else {
        /* Duff's device */
        int n = (count + 7) / 8;
        switch (count & 0x07)
        {
        case 0: do { *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 7:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 6:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 5:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 4:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 3:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 2:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            case 1:      *dest++ = qt_colorConvert<DST, SRC>(*src++, 0);
            } while (--n > 0);
        }
    }
}

#define QT_TRIVIAL_MEMCONVERT_IMPL(T) \
    template <> \
    inline void qt_memconvert(T *dest, const T *src, int count) \
    { \
        ::memcpy(dest, src, count * sizeof(T)); \
    }
QT_TRIVIAL_MEMCONVERT_IMPL(quint32)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb888)
QT_TRIVIAL_MEMCONVERT_IMPL(qargb6666)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb666)
QT_TRIVIAL_MEMCONVERT_IMPL(quint16)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb565)
QT_TRIVIAL_MEMCONVERT_IMPL(qargb8565)
QT_TRIVIAL_MEMCONVERT_IMPL(qargb8555)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb555)
QT_TRIVIAL_MEMCONVERT_IMPL(qargb4444)
QT_TRIVIAL_MEMCONVERT_IMPL(qrgb444)
#undef QT_TRIVIAL_MEMCONVERT_IMPL

template <class DST, class SRC>
inline void qt_rectconvert(DST *dest, const SRC *src,
                           int width, int height,
                           int dstStride, int srcStride)
{
    char *d = (char*)dest;
    const char *s = (const char*)(src);
    for (int i = 0; i < height; ++i) {
        qt_memconvert<DST,SRC>((DST*)d, (const SRC*)s, width);
        d += dstStride;
        s += srcStride;
    }
}

#define QT_RECTCONVERT_TRIVIAL_IMPL(T)                                  \
    template <>                                                         \
    inline void qt_rectconvert(T *dest, const T *src,                   \
                               int width, int height,                   \
                               int dstStride, int srcStride)            \
    {                                                                   \
        char *d = (char*)dest; \
        const char *s = (const char*)(src); \
        for (int i = 0; i < height; ++i) { \
            ::memcpy(d, s, width * sizeof(T)); \
            d += dstStride; \
            s += srcStride; \
        } \
    }

QT_RECTCONVERT_TRIVIAL_IMPL(quint32)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb888)
QT_RECTCONVERT_TRIVIAL_IMPL(qargb6666)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb666)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb565)
QT_RECTCONVERT_TRIVIAL_IMPL(qargb8565)
QT_RECTCONVERT_TRIVIAL_IMPL(quint16)
QT_RECTCONVERT_TRIVIAL_IMPL(qargb8555)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb555)
QT_RECTCONVERT_TRIVIAL_IMPL(qargb4444)
QT_RECTCONVERT_TRIVIAL_IMPL(qrgb444)
#undef QT_RECTCONVERT_TRIVIAL_IMPL

static inline int qt_div_255(int x) { return (x + (x>>8) + 0x80) >> 8; }

static const uint qt_bayer_matrix[16][16] = {
    { 0x1, 0xc0, 0x30, 0xf0, 0xc, 0xcc, 0x3c, 0xfc,
      0x3, 0xc3, 0x33, 0xf3, 0xf, 0xcf, 0x3f, 0xff},
    { 0x80, 0x40, 0xb0, 0x70, 0x8c, 0x4c, 0xbc, 0x7c,
      0x83, 0x43, 0xb3, 0x73, 0x8f, 0x4f, 0xbf, 0x7f},
    { 0x20, 0xe0, 0x10, 0xd0, 0x2c, 0xec, 0x1c, 0xdc,
      0x23, 0xe3, 0x13, 0xd3, 0x2f, 0xef, 0x1f, 0xdf},
    { 0xa0, 0x60, 0x90, 0x50, 0xac, 0x6c, 0x9c, 0x5c,
      0xa3, 0x63, 0x93, 0x53, 0xaf, 0x6f, 0x9f, 0x5f},
    { 0x8, 0xc8, 0x38, 0xf8, 0x4, 0xc4, 0x34, 0xf4,
      0xb, 0xcb, 0x3b, 0xfb, 0x7, 0xc7, 0x37, 0xf7},
    { 0x88, 0x48, 0xb8, 0x78, 0x84, 0x44, 0xb4, 0x74,
      0x8b, 0x4b, 0xbb, 0x7b, 0x87, 0x47, 0xb7, 0x77},
    { 0x28, 0xe8, 0x18, 0xd8, 0x24, 0xe4, 0x14, 0xd4,
      0x2b, 0xeb, 0x1b, 0xdb, 0x27, 0xe7, 0x17, 0xd7},
    { 0xa8, 0x68, 0x98, 0x58, 0xa4, 0x64, 0x94, 0x54,
      0xab, 0x6b, 0x9b, 0x5b, 0xa7, 0x67, 0x97, 0x57},
    { 0x2, 0xc2, 0x32, 0xf2, 0xe, 0xce, 0x3e, 0xfe,
      0x1, 0xc1, 0x31, 0xf1, 0xd, 0xcd, 0x3d, 0xfd},
    { 0x82, 0x42, 0xb2, 0x72, 0x8e, 0x4e, 0xbe, 0x7e,
      0x81, 0x41, 0xb1, 0x71, 0x8d, 0x4d, 0xbd, 0x7d},
    { 0x22, 0xe2, 0x12, 0xd2, 0x2e, 0xee, 0x1e, 0xde,
      0x21, 0xe1, 0x11, 0xd1, 0x2d, 0xed, 0x1d, 0xdd},
    { 0xa2, 0x62, 0x92, 0x52, 0xae, 0x6e, 0x9e, 0x5e,
      0xa1, 0x61, 0x91, 0x51, 0xad, 0x6d, 0x9d, 0x5d},
    { 0xa, 0xca, 0x3a, 0xfa, 0x6, 0xc6, 0x36, 0xf6,
      0x9, 0xc9, 0x39, 0xf9, 0x5, 0xc5, 0x35, 0xf5},
    { 0x8a, 0x4a, 0xba, 0x7a, 0x86, 0x46, 0xb6, 0x76,
      0x89, 0x49, 0xb9, 0x79, 0x85, 0x45, 0xb5, 0x75},
    { 0x2a, 0xea, 0x1a, 0xda, 0x26, 0xe6, 0x16, 0xd6,
      0x29, 0xe9, 0x19, 0xd9, 0x25, 0xe5, 0x15, 0xd5},
    { 0xaa, 0x6a, 0x9a, 0x5a, 0xa6, 0x66, 0x96, 0x56,
      0xa9, 0x69, 0x99, 0x59, 0xa5, 0x65, 0x95, 0x55}
};

QT_END_NAMESPACE

#endif // QDRAWHELPER_P_H
