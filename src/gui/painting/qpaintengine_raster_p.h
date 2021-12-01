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

#ifndef QPAINTENGINE_RASTER_P_H
#define QPAINTENGINE_RASTER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qpaintengineex_p.h"
#include "QtGui/qpainterpath.h"
#include "qdrawhelper_p.h"
#include "qpaintengine_p.h"
#include "qrasterizer_p.h"
#include "qstroker_p.h"
#include "qpainter_p.h"
#include "qtextureglyphcache_p.h"
#include "qoutlinemapper_p.h"

#include <stdlib.h>

QT_BEGIN_NAMESPACE

class QRasterPaintEnginePrivate;
class QRasterBuffer;
class QClipData;

class QRasterPaintEngineState : public QPainterState
{
public:
    QRasterPaintEngineState(QRasterPaintEngineState &other);
    QRasterPaintEngineState();
    ~QRasterPaintEngineState();


    QPen lastPen;
    QSpanData penData;
    QStrokerOps *stroker;
    QPaintEngine::DirtyFlags strokeFlags;

    QBrush lastBrush;
    QSpanData brushData;
    QPaintEngine::DirtyFlags fillFlags;

    int intOpacity;

    qreal txscale;

    QClipData *clip;
//     QRect clipRect;
//     QRegion clipRegion;

//     QPainter::RenderHints hints;
//     QPainter::CompositionMode compositionMode;

    QPaintEngine::DirtyFlags dirty;

    struct Flags {
        bool has_clip_ownership;        // should delete the clip member..
        bool non_complex_pen;           // can use rasterizer, rather than stroker
        bool antialiased;
        bool bilinear;
        bool fast_text;
        bool tx_noshear;
    };

    Flags flags;
};




/*******************************************************************************
 * QRasterPaintEngine
 */
class QRasterPaintEngine : public QPaintEngineEx
{
    Q_DECLARE_PRIVATE(QRasterPaintEngine)
public:

    QRasterPaintEngine(QPaintDevice *device);
    bool begin(QPaintDevice *device);
    bool end();

    void penChanged();
    void brushChanged();
    void brushOriginChanged();
    void opacityChanged();
    void compositionModeChanged();
    void renderHintsChanged();
    void transformChanged();
    void clipEnabledChanged();

    void setState(QPainterState *s);
    QPainterState *createState(QPainterState *orig) const;
    inline QRasterPaintEngineState *state() {
        return static_cast<QRasterPaintEngineState *>(QPaintEngineEx::state());
    }
    inline const QRasterPaintEngineState *state() const {
        return static_cast<const QRasterPaintEngineState *>(QPaintEngineEx::state());
    }

    void updateBrush(const QBrush &brush);
    void updatePen(const QPen &pen);

    void updateMatrix(const QTransform &matrix);

    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);
    void fillPath(const QPainterPath &path, QSpanData *fillData);
    void fillPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);

    void drawEllipse(const QRectF &rect);

    void fillRect(const QRectF &rect, const QBrush &brush);
    void fillRect(const QRectF &rect, const QColor &color);

    void drawRects(const QRect  *rects, int rectCount);

    void drawPixmap(const QPointF &p, const QPixmap &pm);
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawImage(const QPointF &p, const QImage &img);
    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                   Qt::ImageConversionFlags flags = Qt::AutoColor);
    void drawTiledPixmap(const QRectF &r, const QPixmap &pm, const QPointF &sr);
    void drawTextItem(const QPointF &p, const QTextItem &textItem);

    void drawLines(const QLine *line, int lineCount);
    void drawLines(const QLineF *line, int lineCount);

    void drawPoints(const QPointF *points, int pointCount);
    void drawPoints(const QPoint *points, int pointCount);

    void stroke(const QVectorPath &path, const QPen &pen);
    void fill(const QVectorPath &path, const QBrush &brush);

    void clip(const QVectorPath &path, Qt::ClipOperation op);
    void clip(const QRect &rect, Qt::ClipOperation op);
    void clip(const QRegion &region, Qt::ClipOperation op);

    void drawStaticTextItem(QStaticTextItem *textItem);

#ifdef Q_NO_USING_KEYWORD
    inline void drawEllipse(const QRect &rect) { QPaintEngineEx::drawEllipse(rect); }
#else
    using QPaintEngineEx::drawEllipse;
#endif

#ifndef QT_NO_DEBUG
    void saveBuffer(const QString &s) const;
#endif

    void alphaPenBlt(const void* src, const int bpl, const int depth,
                    int rx, int ry, int w, int h);

    Type type() const { return Raster; }

    bool supportsTransformations(const QFontEngine *fontEngine) const;
    bool supportsTransformations(const qreal pixelSize, const QTransform &m) const;

protected:
    QRasterPaintEngine(QRasterPaintEnginePrivate &d, QPaintDevice *);
private:
    friend class QSpanData;
    void init();

    void fillRect(const QRectF &rect, QSpanData *data);
    void drawBitmap(const QPointF &pos, const QImage &image, QSpanData *fill);

    bool drawCachedGlyphs(int numGlyphs, const glyph_t *glyphs, const QFixedPoint *positions,
                          QFontEngine *fontEngine);


    bool setClipRectInDeviceCoords(const QRect &r, Qt::ClipOperation op);

    inline void ensureBrush(const QBrush &brush) {
        if (state()->lastBrush != brush || (brush.style() != Qt::NoBrush && state()->fillFlags))
            updateBrush(brush);
    }
    inline void ensureBrush() { ensureBrush(state()->brush); }

    inline void ensurePen(const QPen &pen) {
        if (state()->lastPen != pen || (pen.style() != Qt::NoPen && state()->strokeFlags))
            updatePen(pen);
    }
    inline void ensurePen() { ensurePen(state()->pen); }

    void updateOutlineMapper();

    void updateRasterState();
    inline void ensureRasterState() {
        if (state()->dirty)
            updateRasterState();
    }
};


/*******************************************************************************
 * QRasterPaintEnginePrivate
 */
class QRasterPaintEnginePrivate : public QPaintEngineExPrivate
{
    Q_DECLARE_PUBLIC(QRasterPaintEngine)
public:
    QRasterPaintEnginePrivate();

    void rasterizeLine_dashed(QLineF line, qreal width,
                              int *dashIndex, qreal *dashOffset, bool *inDash);
    void rasterize(QT_FT_Outline *outline, ProcessSpans callback, void *userData);
    void updateMatrixData(QSpanData *spanData, const QBrush &brush, const QTransform &brushMatrix);

    void systemStateChanged();

    QTransform brushMatrix() const {
        Q_Q(const QRasterPaintEngine);
        const QRasterPaintEngineState *s = q->state();
        QTransform m(s->matrix);
        m.translate(s->brushOrigin.x(), s->brushOrigin.y());
        return m;
    }

    bool isUnclipped_normalized(const QRect &rect) const;
    bool isUnclipped(const QRect &rect) const;
    bool isUnclipped(const QRectF &rect) const;
    ProcessSpans getBrushFunc(const QRect &rect, const QSpanData *data) const;
    ProcessSpans getBrushFunc(const QRectF &rect, const QSpanData *data) const;

    inline const QClipData *clip() const;

    void initializeRasterizer(QSpanData *data);

    QPaintDevice *device;
    QScopedPointer<QOutlineMapper> outlineMapper;
    QScopedPointer<QRasterBuffer>  rasterBuffer;

    QRect deviceRect;

    QStroker basicStroker;
    QScopedPointer<QDashStroker> dashStroker;

    QSpanData image_filler;
    QSpanData image_filler_xform;
    QSpanData solid_color_filler;


    QFontEngineGlyphCache::Type glyphCacheType;

    QScopedPointer<QClipData> baseClip;

    int deviceDepth;

    bool mono_surface;

    QScopedPointer<QRasterizer> rasterizer;
};


class QClipData {
public:
    QClipData(int height);
    ~QClipData();

    int clipSpanHeight;
    struct ClipLine {
        int count;
        QSpan *spans;
    } *m_clipLines;

    void initialize();

    inline ClipLine *clipLines() {
        if (!m_clipLines)
            initialize();
        return m_clipLines;
    }

    inline QSpan *spans() {
        if (!m_spans)
            initialize();
        return m_spans;
    }

    int allocated;
    int count;
    QSpan *m_spans;
    int xmin, xmax, ymin, ymax;

    QRect clipRect;
    QRegion clipRegion;

    bool enabled;
    bool hasRectClip;
    bool hasRegionClip;

    void appendSpan(const int x, const int length, const int y, const int coverage);
    void appendSpans(const QSpan *s, const int num);

    // ### Should optimize and actually kill the QSpans if the rect is
    // ### a subset of The current region. Thus the "fast" clipspan
    // ### callback can be used
    void setClipRect(const QRect &rect);
    void setClipRegion(const QRegion &region);
    void fixup();
};

inline void QClipData::appendSpan(const int x, const int length, const int y, const int coverage)
{
    Q_ASSERT(m_spans); // initialize() has to be called prior to adding spans..

    if (count == allocated) {
        allocated *= 2;
        m_spans = (QSpan *)realloc(m_spans, allocated*sizeof(QSpan));
    }
    m_spans[count].x = x;
    m_spans[count].len = length;
    m_spans[count].y = y;
    m_spans[count].coverage = coverage;
    ++count;
}

inline void QClipData::appendSpans(const QSpan *s, const int num)
{
    Q_ASSERT(m_spans);

    if (count + num > allocated) {
        do {
            allocated *= 2;
        } while (count + num > allocated);
        m_spans = (QSpan *)realloc(m_spans, allocated*sizeof(QSpan));
    }
    memcpy(m_spans+count, s, num*sizeof(QSpan));
    count += num;
}

/*******************************************************************************
 * QRasterBuffer
 */
class QRasterBuffer
{
public:
    QRasterBuffer();

    ~QRasterBuffer();

    QImage::Format prepare(QImage *image);
    QImage::Format prepare(QPixmap *pix);
    void prepare(int w, int h);

    uchar *scanLine(int y) { Q_ASSERT(y>=0); Q_ASSERT(y<m_height); return m_buffer + y * bytes_per_line; }
    const uchar *scanLine(int y) const { Q_ASSERT(y>=0); Q_ASSERT(y<m_height); return m_buffer + y * bytes_per_line; }

#ifndef QT_NO_DEBUG
    QImage bufferImage() const;
#endif

    int width() const { return m_width; }
    int height() const { return m_height; }
    int bytesPerLine() const { return bytes_per_line; }
    int bytesPerPixel() const { return bytes_per_pixel; }

    uchar *buffer() const { return m_buffer; }

    bool monoDestinationWithClut;
    QRgb destColor0;
    QRgb destColor1;

    QPainter::CompositionMode compositionMode;
    QImage::Format format;
    DrawHelper *drawHelper;
    QImage colorizeBitmap(const QImage &image, const QColor &color);

private:
    int m_width;
    int m_height;
    int bytes_per_line;
    int bytes_per_pixel;
    uchar *m_buffer;
};

inline const QClipData *QRasterPaintEnginePrivate::clip() const {
    Q_Q(const QRasterPaintEngine);
    if (q->state() && q->state()->clip && q->state()->clip->enabled)
        return q->state()->clip;
    return baseClip.data();
}


QT_END_NAMESPACE
#endif // QPAINTENGINE_RASTER_P_H
