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

#include "qmutex.h"
#include "qrasterdefs_p.h"
#include "qpainterpath.h"
#include "qdebug.h"
#include "qcache.h"
#include "qlabel.h"
#include "qbitmap.h"
#include "qmath.h"
#include "qfontengine_ft_p.h"
#include "qtextengine_p.h"
#include "qfontengine_p.h"
#include "qpixmap_raster_p.h"
#include "qimage_p.h"
#include "qstatictext_p.h"
#include "qpaintengine_raster_p.h"
#include "qoutlinemapper_p.h"
#include "qguicommon_p.h"
#include "qcorecommon_p.h"

#include <limits.h>

QT_BEGIN_NAMESPACE

Q_GUI_EXPORT extern bool qt_scaleForTransform(const QTransform &transform, qreal *scale); // qtransform.cpp

// #define QT_DEBUG_DRAW
#ifdef QT_DEBUG_DRAW
void dumpClip(int width, int height, const QClipData *clip);
#endif

/********************************************************************************
 * Span functions
 */
static void qt_span_fill_clipRect(int count, const QSpan *spans, void *userData);
static void qt_span_fill_clipped(int count, const QSpan *spans, void *userData);
static void qt_span_clip(int count, const QSpan *spans, void *userData);
static void qt_merge_clip(const QClipData *c1, const QClipData *c2, QClipData *result);

struct ClipData
{
    QClipData *oldClip;
    QClipData *newClip;
    Qt::ClipOperation operation;
};

#ifdef QT_DEBUG_DRAW
static const QRectF boundingRect(const QPointF *points, int pointCount)
{
    const QPointF *e = points;
    const QPointF *last = points + pointCount;
    qreal minx, maxx, miny, maxy;
    minx = maxx = e->x();
    miny = maxy = e->y();
    while (++e < last) {
        if (e->x() < minx)
            minx = e->x();
        else if (e->x() > maxx)
            maxx = e->x();
        if (e->y() < miny)
            miny = e->y();
        else if (e->y() > maxy)
            maxy = e->y();
    }
    return QRectF(QPointF(minx, miny), QPointF(maxx, maxy));
}
#endif

template <typename T> static inline bool isRect(const T *pts, int elementCount) {
    return (elementCount == 5 // 5-point polygon, check for closed rect
            && pts[0] == pts[8] && pts[1] == pts[9] // last point == first point
            && pts[0] == pts[6] && pts[2] == pts[4] // x values equal
            && pts[1] == pts[3] && pts[5] == pts[7] // y values equal...
            && pts[0] < pts[4] && pts[1] < pts[5]
            ) ||
           (elementCount == 4 // 4-point polygon, check for unclosed rect
            && pts[0] == pts[6] && pts[2] == pts[4] // x values equal
            && pts[1] == pts[3] && pts[5] == pts[7] // y values equal...
            && pts[0] < pts[4] && pts[1] < pts[5]
            );
}


static void qt_ft_outline_move_to(qfixed x, qfixed y, void *data)
{
    ((QOutlineMapper *) data)->moveTo(QPointF(x, y));
}

static void qt_ft_outline_line_to(qfixed x, qfixed y, void *data)
{
    ((QOutlineMapper *) data)->lineTo(QPointF(x, y));
}

static void qt_ft_outline_cubic_to(qfixed c1x, qfixed c1y,
                             qfixed c2x, qfixed c2y,
                             qfixed ex, qfixed ey,
                             void *data)
{
    ((QOutlineMapper *) data)->curveTo(QPointF(c1x, c1y),
                                       QPointF(c2x, c2y),
                                       QPointF(ex, ey));
}



QRasterPaintEnginePrivate::QRasterPaintEnginePrivate() :
    QPaintEngineExPrivate()
{
}

/*!
    \class QRasterPaintEngine
    \preliminary
    \since 4.2

    \brief The QRasterPaintEngine class enables hardware acceleration
    of painting operations in Qt for Embedded Linux.

    Note that this functionality is only available in
    \l{Qt for Embedded Linux}.

    \note The QRasterPaintEngine class does not support 8-bit images.
    Instead, they need to be converted to a supported format, such as
    QImage::Format_ARGB32_Premultiplied.

    See the \l {Adding an Accelerated Graphics Driver to Qt for Embedded Linux}
    documentation for details.

    \sa QPaintEngine
*/

/*!
    \fn Type QRasterPaintEngine::type() const
    \reimp
*/

/*!
    \typedef QSpan
    \relates QRasterPaintEngine

    A struct equivalent to QT_FT_Span, containing a position (x,
    y), the span's length in pixels and its color/coverage (a value
    ranging from 0 to 255).
*/

/*!
    \since 4.5

    Creates a raster based paint engine for operating on the given
    \a device, with the complete set of \l
    {QPaintEngine::PaintEngineFeature}{paint engine features and
    capabilities}.
*/
QRasterPaintEngine::QRasterPaintEngine(QPaintDevice *device)
    : QPaintEngineEx(*(new QRasterPaintEnginePrivate))
{
    d_func()->device = device;
    init();
}

/*!
    \internal
*/
QRasterPaintEngine::QRasterPaintEngine(QRasterPaintEnginePrivate &dd, QPaintDevice *device)
    : QPaintEngineEx(dd)
{
    d_func()->device = device;
    init();
}

void QRasterPaintEngine::init()
{
    Q_D(QRasterPaintEngine);

    d->rasterizer.reset(new QRasterizer);
    d->rasterBuffer.reset(new QRasterBuffer());
    d->outlineMapper.reset(new QOutlineMapper);

    d->basicStroker.setMoveToHook(qt_ft_outline_move_to);
    d->basicStroker.setLineToHook(qt_ft_outline_line_to);
    d->basicStroker.setCubicToHook(qt_ft_outline_cubic_to);

    d->baseClip.reset(new QClipData(d->device->height()));
    d->baseClip->setClipRect(QRect(0, 0, d->device->width(), d->device->height()));

    d->image_filler.init(d->rasterBuffer.data(), this);
    d->image_filler.type = QSpanData::Texture;

    d->image_filler_xform.init(d->rasterBuffer.data(), this);
    d->image_filler_xform.type = QSpanData::Texture;

    d->solid_color_filler.init(d->rasterBuffer.data(), this);
    d->solid_color_filler.type = QSpanData::Solid;

    d->deviceDepth = d->device->depth();

    d->mono_surface = false;
    gccaps &= ~PorterDuff;

    if (Q_UNLIKELY(d->device->devType() != QInternal::Image)) {
        qWarning("QRasterPaintEngine: unsupported target device %d\n", d->device->devType());
        d->device = 0;
        return;
    }

    QImage::Format format = d->rasterBuffer->prepare(static_cast<QImage *>(d->device));
    switch (format) {
    case QImage::Format_MonoLSB:
    case QImage::Format_Mono:
        d->mono_surface = true;
        break;
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_ARGB32:
        gccaps |= PorterDuff;
        break;
    case QImage::Format_RGB32:
    case QImage::Format_RGB16:
        break;
    default:
        break;
    }
}

/*!
    \reimp
*/
bool QRasterPaintEngine::begin(QPaintDevice *device)
{
    Q_D(QRasterPaintEngine);

    if (device->devType() == QInternal::Pixmap) {
        QPixmap *pixmap = static_cast<QPixmap *>(device);
        QPixmapData *pd = pixmap->pixmapData();
        if (pd->classId() == QPixmapData::RasterClass)
            d->device = pd->buffer();
    } else {
        d->device = device;
    }

    // Make sure QPaintEngine::paintDevice() returns the proper device.
    d->pdev = d->device;

    Q_ASSERT(d->device->devType() == QInternal::Image);

    d->systemStateChanged();

    QRasterPaintEngineState *s = state();
    updateOutlineMapper();
    d->outlineMapper->m_clip_rect = d->deviceRect;

    if (d->outlineMapper->m_clip_rect.width() > RASTER_COORD_LIMIT)
        d->outlineMapper->m_clip_rect.setWidth(RASTER_COORD_LIMIT);
    if (d->outlineMapper->m_clip_rect.height() > RASTER_COORD_LIMIT)
        d->outlineMapper->m_clip_rect.setHeight(RASTER_COORD_LIMIT);

    d->rasterizer->setClipRect(d->deviceRect);

    s->penData.init(d->rasterBuffer.data(), this);
    s->penData.setup(s->pen.brush(), s->intOpacity, s->composition_mode);
    s->stroker = &d->basicStroker;
    d->basicStroker.setClipRect(d->deviceRect);

    s->brushData.init(d->rasterBuffer.data(), this);
    s->brushData.setup(s->brush, s->intOpacity, s->composition_mode);

    d->rasterBuffer->compositionMode = QPainter::CompositionMode_SourceOver;

    setDirty(DirtyBrushOrigin);

#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::begin(" << (void *) device
             << ") devType:" << device->devType()
             << "devRect:" << d->deviceRect;
    if (d->baseClip) {
        dumpClip(d->rasterBuffer->width(), d->rasterBuffer->height(), &*d->baseClip);
    }
#endif

    if (d->mono_surface)
        d->glyphCacheType = QFontEngineGlyphCache::Raster_Mono;
    d->glyphCacheType = QFontEngineGlyphCache::Raster_A8;

    setActive(true);
    return true;
}

/*!
    \reimp
*/
bool QRasterPaintEngine::end()
{
#ifdef QT_DEBUG_DRAW
    Q_D(QRasterPaintEngine);
    qDebug() << "QRasterPaintEngine::end devRect:" << d->deviceRect;
    if (d->baseClip) {
        dumpClip(d->rasterBuffer->width(), d->rasterBuffer->height(), &*d->baseClip);
    }
#endif

    return true;
}

/*!
    \internal
*/
#ifndef QT_NO_DEBUG
void QRasterPaintEngine::saveBuffer(const QString &s) const
{
    Q_D(const QRasterPaintEngine);
    d->rasterBuffer->bufferImage().save(s, "PNG");
}
#endif

/*!
    \internal
*/
void QRasterPaintEngine::updateMatrix(const QTransform &matrix)
{
    QRasterPaintEngineState *s = state();
    // FALCON: get rid of this line, see drawImage call below.
    s->matrix = matrix;
    s->flags.tx_noshear = qt_scaleForTransform(s->matrix, &s->txscale);

    updateOutlineMapper();
}



QRasterPaintEngineState::~QRasterPaintEngineState()
{
    if (flags.has_clip_ownership)
        delete clip;
}


QRasterPaintEngineState::QRasterPaintEngineState()
{
    stroker = 0;

    fillFlags = 0;
    strokeFlags = 0;

    intOpacity = 256;

    txscale = 1.;

    flags.antialiased = false;
    flags.bilinear = false;
    flags.fast_text = true;
    flags.tx_noshear = true;

    clip = 0;
    flags.has_clip_ownership = false;

    dirty = 0;
}

QRasterPaintEngineState::QRasterPaintEngineState(QRasterPaintEngineState &s)
    : QPainterState(s)
    , lastPen(s.lastPen)
    , penData(s.penData)
    , stroker(s.stroker)
    , strokeFlags(s.strokeFlags)
    , lastBrush(s.lastBrush)
    , brushData(s.brushData)
    , fillFlags(s.fillFlags)
    , intOpacity(s.intOpacity)
    , txscale(s.txscale)
    , clip(s.clip)
    , dirty(s.dirty)
    , flags(s.flags)
{
    brushData.tempImage = 0;
    penData.tempImage = 0;
    flags.has_clip_ownership = false;
}

/*!
    \internal
*/
QPainterState *QRasterPaintEngine::createState(QPainterState *orig) const
{
    if (!orig)
        return new QRasterPaintEngineState();
    return new QRasterPaintEngineState(*static_cast<QRasterPaintEngineState *>(orig));
}

/*!
    \internal
*/
void QRasterPaintEngine::setState(QPainterState *s)
{
    Q_D(QRasterPaintEngine);
    QPaintEngineEx::setState(s);
    d->rasterBuffer->compositionMode = s->composition_mode;
}

/*!
    \fn QRasterPaintEngineState *QRasterPaintEngine::state()
    \internal
*/

/*!
    \fn const QRasterPaintEngineState *QRasterPaintEngine::state() const
    \internal
*/

/*!
    \internal
*/
void QRasterPaintEngine::penChanged()
{
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::penChanged():" << state()->pen;
#endif
    QRasterPaintEngineState *s = state();
    s->strokeFlags |= DirtyPen;
    s->dirty |= DirtyPen;
}

/*!
    \internal
*/
void QRasterPaintEngine::updatePen(const QPen &pen)
{
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::updatePen():" << s->pen;
#endif

    Qt::PenStyle pen_style = pen.style();

    s->lastPen = pen;
    s->strokeFlags = 0;

    s->penData.clip = d->clip();
    s->penData.setup(pen_style == Qt::NoPen ? QBrush() : pen.brush(), s->intOpacity, s->composition_mode);

    if (s->strokeFlags & QRasterPaintEngine::DirtyTransform
        || pen.brush().transform().type() >= QTransform::TxNone) {
        d->updateMatrixData(&s->penData, pen.brush(), s->matrix);
    }

    // Slightly ugly handling of an uncommon case... We need to change
    // the pen because it is reused in draw_midpoint to decide dashed
    // or non-dashed.
    if (pen_style == Qt::CustomDashLine && pen.dashPattern().size() == 0) {
        pen_style = Qt::SolidLine;
        s->lastPen.setStyle(Qt::SolidLine);
    }

    d->basicStroker.setJoinStyle(pen.joinStyle());
    d->basicStroker.setCapStyle(pen.capStyle());
    d->basicStroker.setMiterLimit(pen.miterLimit());

    const qreal penWidth = pen.widthF();
    if (penWidth == 0)
        d->basicStroker.setStrokeWidth(1);
    else
        d->basicStroker.setStrokeWidth(penWidth);

    if(pen_style == Qt::SolidLine) {
        s->stroker = &d->basicStroker;
    } else if (pen_style != Qt::NoPen) {
        if (!d->dashStroker)
            d->dashStroker.reset(new QDashStroker(&d->basicStroker));
        if (pen.isCosmetic()) {
            d->dashStroker->setClipRect(d->deviceRect);
        } else {
            // ### I've seen this inverted devrect multiple places now...
            QRectF clipRect = s->matrix.inverted().mapRect(QRectF(d->deviceRect));
            d->dashStroker->setClipRect(clipRect);
        }
        d->dashStroker->setDashPattern(pen.dashPattern());
        d->dashStroker->setDashOffset(pen.dashOffset());
        s->stroker = d->dashStroker.data();
    } else {
        s->stroker = 0;
    }

    ensureRasterState(); // needed because of tx_noshear...

    s->flags.non_complex_pen = s->lastPen.capStyle() <= Qt::SquareCap && s->flags.tx_noshear;

    s->strokeFlags = 0;
}



/*!
    \internal
*/
void QRasterPaintEngine::brushOriginChanged()
{
    QRasterPaintEngineState *s = state();
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::brushOriginChanged()" << s->brushOrigin;
#endif

    s->fillFlags |= DirtyBrushOrigin;
}


/*!
    \internal
*/
void QRasterPaintEngine::brushChanged()
{
    QRasterPaintEngineState *s = state();
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::brushChanged():" << s->brush;
#endif
    s->fillFlags |= DirtyBrush;
}




/*!
    \internal
*/
void QRasterPaintEngine::updateBrush(const QBrush &brush)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::updateBrush()" << brush;
#endif
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();
    // must set clip prior to setup, as setup uses it...
    s->brushData.clip = d->clip();
    s->brushData.setup(brush, s->intOpacity, s->composition_mode);
    if (s->fillFlags & DirtyTransform
        || brush.transform().type() >= QTransform::TxNone)
        d_func()->updateMatrixData(&s->brushData, brush, d->brushMatrix());
    s->lastBrush = brush;
    s->fillFlags = 0;
}

void QRasterPaintEngine::updateOutlineMapper()
{
    Q_D(QRasterPaintEngine);
    d->outlineMapper->setMatrix(state()->matrix);
}

void QRasterPaintEngine::updateRasterState()
{
    QRasterPaintEngineState *s = state();

    if (s->dirty & DirtyTransform)
        updateMatrix(s->matrix);

    if (s->dirty & (DirtyPen|DirtyCompositionMode|DirtyOpacity)) {
        const QPainter::CompositionMode mode = s->composition_mode;
        s->flags.fast_text = (s->penData.type == QSpanData::Solid)
                       && s->intOpacity == 256
                       && (mode == QPainter::CompositionMode_Source
                           || (mode == QPainter::CompositionMode_SourceOver
                               && qAlpha(s->penData.solid.color) == 255));
    }

    s->dirty = 0;
}


/*!
    \internal
*/
void QRasterPaintEngine::opacityChanged()
{
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::opacityChanged()" << s->opacity;
#endif

    s->fillFlags |= DirtyOpacity;
    s->strokeFlags |= DirtyOpacity;
    s->dirty |= DirtyOpacity;
    s->intOpacity = (int) (s->opacity * 256);
}

/*!
    \internal
*/
void QRasterPaintEngine::compositionModeChanged()
{
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::compositionModeChanged()" << s->composition_mode;
#endif

    s->fillFlags |= DirtyCompositionMode;
    s->dirty |= DirtyCompositionMode;

    s->strokeFlags |= DirtyCompositionMode;
    d->rasterBuffer->compositionMode = s->composition_mode;
}

/*!
    \internal
*/
void QRasterPaintEngine::renderHintsChanged()
{
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::renderHintsChanged()" << hex << s->renderHints;
#endif

    bool was_aa = s->flags.antialiased;
    bool was_bilinear = s->flags.bilinear;

    s->flags.antialiased = bool(s->renderHints & QPainter::Antialiasing);
    s->flags.bilinear = bool(s->renderHints & QPainter::SmoothPixmapTransform);

    if (was_aa != s->flags.antialiased)
        s->strokeFlags |= DirtyHints;

    if (was_bilinear != s->flags.bilinear) {
        s->strokeFlags |= DirtyPen;
        s->fillFlags |= DirtyBrush;
    }
}

/*!
    \internal
*/
void QRasterPaintEngine::transformChanged()
{
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::transformChanged()" << s->matrix;
#endif

    s->fillFlags |= DirtyTransform;
    s->strokeFlags |= DirtyTransform;

    s->dirty |= DirtyTransform;
}

/*!
    \internal
*/
void QRasterPaintEngine::clipEnabledChanged()
{
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::clipEnabledChanged()" << s->clipEnabled;
#endif

    if (s->clip) {
        s->clip->enabled = s->clipEnabled;
        s->fillFlags |= DirtyClipEnabled;
        s->strokeFlags |= DirtyClipEnabled;
    }
}

void QRasterPaintEnginePrivate::systemStateChanged()
{
    const QRect deviceRectUnclipped = QRect(0, 0,
            qMin(RASTER_COORD_LIMIT, device->width()),
            qMin(RASTER_COORD_LIMIT, device->height()));

    if (!systemClip.isEmpty()) {
        QRegion clippedDeviceRgn = systemClip & deviceRectUnclipped;
        deviceRect = clippedDeviceRgn.boundingRect();
        baseClip->setClipRegion(clippedDeviceRgn);
    } else {
        deviceRect = deviceRectUnclipped;
        baseClip->setClipRect(deviceRect);
    }
#ifdef QT_DEBUG_DRAW
    qDebug() << "systemStateChanged" << this << "deviceRect" << deviceRect << deviceRectUnclipped << systemClip;
#endif

    exDeviceRect = deviceRect;

    Q_Q(QRasterPaintEngine);
    q->state()->strokeFlags |= QPaintEngine::DirtyClipRegion;
    q->state()->fillFlags |= QPaintEngine::DirtyClipRegion;
}

void QRasterPaintEnginePrivate::updateMatrixData(QSpanData *spanData, const QBrush &b, const QTransform &m)
{
    const Qt::BrushStyle bstyle(b.style());
    if (bstyle == Qt::NoBrush || bstyle == Qt::SolidPattern)
        return;

    Q_Q(QRasterPaintEngine);
    bool bilinear = q->state()->flags.bilinear;

    const QTransform btransform(b.transform());
    if (btransform.type() > QTransform::TxNone) { // FALCON: optimize
        spanData->setupMatrix(btransform * m, bilinear);
    } else if (m.type() <= QTransform::TxTranslate) {
        // specialize setupMatrix for translation matrices
        // to avoid needless matrix inversion
        spanData->m11 = 1;
        spanData->m12 = 0;
        spanData->m13 = 0;
        spanData->m21 = 0;
        spanData->m22 = 1;
        spanData->m23 = 0;
        spanData->m33 = 1;
        spanData->dx = -m.dx();
        spanData->dy = -m.dy();
        spanData->txop = m.type();
        spanData->bilinear = bilinear;
        spanData->adjustSpanMethods();
    } else {
        spanData->setupMatrix(m, bilinear);
    }
}

// #define QT_CLIPPING_RATIOS

#ifdef QT_CLIPPING_RATIOS
int rectClips;
int regionClips;
int totalClips;

static void checkClipRatios(QRasterPaintEnginePrivate *d)
{
    if (d->clip()->hasRectClip)
        rectClips++;
    if (d->clip()->hasRegionClip)
        regionClips++;
    totalClips++;

    if ((totalClips % 5000) == 0) {
        printf("Clipping ratio: rectangular=%f%%, region=%f%%, complex=%f%%\n",
               rectClips * 100.0 / (qreal) totalClips,
               regionClips * 100.0 / (qreal) totalClips,
               (totalClips - rectClips - regionClips) * 100.0 / (qreal) totalClips);
        totalClips = 0;
        rectClips = 0;
        regionClips = 0;
    }

}
#endif

static inline void qrasterpaintengine_state_setNoClip(QRasterPaintEngineState *s)
{
    if (s->flags.has_clip_ownership)
        delete s->clip;
    s->clip = 0;
    s->flags.has_clip_ownership = false;
}

static void qrasterpaintengine_dirty_clip(QRasterPaintEnginePrivate *d, QRasterPaintEngineState *s)
{
    s->fillFlags |= QPaintEngine::DirtyClipPath;
    s->strokeFlags |= QPaintEngine::DirtyClipPath;

    d->solid_color_filler.clip = d->clip();
    d->solid_color_filler.adjustSpanMethods();

#ifdef QT_DEBUG_DRAW
    dumpClip(d->rasterBuffer->width(), d->rasterBuffer->height(), &*d->clip());
#endif

}


/*!
    \internal
*/
void QRasterPaintEngine::clip(const QVectorPath &path, Qt::ClipOperation op)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::clip(): " << path << op;

    if (path.elements()) {
        for (int i=0; i<path.elementCount(); ++i) {
            qDebug() << " - " << path.elements()[i]
                     << '(' << path.points()[i*2] << ", " << path.points()[i*2+1] << ')';
        }
    } else {
        for (int i=0; i<path.elementCount(); ++i) {
            qDebug() << " ---- "
                     << '(' << path.points()[i*2] << ", " << path.points()[i*2+1] << ')';
        }
    }
#endif

    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    const qreal *points = path.points();
    const QPainterPath::ElementType *types = path.elements();

    // There are some cases that are not supported by clip(QRect)
    if (op != Qt::UniteClip && (op != Qt::IntersectClip || !s->clip
                                || s->clip->hasRectClip || s->clip->hasRegionClip)) {
        if (s->matrix.type() <= QTransform::TxScale
            && ((path.shape() == QVectorPath::RectangleHint)
                || (isRect(points, path.elementCount())
                    && (!types || (types[0] == QPainterPath::MoveToElement
                                   && types[1] == QPainterPath::LineToElement
                                   && types[2] == QPainterPath::LineToElement
                                   && types[3] == QPainterPath::LineToElement))))) {
#ifdef QT_DEBUG_DRAW
            qDebug() << " --- optimizing vector clip to rect clip...";
#endif

            QRectF r(points[0], points[1], points[4]-points[0], points[5]-points[1]);
            if (setClipRectInDeviceCoords(s->matrix.mapRect(r).toRect(), op))
                return;
        }
    }

    if (op == Qt::NoClip) {
        qrasterpaintengine_state_setNoClip(s);

    } else {
        QClipData *base = d->baseClip.data();

        // Intersect with current clip when available...
        if (op == Qt::IntersectClip && s->clip)
            base = s->clip;

        // We always intersect, except when there is nothing to
        // intersect with, in which case we simplify the operation to
        // a replace...
        Qt::ClipOperation isectOp = Qt::IntersectClip;
        if (!base)
            isectOp = Qt::ReplaceClip;

        QClipData *newClip = new QClipData(d->rasterBuffer->height());
        newClip->initialize();
        ClipData clipData = { base, newClip, isectOp };
        updateOutlineMapper();
        d->rasterize(d->outlineMapper->convertPath(path), qt_span_clip, &clipData);

        newClip->fixup();

        if (op == Qt::UniteClip) {
            // merge clips
            QClipData *result = new QClipData(d->rasterBuffer->height());
            QClipData *current = s->clip ? s->clip : new QClipData(d->rasterBuffer->height());
            qt_merge_clip(current, newClip, result);
            result->fixup();
            delete newClip;
            if (!s->clip)
                delete current;
            newClip = result;
        }

        if (s->flags.has_clip_ownership)
            delete s->clip;

        s->clip = newClip;
        s->flags.has_clip_ownership = true;
    }
    qrasterpaintengine_dirty_clip(d, s);
}



/*!
    \internal
*/
void QRasterPaintEngine::clip(const QRect &rect, Qt::ClipOperation op)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::clip(): " << rect << op;
#endif

    QRasterPaintEngineState *s = state();

    if (op == Qt::NoClip) {
        qrasterpaintengine_state_setNoClip(s);

    } else if (op == Qt::UniteClip || s->matrix.type() > QTransform::TxScale
        || !setClipRectInDeviceCoords(s->matrix.mapRect(rect), op)) {
        QPaintEngineEx::clip(rect, op);
    }
}


bool QRasterPaintEngine::setClipRectInDeviceCoords(const QRect &r, Qt::ClipOperation op)
{
    Q_D(QRasterPaintEngine);
    const QRect clipRect = r & d->deviceRect;
    QRasterPaintEngineState *s = state();

    if (op == Qt::ReplaceClip || s->clip == 0) {

        // No current clip, hence we intersect with sysclip and be
        // done with it...
        QRegion clipRegion = systemClip();
        QClipData *clip = new QClipData(d->rasterBuffer->height());

        if (clipRegion.isEmpty())
            clip->setClipRect(clipRect);
        else
            clip->setClipRegion(clipRegion & clipRect);

        if (s->flags.has_clip_ownership)
            delete s->clip;

        s->clip = clip;
        s->clip->enabled = true;
        s->flags.has_clip_ownership = true;

    } else if (op == Qt::IntersectClip){ // intersect clip with current clip
        QClipData *base = s->clip;

        Q_ASSERT(base);
        if (base->hasRectClip || base->hasRegionClip) {
            if (!s->flags.has_clip_ownership) {
                s->clip = new QClipData(d->rasterBuffer->height());
                s->flags.has_clip_ownership = true;
            }
            if (base->hasRectClip)
                s->clip->setClipRect(base->clipRect & clipRect);
            else
                s->clip->setClipRegion(base->clipRegion & clipRect);
            s->clip->enabled = true;
        } else {
            return false;
        }
    } else {
        return false;
    }

    qrasterpaintengine_dirty_clip(d, s);
    return true;
}


/*!
    \internal
*/
void QRasterPaintEngine::clip(const QRegion &region, Qt::ClipOperation op)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::clip(): " << region << op;
#endif

    Q_D(QRasterPaintEngine);

    if (region.rectCount() == 1) {
        clip(region.boundingRect(), op);
        return;
    }

    QRasterPaintEngineState *s = state();
    const QClipData *clip = d->clip();
    const QClipData *baseClip = d->baseClip.data();

    if (op == Qt::NoClip) {
        qrasterpaintengine_state_setNoClip(s);
    } else if (s->matrix.type() > QTransform::TxScale
               || op == Qt::UniteClip
               || (op == Qt::IntersectClip && !clip->hasRectClip && !clip->hasRegionClip)
               || (op == Qt::ReplaceClip && !baseClip->hasRectClip && !baseClip->hasRegionClip)) {
        QPaintEngineEx::clip(region, op);
    } else {
        const QClipData *curClip;
        QClipData *newClip;

        if (op == Qt::IntersectClip)
            curClip = clip;
        else
            curClip = baseClip;

        if (s->flags.has_clip_ownership) {
            newClip = s->clip;
            Q_ASSERT(newClip);
        } else {
            newClip = new QClipData(d->rasterBuffer->height());
            s->clip = newClip;
            s->flags.has_clip_ownership = true;
        }

        QRegion r = s->matrix.map(region);
        if (curClip->hasRectClip)
            newClip->setClipRegion(r & curClip->clipRect);
        else if (curClip->hasRegionClip)
            newClip->setClipRegion(r & curClip->clipRegion);

        qrasterpaintengine_dirty_clip(d, s);
    }
}

/*!
    \internal
*/
void QRasterPaintEngine::fillPath(const QPainterPath &path, QSpanData *fillData)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " --- fillPath, bounds=" << path.boundingRect();
#endif

    if (!fillData->blend)
        return;

    Q_D(QRasterPaintEngine);

    const QRectF controlPointRect = path.controlPointRect();

    QRasterPaintEngineState *s = state();
    const QRect deviceRect = s->matrix.mapRect(controlPointRect).toRect();
    ProcessSpans blend = d->getBrushFunc(deviceRect, fillData);
    const bool do_clip = (deviceRect.left() < -RASTER_COORD_LIMIT
                          || deviceRect.right() > RASTER_COORD_LIMIT
                          || deviceRect.top() < -RASTER_COORD_LIMIT
                          || deviceRect.bottom() > RASTER_COORD_LIMIT);

    if (!s->flags.antialiased && !do_clip) {
        d->initializeRasterizer(fillData);
        d->rasterizer->rasterize(path * s->matrix, path.fillRule());
        return;
    }

    updateOutlineMapper();
    d->rasterize(d->outlineMapper->convertPath(path), blend, fillData);
}

static void fillRect_normalized(const QRect &r, QSpanData *data,
                                QRasterPaintEnginePrivate *pe)
{
    int x1, x2, y1, y2;

    bool rectClipped = true;

    if (data->clip) {
        x1 = qMax(r.x(), data->clip->xmin);
        x2 = qMin(r.x() + r.width(), data->clip->xmax);
        y1 = qMax(r.y(), data->clip->ymin);
        y2 = qMin(r.y() + r.height(), data->clip->ymax);
        rectClipped = data->clip->hasRectClip;

    } else if (pe) {
        x1 = qMax(r.x(), pe->deviceRect.x());
        x2 = qMin(r.x() + r.width(), pe->deviceRect.x() + pe->deviceRect.width());
        y1 = qMax(r.y(), pe->deviceRect.y());
        y2 = qMin(r.y() + r.height(), pe->deviceRect.y() + pe->deviceRect.height());
    } else {
        x1 = qMax(r.x(), 0);
        x2 = qMin(r.x() + r.width(), data->rasterBuffer->width());
        y1 = qMax(r.y(), 0);
        y2 = qMin(r.y() + r.height(), data->rasterBuffer->height());
    }

    if (x2 <= x1 || y2 <= y1)
        return;

    const int width = x2 - x1;
    const int height = y2 - y1;

    bool isUnclipped = rectClipped
                       || (pe && pe->isUnclipped_normalized(QRect(x1, y1, width, height)));

    if (pe && isUnclipped) {
        const QPainter::CompositionMode mode = pe->rasterBuffer->compositionMode;

        if (data->fillRect && (mode == QPainter::CompositionMode_Source
                               || (mode == QPainter::CompositionMode_SourceOver
                                   && qAlpha(data->solid.color) == 255)))
        {
            data->fillRect(data->rasterBuffer, x1, y1, width, height,
                           data->solid.color);
            return;
        }
    }

    ProcessSpans blend = isUnclipped ? data->unclipped_blend : data->blend;

    const int nspans = 256;
    QSTACKARRAY(QT_FT_Span, spans, nspans);

    Q_ASSERT(data->blend);
    int y = y1;
    while (y < y2) {
        int n = qMin(nspans, y2 - y);
        int i = 0;
        while (i < n) {
            spans[i].x = x1;
            spans[i].len = width;
            spans[i].y = y + i;
            spans[i].coverage = 255;
            ++i;
        }

        blend(n, spans, data);
        y += n;
    }
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawRects(const QRect *rects, int rectCount)
{
#ifdef QT_DEBUG_DRAW
    qDebug(" - QRasterPaintEngine::drawRect(), rectCount=%d", rectCount);
#endif
    Q_D(QRasterPaintEngine);
    ensureRasterState();
    QRasterPaintEngineState *s = state();

    // Fill
    ensureBrush();
    if (s->brushData.blend) {
        if (!s->flags.antialiased && s->matrix.type() <= QTransform::TxTranslate) {
            const QRect *r = rects;
            const QRect *lastRect = rects + rectCount;

            int offset_x = int(s->matrix.dx());
            int offset_y = int(s->matrix.dy());
            while (r < lastRect) {
                QRect rect = r->normalized();
                fillRect_normalized(rect.translated(offset_x, offset_y), &s->brushData, d);
                ++r;
            }
        } else {
            QRectVectorPath path;
            for (int i=0; i<rectCount; ++i) {
                path.set(rects[i]);
                fill(path, s->brush);
            }
        }
    }

    ensurePen();
    if (s->penData.blend) {
        QRectVectorPath path;
        for (int i = 0; i < rectCount; ++i) {
            path.set(rects[i]);
            stroke(path, s->pen);
        }
    }
}


/*!
    \internal
*/
void QRasterPaintEngine::stroke(const QVectorPath &path, const QPen &pen)
{
    QRasterPaintEngineState *s = state();

    ensurePen(pen);
    if (!s->penData.blend)
        return;

    if (s->flags.non_complex_pen && path.shape() == QVectorPath::LinesHint) {
        const qreal lastwidthf = s->lastPen.widthF();
        const qreal width = s->lastPen.isCosmetic()
                            ? (lastwidthf == 0 ? 1 : lastwidthf)
                            : lastwidthf * s->txscale;
        int dashIndex = 0;
        qreal dashOffset = s->lastPen.dashOffset();
        bool inDash = true;
        qreal patternLength = 0;
        const QVector<qreal> pattern = s->lastPen.dashPattern();
        for (int i = 0; i < pattern.size(); ++i)
            patternLength += pattern.at(i);

        if (patternLength > 0) {
            int n = qFloor(dashOffset / patternLength);
            dashOffset -= n * patternLength;
            while (dashOffset >= pattern.at(dashIndex)) {
                dashOffset -= pattern.at(dashIndex);
                if (++dashIndex >= pattern.size())
                    dashIndex = 0;
                inDash = !inDash;
            }
        }

        Q_D(QRasterPaintEngine);
        d->initializeRasterizer(&s->penData);
        int lineCount = path.elementCount() / 2;
        const QLineF *lines = reinterpret_cast<const QLineF *>(path.points());

        for (int i = 0; i < lineCount; ++i) {
            if (lines[i].p1() == lines[i].p2()) {
                if (s->lastPen.capStyle() != Qt::FlatCap) {
                    QPointF p = lines[i].p1();
                    QLineF line = s->matrix.map(QLineF(QPointF(p.x() - width*qreal(0.5), p.y()),
                                                       QPointF(p.x() + width*qreal(0.5), p.y())));
                    d->rasterizer->rasterizeLine(line.p1(), line.p2(), 1);
                }
                continue;
            }

            const QLineF line = s->matrix.map(lines[i]);
            if (s->lastPen.style() == Qt::SolidLine) {
                d->rasterizer->rasterizeLine(line.p1(), line.p2(),
                                            width / line.length(),
                                            s->lastPen.capStyle() == Qt::SquareCap);
            } else {
                d->rasterizeLine_dashed(line, width,
                                        &dashIndex, &dashOffset, &inDash);
            }
        }
    } else {
        QPaintEngineEx::stroke(path, pen);
    }
}

static inline QRect toNormalizedFillRect(const QRectF &rect)
{
    int x1 = qRound(rect.x() + aliasedCoordinateDelta);
    int y1 = qRound(rect.y() + aliasedCoordinateDelta);
    int x2 = qRound(rect.right() + aliasedCoordinateDelta);
    int y2 = qRound(rect.bottom() + aliasedCoordinateDelta);

    if (x2 < x1)
        qSwap(x1, x2);
    if (y2 < y1)
        qSwap(y1, y2);

    return QRect(x1, y1, x2 - x1, y2 - y1);
}

/*!
    \internal
*/
void QRasterPaintEngine::fill(const QVectorPath &path, const QBrush &brush)
{
    if (path.isEmpty())
        return;
#ifdef QT_DEBUG_DRAW
    QRectF rf = path.controlPointRect();
    qDebug() << "QRasterPaintEngine::fill(): "
             << "size=" << path.elementCount()
             << ", hints=" << hex << path.hints()
             << rf << brush;
#endif

    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    ensureBrush(brush);
    if (!s->brushData.blend)
        return;

    if (path.shape() == QVectorPath::RectangleHint) {
        if (!s->flags.antialiased && s->matrix.type() <= QTransform::TxScale) {
            const qreal *p = path.points();
            QPointF tl = QPointF(p[0], p[1]) * s->matrix;
            QPointF br = QPointF(p[4], p[5]) * s->matrix;
            fillRect_normalized(toNormalizedFillRect(QRectF(tl, br)), &s->brushData, d);
            return;
        }
        ensureRasterState();
        if (s->flags.tx_noshear) {
            d->initializeRasterizer(&s->brushData);
            // ### Is normalizing really necessary here?
            const qreal *p = path.points();
            QRectF r = QRectF(p[0], p[1], p[2] - p[0], p[7] - p[1]).normalized();
            if (!r.isEmpty()) {
                const QPointF a = s->matrix.map((r.topLeft() + r.bottomLeft()) * 0.5f);
                const QPointF b = s->matrix.map((r.topRight() + r.bottomRight()) * 0.5f);
                d->rasterizer->rasterizeLine(a, b, r.height() / r.width());
            }
            return;
        }
    }

    // ### Optimize for non transformed ellipses and rectangles...
    QRectF cpRect = path.controlPointRect();
    const QRect deviceRect = s->matrix.mapRect(cpRect).toRect();
    ProcessSpans blend = d->getBrushFunc(deviceRect, &s->brushData);

        // ### Falcon
//         const bool do_clip = (deviceRect.left() < -RASTER_COORD_LIMIT
//                               || deviceRect.right() > RASTER_COORD_LIMIT
//                               || deviceRect.top() < -RASTER_COORD_LIMIT
//                               || deviceRect.bottom() > RASTER_COORD_LIMIT);

        // ### Falonc: implement....
//         if (!s->flags.antialiased && !do_clip) {
//             d->initializeRasterizer(&s->brushData);
//             d->rasterizer->rasterize(path * d->matrix, path.fillRule());
//             return;
//         }

    updateOutlineMapper();
    d->rasterize(d->outlineMapper->convertPath(path), blend, &s->brushData);
}

void QRasterPaintEngine::fillRect(const QRectF &r, QSpanData *data)
{
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    if (!s->flags.antialiased) {
        QTransform::TransformationType txop = s->matrix.type();
        if (txop == QTransform::TxNone) {
            fillRect_normalized(toNormalizedFillRect(r), data, d);
            return;
        } else if (txop == QTransform::TxTranslate) {
            const QRect rr = toNormalizedFillRect(r.translated(s->matrix.dx(), s->matrix.dy()));
            fillRect_normalized(rr, data, d);
            return;
        } else if (txop == QTransform::TxScale) {
            const QRect rr = toNormalizedFillRect(s->matrix.mapRect(r));
            fillRect_normalized(rr, data, d);
            return;
        }
    }
    ensureRasterState();
    if (s->flags.tx_noshear) {
        d->initializeRasterizer(data);
        QRectF nr = r.normalized();
        if (!nr.isEmpty()) {
            const QPointF a = s->matrix.map((nr.topLeft() + nr.bottomLeft()) * 0.5f);
            const QPointF b = s->matrix.map((nr.topRight() + nr.bottomRight()) * 0.5f);
            d->rasterizer->rasterizeLine(a, b, nr.height() / nr.width());
        }
        return;
    }

    QPainterPath path;
    path.addRect(r);
    updateOutlineMapper();
    fillPath(path, data);
}

/*!
    \reimp
*/
void QRasterPaintEngine::fillRect(const QRectF &r, const QBrush &brush)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::fillRecct(): " << r << brush;
#endif
    QRasterPaintEngineState *s = state();

    ensureBrush(brush);
    if (!s->brushData.blend)
        return;

    fillRect(r, &s->brushData);
}

/*!
    \reimp
*/
void QRasterPaintEngine::fillRect(const QRectF &r, const QColor &color)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << "QRasterPaintEngine::fillRect(): " << r << color;
#endif
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    d->solid_color_filler.solid.color = PREMUL(ARGB_COMBINE_ALPHA(color.rgba(), s->intOpacity));
    if ((d->solid_color_filler.solid.color & 0xff000000) == 0
        && s->composition_mode == QPainter::CompositionMode_SourceOver) {
        return;
    }
    d->solid_color_filler.clip = d->clip();
    d->solid_color_filler.adjustSpanMethods();
    fillRect(r, &d->solid_color_filler);
}

/*!
  \internal
 */
void QRasterPaintEngine::fillPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    // max amount of points that raster engine can reliably handle
    const int maxPoints = 0xffff;
    if (Q_UNLIKELY(pointCount > maxPoints)) {
        qWarning("Polygon too complex for filling.");
        return;
    }

    // Compose polygon fill..,
    QVectorPath vp((qreal *) points, pointCount, 0, QVectorPath::polygonFlags(mode));
    updateOutlineMapper();
    QT_FT_Outline *outline = d->outlineMapper->convertPath(vp);

    // scanconvert.
    ProcessSpans brushBlend = d->getBrushFunc(d->outlineMapper->controlPointRect,
                                              &s->brushData);
    d->rasterize(outline, brushBlend, &s->brushData);
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode)
{
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    qDebug(" - QRasterPaintEngine::drawPolygon(F), pointCount=%d", pointCount);
    for (int i=0; i<pointCount; ++i)
        qDebug() << "   - " << points[i];
#endif
    Q_ASSERT(pointCount >= 2);

    if (mode != PolylineMode && isRect((qreal *) points, pointCount)) {
        QRectF r(points[0], points[2]);
        QPaintEngineEx::drawRects(&r, 1);
        return;
    }

    ensurePen();
    if (mode != PolylineMode) {
        // Do the fill...
        ensureBrush();
        if (s->brushData.blend)
            fillPolygon(points, pointCount, mode);
    }

    // Do the outline...
    if (s->penData.blend) {
        QVectorPath vp((qreal *) points, pointCount, 0, QVectorPath::polygonFlags(mode));
        QPaintEngineEx::stroke(vp, s->lastPen);
    }
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
{
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    qDebug(" - QRasterPaintEngine::drawPolygon(I), pointCount=%d", pointCount);
    for (int i=0; i<pointCount; ++i)
        qDebug() << "   - " << points[i];
#endif
    Q_ASSERT(pointCount >= 2);
    if (mode != PolylineMode && isRect((int *) points, pointCount)) {
        QRect r(points[0].x(),
                points[0].y(),
                points[2].x() - points[0].x(),
                points[2].y() - points[0].y());
        drawRects(&r, 1);
        return;
    }

    ensurePen();

    // Do the fill
    if (mode != PolylineMode) {
        ensureBrush();
        if (s->brushData.blend) {
            // Compose polygon fill..,
            updateOutlineMapper();
            d->outlineMapper->beginOutline(mode == WindingMode ? Qt::WindingFill : Qt::OddEvenFill);
            d->outlineMapper->moveTo(*points);
            const QPoint *p = points;
            const QPoint *ep = points + pointCount - 1;
            do {
                d->outlineMapper->lineTo(*(++p));
            } while (p < ep);
            d->outlineMapper->endOutline();

            // scanconvert.
            ProcessSpans brushBlend = d->getBrushFunc(d->outlineMapper->controlPointRect,
                                                      &s->brushData);
            d->rasterize(d->outlineMapper->outline(), brushBlend, &s->brushData);
        }
    }

    // Do the outline...
    if (s->penData.blend) {
        const int count = pointCount * 2;
        QSTACKARRAY(qreal, fpoints, count);
        for (int i=0; i<count; ++i)
            fpoints[i] = ((int *) points)[i];
        QVectorPath vp((qreal *) fpoints, pointCount, 0, QVectorPath::polygonFlags(mode));

        QPaintEngineEx::stroke(vp, s->lastPen);
    }
}

/*!
    \internal
*/
void QRasterPaintEngine::drawPixmap(const QPointF &pos, const QPixmap &pixmap)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QRasterPaintEngine::drawPixmap(), pos=" << pos << " pixmap=" << pixmap.size() << "depth=" << pixmap.depth();
#endif

    QPixmapData *pd = pixmap.pixmapData();
    if (pd->classId() == QPixmapData::RasterClass) {
        const QImage &image = static_cast<QRasterPixmapData *>(pd)->image;
        if (image.depth() == 1) {
            Q_D(QRasterPaintEngine);
            QRasterPaintEngineState *s = state();
            if (s->matrix.type() <= QTransform::TxTranslate) {
                ensurePen();
                drawBitmap(pos + QPointF(s->matrix.dx(), s->matrix.dy()), image, &s->penData);
            } else {
                drawImage(pos, d->rasterBuffer->colorizeBitmap(image, s->pen.color()));
            }
        } else {
            QRasterPaintEngine::drawImage(pos, image);
        }
    } else {
        const QImage image = pixmap.toImage();
        if (pixmap.depth() == 1) {
            Q_D(QRasterPaintEngine);
            QRasterPaintEngineState *s = state();
            if (s->matrix.type() <= QTransform::TxTranslate) {
                ensurePen();
                drawBitmap(pos + QPointF(s->matrix.dx(), s->matrix.dy()), image, &s->penData);
            } else {
                drawImage(pos, d->rasterBuffer->colorizeBitmap(image, s->pen.color()));
            }
        } else {
            QRasterPaintEngine::drawImage(pos, image);
        }
    }
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pixmap, const QRectF &sr)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QRasterPaintEngine::drawPixmap(), r=" << r << " sr=" << sr << " pixmap=" << pixmap.size() << "depth=" << pixmap.depth();
#endif

    QPixmapData* pd = pixmap.pixmapData();
    if (pd->classId() == QPixmapData::RasterClass) {
        const QImage &image = static_cast<QRasterPixmapData *>(pd)->image;
        if (image.depth() == 1) {
            Q_D(QRasterPaintEngine);
            QRasterPaintEngineState *s = state();
            if (s->matrix.type() <= QTransform::TxTranslate
                && r.size() == sr.size()
                && r.size() == pixmap.size()) {
                ensurePen();
                drawBitmap(r.topLeft() + QPointF(s->matrix.dx(), s->matrix.dy()), image, &s->penData);
                return;
            } else {
                drawImage(r, d->rasterBuffer->colorizeBitmap(image, s->pen.color()), sr);
            }
        } else {
            drawImage(r, image, sr);
        }
    } else {
        QRect clippedSource = sr.toAlignedRect().intersected(pixmap.rect());
        const QImage image = pd->toImage(clippedSource);
        QRectF translatedSource = sr.translated(-clippedSource.topLeft());
        if (image.depth() == 1) {
            Q_D(QRasterPaintEngine);
            QRasterPaintEngineState *s = state();
            if (s->matrix.type() <= QTransform::TxTranslate
                && r.size() == sr.size()
                && r.size() == pixmap.size()) {
                ensurePen();
                drawBitmap(r.topLeft() + QPointF(s->matrix.dx(), s->matrix.dy()), image, &s->penData);
                return;
            } else {
                drawImage(r, d->rasterBuffer->colorizeBitmap(image, s->pen.color()), translatedSource);
            }
        } else {
            drawImage(r, image, translatedSource);
        }
    }
}

static inline int fast_ceil_positive(const qreal &v)
{
    const int iv = int(v);
    if (v - iv == 0)
        return iv;
    else
        return iv + 1;
}

static inline const QRect toAlignedRect_positive(const QRectF &rect)
{
    const int xmin = int(rect.x());
    const int xmax = int(fast_ceil_positive(rect.right()));
    const int ymin = int(rect.y());
    const int ymax = int(fast_ceil_positive(rect.bottom()));
    return QRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

/*!
    \internal
*/
void QRasterPaintEngine::drawImage(const QPointF &p, const QImage &img)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QRasterPaintEngine::drawImage(), p=" <<  p << " image=" << img.size() << "depth=" << img.depth();
#endif

    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    if (s->matrix.type() > QTransform::TxTranslate) {
        drawImage(QRectF(p.x(), p.y(), img.width(), img.height()),
                  img,
                  QRectF(0, 0, img.width(), img.height()));
    } else {

        const QClipData *clip = d->clip();
        QPointF pt(p.x() + s->matrix.dx(), p.y() + s->matrix.dy());

        d->image_filler.clip = clip;
        d->image_filler.initTexture(&img, s->intOpacity, QTextureData::Plain, img.rect());
        if (!d->image_filler.blend)
            return;
        d->image_filler.dx = -pt.x();
        d->image_filler.dy = -pt.y();
        QRect rr = img.rect().translated(qRound(pt.x()), qRound(pt.y()));

        fillRect_normalized(rr, &d->image_filler, d);
    }

}

/*!
    \reimp
*/
void QRasterPaintEngine::drawImage(const QRectF &r, const QImage &img, const QRectF &sr,
                                   Qt::ImageConversionFlags)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QRasterPaintEngine::drawImage(), r=" << r << " sr=" << sr << " image=" << img.size() << "depth=" << img.depth();
#endif

    if (r.isEmpty())
        return;

    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();
    int sr_l = qFloor(sr.left());
    int sr_r = qCeil(sr.right()) - 1;
    int sr_t = qFloor(sr.top());
    int sr_b = qCeil(sr.bottom()) - 1;

    if (s->matrix.type() <= QTransform::TxScale && !s->flags.antialiased && sr_l == sr_r && sr_t == sr_b) {
        // as fillRect will apply the aliased coordinate delta we need to
        // subtract it here as we don't use it for image drawing
        QTransform old = s->matrix;
        s->matrix = s->matrix * QTransform::fromTranslate(-aliasedCoordinateDelta, -aliasedCoordinateDelta);

        // Do whatever fillRect() does, but without premultiplying the color if it's already premultiplied.
        QRgb color = img.pixel(sr_l, sr_t);
        switch (img.format()) {
        case QImage::Format_ARGB32_Premultiplied:
            // Combine premultiplied color with the opacity set on the painter.
            d->solid_color_filler.solid.color =
                ((((color & 0x00ff00ff) * s->intOpacity) >> 8) & 0x00ff00ff)
                | ((((color & 0xff00ff00) >> 8) * s->intOpacity) & 0xff00ff00);
            break;
        default:
            d->solid_color_filler.solid.color = PREMUL(ARGB_COMBINE_ALPHA(color, s->intOpacity));
            break;
        }

        if ((d->solid_color_filler.solid.color & 0xff000000) == 0
            && s->composition_mode == QPainter::CompositionMode_SourceOver) {
            return;
        }

        d->solid_color_filler.clip = d->clip();
        d->solid_color_filler.adjustSpanMethods();
        fillRect(r, &d->solid_color_filler);

        s->matrix = old;
        return;
    }

    bool stretch_sr = r.width() != sr.width() || r.height() != sr.height();

    const QClipData *clip = d->clip();

    if (s->matrix.type() > QTransform::TxTranslate || stretch_sr) {
        QTransform copy = s->matrix;
        copy.translate(r.x(), r.y());
        if (stretch_sr)
            copy.scale(r.width() / sr.width(), r.height() / sr.height());
        copy.translate(-sr.x(), -sr.y());

        d->image_filler_xform.clip = clip;
        d->image_filler_xform.initTexture(&img, s->intOpacity, QTextureData::Plain, toAlignedRect_positive(sr));
        if (!d->image_filler_xform.blend)
            return;
        d->image_filler_xform.setupMatrix(copy, s->flags.bilinear);

        if (!s->flags.antialiased && s->matrix.type() == QTransform::TxScale) {
            QPointF rr_tl = s->matrix.map(r.topLeft());
            QPointF rr_br = s->matrix.map(r.bottomRight());

            int x1 = qRound(rr_tl.x());
            int y1 = qRound(rr_tl.y());
            int x2 = qRound(rr_br.x());
            int y2 = qRound(rr_br.y());

            if (x1 > x2)
                qSwap(x1, x2);
            if (y1 > y2)
                qSwap(y1, y2);

            fillRect_normalized(QRect(x1, y1, x2-x1, y2-y1), &d->image_filler_xform, d);
            return;
        }

        const qreal offs = s->flags.antialiased ? qreal(0) : aliasedCoordinateDelta;
        QPainterPath path;
        path.addRect(r);
        QTransform m = s->matrix;
        s->matrix = QTransform(m.m11(), m.m12(), m.m13(),
                               m.m21(), m.m22(), m.m23(),
                               m.m31() - offs, m.m32() - offs, m.m33());
        fillPath(path, &d->image_filler_xform);
        s->matrix = m;
    } else {
        d->image_filler.clip = clip;
        d->image_filler.initTexture(&img, s->intOpacity, QTextureData::Plain, toAlignedRect_positive(sr));
        if (!d->image_filler.blend)
            return;
        d->image_filler.dx = -(r.x() + s->matrix.dx()) + sr.x();
        d->image_filler.dy = -(r.y() + s->matrix.dy()) + sr.y();

        QRectF rr = r.translated(s->matrix.dx(), s->matrix.dy());

        const int x1 = qRound(rr.x());
        const int y1 = qRound(rr.y());
        const int x2 = qRound(rr.right());
        const int y2 = qRound(rr.bottom());

        fillRect_normalized(QRect(x1, y1, x2-x1, y2-y1), &d->image_filler, d);
    }
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &sr)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QRasterPaintEngine::drawTiledPixmap(), r=" << r << "pixmap=" << pixmap.size();
#endif
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    QImage image;

    QPixmapData *pd = pixmap.pixmapData();
    if (pd->classId() == QPixmapData::RasterClass) {
        image = static_cast<QRasterPixmapData *>(pd)->image;
    } else {
        image = pixmap.toImage();
    }

    if (image.depth() == 1)
        image = d->rasterBuffer->colorizeBitmap(image, s->pen.color());

    if (s->matrix.type() > QTransform::TxTranslate) {
        QTransform copy = s->matrix;
        copy.translate(r.x(), r.y());
        copy.translate(-sr.x(), -sr.y());
        d->image_filler_xform.clip = d->clip();
        d->image_filler_xform.initTexture(&image, s->intOpacity, QTextureData::Tiled);
        if (!d->image_filler_xform.blend)
            return;
        d->image_filler_xform.setupMatrix(copy, s->flags.bilinear);

        QPainterPath path;
        path.addRect(r);
        fillPath(path, &d->image_filler_xform);
    } else {
        d->image_filler.clip = d->clip();

        d->image_filler.initTexture(&image, s->intOpacity, QTextureData::Tiled);
        if (!d->image_filler.blend)
            return;
        d->image_filler.dx = -(r.x() + s->matrix.dx()) + sr.x();
        d->image_filler.dy = -(r.y() + s->matrix.dy()) + sr.y();

        QRectF rr = r.translated(s->matrix.dx(), s->matrix.dy());
        fillRect_normalized(rr.toRect().normalized(), &d->image_filler, d);
    }
}


//QWS hack
static inline bool monoVal(const uchar* s, int x)
{
    return  (s[x>>3] << (x&7)) & 0x80;
}

/*!
    \internal
*/
void QRasterPaintEngine::alphaPenBlt(const void* src, const int bpl, const int depth,
                                    int rx, int ry, int w, int h)
{
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();

    if (!s->penData.blend)
        return;

    QRasterBuffer *rb = d->rasterBuffer.data();

    const QRect rect(rx, ry, w, h);
    const QClipData *clip = d->clip();
    bool unclipped = false;
    if (clip) {
        // inlined QRect::intersects
        const bool intersects = qMax(clip->xmin, rect.left()) <= qMin(clip->xmax - 1, rect.right())
                                && qMax(clip->ymin, rect.top()) <= qMin(clip->ymax - 1, rect.bottom());

        if (clip->hasRectClip) {
            unclipped = rx > clip->xmin
                        && rx + w < clip->xmax
                        && ry > clip->ymin
                        && ry + h < clip->ymax;
        }

        if (!intersects)
            return;
    } else {
        // inlined QRect::intersects
        const bool intersects = qMax(0, rect.left()) <= qMin(rb->width() - 1, rect.right())
                                && qMax(0, rect.top()) <= qMin(rb->height() - 1, rect.bottom());
        if (!intersects)
            return;

        // inlined QRect::contains
        const bool contains = rect.left() >= 0 && rect.right() < rb->width()
                              && rect.top() >= 0 && rect.bottom() < rb->height();

        unclipped = contains && d->isUnclipped_normalized(rect);
    }

    ProcessSpans blend = unclipped ? s->penData.unclipped_blend : s->penData.blend;
    const uchar * scanline = static_cast<const uchar *>(src);

    if (s->flags.fast_text) {
        if (unclipped) {
            if (depth == 1) {
                if (s->penData.bitmapBlit) {
                    s->penData.bitmapBlit(rb, rx, ry, s->penData.solid.color,
                                          scanline, w, h, bpl);
                    return;
                }
            } else if (depth == 8) {
                if (s->penData.alphamapBlit) {
                    s->penData.alphamapBlit(rb, rx, ry, s->penData.solid.color,
                                            scanline, w, h, bpl, 0);
                    return;
                }
            } else if (depth == 32) {
                // (A)RGB Alpha mask where the alpha component is not used.
                if (s->penData.alphaRGBBlit) {
                    s->penData.alphaRGBBlit(rb, rx, ry, s->penData.solid.color,
                                            (const uint *) scanline, w, h, bpl / 4, 0);
                    return;
                }
            }
        } else if (d->deviceDepth == 32 && (depth == 8 || depth == 32)) {
            // (A)RGB Alpha mask where the alpha component is not used.
            if (!clip) {
                int nx = qMax(0, rx);
                int ny = qMax(0, ry);

                // Move scanline pointer to compensate for moved x and y
                int xdiff = nx - rx;
                int ydiff = ny - ry;
                scanline += ydiff * bpl;
                scanline += xdiff * (depth == 32 ? 4 : 1);

                w -= xdiff;
                h -= ydiff;

                if (nx + w > d->rasterBuffer->width())
                    w = d->rasterBuffer->width() - nx;
                if (ny + h > d->rasterBuffer->height())
                    h = d->rasterBuffer->height() - ny;

                rx = nx;
                ry = ny;
            }
            if (depth == 8 && s->penData.alphamapBlit) {
                s->penData.alphamapBlit(rb, rx, ry, s->penData.solid.color,
                                        scanline, w, h, bpl, clip);
            } else if (depth == 32 && s->penData.alphaRGBBlit) {
                s->penData.alphaRGBBlit(rb, rx, ry, s->penData.solid.color,
                                        (const uint *) scanline, w, h, bpl / 4, clip);
            }
            return;
        }
    }

    int x0 = 0;
    if (rx < 0) {
        x0 = -rx;
        w -= x0;
    }

    int y0 = 0;
    if (ry < 0) {
        y0 = -ry;
        scanline += bpl * y0;
        h -= y0;
    }

    w = qMin(w, rb->width() - qMax(0, rx));
    h = qMin(h, rb->height() - qMax(0, ry));

    if (w <= 0 || h <= 0)
        return;

    const int NSPANS = 256;
    QSpan spans[NSPANS];
    int current = 0;

    const int x1 = x0 + w;
    const int y1 = y0 + h;

    if (depth == 1) {
        for (int y = y0; y < y1; ++y) {
            for (int x = x0; x < x1; ) {
                if (!monoVal(scanline, x)) {
                    ++x;
                    continue;
                }

                if (current == NSPANS) {
                    blend(current, spans, &s->penData);
                    current = 0;
                }
                spans[current].x = x + rx;
                spans[current].y = y + ry;
                spans[current].coverage = 255;
                int len = 1;
                ++x;
                // extend span until we find a different one.
                while (x < x1 && monoVal(scanline, x)) {
                    ++x;
                    ++len;
                }
                spans[current].len = len;
                ++current;
            }
            scanline += bpl;
        }
    } else if (depth == 8) {
        for (int y = y0; y < y1; ++y) {
            for (int x = x0; x < x1; ) {
                // Skip those with 0 coverage
                if (scanline[x] == 0) {
                    ++x;
                    continue;
                }

                if (current == NSPANS) {
                    blend(current, spans, &s->penData);
                    current = 0;
                }
                int coverage = scanline[x];
                spans[current].x = x + rx;
                spans[current].y = y + ry;
                spans[current].coverage = coverage;
                int len = 1;
                ++x;

                // extend span until we find a different one.
                while (x < x1 && scanline[x] == coverage) {
                    ++x;
                    ++len;
                }
                spans[current].len = len;
                ++current;
            }
            scanline += bpl;
        }
    } else { // 32-bit alpha...
        uint *sl = (uint *) src;
        for (int y = y0; y < y1; ++y) {
            for (int x = x0; x < x1; ) {
                // Skip those with 0 coverage
                if ((sl[x] & 0x00ffffff) == 0) {
                    ++x;
                    continue;
                }

                if (current == NSPANS) {
                    blend(current, spans, &s->penData);
                    current = 0;
                }
                uint rgbCoverage = sl[x];
                int coverage = qGreen(rgbCoverage);
                spans[current].x = x + rx;
                spans[current].y = y + ry;
                spans[current].coverage = coverage;
                int len = 1;
                ++x;

                // extend span until we find a different one.
                while (x < x1 && sl[x] == rgbCoverage) {
                    ++x;
                    ++len;
                }
                spans[current].len = len;
                ++current;
            }
            sl += bpl / sizeof(uint);
        }
    }
//     qDebug() << "alphaPenBlt: num spans=" << current
//              << "span:" << spans->x << spans->y << spans->len << spans->coverage;
        // Call span func for current set of spans.
    if (current != 0)
        blend(current, spans, &s->penData);
}

bool QRasterPaintEngine::drawCachedGlyphs(int numGlyphs, const glyph_t *glyphs,
                                          const QFixedPoint *positions, QFontEngine *fontEngine)
{
    Q_D(QRasterPaintEngine);
    QRasterPaintEngineState *s = state();
    const QFixed offs = QFixed::fromReal(aliasedCoordinateDelta);

    if (fontEngine->type() == QFontEngine::Freetype) {
        QFontEngineFT *fe = static_cast<QFontEngineFT *>(fontEngine);
        const QFixed xOffs = fe->supportsSubPixelPositions() ? 0 : offs;
        QFontEngineFT::GlyphFormat neededFormat =
            painter()->device()->devType() == QInternal::Widget
            ? fe->defaultGlyphFormat()
            : QFontEngineFT::Format_A8;

        if (d_func()->mono_surface
            || fe->isBitmapFont() // alphaPenBlt can handle mono, too
            )
            neededFormat = QFontEngineFT::Format_Mono;

        if (neededFormat == QFontEngineFT::Format_None)
            neededFormat = QFontEngineFT::Format_A8;

        QFontEngineFT::QGlyphSet *gset = fe->defaultGlyphs();
        if (s->matrix.type() >= QTransform::TxScale) {
            if (s->matrix.isAffine())
                gset = fe->loadTransformedGlyphSet(s->matrix);
            else
                gset = 0;
        }

        if (!gset || gset->outline_drawing
            || !fe->loadGlyphs(gset, glyphs, numGlyphs, positions, neededFormat))
            return false;

        FT_Face lockedFace = 0;

        int depth;
        switch (neededFormat) {
        case QFontEngineFT::Format_Mono:
            depth = 1;
            break;
        case QFontEngineFT::Format_A8:
            depth = 8;
            break;
        case QFontEngineFT::Format_A32:
            depth = 32;
            break;
        default:
            Q_ASSERT(false);
            depth = 0;
        };

        for (int i = 0; i < numGlyphs; i++) {
            QFixed spp = fe->subPixelPositionForX(positions[i].x);
            QFontEngineFT::Glyph *glyph = gset->getGlyph(glyphs[i], spp);

            if (!glyph || glyph->format != neededFormat) {
                if (!lockedFace)
                    lockedFace = fe->lockFace();
                glyph = fe->loadGlyph(gset, glyphs[i], spp, neededFormat);
            }

            if (!glyph || !glyph->data)
                continue;

            int pitch;
            switch (neededFormat) {
            case QFontEngineFT::Format_Mono:
                pitch = ((glyph->width + 31) & ~31) >> 3;
                break;
            case QFontEngineFT::Format_A8:
                pitch = (glyph->width + 3) & ~3;
                break;
            case QFontEngineFT::Format_A32:
                pitch = glyph->width * 4;
                break;
            default:
                Q_ASSERT(false);
                pitch = 0;
            };

            alphaPenBlt(glyph->data, pitch, depth,
                        qFloor(positions[i].x + xOffs) + glyph->x,
                        qFloor(positions[i].y + offs) - glyph->y,
                        glyph->width, glyph->height);
        }
        if (lockedFace)
            fe->unlockFace();
    } else {
        QFontEngineGlyphCache::Type glyphType = fontEngine->glyphFormat >= 0
                ? QFontEngineGlyphCache::Type(fontEngine->glyphFormat)
                : d->glyphCacheType;

        QImageTextureGlyphCache *cache =
            static_cast<QImageTextureGlyphCache *>(fontEngine->glyphCache(0, glyphType, s->matrix));
        if (!cache) {
            cache = new QImageTextureGlyphCache(glyphType, s->matrix);
            fontEngine->setGlyphCache(0, cache);
        }

        cache->populate(fontEngine, numGlyphs, glyphs, positions);
        cache->fillInPendingGlyphs();

        const QImage &image = cache->image();
        int bpl = image.bytesPerLine();

        int depth = image.depth();
        int rightShift = 0;
        int leftShift = 0;
        if (depth == 32)
            leftShift = 2; // multiply by 4
        else if (depth == 1)
            rightShift = 3; // divide by 8

        const uchar *bits = image.constBits();
        for (int i=0; i<numGlyphs; ++i) {

            QFixed subPixelPosition = cache->subPixelPositionForX(positions[i].x);
            QTextureGlyphCache::GlyphAndSubPixelPosition glyph(glyphs[i], subPixelPosition);
            const QTextureGlyphCache::Coord &c = cache->coords[glyph];
            if (c.isNull())
                continue;

            int x = qFloor(positions[i].x) + c.baseLineX ;
            int y = qFloor(positions[i].y + offs) - c.baseLineY;

            // printf("drawing [%d %d %d %d] baseline [%d %d], glyph: %d, to: %d %d, pos: %d %d\n",
            //        c.x, c.y,
            //        c.w, c.h,
            //        c.baseLineX, c.baseLineY,
            //        glyphs[i],
            //        x, y,
            //        positions[i].x.toInt(), positions[i].y.toInt());

            alphaPenBlt(bits + ((c.x << leftShift) >> rightShift) + c.y * bpl, bpl, depth, x, y, c.w, c.h);
        }
    }
    return true;
}


/*!
 * Returns true if the rectangle is completely within the current clip
 * state of the paint engine.
 */
bool QRasterPaintEnginePrivate::isUnclipped_normalized(const QRect &r) const
{
    const QClipData *cl = clip();
    if (!cl) {
        // inline contains() for performance (we know the rects are normalized)
        const QRect &r1 = deviceRect;
        return (r.left() >= r1.left() && r.right() <= r1.right()
                && r.top() >= r1.top() && r.bottom() <= r1.bottom());
    }


    if (cl->hasRectClip) {
        // currently all painting functions clips to deviceRect internally
        if (cl->clipRect == deviceRect)
            return true;

        // inline contains() for performance (we know the rects are normalized)
        const QRect &r1 = cl->clipRect;
        return (r.left() >= r1.left() && r.right() <= r1.right()
                && r.top() >= r1.top() && r.bottom() <= r1.bottom());
    } else {
        return qt_region_strictContains(cl->clipRegion, r);
    }
}

bool QRasterPaintEnginePrivate::isUnclipped(const QRect &rect) const
{
    Q_Q(const QRasterPaintEngine);
    const QRasterPaintEngineState *s = q->state();
    const QClipData *cl = clip();
    if (!cl) {
        QRect r = rect.normalized();
        // inline contains() for performance (we know the rects are normalized)
        const QRect &r1 = deviceRect;
        return (r.left() >= r1.left() && r.right() <= r1.right()
                && r.top() >= r1.top() && r.bottom() <= r1.bottom());
    }


    // currently all painting functions that call this function clip to deviceRect internally
    if (cl->hasRectClip && cl->clipRect == deviceRect)
        return true;

    QRect r = rect.normalized();
    if (s->flags.antialiased) {
        r.setX(r.x() - 1);
        r.setY(r.y() - 1);
        r.setWidth(r.width() + 2);
        r.setHeight(r.height() + 2);
    }

    if (cl->hasRectClip) {
        // inline contains() for performance (we know the rects are normalized)
        const QRect &r1 = cl->clipRect;
        return (r.left() >= r1.left() && r.right() <= r1.right()
                && r.top() >= r1.top() && r.bottom() <= r1.bottom());
    } else {
        return qt_region_strictContains(cl->clipRegion, r);
    }
}

inline bool QRasterPaintEnginePrivate::isUnclipped(const QRectF &rect) const
{
    return isUnclipped(rect.normalized().toAlignedRect());
}

inline ProcessSpans
QRasterPaintEnginePrivate::getBrushFunc(const QRect &rect,
                                        const QSpanData *data) const
{
    return isUnclipped(rect) ? data->unclipped_blend : data->blend;
}

inline ProcessSpans
QRasterPaintEnginePrivate::getBrushFunc(const QRectF &rect,
                                        const QSpanData *data) const
{
    return isUnclipped(rect) ? data->unclipped_blend : data->blend;
}

/*!
   \reimp
*/
void QRasterPaintEngine::drawStaticTextItem(QStaticTextItem *textItem)
{
    ensurePen();
    ensureRasterState();

    QFontEngine *fontEngine = textItem->fontEngine();
    if (!supportsTransformations(fontEngine)) {
        drawCachedGlyphs(textItem->numGlyphs, textItem->glyphs, textItem->glyphPositions,
                         fontEngine);
    } else {
        QPaintEngineEx::drawStaticTextItem(textItem);
    }
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawTextItem(const QPointF &p, const QTextItem &textItem)
{
    const QTextItemInt &ti = static_cast<const QTextItemInt &>(textItem);
    QRasterPaintEngineState *s = state();

#ifdef QT_DEBUG_DRAW
    Q_D(QRasterPaintEngine);
    fprintf(stderr," - QRasterPaintEngine::drawTextItem(), (%.2f,%.2f), string=%s ct=%d\n",
           p.x(), p.y(), QString::fromRawData(ti.chars, ti.num_chars).toLatin1().data(),
           d->glyphCacheType);
#endif

    ensurePen();
    ensureRasterState();

    QFontEngine *fontEngine = ti.fontEngine;

#if defined(Q_WS_X11)
    if (fontEngine->type() != QFontEngine::Freetype) {
        QPaintEngineEx::drawTextItem(p, ti);
        return;
    }

    QFontEngineFT *fe = static_cast<QFontEngineFT *>(fontEngine);

    QTransform matrix = s->matrix;
    matrix.translate(p.x(), p.y());

    QVarLengthArray<QFixedPoint> positions;
    QVarLengthArray<glyph_t> glyphs;
    fe->getGlyphPositions(ti.glyphs, matrix, ti.flags, glyphs, positions);
    if (glyphs.size() == 0)
        return;

    if (!drawCachedGlyphs(glyphs.size(), glyphs.constData(), positions.constData(), fontEngine))
        QPaintEngine::drawTextItem(p, ti);
#else
    QPaintEngineEx::drawTextItem(p, ti);
#endif
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawPoints(const QPointF *points, int pointCount)
{
    QRasterPaintEngineState *s = state();

    ensurePen();
    if (!s->penData.blend)
        return;

    QPaintEngineEx::drawPoints(points, pointCount);
}


void QRasterPaintEngine::drawPoints(const QPoint *points, int pointCount)
{
    QRasterPaintEngineState *s = state();

    ensurePen();
    if (!s->penData.blend)
        return;

    QPaintEngineEx::drawPoints(points, pointCount);
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawLines(const QLine *lines, int lineCount)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QRasterPaintEngine::drawLines(QLine*)" << lineCount;
#endif
    QRasterPaintEngineState *s = state();

    ensurePen();
    if (!s->penData.blend)
        return;

    QPaintEngineEx::drawLines(lines, lineCount);
}

void QRasterPaintEnginePrivate::rasterizeLine_dashed(QLineF line,
                                                     qreal width,
                                                     int *dashIndex,
                                                     qreal *dashOffset,
                                                     bool *inDash)
{
    Q_Q(QRasterPaintEngine);
    QRasterPaintEngineState *s = q->state();

    const bool squareCap = (s->lastPen.capStyle() == Qt::SquareCap);
    const QVector<qreal> pattern = s->lastPen.dashPattern();

    qreal patternLength = 0;
    for (int i = 0; i < pattern.size(); ++i)
        patternLength += pattern.at(i);

    if (patternLength <= 0)
        return;

    qreal length = line.length();
    Q_ASSERT(length > 0);
    while (length > 0) {
        const bool rasterize = *inDash;
        qreal dash = (pattern.at(*dashIndex) - *dashOffset) * width;
        QLineF l = line;

        if (dash >= length) {
            dash = length;
            *dashOffset += dash / width;
            length = 0;
        } else {
            *dashOffset = 0;
            *inDash = !(*inDash);
            if (++*dashIndex >= pattern.size())
                *dashIndex = 0;
            length -= dash;
            l.setLength(dash);
            line.setP1(l.p2());
        }

        if (rasterize && dash > 0)
            rasterizer->rasterizeLine(l.p1(), l.p2(), width / dash, squareCap);
    }
}

/*!
    \reimp
*/
void QRasterPaintEngine::drawLines(const QLineF *lines, int lineCount)
{
#ifdef QT_DEBUG_DRAW
    qDebug() << " - QRasterPaintEngine::drawLines(QLineF *)" << lineCount;
#endif
    QRasterPaintEngineState *s = state();

    ensurePen();
    if (!s->penData.blend)
        return;

    QPaintEngineEx::drawLines(lines, lineCount);
}


/*!
    \reimp
*/
void QRasterPaintEngine::drawEllipse(const QRectF &rect)
{
    ensurePen();

    QPaintEngineEx::drawEllipse(rect);
}

bool QRasterPaintEngine::supportsTransformations(const QFontEngine *fontEngine) const
{
    if (!state()->WxF)
        return false;
    return supportsTransformations(fontEngine->fontDef.pixelSize, state()->matrix);
}

bool QRasterPaintEngine::supportsTransformations(const qreal pixelSize, const QTransform &m) const
{
    if (m.type() >= QTransform::TxProject)
        return true;

    if (pixelSize * pixelSize * qAbs(m.determinant()) >= 64 * 64)
        return true;

    return false;
}

void QRasterPaintEngine::drawBitmap(const QPointF &pos, const QImage &image, QSpanData *fg)
{
    Q_ASSERT(fg);
    if (!fg->blend)
        return;
    Q_D(QRasterPaintEngine);

    Q_ASSERT(image.depth() == 1);

    const int spanCount = 256;
    QSTACKARRAY(QT_FT_Span, spans, spanCount);
    int n = 0;

    // Boundaries
    int w = image.width();
    int h = image.height();
    int ymax = qMin(qRound(pos.y() + h), d->rasterBuffer->height());
    int ymin = qMax(qRound(pos.y()), 0);
    int xmax = qMin(qRound(pos.x() + w), d->rasterBuffer->width());
    int xmin = qMax(qRound(pos.x()), 0);

    int x_offset = xmin - qRound(pos.x());

    QImage::Format format = image.format();
    for (int y = ymin; y < ymax; ++y) {
        const uchar *src = image.constScanLine(y - qRound(pos.y()));
        if (format == QImage::Format_MonoLSB) {
            for (int x = 0; x < xmax - xmin; ++x) {
                int src_x = x + x_offset;
                uchar pixel = src[src_x >> 3];
                if (!pixel) {
                    x += 7 - (src_x%8);
                    continue;
                }
                if (pixel & (0x1 << (src_x & 7))) {
                    spans[n].x = xmin + x;
                    spans[n].y = y;
                    spans[n].coverage = 255;
                    int len = 1;
                    while (src_x+1 < w && src[(src_x+1) >> 3] & (0x1 << ((src_x+1) & 7))) {
                        ++src_x;
                        ++len;
                    }
                    spans[n].len = ((len + spans[n].x) > xmax) ? (xmax - spans[n].x) : len;
                    x += len;
                    ++n;
                    if (n == spanCount) {
                        fg->blend(n, spans, fg);
                        n = 0;
                    }
                }
            }
        } else {
            for (int x = 0; x < xmax - xmin; ++x) {
                int src_x = x + x_offset;
                uchar pixel = src[src_x >> 3];
                if (!pixel) {
                    x += 7 - (src_x%8);
                    continue;
                }
                if (pixel & (0x80 >> (x & 7))) {
                    spans[n].x = xmin + x;
                    spans[n].y = y;
                    spans[n].coverage = 255;
                    int len = 1;
                    while (src_x+1 < w && src[(src_x+1) >> 3] & (0x80 >> ((src_x+1) & 7))) {
                        ++src_x;
                        ++len;
                    }
                    spans[n].len = ((len + spans[n].x) > xmax) ? (xmax - spans[n].x) : len;
                    x += len;
                    ++n;
                    if (n == spanCount) {
                        fg->blend(n, spans, fg);
                        n = 0;
                    }
                }
            }
        }
    }
    if (n) {
        fg->blend(n, spans, fg);
    }
}

static void qt_merge_clip(const QClipData *c1, const QClipData *c2, QClipData *result)
{
    Q_ASSERT(c1->clipSpanHeight == c2->clipSpanHeight && c1->clipSpanHeight == result->clipSpanHeight);

    QVarLengthArray<short, 4096> buffer;

    QClipData::ClipLine *c1ClipLines = const_cast<QClipData *>(c1)->clipLines();
    QClipData::ClipLine *c2ClipLines = const_cast<QClipData *>(c2)->clipLines();
    result->initialize();

    for (int y = 0; y < c1->clipSpanHeight; ++y) {
        const QSpan *c1_spans = c1ClipLines[y].spans;
        int c1_count = c1ClipLines[y].count;
        const QSpan *c2_spans = c2ClipLines[y].spans;
        int c2_count = c2ClipLines[y].count;

        if (c1_count == 0 && c2_count == 0)
            continue;
        if (c1_count == 0) {
            result->appendSpans(c2_spans, c2_count);
            continue;
        } else if (c2_count == 0) {
            result->appendSpans(c1_spans, c1_count);
            continue;
        }

        // we need to merge the two

        // find required length
        int max = qMax(c1_spans[c1_count - 1].x + c1_spans[c1_count - 1].len,
                c2_spans[c2_count - 1].x + c2_spans[c2_count - 1].len);
        buffer.resize(max);
        memset(buffer.data(), 0, buffer.size() * sizeof(short));

        // Fill with old spans.
        for (int i = 0; i < c1_count; ++i) {
            const QSpan *cs = c1_spans + i;
            for (int j=cs->x; j<cs->x + cs->len; ++j)
                buffer[j] = cs->coverage;
        }

        // Fill with new spans
        for (int i = 0; i < c2_count; ++i) {
            const QSpan *cs = c2_spans + i;
            for (int j = cs->x; j < cs->x + cs->len; ++j) {
                buffer[j] += cs->coverage;
                if (buffer[j] > 255)
                    buffer[j] = 255;
            }
        }

        int x = 0;
        while (x<max) {

            // Skip to next span
            while (x < max && buffer[x] == 0) ++x;
            if (x >= max) break;

            int sx = x;
            int coverage = buffer[x];

            // Find length of span
            while (x < max && buffer[x] == coverage)
                ++x;

            result->appendSpan(sx, x - sx, y, coverage);
        }
    }
}

void QRasterPaintEnginePrivate::initializeRasterizer(QSpanData *data)
{
    Q_Q(QRasterPaintEngine);
    QRasterPaintEngineState *s = q->state();

    rasterizer->setAntialiased(s->flags.antialiased);

    QRect clipRect(deviceRect);
    ProcessSpans blend;
    // ### get from optimized rectbased QClipData

    const QClipData *c = clip();
    if (c) {
        const QRect r(QPoint(c->xmin, c->ymin),
                      QSize(c->xmax - c->xmin, c->ymax - c->ymin));
        clipRect = clipRect.intersected(r);
        blend = data->blend;
    } else {
        blend = data->unclipped_blend;
    }

    rasterizer->setClipRect(clipRect);
    rasterizer->initialize(blend, data);
}

void QRasterPaintEnginePrivate::rasterize(QT_FT_Outline *outline,
                                          ProcessSpans callback,
                                          void *userData)
{
    if (!callback || !outline)
        return;

    Q_Q(QRasterPaintEngine);
    QRasterPaintEngineState *s = q->state();

    if (!s->flags.antialiased) {
        rasterizer->setAntialiased(s->flags.antialiased);
        rasterizer->setClipRect(deviceRect);
        rasterizer->initialize(callback, userData);

        const Qt::FillRule fillRule = outline->flags == QT_FT_OUTLINE_NONE
                                      ? Qt::WindingFill
                                      : Qt::OddEvenFill;

        rasterizer->rasterize(outline, fillRule);
        return;
    }

    gray_raster_reset();

    QT_FT_BBox clip_box = { deviceRect.x(),
                            deviceRect.y(),
                            deviceRect.x() + deviceRect.width(),
                            deviceRect.y() + deviceRect.height() };

    QT_FT_Raster_Params rasterParams;
    rasterParams.source = outline;
    rasterParams.user = userData;
    rasterParams.clip_box = clip_box;
    rasterParams.gray_spans = callback;
    gray_raster_render(&rasterParams);
}

QImage QRasterBuffer::colorizeBitmap(const QImage &image, const QColor &color)
{
    Q_ASSERT(image.depth() == 1);

    QImage sourceImage = image.convertToFormat(QImage::Format_MonoLSB);
    QImage dest = QImage(sourceImage.size(), QImage::Format_ARGB32_Premultiplied);

    QRgb fg = PREMUL(color.rgba());
    QRgb bg = 0;

    int height = sourceImage.height();
    int width = sourceImage.width();
    int bpl = dest.bytesPerLine();
    uchar *data = dest.bits();
    for (int y=0; y<height; ++y) {
        const uchar *source = sourceImage.constScanLine(y);
        QRgb *target = reinterpret_cast<QRgb *>(QFAST_SCAN_LINE(data, bpl, y));
        for (int x=0; x < width; ++x)
            target[x] = (source[x>>3] >> (x&7)) & 1 ? fg : bg;
    }
    return dest;
}

QRasterBuffer::~QRasterBuffer()
{
}

QRasterBuffer::QRasterBuffer()
    : monoDestinationWithClut(false),
    destColor0(0),
    destColor1(0),
    compositionMode(QPainter::CompositionMode_SourceOver),
    m_width(0),
    m_height(0),
    m_buffer(nullptr)
{
}

QImage::Format QRasterBuffer::prepare(QImage *image)
{
    m_buffer = image->bits();
    m_width = qMin(RASTER_COORD_LIMIT, image->width());
    m_height = qMin(RASTER_COORD_LIMIT, image->height());
    bytes_per_pixel = image->depth()/8;
    bytes_per_line = image->bytesPerLine();

    format = image->format();
    drawHelper = qDrawHelper + format;
    if (image->depth() == 1 && image->colorTable().size() == 2) {
        monoDestinationWithClut = true;
        destColor0 = PREMUL(image->colorTable()[0]);
        destColor1 = PREMUL(image->colorTable()[1]);
    }

    return format;
}

QClipData::QClipData(int height)
{
    clipSpanHeight = height;
    m_clipLines = 0;

    allocated = 0;
    m_spans = 0;
    xmin = xmax = ymin = ymax = 0;
    count = 0;

    enabled = true;
    hasRectClip = hasRegionClip = false;
}

QClipData::~QClipData()
{
    if (m_clipLines)
        free(m_clipLines);
    if (m_spans)
        free(m_spans);
}

void QClipData::initialize()
{
    if (m_spans)
        return;

    if (!m_clipLines)
        m_clipLines = (ClipLine *)calloc(sizeof(ClipLine), clipSpanHeight);

    Q_CHECK_PTR(m_clipLines);
    QT_TRY {
        m_spans = (QSpan *)malloc(clipSpanHeight*sizeof(QSpan));
        allocated = clipSpanHeight;
        Q_CHECK_PTR(m_spans);

        QT_TRY {
            if (hasRectClip) {
                int y = 0;
                while (y < ymin) {
                    m_clipLines[y].spans = 0;
                    m_clipLines[y].count = 0;
                    ++y;
                }

                const int len = clipRect.width();
                count = 0;
                while (y < ymax) {
                    QSpan *span = m_spans + count;
                    span->x = xmin;
                    span->len = len;
                    span->y = y;
                    span->coverage = 255;
                    ++count;

                    m_clipLines[y].spans = span;
                    m_clipLines[y].count = 1;
                    ++y;
                }

                while (y < clipSpanHeight) {
                    m_clipLines[y].spans = 0;
                    m_clipLines[y].count = 0;
                    ++y;
                }
            } else if (hasRegionClip) {

                const QVector<QRect> rects = clipRegion.rects();
                const int numRects = rects.size();

                { // resize
                    const int maxSpans = (ymax - ymin) * numRects;
                    if (maxSpans > allocated) {
                        m_spans = (QSpan *)::realloc(m_spans, maxSpans * sizeof(QSpan));
                        Q_CHECK_PTR(m_spans);
                        allocated = maxSpans;
                    }
                }

                int y = 0;
                int firstInBand = 0;
                count = 0;
                while (firstInBand < numRects) {
                    const int currMinY = rects.at(firstInBand).y();
                    const int currMaxY = currMinY + rects.at(firstInBand).height();

                    while (y < currMinY) {
                        m_clipLines[y].spans = 0;
                        m_clipLines[y].count = 0;
                        ++y;
                    }

                    int lastInBand = firstInBand;
                    while (lastInBand + 1 < numRects && rects.at(lastInBand+1).top() == y)
                        ++lastInBand;

                    while (y < currMaxY) {

                        m_clipLines[y].spans = m_spans + count;
                        m_clipLines[y].count = lastInBand - firstInBand + 1;

                        for (int r = firstInBand; r <= lastInBand; ++r) {
                            const QRect &currRect = rects.at(r);
                            QSpan *span = m_spans + count;
                            span->x = currRect.x();
                            span->len = currRect.width();
                            span->y = y;
                            span->coverage = 255;
                            ++count;
                        }
                        ++y;
                    }

                    firstInBand = lastInBand + 1;
                }

                Q_ASSERT(count <= allocated);

                while (y < clipSpanHeight) {
                    m_clipLines[y].spans = 0;
                    m_clipLines[y].count = 0;
                    ++y;
                }

            }
        } QT_CATCH(...) {
            free(m_spans); // have to free m_spans again or someone might think that we were successfully initialized.
            m_spans = 0;
            QT_RETHROW;
        }
    } QT_CATCH(...) {
        free(m_clipLines); // same for clipLines
        m_clipLines = 0;
        QT_RETHROW;
    }
}

void QClipData::fixup()
{
    Q_ASSERT(m_spans);

    if (count == 0) {
        ymin = ymax = xmin = xmax = 0;
        return;
    }

    int y = -1;
    ymin = m_spans[0].y;
    ymax = m_spans[count-1].y + 1;
    xmin = INT_MAX;
    xmax = 0;

    const int firstLeft = m_spans[0].x;
    const int firstRight = m_spans[0].x + m_spans[0].len;
    bool isRect = true;

    for (int i = 0; i < count; ++i) {
        QT_FT_Span_& span = m_spans[i];

        if (span.y != y) {
            if (span.y != y + 1 && y != -1)
                isRect = false;
            y = span.y;
            m_clipLines[y].spans = &span;
            m_clipLines[y].count = 1;
        } else
            ++m_clipLines[y].count;

        const int spanLeft = span.x;
        const int spanRight = spanLeft + span.len;

        if (spanLeft < xmin)
            xmin = spanLeft;

        if (spanRight > xmax)
            xmax = spanRight;

        if (spanLeft != firstLeft || spanRight != firstRight)
            isRect = false;
    }

    if (isRect) {
        hasRectClip = true;
        clipRect.setRect(xmin, ymin, xmax - xmin, ymax - ymin);
    }
}

/*
    Convert \a rect to clip spans.
 */
void QClipData::setClipRect(const QRect &rect)
{
    if (hasRectClip && rect == clipRect)
        return;

//    qDebug() << "setClipRect" << clipSpanHeight << count << allocated << rect;
    hasRectClip = true;
    hasRegionClip = false;
    clipRect = rect;

    xmin = rect.x();
    xmax = rect.x() + rect.width();
    ymin = qMin(rect.y(), clipSpanHeight);
    ymax = qMin(rect.y() + rect.height(), clipSpanHeight);

    if (m_spans) {
        free(m_spans);
        m_spans = 0;
    }

//    qDebug() << xmin << xmax << ymin << ymax;
}

/*
    Convert \a region to clip spans.
 */
void QClipData::setClipRegion(const QRegion &region)
{
    if (region.rectCount() == 1) {
        setClipRect(region.rects().at(0));
        return;
    }

    hasRegionClip = true;
    hasRectClip = false;
    clipRegion = region;

    { // set bounding rect
        const QRect rect = region.boundingRect();
        xmin = rect.x();
        xmax = rect.x() + rect.width();
        ymin = rect.y();
        ymax = rect.y() + rect.height();
    }

    if (m_spans) {
        free(m_spans);
        m_spans = 0;
    }

}

/*!
    \internal
    spans must be sorted on y
*/
static const QSpan *qt_intersect_spans(const QClipData *clip, int *currentClip,
                                       const QSpan *spans, const QSpan *end,
                                       QSpan **outSpans, int available)
{
    const_cast<QClipData *>(clip)->initialize();

    QSpan *out = *outSpans;

    const QSpan *clipSpans = clip->m_spans + *currentClip;
    const QSpan *clipEnd = clip->m_spans + clip->count;

    while (available && spans < end ) {
        if (clipSpans >= clipEnd) {
            spans = end;
            break;
        }
        if (clipSpans->y > spans->y) {
            ++spans;
            continue;
        }
        if (spans->y != clipSpans->y) {
            if (spans->y < clip->count && clip->m_clipLines[spans->y].spans)
                clipSpans = clip->m_clipLines[spans->y].spans;
            else
                ++clipSpans;
            continue;
        }
        Q_ASSERT(spans->y == clipSpans->y);

        int sx1 = spans->x;
        int sx2 = sx1 + spans->len;
        int cx1 = clipSpans->x;
        int cx2 = cx1 + clipSpans->len;

        if (cx1 < sx1 && cx2 < sx1) {
            ++clipSpans;
            continue;
        } else if (sx1 < cx1 && sx2 < cx1) {
            ++spans;
            continue;
        }
        int x = qMax(sx1, cx1);
        int len = qMin(sx2, cx2) - x;
        if (len) {
            out->x = qMax(sx1, cx1);
            out->len = qMin(sx2, cx2) - out->x;
            out->y = spans->y;
            out->coverage = qt_div_255(spans->coverage * clipSpans->coverage);
            ++out;
            --available;
        }
        if (sx2 < cx2) {
            ++spans;
        } else {
            ++clipSpans;
        }
    }

    *outSpans = out;
    *currentClip = clipSpans - clip->m_spans;
    return spans;
}

static void qt_span_fill_clipped(int spanCount, const QSpan *spans, void *userData)
{
//     qDebug() << "qt_span_fill_clipped" << spanCount;
    QSpanData *fillData = reinterpret_cast<QSpanData *>(userData);

    Q_ASSERT(fillData->blend && fillData->unclipped_blend);

    const int NSPANS = 256;
    QSpan cspans[NSPANS];
    int currentClip = 0;
    const QSpan *end = spans + spanCount;
    while (spans < end) {
        QSpan *clipped = cspans;
        spans = qt_intersect_spans(fillData->clip, &currentClip, spans, end, &clipped, NSPANS);
//         qDebug() << "processed " << spanCount - (end - spans) << "clipped" << clipped-cspans
//                  << "span:" << cspans->x << cspans->y << cspans->len << spans->coverage;

        if (clipped - cspans)
            fillData->unclipped_blend(clipped - cspans, cspans, fillData);
    }
}

/*
    \internal
    Clip spans to \a{clip}-rectangle.
    Returns number of unclipped spans
*/
static int qt_intersect_spans(QT_FT_Span *spans, int numSpans,
                              const QRect &clip)
{
    const short minx = clip.left();
    const short miny = clip.top();
    const short maxx = clip.right();
    const short maxy = clip.bottom();

    int n = 0;
    for (int i = 0; i < numSpans; ++i) {
        if (spans[i].y > maxy)
            break;
        if (spans[i].y < miny
            || spans[i].x > maxx
            || spans[i].x + spans[i].len <= minx) {
            continue;
        }
        if (spans[i].x < minx) {
            spans[n].len = qMin(spans[i].len - (minx - spans[i].x), maxx - minx + 1);
            spans[n].x = minx;
        } else {
            spans[n].x = spans[i].x;
            spans[n].len = qMin(spans[i].len, ushort(maxx - spans[n].x + 1));
        }
        if (spans[n].len == 0)
            continue;
        spans[n].y = spans[i].y;
        spans[n].coverage = spans[i].coverage;
        ++n;
    }
    return n;
}


static void qt_span_fill_clipRect(int count, const QSpan *spans,
                                  void *userData)
{
    QSpanData *fillData = reinterpret_cast<QSpanData *>(userData);
    Q_ASSERT(fillData->blend && fillData->unclipped_blend);

    Q_ASSERT(fillData->clip);
    Q_ASSERT(!fillData->clip->clipRect.isEmpty());

    // hw: check if this const_cast<> is safe!!!
    count = qt_intersect_spans(const_cast<QSpan*>(spans), count,
                               fillData->clip->clipRect);
    if (count > 0)
        fillData->unclipped_blend(count, spans, fillData);
}

static void qt_span_clip(int count, const QSpan *spans, void *userData)
{
    ClipData *clipData = reinterpret_cast<ClipData *>(userData);

//     qDebug() << " qt_span_clip: " << count << clipData->operation;
//     for (int i = 0; i < qMin(count, 10); ++i) {
//         qDebug() << "    " << spans[i].x << spans[i].y << spans[i].len << spans[i].coverage;
//     }

    switch (clipData->operation) {

    case Qt::IntersectClip:
        {
            QClipData *newClip = clipData->newClip;
            newClip->initialize();

            int currentClip = 0;
            const QSpan *end = spans + count;
            while (spans < end) {
                QSpan *newspans = newClip->m_spans + newClip->count;
                spans = qt_intersect_spans(clipData->oldClip, &currentClip, spans, end,
                                           &newspans, newClip->allocated - newClip->count);
                newClip->count = newspans - newClip->m_spans;
                if (spans < end) {
                    newClip->m_spans = (QSpan *)::realloc(newClip->m_spans, newClip->allocated*2*sizeof(QSpan));
                    Q_CHECK_PTR(newClip->m_spans);
                    newClip->allocated *= 2;
                }
            }
        }
        break;

    case Qt::UniteClip:
    case Qt::ReplaceClip:
        clipData->newClip->appendSpans(spans, count);
        break;
    case Qt::NoClip:
        break;
    }
}

#ifndef QT_NO_DEBUG
QImage QRasterBuffer::bufferImage() const
{
    QImage image(m_width, m_height, QImage::Format_ARGB32_Premultiplied);

    for (int y = 0; y < m_height; ++y) {
        const uint *span = (const uint *)this->scanLine(y);

        for (int x=0; x<m_width; ++x) {
            image.setPixel(x, y, span[x]);
        }
    }
    return image;
}
#endif

void QSpanData::init(QRasterBuffer *rb, const QRasterPaintEngine *pe)
{
    rasterBuffer = rb;
    type = None;
    txop = QTransform::TxNone;
    bilinear = false;
    m11 = m22 = m33 = 1.;
    m12 = m13 = m21 = m23 = dx = dy = 0.0;
    clip = pe ? pe->d_func()->clip() : 0;
}

Q_GUI_EXPORT extern QImage qt_imageForBrush(int brushStyle);

void QSpanData::setup(const QBrush &brush, int alpha, QPainter::CompositionMode compositionMode)
{
    Qt::BrushStyle brushStyle = brush.style();
    switch (brushStyle) {
    case Qt::SolidPattern: {
        type = Solid;
        const QColor c = brush.color();
        const QRgb rgba = c.rgba();
        solid.color = PREMUL(ARGB_COMBINE_ALPHA(rgba, alpha));
        if ((solid.color & 0xff000000) == 0
            && compositionMode == QPainter::CompositionMode_SourceOver) {
            type = None;
        }
        break;
    }

    case Qt::LinearGradientPattern:
        {
            type = LinearGradient;
            const QLinearGradient *g = static_cast<const QLinearGradient *>(brush.gradient());
            gradient.alphaColor = !brush.isOpaque() || alpha != 256;
            gradient.generateGradientColorTable(*g, alpha);
            gradient.spread = g->spread();

            QLinearGradientData &linearData = gradient.linear;

            linearData.origin.x = g->start().x();
            linearData.origin.y = g->start().y();
            linearData.end.x = g->finalStop().x();
            linearData.end.y = g->finalStop().y();
            break;
        }

    case Qt::RadialGradientPattern:
        {
            type = RadialGradient;
            const QRadialGradient *g = static_cast<const QRadialGradient *>(brush.gradient());
            gradient.alphaColor = !brush.isOpaque() || alpha != 256;
            gradient.generateGradientColorTable(*g, alpha);
            gradient.spread = g->spread();

            QRadialGradientData &radialData = gradient.radial;

            QPointF center = g->center();
            radialData.center.x = center.x();
            radialData.center.y = center.y();
            radialData.center.radius = g->centerRadius();
            QPointF focal = g->focalPoint();
            radialData.focal.x = focal.x();
            radialData.focal.y = focal.y();
            radialData.focal.radius = g->focalRadius();
        }
        break;

    case Qt::ConicalGradientPattern:
        {
            type = ConicalGradient;
            const QConicalGradient *g = static_cast<const QConicalGradient *>(brush.gradient());
            gradient.alphaColor = !brush.isOpaque() || alpha != 256;
            gradient.generateGradientColorTable(*g, alpha);
            gradient.spread = QGradient::RepeatSpread;

            QConicalGradientData &conicalData = gradient.conical;

            QPointF center = g->center();
            conicalData.center.x = center.x();
            conicalData.center.y = center.y();
            conicalData.angle = g->angle() * 2 * M_PI / 360.0;
        }
        break;

    case Qt::Dense1Pattern:
    case Qt::Dense2Pattern:
    case Qt::Dense3Pattern:
    case Qt::Dense4Pattern:
    case Qt::Dense5Pattern:
    case Qt::Dense6Pattern:
    case Qt::Dense7Pattern:
    case Qt::HorPattern:
    case Qt::VerPattern:
    case Qt::CrossPattern:
    case Qt::BDiagPattern:
    case Qt::FDiagPattern:
    case Qt::DiagCrossPattern:
        type = Texture;
        if (!tempImage)
            tempImage = new QImage();
        *tempImage = rasterBuffer->colorizeBitmap(qt_imageForBrush(brushStyle), brush.color());
        initTexture(tempImage, alpha, QTextureData::Tiled);
        break;
    case Qt::TexturePattern:
        type = Texture;
        if (!tempImage)
            tempImage = new QImage();

        if (qHasPixmapTexture(brush) && brush.texture().isQBitmap())
            *tempImage = rasterBuffer->colorizeBitmap(brush.textureImage(), brush.color());
        else
            *tempImage = brush.textureImage();
        initTexture(tempImage, alpha, QTextureData::Tiled, tempImage->rect());
        break;

    case Qt::NoBrush:
        type = None;
        break;
    }
    adjustSpanMethods();
}

void QSpanData::adjustSpanMethods()
{
    bitmapBlit = 0;
    alphamapBlit = 0;
    alphaRGBBlit = 0;

    fillRect = 0;

    switch(type) {
    case None:
        unclipped_blend = 0;
        break;
    case Solid:
        unclipped_blend = rasterBuffer->drawHelper->blendColor;
        bitmapBlit = rasterBuffer->drawHelper->bitmapBlit;
        alphamapBlit = rasterBuffer->drawHelper->alphamapBlit;
        alphaRGBBlit = rasterBuffer->drawHelper->alphaRGBBlit;
        fillRect = rasterBuffer->drawHelper->fillRect;
        break;
    case LinearGradient:
    case RadialGradient:
    case ConicalGradient:
        unclipped_blend = rasterBuffer->drawHelper->blendGradient;
        break;
    case Texture:
        unclipped_blend = qBlendTexture;
        if (!texture.imageData)
            unclipped_blend = 0;

        break;
    }
    // setup clipping
    if (!unclipped_blend) {
        blend = 0;
    } else if (!clip) {
        blend = unclipped_blend;
    } else if (clip->hasRectClip) {
        blend = clip->clipRect.isEmpty() ? 0 : qt_span_fill_clipRect;
    } else {
        blend = qt_span_fill_clipped;
    }
}

void QSpanData::setupMatrix(const QTransform &matrix, bool bilin)
{
    // make sure we round off correctly in qdrawhelper.cpp
    static const qreal delta = 1.0 / 65536;
    static const QTransform deltam = QTransform::fromTranslate(delta, delta);

    QTransform inv = (deltam * matrix).inverted();
    m11 = inv.m11();
    m12 = inv.m12();
    m13 = inv.m13();
    m21 = inv.m21();
    m22 = inv.m22();
    m23 = inv.m23();
    m33 = inv.m33();
    dx = inv.dx();
    dy = inv.dy();
    txop = inv.type();
    bilinear = bilin;

    adjustSpanMethods();
}

void QSpanData::initTexture(const QImage *image, int alpha, QTextureData::Type _type, const QRect &sourceRect)
{
    const QImageData *d = image->d;
    if (!d || d->height == 0) {
        texture.imageData = 0;
        texture.width = 0;
        texture.height = 0;
        texture.x1 = 0;
        texture.y1 = 0;
        texture.x2 = 0;
        texture.y2 = 0;
        texture.bytesPerLine = 0;
        texture.format = QImage::Format_Invalid;
        texture.colorTable = 0;
        texture.hasAlpha = alpha != 256;
    } else {
        texture.imageData = d->data;
        texture.width = d->width;
        texture.height = d->height;

        if (sourceRect.isNull()) {
            texture.x1 = 0;
            texture.y1 = 0;
            texture.x2 = texture.width;
            texture.y2 = texture.height;
        } else {
            texture.x1 = sourceRect.x();
            texture.y1 = sourceRect.y();
            texture.x2 = qMin(texture.x1 + sourceRect.width(), d->width);
            texture.y2 = qMin(texture.y1 + sourceRect.height(), d->height);
        }

        texture.bytesPerLine = d->bytes_per_line;

        texture.format = d->format;
        texture.colorTable = (d->format <= QImage::Format_Indexed8 && !d->colortable.isEmpty()) ? &d->colortable : 0;
        texture.hasAlpha = image->hasAlphaChannel() || alpha != 256;
    }
    texture.const_alpha = alpha;
    texture.type = _type;

    adjustSpanMethods();
}

/*!
    \fn void QRasterPaintEngine::drawPoints(const QPoint *points, int pointCount)
    \overload

    Draws the first \a pointCount points in the buffer \a points

    The default implementation converts the first \a pointCount QPoints in \a points
    to QPointFs and calls the floating point version of drawPoints.
*/

/*!
    \fn void QRasterPaintEngine::drawEllipse(const QRect &rect)
    \overload

    Reimplement this function to draw the largest ellipse that can be
    contained within rectangle \a rect.
*/

#ifdef QT_DEBUG_DRAW
void dumpClip(int width, int height, const QClipData *clip)
{
    QImage clipImg(width, height, QImage::Format_ARGB32_Premultiplied);
    clipImg.fill(0xffff0000);

    int x0 = width;
    int x1 = 0;
    int y0 = height;
    int y1 = 0;

    ((QClipData *) clip)->spans(); // Force allocation of the spans structure...

    for (int i = 0; i < clip->count; ++i) {
        const QSpan *span = ((QClipData *) clip)->spans() + i;
        for (int j = 0; j < span->len; ++j)
            clipImg.setPixel(span->x + j, span->y, 0xffffff00);
        x0 = qMin(x0, int(span->x));
        x1 = qMax(x1, int(span->x + span->len - 1));

        y0 = qMin(y0, int(span->y));
        y1 = qMax(y1, int(span->y));
    }

    static int counter = 0;

    Q_ASSERT(y0 >= 0);
    Q_ASSERT(x0 >= 0);
    Q_ASSERT(y1 >= 0);
    Q_ASSERT(x1 >= 0);

    fprintf(stderr,"clip %d: %d %d - %d %d\n", counter, x0, y0, x1, y1);
    clipImg.save(QString::fromLatin1("clip-%0.png").arg(counter++));
}
#endif


QT_END_NAMESPACE





