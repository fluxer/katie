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

#include "qstroker_p.h"
#include "qbezier_p.h"
#include "qline.h"
#include "qtransform.h"
#include "qmath.h"
#include "qnumeric.h"
#include "qcorecommon_p.h"
#include "qguicommon_p.h"

QT_BEGIN_NAMESPACE

// #define QPP_STROKE_DEBUG

class QSubpathForwardIterator
{
public:
    QSubpathForwardIterator(const QDataBuffer<QStrokerOps::Element> *path)
        : m_path(path), m_pos(0) { }
    inline int position() const { return m_pos; }
    inline bool hasNext() const { return m_pos < m_path->size(); }
    inline QStrokerOps::Element next() { Q_ASSERT(hasNext()); return m_path->at(m_pos++); }

private:
    const QDataBuffer<QStrokerOps::Element> *m_path;
    int m_pos;
};

class QSubpathBackwardIterator
{
public:
    QSubpathBackwardIterator(const QDataBuffer<QStrokerOps::Element> *path)
        : m_path(path), m_pos(path->size() - 1) { }

    inline int position() const { return m_pos; }

    inline bool hasNext() const { return m_pos >= 0; }

    inline QStrokerOps::Element next()
    {
        Q_ASSERT(hasNext());

        QStrokerOps::Element ce = m_path->at(m_pos);   // current element

        if (m_pos == m_path->size() - 1) {
            --m_pos;
            ce.type = QPainterPath::MoveToElement;
            return ce;
        }

        const QStrokerOps::Element &pe = m_path->at(m_pos + 1); // previous element

        switch (pe.type) {
        case QPainterPath::LineToElement:
            ce.type = QPainterPath::LineToElement;
            break;
        case QPainterPath::CurveToDataElement:
            // First control point?
            if (ce.type == QPainterPath::CurveToElement) {
                ce.type = QPainterPath::CurveToDataElement;
            } else { // Second control point then
                ce.type = QPainterPath::CurveToElement;
            }
            break;
        case QPainterPath::CurveToElement:
            ce.type = QPainterPath::CurveToDataElement;
            break;
        default:
            qWarning("QSubpathReverseIterator::next: Case %d unhandled", ce.type);
            break;
        }
        --m_pos;

        return ce;
    }

private:
    const QDataBuffer<QStrokerOps::Element> *m_path;
    int m_pos;
};

class QSubpathFlatIterator
{
public:
    QSubpathFlatIterator(const QDataBuffer<QStrokerOps::Element> *path, qreal threshold)
        : m_path(path), m_pos(0), m_curve_index(-1), m_curve_threshold(threshold) { }

    inline bool hasNext() const { return m_curve_index >= 0 || m_pos < m_path->size(); }

    QStrokerOps::Element next()
    {
        Q_ASSERT(hasNext());

        if (m_curve_index >= 0) {
            QStrokerOps::Element e = { QPainterPath::LineToElement,
                                       qreal(m_curve.at(m_curve_index).x()),
                                       qreal(m_curve.at(m_curve_index).y())
                                       };
            ++m_curve_index;
            if (m_curve_index >= m_curve.size())
                m_curve_index = -1;
            return e;
        }

        QStrokerOps::Element e = m_path->at(m_pos);
        if (e.isCurveTo()) {
            Q_ASSERT(m_pos > 0);
            Q_ASSERT(m_pos < m_path->size());

            m_curve = QBezier::fromPoints(QPointF(m_path->at(m_pos-1).x,
                                                  m_path->at(m_pos-1).y),
                                          QPointF(e.x, e.y),
                                          QPointF(m_path->at(m_pos+1).x,
                                                  m_path->at(m_pos+1).y),
                                          QPointF(m_path->at(m_pos+2).x,
                                                  m_path->at(m_pos+2).y)).toPolygon(m_curve_threshold);
            m_curve_index = 1;
            e.type = QPainterPath::LineToElement;
            e.x = m_curve.at(0).x();
            e.y = m_curve.at(0).y();
            m_pos += 2;
        }
        Q_ASSERT(e.isLineTo() || e.isMoveTo());
        ++m_pos;
        return e;
    }

private:
    const QDataBuffer<QStrokerOps::Element> *m_path;
    int m_pos;
    QPolygonF m_curve;
    int m_curve_index;
    qreal m_curve_threshold;
};

template <class Iterator> bool qt_stroke_side(Iterator *it, QStroker *stroker,
                                              bool capFirst, QLineF *startTangent);

/*******************************************************************************
 * QLineF::angle gives us the smalles angle between two lines. Here we
 * want to identify the line's angle direction on the unit circle.
 */
static inline qreal adapted_angle_on_x(const QLineF &line)
{
    qreal angle = line.angle(QLineF(0, 0, 1, 0));
    if (line.dy() > 0)
        angle = 360 - angle;
    return angle;
}

QStrokerOps::QStrokerOps()
    : m_elements(0)
    , m_curveThreshold(qreal(0.25))
    , m_dashThreshold(qreal(0.25))
    , m_customData(0)
    , m_moveTo(0)
    , m_lineTo(0)
    , m_cubicTo(0)
{
}

QStrokerOps::~QStrokerOps()
{
}

/*!
    Prepares the stroker. Call this function once before starting a
    stroke by calling moveTo, lineTo or cubicTo.

    The \a customData is passed back through that callback functions
    and can be used by the user to for instance maintain state
    information.
*/
void QStrokerOps::begin(void *customData)
{
    m_customData = customData;
    m_elements.reset();
}


/*!
    Finishes the stroke. Call this function once when an entire
    primitive has been stroked.
*/
void QStrokerOps::end()
{
    if (m_elements.size() > 1)
        processCurrentSubpath();
    m_customData = 0;
}

/*!
    Convenience function that decomposes \a path into begin(),
    moveTo(), lineTo(), curevTo() and end() calls.

    The \a customData parameter is used in the callback functions

    The \a matrix is used to transform the points before input to the
    stroker.

    \sa begin()
*/
void QStrokerOps::strokePath(const QPainterPath &path, void *customData, const QTransform &matrix)
{
    if (path.isEmpty())
        return;

    setCurveThresholdFromTransform(QTransform());
    begin(customData);
    int count = path.elementCount();
    if (matrix.isIdentity()) {
        for (int i=0; i<count; ++i) {
            const QPainterPath::Element &e = path.elementAt(i);
            switch (e.type) {
            case QPainterPath::MoveToElement:
                moveTo(qreal(e.x), qreal(e.y));
                break;
            case QPainterPath::LineToElement:
                lineTo(qreal(e.x), qreal(e.y));
                break;
            case QPainterPath::CurveToElement:
                {
                    const QPainterPath::Element &cp2 = path.elementAt(++i);
                    const QPainterPath::Element &ep = path.elementAt(++i);
                    cubicTo(qreal(e.x), qreal(e.y),
                            qreal(cp2.x), qreal(cp2.y),
                            qreal(ep.x), qreal(ep.y));
                }
                break;
            default:
                break;
            }
        }
    } else {
        for (int i=0; i<count; ++i) {
            const QPainterPath::Element &e = path.elementAt(i);
            QPointF pt = QPointF(e.x, e.y) * matrix;
            switch (e.type) {
            case QPainterPath::MoveToElement:
                moveTo(qreal(pt.x()), qreal(pt.y()));
                break;
            case QPainterPath::LineToElement:
                lineTo(qreal(pt.x()), qreal(pt.y()));
                break;
            case QPainterPath::CurveToElement:
                {
                    QPointF cp2 = ((QPointF) path.elementAt(++i)) * matrix;
                    QPointF ep = ((QPointF) path.elementAt(++i)) * matrix;
                    cubicTo(qreal(pt.x()), qreal(pt.y()),
                            qreal(cp2.x()), qreal(cp2.y()),
                            qreal(ep.x()), qreal(ep.y()));
                }
                break;
            default:
                break;
            }
        }
    }
    end();
}

/*!
    Convenience function for stroking a polygon of the \a pointCount
    first points in \a points. If \a implicit_close is set to true a
    line is implictly drawn between the first and last point in the
    polygon. Typically true for polygons and false for polylines.

    The \a matrix is used to transform the points before they enter the
    stroker.

    \sa begin()
*/

void QStrokerOps::strokePolygon(const QPointF *points, int pointCount, bool implicit_close,
                                void *data, const QTransform &matrix)
{
    if (!pointCount)
        return;

    setCurveThresholdFromTransform(QTransform());
    begin(data);
    if (matrix.isIdentity()) {
        moveTo(qreal(points[0].x()), qreal(points[0].y()));
        for (int i=1; i<pointCount; ++i)
            lineTo(qreal(points[i].x()),
                   qreal(points[i].y()));
        if (implicit_close)
            lineTo(qreal(points[0].x()), qreal(points[0].y()));
    } else {
        QPointF start = points[0] * matrix;
        moveTo(qreal(start.x()), qreal(start.y()));
        for (int i=1; i<pointCount; ++i) {
            QPointF pt = points[i] * matrix;
            lineTo(qreal(pt.x()), qreal(pt.y()));
        }
        if (implicit_close)
            lineTo(qreal(start.x()), qreal(start.y()));
    }
    end();
}

/*!
    Convenience function for stroking an ellipse with bounding rect \a
    rect. The \a matrix is used to transform the coordinates before
    they enter the stroker.
*/
void QStrokerOps::strokeEllipse(const QRectF &rect, void *data, const QTransform &matrix)
{
    int count = 0;
    QPointF pts[12];
    QPointF start = qt_curves_for_arc(rect, 0, -360, pts, &count);
    Q_ASSERT(count == 12); // a perfect circle..

    if (!matrix.isIdentity()) {
        start = start * matrix;
        for (int i=0; i<12; ++i) {
            pts[i] = pts[i] * matrix;
        }
    }

    setCurveThresholdFromTransform(QTransform());
    begin(data);
    moveTo(qreal(start.x()), qreal(start.y()));
    for (int i=0; i<12; i+=3) {
        cubicTo(qreal(pts[i].x()), qreal(pts[i].y()),
                qreal(pts[i+1].x()), qreal(pts[i+1].y()),
                qreal(pts[i+2].x()), qreal(pts[i+2].y()));
    }
    end();
}


QStroker::QStroker()
    : m_capStyle(SquareJoin), m_joinStyle(FlatJoin),
      m_back1X(0), m_back1Y(0),
      m_back2X(0), m_back2Y(0)
{
    m_strokeWidth = qreal(1);
    m_miterLimit = qreal(2);
}

QStroker::~QStroker()
{
}

Qt::PenCapStyle QStroker::capForJoinMode(LineJoinMode mode)
{
    if (mode == FlatJoin) return Qt::FlatCap;
    else if (mode == SquareJoin) return Qt::SquareCap;
    else return Qt::RoundCap;
}

QStroker::LineJoinMode QStroker::joinModeForCap(Qt::PenCapStyle style)
{
    if (style == Qt::FlatCap) return FlatJoin;
    else if (style == Qt::SquareCap) return SquareJoin;
    else return RoundCap;
}

Qt::PenJoinStyle QStroker::joinForJoinMode(LineJoinMode mode)
{
    if (mode == FlatJoin) return Qt::BevelJoin;
    else if (mode == MiterJoin) return Qt::MiterJoin;
    else if (mode == SvgMiterJoin) return Qt::SvgMiterJoin;
    else return Qt::RoundJoin;
}

QStroker::LineJoinMode QStroker::joinModeForJoin(Qt::PenJoinStyle joinStyle)
{
    if (joinStyle == Qt::BevelJoin) return FlatJoin;
    else if (joinStyle == Qt::MiterJoin) return MiterJoin;
    else if (joinStyle == Qt::SvgMiterJoin) return SvgMiterJoin;
    else return RoundJoin;
}


/*!
    This function is called to stroke the currently built up
    subpath. The subpath is cleared when the function completes.
*/
void QStroker::processCurrentSubpath()
{
    Q_ASSERT(!m_elements.isEmpty());
    Q_ASSERT(m_elements.first().type == QPainterPath::MoveToElement);
    Q_ASSERT(m_elements.size() > 1);

    QSubpathForwardIterator fwit(&m_elements);
    QSubpathBackwardIterator bwit(&m_elements);

    QLineF fwStartTangent, bwStartTangent;

    bool fwclosed = qt_stroke_side(&fwit, this, false, &fwStartTangent);
    bool bwclosed = qt_stroke_side(&bwit, this, !fwclosed, &bwStartTangent);

    if (!bwclosed)
        joinPoints(m_elements.at(0).x, m_elements.at(0).y, fwStartTangent, m_capStyle);
}


/*!
    \internal
*/
void QStroker::joinPoints(qreal focal_x, qreal focal_y, const QLineF &nextLine, LineJoinMode join)
{
#ifdef QPP_STROKE_DEBUG
    printf(" -----> joinPoints: around=(%.0f, %.0f), next_p1=(%.0f, %.f) next_p2=(%.0f, %.f)\n",
           focal_x, focal_y,
           nextLine.x1(), nextLine.y1(), nextLine.x2(), nextLine.y2());
#endif
    // points connected already, don't join

#if !defined (QFIXED_26_6) && !defined (Q_FIXED_32_32)
    if (qFuzzyCompare(m_back1X, nextLine.x1()) && qFuzzyCompare(m_back1Y, nextLine.y1()))
        return;
#else
    if (m_back1X == qreal(nextLine.x1())
        && m_back1Y == qreal(nextLine.y1())) {
        return;
    }
#endif

    if (join == FlatJoin) {
        QLineF prevLine(m_back2X, m_back2Y, m_back1X, m_back1Y);
        QPointF isect;
        QLineF::IntersectType type = prevLine.intersect(nextLine, &isect);
        QLineF shortCut(prevLine.p2(), nextLine.p1());
        qreal angle = shortCut.angleTo(prevLine);
        if (type == QLineF::BoundedIntersection || (angle > 90 && !qFuzzyCompare(angle, (qreal)90))) {
            emitLineTo(focal_x, focal_y);
            emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));
            return;
        }
        emitLineTo(qreal(nextLine.x1()),
                   qreal(nextLine.y1()));

    } else {
        QLineF prevLine(m_back2X, m_back2Y, m_back1X, m_back1Y);

        QPointF isect;
        QLineF::IntersectType type = prevLine.intersect(nextLine, &isect);

        if (join == MiterJoin) {
            qreal appliedMiterLimit = m_strokeWidth * m_miterLimit;

            // If we are on the inside, do the short cut...
            QLineF shortCut(prevLine.p2(), nextLine.p1());
            qreal angle = shortCut.angleTo(prevLine);
            if (type == QLineF::BoundedIntersection || (angle > 90 && !qFuzzyCompare(angle, (qreal)90))) {
                emitLineTo(focal_x, focal_y);
                emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));
                return;
            }
            QLineF miterLine(QPointF(m_back1X, m_back1Y), isect);
            if (type == QLineF::NoIntersection || miterLine.length() > appliedMiterLimit) {
                QLineF l1(prevLine);
                l1.setLength(appliedMiterLimit);
                l1.translate(prevLine.dx(), prevLine.dy());

                QLineF l2(nextLine);
                l2.setLength(appliedMiterLimit);
                l2.translate(-l2.dx(), -l2.dy());

                emitLineTo(qreal(l1.x2()), qreal(l1.y2()));
                emitLineTo(qreal(l2.x1()), qreal(l2.y1()));
                emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));
            } else {
                emitLineTo(qreal(isect.x()), qreal(isect.y()));
                emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));
            }

        } else if (join == SquareJoin) {
            qreal offset = m_strokeWidth / 2;

            QLineF l1(prevLine);
            l1.translate(l1.dx(), l1.dy());
            l1.setLength(offset);
            QLineF l2(nextLine.p2(), nextLine.p1());
            l2.translate(l2.dx(), l2.dy());
            l2.setLength(offset);
            emitLineTo(qreal(l1.x2()), qreal(l1.y2()));
            emitLineTo(qreal(l2.x2()), qreal(l2.y2()));
            emitLineTo(qreal(l2.x1()), qreal(l2.y1()));

        } else if (join == RoundJoin) {
            qreal offset = m_strokeWidth / 2;

            QLineF shortCut(prevLine.p2(), nextLine.p1());
            qreal angle = shortCut.angleTo(prevLine);
            if (type == QLineF::BoundedIntersection || (angle > 90 && !qFuzzyCompare(angle, (qreal)90))) {
                emitLineTo(focal_x, focal_y);
                emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));
                return;
            }
            qreal l1_on_x = adapted_angle_on_x(prevLine);
            qreal l2_on_x = adapted_angle_on_x(nextLine);

            qreal sweepLength = qAbs(l2_on_x - l1_on_x);

            int point_count;
            QPointF curves[15];

            QPointF curve_start =
                qt_curves_for_arc(QRectF(focal_x - offset,
                                         focal_y - offset,
                                         offset * 2,
                                         offset * 2),
                                  l1_on_x + 90, -sweepLength,
                                  curves, &point_count);

//             // line to the beginning of the arc segment, (should not be needed).
//             emitLineTo(qreal(curve_start.x()), qreal(curve_start.y()));
            Q_UNUSED(curve_start);

            for (int i=0; i<point_count; i+=3) {
                emitCubicTo(qreal(curves[i].x()),
                            qreal(curves[i].y()),
                            qreal(curves[i+1].x()),
                            qreal(curves[i+1].y()),
                            qreal(curves[i+2].x()),
                            qreal(curves[i+2].y()));
            }

            // line to the end of the arc segment, (should also not be needed).
            emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));

        // Same as round join except we know its 180 degrees. Can also optimize this
        // later based on the addEllipse logic
        } else if (join == RoundCap) {
            qreal offset = m_strokeWidth / 2;

            // first control line
            QLineF l1 = prevLine;
            l1.translate(l1.dx(), l1.dy());
            l1.setLength(QT_PATH_KAPPA * offset);

            // second control line, find through normal between prevLine and focal.
            QLineF l2(focal_x, focal_y,
                      prevLine.x2(), prevLine.y2());
            l2.translate(-l2.dy(), l2.dx());
            l2.setLength(QT_PATH_KAPPA * offset);

            emitCubicTo(qreal(l1.x2()),
                        qreal(l1.y2()),
                        qreal(l2.x2()),
                        qreal(l2.y2()),
                        qreal(l2.x1()),
                        qreal(l2.y1()));

            // move so that it matches
            l2 = QLineF(l2.x1(), l2.y1(), l2.x1()-l2.dx(), l2.y1()-l2.dy());

            // last line is parallel to l1 so just shift it down.
            l1.translate(nextLine.x1() - l1.x1(), nextLine.y1() - l1.y1());

            emitCubicTo(qreal(l2.x2()),
                        qreal(l2.y2()),
                        qreal(l1.x2()),
                        qreal(l1.y2()),
                        qreal(l1.x1()),
                        qreal(l1.y1()));
        } else if (join == SvgMiterJoin) {
            QLineF shortCut(prevLine.p2(), nextLine.p1());
            qreal angle = shortCut.angleTo(prevLine);
            if (type == QLineF::BoundedIntersection || (angle > 90 && !qFuzzyCompare(angle, (qreal)90))) {
                emitLineTo(focal_x, focal_y);
                emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));
                return;
            }
            QLineF miterLine(QPointF(focal_x, focal_y), isect);
            if (type == QLineF::NoIntersection || miterLine.length() > m_strokeWidth * m_miterLimit / 2) {
                emitLineTo(qreal(nextLine.x1()),
                           qreal(nextLine.y1()));
            } else {
                emitLineTo(qreal(isect.x()), qreal(isect.y()));
                emitLineTo(qreal(nextLine.x1()), qreal(nextLine.y1()));
            }
        } else {
            Q_ASSERT(!"QStroker::joinPoints(), bad join style...");
        }
    }
}


/*
   Strokes a subpath side using the \a it as source. Results are put into
   \a stroke. The function returns true if the subpath side was closed.
   If \a capFirst is true, we will use capPoints instead of joinPoints to
   connect the first segment, other segments will be joined using joinPoints.
   This is to put capping in order...
*/
template <class Iterator> bool qt_stroke_side(Iterator *it,
                                              QStroker *stroker,
                                              bool capFirst,
                                              QLineF *startTangent)
{
    // Used in CurveToElement section below.
    const int MAX_OFFSET = 16;
    QBezier offsetCurves[MAX_OFFSET];

    Q_ASSERT(it->hasNext()); // The initaial move to
    QStrokerOps::Element first_element = it->next();
    Q_ASSERT(first_element.isMoveTo());

    qfixed2d start = first_element;

#ifdef QPP_STROKE_DEBUG
    qDebug(" -> (side) [%.2f, %.2f], startPos=%d", start.x, start.y);
#endif

    qfixed2d prev = start;

    bool first = true;

    qreal offset = stroker->strokeWidth() / 2;

    while (it->hasNext()) {
        QStrokerOps::Element e = it->next();

        // LineToElement
        if (e.isLineTo()) {
#ifdef QPP_STROKE_DEBUG
            qDebug("\n ---> (side) lineto [%.2f, %.2f]", e.x, e.y);
#endif
            QLineF line(prev.x, prev.y, e.x, e.y);
            if (line.p1() != line.p2()) {
                QLineF normal = line.normalVector();
                normal.setLength(offset);
                line.translate(normal.dx(), normal.dy());

                // If we are starting a new subpath, move to correct starting point.
                if (first) {
                    if (capFirst)
                        stroker->joinPoints(prev.x, prev.y, line, stroker->capStyleMode());
                    else
                        stroker->emitMoveTo(qreal(line.x1()), qreal(line.y1()));
                    *startTangent = line;
                    first = false;
                } else {
                    stroker->joinPoints(prev.x, prev.y, line, stroker->joinStyleMode());
                }

                // Add the stroke for this line.
                stroker->emitLineTo(qreal(line.x2()),
                                    qreal(line.y2()));
                prev = e;
            }

        // CurveToElement
        } else if (e.isCurveTo()) {
            QStrokerOps::Element cp2 = it->next(); // control point 2
            QStrokerOps::Element ep = it->next();  // end point

#ifdef QPP_STROKE_DEBUG
            qDebug("\n ---> (side) cubicTo [%.2f, %.2f]", ep.x, ep.y);
#endif

            QBezier bezier =
                QBezier::fromPoints(QPointF(prev.x, prev.y),
                                    QPointF(e.x, e.y),
                                    QPointF(cp2.x, cp2.y),
                                    QPointF(ep.x, ep.y));

            int count = bezier.shifted(offsetCurves,
                                       MAX_OFFSET,
                                       offset,
                                       stroker->curveThreshold());

            if (count) {
                // If we are starting a new subpath, move to correct starting point
                QLineF tangent = bezier.startTangent();
                tangent.translate(offsetCurves[0].pt1() - bezier.pt1());
                if (first) {
                    QPointF pt = offsetCurves[0].pt1();
                    if (capFirst) {
                        stroker->joinPoints(prev.x, prev.y,
                                            tangent,
                                            stroker->capStyleMode());
                    } else {
                        stroker->emitMoveTo(qreal(pt.x()),
                                            qreal(pt.y()));
                    }
                    *startTangent = tangent;
                    first = false;
                } else {
                    stroker->joinPoints(prev.x, prev.y,
                                        tangent,
                                        stroker->joinStyleMode());
                }

                // Add these beziers
                for (int i=0; i<count; ++i) {
                    QPointF cp1 = offsetCurves[i].pt2();
                    QPointF cp2 = offsetCurves[i].pt3();
                    QPointF ep = offsetCurves[i].pt4();
                    stroker->emitCubicTo(qreal(cp1.x()), qreal(cp1.y()),
                                         qreal(cp2.x()), qreal(cp2.y()),
                                         qreal(ep.x()), qreal(ep.y()));
                }
            }

            prev = ep;
        }
    }

    if (start == prev) {
        // closed subpath, join first and last point
#ifdef QPP_STROKE_DEBUG
        qDebug("\n ---> (side) closed subpath");
#endif
        // don't join empty subpaths
        if (!first)
            stroker->joinPoints(prev.x, prev.y, *startTangent, stroker->joinStyleMode());
        return true;
    } else {
#ifdef QPP_STROKE_DEBUG
        qDebug("\n ---> (side) open subpath");
#endif
        return false;
    }
}

/*!
    \internal

    For a given angle in the range [0 .. 90], finds the corresponding parameter t
    of the prototype cubic bezier arc segment
    b = fromPoints(QPointF(1, 0), QPointF(1, QT_PATH_KAPPA), QPointF(QT_PATH_KAPPA, 1), QPointF(0, 1));

    From the bezier equation:
    b.pointAt(t).x() = (1-t)^3 + t*(1-t)^2 + t^2*(1-t)*QT_PATH_KAPPA
    b.pointAt(t).y() = t*(1-t)^2 * QT_PATH_KAPPA + t^2*(1-t) + t^3

    Third degree coefficients:
    b.pointAt(t).x() = at^3 + bt^2 + ct + d
    where a = 2-3*QT_PATH_KAPPA, b = 3*(QT_PATH_KAPPA-1), c = 0, d = 1

    b.pointAt(t).y() = at^3 + bt^2 + ct + d
    where a = 3*QT_PATH_KAPPA-2, b = 6*QT_PATH_KAPPA+3, c = 3*QT_PATH_KAPPA, d = 0

    Newton's method to find the zero of a function:
    given a function f(x) and initial guess x_0
    x_1 = f(x_0) / f'(x_0)
    x_2 = f(x_1) / f'(x_1)
    etc...
*/

qreal qt_t_for_arc_angle(qreal angle)
{
    if (qFuzzyIsNull(angle))
        return 0;

    if (qFuzzyCompare(angle, qreal(90)))
        return 1;

    qreal radians = M_PI * angle / 180;
    qreal cosAngle = qCos(radians);
    qreal sinAngle = qSin(radians);

    // initial guess
    qreal tc = angle / 90;
    // do some iterations of newton's method to approximate cosAngle
    // finds the zero of the function b.pointAt(tc).x() - cosAngle
    tc -= ((((2-3*QT_PATH_KAPPA) * tc + 3*(QT_PATH_KAPPA-1)) * tc) * tc + 1 - cosAngle) // value
         / (((6-9*QT_PATH_KAPPA) * tc + 6*(QT_PATH_KAPPA-1)) * tc); // derivative
    tc -= ((((2-3*QT_PATH_KAPPA) * tc + 3*(QT_PATH_KAPPA-1)) * tc) * tc + 1 - cosAngle) // value
         / (((6-9*QT_PATH_KAPPA) * tc + 6*(QT_PATH_KAPPA-1)) * tc); // derivative

    // initial guess
    qreal ts = tc;
    // do some iterations of newton's method to approximate sinAngle
    // finds the zero of the function b.pointAt(tc).y() - sinAngle
    ts -= ((((3*QT_PATH_KAPPA-2) * ts -  6*QT_PATH_KAPPA + 3) * ts + 3*QT_PATH_KAPPA) * ts - sinAngle)
         / (((9*QT_PATH_KAPPA-6) * ts + 12*QT_PATH_KAPPA - 6) * ts + 3*QT_PATH_KAPPA);
    ts -= ((((3*QT_PATH_KAPPA-2) * ts -  6*QT_PATH_KAPPA + 3) * ts + 3*QT_PATH_KAPPA) * ts - sinAngle)
         / (((9*QT_PATH_KAPPA-6) * ts + 12*QT_PATH_KAPPA - 6) * ts + 3*QT_PATH_KAPPA);

    // use the average of the t that best approximates cosAngle
    // and the t that best approximates sinAngle
    qreal t = 0.5 * (tc + ts);

#if 0
    printf("angle: %f, t: %f\n", angle, t);
    qreal a, b, c, d;
    bezierCoefficients(t, a, b, c, d);
    printf("cosAngle: %.10f, value: %.10f\n", cosAngle, a + b + c * QT_PATH_KAPPA);
    printf("sinAngle: %.10f, value: %.10f\n", sinAngle, b * QT_PATH_KAPPA + c + d);
#endif

    return t;
}

Q_GUI_EXPORT void qt_find_ellipse_coords(const QRectF &r, qreal angle, qreal length,
                            QPointF* startPoint, QPointF *endPoint);

/*!
    \internal

    Creates a number of curves for a given arc definition. The arc is
    defined an arc along the ellipses that fits into \a rect starting
    at \a startAngle and an arc length of \a sweepLength.

    The function has three out parameters. The return value is the
    starting point of the arc. The \a curves array represents the list
    of cubicTo elements up to a maximum of \a point_count. There are of course
    3 points pr curve.
*/
QPointF qt_curves_for_arc(const QRectF &rect, qreal startAngle, qreal sweepLength,
                       QPointF *curves, int *point_count)
{
    Q_ASSERT(point_count);
    Q_ASSERT(curves);

    *point_count = 0;
    if (qIsNaN(rect.x()) || qIsNaN(rect.y()) || qIsNaN(rect.width()) || qIsNaN(rect.height())
        || qIsNaN(startAngle) || qIsNaN(sweepLength)) {
        qWarning("QPainterPath::arcTo: Adding arc where a parameter is NaN, results are undefined");
        return QPointF();
    }

    if (rect.isNull()) {
        return QPointF();
    }

    qreal x = rect.x();
    qreal y = rect.y();

    qreal w = rect.width();
    qreal w2 = rect.width() / 2;
    qreal w2k = w2 * QT_PATH_KAPPA;

    qreal h = rect.height();
    qreal h2 = rect.height() / 2;
    qreal h2k = h2 * QT_PATH_KAPPA;

    QPointF points[16] =
    {
        // start point
        QPointF(x + w, y + h2),

        // 0 -> 270 degrees
        QPointF(x + w, y + h2 + h2k),
        QPointF(x + w2 + w2k, y + h),
        QPointF(x + w2, y + h),

        // 270 -> 180 degrees
        QPointF(x + w2 - w2k, y + h),
        QPointF(x, y + h2 + h2k),
        QPointF(x, y + h2),

        // 180 -> 90 degrees
        QPointF(x, y + h2 - h2k),
        QPointF(x + w2 - w2k, y),
        QPointF(x + w2, y),

        // 90 -> 0 degrees
        QPointF(x + w2 + w2k, y),
        QPointF(x + w, y + h2 - h2k),
        QPointF(x + w, y + h2)
    };

    if (sweepLength > 360) sweepLength = 360;
    else if (sweepLength < -360) sweepLength = -360;

    // Special case fast paths
    if (startAngle == 0.0) {
        if (sweepLength == 360.0) {
            for (int i = 11; i >= 0; --i)
                curves[(*point_count)++] = points[i];
            return points[12];
        } else if (sweepLength == -360.0) {
            for (int i = 1; i <= 12; ++i)
                curves[(*point_count)++] = points[i];
            return points[0];
        }
    }

    int startSegment = qFloor(startAngle / 90);
    int endSegment = qFloor((startAngle + sweepLength) / 90);

    qreal startT = (startAngle - startSegment * 90) / 90;
    qreal endT = (startAngle + sweepLength - endSegment * 90) / 90;

    int delta = sweepLength > 0 ? 1 : -1;
    if (delta < 0) {
        startT = 1 - startT;
        endT = 1 - endT;
    }

    // avoid empty start segment
    if (qFuzzyIsNull(startT - qreal(1))) {
        startT = 0;
        startSegment += delta;
    }

    // avoid empty end segment
    if (qFuzzyIsNull(endT)) {
        endT = 1;
        endSegment -= delta;
    }

    startT = qt_t_for_arc_angle(startT * 90);
    endT = qt_t_for_arc_angle(endT * 90);

    const bool splitAtStart = !qFuzzyIsNull(startT);
    const bool splitAtEnd = !qFuzzyIsNull(endT - qreal(1));

    const int end = endSegment + delta;

    // empty arc?
    if (startSegment == end) {
        const int quadrant = 3 - ((startSegment % 4) + 4) % 4;
        const int j = 3 * quadrant;
        return delta > 0 ? points[j + 3] : points[j];
    }

    QPointF startPoint, endPoint;
    qt_find_ellipse_coords(rect, startAngle, sweepLength, &startPoint, &endPoint);

    for (int i = startSegment; i != end; i += delta) {
        const int quadrant = 3 - ((i % 4) + 4) % 4;
        const int j = 3 * quadrant;

        QBezier b;
        if (delta > 0)
            b = QBezier::fromPoints(points[j + 3], points[j + 2], points[j + 1], points[j]);
        else
            b = QBezier::fromPoints(points[j], points[j + 1], points[j + 2], points[j + 3]);

        // empty arc?
        if (startSegment == endSegment && qFuzzyCompare(startT, endT))
            return startPoint;

        if (i == startSegment) {
            if (i == endSegment && splitAtEnd)
                b = b.bezierOnInterval(startT, endT);
            else if (splitAtStart)
                b = b.bezierOnInterval(startT, 1);
        } else if (i == endSegment && splitAtEnd) {
            b = b.bezierOnInterval(0, endT);
        }

        // push control points
        curves[(*point_count)++] = b.pt2();
        curves[(*point_count)++] = b.pt3();
        curves[(*point_count)++] = b.pt4();
    }

    Q_ASSERT(*point_count > 0);
    curves[*(point_count)-1] = endPoint;

    return startPoint;
}


static inline void qdashstroker_moveTo(qreal x, qreal y, void *data) {
    ((QStroker *) data)->moveTo(x, y);
}

static inline void qdashstroker_lineTo(qreal x, qreal y, void *data) {
    ((QStroker *) data)->lineTo(x, y);
}

static inline void qdashstroker_cubicTo(qreal, qreal, qreal, qreal, qreal, qreal, void *) {
    Q_ASSERT(false);
//     ((QStroker *) data)->cubicTo(c1x, c1y, c2x, c2y, ex, ey);
}


/*******************************************************************************
 * QDashStroker members
 */
QDashStroker::QDashStroker(QStroker *stroker)
    : m_stroker(stroker), m_dashOffset(0), m_stroke_width(1), m_miter_limit(1)
{
    if (m_stroker) {
        setMoveToHook(qdashstroker_moveTo);
        setLineToHook(qdashstroker_lineTo);
        setCubicToHook(qdashstroker_cubicTo);
    }
}

QVector<qreal> QDashStroker::patternForStyle(Qt::PenStyle style)
{
    const qreal space = 2;
    const qreal dot = 1;
    const qreal dash = 4;

    QVector<qreal> pattern;

    switch (style) {
    case Qt::DashLine:
        pattern << dash << space;
        break;
    case Qt::DotLine:
        pattern << dot << space;
        break;
    case Qt::DashDotLine:
        pattern << dash << space << dot << space;
        break;
    case Qt::DashDotDotLine:
        pattern << dash << space << dot << space << dot << space;
        break;
    default:
        break;
    }

    return pattern;
}

static inline bool lineRectIntersectsRect(qfixed2d p1, qfixed2d p2, const qfixed2d &tl, const qfixed2d &br)
{
    return ((p1.x > tl.x || p2.x > tl.x) && (p1.x < br.x || p2.x < br.x)
        && (p1.y > tl.y || p2.y > tl.y) && (p1.y < br.y || p2.y < br.y));
}

// If the line intersects the rectangle, this function will return true.
static bool lineIntersectsRect(qfixed2d p1, qfixed2d p2, const qfixed2d &tl, const qfixed2d &br)
{
    if (!lineRectIntersectsRect(p1, p2, tl, br))
        return false;
    if (p1.x == p2.x || p1.y == p2.y)
        return true;

    if (p1.y > p2.y)
        qSwap(p1, p2); // make p1 above p2
    qfixed2d u;
    qfixed2d v;
    qfixed2d w = {p2.x - p1.x, p2.y - p1.y};
    if (p1.x < p2.x) {
        // backslash
        u.x = tl.x - p1.x; u.y = br.y - p1.y;
        v.x = br.x - p1.x; v.y = tl.y - p1.y;
    } else {
        // slash
        u.x = tl.x - p1.x; u.y = tl.y - p1.y;
        v.x = br.x - p1.x; v.y = br.y - p1.y;
    }
    qreal val1 = u.x * w.y - u.y * w.x;
    qreal val2 = v.x * w.y - v.y * w.x;
    return (val1 < 0 && val2 > 0) || (val1 > 0 && val2 < 0);
}

void QDashStroker::processCurrentSubpath()
{
    int dashCount = qMin(m_dashPattern.size(), 32);
    QSTACKARRAY(qreal, dashes, 32);

    if (m_stroker) {
        m_customData = m_stroker;
        m_stroke_width = m_stroker->strokeWidth();
        m_miter_limit = m_stroker->miterLimit();
    }

    qreal longestLength = 0;
    qreal sumLength = 0;
    for (int i=0; i<dashCount; ++i) {
        dashes[i] = qMax(m_dashPattern.at(i), qreal(0)) * m_stroke_width;
        sumLength += dashes[i];
        if (dashes[i] > longestLength)
            longestLength = dashes[i];
    }

    if (qFuzzyIsNull(sumLength))
        return;

    qreal invSumLength = qreal(1) / sumLength;

    Q_ASSERT(dashCount > 0);

    dashCount = dashCount & -2; // Round down to even number

    int idash = 0; // Index to current dash
    qreal pos = 0; // The position on the curve, 0 <= pos <= path.length
    qreal elen = 0; // element length
    qreal doffset = m_dashOffset * m_stroke_width;

    // make sure doffset is in range [0..sumLength)
    doffset -= qFloor(doffset * invSumLength) * sumLength;

    while (doffset >= dashes[idash]) {
        doffset -= dashes[idash];
        if (++idash >= dashCount)
            idash = 0;
    }

    qreal estart = 0; // The elements starting position
    qreal estop = 0; // The element stop position

    QLineF cline;

    QSubpathFlatIterator it(&m_elements, m_dashThreshold);
    qfixed2d prev = it.next();

    bool clipping = !m_clip_rect.isEmpty();
    qfixed2d move_to_pos = prev;
    qfixed2d line_to_pos;

    // Pad to avoid clipping the borders of thick pens.
    qreal padding = qreal(qMax(m_stroke_width, m_miter_limit) * longestLength);
    qfixed2d clip_tl = { qreal(m_clip_rect.left()) - padding,
                         qreal(m_clip_rect.top()) - padding };
    qfixed2d clip_br = { qreal(m_clip_rect.right()) + padding ,
                         qreal(m_clip_rect.bottom()) + padding };

    bool hasMoveTo = false;
    while (it.hasNext()) {
        QStrokerOps::Element e = it.next();

        Q_ASSERT(e.isLineTo());
        cline = QLineF(prev.x, prev.y, e.x, e.y);
        elen = cline.length();

        estop = estart + elen;

        bool done = pos >= estop;

        if (clipping) {
            // Check if the entire line can be clipped away.
            if (!lineIntersectsRect(prev, e, clip_tl, clip_br)) {
                // Cut away full dash sequences.
                elen -= qFloor(elen * invSumLength) * sumLength;
                // Update dash offset.
                while (!done) {
                    qreal dpos = pos + dashes[idash] - doffset - estart;

                    Q_ASSERT(dpos >= 0);

                    if (dpos > elen) { // dash extends this line
                        doffset = dashes[idash] - (dpos - elen); // subtract the part already used
                        pos = estop; // move pos to next path element
                        done = true;
                    } else { // Dash is on this line
                        pos = dpos + estart;
                        done = pos >= estop;
                        if (++idash >= dashCount)
                            idash = 0;
                        doffset = 0; // full segment so no offset on next.
                    }
                }
                hasMoveTo = false;
                move_to_pos = e;
            }
        }

        // Dash away...
        while (!done) {
            QPointF p2;

            bool has_offset = doffset > 0;
            bool evenDash = (idash & 1) == 0;
            qreal dpos = pos + dashes[idash] - doffset - estart;

            Q_ASSERT(dpos >= 0);

            if (dpos > elen) { // dash extends this line
                doffset = dashes[idash] - (dpos - elen); // subtract the part already used
                pos = estop; // move pos to next path element
                done = true;
                p2 = cline.p2();
            } else { // Dash is on this line
                p2 = cline.pointAt(dpos/elen);
                pos = dpos + estart;
                done = pos >= estop;
                if (++idash >= dashCount)
                    idash = 0;
                doffset = 0; // full segment so no offset on next.
            }

            if (evenDash) {
                line_to_pos.x = qreal(p2.x());
                line_to_pos.y = qreal(p2.y());

                if (!clipping
                    || lineRectIntersectsRect(move_to_pos, line_to_pos, clip_tl, clip_br))
                {
                    // If we have an offset, we're continuing a dash
                    // from a previous element and should only
                    // continue the current dash, without starting a
                    // new subpath.
                    if (!has_offset || !hasMoveTo) {
                        emitMoveTo(move_to_pos.x, move_to_pos.y);
                        hasMoveTo = true;
                    }

                    emitLineTo(line_to_pos.x, line_to_pos.y);
                } else {
                    hasMoveTo = false;
                }
                move_to_pos = line_to_pos;
            } else {
                move_to_pos.x = qreal(p2.x());
                move_to_pos.y = qreal(p2.y());
            }
        }

        // Shuffle to the next cycle...
        estart = estop;
        prev = e;
    }

}

QT_END_NAMESPACE




