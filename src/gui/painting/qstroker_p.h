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

#ifndef QSTROKER_P_H
#define QSTROKER_P_H

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

#include "QtGui/qpainterpath.h"
#include "qdatabuffer_p.h"

QT_BEGIN_NAMESPACE

typedef qreal qfixed;
struct qfixed2d
{
    qreal x;
    qreal y;

    bool operator==(const qfixed2d &other) const { return qFuzzyCompare(x, other.x)
                                                       && qFuzzyCompare(y, other.y); }
};

#define QT_PATH_KAPPA 0.5522847498

QPointF qt_curves_for_arc(const QRectF &rect, qreal startAngle, qreal sweepLength,
                          QPointF *controlPoints, int *point_count);

qreal qt_t_for_arc_angle(qreal angle);

typedef void (*qStrokerMoveToHook)(qreal x, qreal y, void *data);
typedef void (*qStrokerLineToHook)(qreal x, qreal y, void *data);
typedef void (*qStrokerCubicToHook)(qreal c1x, qreal c1y,
                                    qreal c2x, qreal c2y,
                                    qreal ex, qreal ey,
                                    void *data);

// qtransform.cpp
Q_GUI_EXPORT bool qt_scaleForTransform(const QTransform &transform, qreal *scale);

class Q_GUI_EXPORT QStrokerOps
{
public:
    struct Element {
        QPainterPath::ElementType type;
        qreal x;
        qreal y;

        inline bool isMoveTo() const { return type == QPainterPath::MoveToElement; }
        inline bool isLineTo() const { return type == QPainterPath::LineToElement; }
        inline bool isCurveTo() const { return type == QPainterPath::CurveToElement; }

        operator qfixed2d () { qfixed2d pt = { x, y }; return pt; }
    };

    QStrokerOps();
    virtual ~QStrokerOps();

    void setMoveToHook(qStrokerMoveToHook moveToHook) { m_moveTo = moveToHook; }
    void setLineToHook(qStrokerLineToHook lineToHook) { m_lineTo = lineToHook; }
    void setCubicToHook(qStrokerCubicToHook cubicToHook) { m_cubicTo = cubicToHook; }

    virtual void begin(void *customData);
    virtual void end();

    inline void moveTo(qreal x, qreal y);
    inline void lineTo(qreal x, qreal y);
    inline void cubicTo(qreal x1, qreal y1, qreal x2, qreal y2, qreal ex, qreal ey);

    void strokePath(const QPainterPath &path, void *data, const QTransform &matrix);
    void strokePolygon(const QPointF *points, int pointCount, bool implicit_close,
                       void *data, const QTransform &matrix);
    void strokeEllipse(const QRectF &ellipse, void *data, const QTransform &matrix);

    QRectF clipRect() const { return m_clip_rect; }
    void setClipRect(const QRectF &clip) { m_clip_rect = clip; }

    void setCurveThresholdFromTransform(const QTransform &transform)
    {
        qreal scale;
        qt_scaleForTransform(transform, &scale);
        m_dashThreshold = scale == 0 ? qreal(0.5) : (qreal(0.5) / scale);
    }

    void setCurveThreshold(qreal threshold) { m_curveThreshold = threshold; }
    qreal curveThreshold() const { return m_curveThreshold; }

protected:
    inline void emitMoveTo(qreal x, qreal y);
    inline void emitLineTo(qreal x, qreal y);
    inline void emitCubicTo(qreal c1x, qreal c1y, qreal c2x, qreal c2y, qreal ex, qreal ey);

    virtual void processCurrentSubpath() = 0;
    QDataBuffer<Element> m_elements;

    QRectF m_clip_rect;
    qreal m_curveThreshold;
    qreal m_dashThreshold;

    void *m_customData;
    qStrokerMoveToHook m_moveTo;
    qStrokerLineToHook m_lineTo;
    qStrokerCubicToHook m_cubicTo;

};

class Q_GUI_EXPORT QStroker : public QStrokerOps
{
public:

    enum LineJoinMode {
        FlatJoin,
        SquareJoin,
        MiterJoin,
        RoundJoin,
        RoundCap,
        SvgMiterJoin
    };

    QStroker();
    ~QStroker();

    void setStrokeWidth(qreal width) { m_strokeWidth = width; }
    qreal strokeWidth() const { return m_strokeWidth; }

    void setCapStyle(Qt::PenCapStyle capStyle) { m_capStyle = joinModeForCap(capStyle); }
    Qt::PenCapStyle capStyle() const { return capForJoinMode(m_capStyle); }
    LineJoinMode capStyleMode() const { return m_capStyle; }

    void setJoinStyle(Qt::PenJoinStyle style) { m_joinStyle = joinModeForJoin(style); }
    Qt::PenJoinStyle joinStyle() const { return joinForJoinMode(m_joinStyle); }
    LineJoinMode joinStyleMode() const { return m_joinStyle; }

    void setMiterLimit(qreal length) { m_miterLimit = length; }
    qreal miterLimit() const { return m_miterLimit; }

    void joinPoints(qreal x, qreal y, const QLineF &nextLine, LineJoinMode join);
    inline void emitMoveTo(qreal x, qreal y);
    inline void emitLineTo(qreal x, qreal y);
    inline void emitCubicTo(qreal c1x, qreal c1y, qreal c2x, qreal c2y, qreal ex, qreal ey);

protected:
    static Qt::PenCapStyle capForJoinMode(LineJoinMode mode);
    static LineJoinMode joinModeForCap(Qt::PenCapStyle);

    static Qt::PenJoinStyle joinForJoinMode(LineJoinMode mode);
    static LineJoinMode joinModeForJoin(Qt::PenJoinStyle joinStyle);

    virtual void processCurrentSubpath();

    qreal m_strokeWidth;
    qreal m_miterLimit;

    LineJoinMode m_capStyle;
    LineJoinMode m_joinStyle;

    qreal m_back1X;
    qreal m_back1Y;

    qreal m_back2X;
    qreal m_back2Y;
};

class Q_GUI_EXPORT QDashStroker : public QStrokerOps
{
public:
    QDashStroker(QStroker *stroker);

    QStroker *stroker() const { return m_stroker; }

    static QVector<qreal> patternForStyle(Qt::PenStyle style);

    void setDashPattern(const QVector<qreal> &dashPattern) { m_dashPattern = dashPattern; }
    QVector<qreal> dashPattern() const { return m_dashPattern; }

    void setDashOffset(qreal offset) { m_dashOffset = offset; }
    qreal dashOffset() const { return m_dashOffset; }

    virtual void begin(void *data);
    virtual void end();

    inline void setStrokeWidth(qreal width) { m_stroke_width = width; }
    inline void setMiterLimit(qreal limit) { m_miter_limit = limit; }

protected:
    virtual void processCurrentSubpath();

    QStroker *m_stroker;
    QVector<qreal> m_dashPattern;
    qreal m_dashOffset;

    qreal m_stroke_width;
    qreal m_miter_limit;
};


/*******************************************************************************
 * QStrokerOps inline membmers
 */

inline void QStrokerOps::emitMoveTo(qreal x, qreal y)
{
    Q_ASSERT(m_moveTo);
    m_moveTo(x, y, m_customData);
}

inline void QStrokerOps::emitLineTo(qreal x, qreal y)
{
    Q_ASSERT(m_lineTo);
    m_lineTo(x, y, m_customData);
}

inline void QStrokerOps::emitCubicTo(qreal c1x, qreal c1y, qreal c2x, qreal c2y, qreal ex, qreal ey)
{
    Q_ASSERT(m_cubicTo);
    m_cubicTo(c1x, c1y, c2x, c2y, ex, ey, m_customData);
}

inline void QStrokerOps::moveTo(qreal x, qreal y)
{
    if (m_elements.size()>1)
        processCurrentSubpath();
    m_elements.reset();
    Element e = { QPainterPath::MoveToElement, x, y };
    m_elements.add(e);
}

inline void QStrokerOps::lineTo(qreal x, qreal y)
{
    Element e = { QPainterPath::LineToElement, x, y };
    m_elements.add(e);
}

inline void QStrokerOps::cubicTo(qreal x1, qreal y1, qreal x2, qreal y2, qreal ex, qreal ey)
{
    Element c1 = { QPainterPath::CurveToElement, x1, y1 };
    Element c2 = { QPainterPath::CurveToDataElement, x2, y2 };
    Element e = { QPainterPath::CurveToDataElement, ex, ey };
    m_elements.add(c1);
    m_elements.add(c2);
    m_elements.add(e);
}

/*******************************************************************************
 * QStroker inline members
 */
inline void QStroker::emitMoveTo(qreal x, qreal y)
{
    m_back2X = m_back1X;
    m_back2Y = m_back1Y;
    m_back1X = x;
    m_back1Y = y;
    QStrokerOps::emitMoveTo(x, y);
}

inline void QStroker::emitLineTo(qreal x, qreal y)
{
    m_back2X = m_back1X;
    m_back2Y = m_back1Y;
    m_back1X = x;
    m_back1Y = y;
    QStrokerOps::emitLineTo(x, y);
}

inline void QStroker::emitCubicTo(qreal c1x, qreal c1y,
                                  qreal c2x, qreal c2y,
                                  qreal ex, qreal ey)
{
    if (c2x == ex && c2y == ey) {
        if (c1x == ex && c1y == ey) {
            m_back2X = m_back1X;
            m_back2Y = m_back1Y;
        } else {
            m_back2X = c1x;
            m_back2Y = c1y;
        }
    } else {
        m_back2X = c2x;
        m_back2Y = c2y;
    }
    m_back1X = ex;
    m_back1Y = ey;
    QStrokerOps::emitCubicTo(c1x, c1y, c2x, c2y, ex, ey);
}

/*******************************************************************************
 * QDashStroker inline members
 */
inline void QDashStroker::begin(void *data)
{
    if (m_stroker)
        m_stroker->begin(data);
    QStrokerOps::begin(data);
}

inline void QDashStroker::end()
{
    QStrokerOps::end();
    if (m_stroker)
        m_stroker->end();
}

QT_END_NAMESPACE

#endif // QSTROKER_P_H
