/****************************************************************************
**
** Copyright (C) 2021 Ivailo Monev
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

#include "qpaintengine_raster_p.h"
#include "qpen.h"
#include "qfont.h"
#include "qdebug.h"
#include "qimage_p.h"
#include "qpixmapdata_p.h"
#include "qdrawhelper_p.h"

QT_BEGIN_NAMESPACE

extern QImage qt_imageForBrush(int brushStyle); // in qbrush.cpp

static QImage qt_colorizeBitmap(const QImage &image, const QColor &color)
{
    if (image.depth() != 1) {
        return image;
    }

    QImage sourceImage = image.convertToFormat(QImage::Format_MonoLSB);
    QImage dest(sourceImage.size(), QImage::Format_ARGB32_Premultiplied);

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

static const QPaintEngine::PaintEngineFeatures qt_raster_features =
    QPaintEngine::PainterPaths |
    QPaintEngine::PerspectiveTransform |
    QPaintEngine::PatternTransform |
    QPaintEngine::PorterDuff |
    QPaintEngine::BlendModes;

QRasterPaintEnginePrivate::QRasterPaintEnginePrivate()
{
}

QRasterPaintEnginePrivate::~QRasterPaintEnginePrivate()
{
}

QRasterPaintEngine::QRasterPaintEngine()
    : QPaintEngine(*(new QRasterPaintEnginePrivate), qt_raster_features)
{
}

QRasterPaintEngine::QRasterPaintEngine(QRasterPaintEnginePrivate &dptr)
    : QPaintEngine(dptr, qt_raster_features)
{
}

QRasterPaintEngine::~QRasterPaintEngine()
{
}

bool QRasterPaintEngine::begin(QPaintDevice *pdev)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << pdev;

    // should not happen but it does
    BLImageData imageData;
    BLResult blresult = d->m_image.getData(&imageData);
    if (blresult == BL_SUCCESS && imageData.pixelData && !end()) {
        return false;
    }

    QImage *image = nullptr;
    switch (pdev->devType()) {
        case QInternal::Image: {
            image = reinterpret_cast<QImage*>(pdev);
            break;
        }
        case QInternal::Pixmap: {
            const QPixmap* pixmap = reinterpret_cast<QPixmap*>(pdev);
            QPixmapData *pixmapdata = pixmap->pixmapData();
            const bool israsterpixmap = (pixmapdata->classId() == QPixmapData::RasterClass);
            Q_ASSERT_X(israsterpixmap, "QRasterPaintEngine::begin", "internal error");

            image = pixmapdata->buffer();

            // set it to the actual image
            setPaintDevice(image);
            break;
        }
        default: {
            Q_ASSERT_X(false, "QRasterPaintEngine::begin", "internal error");
            break;
        }
    }

    d->m_sourceimage = image->convertToFormat(QImage::Format_ARGB32_Premultiplied);
    d->m_image = imageObject(d->m_sourceimage);

    return true;
}

bool QRasterPaintEngine::end()
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO;

    QImage *image = nullptr;
    // something changed the paint device between begin() and end()? that's
    // absolutely the wrong thing to do
    switch (d->pdev->devType()) {
        case QInternal::Image: {
            image = reinterpret_cast<QImage*>(d->pdev);
            break;
        }
        case QInternal::Pixmap: {
            const QPixmap* pixmap = reinterpret_cast<QPixmap*>(d->pdev);
            QPixmapData *pixmapdata = pixmap->pixmapData();
            const bool israsterpixmap = (pixmapdata->classId() == QPixmapData::RasterClass);
            Q_ASSERT_X(israsterpixmap, "QRasterPaintEngine::end", "internal error");

            image = pixmapdata->buffer();
            break;
        }
        default: {
            Q_ASSERT_X(false, "QRasterPaintEngine::end", "internal error");
            break;
        }
    }

    BLResult blresult = d->m_image.convert(BL_FORMAT_PRGB32);
    if (blresult != BL_SUCCESS) {
        qWarning("QRasterPaintEngine::end: Could not convert image");
        return false;
    }

    BLImageData imageData;
    blresult = d->m_image.getData(&imageData);
    if (blresult != BL_SUCCESS) {
        qWarning("QRasterPaintEngine::end: Could not get image data");
        return false;
    }

    // Q_ASSERT_X(image->d->ref == 1, "QRasterPaintEngine::end", "internal error");
    QImage converted = d->m_sourceimage.convertToFormat(image->format());
    ::memcpy(image->d->data, converted.constBits(), image->byteCount());

    return true;
}

void QRasterPaintEngine::updateState(const QPaintEngineState &state)
{
#if QT_RASTER_DEBUG
    qDebug() << Q_FUNC_INFO << state.pen() << state.brush() << state.brushOrigin()
        << state.backgroundBrush() << state.backgroundMode() << state.font()
        << state.matrix() << state.transform() << state.clipOperation()
        << state.clipRegion() << state.clipPath() << state.isClipEnabled()
        << state.renderHints() << state.compositionMode() << state.opacity();
#endif
}

void QRasterPaintEngine::drawPath(const QPainterPath &path)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << points << pointCount << mode;

    BLContext blcontext(d->m_image);

    const QTransform statetransform(state->transform());
    BLMatrix2D blmatrix(
        statetransform.m11(), statetransform.m12(),
        statetransform.m21(), statetransform.m22(),
        statetransform.dx(), statetransform.dy()
    );
    blcontext.transform(blmatrix);

    switch (state->compositionMode()) {
        case QPainter::CompositionMode_SourceOver: {
            blcontext.setCompOp(BL_COMP_OP_SRC_OVER);
            break;
        }
        case QPainter::CompositionMode_DestinationOver: {
            blcontext.setCompOp(BL_COMP_OP_DST_OVER);
            break;
        }
        case QPainter::CompositionMode_Clear: {
            blcontext.setCompOp(BL_COMP_OP_CLEAR);
            break;
        }
        case QPainter::CompositionMode_Source: {
            blcontext.setCompOp(BL_COMP_OP_SRC_COPY);
            break;
        }
        case QPainter::CompositionMode_Destination: {
            blcontext.setCompOp(BL_COMP_OP_DST_COPY);
            break;
        }
        case QPainter::CompositionMode_SourceIn: {
            blcontext.setCompOp(BL_COMP_OP_SRC_IN);
            break;
        }
        case QPainter::CompositionMode_DestinationIn: {
            blcontext.setCompOp(BL_COMP_OP_DST_IN);
            break;
        }
        case QPainter::CompositionMode_SourceOut: {
            blcontext.setCompOp(BL_COMP_OP_SRC_OUT);
            break;
        }
        case QPainter::CompositionMode_DestinationOut: {
            blcontext.setCompOp(BL_COMP_OP_DST_OUT);
            break;
        }
        case QPainter::CompositionMode_SourceAtop: {
            blcontext.setCompOp(BL_COMP_OP_SRC_ATOP);
            break;
        }
        case QPainter::CompositionMode_DestinationAtop: {
            blcontext.setCompOp(BL_COMP_OP_SRC_ATOP);
            break;
        }
        case QPainter::CompositionMode_Xor: {
            blcontext.setCompOp(BL_COMP_OP_DST_ATOP);
            break;
        }
        case QPainter::CompositionMode_Plus: {
            blcontext.setCompOp(BL_COMP_OP_PLUS);
            break;
        }
        case QPainter::CompositionMode_Multiply: {
            blcontext.setCompOp(BL_COMP_OP_MULTIPLY);
            break;
        }
        case QPainter::CompositionMode_Screen: {
            blcontext.setCompOp(BL_COMP_OP_SCREEN);
            break;
        }
        case QPainter::CompositionMode_Overlay: {
            blcontext.setCompOp(BL_COMP_OP_OVERLAY);
            break;
        }
        case QPainter::CompositionMode_Darken: {
            blcontext.setCompOp(BL_COMP_OP_DARKEN);
            break;
        }
        case QPainter::CompositionMode_Lighten: {
            blcontext.setCompOp(BL_COMP_OP_LIGHTEN);
            break;
        }
        case QPainter::CompositionMode_ColorDodge: {
            blcontext.setCompOp(BL_COMP_OP_COLOR_DODGE);
            break;
        }
        case QPainter::CompositionMode_ColorBurn: {
            blcontext.setCompOp(BL_COMP_OP_COLOR_BURN);
            break;
        }
        case QPainter::CompositionMode_HardLight: {
            blcontext.setCompOp(BL_COMP_OP_HARD_LIGHT);
            break;
        }
        case QPainter::CompositionMode_SoftLight: {
            blcontext.setCompOp(BL_COMP_OP_SOFT_LIGHT);
            break;
        }
        case QPainter::CompositionMode_Difference: {
            blcontext.setCompOp(BL_COMP_OP_DIFFERENCE);
            break;
        }
        case QPainter::CompositionMode_Exclusion: {
            blcontext.setCompOp(BL_COMP_OP_EXCLUSION);
            break;
        }
    }

    // TODO: no clipping needed?
#if 0
    const QRegion stateregion(state->clipRegion());
    if (!stateregion.isEmpty()) {
        foreach (const QRect &regionrect, stateregion.rects()) {
            blcontext.clipToRect(
                regionrect.x(), regionrect.y(),
                regionrect.width(), regionrect.height()
            );
        }
    }
#endif

    const QPen statepen(state->pen());
    if (statepen.isCosmetic()) {
        blcontext.setStrokeWidth(1.0);
    } else {
        blcontext.setStrokeWidth(statepen.widthF());
    }

    switch (statepen.capStyle()) {
        case Qt::FlatCap: {
            blcontext.setStrokeCaps(BL_STROKE_CAP_BUTT);
            break;
        }
        case Qt::SquareCap: {
            blcontext.setStrokeCaps(BL_STROKE_CAP_SQUARE);
            break;
        }
        case Qt::RoundCap: {
            blcontext.setStrokeCaps(BL_STROKE_CAP_ROUND);
            break;
        }
    }

    switch (statepen.joinStyle()) {
        case Qt::MiterJoin: {
            blcontext.setStrokeJoin(BL_STROKE_JOIN_MITER_CLIP);
            break;
        }
        case Qt::BevelJoin: {
            blcontext.setStrokeJoin(BL_STROKE_JOIN_BEVEL);
            break;
        }
        case Qt::RoundJoin: {
            blcontext.setStrokeJoin(BL_STROKE_JOIN_MITER_ROUND);
            break;
        }
        case Qt::SvgMiterJoin: {
            blcontext.setStrokeJoin(BL_STROKE_JOIN_MITER_CLIP);
            break;
        }
    }

    blcontext.setStrokeMiterLimit(statepen.miterLimit());

    const QVector<qreal> pendashpattern(statepen.dashPattern());
    BLArray<double> blarray;
    for (int i = 0; i < pendashpattern.size(); i++) {
        blarray.append(pendashpattern.at(i));
    }
    blcontext.setStrokeDashArray(blarray);
    blcontext.setStrokeDashOffset(statepen.dashOffset());

    BLPath blpath;
    for (int i = 0; i < path.elementCount(); i++) {
        const QPainterPath::Element pathelement = path.elementAt(i);
        switch (pathelement.type) {
            case QPainterPath::MoveToElement: {
                blpath.moveTo(pathelement.x, pathelement.y);
                break;
            }
            case QPainterPath::LineToElement: {
                blpath.lineTo(pathelement.x, pathelement.y);
                break;
            }
            case QPainterPath::CurveToElement: {
                blpath.cubicTo(
                    pathelement.x, pathelement.y,
                    path.elementAt(i + 1).x, path.elementAt(i + 1).y,
                    path.elementAt(i + 2).x, path.elementAt(i + 2).y
                );
                i += 2;
                break;
            }
            case QPainterPath::CurveToDataElement: {
                Q_ASSERT_X(false, "QRasterPaintEngine::drawPath", "internal error");
                break;
            }
        }
    }

    blcontext.setStrokeStyle(BLRgba32(statepen.color().rgba()));
    blcontext.strokePath(blpath);

    switch (path.fillRule()) {
        case Qt::WindingFill: {
            blcontext.setFillRule(BL_FILL_RULE_NON_ZERO);
            break;
        }
        case Qt::OddEvenFill: {
            blcontext.setFillRule(BL_FILL_RULE_EVEN_ODD);
            break;
        }
    }

    // TODO: set matrix for image patterns?
    const QBrush statebrush(state->brush());
    switch (statebrush.style()) {
        case Qt::NoBrush: {
            break;
        }
        case Qt::SolidPattern: {
            blcontext.setFillStyle(BLRgba32(statebrush.color().rgba()));
            blcontext.fillPath(blpath);
            break;
        }
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
        case Qt::DiagCrossPattern: {
            const QImage sourceimage(qt_imageForBrush(statebrush.style()).convertToFormat(QImage::Format_ARGB32_Premultiplied));
            const BLImage blimage = imageObject(qt_colorizeBitmap(sourceimage, statebrush.color()));

            blcontext.setFillStyle(BLPattern(blimage));
            blcontext.fillPath(blpath);
            break;
        }
        case Qt::LinearGradientPattern: {
            const QLinearGradient* brushgradient(static_cast<const QLinearGradient*>(statebrush.gradient()));

            BLExtendMode blextend = BL_EXTEND_MODE_PAD;
            switch (brushgradient->spread()) {
                case QGradient::PadSpread: {
                    blextend = BL_EXTEND_MODE_PAD;
                    break;
                }
                case QGradient::ReflectSpread: {
                    blextend = BL_EXTEND_MODE_REFLECT;
                    break;
                }
                case QGradient::RepeatSpread: {
                    blextend = BL_EXTEND_MODE_REPEAT;
                    break;
                }
            }

            BLGradient blgradient(
                BLLinearGradientValues(
                    brushgradient->start().x(), brushgradient->start().y(),
                    brushgradient->finalStop().x(), brushgradient->finalStop().y()
                ),
                blextend
            );

            foreach (const QGradientStop &gradientstop, brushgradient->stops()) {
                blgradient.addStop(
                    gradientstop.first, BLRgba32(gradientstop.second.rgba())
                );
            }

            const QPointF stateorigin(state->brushOrigin());
            const QTransform brushtransform(statebrush.transform());
            BLMatrix2D blmatrix(
                brushtransform.m11(), brushtransform.m12(),
                brushtransform.m21(), brushtransform.m22(),
                brushtransform.dx(), brushtransform.dy()
            );
            blmatrix.translate(stateorigin.x(), stateorigin.y());
            blgradient.setMatrix(blmatrix);

            blcontext.setFillStyle(blgradient);
            blcontext.fillPath(blpath);
            break;
        }
        case Qt::RadialGradientPattern: {
            const QRadialGradient* brushgradient(static_cast<const QRadialGradient*>(statebrush.gradient()));

            BLExtendMode blextend = BL_EXTEND_MODE_PAD;
            switch (brushgradient->spread()) {
                case QGradient::PadSpread: {
                    blextend = BL_EXTEND_MODE_PAD;
                    break;
                }
                case QGradient::ReflectSpread: {
                    blextend = BL_EXTEND_MODE_REFLECT;
                    break;
                }
                case QGradient::RepeatSpread: {
                    blextend = BL_EXTEND_MODE_REPEAT;
                    break;
                }
            }

            BLGradient blgradient(
                BLRadialGradientValues(
                    brushgradient->center().x(), brushgradient->center().y(),
                    // TODO: brushgradient->centerRadius(),
                    brushgradient->focalPoint().x(), brushgradient->focalPoint().y(),
                    brushgradient->focalRadius()
                ),
                blextend
            );

            foreach (const QGradientStop &gradientstop, brushgradient->stops()) {
                blgradient.addStop(
                    gradientstop.first, BLRgba32(gradientstop.second.rgba())
                );
            }

            const QPointF stateorigin(state->brushOrigin());
            const QTransform brushtransform(statebrush.transform());
            BLMatrix2D blmatrix(
                brushtransform.m11(), brushtransform.m12(),
                brushtransform.m21(), brushtransform.m22(),
                brushtransform.dx(), brushtransform.dy()
            );
            blmatrix.translate(stateorigin.x(), stateorigin.y());
            blgradient.setMatrix(blmatrix);

            blcontext.setFillStyle(blgradient);
            blcontext.fillPath(blpath);
            break;
        }
        case Qt::TexturePattern: {
            BLImage blimage;
            QImage sourceimage;
            if (qHasPixmapTexture(statebrush) && statebrush.texture().isQBitmap()) {
                sourceimage = qt_colorizeBitmap(statebrush.textureImage(), statebrush.color());
            } else {
                sourceimage = statebrush.textureImage();
            }
            sourceimage = sourceimage.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            blimage = imageObject(sourceimage);

            blcontext.setFillStyle(BLPattern(blimage));
            blcontext.fillPath(blpath);
            break;
        }
    }

    blcontext.end();
}

void QRasterPaintEngine::drawPolygon(const QPointF *points, int pointCount, QPaintEngine::PolygonDrawMode mode)
{
    // qDebug() << Q_FUNC_INFO << points << pointCount << mode;

    QPainterPath path(points[0]);
    for (int i = 1; i < pointCount; i++) {
        path.lineTo(points[i]);
    }
    if (mode == QPaintEngine::PolylineMode) {
        const QBrush statebrush(state->brush());
        painter()->setBrush(QBrush(Qt::NoBrush));
        path.setFillRule(Qt::WindingFill);
        drawPath(path);
        painter()->setBrush(statebrush);
    } else {
        path.setFillRule(mode == QPaintEngine::OddEvenMode ? Qt::OddEvenFill : Qt::WindingFill);
        path.closeSubpath();
        drawPath(path);
    }
}

void QRasterPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pixmap, const QRectF &sr)
{
    // qDebug() << Q_FUNC_INFO << r << pixmap.cacheKey() << sr;

    drawImage(r, pixmap.toImage(), sr);
}

void QRasterPaintEngine::drawImage(const QRectF &r, const QImage &image, const QRectF &sr,
                                   Qt::ImageConversionFlags flags)
{
    Q_D(QRasterPaintEngine);

    // qDebug() << Q_FUNC_INFO << r << image.cacheKey() << sr << flags;

    const QImage sourceimage = image.copy(sr.toRect()).convertToFormat(QImage::Format_ARGB32_Premultiplied, flags);
    const BLImage blimage = imageObject(sourceimage);

    BLContext blcontext(d->m_image);

    blcontext.setFillStyle(BLPattern(blimage));
    blcontext.fillRect(r.x(), r.y(), r.width(), r.height());

    blcontext.end();
}

BLImage QRasterPaintEngine::imageObject(const QImage &image)
{
    BLImage blimage;
    const BLResult blresult = blimage.createFromData(
        image.width(), image.height(),
        BL_FORMAT_PRGB32, (void*)image.bits(), image.bytesPerLine()
    );
    if (blresult != BL_SUCCESS) {
        qWarning("QRasterPaintEngine::imageObject: Could not create image");
        return BLImage();
    }
    return blimage;
}

QT_END_NAMESPACE
