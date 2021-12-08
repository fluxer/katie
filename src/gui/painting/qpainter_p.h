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

#ifndef QPAINTER_P_H
#define QPAINTER_P_H

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

#include "QtGui/qbrush.h"
#include "QtGui/qfont.h"
#include "QtGui/qpen.h"
#include "QtGui/qregion.h"
#include "QtGui/qmatrix.h"
#include "QtGui/qpainter.h"
#include "QtGui/qpainterpath.h"
#include "QtGui/qpaintengine.h"
#include <QtCore/qhash.h>

QT_BEGIN_NAMESPACE

class QPaintEngine;
class QPaintEngineEx;
struct QFixedPoint;

class QPainterClipInfo
{
public:
    enum ClipType { RegionClip, PathClip, RectClip, RectFClip };

    QPainterClipInfo(const QPainterPath &p, Qt::ClipOperation op, const QTransform &m) :
        clipType(PathClip), matrix(m), operation(op), path(p) { }

    QPainterClipInfo(const QRegion &r, Qt::ClipOperation op, const QTransform &m) :
        clipType(RegionClip), matrix(m), operation(op), region(r) { }

    QPainterClipInfo(const QRect &r, Qt::ClipOperation op, const QTransform &m) :
        clipType(RectClip), matrix(m), operation(op), rect(r) { }

    QPainterClipInfo(const QRectF &r, Qt::ClipOperation op, const QTransform &m) :
        clipType(RectFClip), matrix(m), operation(op), rectf(r) { }

    ClipType clipType;
    QTransform matrix;
    Qt::ClipOperation operation;
    QPainterPath path;
    QRegion region;
    QRect rect;
    QRectF rectf;

    // ###
//     union {
//         QRegionData *d;
//         QPainterPathPrivate *pathData;

//         struct {
//             int x, y, w, h;
//         } rectData;
//         struct {
//             qreal x, y, w, h;
//         } rectFData;
//     };

};


class Q_GUI_EXPORT QPainterState : public QPaintEngineState
{
public:
    QPainterState();
    QPainterState(const QPainterState *s);
    virtual ~QPainterState();
    void init(QPainter *p);

    QPointF brushOrigin;
    QFont font;
    QFont deviceFont;
    QPen pen;
    QBrush brush;
    QBrush bgBrush;             // background brush
    QRegion clipRegion;
    QPainterPath clipPath;
    Qt::ClipOperation clipOperation;
    QPainter::RenderHints renderHints;
    QList<QPainterClipInfo> clipInfo; // ### Make me smaller and faster to copy around...
    QTransform worldMatrix;       // World transformation matrix, not window and viewport
    QTransform matrix;            // Complete transformation matrix,
    QTransform redirectionMatrix;
    int wx, wy, ww, wh;         // window rectangle
    int vx, vy, vw, vh;         // viewport rectangle
    qreal opacity;

    bool WxF;                 // World transformation
    bool VxF;                 // View transformation
    bool clipEnabled;

    QPaintEngine::DirtyFlags changeFlags;
    Qt::BGMode bgMode;
    QPainter *painter;
    Qt::LayoutDirection layoutDirection;
    QPainter::CompositionMode composition_mode;
};

struct QPainterDummyState
{
    QFont font;
    QPen pen;
    QBrush brush;
    QTransform transform;
};

class QPainterPrivate
{
    Q_DECLARE_PUBLIC(QPainter)
public:
    QPainterPrivate(QPainter *painter)
    : q_ptr(painter), d_ptrs(nullptr), state(nullptr), dummyState(nullptr), txinv(false),
        inDestructor(false), d_ptrs_size(0), refcount(1), device(nullptr),
        original_device(nullptr), helper_device(nullptr), engine(nullptr),
        extended(nullptr)
    {
    }

    ~QPainterPrivate();

    QPainter *q_ptr;
    QPainterPrivate **d_ptrs;

    QPainterState *state;
    QVector<QPainterState*> states;

    mutable QPainterDummyState *dummyState;

    QTransform invMatrix;
    bool txinv;
    bool inDestructor;
    int d_ptrs_size;
    uint refcount;

    QPainterDummyState *fakeState() const {
        if (!dummyState)
            dummyState = new QPainterDummyState();
        return dummyState;
    }

    void updateStateImpl(QPainterState *state);
    void updateState(QPainterState *state);

    void draw_helper(const QPainterPath &path);
    void drawStretchedGradient(const QPainterPath &path);

    void updateMatrix();
    void updateInvMatrix();

    int rectSubtraction() const {
        return state->pen.style() != Qt::NoPen && state->pen.width() == 0 ? 1 : 0;
    }

    QTransform viewTransform() const;
    static bool attachPainterPrivate(QPainter *q, QPaintDevice *pdev);
    void detachPainterPrivate(QPainter *q);

    QPaintDevice *device;
    QPaintDevice *original_device;
    QPaintDevice *helper_device;
    QPaintEngine *engine;
    QPaintEngineEx *extended;
};

QT_END_NAMESPACE

#endif // QPAINTER_P_H
