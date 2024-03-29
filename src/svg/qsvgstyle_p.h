/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtSvg module of the Katie Toolkit.
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

#ifndef QSVGSTYLE_P_H
#define QSVGSTYLE_P_H

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

#include "QtGui/qpainter.h"
#include "QtGui/qpen.h"
#include "QtGui/qbrush.h"
#include "QtGui/qmatrix.h"
#include "QtGui/qcolor.h"
#include "QtGui/qfont.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

class QPainter;
class QSvgNode;
class QSvgFont;
class QSvgTinyDocument;

template <class T> class QSvgRefCounter
{
public:
    QSvgRefCounter() { t = 0; }
    QSvgRefCounter(T *_t)
    {
        t = _t;
        if (t)
            t->ref();
    }
    QSvgRefCounter(const QSvgRefCounter &other)
    {
        t = other.t;
        if (t)
            t->ref();
    }
    QSvgRefCounter &operator =(T *_t)
    {
        if(_t)
            _t->ref();
        if (t)
            t->deref();
        t = _t;
        return *this;
    }
    QSvgRefCounter &operator =(const QSvgRefCounter &other)
    {
        if(other.t)
            other.t->ref();
        if (t)
            t->deref();
        t = other.t;
        return *this;
    }
    ~QSvgRefCounter()
    {
        if (t)
            t->deref();
    }

    inline T *operator->() const { return t; }
    inline operator T*() const { return t; }

private:
    T *t;
};

class QSvgRefCounted
{
public:
    QSvgRefCounted() { _ref = 0; }
    virtual ~QSvgRefCounted() {}
    void ref() {
        ++_ref;
//        qDebug() << this << ": adding ref, now " << _ref;
    }
    void deref() {
//        qDebug() << this << ": removing ref, now " << _ref;
        if(!--_ref) {
//            qDebug("     deleting");
            delete this;
        }
    }
private:
    int _ref;
};

struct QSvgExtraStates
{
    QSvgExtraStates();

    qreal fillOpacity;
    qreal strokeOpacity;
    QSvgFont *svgFont;
    Qt::Alignment textAnchor;
    int fontWeight;
    Qt::FillRule fillRule;
    qreal strokeDashOffset;
    bool vectorEffect; // true if pen is cosmetic
};

class QSvgStyleProperty : public QSvgRefCounted
{
public:
    enum Type
    {
        FILL,
        VIEWPORT_FILL,
        FONT,
        STROKE,
        SOLID_COLOR,
        GRADIENT,
        TRANSFORM,
        OPACITY,
        COMP_OP
    };
public:
    virtual ~QSvgStyleProperty();
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states) = 0;
    virtual void revert(QPainter *p, QSvgExtraStates &states) =0;
    virtual Type type() const=0;
};

class QSvgFillStyleProperty : public QSvgStyleProperty
{
public:
    virtual QBrush brush(QPainter *p, QSvgExtraStates &states) = 0;
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
};

class QSvgOpacityStyle : public QSvgStyleProperty
{
public:
    QSvgOpacityStyle(qreal opacity);
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
    virtual Type type() const;
private:
    qreal m_opacity;
    qreal m_oldOpacity;
};

class QSvgFillStyle : public QSvgStyleProperty
{
public:
    QSvgFillStyle();
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
    virtual Type type() const;

    void setFillRule(Qt::FillRule f);
    void setFillOpacity(qreal opacity);
    void setFillStyle(QSvgFillStyleProperty* style);
    void setBrush(QBrush brush);

    const QBrush & qbrush() const
    {
        return m_fill;
    }

    qreal fillOpacity() const
    {
        return m_fillOpacity;
    }

    Qt::FillRule fillRule() const
    {
        return m_fillRule;
    }

    QSvgFillStyleProperty* style() const
    {
        return m_style;
    }

    void setGradientId(const QString &Id)
    {
        m_gradientId = Id;
    }

    QString gradientId() const
    {
        return m_gradientId;
    }

    void setGradientResolved(bool resolved)
    {
        m_gradientResolved = resolved;
    }

    bool isGradientResolved() const
    {
        return m_gradientResolved;
    }

private:
    // fill            v 	v 	'inherit' | <Paint.datatype>
    // fill-opacity    v 	v 	'inherit' | <OpacityValue.datatype>
    QBrush m_fill;
    QBrush m_oldFill;
    QSvgFillStyleProperty *m_style;

    Qt::FillRule m_fillRule;
    Qt::FillRule m_oldFillRule;
    qreal m_fillOpacity;
    qreal m_oldFillOpacity;

    QString m_gradientId;
    bool m_gradientResolved;

    bool m_fillRuleSet;
    bool m_fillOpacitySet;
    bool m_fillSet;
};

class QSvgViewportFillStyle : public QSvgStyleProperty
{
public:
    QSvgViewportFillStyle(const QBrush &brush);
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
    virtual Type type() const;

    const QBrush & qbrush() const
    {
        return m_viewportFill;
    }
private:
    // viewport-fill         v 	x 	'inherit' | <Paint.datatype>
    // viewport-fill-opacity 	v 	x 	'inherit' | <OpacityValue.datatype>
    QBrush m_viewportFill;

    QBrush m_oldFill;
};

class QSvgFontStyle : public QSvgStyleProperty
{
public:
    static const int LIGHTER = -1;
    static const int BOLDER = 1;

    QSvgFontStyle(QSvgFont *font, QSvgTinyDocument *doc);
    QSvgFontStyle();
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
    virtual Type type() const;

    void setSize(qreal size)
    {
        // Store the _pixel_ size in the font. Since QFont::setPixelSize() only takes an int, call
        // QFont::SetPointSize() instead. Set proper font size just before rendering.
        m_qfont.setPointSizeF(size);
        m_sizeSet = true;
    }

    void setTextAnchor(Qt::Alignment anchor)
    {
        m_textAnchor = anchor;
        m_textAnchorSet = true;
    }

    void setFamily(const QString &family)
    {
        m_qfont.setFamily(family);
        m_familySet = true;
    }

    void setStyle(QFont::Style fontStyle) {
        m_qfont.setStyle(fontStyle);
        m_styleSet = true;
    }

    static int SVGToQtWeight(int weight);

    void setWeight(int weight)
    {
        m_weight = weight;
        m_weightSet = true;
    }

    QSvgFont * svgFont() const
    {
        return m_svgFont;
    }

    const QFont &qfont() const
    {
        return m_qfont;
    }

    QSvgTinyDocument *doc() const {return m_doc;}

private:
    QSvgFont *m_svgFont;
    QSvgTinyDocument *m_doc;
    QFont m_qfont;

    int m_weight;
    Qt::Alignment m_textAnchor;

    QSvgFont *m_oldSvgFont;
    QFont m_oldQFont;
    Qt::Alignment m_oldTextAnchor;
    int m_oldWeight;

    bool m_familySet;
    bool m_sizeSet;
    bool m_styleSet;
    bool m_weightSet;
    bool m_textAnchorSet;
};

class QSvgStrokeStyle : public QSvgStyleProperty
{
public:
    QSvgStrokeStyle();
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
    virtual Type type() const;

    void setStroke(QBrush brush)
    {
        m_stroke.setBrush(brush);
        m_style = 0;
        m_strokeSet = true;
    }

    void setStyle(QSvgFillStyleProperty *style)
    {
        m_style = style;
        m_strokeSet = true;
    }

    void setDashArray(const QVector<qreal> &dashes);

    void setDashArrayNone()
    {
        m_stroke.setStyle(Qt::SolidLine);
        m_strokeDashArraySet = true;
    }

    void setDashOffset(qreal offset)
    {
        m_strokeDashOffset = offset;
        m_strokeDashOffsetSet = true;
    }

    void setLineCap(Qt::PenCapStyle cap)
    {
        m_stroke.setCapStyle(cap);
        m_strokeLineCapSet = true;
    }

    void setLineJoin(Qt::PenJoinStyle join)
    {
        m_stroke.setJoinStyle(join);
        m_strokeLineJoinSet = true;
    }

    void setMiterLimit(qreal limit)
    {
        m_stroke.setMiterLimit(limit);
        m_strokeMiterLimitSet = true;
    }

    void setOpacity(qreal opacity)
    {
        m_strokeOpacity = opacity;
        m_strokeOpacitySet = true;
    }

    void setWidth(qreal width)
    {
        m_stroke.setWidthF(width);
        m_strokeWidthSet = true;
        Q_ASSERT(!m_strokeDashArraySet); // set width before dash array.
    }

    qreal width()
    {
        return m_stroke.widthF();
    }

    void setVectorEffect(bool nonScalingStroke)
    {
        m_vectorEffect = nonScalingStroke;
        m_vectorEffectSet = true;
    }

    QSvgFillStyleProperty* style() const
    {
        return m_style;
    }

    void setGradientId(const QString &Id)
    {
        m_gradientId = Id;
    }

    QString gradientId() const
    {
        return m_gradientId;
    }

    void setGradientResolved(bool resolved)
    {
        m_gradientResolved = resolved;
    }

    bool isGradientResolved() const
    {
        return m_gradientResolved;
    }

    QPen stroke() const
    {
        return m_stroke;
    }

private:
    // stroke            v 	v 	'inherit' | <Paint.datatype>
    // stroke-dasharray  v 	v 	'inherit' | <StrokeDashArrayValue.datatype>
    // stroke-dashoffset v 	v 	'inherit' | <StrokeDashOffsetValue.datatype>
    // stroke-linecap    v 	v 	'butt' | 'round' | 'square' | 'inherit'
    // stroke-linejoin   v 	v 	'miter' | 'round' | 'bevel' | 'inherit'
    // stroke-miterlimit v 	v 	'inherit' | <StrokeMiterLimitValue.datatype>
    // stroke-opacity    v 	v 	'inherit' | <OpacityValue.datatype>
    // stroke-width      v 	v 	'inherit' | <StrokeWidthValue.datatype>
    QPen m_stroke;
    QPen m_oldStroke;
    qreal m_strokeOpacity;
    qreal m_oldStrokeOpacity;
    qreal m_strokeDashOffset;
    qreal m_oldStrokeDashOffset;

    QSvgFillStyleProperty *m_style;
    QString m_gradientId;
    bool m_gradientResolved;
    bool m_vectorEffect;
    bool m_oldVectorEffect;

    bool m_strokeSet;
    bool m_strokeDashArraySet;
    bool m_strokeDashOffsetSet;
    bool m_strokeLineCapSet;
    bool m_strokeLineJoinSet;
    bool m_strokeMiterLimitSet;
    bool m_strokeOpacitySet;
    bool m_strokeWidthSet;
    bool m_vectorEffectSet;
};

class QSvgSolidColorStyle : public QSvgFillStyleProperty
{
public:
    QSvgSolidColorStyle(const QColor &color);
    virtual Type type() const;

    const QColor & qcolor() const
    {
        return m_solidColor;
    }

    QBrush brush(QPainter *, QSvgExtraStates &)
    {
        return m_solidColor;
    }

private:
    // solid-color       v 	x 	'inherit' | <SVGColor.datatype>
    // solid-opacity     v 	x 	'inherit' | <OpacityValue.datatype>
    QColor m_solidColor;

    QBrush m_oldFill;
    QPen   m_oldStroke;
};

class QSvgGradientStyle : public QSvgFillStyleProperty
{
public:
    QSvgGradientStyle(QGradient *grad);
    ~QSvgGradientStyle() { delete m_gradient; }
    virtual Type type() const;

    void setStopLink(const QString &link, QSvgTinyDocument *doc);
    QString stopLink() const { return m_link; }
    void resolveStops();

    void setMatrix(const QMatrix &matrix);
    QMatrix  qmatrix() const
    {
        return m_matrix;
    }

    QGradient *qgradient() const
    {
        return m_gradient;
    }

    bool gradientStopsSet() const
    {
        return m_gradientStopsSet;
    }

    void setGradientStopsSet(bool set)
    {
        m_gradientStopsSet = set;
    }

    QBrush brush(QPainter *, QSvgExtraStates &);
private:
    QGradient      *m_gradient;
    QMatrix m_matrix;

    QSvgTinyDocument *m_doc;
    QString           m_link;
    bool m_gradientStopsSet;
};

class QSvgTransformStyle : public QSvgStyleProperty
{
public:
    QSvgTransformStyle(const QTransform &transform);
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
    virtual Type type() const;

    const QTransform & qtransform() const
    {
        return m_transform;
    }
private:
    //7.6 The transform  attribute
    QTransform m_transform;
    QTransform m_oldWorldTransform;
};


class QSvgCompOpStyle : public QSvgStyleProperty
{
public:
    QSvgCompOpStyle(QPainter::CompositionMode mode);
    virtual void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    virtual void revert(QPainter *p, QSvgExtraStates &states);
    virtual Type type() const;

    const QPainter::CompositionMode & compOp() const
    {
        return m_mode;
    }
private:
    //comp-op attribute
    QPainter::CompositionMode m_mode;

    QPainter::CompositionMode m_oldMode;
};


class QSvgStyle
{
public:
    QSvgStyle()
        : fill(0),
          viewportFill(0),
          font(0),
          stroke(0),
          solidColor(0),
          gradient(0),
          transform(0),
          opacity(0),
          compop(0)
    {}
    ~QSvgStyle();

    void apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states);
    void revert(QPainter *p, QSvgExtraStates &states);

    QSvgRefCounter<QSvgFillStyle>         fill;
    QSvgRefCounter<QSvgViewportFillStyle> viewportFill;
    QSvgRefCounter<QSvgFontStyle>         font;
    QSvgRefCounter<QSvgStrokeStyle>       stroke;
    QSvgRefCounter<QSvgSolidColorStyle>   solidColor;
    QSvgRefCounter<QSvgGradientStyle>     gradient;
    QSvgRefCounter<QSvgTransformStyle>    transform;
    QSvgRefCounter<QSvgOpacityStyle>      opacity;
    QSvgRefCounter<QSvgCompOpStyle>       compop;
};

/********************************************************/
// NOT implemented:

// color           v 	v 	'inherit' | <Color.datatype>
//QColor m_color;

// display         v 	x 	'inline' | 'block' | 'list-item'
//                                 | 'run-in' | 'compact' | 'marker' |
//                                 'table' | 'inline-table' |
//                                 'table-row-group' | 'table-header-group' |
//                                 'table-footer-group' | 'table-row' |
//                                 'table-column-group' | 'table-column' |
//                                 'table-cell' | 'table-caption' |
//                                 'none' | 'inherit'
//QSvgDisplayStyle m_display;

// display-align   v 	v 	'auto' | 'before' | 'center' | 'after' | 'inherit'
//QSvgDisplayAlign m_displayAlign;

// line-increment  v 	v 	'auto' | 'inherit' | <Number.datatype>
//int m_lineIncrement;

// text-anchor       v 	v 	'start' | 'middle' | 'end' | 'inherit'
//QSvgTextAnchor m_textAnchor;

// visibility 	v 	v 	'visible' | 'hidden' | 'inherit'
//QSvgVisibility m_visibility;

/******************************************************/
// the following do not make sense for us

// pointer-events  v 	v 	'visiblePainted' | 'visibleFill' | 'visibleStroke' |
//                              'visible' | 'painted' | 'fill' | 'stroke' | 'all' |
//                              'none' | 'inherit'
//QSvgPointEvents m_pointerEvents;

// audio-level     v  	x  	'inherit' | <Number.datatype>

QT_END_NAMESPACE

#endif // QSVGSTYLE_P_H
