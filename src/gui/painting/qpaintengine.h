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

#ifndef QPAINTENGINE_H
#define QPAINTENGINE_H

#include <QtCore/qnamespace.h>
#include <QtCore/qobjectdefs.h>
#include <QtCore/qscopedpointer.h>
#include <QtGui/qpainter.h>


QT_BEGIN_NAMESPACE

class QLineF;
class QPaintDevice;
class QPaintEnginePrivate;
class QPainterPath;
class QPointF;
class QRectF;
class QPaintEngineState;

class Q_GUI_EXPORT QTextItem {
public:
    enum RenderFlag {
        RightToLeft = 0x1,
        Overline = 0x10,
        Underline = 0x20,
        StrikeOut = 0x40,

        Dummy = 0xffffffff
    };
    Q_DECLARE_FLAGS(RenderFlags, RenderFlag)
    qreal descent() const;
    qreal ascent() const;
    qreal width() const;

    RenderFlags renderFlags() const;
    QString text() const;
    QFont font() const;
};
Q_DECLARE_TYPEINFO(QTextItem, Q_PRIMITIVE_TYPE);


class Q_GUI_EXPORT QPaintEngine
{
    Q_DECLARE_PRIVATE(QPaintEngine)
public:
    enum PaintEngineFeature {
        PatternTransform            = 0x00000001, // Can transform pattern brushes
        PixmapTransform             = 0x00000002, // Can transform pixmaps
        AlphaBlend                  = 0x00000004, // Can do source over alpha blend
        PorterDuff                  = 0x00000008, // Can do general porter duff compositions
        PainterPaths                = 0x00000010, // Can fill, outline and clip paths
        Antialiasing                = 0x00000020, // Can antialias lines
        ConstantOpacity             = 0x00000040, // Can render at constant opacity
        PerspectiveTransform        = 0x00000080, // Can do perspective transformations
        BlendModes                  = 0x00000100, // Can do extended Porter&Duff composition
        RasterOpModes               = 0x00000200, // Can do logical raster operations
        PaintOutsidePaintEvent      = 0x00000400, // Engine is capable of painting outside paint events

        AllFeatures                 = 0xffffffff  // For convenience
    };
    Q_DECLARE_FLAGS(PaintEngineFeatures, PaintEngineFeature)

    enum DirtyFlag {
        DirtyPen                = 0x0001,
        DirtyBrush              = 0x0002,
        DirtyBrushOrigin        = 0x0004,
        DirtyFont               = 0x0008,
        DirtyBackground         = 0x0010,
        DirtyBackgroundMode     = 0x0020,
        DirtyTransform          = 0x0040,
        DirtyClipRegion         = 0x0080,
        DirtyClipPath           = 0x0100,
        DirtyHints              = 0x0200,
        DirtyCompositionMode    = 0x0400,
        DirtyClipEnabled        = 0x0800,
        DirtyOpacity            = 0x1000,

        AllDirty                = 0xffff
    };
    Q_DECLARE_FLAGS(DirtyFlags, DirtyFlag)

    enum PolygonDrawMode {
        OddEvenMode,
        WindingMode,
        ConvexMode,
        PolylineMode
    };

    explicit QPaintEngine(PaintEngineFeatures features=0);
    virtual ~QPaintEngine();

    bool isActive() const { return active; }
    void setActive(bool newState) { active = newState; }

    virtual bool begin(QPaintDevice *pdev) = 0;
    virtual bool end() = 0;

    virtual void updateState(const QPaintEngineState &state) = 0;

    virtual void drawRects(const QRect *rects, int rectCount);
    virtual void drawRects(const QRectF *rects, int rectCount);

    virtual void drawLines(const QLine *lines, int lineCount);
    virtual void drawLines(const QLineF *lines, int lineCount);

    virtual void drawEllipse(const QRectF &r);
    virtual void drawEllipse(const QRect &r);

    virtual void drawPath(const QPainterPath &path);

    virtual void drawPoints(const QPointF *points, int pointCount);
    virtual void drawPoints(const QPoint *points, int pointCount);

    virtual void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    virtual void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);

    virtual void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) = 0;
    virtual void drawTextItem(const QPointF &p, const QTextItem &textItem);
    virtual void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);
    virtual void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                           Qt::ImageConversionFlags flags = Qt::AutoColor);

    void setPaintDevice(QPaintDevice *device);
    QPaintDevice *paintDevice() const;

    void setSystemClip(const QRegion &baseClip);
    QRegion systemClip() const;

    void setSystemRect(const QRect &rect);
    QRect systemRect() const;

    enum Type {
        X11,
        PostScript,
        SVG,
        Raster,
        Pdf,

        User = 50,    // first user type id
        MaxUser = 100 // last user type id
    };
    virtual Type type() const = 0;

    inline bool testDirty(DirtyFlags df);
    inline void setDirty(DirtyFlags df);
    inline void clearDirty(DirtyFlags df);

    bool hasFeature(PaintEngineFeatures feature) const { return (gccaps & feature) != 0; }

    QPainter *painter() const;

    inline bool isExtended() const { return extended; }

protected:
    QPaintEngine(QPaintEnginePrivate &data, PaintEngineFeatures devcaps=0);

    QPaintEngineState *state;
    PaintEngineFeatures gccaps;

    bool active;
    bool extended;

    QScopedPointer<QPaintEnginePrivate> d_ptr;

private:
    Q_DISABLE_COPY(QPaintEngine)

    friend class QFontEngineBox;
    friend class QFontEngineFT;
    friend class QPSPrintEngine;
    friend class QPainter;
    friend class QPainterPrivate;
    friend class QWidget;
    friend class QWidgetPrivate;
    friend class QPreviewPaintEngine;
};


class Q_GUI_EXPORT QPaintEngineState
{
public:
    QPaintEngine::DirtyFlags state() const { return dirtyFlags; }

    QPen pen() const;
    QBrush brush() const;
    QPointF brushOrigin() const;
    QBrush backgroundBrush() const;
    Qt::BGMode backgroundMode() const;
    QFont font() const;
    QMatrix matrix() const;
    QTransform transform() const;

    Qt::ClipOperation clipOperation() const;
    QRegion clipRegion() const;
    QPainterPath clipPath() const;
    bool isClipEnabled() const;

    QPainter::RenderHints renderHints() const;
    QPainter::CompositionMode compositionMode() const;
    qreal opacity() const;

    QPainter *painter() const;

    bool brushNeedsResolving() const;
    bool penNeedsResolving() const;

protected:
    friend class QPaintEngine;
    friend class QRasterPaintEngine;
    friend class QWidget;
    friend class QPainter;
    friend class QPainterPrivate;

    QPaintEngine::DirtyFlags dirtyFlags;
};

//
// inline functions
//
inline bool QPaintEngine::testDirty(DirtyFlags df) {
    Q_ASSERT(state);
    return ((state->dirtyFlags & df) != 0);
}

inline void QPaintEngine::setDirty(DirtyFlags df) {
    Q_ASSERT(state);
    state->dirtyFlags |= df;
}

inline void QPaintEngine::clearDirty(DirtyFlags df)
{
    Q_ASSERT(state);
    state->dirtyFlags &= ~static_cast<uint>(df);
}

Q_DECLARE_OPERATORS_FOR_FLAGS(QTextItem::RenderFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(QPaintEngine::PaintEngineFeatures)
Q_DECLARE_OPERATORS_FOR_FLAGS(QPaintEngine::DirtyFlags)

QT_END_NAMESPACE


#endif // QPAINTENGINE_H
