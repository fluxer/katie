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

#ifndef QPAINTENGINE_X11_P_H
#define QPAINTENGINE_X11_P_H

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

#include "QtGui/qpaintengine.h"
#include "QtGui/qregion.h"
#include "QtGui/qpen.h"
#include "QtCore/qpoint.h"
#include "qpaintengine_p.h"
#include "qpainter_p.h"
#include "qpolygonclipper_p.h"

typedef unsigned long Picture;

QT_BEGIN_NAMESPACE

class QX11PaintEnginePrivate;
class QFontEngineFT;
class QXRenderTessellator;

class QX11PaintEngine : public QPaintEngine
{
    Q_DECLARE_PRIVATE(QX11PaintEngine)
public:
    QX11PaintEngine();
    ~QX11PaintEngine();

    bool begin(QPaintDevice *pdev);
    bool end();

    void updateState(const QPaintEngineState &state);

    void updatePen(const QPen &pen);
    void updateBrush(const QBrush &brush, const QPointF &pt);
    void updateRenderHints(QPainter::RenderHints hints);
    void updateMatrix(const QTransform &matrix);
    void updateClipRegion_dev(const QRegion &region, Qt::ClipOperation op);

    void drawLines(const QLine *lines, int lineCount);
    void drawLines(const QLineF *lines, int lineCount);

    void drawRects(const QRect *rects, int rectCount);
    void drawRects(const QRectF *rects, int rectCount);

    void drawPoints(const QPoint *points, int pointCount);
    void drawPoints(const QPointF *points, int pointCount);

    void drawEllipse(const QRect &r);
    void drawEllipse(const QRectF &r);

    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);

    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);
    void drawPath(const QPainterPath &path);
    void drawTextItem(const QPointF &p, const QTextItem &textItem);
    void drawImage(const QRectF &r, const QImage &img, const QRectF &sr,
                   Qt::ImageConversionFlags flags = Qt::AutoColor);

    inline Type type() const { return QPaintEngine::X11; }

    QPainter::RenderHints supportedRenderHints() const;

protected:
    QX11PaintEngine(QX11PaintEnginePrivate &dptr);

#ifndef QT_NO_FONTCONFIG
    void drawFreetype(const QPointF &p, const QTextItemInt &si);
#endif

    friend class QPixmap;
    friend class QFontEngineBox;

private:
    Q_DISABLE_COPY(QX11PaintEngine)
};

class QX11PaintEnginePrivate : public QPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QX11PaintEngine)
public:
    QX11PaintEnginePrivate()
    {
        scrn = -1;
        hd = 0;
        picture = 0;
        gc = gc_brush = 0;
        dpy  = 0;
        xinfo = 0;
        txop = QTransform::TxNone;
        has_clipping = false;
        render_hints = 0;
        xform_scale = 1;
#ifndef QT_NO_XRENDER
        tessellator = 0;
#endif
    }
    enum GCMode {
        PenGC,
        BrushGC
    };

    void init();
    void fillPolygon_translated(const QPointF *points, int pointCount, GCMode gcMode,
                                QPaintEngine::PolygonDrawMode mode);
    void fillPolygon_dev(const QPointF *points, int pointCount, GCMode gcMode,
                         QPaintEngine::PolygonDrawMode mode);
    void fillPath(const QPainterPath &path, GCMode gcmode, bool transform);
    void strokePolygon_dev(const QPointF *points, int pointCount, bool close);
    void strokePolygon_translated(const QPointF *points, int pointCount, bool close);
    void setupAdaptedOrigin(const QPoint &p);
    void resetAdaptedOrigin();
    void decidePathFallback() {
        use_path_fallback = has_alpha_brush
                            || has_alpha_pen
                            || has_custom_pen
                            || has_complex_xform
                            || (render_hints & QPainter::Antialiasing);
    }
    void decideCoordAdjust() {
        adjust_coords = !(render_hints & QPainter::Antialiasing)
                        && (has_alpha_pen
                            || (has_alpha_brush && has_pen && !has_alpha_pen)
                            || (cpen.style() > Qt::SolidLine));
    }
    void clipPolygon_dev(const QPolygonF &poly, QPolygonF *clipped_poly);
    void systemStateChanged();

    Display *dpy;
    int scrn;
    int pdev_depth;
    Qt::HANDLE hd;
    QPixmap brush_pm;
#if !defined (QT_NO_XRENDER)
    Qt::HANDLE picture;
    Qt::HANDLE current_brush;
    QPixmap bitmap_texture;
    int composition_mode;
#else
    Qt::HANDLE picture;
#endif
    GC gc;
    GC gc_brush;

    QPen cpen;
    QBrush cbrush;
    QRegion crgn;
    QTransform matrix;
    qreal opacity;

    bool has_complex_xform;
    bool has_scaling_xform;
    bool has_custom_pen;
    bool use_path_fallback;
    bool adjust_coords;
    bool has_clipping;
    bool adapted_brush_origin;
    bool has_pen;
    bool has_brush;
    bool has_texture;
    bool has_alpha_texture;
    bool has_pattern;
    bool has_alpha_pen;
    bool has_alpha_brush;
    QPainter::RenderHints render_hints;

    const QX11Info *xinfo;
    QPointF bg_origin;
    QTransform::TransformationType txop;
    qreal xform_scale;
    QPolygonClipper polygonClipper;

    int xlibMaxLinePoints;
#ifndef QT_NO_XRENDER
    QXRenderTessellator *tessellator;
#endif
};

QT_END_NAMESPACE

#endif // QPAINTENGINE_X11_P_H
