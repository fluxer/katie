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

class QSpanData;
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

    uint colorTable[GRADIENT_STOPTABLE_SIZE];
    bool alphaColor;

    void generateGradientColorTable(const QGradient& g, int opacity);
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

class QSpanData
{
public:
    QSpanData() : tempImage(nullptr) {}
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

static inline uint INV_PREMUL(uint x) {
    int alpha = qAlpha(x);

    if (alpha == 0)
        return 0;

    return ((alpha << 24)
            | (((255*qRed(x))/ alpha) << 16)
            | (((255*qGreen(x)) / alpha)  << 8)
            | ((255*qBlue(x)) / alpha));
}

#define ARGB_COMBINE_ALPHA(argb, alpha) \
    ((((argb >> 24) * alpha) >> 8) << 24) | (argb & 0x00ffffff)

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

template <class T>
inline void qt_rectfill(T *dest, const T value,
                        int x, int y, int width, int height, int stride)
{
    char *d = reinterpret_cast<char*>(dest + x) + y * stride;
    if (stride == (width * sizeof(T))) {
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
    // quint32 to quint16 and vise-versa
    Q_ASSERT(sizeof(DST) != sizeof(SRC));
    Q_ASSERT(sizeof(DST) == sizeof(quint16) || sizeof(DST) == sizeof(quint32));
    Q_ASSERT(sizeof(SRC) == sizeof(quint16) || sizeof(SRC) == sizeof(quint32));
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

// avoid going through QImage::scanLine() which calls detach
#define QFAST_SCAN_LINE(data, bpl, y) (data + y * bpl)

QT_END_NAMESPACE

#endif // QDRAWHELPER_P_H
