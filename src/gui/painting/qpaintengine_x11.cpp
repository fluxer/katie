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

#include "qplatformdefs.h"

#include "qpixmap_x11_p.h"

#include "qapplication.h"
#include "qdebug.h"
#include "qwidget.h"
#include "qbitmap.h"
#include "qpixmapcache.h"
#include "qcoreevent.h"
#include "qmath.h"
#include "qpainter_p.h"
#include "qwidget_p.h"
#include "qpainterpath_p.h"
#include "qpen.h"
#include "qcolor.h"
#include "qcolormap.h"
#include "qstylehelper_p.h"
#include "qpaintengine_p.h"
#include "qpaintengine_x11_p.h"
#include "qt_x11_p.h"
#include "qguicommon_p.h"
#include "qcorecommon_p.h"

#ifndef QT_NO_XRENDER
#include "qtessellator_p.h"
#endif

#include <limits.h>

QT_BEGIN_NAMESPACE

extern Qt::HANDLE qt_x11Handle(const QPaintDevice *pd);
extern const QX11Info *qt_x11Info(const QPaintDevice *pd);
extern QPixmap qt_pixmapForBrush(int brushStyle); //in qbrush.cpp
extern QPixmap qt_toX11Pixmap(const QPixmap &pixmap);

// hack, so we don't have to make QRegion::clipRectangles() public or include
// X11 headers in qregion.h
Q_GUI_EXPORT void *qt_getClipRects(const QRegion &r, int &num)
{
    return r.clipRectangles(num);
}

static inline void x11SetClipRegion(Display *dpy, GC gc, GC gc2,
#ifndef QT_NO_XRENDER
                                    Picture picture,
#else
                                    Qt::HANDLE picture,
#endif
                                    const QRegion &r)
{
    int num;
    XRectangle *rects = (XRectangle *)qt_getClipRects(r, num);

    if (gc)
        XSetClipRectangles( dpy, gc, 0, 0, rects, num, YXBanded );
    if (gc2)
        XSetClipRectangles( dpy, gc2, 0, 0, rects, num, YXBanded );

#ifndef QT_NO_XRENDER
    if (picture)
        XRenderSetPictureClipRectangles(dpy, picture, 0, 0, rects, num);
#else
    Q_UNUSED(picture);
#endif // QT_NO_XRENDER
}


static inline void x11ClearClipRegion(Display *dpy, GC gc, GC gc2,
#ifndef QT_NO_XRENDER
                                    Picture picture
#else
                                    Qt::HANDLE picture
#endif
                                      )
{
    if (gc)
        XSetClipMask(dpy, gc, XNone);
    if (gc2)
        XSetClipMask(dpy, gc2, XNone);

#ifndef QT_NO_XRENDER
    if (picture) {
        XRenderPictureAttributes attrs;
        attrs.clip_mask = XNone;
        XRenderChangePicture (dpy, picture, CPClipMask, &attrs);
    }
#else
    Q_UNUSED(picture);
#endif // QT_NO_XRENDER
}


#define DITHER_SIZE 16
static const uchar base_dither_matrix[DITHER_SIZE][DITHER_SIZE] = {
  {   0,192, 48,240, 12,204, 60,252,  3,195, 51,243, 15,207, 63,255 },
  { 128, 64,176,112,140, 76,188,124,131, 67,179,115,143, 79,191,127 },
  {  32,224, 16,208, 44,236, 28,220, 35,227, 19,211, 47,239, 31,223 },
  { 160, 96,144, 80,172,108,156, 92,163, 99,147, 83,175,111,159, 95 },
  {   8,200, 56,248,  4,196, 52,244, 11,203, 59,251,  7,199, 55,247 },
  { 136, 72,184,120,132, 68,180,116,139, 75,187,123,135, 71,183,119 },
  {  40,232, 24,216, 36,228, 20,212, 43,235, 27,219, 39,231, 23,215 },
  { 168,104,152, 88,164,100,148, 84,171,107,155, 91,167,103,151, 87 },
  {   2,194, 50,242, 14,206, 62,254,  1,193, 49,241, 13,205, 61,253 },
  { 130, 66,178,114,142, 78,190,126,129, 65,177,113,141, 77,189,125 },
  {  34,226, 18,210, 46,238, 30,222, 33,225, 17,209, 45,237, 29,221 },
  { 162, 98,146, 82,174,110,158, 94,161, 97,145, 81,173,109,157, 93 },
  {  10,202, 58,250,  6,198, 54,246,  9,201, 57,249,  5,197, 53,245 },
  { 138, 74,186,122,134, 70,182,118,137, 73,185,121,133, 69,181,117 },
  {  42,234, 26,218, 38,230, 22,214, 41,233, 25,217, 37,229, 21,213 },
  { 170,106,154, 90,166,102,150, 86,169,105,153, 89,165,101,149, 85 }
};

static QPixmap qt_patternForAlpha(uchar alpha, int screen)
{
    QPixmap pm;
    QString key = QLatin1String("$qt-alpha-brush$")
                  + HexString<uchar>(alpha)
                  + HexString<int>(screen);

    if (!QPixmapCache::find(key, pm)) {
        // #### why not use a mono image here????
        QImage pattern(DITHER_SIZE, DITHER_SIZE, QImage::Format_ARGB32);
        pattern.fill(0xffffffff);
        for (int y = 0; y < DITHER_SIZE; ++y) {
            for (int x = 0; x < DITHER_SIZE; ++x) {
                if (base_dither_matrix[x][y] <= alpha)
                    pattern.setPixel(x, y, 0x00000000);
            }
        }
        pm = QBitmap::fromImage(pattern);
        pm.x11SetScreen(screen);
        QPixmapCache::insert(key, pm);
    }
    return pm;
}

#if !defined(QT_NO_XRENDER)

class QXRenderTessellator : public QTessellator
{
public:
    QXRenderTessellator() : traps(0), allocated(0), size(0) {}
    ~QXRenderTessellator() { free(traps); }
    XTrapezoid *traps;
    int allocated;
    int size;
    void addTrap(const Trapezoid &trap);
    QRect tessellate(const QPointF *points, int nPoints, bool winding) {
        size = 0;
        setWinding(winding);
        return QTessellator::tessellate(points, nPoints).toRect();
    }
    void done() {
        if (allocated > 64) {
            free(traps);
            traps = 0;
            allocated = 0;
        }
    }
};

void QXRenderTessellator::addTrap(const Trapezoid &trap)
{
    if (size == allocated) {
        allocated = qMax(2*allocated, 64);
        traps = (XTrapezoid *)::realloc(traps, allocated * sizeof(XTrapezoid));
        Q_CHECK_PTR(traps);
    }
    traps[size].top = Q27Dot5ToXFixed(trap.top);
    traps[size].bottom = Q27Dot5ToXFixed(trap.bottom);
    traps[size].left.p1.x = Q27Dot5ToXFixed(trap.topLeft->x);
    traps[size].left.p1.y = Q27Dot5ToXFixed(trap.topLeft->y);
    traps[size].left.p2.x = Q27Dot5ToXFixed(trap.bottomLeft->x);
    traps[size].left.p2.y = Q27Dot5ToXFixed(trap.bottomLeft->y);
    traps[size].right.p1.x = Q27Dot5ToXFixed(trap.topRight->x);
    traps[size].right.p1.y = Q27Dot5ToXFixed(trap.topRight->y);
    traps[size].right.p2.x = Q27Dot5ToXFixed(trap.bottomRight->x);
    traps[size].right.p2.y = Q27Dot5ToXFixed(trap.bottomRight->y);
    ++size;
}

static Picture getPatternFill(int screen, const QBrush &b)
{
    if (!qt_x11Data->use_xrender)
        return XNone;

    XRenderColor color = qt_x11Data->preMultiply(b.color());
    XRenderColor bg_color = qt_x11Data->preMultiply(QColor(0, 0, 0, 0));

    for (int i = 0; i < QX11Data::pattern_fill_count; ++i) {
        if (qt_x11Data->pattern_fills[i].screen == screen
            && qt_x11Data->pattern_fills[i].style == b.style()
            && qt_x11Data->pattern_fills[i].color.alpha == color.alpha
            && qt_x11Data->pattern_fills[i].color.red == color.red
            && qt_x11Data->pattern_fills[i].color.green == color.green
            && qt_x11Data->pattern_fills[i].color.blue == color.blue
            && qt_x11Data->pattern_fills[i].bg_color.alpha == bg_color.alpha
            && qt_x11Data->pattern_fills[i].bg_color.red == bg_color.red
            && qt_x11Data->pattern_fills[i].bg_color.green == bg_color.green
            && qt_x11Data->pattern_fills[i].bg_color.blue == bg_color.blue)
            return qt_x11Data->pattern_fills[i].picture;
    }
    // none found, replace one
    int i = qrand() % QX11Data::pattern_fill_count;

    if (qt_x11Data->pattern_fills[i].screen != screen && qt_x11Data->pattern_fills[i].picture) {
        XRenderFreePicture (qt_x11Data->display, qt_x11Data->pattern_fills[i].picture);
        qt_x11Data->pattern_fills[i].picture = 0;
    }

    if (!qt_x11Data->pattern_fills[i].picture) {
        Pixmap pixmap = XCreatePixmap (qt_x11Data->display, RootWindow (qt_x11Data->display, screen), 8, 8, 32);
        XRenderPictureAttributes attrs;
        attrs.repeat = True;
        qt_x11Data->pattern_fills[i].picture = XRenderCreatePicture (qt_x11Data->display, pixmap,
                                                              XRenderFindStandardFormat(qt_x11Data->display, PictStandardARGB32),
                                                              CPRepeat, &attrs);
        XFreePixmap (qt_x11Data->display, pixmap);
    }

    qt_x11Data->pattern_fills[i].screen = screen;
    qt_x11Data->pattern_fills[i].color = color;
    qt_x11Data->pattern_fills[i].bg_color = bg_color;
    qt_x11Data->pattern_fills[i].style = b.style();

    XRenderFillRectangle(qt_x11Data->display, PictOpSrc, qt_x11Data->pattern_fills[i].picture, &bg_color, 0, 0, 8, 8);

    QPixmap pattern(qt_toX11Pixmap(qt_pixmapForBrush(b.style())));
    XRenderPictureAttributes attrs;
    attrs.repeat = true;
    XRenderChangePicture(qt_x11Data->display, pattern.x11PictureHandle(), CPRepeat, &attrs);

    Picture fill_fg = qt_x11Data->getSolidFill(screen, b.color());
    XRenderComposite(qt_x11Data->display, PictOpOver, fill_fg, pattern.x11PictureHandle(),
                     qt_x11Data->pattern_fills[i].picture,
                     0, 0, 0, 0, 0, 0, 8, 8);

    return qt_x11Data->pattern_fills[i].picture;
}

static void qt_render_bitmap(Display *dpy, int scrn, Picture src, Picture dst,
                      int sx, int sy, int x, int y, int sw, int sh,
                      const QPen &pen)
{
    Picture fill_fg = qt_x11Data->getSolidFill(scrn, pen.color());
    XRenderComposite(dpy, PictOpOver,
                     fill_fg, src, dst, sx, sy, sx, sy, x, y, sw, sh);
}
#endif // QT_NO_XRENDER

void QX11PaintEnginePrivate::init()
{
    dpy = 0;
    scrn = 0;
    hd = 0;
    picture = 0;
    xinfo = 0;
#ifndef QT_NO_XRENDER
    current_brush = 0;
    composition_mode = PictOpOver;
    tessellator = new QXRenderTessellator;
#endif
}

void QX11PaintEnginePrivate::setupAdaptedOrigin(const QPoint &p)
{
    if (adapted_brush_origin)
        XSetTSOrigin(dpy, gc_brush, p.x(), p.y());
}

void QX11PaintEnginePrivate::resetAdaptedOrigin()
{
    if (adapted_brush_origin)
        XSetTSOrigin(dpy, gc_brush, 0, 0);
}

void QX11PaintEnginePrivate::clipPolygon_dev(const QPolygonF &poly, QPolygonF *clipped_poly)
{
    int clipped_count = 0;
    QPointF *clipped_points = 0;
    polygonClipper.clipPolygon(poly.data(), poly.size(),
                               &clipped_points, &clipped_count);
    clipped_poly->resize(clipped_count);
    for (int i=0; i<clipped_count; ++i)
        (*clipped_poly)[i] = *(&clipped_points[i]);
}

void QX11PaintEnginePrivate::systemStateChanged()
{
    Q_Q(QX11PaintEngine);
    QPainter *painter = q->state ? static_cast<QPainterState *>(q->state)->painter : 0;
    if (painter && painter->hasClipping()) {
        if (q->testDirty(QPaintEngine::DirtyTransform))
            q->updateMatrix(q->state->transform());
        QPolygonF clip_poly_dev(matrix.map(painter->clipPath().toFillPolygon()));
        QPolygonF clipped_poly_dev;
        clipPolygon_dev(clip_poly_dev, &clipped_poly_dev);
        q->updateClipRegion_dev(QRegion(clipped_poly_dev.toPolygon()), Qt::ReplaceClip);
    } else {
        q->updateClipRegion_dev(QRegion(), Qt::NoClip);
    }
}

static QPaintEngine::PaintEngineFeatures qt_decide_features()
{
    QPaintEngine::PaintEngineFeatures features =
        QPaintEngine::AlphaBlend
        | QPaintEngine::PainterPaths;

    if (qt_x11Data->use_xrender) {
        features |= QPaintEngine::Antialiasing;
        features |= QPaintEngine::PorterDuff;
    }

    return features;
}

/*
 * QX11PaintEngine members
 */

QX11PaintEngine::QX11PaintEngine()
    : QPaintEngine(*(new QX11PaintEnginePrivate), qt_decide_features())
{
    d_func()->init();
}

QX11PaintEngine::QX11PaintEngine(QX11PaintEnginePrivate &dptr)
    : QPaintEngine(dptr, qt_decide_features())
{
    d_func()->init();
}

QX11PaintEngine::~QX11PaintEngine()
{
#ifndef QT_NO_XRENDER
    Q_D(QX11PaintEngine);
    delete d->tessellator;
#endif
}

bool QX11PaintEngine::begin(QPaintDevice *pdev)
{
    Q_D(QX11PaintEngine);
    d->xinfo = qt_x11Info(pdev);
    QWidget *w = d->pdev->devType() == QInternal::Widget ? static_cast<QWidget *>(d->pdev) : 0;
    const bool isAlienWidget = w && !w->internalWinId() && w->testAttribute(Qt::WA_WState_Created);
#ifndef QT_NO_XRENDER
    if (w) {
        if (isAlienWidget)
            d->picture = (::Picture)w->nativeParentWidget()->x11PictureHandle();
        else
            d->picture = (::Picture)w->x11PictureHandle();
    } else if (pdev->devType() == QInternal::Pixmap) {
        const QPixmap *pm = static_cast<const QPixmap *>(pdev);
        QX11PixmapData *data = static_cast<QX11PixmapData*>(pm->data.data());
        if (qt_x11Data->use_xrender && data->depth() != 32 && data->x11_mask)
            data->convertToARGB32();
        d->picture = (::Picture)static_cast<const QPixmap *>(pdev)->x11PictureHandle();
    }
#else
    d->picture = 0;
#endif
    d->hd = !isAlienWidget ? qt_x11Handle(pdev) : qt_x11Handle(w->nativeParentWidget());

    Q_ASSERT(d->xinfo != 0);
    d->dpy = d->xinfo->display(); // get display variable
    d->scrn = d->xinfo->screen(); // get screen variable

    d->crgn = QRegion();
    d->gc = XCreateGC(d->dpy, d->hd, 0, 0);
    d->gc_brush = XCreateGC(d->dpy, d->hd, 0, 0);
    d->has_alpha_brush = false;
    d->has_alpha_pen = false;
    d->has_clipping = false;
    d->has_scaling_xform = false;
    d->xform_scale = 1;
    d->matrix = QTransform();
    d->pdev_depth = d->pdev->depth();
    d->render_hints = 0;
    d->txop = QTransform::TxNone;
#if !defined(QT_NO_XRENDER)
    d->composition_mode = PictOpOver;
#endif
    d->opacity = 1;

    // Set up the polygon clipper. Note: This will only work in
    // polyline mode as long as we have a buffer zone, since a
    // polyline may be clipped into several non-connected polylines.
    const int BUFFERZONE = 1000;
    QRect devClipRect(-BUFFERZONE, -BUFFERZONE,
                      pdev->width() + 2*BUFFERZONE, pdev->height() + 2*BUFFERZONE);
    d->polygonClipper.setBoundingRect(devClipRect);

    if (isAlienWidget) {
        // Set system clip for alien widgets painting outside the paint event.
        // This is not a problem with native windows since the windowing system
        // will handle the clip.
        QWidgetPrivate *wd = w->d_func();
        QRegion widgetClip(wd->clipRect());
        wd->clipToEffectiveMask(widgetClip);
        wd->subtractOpaqueSiblings(widgetClip);
        widgetClip.translate(w->mapTo(w->nativeParentWidget(), QPoint()));
        setSystemClip(widgetClip);
    }

    QPixmap::x11SetDefaultScreen(d->xinfo->screen());

    if (w && w->testAttribute(Qt::WA_PaintUnclipped)) {  // paint direct on device
        updatePen(QPen(Qt::black));
        updateBrush(QBrush(Qt::white), QPoint());
        XSetSubwindowMode(d->dpy, d->gc, IncludeInferiors);
        XSetSubwindowMode(d->dpy, d->gc_brush, IncludeInferiors);
#ifndef QT_NO_XRENDER
        XRenderPictureAttributes attrs;
        attrs.subwindow_mode = IncludeInferiors;
        XRenderChangePicture(d->dpy, d->picture, CPSubwindowMode, &attrs);
#endif
    }

    setDirty(QPaintEngine::DirtyClipRegion);
    setDirty(QPaintEngine::DirtyPen);
    setDirty(QPaintEngine::DirtyBrush);
    setDirty(QPaintEngine::DirtyBackground);

    return true;
}

bool QX11PaintEngine::end()
{
    Q_D(QX11PaintEngine);

#if !defined(QT_NO_XRENDER)
    if (d->picture) {
        // reset clipping/subwindow mode on our render picture
        XRenderPictureAttributes attrs;
        attrs.subwindow_mode = ClipByChildren;
        attrs.clip_mask = XNone;
        XRenderChangePicture(d->dpy, d->picture, CPClipMask|CPSubwindowMode, &attrs);
    }
#endif

    if (d->gc_brush && d->pdev->painters < 2) {
        XFreeGC(d->dpy, d->gc_brush);
        d->gc_brush = 0;
    }

    if (d->gc && d->pdev->painters < 2) {
        XFreeGC(d->dpy, d->gc);
        d->gc = 0;
    }

    // Restore system clip for alien widgets painting outside the paint event.
    if (d->pdev->devType() == QInternal::Widget && !static_cast<QWidget *>(d->pdev)->internalWinId())
        setSystemClip(QRegion());

    return true;
}

void QX11PaintEngine::updateState(const QPaintEngineState &state)
{
    Q_D(QX11PaintEngine);
    QPaintEngine::DirtyFlags flags = state.state();


    if (flags & DirtyOpacity) {
        d->opacity = state.opacity();
        // Force update pen/brush as to get proper alpha colors propagated
        flags |= DirtyPen;
        flags |= DirtyBrush;
    }

    if (flags & DirtyTransform) updateMatrix(state.transform());
    if (flags & DirtyPen) updatePen(state.pen());
    if (flags & (DirtyBrush | DirtyBrushOrigin)) updateBrush(state.brush(), state.brushOrigin());

    if (state.state() & DirtyClipEnabled) {
        if (state.isClipEnabled()) {
            QPolygonF clip_poly_dev(d->matrix.map(painter()->clipPath().toFillPolygon()));
            QPolygonF clipped_poly_dev;
            d->clipPolygon_dev(clip_poly_dev, &clipped_poly_dev);
            updateClipRegion_dev(QRegion(clipped_poly_dev.toPolygon()), Qt::ReplaceClip);
        } else {
            updateClipRegion_dev(QRegion(), Qt::NoClip);
        }
    }

    if (flags & DirtyClipPath) {
        QPolygonF clip_poly_dev(d->matrix.map(state.clipPath().toFillPolygon()));
        QPolygonF clipped_poly_dev;
        d->clipPolygon_dev(clip_poly_dev, &clipped_poly_dev);
        updateClipRegion_dev(QRegion(clipped_poly_dev.toPolygon(), state.clipPath().fillRule()),
                             state.clipOperation());
    } else if (flags & DirtyClipRegion) {
        extern QPainterPath qt_regionToPath(const QRegion &region);
        QPainterPath clip_path = qt_regionToPath(state.clipRegion());
        QPolygonF clip_poly_dev(d->matrix.map(clip_path.toFillPolygon()));
        QPolygonF clipped_poly_dev;
        d->clipPolygon_dev(clip_poly_dev, &clipped_poly_dev);
        updateClipRegion_dev(QRegion(clipped_poly_dev.toPolygon()), state.clipOperation());
    }
    if (flags & DirtyHints) updateRenderHints(state.renderHints());
    if (flags & DirtyCompositionMode) {
        int function = GXcopy;
        switch (state.compositionMode()) {
#if !defined(QT_NO_XRENDER)
            case QPainter::CompositionMode_SourceOver:
                d->composition_mode = PictOpOver;
                break;
            case QPainter::CompositionMode_DestinationOver:
                d->composition_mode = PictOpOverReverse;
                break;
            case QPainter::CompositionMode_Clear:
                d->composition_mode = PictOpClear;
                break;
            case QPainter::CompositionMode_Source:
                d->composition_mode = PictOpSrc;
                break;
            case QPainter::CompositionMode_Destination:
                d->composition_mode = PictOpDst;
                break;
            case QPainter::CompositionMode_SourceIn:
                d->composition_mode = PictOpIn;
                break;
            case QPainter::CompositionMode_DestinationIn:
                d->composition_mode = PictOpInReverse;
                break;
            case QPainter::CompositionMode_SourceOut:
                d->composition_mode = PictOpOut;
                break;
            case QPainter::CompositionMode_DestinationOut:
                d->composition_mode = PictOpOutReverse;
                break;
            case QPainter::CompositionMode_SourceAtop:
                d->composition_mode = PictOpAtop;
                break;
            case QPainter::CompositionMode_DestinationAtop:
                d->composition_mode = PictOpAtopReverse;
                break;
            case QPainter::CompositionMode_Xor:
                d->composition_mode = PictOpXor;
                break;
#endif
            default:
                break;
        }
        XSetFunction(qt_x11Data->display, d->gc, function);
        XSetFunction(qt_x11Data->display, d->gc_brush, function);
    }
    d->decideCoordAdjust();
}

void QX11PaintEngine::updateRenderHints(QPainter::RenderHints hints)
{
    Q_D(QX11PaintEngine);
    d->render_hints = hints;

#if !defined(QT_NO_XRENDER)
    if (qt_x11Data->use_xrender && d->picture) {
        XRenderPictureAttributes attrs;
        attrs.poly_edge = (hints & QPainter::Antialiasing) ? PolyEdgeSmooth : PolyEdgeSharp;
        XRenderChangePicture(d->dpy, d->picture, CPPolyEdge, &attrs);
    }
#endif
}

void QX11PaintEngine::updatePen(const QPen &pen)
{
    Q_D(QX11PaintEngine);
    d->cpen = pen;
    int cp = CapButt;
    int jn = JoinMiter;
    int ps = pen.style();

    if (d->opacity < 1.0) {
        QColor c = d->cpen.color();
        c.setAlpha(qRound(c.alpha()*d->opacity));
        d->cpen.setColor(c);
    }

    d->has_pen = (ps != Qt::NoPen);
    d->has_alpha_pen = (pen.color().alpha() != 255);

    switch (pen.capStyle()) {
    case Qt::SquareCap:
        cp = CapProjecting;
        break;
    case Qt::RoundCap:
        cp = CapRound;
        break;
    case Qt::FlatCap:
    default:
        cp = CapButt;
        break;
    }
    switch (pen.joinStyle()) {
    case Qt::BevelJoin:
        jn = JoinBevel;
        break;
    case Qt::RoundJoin:
        jn = JoinRound;
        break;
    case Qt::MiterJoin:
    default:
        jn = JoinMiter;
        break;
    }

    QSTACKARRAY(char, dashes, 6);               // custom pen dashes
    int dash_len = 0;                           // length of dash list
    int xStyle = LineSolid;

    /*
      We are emulating Windows here.  Windows treats cpen.width() == 1
      (or 0) as a very special case.  The fudge variable unifies this
      case with the general case.
    */
    qreal pen_width = pen.widthF();
    int scale =  qRound(pen_width < 1 ? 1 : pen_width);
    int space = (pen_width < 1 && pen_width > 0 ? 1 : (2 * scale));
    int dot = 1 * scale;
    int dash = 4 * scale;

    switch (ps) {
    case Qt::NoPen:
    case Qt::SolidLine:
        xStyle = LineSolid;
        break;
    case Qt::DashLine:
        dashes[0] = dash;
        dashes[1] = space;
        dash_len = 2;
        xStyle = LineOnOffDash;
        break;
    case Qt::DotLine:
        dashes[0] = dot;
        dashes[1] = space;
        dash_len = 2;
        xStyle = LineOnOffDash;
        break;
    case Qt::DashDotLine:
        dashes[0] = dash;
        dashes[1] = space;
        dashes[2] = dot;
        dashes[3] = space;
        dash_len = 4;
        xStyle = LineOnOffDash;
        break;
    case Qt::DashDotDotLine:
        dashes[0] = dash;
        dashes[1] = space;
        dashes[2] = dot;
        dashes[3] = space;
        dashes[4] = dot;
        dashes[5] = space;
        dash_len = 6;
        xStyle = LineOnOffDash;
        break;
    case Qt::CustomDashLine:
        break;
    }

    ulong mask = GCForeground | GCBackground | GCGraphicsExposures | GCLineWidth
                 | GCCapStyle | GCJoinStyle | GCLineStyle;
    XGCValues vals;
    vals.graphics_exposures = false;
    if (d->pdev_depth == 1) {
        vals.foreground = qGray(pen.color().rgb()) > 127 ? 0 : 1;
        vals.background = qGray(qt_transparentrgb) > 127 ? 0 : 1;
    } else if (d->pdev->devType() == QInternal::Pixmap && d->pdev_depth == 32
        && qt_x11Data->use_xrender) {
        vals.foreground = pen.color().rgba();
        vals.background = qt_transparentrgba;
    } else {
        QColormap cmap = QColormap::instance(d->scrn);
        vals.foreground = cmap.pixel(pen.color());
        vals.background = cmap.pixel(QColor(Qt::transparent));
    }


    vals.line_width = qRound(pen_width);
    vals.cap_style = cp;
    vals.join_style = jn;
    vals.line_style = xStyle;

    XChangeGC(d->dpy, d->gc, mask, &vals);

    if (dash_len) { // make dash list
        XSetDashes(d->dpy, d->gc, 0, dashes, dash_len);
    }

    if (!d->has_clipping) { // if clipping is set the paintevent clip region is merged with the clip region
        QRegion sysClip = systemClip();
        if (!sysClip.isEmpty())
            x11SetClipRegion(d->dpy, d->gc, 0, d->picture, sysClip);
        else
            x11ClearClipRegion(d->dpy, d->gc, 0, d->picture);
    }
}

void QX11PaintEngine::updateBrush(const QBrush &brush, const QPointF &origin)
{
    Q_D(QX11PaintEngine);
    d->cbrush = brush;
    d->bg_origin = origin;
    d->adapted_brush_origin = false;
#if !defined(QT_NO_XRENDER)
    d->current_brush = 0;
#endif
    if (d->opacity < 1.0) {
        QColor c = d->cbrush.color();
        c.setAlpha(qRound(c.alpha()*d->opacity));
        d->cbrush.setColor(c);
    }

    int s  = FillSolid;
    int  bs = d->cbrush.style();
    d->has_brush = (bs != Qt::NoBrush);
    d->has_pattern = bs >= Qt::Dense1Pattern && bs <= Qt::DiagCrossPattern;
    d->has_texture = bs == Qt::TexturePattern;
    d->has_alpha_brush = brush.color().alpha() != 255;

    ulong mask = GCForeground | GCBackground | GCGraphicsExposures
                 | GCLineStyle | GCCapStyle | GCJoinStyle | GCFillStyle;
    XGCValues vals;
    vals.graphics_exposures = false;
    if (d->pdev_depth == 1) {
        vals.foreground = qGray(d->cbrush.color().rgb()) > 127 ? 0 : 1;
        vals.background = qGray(qt_transparentrgb) > 127 ? 0 : 1;
    } else if (qt_x11Data->use_xrender && d->pdev->devType() == QInternal::Pixmap
               && d->pdev_depth == 32) {
        vals.foreground = d->cbrush.color().rgba();
        vals.background = qt_transparentrgba;
    } else {
        QColormap cmap = QColormap::instance(d->scrn);
        vals.foreground = cmap.pixel(d->cbrush.color());
        vals.background = cmap.pixel(QColor(Qt::transparent));

        if (!qt_x11Data->use_xrender && d->has_brush && !d->has_pattern && !brush.isOpaque()) {
            QPixmap pattern = qt_patternForAlpha(brush.color().alpha(), d->scrn);
            mask |= GCStipple;
            vals.stipple = pattern.handle();
            s = FillStippled;
            d->adapted_brush_origin = true;
        }
    }
    vals.cap_style = CapButt;
    vals.join_style = JoinMiter;
    vals.line_style = LineSolid;

    if (d->has_pattern || d->has_texture) {
        if (bs == Qt::TexturePattern) {
            d->brush_pm = qt_toX11Pixmap(d->cbrush.texture());
#if !defined(QT_NO_XRENDER)
            if (qt_x11Data->use_xrender) {
                XRenderPictureAttributes attrs;
                attrs.repeat = true;
                XRenderChangePicture(d->dpy, d->brush_pm.x11PictureHandle(), CPRepeat, &attrs);
                QX11PixmapData *data = static_cast<QX11PixmapData*>(d->brush_pm.data.data());
                if (data->mask_picture)
                    XRenderChangePicture(d->dpy, data->mask_picture, CPRepeat, &attrs);
            }
#endif
        } else {
            d->brush_pm = qt_toX11Pixmap(qt_pixmapForBrush(bs));
        }
        d->brush_pm.x11SetScreen(d->scrn);
        if (d->brush_pm.depth() == 1) {
            mask |= GCStipple;
            vals.stipple = d->brush_pm.handle();
            s = FillStippled;
#if !defined(QT_NO_XRENDER)
            if (qt_x11Data->use_xrender) {
                d->bitmap_texture = QPixmap(d->brush_pm.size());
                d->bitmap_texture.fill(Qt::transparent);
                d->bitmap_texture = qt_toX11Pixmap(d->bitmap_texture);
                d->bitmap_texture.x11SetScreen(d->scrn);

                ::Picture src  = qt_x11Data->getSolidFill(d->scrn, d->cbrush.color());
                XRenderComposite(d->dpy, PictOpSrc, src, d->brush_pm.x11PictureHandle(),
                                 d->bitmap_texture.x11PictureHandle(),
                                 0, 0, d->brush_pm.width(), d->brush_pm.height(),
                                 0, 0, d->brush_pm.width(), d->brush_pm.height());

                XRenderPictureAttributes attrs;
                attrs.repeat = true;
                XRenderChangePicture(d->dpy, d->bitmap_texture.x11PictureHandle(), CPRepeat, &attrs);

                d->current_brush = d->bitmap_texture.x11PictureHandle();
            }
#endif
        } else {
            mask |= GCTile;
#ifndef QT_NO_XRENDER
            if (d->pdev_depth == 32 && d->brush_pm.depth() != 32) {
                d->brush_pm.detach();
                QX11PixmapData *brushData = static_cast<QX11PixmapData*>(d->brush_pm.data.data());
                brushData->convertToARGB32();
            }
#endif
            vals.tile = d->brush_pm.handle();
            s = FillTiled;
#if !defined(QT_NO_XRENDER)
            d->current_brush = d->cbrush.texture().x11PictureHandle();
#endif
        }

        mask |= GCTileStipXOrigin | GCTileStipYOrigin;
        vals.ts_x_origin = qRound(origin.x());
        vals.ts_y_origin = qRound(origin.y());
    }
#if !defined(QT_NO_XRENDER)
    else if (d->has_alpha_brush) {
        d->current_brush = qt_x11Data->getSolidFill(d->scrn, d->cbrush.color());
    }
#endif

    vals.fill_style = s;
    XChangeGC(d->dpy, d->gc_brush, mask, &vals);
    if (!d->has_clipping) {
        QRegion sysClip = systemClip();
        if (!sysClip.isEmpty())
            x11SetClipRegion(d->dpy, d->gc_brush, 0, d->picture, sysClip);
        else
            x11ClearClipRegion(d->dpy, d->gc_brush, 0, d->picture);
    }
}

#ifndef QT_NO_XRENDER
static void qt_XRenderCompositeTrapezoids(Display *dpy,
                                          int op,
                                          Picture src,
                                          Picture dst,
                                          const XRenderPictFormat *maskFormat,
                                          int xSrc,
                                          int ySrc,
                                          const XTrapezoid *traps, int size)
{
    static const int MAX_TRAPS = 50000;
    while (size) {
        int to_draw = size;
        if (to_draw > MAX_TRAPS)
            to_draw = MAX_TRAPS;
        XRenderCompositeTrapezoids(dpy, op, src, dst,
                                   maskFormat,
                                   xSrc, ySrc,
                                   traps, to_draw);
        size -= to_draw;
        traps += to_draw;
    }
}
#endif

void QX11PaintEnginePrivate::fillPolygon_dev(const QPointF *polygonPoints, int pointCount,
                                             QX11PaintEnginePrivate::GCMode gcMode,
                                             QPaintEngine::PolygonDrawMode mode)
{
    Q_Q(QX11PaintEngine);

    int clippedCount = 0;
    QPointF *clippedPoints = nullptr;

#ifndef QT_NO_XRENDER
    // can change if we switch to pen if gcMode != BrushGC
    bool has_fill_texture = has_texture;
    bool has_fill_pattern = has_pattern;
    ::Picture src;
#endif
    QBrush fill;
    GC fill_gc;
    if (gcMode == BrushGC) {
        fill = cbrush;
        fill_gc = gc_brush;
#ifndef QT_NO_XRENDER
        if (current_brush)
            src = current_brush;
        else
            src = qt_x11Data->getSolidFill(scrn, fill.color());
#endif
    } else {
        fill = QBrush(cpen.brush());
        fill_gc = gc;
#ifndef QT_NO_XRENDER
        //we use the pens brush
        has_fill_texture = (fill.style() == Qt::TexturePattern);
        has_fill_pattern = (fill.style() >= Qt::Dense1Pattern && fill.style() <= Qt::DiagCrossPattern);
        if (has_fill_texture)
            src = fill.texture().x11PictureHandle();
        else if (has_fill_pattern)
            src = getPatternFill(scrn, fill);
        else
            src = qt_x11Data->getSolidFill(scrn, fill.color());
#endif
    }

    polygonClipper.clipPolygon(polygonPoints, pointCount,
                               &clippedPoints, &clippedCount);

#ifndef QT_NO_XRENDER
    bool solid_fill = fill.color().alpha() == 255;
    if (has_fill_texture && fill.texture().depth() == 1 && solid_fill) {
        has_fill_texture = false;
        has_fill_pattern = true;
    }

    bool antialias = render_hints & QPainter::Antialiasing;

    if (qt_x11Data->use_xrender
        && picture
        && !has_fill_pattern
        && (clippedCount > 0)
        && (fill.style() != Qt::NoBrush)
        && ((has_fill_texture && fill.texture().hasAlpha()) || antialias || !solid_fill || has_alpha_pen != has_alpha_brush))
    {
        tessellator->tessellate(clippedPoints, clippedCount,
                                mode == QPaintEngine::WindingMode);
        if (tessellator->size > 0) {
            XRenderPictureAttributes attrs;
            attrs.poly_edge = antialias ? PolyEdgeSmooth : PolyEdgeSharp;
            XRenderChangePicture(dpy, picture, CPPolyEdge, &attrs);
            int x_offset = int(XFixedToDouble(tessellator->traps[0].left.p1.x) - bg_origin.x());
            int y_offset = int(XFixedToDouble(tessellator->traps[0].left.p1.y) - bg_origin.y());
            qt_XRenderCompositeTrapezoids(dpy, composition_mode, src, picture,
                                          antialias
                                          ? XRenderFindStandardFormat(dpy, PictStandardA8)
                                          : XRenderFindStandardFormat(dpy, PictStandardA1),
                                          x_offset, y_offset,
                                          tessellator->traps, tessellator->size);
            tessellator->done();
        }
    } else
#endif
        if (fill.style() != Qt::NoBrush) {
            if (clippedCount > 200000) {
                QPolygon poly;
                for (int i = 0; i < clippedCount; ++i)
                    poly << QPoint(qFloor(clippedPoints[i].x()), qFloor(clippedPoints[i].y()));

                const QRect bounds = poly.boundingRect();
                const QRect aligned = bounds
                    & QRect(QPoint(), QSize(pdev->width(), pdev->height()));

                QImage img(aligned.size(), QImage::Format_ARGB32_Premultiplied);
                img.fill(0);

                QPainter painter(&img);
                painter.translate(-aligned.x(), -aligned.y());
                painter.setPen(Qt::NoPen);
                painter.setBrush(fill);
                if (gcMode == BrushGC)
                    painter.setBrushOrigin(q->painter()->brushOrigin());
                painter.drawPolygon(poly);
                painter.end();

                q->drawImage(aligned, img, img.rect(), Qt::AutoColor);
            } else if (clippedCount > 0) {
                XPoint xpoints[clippedCount];
                for (int i = 0; i < clippedCount; ++i) {
                    xpoints[i].x = qFloor(clippedPoints[i].x());
                    xpoints[i].y = qFloor(clippedPoints[i].y());
                }
                if (mode == QPaintEngine::WindingMode)
                    XSetFillRule(dpy, fill_gc, WindingRule);
                setupAdaptedOrigin(QPoint(xpoints[0].x, xpoints[0].y));
                XFillPolygon(dpy, hd, fill_gc,
                             xpoints, clippedCount,
                             mode == QPaintEngine::ConvexMode ? Convex : Complex, CoordModeOrigin);
                resetAdaptedOrigin();
                if (mode == QPaintEngine::WindingMode)
                    XSetFillRule(dpy, fill_gc, EvenOddRule);
            }
        }
}

void QX11PaintEnginePrivate::strokePolygon_dev(const QPointF *polygonPoints, int pointCount, bool close)
{
    // a safe number used to avoid, call to XMaxRequestSize(d->dpy) - 3;
    static const int xlibMaxLinePoints = 32762;

    int clippedCount = 0;
    QPointF *clippedPoints = nullptr;
    polygonClipper.clipPolygon(polygonPoints, pointCount,
                               &clippedPoints, &clippedCount, close);

    if (clippedCount > 0) {
        XPoint xpoints[clippedCount];
        for (int i = 0; i < clippedCount; ++i) {
            xpoints[i].x = qRound(clippedPoints[i].x() + aliasedCoordinateDelta);
            xpoints[i].y = qRound(clippedPoints[i].y() + aliasedCoordinateDelta);
        }
        uint numberPoints = qMin(clippedCount, xlibMaxLinePoints);
        XDrawLines(dpy, hd, gc, xpoints, numberPoints, CoordModeOrigin);
        XPoint *pts = xpoints + numberPoints;
        clippedCount -= numberPoints;
        numberPoints = qMin(clippedCount, xlibMaxLinePoints-1);
        while (clippedCount) {
            XDrawLines(dpy, hd, gc, pts-1, numberPoints+1, CoordModeOrigin);
            pts += numberPoints;
            clippedCount -= numberPoints;
            numberPoints = qMin(clippedCount, xlibMaxLinePoints-1);
        }
    }
}

void QX11PaintEngine::drawPolygon(const QPointF *polygonPoints, int pointCount, PolygonDrawMode mode)
{
    Q_D(QX11PaintEngine);
    QPainterPath path(polygonPoints[0]);
    for (int i = 1; i < pointCount; ++i)
        path.lineTo(polygonPoints[i]);
    if (mode == PolylineMode) {
        QBrush oldBrush = d->cbrush;
        d->cbrush = QBrush(Qt::NoBrush);
        path.setFillRule(Qt::WindingFill);
        drawPath(path);
        d->cbrush = oldBrush;
    } else {
        path.setFillRule(mode == OddEvenMode ? Qt::OddEvenFill : Qt::WindingFill);
        path.closeSubpath();
        drawPath(path);
    }
}


void QX11PaintEnginePrivate::fillPath(const QPainterPath &path, QX11PaintEnginePrivate::GCMode gc_mode, bool transform)
{
    QPainterPath clippedPath;
    QPainterPath clipPath;
    clipPath.addRect(polygonClipper.boundingRect());

    if (transform)
         clippedPath = (path*matrix).intersected(clipPath);
    else
        clippedPath = path.intersected(clipPath);

    foreach (const QPolygonF &poly, clippedPath.toFillPolygons()) {
        const int translated_size = poly.size();
        QPointF translated_points[translated_size];

        for (int j = 0; j < translated_size; ++j) {
            translated_points[j] = poly.at(j);

            if (!qt_x11Data->use_xrender || adjust_coords) {
                translated_points[j].rx() = qRound(translated_points[j].rx() + aliasedCoordinateDelta) + aliasedCoordinateDelta;
                translated_points[j].ry() = qRound(translated_points[j].ry() + aliasedCoordinateDelta) + aliasedCoordinateDelta;
            }
        }

        fillPolygon_dev(translated_points, translated_size, gc_mode,
                        path.fillRule() == Qt::OddEvenFill ? QPaintEngine::OddEvenMode : QPaintEngine::WindingMode);
    }
}

void QX11PaintEngine::drawPath(const QPainterPath &path)
{
    Q_D(QX11PaintEngine);
    if (path.isEmpty())
        return;

    if (d->has_brush)
        d->fillPath(path, QX11PaintEnginePrivate::BrushGC, true);
    if (d->has_pen
        && ((qt_x11Data->use_xrender && (d->has_alpha_pen || (d->render_hints & QPainter::Antialiasing)))
            || (!d->cpen.isCosmetic() && d->txop > QTransform::TxTranslate
                && d->has_scaling_xform)
            || (d->cpen.style() == Qt::CustomDashLine))) {
        QPainterPathStroker stroker;
        if (d->cpen.style() == Qt::CustomDashLine) {
            stroker.setDashPattern(d->cpen.dashPattern());
            stroker.setDashOffset(d->cpen.dashOffset());
        } else {
            stroker.setDashPattern(d->cpen.style());
        }
        stroker.setCapStyle(d->cpen.capStyle());
        stroker.setJoinStyle(d->cpen.joinStyle());
        QPainterPath stroke;
        qreal width = d->cpen.widthF();
        QRectF deviceRect(0, 0, d->pdev->width(), d->pdev->height());
        // necessary to get aliased alphablended primitives to be drawn correctly
        if (d->cpen.isCosmetic() || d->has_scaling_xform) {
            if (d->cpen.isCosmetic())
                stroker.setWidth(width == 0 ? 1 : width);
            else
                stroker.setWidth(width * d->xform_scale);
            stroker.d_ptr->stroker.setClipRect(deviceRect);
            stroke = stroker.createStroke(path * d->matrix);
            if (stroke.isEmpty())
                return;
            stroke.setFillRule(Qt::WindingFill);
            d->fillPath(stroke, QX11PaintEnginePrivate::PenGC, false);
        } else {
            stroker.setWidth(width);
            stroker.d_ptr->stroker.setClipRect(d->matrix.inverted().mapRect(deviceRect));
            stroke = stroker.createStroke(path);
            if (stroke.isEmpty())
                return;
            stroke.setFillRule(Qt::WindingFill);
            d->fillPath(stroke, QX11PaintEnginePrivate::PenGC, true);
        }
    } else if (d->has_pen) {
        // if we have a cosmetic pen - use XDrawLine() for speed
        QList<QPolygonF> polys = path.toSubpathPolygons(d->matrix);
        for (int i = 0; i < polys.size(); ++i)
            d->strokePolygon_dev(polys.at(i).data(), polys.at(i).size(), false);
    }
}

void QX11PaintEngine::drawPixmap(const QRectF &r, const QPixmap &px, const QRectF &sr)
{
    Q_D(QX11PaintEngine);
    int x = qRound(r.x());
    int y = qRound(r.y());
    int sx = qRound(sr.x());
    int sy = qRound(sr.y());
    int sw = qRound(sr.width());
    int sh = qRound(sr.height());

    QPixmap pixmap = qt_toX11Pixmap(px);
    if(pixmap.isNull())
        return;

    if ((d->xinfo && d->xinfo->screen() != pixmap.x11Info().screen())
        || (pixmap.x11Info().screen() != DefaultScreen(qt_x11Data->display))) {
        pixmap.x11SetScreen(d->xinfo ? d->xinfo->screen() : DefaultScreen(qt_x11Data->display));
    }

    QPixmap::x11SetDefaultScreen(pixmap.x11Info().screen());

#ifndef QT_NO_XRENDER
    ::Picture src_pict = static_cast<QX11PixmapData*>(pixmap.data.data())->picture;
    if (src_pict && d->picture) {
        const int pDepth = pixmap.depth();
        if (pDepth == 1 && (d->has_alpha_pen)) {
            qt_render_bitmap(d->dpy, d->scrn, src_pict, d->picture,
                             sx, sy, x, y, sw, sh, d->cpen);
            return;
        } else if (pDepth != 1 && (pDepth == 32 || pDepth != d->pdev_depth)) {
            XRenderComposite(d->dpy, d->composition_mode,
                             src_pict, 0, d->picture, sx, sy, 0, 0, x, y, sw, sh);
            return;
        }
    }
#endif

    bool mono_src = pixmap.depth() == 1;
    bool mono_dst = d->pdev_depth == 1;
    bool restore_clip = false;

    if (static_cast<QX11PixmapData*>(pixmap.data.data())->x11_mask) { // pixmap has a mask
        QBitmap comb(sw, sh);
        GC cgc = XCreateGC(d->dpy, comb.handle(), 0, 0);
        XSetForeground(d->dpy, cgc, 0);
        XFillRectangle(d->dpy, comb.handle(), cgc, 0, 0, sw, sh);
        XSetBackground(d->dpy, cgc, 0);
        XSetForeground(d->dpy, cgc, 1);
        if (!d->crgn.isEmpty()) {
            int num;
            XRectangle *rects = (XRectangle *)qt_getClipRects(d->crgn, num);
            XSetClipRectangles(d->dpy, cgc, -x, -y, rects, num, XUnsorted);
        } else if (d->has_clipping) {
            XSetClipRectangles(d->dpy, cgc, 0, 0, 0, 0, XUnsorted);
        }
        XSetFillStyle(d->dpy, cgc, FillOpaqueStippled);
        XSetTSOrigin(d->dpy, cgc, -sx, -sy);
        XSetStipple(d->dpy, cgc,
                    static_cast<QX11PixmapData*>(pixmap.data.data())->x11_mask);
        XFillRectangle(d->dpy, comb.handle(), cgc, 0, 0, sw, sh);
        XFreeGC(d->dpy, cgc);

        XSetClipOrigin(d->dpy, d->gc, x, y);
        XSetClipMask(d->dpy, d->gc, comb.handle());
        restore_clip = true;
    }

    if (mono_src) {
        if (!d->crgn.isEmpty()) {
            Pixmap comb = XCreatePixmap(d->dpy, d->hd, sw, sh, 1);
            GC cgc = XCreateGC(d->dpy, comb, 0, 0);
            XSetForeground(d->dpy, cgc, 0);
            XFillRectangle(d->dpy, comb, cgc, 0, 0, sw, sh);
            int num;
            XRectangle *rects = (XRectangle *)qt_getClipRects(d->crgn, num);
            XSetClipRectangles(d->dpy, cgc, -x, -y, rects, num, XUnsorted);
            XCopyArea(d->dpy, pixmap.handle(), comb, cgc, sx, sy, sw, sh, 0, 0);
            XFreeGC(d->dpy, cgc);

            XSetClipMask(d->dpy, d->gc, comb);
            XSetClipOrigin(d->dpy, d->gc, x, y);
            XFreePixmap(d->dpy, comb);
        } else {
            XSetClipMask(d->dpy, d->gc, pixmap.handle());
            XSetClipOrigin(d->dpy, d->gc, x - sx, y - sy);
        }

        if (mono_dst) {
            XSetForeground(d->dpy, d->gc, qGray(d->cpen.color().rgb()) > 127 ? 0 : 1);
        } else {
            QColormap cmap = QColormap::instance(d->scrn);
            XSetForeground(d->dpy, d->gc, cmap.pixel(d->cpen.color()));
        }
        XFillRectangle(d->dpy, d->hd, d->gc, x, y, sw, sh);
        restore_clip = true;
    } else if (mono_dst && !mono_src) {
        QBitmap bitmap(pixmap);
        XCopyArea(d->dpy, bitmap.handle(), d->hd, d->gc, sx, sy, sw, sh, x, y);
    } else {
        XCopyArea(d->dpy, pixmap.handle(), d->hd, d->gc, sx, sy, sw, sh, x, y);
    }

    if (d->pdev->devType() == QInternal::Pixmap) {
        const QPixmap *px = static_cast<const QPixmap*>(d->pdev);
        Pixmap src_mask = static_cast<QX11PixmapData*>(pixmap.data.data())->x11_mask;
        Pixmap dst_mask = static_cast<QX11PixmapData*>(px->data.data())->x11_mask;
        if (dst_mask) {
            GC cgc = XCreateGC(d->dpy, dst_mask, 0, 0);
            if (src_mask) { // copy src mask into dst mask
                XCopyArea(d->dpy, src_mask, dst_mask, cgc, sx, sy, sw, sh, x, y);
            } else { // no src mask, but make sure the area copied is opaque in dest
                XSetBackground(d->dpy, cgc, 0);
                XSetForeground(d->dpy, cgc, 1);
                XFillRectangle(d->dpy, dst_mask, cgc, x, y, sw, sh);
            }
            XFreeGC(d->dpy, cgc);
        }
    }

    if (restore_clip) {
        XSetClipOrigin(d->dpy, d->gc, 0, 0);
        int num;
        XRectangle *rects = (XRectangle *)qt_getClipRects(d->crgn, num);
        if (num == 0)
            XSetClipMask(d->dpy, d->gc, XNone);
        else
            XSetClipRectangles(d->dpy, d->gc, 0, 0, rects, num, XUnsorted);
    }
}

void QX11PaintEngine::updateMatrix(const QTransform &mtx)
{
    Q_D(QX11PaintEngine);
    d->txop = mtx.type();
    d->matrix = mtx;

    extern bool qt_scaleForTransform(const QTransform &transform, qreal *scale);
    bool scaling = qt_scaleForTransform(d->matrix, &d->xform_scale);
    d->has_scaling_xform = scaling && d->xform_scale != 1.0;
}

/*
   NB! the clip region is expected to be in dev coordinates
*/
void QX11PaintEngine::updateClipRegion_dev(const QRegion &clipRegion, Qt::ClipOperation op)
{
    Q_D(QX11PaintEngine);
    QRegion sysClip = systemClip();
    switch (op) {
    case Qt::NoClip:
        d->has_clipping = false;
        d->crgn = sysClip;
        if (!sysClip.isEmpty()) {
            x11SetClipRegion(d->dpy, d->gc, d->gc_brush, d->picture, sysClip);
        } else {
            x11ClearClipRegion(d->dpy, d->gc, d->gc_brush, d->picture);
        }
        return;
    case Qt::IntersectClip:
        if (d->has_clipping) {
            d->crgn &= clipRegion;
            break;
        }
        // fall through
    case Qt::ReplaceClip:
        if (!sysClip.isEmpty())
            d->crgn = clipRegion.intersected(sysClip);
        else
            d->crgn = clipRegion;
        break;
    case Qt::UniteClip:
        d->crgn |= clipRegion;
        if (!sysClip.isEmpty())
            d->crgn = d->crgn.intersected(sysClip);
        break;
    }
    d->has_clipping = true;
    x11SetClipRegion(d->dpy, d->gc, d->gc_brush, d->picture, d->crgn);
}

QT_END_NAMESPACE
