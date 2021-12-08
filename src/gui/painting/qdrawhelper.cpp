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

#include "qdrawhelper_p.h"
#include "qpaintengine_raster_p.h"
#include "qpainter_p.h"
#include "qmath.h"
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

// must be multiple of 4 for easier SIMD implementations
static const int buffer_size = 2048;

static inline uint INTERPOLATE_PIXEL_255(uint x, uint a, uint y, uint b) {
    uint t = (x & 0xff00ff) * a + (y & 0xff00ff) * b;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff00ff) * a + ((y >> 8) & 0xff00ff) * b;
    x = (x + ((x >> 8) & 0xff00ff) + 0x800080);
    x &= 0xff00ff00;
    x |= t;
    return x;
}

static inline uint BYTE_MUL_RGB16(uint x, uint a) {
    a += 1;
    uint t = (((x & 0x07e0)*a) >> 8) & 0x07e0;
    t |= (((x & 0xf81f)*(a>>2)) >> 6) & 0xf81f;
    return t;
}

static inline QRgb qConvertRgb16To32(uint c)
{
    return 0xff000000
        | ((((c) << 3) & 0xf8) | (((c) >> 2) & 0x7))
        | ((((c) << 5) & 0xfc00) | (((c) >> 1) & 0x300))
        | ((((c) << 8) & 0xf80000) | (((c) << 3) & 0x70000));
}

void QGradientData::generateGradientColorTable(const QGradient& gradient, int opacity)
{
    QGradientStops stops = gradient.stops();
    int stopCount = stops.count();
    Q_ASSERT(stopCount > 0);

    bool colorInterpolation = (gradient.interpolationMode() == QGradient::ColorInterpolation);

    if (stopCount == 2) {
        uint first_color = ARGB_COMBINE_ALPHA(stops[0].second.rgba(), opacity);
        uint second_color = ARGB_COMBINE_ALPHA(stops[1].second.rgba(), opacity);

        qreal first_stop = stops[0].first;
        qreal second_stop = stops[1].first;

        if (second_stop < first_stop) {
            qSwap(first_color, second_color);
            qSwap(first_stop, second_stop);
        }

        if (colorInterpolation) {
            first_color = PREMUL(first_color);
            second_color = PREMUL(second_color);
        }

        int first_index = qRound(first_stop * (GRADIENT_STOPTABLE_SIZE-1));
        int second_index = qRound(second_stop * (GRADIENT_STOPTABLE_SIZE-1));

        uint red_first = qRed(first_color) << 16;
        uint green_first = qGreen(first_color) << 16;
        uint blue_first = qBlue(first_color) << 16;
        uint alpha_first = qAlpha(first_color) << 16;

        uint red_second = qRed(second_color) << 16;
        uint green_second = qGreen(second_color) << 16;
        uint blue_second = qBlue(second_color) << 16;
        uint alpha_second = qAlpha(second_color) << 16;

        int i = 0;
        for (; i <= qMin(GRADIENT_STOPTABLE_SIZE, first_index); ++i) {
            if (colorInterpolation)
                colorTable[i] = first_color;
            else
                colorTable[i] = PREMUL(first_color);
        }

        if (i < second_index) {
            qreal reciprocal = qreal(1) / (second_index - first_index);

            int red_delta = qRound(int(red_second - red_first) * reciprocal);
            int green_delta = qRound(int(green_second - green_first) * reciprocal);
            int blue_delta = qRound(int(blue_second - blue_first) * reciprocal);
            int alpha_delta = qRound(int(alpha_second - alpha_first) * reciprocal);

            // rounding
            red_first += 1 << 15;
            green_first += 1 << 15;
            blue_first += 1 << 15;
            alpha_first += 1 << 15;

            for (; i < qMin(GRADIENT_STOPTABLE_SIZE, second_index); ++i) {
                red_first += red_delta;
                green_first += green_delta;
                blue_first += blue_delta;
                alpha_first += alpha_delta;

                const uint color = ((alpha_first << 8) & 0xff000000) | (red_first & 0xff0000)
                                 | ((green_first >> 8) & 0xff00) | (blue_first >> 16);

                if (colorInterpolation)
                    colorTable[i] = color;
                else
                    colorTable[i] = PREMUL(color);
            }
        }

        for (; i < GRADIENT_STOPTABLE_SIZE; ++i) {
            if (colorInterpolation)
                colorTable[i] = second_color;
            else
                colorTable[i] = PREMUL(second_color);
        }

        return;
    }

    uint current_color = ARGB_COMBINE_ALPHA(stops[0].second.rgba(), opacity);
    if (stopCount == 1) {
        current_color = PREMUL(current_color);
        for (int i = 0; i < GRADIENT_STOPTABLE_SIZE; ++i)
            colorTable[i] = current_color;
        return;
    }

    // The position where the gradient begins and ends
    qreal begin_pos = stops[0].first;
    qreal end_pos = stops[stopCount-1].first;

    int pos = 0; // The position in the color table.
    uint next_color;

    qreal incr = 1 / qreal(GRADIENT_STOPTABLE_SIZE); // the double increment.
    qreal dpos = 1.5 * incr; // current position in gradient stop list (0 to 1)

     // Up to first point
    colorTable[pos++] = PREMUL(current_color);
    while (dpos <= begin_pos) {
        colorTable[pos] = colorTable[pos - 1];
        ++pos;
        dpos += incr;
    }

    int current_stop = 0; // We always interpolate between current and current + 1.

    qreal t; // position between current left and right stops
    qreal t_delta; // the t increment per entry in the color table

    if (dpos < end_pos) {
        // Gradient area
        while (dpos > stops[current_stop+1].first)
            ++current_stop;

        if (current_stop != 0)
            current_color = ARGB_COMBINE_ALPHA(stops[current_stop].second.rgba(), opacity);
        next_color = ARGB_COMBINE_ALPHA(stops[current_stop+1].second.rgba(), opacity);

        if (colorInterpolation) {
            current_color = PREMUL(current_color);
            next_color = PREMUL(next_color);
        }

        qreal diff = stops[current_stop+1].first - stops[current_stop].first;
        qreal c = (diff == 0) ? qreal(0) : 256 / diff;
        t = (dpos - stops[current_stop].first) * c;
        t_delta = incr * c;

        while (true) {
            Q_ASSERT(current_stop < stopCount);

            int dist = qRound(t);
            int idist = 256 - dist;

            if (colorInterpolation)
                colorTable[pos] = INTERPOLATE_PIXEL_256(current_color, idist, next_color, dist);
            else
                colorTable[pos] = PREMUL(INTERPOLATE_PIXEL_256(current_color, idist, next_color, dist));

            ++pos;
            dpos += incr;

            if (dpos >= end_pos)
                break;

            t += t_delta;

            int skip = 0;
            while (dpos > stops[current_stop+skip+1].first)
                ++skip;

            if (skip != 0) {
                current_stop += skip;
                if (skip == 1)
                    current_color = next_color;
                else
                    current_color = ARGB_COMBINE_ALPHA(stops[current_stop].second.rgba(), opacity);
                next_color = ARGB_COMBINE_ALPHA(stops[current_stop+1].second.rgba(), opacity);

                if (colorInterpolation) {
                    if (skip != 1)
                        current_color = PREMUL(current_color);
                    next_color = PREMUL(next_color);
                }

                qreal diff = stops[current_stop+1].first - stops[current_stop].first;
                qreal c = (diff == 0) ? qreal(0) : 256 / diff;
                t = (dpos - stops[current_stop].first) * c;
                t_delta = incr * c;
            }
        }
    }

    // After last point
    current_color = PREMUL(ARGB_COMBINE_ALPHA(stops[stopCount - 1].second.rgba(), opacity));
    while (pos < GRADIENT_STOPTABLE_SIZE - 1) {
        colorTable[pos] = current_color;
        ++pos;
    }

    // Make sure the last color stop is represented at the end of the table
    colorTable[GRADIENT_STOPTABLE_SIZE - 1] = current_color;
}

/*
  Destination fetch. This is simple as we don't have to do bounds checks or
  transformations
*/

static uint * QT_FASTCALL destFetchMono(uint *buffer, const QRasterBuffer *rasterBuffer, int x, int y, int length)
{
    const uchar *data = (const uchar *)rasterBuffer->scanLine(y);
    uint *start = buffer;
    const uint *end = buffer + length;
    while (buffer < end) {
        *buffer = data[x>>3] & (0x80 >> (x & 7)) ? rasterBuffer->destColor1 : rasterBuffer->destColor0;
        ++buffer;
        ++x;
    }
    return start;
}

static uint * QT_FASTCALL destFetchMonoLsb(uint *buffer, const QRasterBuffer *rasterBuffer, int x, int y, int length)
{
    const uchar *data = (const uchar *)rasterBuffer->scanLine(y);
    uint *start = buffer;
    const uint *end = buffer + length;
    while (buffer < end) {
        *buffer = data[x>>3] & (0x1 << (x & 7)) ? rasterBuffer->destColor1 : rasterBuffer->destColor0;
        ++buffer;
        ++x;
    }
    return start;
}

static uint * QT_FASTCALL destFetchARGB32(uint *buffer, const QRasterBuffer *rasterBuffer, int x, int y, int length)
{
    const uint *data = (const uint *)rasterBuffer->scanLine(y) + x;
    for (int i = 0; i < length; ++i)
        buffer[i] = PREMUL(data[i]);
    return buffer;
}

static uint * QT_FASTCALL destFetchARGB32P(uint *, const QRasterBuffer *rasterBuffer, int x, int y, int)
{
    return (uint *)rasterBuffer->scanLine(y) + x;
}

static uint * QT_FASTCALL destFetchRGB16(uint *buffer, const QRasterBuffer *rasterBuffer, int x, int y, int length)
{
    const ushort *data = (const ushort *)rasterBuffer->scanLine(y) + x;
    for (int i = 0; i < length; ++i)
        buffer[i] = qConvertRgb16To32(data[i]);
    return buffer;
}

static DestFetchProc destFetchProc[QImage::NImageFormats] =
{
    0, // Format_Invalid
    destFetchMono, // Format_Mono,
    destFetchMonoLsb, // Format_MonoLSB
    0, // Format_Indexed8
    destFetchARGB32P, // Format_RGB32
    destFetchARGB32, // Format_ARGB32,
    destFetchARGB32P, // Format_ARGB32_Premultiplied
    destFetchRGB16,   // Format_RGB16
};

/*
   Returns the color in the mono destination color table
   that is the "nearest" to /color/.
*/
static inline QRgb findNearestColor(QRgb color, QRasterBuffer *rbuf)
{
    QRgb color_0 = PREMUL(rbuf->destColor0);
    QRgb color_1 = PREMUL(rbuf->destColor1);
    color = PREMUL(color);

    const int r = qRed(color);
    const int g = qGreen(color);
    const int b = qBlue(color);

    int rx = r - qRed(color_0);
    int gx = g - qGreen(color_0);
    int bx = b - qBlue(color_0);
    const int dist_0 = rx*rx + gx*gx + bx*bx;

    rx = r - qRed(color_1);
    gx = g - qGreen(color_1);
    bx = b - qBlue(color_1);
    const int dist_1 = rx*rx + gx*gx + bx*bx;

    if (dist_0 < dist_1)
        return color_0;
    return color_1;
}

/*
  Destination store.
*/

static void QT_FASTCALL destStoreMono(QRasterBuffer *rasterBuffer, int x, int y, const uint *buffer, int length)
{
    uchar *data = (uchar *)rasterBuffer->scanLine(y);
    if (rasterBuffer->monoDestinationWithClut) {
        for (int i = 0; i < length; ++i) {
            if (buffer[i] == rasterBuffer->destColor0) {
                data[x >> 3] &= ~(0x80 >> (x & 7));
            } else if (buffer[i] == rasterBuffer->destColor1) {
                data[x >> 3] |= 0x80 >> (x & 7);
            } else if (findNearestColor(buffer[i], rasterBuffer) == rasterBuffer->destColor0) {
                data[x >> 3] &= ~(0x80 >> (x & 7));
            } else {
                data[x >> 3] |= 0x80 >> (x & 7);
            }
            ++x;
        }
    } else {
        for (int i = 0; i < length; ++i) {
            if (qGray(buffer[i]) < int(qt_bayer_matrix[y & 15][x & 15]))
                data[x >> 3] |= 0x80 >> (x & 7);
            else
                data[x >> 3] &= ~(0x80 >> (x & 7));
            ++x;
        }
    }
}

static void QT_FASTCALL destStoreMonoLsb(QRasterBuffer *rasterBuffer, int x, int y, const uint *buffer, int length)
{
    uchar *data = (uchar *)rasterBuffer->scanLine(y);
    if (rasterBuffer->monoDestinationWithClut) {
        for (int i = 0; i < length; ++i) {
            if (buffer[i] == rasterBuffer->destColor0) {
                data[x >> 3] &= ~(1 << (x & 7));
            } else if (buffer[i] == rasterBuffer->destColor1) {
                data[x >> 3] |= 1 << (x & 7);
            } else if (findNearestColor(buffer[i], rasterBuffer) == rasterBuffer->destColor0) {
                data[x >> 3] &= ~(1 << (x & 7));
            } else {
                data[x >> 3] |= 1 << (x & 7);
            }
            ++x;
        }
    } else {
        for (int i = 0; i < length; ++i) {
            if (qGray(buffer[i]) < int(qt_bayer_matrix[y & 15][x & 15]))
                data[x >> 3] |= 1 << (x & 7);
            else
                data[x >> 3] &= ~(1 << (x & 7));
            ++x;
        }
    }
}

static void QT_FASTCALL destStoreARGB32(QRasterBuffer *rasterBuffer, int x, int y, const uint *buffer, int length)
{
    uint *data = (uint *)rasterBuffer->scanLine(y) + x;
    for (int i = 0; i < length; ++i) {
        int p = buffer[i];
        int alpha = qAlpha(p);
        if (alpha == 255)
            data[i] = p;
        else if (alpha == 0)
            data[i] = 0;
        else {
            int inv_alpha = 0xff0000/alpha;
            data[i] = (p & 0xff000000)
                      | ((qRed(p)*inv_alpha) & 0xff0000)
                      | (((qGreen(p)*inv_alpha) >> 8) & 0xff00)
                      | ((qBlue(p)*inv_alpha) >> 16);
        }
    }
}

static void QT_FASTCALL destStoreRGB32(QRasterBuffer *rasterBuffer, int x, int y, const uint *buffer, int length)
{
    quint32 *data = (quint32*)rasterBuffer->scanLine(y) + x;
    ::memcpy(data, buffer, length * sizeof(quint32));
}

static void QT_FASTCALL destStoreRGB16(QRasterBuffer *rasterBuffer, int x, int y, const uint *buffer, int length)
{
    quint16 *data = (quint16*)rasterBuffer->scanLine(y) + x;
    qt_memconvert<quint16, quint32>(data, buffer, length);
}

static DestStoreProc destStoreProc[QImage::NImageFormats] =
{
    0, // Format_Invalid
    destStoreMono, // Format_Mono,
    destStoreMonoLsb, // Format_MonoLSB
    0, // Format_Indexed8
    destStoreRGB32, // Format_RGB32
    destStoreARGB32, // Format_ARGB32,
    destStoreRGB32, // Format_ARGB32_Premultiplied
    destStoreRGB16, // Format_RGB16
};

/*
  Source fetches

  This is a bit more complicated, as we need several fetch routines for every surface type

  We need 5 fetch methods per surface type:
  untransformed
  transformed (tiled and not tiled)
  transformed bilinear (tiled and not tiled)

  We don't need bounds checks for untransformed, but we need them for the other ones.

  The generic implementation does pixel by pixel fetches
*/

template <QImage::Format format>
static uint QT_FASTCALL qt_fetchPixel(const uchar *scanLine, int x, const QVector<QRgb> *rgb);

template<>
uint QT_FASTCALL qt_fetchPixel<QImage::Format_Mono>(const uchar *scanLine,
                                                 int x, const QVector<QRgb> *rgb)
{
    bool pixel = scanLine[x>>3] & (0x80 >> (x & 7));
    if (rgb) return PREMUL(rgb->at(pixel ? 1 : 0));
    return pixel ? 0xff000000 : 0xffffffff;
}

template<>
uint QT_FASTCALL qt_fetchPixel<QImage::Format_MonoLSB>(const uchar *scanLine,
                                                    int x, const QVector<QRgb> *rgb)
{
    bool pixel = scanLine[x>>3] & (0x1 << (x & 7));
    if (rgb) return PREMUL(rgb->at(pixel ? 1 : 0));
    return pixel ? 0xff000000 : 0xffffffff;
}

template<>
uint QT_FASTCALL qt_fetchPixel<QImage::Format_Indexed8>(const uchar *scanLine,
                                                     int x, const QVector<QRgb> *rgb)
{
    return PREMUL(rgb->at(scanLine[x]));
}

template<>
uint QT_FASTCALL qt_fetchPixel<QImage::Format_ARGB32>(const uchar *scanLine,
                                                   int x, const QVector<QRgb> *)
{
    return PREMUL(((const uint *)scanLine)[x]);
}

template<>
uint QT_FASTCALL qt_fetchPixel<QImage::Format_ARGB32_Premultiplied>(const uchar *scanLine,
                                                                 int x, const QVector<QRgb> *)
{
    return ((const uint *)scanLine)[x];
}

template<>
uint QT_FASTCALL qt_fetchPixel<QImage::Format_RGB16>(const uchar *scanLine,
                                                  int x, const QVector<QRgb> *)
{
    return qConvertRgb16To32(((const ushort *)scanLine)[x]);
}

template<>
uint QT_FASTCALL qt_fetchPixel<QImage::Format_Invalid>(const uchar *,
                                                     int ,
                                                     const QVector<QRgb> *)
{
    return 0;
}

typedef uint (QT_FASTCALL *FetchPixelProc)(const uchar *scanLine, int x, const QVector<QRgb> *);


static const FetchPixelProc fetchPixelProc[QImage::NImageFormats] =
{
    0,
    qt_fetchPixel<QImage::Format_Mono>,
    qt_fetchPixel<QImage::Format_MonoLSB>,
    qt_fetchPixel<QImage::Format_Indexed8>,
    qt_fetchPixel<QImage::Format_ARGB32_Premultiplied>,
    qt_fetchPixel<QImage::Format_ARGB32>,
    qt_fetchPixel<QImage::Format_ARGB32_Premultiplied>,
    qt_fetchPixel<QImage::Format_RGB16>
};

enum TextureBlendType {
    BlendUntransformed,
    BlendTiled,
    BlendTransformed,
    BlendTransformedTiled,
    BlendTransformedBilinear,
    BlendTransformedBilinearTiled,
    NBlendTypes
};

template <QImage::Format format>
static const uint * QT_FASTCALL fetchUntransformed(uint *buffer, const Operator *, const QSpanData *data,
                                             int y, int x, int length)
{
    const uchar *scanLine = data->texture.scanLine(y);
    for (int i = 0; i < length; ++i)
        buffer[i] = qt_fetchPixel<format>(scanLine, x + i, data->texture.colorTable);
    return buffer;
}

template <>
const uint * QT_FASTCALL
fetchUntransformed<QImage::Format_ARGB32_Premultiplied>(uint *, const Operator *,
                                                         const QSpanData *data,
                                                         int y, int x, int)
{
    const uchar *scanLine = data->texture.scanLine(y);
    return ((const uint *)scanLine) + x;
}

template<TextureBlendType blendType>  /* either BlendTransformed or BlendTransformedTiled */
static const uint * QT_FASTCALL fetchTransformed(uint *buffer, const Operator *, const QSpanData *data,
                                                         int y, int x, int length)
{
    const FetchPixelProc fetch = fetchPixelProc[data->texture.format];

    const int image_width = data->texture.width;
    const int image_height = data->texture.height;

    const qreal cx = x + qreal(0.5);
    const qreal cy = y + qreal(0.5);

    const uint *end = buffer + length;
    uint *b = buffer;
    const qreal fdx = data->m11;
    const qreal fdy = data->m12;
    const qreal fdw = data->m13;

    qreal fx = data->m21 * cy + data->m11 * cx + data->dx;
    qreal fy = data->m22 * cy + data->m12 * cx + data->dy;
    qreal fw = data->m23 * cy + data->m13 * cx + data->m33;

    while (b < end) {
        const qreal iw = fw == 0 ? 1 : 1 / fw;
        const qreal tx = fx * iw;
        const qreal ty = fy * iw;
        int px = int(tx) - (tx < 0);
        int py = int(ty) - (ty < 0);

        if (blendType == BlendTransformedTiled) {
            px %= image_width;
            py %= image_height;
            if (px < 0) px += image_width;
            if (py < 0) py += image_height;

            const uchar *scanLine = data->texture.scanLine(py);
            *b = fetch(scanLine, px, data->texture.colorTable);
        } else {
            if ((px < 0) || (px >= image_width)
                || (py < 0) || (py >= image_height)) {
                *b = uint(0);
            } else {
                const uchar *scanLine = data->texture.scanLine(py);
                *b = fetch(scanLine, px, data->texture.colorTable);
            }
        }
        fx += fdx;
        fy += fdy;
        fw += fdw;
        //force increment to avoid /0
        if (!fw) {
            fw += fdw;
        }
        ++b;
    }

    return buffer;
}

static inline void fetchTransformedBilinear_pixelBounds(TextureBlendType blendType,
                                                        int max, int l1, int l2, int &v1, int &v2)
{
    if (blendType == BlendTransformedBilinearTiled) {
        v1 %= max;
        if (v1 < 0) v1 += max;
        v2 = v1 + 1;
        v2 %= max;
    } else {
        if (v1 < l1) {
            v2 = v1 = l1;
        } else if (v1 >= l2) {
            v2 = v1 = l2;
        } else {
            v2 = v1 + 1;
        }
    }

    Q_ASSERT(v1 >= 0 && v1 < max);
    Q_ASSERT(v2 >= 0 && v2 < max);
}

template<TextureBlendType blendType, QImage::Format format> /* blendType = BlendTransformedBilinear or BlendTransformedBilinearTiled */
static const uint * QT_FASTCALL fetchTransformedBilinear(uint *buffer, const Operator *, const QSpanData *data,
                                                  int y, int x, int length)
{
    FetchPixelProc fetch = (format != QImage::Format_Invalid) ? fetchPixelProc[format] : fetchPixelProc[data->texture.format];

    int image_width = data->texture.width;
    int image_height = data->texture.height;

    int image_x1 = data->texture.x1;
    int image_y1 = data->texture.y1;
    int image_x2 = data->texture.x2 - 1;
    int image_y2 = data->texture.y2 - 1;

    const qreal cx = x + qreal(0.5);
    const qreal cy = y + qreal(0.5);

    uint *end = buffer + length;
    uint *b = buffer;
    const qreal fdx = data->m11;
    const qreal fdy = data->m12;
    const qreal fdw = data->m13;

    qreal fx = data->m21 * cy + data->m11 * cx + data->dx;
    qreal fy = data->m22 * cy + data->m12 * cx + data->dy;
    qreal fw = data->m23 * cy + data->m13 * cx + data->m33;

    while (b < end) {
        const qreal iw = fw == 0 ? 1 : 1 / fw;
        const qreal px = fx * iw - qreal(0.5);
        const qreal py = fy * iw - qreal(0.5);

        int x1 = int(px) - (px < 0);
        int x2;
        int y1 = int(py) - (py < 0);
        int y2;

        int distx = int((px - x1) * 256);
        int disty = int((py - y1) * 256);
        int idistx = 256 - distx;
        int idisty = 256 - disty;

        fetchTransformedBilinear_pixelBounds(blendType, image_width, image_x1, image_x2, x1, x2);
        fetchTransformedBilinear_pixelBounds(blendType, image_height, image_y1, image_y2, y1, y2);

        const uchar *s1 = data->texture.scanLine(y1);
        const uchar *s2 = data->texture.scanLine(y2);

        uint tl = fetch(s1, x1, data->texture.colorTable);
        uint tr = fetch(s1, x2, data->texture.colorTable);
        uint bl = fetch(s2, x1, data->texture.colorTable);
        uint br = fetch(s2, x2, data->texture.colorTable);

        uint xtop = INTERPOLATE_PIXEL_256(tl, idistx, tr, distx);
        uint xbot = INTERPOLATE_PIXEL_256(bl, idistx, br, distx);
        *b = INTERPOLATE_PIXEL_256(xtop, idisty, xbot, disty);

        fx += fdx;
        fy += fdy;
        fw += fdw;
        //force increment to avoid /0
        if (!fw) {
            fw += fdw;
        }
        ++b;
    }

    return buffer;
}

static const SourceFetchProc sourceFetch[NBlendTypes][QImage::NImageFormats] = {
    // Untransformed
    {
        0, // Invalid
        fetchUntransformed<QImage::Format_Mono>,   // Mono
        fetchUntransformed<QImage::Format_MonoLSB>,   // MonoLsb
        fetchUntransformed<QImage::Format_Indexed8>,   // Indexed8
        fetchUntransformed<QImage::Format_ARGB32_Premultiplied>,   // RGB32
        fetchUntransformed<QImage::Format_ARGB32>,   // ARGB32
        fetchUntransformed<QImage::Format_ARGB32_Premultiplied>,   // ARGB32_Premultiplied
        fetchUntransformed<QImage::Format_RGB16>   // RGB16
    },
    // Tiled
    {
        0, // Invalid
        fetchUntransformed<QImage::Format_Mono>,   // Mono
        fetchUntransformed<QImage::Format_MonoLSB>,   // MonoLsb
        fetchUntransformed<QImage::Format_Indexed8>,   // Indexed8
        fetchUntransformed<QImage::Format_ARGB32_Premultiplied>,   // RGB32
        fetchUntransformed<QImage::Format_ARGB32>,   // ARGB32
        fetchUntransformed<QImage::Format_ARGB32_Premultiplied>,   // ARGB32_Premultiplied
        fetchUntransformed<QImage::Format_RGB16>   // RGB16
    },
    // Transformed
    {
        0, // Invalid
        fetchTransformed<BlendTransformed>,   // Mono
        fetchTransformed<BlendTransformed>,   // MonoLsb
        fetchTransformed<BlendTransformed>,   // Indexed8
        fetchTransformed<BlendTransformed>,   // RGB32
        fetchTransformed<BlendTransformed>,   // ARGB32
        fetchTransformed<BlendTransformed>,   // ARGB32_Premultiplied
        fetchTransformed<BlendTransformed>   // RGB16
    },
    {
        0, // TransformedTiled
        fetchTransformed<BlendTransformedTiled>,   // Mono
        fetchTransformed<BlendTransformedTiled>,   // MonoLsb
        fetchTransformed<BlendTransformedTiled>,   // Indexed8
        fetchTransformed<BlendTransformedTiled>,   // RGB32
        fetchTransformed<BlendTransformedTiled>,   // ARGB32
        fetchTransformed<BlendTransformedTiled>,   // ARGB32_Premultiplied
        fetchTransformed<BlendTransformedTiled>   // RGB16
    },
    {
        0, // Bilinear
        fetchTransformedBilinear<BlendTransformedBilinear, QImage::Format_Invalid>,   // Mono
        fetchTransformedBilinear<BlendTransformedBilinear, QImage::Format_Invalid>,   // MonoLsb
        fetchTransformedBilinear<BlendTransformedBilinear, QImage::Format_Invalid>,   // Indexed8
        fetchTransformedBilinear<BlendTransformedBilinear, QImage::Format_ARGB32_Premultiplied>,   // RGB32
        fetchTransformedBilinear<BlendTransformedBilinear, QImage::Format_ARGB32>,   // ARGB32
        fetchTransformedBilinear<BlendTransformedBilinear, QImage::Format_ARGB32_Premultiplied>,   // ARGB32_Premultiplied
        fetchTransformedBilinear<BlendTransformedBilinear, QImage::Format_Invalid>,   // RGB16
    },
    {
        0, // BilinearTiled
        fetchTransformedBilinear<BlendTransformedBilinearTiled, QImage::Format_Invalid>,   // Mono
        fetchTransformedBilinear<BlendTransformedBilinearTiled, QImage::Format_Invalid>,   // MonoLsb
        fetchTransformedBilinear<BlendTransformedBilinearTiled, QImage::Format_Invalid>,   // Indexed8
        fetchTransformedBilinear<BlendTransformedBilinearTiled, QImage::Format_ARGB32_Premultiplied>,   // RGB32
        fetchTransformedBilinear<BlendTransformedBilinearTiled, QImage::Format_ARGB32>,   // ARGB32
        fetchTransformedBilinear<BlendTransformedBilinearTiled, QImage::Format_ARGB32_Premultiplied>,   // ARGB32_Premultiplied
        fetchTransformedBilinear<BlendTransformedBilinearTiled, QImage::Format_Invalid>   // RGB16
    },
};

#define FIXPT_BITS 8
#define FIXPT_SIZE (1<<FIXPT_BITS)

static void QT_FASTCALL getLinearGradientValues(LinearGradientValues *v, const QSpanData *data)
{
    v->dx = data->gradient.linear.end.x - data->gradient.linear.origin.x;
    v->dy = data->gradient.linear.end.y - data->gradient.linear.origin.y;
    v->l = v->dx * v->dx + v->dy * v->dy;
    v->off = 0;
    if (v->l != 0) {
        v->dx /= v->l;
        v->dy /= v->l;
        v->off = -v->dx * data->gradient.linear.origin.x - v->dy * data->gradient.linear.origin.y;
    }
}

static inline uint qt_gradient_clamp(const QGradientData *data, int ipos)
{
    if (ipos < 0 || ipos >= GRADIENT_STOPTABLE_SIZE) {
        if (data->spread == QGradient::RepeatSpread) {
            ipos = ipos % GRADIENT_STOPTABLE_SIZE;
            ipos = ipos < 0 ? GRADIENT_STOPTABLE_SIZE + ipos : ipos;
        } else if (data->spread == QGradient::ReflectSpread) {
            const int limit = GRADIENT_STOPTABLE_SIZE * 2;
            ipos = ipos % limit;
            ipos = ipos < 0 ? limit + ipos : ipos;
            ipos = ipos >= GRADIENT_STOPTABLE_SIZE ? limit - 1 - ipos : ipos;
        } else {
            if (ipos < 0)
                ipos = 0;
            else if (ipos >= GRADIENT_STOPTABLE_SIZE)
                ipos = GRADIENT_STOPTABLE_SIZE-1;
        }
    }

    Q_ASSERT(ipos >= 0);
    Q_ASSERT(ipos < GRADIENT_STOPTABLE_SIZE);

    return ipos;
}

static inline uint qt_gradient_pixel(const QGradientData *data, qreal pos)
{
    int ipos = int(pos * (GRADIENT_STOPTABLE_SIZE - 1) + qreal(0.5));
    return data->colorTable[qt_gradient_clamp(data, ipos)];
}

static inline uint qt_gradient_pixel_fixed(const QGradientData *data, int fixed_pos)
{
    int ipos = (fixed_pos + (FIXPT_SIZE / 2)) >> FIXPT_BITS;
    return data->colorTable[qt_gradient_clamp(data, ipos)];
}

static const uint * QT_FASTCALL qt_fetch_linear_gradient(uint *buffer, const Operator *op, const QSpanData *data,
                                                         int y, int x, int length)
{
    const uint *b = buffer;
    qreal t, inc;

    bool affine = true;
    qreal rx=0, ry=0;
    if (op->linear.l == 0) {
        t = inc = 0;
    } else {
        rx = data->m21 * (y + qreal(0.5)) + data->m11 * (x + qreal(0.5)) + data->dx;
        ry = data->m22 * (y + qreal(0.5)) + data->m12 * (x + qreal(0.5)) + data->dy;
        t = op->linear.dx*rx + op->linear.dy*ry + op->linear.off;
        inc = op->linear.dx * data->m11 + op->linear.dy * data->m12;
        affine = !data->m13 && !data->m23;

        if (affine) {
            t *= (GRADIENT_STOPTABLE_SIZE - 1);
            inc *= (GRADIENT_STOPTABLE_SIZE - 1);
        }
    }

    const uint *end = buffer + length;
    if (affine) {
        if (inc > qreal(-1e-5) && inc < qreal(1e-5)) {
            qt_memfill<quint32>(buffer, qt_gradient_pixel_fixed(&data->gradient, int(t * FIXPT_SIZE)), length);
        } else {
            if (t+inc*length < qreal(INT_MAX >> (FIXPT_BITS + 1)) &&
                t+inc*length > qreal(INT_MIN >> (FIXPT_BITS + 1))) {
                // we can use fixed point math
                int t_fixed = int(t * FIXPT_SIZE);
                int inc_fixed = int(inc * FIXPT_SIZE);
                while (buffer < end) {
                    *buffer = qt_gradient_pixel_fixed(&data->gradient, t_fixed);
                    t_fixed += inc_fixed;
                    ++buffer;
                }
            } else {
                // we have to fall back to float math
                while (buffer < end) {
                    *buffer = qt_gradient_pixel(&data->gradient, t/GRADIENT_STOPTABLE_SIZE);
                    t += inc;
                    ++buffer;
                }
            }
        }
    } else { // fall back to float math here as well
        qreal rw = data->m23 * (y + qreal(0.5)) + data->m13 * (x + qreal(0.5)) + data->m33;
        while (buffer < end) {
            qreal x = rx/rw;
            qreal y = ry/rw;
            t = (op->linear.dx*x + op->linear.dy *y) + op->linear.off;

            *buffer = qt_gradient_pixel(&data->gradient, t);
            rx += data->m11;
            ry += data->m12;
            rw += data->m13;
            if (!rw) {
                rw += data->m13;
            }
            ++buffer;
        }
    }

    return b;
}

static void QT_FASTCALL getRadialGradientValues(RadialGradientValues *v, const QSpanData *data)
{
    v->dx = data->gradient.radial.center.x - data->gradient.radial.focal.x;
    v->dy = data->gradient.radial.center.y - data->gradient.radial.focal.y;

    v->dr = data->gradient.radial.center.radius - data->gradient.radial.focal.radius;
    v->sqrfr = data->gradient.radial.focal.radius * data->gradient.radial.focal.radius;

    v->a = v->dr * v->dr - v->dx*v->dx - v->dy*v->dy;
    v->inv2a = 1 / (2 * v->a);

    v->extended = !qFuzzyIsNull(data->gradient.radial.focal.radius) || v->a <= 0;
}

static inline void radial_fetch_plain(uint *buffer, uint *end, const Operator *op, const QSpanData *data, qreal det,
                                      qreal delta_det, qreal delta_delta_det, qreal b, qreal delta_b)
{
    if (op->radial.extended) {
        while (buffer < end) {
            quint32 result = 0;
            if (det >= 0) {
                qreal w = qSqrt(det) - b;
                if (data->gradient.radial.focal.radius + op->radial.dr * w >= 0)
                    result = qt_gradient_pixel(&data->gradient, w);
            }

            *buffer = result;

            det += delta_det;
            delta_det += delta_delta_det;
            b += delta_b;

            ++buffer;
        }
    } else {
        while (buffer < end) {
            *buffer++ = qt_gradient_pixel(&data->gradient, qSqrt(det) - b);

            det += delta_det;
            delta_det += delta_delta_det;
            b += delta_b;
        }
    }
}

static inline qreal qRadialDeterminant(qreal a, qreal b, qreal c)
{
    return (b * b) - (4 * a * c);
}

static const uint * QT_FASTCALL qt_fetch_radial_gradient(uint *buffer, const Operator *op, const QSpanData *data,
                                                         int y, int x, int length)
{
    // avoid division by zero
    if (qFuzzyIsNull(op->radial.a)) {
        qt_memfill(buffer, (uint)0, length);
        return buffer;
    }

    const uint *b = buffer;
    qreal rx = data->m21 * (y + qreal(0.5))
               + data->dx + data->m11 * (x + qreal(0.5));
    qreal ry = data->m22 * (y + qreal(0.5))
               + data->dy + data->m12 * (x + qreal(0.5));
    bool affine = !data->m13 && !data->m23;

    uint *end = buffer + length;
    if (affine) {
        rx -= data->gradient.radial.focal.x;
        ry -= data->gradient.radial.focal.y;

        qreal inv_a = 1 / qreal(2 * op->radial.a);

        const qreal delta_rx = data->m11;
        const qreal delta_ry = data->m12;

        qreal b = 2*(op->radial.dr*data->gradient.radial.focal.radius + rx * op->radial.dx + ry * op->radial.dy);
        qreal delta_b = 2*(delta_rx * op->radial.dx + delta_ry * op->radial.dy);
        const qreal b_delta_b = 2 * b * delta_b;
        const qreal delta_b_delta_b = 2 * delta_b * delta_b;

        const qreal bb = b * b;
        const qreal delta_bb = delta_b * delta_b;

        b *= inv_a;
        delta_b *= inv_a;

        const qreal rxrxryry = rx * rx + ry * ry;
        const qreal delta_rxrxryry = delta_rx * delta_rx + delta_ry * delta_ry;
        const qreal rx_plus_ry = 2*(rx * delta_rx + ry * delta_ry);
        const qreal delta_rx_plus_ry = 2 * delta_rxrxryry;

        inv_a *= inv_a;

        qreal det = (bb - 4 * op->radial.a * (op->radial.sqrfr - rxrxryry)) * inv_a;
        qreal delta_det = (b_delta_b + delta_bb + 4 * op->radial.a * (rx_plus_ry + delta_rxrxryry)) * inv_a;
        const qreal delta_delta_det = (delta_b_delta_b + 4 * op->radial.a * delta_rx_plus_ry) * inv_a;

        radial_fetch_plain(buffer, end, op, data, det, delta_det, delta_delta_det, b, delta_b);
    } else {
        qreal rw = data->m23 * (y + qreal(0.5))
                   + data->m33 + data->m13 * (x + qreal(0.5));

        while (buffer < end) {
            if (rw == 0) {
                *buffer = 0;
            } else {
                qreal invRw = 1 / rw;
                qreal gx = rx * invRw - data->gradient.radial.focal.x;
                qreal gy = ry * invRw - data->gradient.radial.focal.y;
                qreal b  = 2*(op->radial.dr*data->gradient.radial.focal.radius + gx*op->radial.dx + gy*op->radial.dy);
                qreal det = qRadialDeterminant(op->radial.a, b, op->radial.sqrfr - (gx*gx + gy*gy));

                quint32 result = 0;
                if (det >= 0) {
                    qreal detSqrt = qSqrt(det);

                    qreal s0 = (-b - detSqrt) * op->radial.inv2a;
                    qreal s1 = (-b + detSqrt) * op->radial.inv2a;

                    qreal s = qMax(s0, s1);

                    if (data->gradient.radial.focal.radius + op->radial.dr * s >= 0)
                        result = qt_gradient_pixel(&data->gradient, s);
                }

                *buffer = result;
            }

            rx += data->m11;
            ry += data->m12;
            rw += data->m13;

            ++buffer;
        }
    }

    return b;
}

static const uint * QT_FASTCALL qt_fetch_conical_gradient(uint *buffer, const Operator *, const QSpanData *data,
                                                          int y, int x, int length)
{
    const uint *b = buffer;
    qreal rx = data->m21 * (y + qreal(0.5))
               + data->dx + data->m11 * (x + qreal(0.5));
    qreal ry = data->m22 * (y + qreal(0.5))
               + data->dy + data->m12 * (x + qreal(0.5));
    bool affine = !data->m13 && !data->m23;

    const uint *end = buffer + length;
    if (affine) {
        rx -= data->gradient.conical.center.x;
        ry -= data->gradient.conical.center.y;
        while (buffer < end) {
            qreal angle = qAtan2(ry, rx) + data->gradient.conical.angle;

            *buffer = qt_gradient_pixel(&data->gradient, 1 - angle / (2*M_PI));

            rx += data->m11;
            ry += data->m12;
            ++buffer;
        }
    } else {
        qreal rw = data->m23 * (y + qreal(0.5))
                   + data->m33 + data->m13 * (x + qreal(0.5));
        if (!rw)
            rw = 1;
        while (buffer < end) {
            qreal angle = qAtan2(ry/rw - data->gradient.conical.center.x,
                                rx/rw - data->gradient.conical.center.y)
                          + data->gradient.conical.angle;

            *buffer = qt_gradient_pixel(&data->gradient, 1. - angle / (2*M_PI));

            rx += data->m11;
            ry += data->m12;
            rw += data->m13;
            if (!rw) {
                rw += data->m13;
            }
            ++buffer;
        }
    }
    return b;
}

/* The constant alpha factor describes an alpha factor that gets applied
   to the result of the composition operation combining it with the destination.

   The intent is that if const_alpha == 0. we get back dest, and if const_alpha == 1.
   we get the unmodified operation

   result = src op dest
   dest = result * const_alpha + dest * (1. - const_alpha)

   This means that in the comments below, the first line is the const_alpha==255 case, the
   second line the general one.

   In the lines below:
   s == src, sa == alpha(src), sia = 1 - alpha(src)
   d == dest, da == alpha(dest), dia = 1 - alpha(dest)
   ca = const_alpha, cia = 1 - const_alpha

   The methods exist in two variants. One where we have a constant source, the other
   where the source is an array of pixels.
*/

/*
  result = 0
  d = d * cia
*/
#define comp_func_Clear_impl(dest, length, const_alpha)\
{\
    if (const_alpha == 255) {\
        qt_memfill<quint32>(dest, 0, length);\
    } else {\
        int ialpha = 255 - const_alpha;\
        for (int i = 0; i < length; ++i) {\
            dest[i] = BYTE_MUL(dest[i], ialpha);\
        }\
    }\
}

static void QT_FASTCALL comp_func_solid_Clear(uint *dest, const int length, uint, const uint const_alpha)
{
    comp_func_Clear_impl(dest, length, const_alpha);
}

static void QT_FASTCALL comp_func_Clear(uint *dest, const uint *, int length, const uint const_alpha)
{
    comp_func_Clear_impl(dest, length, const_alpha);
}

/*
  result = s
  dest = s * ca + d * cia
*/
static void QT_FASTCALL comp_func_solid_Source(uint *dest, const int length, uint color, const uint const_alpha)
{
    if (const_alpha == 255) {
        qt_memfill<quint32>(dest, color, length);
    } else {
        int ialpha = 255 - const_alpha;
        color = BYTE_MUL(color, const_alpha);
        for (int i = 0; i < length; ++i) {
            dest[i] = color + BYTE_MUL(dest[i], ialpha);
        }
    }
}

static void QT_FASTCALL comp_func_Source(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        ::memcpy(dest, src, length * sizeof(uint));
    } else {
        int ialpha = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            dest[i] = INTERPOLATE_PIXEL_255(src[i], const_alpha, dest[i], ialpha);
        }
    }
}

static void QT_FASTCALL comp_func_solid_Destination(uint *, int, uint, uint)
{
}

static void QT_FASTCALL comp_func_Destination(uint *, const uint *, int, uint)
{
}

/*
  result = s + d * sia
  dest = (s + d * sia) * ca + d * cia
       = s * ca + d * (sia * ca + cia)
       = s * ca + d * (1 - sa*ca)
*/
static void QT_FASTCALL comp_func_solid_SourceOver(uint *dest, const int length, uint color, const uint const_alpha)
{
    if ((const_alpha & qAlpha(color)) == 255) {
        qt_memfill<quint32>(dest, color, length);
    } else {
        if (const_alpha != 255)
            color = BYTE_MUL(color, const_alpha);
        for (int i = 0; i < length; ++i) {
            dest[i] = color + BYTE_MUL(dest[i], qAlpha(~color));
        }
    }
}

static void QT_FASTCALL comp_func_SourceOver(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint s = src[i];
            if (s >= 0xff000000)
                dest[i] = s;
            else if (s != 0)
                dest[i] = s + BYTE_MUL(dest[i], qAlpha(~s));
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint s = BYTE_MUL(src[i], const_alpha);
            dest[i] = s + BYTE_MUL(dest[i], qAlpha(~s));
        }
    }
}

/*
  result = d + s * dia
  dest = (d + s * dia) * ca + d * cia
       = d + s * dia * ca
*/
static void QT_FASTCALL comp_func_solid_DestinationOver(uint *dest, const int length, uint color, const uint const_alpha)
{
    if (const_alpha != 255)
        color = BYTE_MUL(color, const_alpha);
    for (int i = 0; i < length; ++i) {
        uint d = dest[i];
        dest[i] = d + BYTE_MUL(color, qAlpha(~d));
    }
}

static void QT_FASTCALL comp_func_DestinationOver(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            dest[i] = d + BYTE_MUL(src[i], qAlpha(~d));
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = BYTE_MUL(src[i], const_alpha);
            dest[i] = d + BYTE_MUL(s, qAlpha(~d));
        }
    }
}

/*
  result = s * da
  dest = s * da * ca + d * cia
*/
static void QT_FASTCALL comp_func_solid_SourceIn(uint *dest, const int length, uint color, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = BYTE_MUL(color, qAlpha(dest[i]));
        }
    } else {
        color = BYTE_MUL(color, const_alpha);
        uint cia = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            dest[i] = INTERPOLATE_PIXEL_255(color, qAlpha(d), d, cia);
        }
    }
}

static void QT_FASTCALL comp_func_SourceIn(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = BYTE_MUL(src[i], qAlpha(dest[i]));
        }
    } else {
        uint cia = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = BYTE_MUL(src[i], const_alpha);
            dest[i] = INTERPOLATE_PIXEL_255(s, qAlpha(d), d, cia);
        }
    }
}

/*
  result = d * sa
  dest = d * sa * ca + d * cia
       = d * (sa * ca + cia)
*/
static void QT_FASTCALL comp_func_solid_DestinationIn(uint *dest, const int length, uint color, const uint const_alpha)
{
    uint a = qAlpha(color);
    if (const_alpha != 255) {
        a = BYTE_MUL(a, const_alpha) + 255 - const_alpha;
    }
    for (int i = 0; i < length; ++i) {
        dest[i] = BYTE_MUL(dest[i], a);
    }
}

static void QT_FASTCALL comp_func_DestinationIn(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = BYTE_MUL(dest[i], qAlpha(src[i]));
        }
    } else {
        int cia = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            uint a = BYTE_MUL(qAlpha(src[i]), const_alpha) + cia;
            dest[i] = BYTE_MUL(dest[i], a);
        }
    }
}

/*
  result = s * dia
  dest = s * dia * ca + d * cia
*/

static void QT_FASTCALL comp_func_solid_SourceOut(uint *dest, const int length, uint color, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = BYTE_MUL(color, qAlpha(~dest[i]));
        }
    } else {
        color = BYTE_MUL(color, const_alpha);
        int cia = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            dest[i] = INTERPOLATE_PIXEL_255(color, qAlpha(~d), d, cia);
        }
    }
}

static void QT_FASTCALL comp_func_SourceOut(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = BYTE_MUL(src[i], qAlpha(~dest[i]));
        }
    } else {
        int cia = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            uint s = BYTE_MUL(src[i], const_alpha);
            uint d = dest[i];
            dest[i] = INTERPOLATE_PIXEL_255(s, qAlpha(~d), d, cia);
        }
    }
}

/*
  result = d * sia
  dest = d * sia * ca + d * cia
       = d * (sia * ca + cia)
*/
static void QT_FASTCALL comp_func_solid_DestinationOut(uint *dest, const int length, uint color, const uint const_alpha)
{
    uint a = qAlpha(~color);
    if (const_alpha != 255)
        a = BYTE_MUL(a, const_alpha) + 255 - const_alpha;
    for (int i = 0; i < length; ++i) {
        dest[i] = BYTE_MUL(dest[i], a);
    }
}

static void QT_FASTCALL comp_func_DestinationOut(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = BYTE_MUL(dest[i], qAlpha(~src[i]));
        }
    } else {
        int cia = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            uint sia = BYTE_MUL(qAlpha(~src[i]), const_alpha) + cia;
            dest[i] = BYTE_MUL(dest[i], sia);
        }
    }
}

/*
  result = s*da + d*sia
  dest = s*da*ca + d*sia*ca + d *cia
       = s*ca * da + d * (sia*ca + cia)
       = s*ca * da + d * (1 - sa*ca)
*/
static void QT_FASTCALL comp_func_solid_SourceAtop(uint *dest, const int length, uint color, const uint const_alpha)
{
    if (const_alpha != 255) {
        color = BYTE_MUL(color, const_alpha);
    }
    uint sia = qAlpha(~color);
    for (int i = 0; i < length; ++i) {
        dest[i] = INTERPOLATE_PIXEL_255(color, qAlpha(dest[i]), dest[i], sia);
    }
}

static void QT_FASTCALL comp_func_SourceAtop(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint s = src[i];
            uint d = dest[i];
            dest[i] = INTERPOLATE_PIXEL_255(s, qAlpha(d), d, qAlpha(~s));
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint s = BYTE_MUL(src[i], const_alpha);
            uint d = dest[i];
            dest[i] = INTERPOLATE_PIXEL_255(s, qAlpha(d), d, qAlpha(~s));
        }
    }
}

/*
  result = d*sa + s*dia
  dest = d*sa*ca + s*dia*ca + d *cia
       = s*ca * dia + d * (sa*ca + cia)
*/
static void QT_FASTCALL comp_func_solid_DestinationAtop(uint *dest, const int length, uint color, const uint const_alpha)
{
    uint a = qAlpha(color);
    if (const_alpha != 255) {
        color = BYTE_MUL(color, const_alpha);
        a = qAlpha(color) + 255 - const_alpha;
    }
    for (int i = 0; i < length; ++i) {
        uint d = dest[i];
        dest[i] = INTERPOLATE_PIXEL_255(d, a, color, qAlpha(~d));
    }
}

static void QT_FASTCALL comp_func_DestinationAtop(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint s = src[i];
            uint d = dest[i];
            dest[i] = INTERPOLATE_PIXEL_255(d, qAlpha(s), s, qAlpha(~d));
        }
    } else {
        int cia = 255 - const_alpha;
        for (int i = 0; i < length; ++i) {
            uint s = BYTE_MUL(src[i], const_alpha);
            uint d = dest[i];
            uint a = qAlpha(s) + cia;
            dest[i] = INTERPOLATE_PIXEL_255(d, a, s, qAlpha(~d));
        }
    }
}

/*
  result = d*sia + s*dia
  dest = d*sia*ca + s*dia*ca + d *cia
       = s*ca * dia + d * (sia*ca + cia)
       = s*ca * dia + d * (1 - sa*ca)
*/
static void QT_FASTCALL comp_func_solid_XOR(uint *dest, const int length, uint color, const uint const_alpha)
{
    if (const_alpha != 255)
        color = BYTE_MUL(color, const_alpha);
    uint sia = qAlpha(~color);

    for (int i = 0; i < length; ++i) {
        uint d = dest[i];
        dest[i] = INTERPOLATE_PIXEL_255(color, qAlpha(~d), d, sia);
    }
}

static void QT_FASTCALL comp_func_XOR(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];
            dest[i] = INTERPOLATE_PIXEL_255(s, qAlpha(~d), d, qAlpha(~s));
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = BYTE_MUL(src[i], const_alpha);
            dest[i] = INTERPOLATE_PIXEL_255(s, qAlpha(~d), d, qAlpha(~s));
        }
    }
}

static inline uint partial_coverage_store(const uint dest, const uint src, uint const_alpha)
{
    return INTERPOLATE_PIXEL_255(src, const_alpha, dest, 255 - const_alpha);
}

static inline int mix_alpha(int da, int sa)
{
    return 255 - ((255 - sa) * (255 - da) >> 8);
}

#if QT_POINTER_SIZE == 8 // 64-bit versions
#define AMIX(mask) (qMin(((qint64(s)&mask) + (qint64(d)&mask)), qint64(mask)))
#define MIX(mask) (qMin(((qint64(s)&mask) + (qint64(d)&mask)), qint64(mask)))
#else // 32 bits
// The mask for alpha can overflow over 32 bits
#define AMIX(mask) quint32(qMin(((qint64(s)&mask) + (qint64(d)&mask)), qint64(mask)))
#define MIX(mask) (qMin(((quint32(s)&mask) + (quint32(d)&mask)), quint32(mask)))
#endif

static const uint AMASK = 0xff000000;
static const uint RMASK = 0x00ff0000;
static const uint GMASK = 0x0000ff00;
static const uint BMASK = 0x000000ff;

static inline int comp_func_Plus_one_pixel(uint d, const uint s)
{
    return (AMIX(AMASK) | MIX(RMASK) | MIX(GMASK) | MIX(BMASK));
}

#undef MIX
#undef AMIX

/*
    Dca' = Sca.Da + Dca.Sa + Sca.(1 - Da) + Dca.(1 - Sa)
         = Sca + Dca
*/
static void QT_FASTCALL comp_func_solid_Plus(uint *dest, const int length, uint color, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = comp_func_Plus_one_pixel(dest[i], color);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = comp_func_Plus_one_pixel(dest[i], color);
            dest[i] = partial_coverage_store(dest[i], d, const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Plus(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            dest[i] = comp_func_Plus_one_pixel(dest[i], src[i]);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = comp_func_Plus_one_pixel(dest[i], src[i]);
            dest[i] = partial_coverage_store(dest[i], d, const_alpha);
        }
    }
}

/*
    Dca' = Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
*/
static inline int multiply_op(int dst, int src, int da, int sa)
{
    return qt_div_255(src * dst + src * (255 - da) + dst * (255 - sa));
}

static void QT_FASTCALL comp_func_solid_Multiply(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) multiply_op(a, b, da, sa)
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) multiply_op(a, b, da, sa)
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Multiply(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) multiply_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) multiply_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
    Dca' = (Sca.Da + Dca.Sa - Sca.Dca) + Sca.(1 - Da) + Dca.(1 - Sa)
         = Sca + Dca - Sca.Dca
*/
static void QT_FASTCALL comp_func_solid_Screen(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];

#define OP(a, b) 255 - qt_div_255((255-a) * (255-b))
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(qAlpha(d), sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];

#define OP(a, b) 255 - qt_div_255((255-a) * (255-b))
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(qAlpha(d), sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Screen(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

#define OP(a, b) 255 - (((255-a) * (255-b)) >> 8)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(qAlpha(d), qAlpha(s));
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

#define OP(a, b) 255 - (((255-a) * (255-b)) >> 8)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(qAlpha(d), qAlpha(s));
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
    if 2.Dca < Da
        Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    otherwise
        Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
*/
static inline int overlay_op(int dst, int src, int da, int sa)
{
    const int temp = src * (255 - da) + dst * (255 - sa);
    if (2 * dst < da)
        return qt_div_255(2 * src * dst + temp);
    else
        return qt_div_255(sa * da - 2 * (da - dst) * (sa - src) + temp);
}

static void QT_FASTCALL comp_func_solid_Overlay(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) overlay_op(a, b, da, sa)
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) overlay_op(a, b, da, sa)
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Overlay(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) overlay_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) overlay_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
    Dca' = min(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    Da'  = Sa + Da - Sa.Da
*/
static inline int darken_op(int dst, int src, int da, int sa)
{
    return qt_div_255(qMin(src * da, dst * sa) + src * (255 - da) + dst * (255 - sa));
}

static void QT_FASTCALL comp_func_solid_Darken(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) darken_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) darken_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Darken(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) darken_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) darken_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
   Dca' = max(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
   Da'  = Sa + Da - Sa.Da
*/
static inline int lighten_op(int dst, int src, int da, int sa)
{
    return qt_div_255(qMax(src * da, dst * sa) + src * (255 - da) + dst * (255 - sa));
}

static void QT_FASTCALL comp_func_solid_Lighten(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) lighten_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) lighten_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Lighten(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) lighten_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) lighten_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
   if Sca.Da + Dca.Sa >= Sa.Da
       Dca' = Sa.Da + Sca.(1 - Da) + Dca.(1 - Sa)
   otherwise
       Dca' = Dca.Sa/(1-Sca/Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
*/
static inline int color_dodge_op(int dst, int src, int da, int sa)
{
    const int sa_da = sa * da;
    const int dst_sa = dst * sa;
    const int src_da = src * da;

    const int temp = src * (255 - da) + dst * (255 - sa);
    if (src_da + dst_sa >= sa_da)
        return qt_div_255(sa_da + temp);
    else
        return qt_div_255(255 * dst_sa / (255 - 255 * src / sa) + temp);
}

static void QT_FASTCALL comp_func_solid_ColorDodge(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a,b) color_dodge_op(a, b, da, sa)
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a,b) color_dodge_op(a, b, da, sa)
            int r = OP(  qRed(d), sr);
            int b = OP( qBlue(d), sb);
            int g = OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_ColorDodge(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) color_dodge_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) color_dodge_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
   if Sca.Da + Dca.Sa <= Sa.Da
       Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
   otherwise
       Dca' = Sa.(Sca.Da + Dca.Sa - Sa.Da)/Sca + Sca.(1 - Da) + Dca.(1 - Sa)
*/
static inline int color_burn_op(int dst, int src, int da, int sa)
{
    const int src_da = src * da;
    const int dst_sa = dst * sa;
    const int sa_da = sa * da;

    const int temp = src * (255 - da) + dst * (255 - sa);

    if (src == 0 || src_da + dst_sa <= sa_da)
        return qt_div_255(temp);
    return qt_div_255(sa * (src_da + dst_sa - sa_da) / src + temp);
}

static void QT_FASTCALL comp_func_solid_ColorBurn(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) color_burn_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) color_burn_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_ColorBurn(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) color_burn_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) color_burn_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
    if 2.Sca < Sa
        Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    otherwise
        Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
*/
static inline uint hardlight_op(int dst, int src, int da, int sa)
{
    const uint temp = src * (255 - da) + dst * (255 - sa);

    if (2 * src < sa)
        return qt_div_255(2 * src * dst + temp);
    else
        return qt_div_255(sa * da - 2 * (da - dst) * (sa - src) + temp);
}

static void QT_FASTCALL comp_func_solid_HardLight(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) hardlight_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) hardlight_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_HardLight(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) hardlight_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) hardlight_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
    if 2.Sca <= Sa
        Dca' = Dca.(Sa + (2.Sca - Sa).(1 - Dca/Da)) + Sca.(1 - Da) + Dca.(1 - Sa)
    otherwise if 2.Sca > Sa and 4.Dca <= Da
        Dca' = Dca.Sa + Da.(2.Sca - Sa).(4.Dca/Da.(4.Dca/Da + 1).(Dca/Da - 1) + 7.Dca/Da) + Sca.(1 - Da) + Dca.(1 - Sa)
    otherwise if 2.Sca > Sa and 4.Dca > Da
        Dca' = Dca.Sa + Da.(2.Sca - Sa).((Dca/Da)^0.5 - Dca/Da) + Sca.(1 - Da) + Dca.(1 - Sa)
*/
static inline int soft_light_op(int dst, int src, int da, int sa)
{
    const int src2 = src << 1;
    const int dst_np = da != 0 ? (255 * dst) / da : 0;
    const int temp = (src * (255 - da) + dst * (255 - sa)) * 255;

    if (src2 < sa)
        return (dst * (sa * 255 + (src2 - sa) * (255 - dst_np)) + temp) / 65025;
    else if (4 * dst <= da)
        return (dst * sa * 255 + da * (src2 - sa) * ((((16 * dst_np - 12 * 255) * dst_np + 3 * 65025) * dst_np) / 65025) + temp) / 65025;
    else {
        return (dst * sa * 255 + da * (src2 - sa) * (int(qSqrt(qreal(dst_np * 255))) - dst_np) + temp) / 65025;
    }
}

static void QT_FASTCALL comp_func_solid_SoftLight(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) soft_light_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) soft_light_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_SoftLight(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) soft_light_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) soft_light_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
   Dca' = abs(Dca.Sa - Sca.Da) + Sca.(1 - Da) + Dca.(1 - Sa)
        = Sca + Dca - 2.min(Sca.Da, Dca.Sa)
*/
static inline int difference_op(int dst, int src, int da, int sa)
{
    return src + dst - qt_div_255(2 * qMin(src * da, dst * sa));
}

static void QT_FASTCALL comp_func_solid_Difference(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) difference_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            int da = qAlpha(d);

#define OP(a, b) difference_op(a, b, da, sa)
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Difference(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) difference_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

            int da = qAlpha(d);
            int sa = qAlpha(s);

#define OP(a, b) difference_op(a, b, da, sa)
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(da, sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

/*
    Dca' = (Sca.Da + Dca.Sa - 2.Sca.Dca) + Sca.(1 - Da) + Dca.(1 - Sa)
*/
static void QT_FASTCALL comp_func_solid_Exclusion(uint *dest, const int length, uint color, const uint const_alpha)
{
    int sa = qAlpha(color);
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);

    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];

#define OP(a, b) (a + b - qt_div_255(2*(a*b)))
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(qAlpha(d), sa);
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
    } else {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];

#define OP(a, b) (a + b - qt_div_255(2*(a*b)))
            int r =  OP(  qRed(d), sr);
            int b =  OP( qBlue(d), sb);
            int g =  OP(qGreen(d), sg);
            int a = mix_alpha(qAlpha(d), sa);
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL comp_func_Exclusion(uint *dest, const uint *src, const int length, const uint const_alpha)
{
    if (const_alpha == 255) {
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

#define OP(a, b) (a + b - ((a*b) >> 7))
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(qAlpha(d), qAlpha(s));
#undef OP

            dest[i] = qRgba(r, g, b, a);
        }
        for (int i = 0; i < length; ++i) {
            uint d = dest[i];
            uint s = src[i];

#define OP(a, b) (a + b - ((a*b) >> 7))
            int r = OP(  qRed(d),   qRed(s));
            int b = OP( qBlue(d),  qBlue(s));
            int g = OP(qGreen(d), qGreen(s));
            int a = mix_alpha(qAlpha(d), qAlpha(s));
#undef OP

            dest[i] = partial_coverage_store(d, qRgba(r, g, b, a), const_alpha);
        }
    }
}

static void QT_FASTCALL rasterop_solid_SourceOrDestination(uint *dest,
                                                    int length,
                                                    uint color,
                                                    const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--)
        *dest++ |= color;
}

static void QT_FASTCALL rasterop_SourceOrDestination(uint *dest,
                                              const uint *src,
                                              int length,
                                              const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--)
        *dest++ |= *src++;
}

static void QT_FASTCALL rasterop_solid_SourceAndDestination(uint *dest,
                                                     int length,
                                                     uint color,
                                                     const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    color |= 0xff000000;
    while (length--)
        *dest++ &= color;
}

static void QT_FASTCALL rasterop_SourceAndDestination(uint *dest,
                                               const uint *src,
                                               int length,
                                               const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = (*src & *dest) | 0xff000000;
        ++dest; ++src;
    }
}

static void QT_FASTCALL rasterop_solid_SourceXorDestination(uint *dest,
                                                     int length,
                                                     uint color,
                                                     const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    color &= 0x00ffffff;
    while (length--)
        *dest++ ^= color;
}

static void QT_FASTCALL rasterop_SourceXorDestination(uint *dest,
                                               const uint *src,
                                               int length,
                                               const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = (*src ^ *dest) | 0xff000000;
        ++dest; ++src;
    }
}

static void QT_FASTCALL rasterop_solid_NotSourceAndNotDestination(uint *dest,
                                                           int length,
                                                           uint color,
                                                           const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    color = ~color;
    while (length--) {
        *dest = (color & ~(*dest)) | 0xff000000;
        ++dest;
    }
}

static void QT_FASTCALL rasterop_NotSourceAndNotDestination(uint *dest,
                                                     const uint *src,
                                                     int length,
                                                     const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = (~(*src) & ~(*dest)) | 0xff000000;
        ++dest; ++src;
    }
}

static void QT_FASTCALL rasterop_solid_NotSourceOrNotDestination(uint *dest,
                                                          int length,
                                                          uint color,
                                                          const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    color = ~color | 0xff000000;
    while (length--) {
        *dest = color | ~(*dest);
        ++dest;
    }
}

static void QT_FASTCALL rasterop_NotSourceOrNotDestination(uint *dest,
                                                    const uint *src,
                                                    int length,
                                                    const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = ~(*src) | ~(*dest) | 0xff000000;
        ++dest; ++src;
    }
}

static void QT_FASTCALL rasterop_solid_NotSourceXorDestination(uint *dest,
                                                        int length,
                                                        uint color,
                                                        const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    color = ~color & 0x00ffffff;
    while (length--) {
        *dest = color ^ (*dest);
        ++dest;
    }
}

static void QT_FASTCALL rasterop_NotSourceXorDestination(uint *dest,
                                                  const uint *src,
                                                  int length,
                                                  const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = ((~(*src)) ^ (*dest)) | 0xff000000;
        ++dest; ++src;
    }
}

static void QT_FASTCALL rasterop_solid_NotSource(uint *dest, const int length,
                                          uint color, const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    qt_memfill(dest, ~color | 0xff000000, length);
}

static void QT_FASTCALL rasterop_NotSource(uint *dest, const uint *src,
                                    int length, const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--)
        *dest++ = ~(*src++) | 0xff000000;
}

static void QT_FASTCALL rasterop_solid_NotSourceAndDestination(uint *dest,
                                                        int length,
                                                        uint color,
                                                        const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    color = ~color | 0xff000000;
    while (length--) {
        *dest = color & *dest;
        ++dest;
    }
}

static void QT_FASTCALL rasterop_NotSourceAndDestination(uint *dest,
                                                  const uint *src,
                                                  int length,
                                                  const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = (~(*src) & *dest) | 0xff000000;
        ++dest; ++src;
    }
}

static void QT_FASTCALL rasterop_solid_SourceAndNotDestination(uint *dest,
                                                        int length,
                                                        uint color,
                                                        uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = (color & ~(*dest)) | 0xff000000;
        ++dest;
    }
}

static void QT_FASTCALL rasterop_SourceAndNotDestination(uint *dest,
                                                  const uint *src,
                                                  int length,
                                                  const uint const_alpha)
{
    Q_UNUSED(const_alpha);
    while (length--) {
        *dest = (*src & ~(*dest)) | 0xff000000;
        ++dest; ++src;
    }
}

static const CompositionFunctionSolid functionForModeSolid[] = {
        comp_func_solid_SourceOver,
        comp_func_solid_DestinationOver,
        comp_func_solid_Clear,
        comp_func_solid_Source,
        comp_func_solid_Destination,
        comp_func_solid_SourceIn,
        comp_func_solid_DestinationIn,
        comp_func_solid_SourceOut,
        comp_func_solid_DestinationOut,
        comp_func_solid_SourceAtop,
        comp_func_solid_DestinationAtop,
        comp_func_solid_XOR,
        comp_func_solid_Plus,
        comp_func_solid_Multiply,
        comp_func_solid_Screen,
        comp_func_solid_Overlay,
        comp_func_solid_Darken,
        comp_func_solid_Lighten,
        comp_func_solid_ColorDodge,
        comp_func_solid_ColorBurn,
        comp_func_solid_HardLight,
        comp_func_solid_SoftLight,
        comp_func_solid_Difference,
        comp_func_solid_Exclusion,
        rasterop_solid_SourceOrDestination,
        rasterop_solid_SourceAndDestination,
        rasterop_solid_SourceXorDestination,
        rasterop_solid_NotSourceAndNotDestination,
        rasterop_solid_NotSourceOrNotDestination,
        rasterop_solid_NotSourceXorDestination,
        rasterop_solid_NotSource,
        rasterop_solid_NotSourceAndDestination,
        rasterop_solid_SourceAndNotDestination
};

static const CompositionFunction functionForMode[] = {
        comp_func_SourceOver,
        comp_func_DestinationOver,
        comp_func_Clear,
        comp_func_Source,
        comp_func_Destination,
        comp_func_SourceIn,
        comp_func_DestinationIn,
        comp_func_SourceOut,
        comp_func_DestinationOut,
        comp_func_SourceAtop,
        comp_func_DestinationAtop,
        comp_func_XOR,
        comp_func_Plus,
        comp_func_Multiply,
        comp_func_Screen,
        comp_func_Overlay,
        comp_func_Darken,
        comp_func_Lighten,
        comp_func_ColorDodge,
        comp_func_ColorBurn,
        comp_func_HardLight,
        comp_func_SoftLight,
        comp_func_Difference,
        comp_func_Exclusion,
        rasterop_SourceOrDestination,
        rasterop_SourceAndDestination,
        rasterop_SourceXorDestination,
        rasterop_NotSourceAndNotDestination,
        rasterop_NotSourceOrNotDestination,
        rasterop_NotSourceXorDestination,
        rasterop_NotSource,
        rasterop_NotSourceAndDestination,
        rasterop_SourceAndNotDestination
};

static inline TextureBlendType getBlendType(const QSpanData *data)
{
    if (data->txop <= QTransform::TxTranslate) {
        if (data->texture.type == QTextureData::Tiled)
            return TextureBlendType::BlendTiled;
        return TextureBlendType::BlendUntransformed;
    } else if (data->bilinear) {
        if (data->texture.type == QTextureData::Tiled)
            return TextureBlendType::BlendTransformedBilinearTiled;
        return TextureBlendType::BlendTransformedBilinear;
    } else if (data->texture.type == QTextureData::Tiled)
        return TextureBlendType::BlendTransformedTiled;
    return TextureBlendType::BlendTransformed;
}

static inline Operator getOperator(const QSpanData *data, const QSpan *spans, int spanCount)
{
    Operator op;
    bool solidSource = false;

    switch(data->type) {
    case QSpanData::Solid:
        solidSource = (qAlpha(data->solid.color) == 255);
        break;
    case QSpanData::LinearGradient:
        solidSource = !data->gradient.alphaColor;
        getLinearGradientValues(&op.linear, data);
        op.src_fetch = qt_fetch_linear_gradient;
        break;
    case QSpanData::RadialGradient:
        solidSource = !data->gradient.alphaColor;
        getRadialGradientValues(&op.radial, data);
        op.src_fetch = qt_fetch_radial_gradient;
        break;
    case QSpanData::ConicalGradient:
        solidSource = !data->gradient.alphaColor;
        op.src_fetch = qt_fetch_conical_gradient;
        break;
    case QSpanData::Texture:
        solidSource = !data->texture.hasAlpha;
        op.src_fetch = sourceFetch[getBlendType(data)][data->texture.format];
        break;
    case QSpanData::None:
        break;
    }

    op.mode = data->rasterBuffer->compositionMode;
    if (op.mode == QPainter::CompositionMode_SourceOver && solidSource)
        op.mode = QPainter::CompositionMode_Source;

    op.dest_fetch = destFetchProc[data->rasterBuffer->format];
    op.dest_store = destStoreProc[data->rasterBuffer->format];

    op.funcSolid = functionForModeSolid[op.mode];
    op.func = functionForMode[op.mode];

    return op;
}

// -------------------- blend methods ---------------------
static void blend_color_generic(int count, const QSpan *spans, void *userData)
{
    QSpanData *data = reinterpret_cast<QSpanData *>(userData);
    QSTACKARRAY(uint, buffer, buffer_size);
    Operator op = getOperator(data, spans, count);

    while (count--) {
        int x = spans->x;
        int length = spans->len;
        while (length) {
            int l = qMin(buffer_size, length);
            uint *dest = op.dest_fetch ? op.dest_fetch(buffer, data->rasterBuffer, x, spans->y, l) : buffer;
            op.funcSolid(dest, l, data->solid.color, spans->coverage);
            if (op.dest_store)
                op.dest_store(data->rasterBuffer, x, spans->y, dest, l);
            length -= l;
            x += l;
        }
        ++spans;
    }
}

static void blend_src_generic(int count, const QSpan *spans, void *userData)
{
    QSpanData *data = reinterpret_cast<QSpanData *>(userData);

    QSTACKARRAY(uint, buffer, buffer_size);
    QSTACKARRAY(uint, src_buffer, buffer_size);
    Operator op = getOperator(data, spans, count);

    uint const_alpha = 256;
    if (data->type == QSpanData::Texture)
        const_alpha = data->texture.const_alpha;

    int coverage = 0;
    while (count) {
        int x = spans->x;
        const int y = spans->y;
        int right = x + spans->len;

        // compute length of adjacent spans
        for (int i = 1; i < count && spans[i].y == y && spans[i].x == right; ++i)
            right += spans[i].len;
        int length = right - x;

        while (length) {
            int l = qMin(buffer_size, length);
            length -= l;

            int process_length = l;
            int process_x = x;

            uint *dest = op.dest_fetch ? op.dest_fetch(buffer, data->rasterBuffer, process_x, y, process_length) : buffer;
            const uint *src = op.src_fetch(src_buffer, &op, data, y, process_x, process_length);

            int offset = 0;
            while (l > 0) {
                if (x == spans->x) // new span?
                    coverage = (spans->coverage * const_alpha) >> 8;

                int right = spans->x + spans->len;
                int len = qMin(l, right - x);

                op.func(dest + offset, src + offset, len, coverage);

                l -= len;
                x += len;
                offset += len;

                if (x == right) { // done with current span?
                    ++spans;
                    --count;
                }
            }

            if (op.dest_store) {
                op.dest_store(data->rasterBuffer, process_x, y, dest, process_length);
            }
        }
    }
}

static void blend_untransformed_generic(int count, const QSpan *spans, void *userData)
{
    QSpanData *data = reinterpret_cast<QSpanData *>(userData);

    QSTACKARRAY(uint, buffer, buffer_size);
    QSTACKARRAY(uint, src_buffer, buffer_size);
    Operator op = getOperator(data, spans, count);

    const int image_width = data->texture.width;
    const int image_height = data->texture.height;
    int xoff = -qRound(-data->dx);
    int yoff = -qRound(-data->dy);

    while (count--) {
        int x = spans->x;
        int length = spans->len;
        int sx = xoff + x;
        int sy = yoff + spans->y;
        if (sy >= 0 && sy < image_height && sx < image_width) {
            if (sx < 0) {
                x -= sx;
                length += sx;
                sx = 0;
            }
            if (sx + length > image_width)
                length = image_width - sx;
            if (length > 0) {
                const int coverage = (spans->coverage * data->texture.const_alpha) >> 8;
                while (length) {
                    int l = qMin(buffer_size, length);
                    const uint *src = op.src_fetch(src_buffer, &op, data, sy, sx, l);
                    uint *dest = op.dest_fetch ? op.dest_fetch(buffer, data->rasterBuffer, x, spans->y, l) : buffer;
                    op.func(dest, src, l, coverage);
                    if (op.dest_store)
                        op.dest_store(data->rasterBuffer, x, spans->y, dest, l);
                    x += l;
                    sx += l;
                    length -= l;
                }
            }
        }
        ++spans;
    }
}

static void blend_tiled_generic(int count, const QSpan *spans, void *userData)
{
    QSpanData *data = reinterpret_cast<QSpanData *>(userData);

    QSTACKARRAY(uint, buffer, buffer_size);
    QSTACKARRAY(uint, src_buffer, buffer_size);
    Operator op = getOperator(data, spans, count);

    const int image_width = data->texture.width;
    const int image_height = data->texture.height;
    int xoff = -qRound(-data->dx) % image_width;
    int yoff = -qRound(-data->dy) % image_height;

    if (xoff < 0)
        xoff += image_width;
    if (yoff < 0)
        yoff += image_height;

    while (count--) {
        int x = spans->x;
        int length = spans->len;
        int sx = (xoff + spans->x) % image_width;
        int sy = (spans->y + yoff) % image_height;
        if (sx < 0)
            sx += image_width;
        if (sy < 0)
            sy += image_height;

        const int coverage = (spans->coverage * data->texture.const_alpha) >> 8;
        while (length) {
            int l = qMin(image_width - sx, length);
            if (buffer_size < l)
                l = buffer_size;
            const uint *src = op.src_fetch(src_buffer, &op, data, sy, sx, l);
            uint *dest = op.dest_fetch ? op.dest_fetch(buffer, data->rasterBuffer, x, spans->y, l) : buffer;
            op.func(dest, src, l, coverage);
            if (op.dest_store)
                op.dest_store(data->rasterBuffer, x, spans->y, dest, l);
            x += l;
            sx += l;
            length -= l;
            if (sx >= image_width)
                sx = 0;
        }
        ++spans;
    }
}

void qBlendTexture(int count, const QSpan *spans, void *userData)
{
    QSpanData *data = reinterpret_cast<QSpanData *>(userData);
    Q_ASSERT(data->rasterBuffer->format != QImage::Format_Invalid);
    if (data->txop <= QTransform::TxTranslate && data->texture.type == QTextureData::Tiled) {
        blend_tiled_generic(count, spans, userData);
    } else if (data->txop <= QTransform::TxTranslate) {
        blend_untransformed_generic(count, spans, userData);
    } else {
        blend_src_generic(count, spans, userData);
    }
}

template <class DST>
inline void qt_bitmapblit_template(QRasterBuffer *rasterBuffer,
                                   int x, int y, quint32 color,
                                   const uchar *map,
                                   int mapWidth, int mapHeight, int mapStride)
{
    const DST c = qt_colorConvert<DST, quint32>(color, 0);
    DST *dest = reinterpret_cast<DST*>(rasterBuffer->scanLine(y)) + x;
    const int destStride = rasterBuffer->bytesPerLine() / sizeof(DST);

    if (mapWidth > 8) {
        while (mapHeight--) {
            int x0 = 0;
            int n = 0;
            for (int x = 0; x < mapWidth; x += 8) {
                uchar s = map[x >> 3];
                for (int i = 0; i < 8; ++i) {
                    if (s & 0x80) {
                        ++n;
                    } else {
                        if (n) {
                            qt_memfill(dest + x0, c, n);
                            x0 += n + 1;
                            n = 0;
                        } else {
                            ++x0;
                        }
                        if (!s) {
                            x0 += 8 - 1 - i;
                            break;
                        }
                    }
                    s <<= 1;
                }
            }
            if (n)
                qt_memfill(dest + x0, c, n);
            dest += destStride;
            map += mapStride;
        }
    } else {
        while (mapHeight--) {
            int x0 = 0;
            int n = 0;
            for (uchar s = *map; s; s <<= 1) {
                if (s & 0x80) {
                    ++n;
                } else if (n) {
                    qt_memfill(dest + x0, c, n);
                    x0 += n + 1;
                    n = 0;
                } else {
                    ++x0;
                }
            }
            if (n)
                qt_memfill(dest + x0, c, n);
            dest += destStride;
            map += mapStride;
        }
    }
}

static void qt_gradient_quint32(int count, const QSpan *spans, void *userData)
{
    QSpanData *data = reinterpret_cast<QSpanData *>(userData);

    bool isVerticalGradient =
        data->txop <= QTransform::TxScale &&
        data->type == QSpanData::LinearGradient &&
        data->gradient.linear.end.x == data->gradient.linear.origin.x;

    if (isVerticalGradient) {
        LinearGradientValues linear;
        getLinearGradientValues(&linear, data);

        CompositionFunctionSolid funcSolid =
            functionForModeSolid[data->rasterBuffer->compositionMode];

        /*
            The logic for vertical gradient calculations is a mathematically
            reduced copy of that in fetchLinearGradient() - which is basically:

                qreal ry = data->m22 * (y + 0.5) + data->dy;
                qreal t = linear.dy*ry + linear.off;
                t *= (GRADIENT_STOPTABLE_SIZE - 1);
                quint32 color = qt_gradient_pixel_fixed(&data->gradient, int(t * FIXPT_SIZE));

            This has then been converted to fixed point to improve performance.
         */
        const int gss = GRADIENT_STOPTABLE_SIZE - 1;
        int yinc = int((linear.dy * data->m22 * gss) * FIXPT_SIZE);
        int off = int((((linear.dy * (data->m22 * qreal(0.5) + data->dy) + linear.off) * gss) * FIXPT_SIZE));

        while (count--) {
            quint32 *dst = (quint32 *)(data->rasterBuffer->scanLine(spans->y)) + spans->x;
            quint32 color = qt_gradient_pixel_fixed(&data->gradient, yinc * spans->y + off);

            funcSolid(dst, spans->len, color, spans->coverage);
            ++spans;
        }

    } else {
        blend_src_generic(count, spans, userData);
    }
}

static void qt_gradient_quint16(int count, const QSpan *spans, void *userData)
{
    QSpanData *data = reinterpret_cast<QSpanData *>(userData);

    bool isVerticalGradient =
        data->txop <= QTransform::TxScale &&
        data->type == QSpanData::LinearGradient &&
        data->gradient.linear.end.x == data->gradient.linear.origin.x;

    if (isVerticalGradient) {

        LinearGradientValues linear;
        getLinearGradientValues(&linear, data);

        /*
            The logic for vertical gradient calculations is a mathematically
            reduced copy of that in fetchLinearGradient() - which is basically:

                qreal ry = data->m22 * (y + 0.5) + data->dy;
                qreal t = linear.dy*ry + linear.off;
                t *= (GRADIENT_STOPTABLE_SIZE - 1);
                quint32 color = qt_gradient_pixel_fixed(&data->gradient, int(t * FIXPT_SIZE));

            This has then been converted to fixed point to improve performance.
         */
        const int gss = GRADIENT_STOPTABLE_SIZE - 1;
        int yinc = int((linear.dy * data->m22 * gss) * FIXPT_SIZE);
        int off = int((((linear.dy * (data->m22 * qreal(0.5) + data->dy) + linear.off) * gss) * FIXPT_SIZE));

        uint oldColor = data->solid.color;
        while (count--) {
            data->solid.color = qt_gradient_pixel_fixed(&data->gradient, yinc * spans->y + off);
            blend_color_generic(1, spans, userData);
            ++spans;
        }
        data->solid.color = oldColor;

    } else {
        blend_src_generic(count, spans, userData);
    }
}

inline static void qt_bitmapblit_quint32(QRasterBuffer *rasterBuffer,
                                   int x, int y, quint32 color,
                                   const uchar *map,
                                   int mapWidth, int mapHeight, int mapStride)
{
    qt_bitmapblit_template<quint32>(rasterBuffer, x,  y,  color,
                                    map, mapWidth, mapHeight, mapStride);
}

inline static void qt_bitmapblit_quint16(QRasterBuffer *rasterBuffer,
                                   int x, int y, quint32 color,
                                   const uchar *map,
                                   int mapWidth, int mapHeight, int mapStride)
{
    qt_bitmapblit_template<quint16>(rasterBuffer, x,  y,  color,
                                    map, mapWidth, mapHeight, mapStride);
}

static void qt_alphamapblit_quint16(QRasterBuffer *rasterBuffer,
                                    int x, int y, quint32 color,
                                    const uchar *map,
                                    int mapWidth, int mapHeight, int mapStride,
                                    const QClipData *)
{
    const quint16 c = qt_colorConvert<quint16, quint32>(color, 0);
    quint16 *dest = reinterpret_cast<quint16*>(rasterBuffer->scanLine(y)) + x;
    const int destStride = rasterBuffer->bytesPerLine() / sizeof(quint16);

    while (mapHeight--) {
        for (int i = 0; i < mapWidth; ++i) {
            const int coverage = map[i];

            if (coverage == 0) {
                // nothing
            } else if (coverage == 255) {
                dest[i] = c;
            } else {
                int ialpha = 255 - coverage;
                dest[i] = BYTE_MUL_RGB16(c, coverage)
                          + BYTE_MUL_RGB16(dest[i], ialpha);
            }
        }
        dest += destStride;
        map += mapStride;
    }
}

static inline void rgbBlendPixel(quint32 *dst, int coverage, int sr, int sg, int sb)
{
    // Do a gray alphablend...
    int da = qAlpha(*dst);
    int dr = qRed(*dst);
    int dg = qGreen(*dst);
    int db = qBlue(*dst);

    if (da != 255) {

        int a = qGray(coverage);
        sr = qt_div_255(sr * a);
        sg = qt_div_255(sg * a);
        sb = qt_div_255(sb * a);

        int ia = 255 - a;
        dr = qt_div_255(dr * ia);
        dg = qt_div_255(dg * ia);
        db = qt_div_255(db * ia);

        *dst = ((a + qt_div_255((255 - a) * da)) << 24)
            |  ((sr + dr) << 16)
            |  ((sg + dg) << 8)
            |  ((sb + db));
        return;
    }

    int mr = qRed(coverage);
    int mg = qGreen(coverage);
    int mb = qBlue(coverage);

    int nr = qt_div_255((sr - dr) * mr) + dr;
    int ng = qt_div_255((sg - dg) * mg) + dg;
    int nb = qt_div_255((sb - db) * mb) + db;

    *dst = qRgb(nr, ng, nb);
}

static void qt_alphamapblit_quint32(QRasterBuffer *rasterBuffer,
                                    int x, int y, quint32 color,
                                    const uchar *map,
                                    int mapWidth, int mapHeight, int mapStride,
                                    const QClipData *clip)
{
    const int destStride = rasterBuffer->bytesPerLine() / sizeof(quint32);

    if (!clip) {
        quint32 *dest = reinterpret_cast<quint32*>(rasterBuffer->scanLine(y)) + x;
        while (mapHeight--) {
            for (int i = 0; i < mapWidth; ++i) {
                const int coverage = map[i];

                if (coverage == 0) {
                    // nothing
                } else if (coverage == 255) {
                    dest[i] = color;
                } else {
                    const int ialpha = 255 - coverage;
                    dest[i] = INTERPOLATE_PIXEL_255(color, coverage, dest[i], ialpha);
                }
            }
            dest += destStride;
            map += mapStride;
        }
    } else {
        const int bottom = qMin(y + mapHeight, rasterBuffer->height());

        const int top = qMax(y, 0);
        map += (top - y) * mapStride;

        const_cast<QClipData *>(clip)->initialize();
        for (int yp = top; yp<bottom; ++yp) {
            const QClipData::ClipLine &line = clip->m_clipLines[yp];

            quint32 *dest = reinterpret_cast<quint32 *>(rasterBuffer->scanLine(yp));

            for (int i=0; i<line.count; ++i) {
                const QSpan &clip = line.spans[i];

                const int start = qMax<int>(x, clip.x);
                const int end = qMin<int>(x + mapWidth, clip.x + clip.len);

                for (int xp=start; xp<end; ++xp) {
                    const int coverage = map[xp - x];

                    if (coverage == 0) {
                        // nothing
                    } else if (coverage == 255) {
                        dest[xp] = color;
                    } else {
                        const int ialpha = 255 - coverage;
                        dest[xp] = INTERPOLATE_PIXEL_255(color, coverage, dest[xp], ialpha);
                    }

                } // for (i -> line.count)
            } // for (yp -> bottom)
            map += mapStride;
        }
    }
}

static void qt_alphargbblit_quint32(QRasterBuffer *rasterBuffer,
                                    int x, int y, quint32 color,
                                    const uint *src, int mapWidth, int mapHeight, int srcStride,
                                    const QClipData *clip)
{
    int sr = qRed(color);
    int sg = qGreen(color);
    int sb = qBlue(color);
    int sa = qAlpha(color);

    if (sa == 0)
        return;

    if (!clip) {
        quint32 *dst = reinterpret_cast<quint32*>(rasterBuffer->scanLine(y)) + x;
        const int destStride = rasterBuffer->bytesPerLine() / sizeof(quint32);
        while (mapHeight--) {
            for (int i = 0; i < mapWidth; ++i) {
                const uint coverage = src[i];
                if (coverage == 0xffffffff) {
                    dst[i] = color;
                } else if (coverage != 0xff000000) {
                    rgbBlendPixel(dst+i, coverage, sr, sg, sb);
                }
            }

            dst += destStride;
            src += srcStride;
        }
    } else {
        int bottom = qMin(y + mapHeight, rasterBuffer->height());

        int top = qMax(y, 0);
        src += (top - y) * srcStride;

        const_cast<QClipData *>(clip)->initialize();
        for (int yp = top; yp<bottom; ++yp) {
            const QClipData::ClipLine &line = clip->m_clipLines[yp];

            quint32 *dst = reinterpret_cast<quint32 *>(rasterBuffer->scanLine(yp));

            for (int i=0; i<line.count; ++i) {
                const QSpan &clip = line.spans[i];

                int start = qMax<int>(x, clip.x);
                int end = qMin<int>(x + mapWidth, clip.x + clip.len);

                for (int xp=start; xp<end; ++xp) {
                    const uint coverage = src[xp - x];
                    if (coverage == 0xffffffff) {
                        dst[xp] = color;
                    } else if (coverage != 0xff000000) {
                        rgbBlendPixel(dst+xp, coverage, sr, sg, sb);
                    }
                }
            } // for (i -> line.count)
            src += srcStride;
        } // for (yp -> bottom)

    }
}

inline static void qt_rectfill_quint32(QRasterBuffer *rasterBuffer,
                                    int x, int y, int width, int height,
                                    quint32 color)
{
    qt_rectfill<quint32>(reinterpret_cast<quint32*>(rasterBuffer->buffer()),
                color, x, y, width, height, rasterBuffer->bytesPerLine());
}

inline static void qt_rectfill_quint16(QRasterBuffer *rasterBuffer,
                                    int x, int y, int width, int height,
                                    quint32 color)
{
    qt_rectfill<quint16>(reinterpret_cast<quint16*>(rasterBuffer->buffer()),
                qt_colorConvert<quint16, quint32>(color, 0),
                x, y, width, height, rasterBuffer->bytesPerLine());
}

inline static void qt_rectfill_nonpremul_quint32(QRasterBuffer *rasterBuffer,
                                                 int x, int y, int width, int height,
                                                 quint32 color)
{
    qt_rectfill<quint32>(reinterpret_cast<quint32 *>(rasterBuffer->buffer()),
                         INV_PREMUL(color), x, y, width, height, rasterBuffer->bytesPerLine());
}


// Map table for destination image format. Contains function pointers
// for blends of various types unto the destination

DrawHelper qDrawHelper[QImage::NImageFormats] =
{
    // Format_Invalid,
    { 0, 0, 0, 0, 0, 0 },
    // Format_Mono,
    {
        blend_color_generic,
        blend_src_generic,
        0, 0, 0, 0
    },
    // Format_MonoLSB,
    {
        blend_color_generic,
        blend_src_generic,
        0, 0, 0, 0
    },
    // Format_Indexed8,
    {
        blend_color_generic,
        blend_src_generic,
        0, 0, 0, 0
    },
    // Format_RGB32,
    {
        blend_color_generic,
        qt_gradient_quint32,
        qt_bitmapblit_quint32,
        qt_alphamapblit_quint32,
        qt_alphargbblit_quint32,
        qt_rectfill_quint32
    },
    // Format_ARGB32,
    {
        blend_color_generic,
        qt_gradient_quint32,
        qt_bitmapblit_quint32,
        qt_alphamapblit_quint32,
        qt_alphargbblit_quint32,
        qt_rectfill_nonpremul_quint32
    },
    // Format_ARGB32_Premultiplied
    {
        blend_color_generic,
        qt_gradient_quint32,
        qt_bitmapblit_quint32,
        qt_alphamapblit_quint32,
        qt_alphargbblit_quint32,
        qt_rectfill_quint32
    },
    // Format_RGB16
    {
        blend_color_generic,
        qt_gradient_quint16,
        qt_bitmapblit_quint16,
        qt_alphamapblit_quint16,
        0,
        qt_rectfill_quint16
    }
};

QT_END_NAMESPACE
