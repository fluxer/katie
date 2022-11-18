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

#include "qsvgstyle_p.h"
#include "qsvgfont_p.h"
#include "qsvggraphics_p.h"
#include "qsvgnode_p.h"
#include "qsvgtinydocument_p.h"
#include "qpainter.h"
#include "qpair.h"
#include "qcolor.h"
#include "qdebug.h"
#include "qmath.h"
#include "qnumeric.h"
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

QSvgExtraStates::QSvgExtraStates()
    : fillOpacity(1.0)
    , strokeOpacity(1.0)
    , svgFont(0)
    , textAnchor(Qt::AlignLeft)
    , fontWeight(400)
    , fillRule(Qt::WindingFill)
    , strokeDashOffset(0)
    , vectorEffect(false)
{
}

QSvgStyleProperty::~QSvgStyleProperty()
{
}

void QSvgFillStyleProperty::apply(QPainter *, const QSvgNode *, QSvgExtraStates &)
{
    Q_ASSERT(!"This should not be called!");
}

void QSvgFillStyleProperty::revert(QPainter *, QSvgExtraStates &)
{
    Q_ASSERT(!"This should not be called!");
}

QSvgFillStyle::QSvgFillStyle()
    : m_style(0)
    , m_fillRule(Qt::WindingFill)
    , m_oldFillRule(Qt::WindingFill)
    , m_fillOpacity(1.0)
    , m_oldFillOpacity(0)
    , m_gradientResolved(true)
    , m_fillRuleSet(false)
    , m_fillOpacitySet(false)
    , m_fillSet(false)
{
}

void QSvgFillStyle::setFillRule(Qt::FillRule f)
{
    m_fillRuleSet = true;
    m_fillRule = f;
}

void QSvgFillStyle::setFillOpacity(qreal opacity)
{
    m_fillOpacitySet = true;
    m_fillOpacity = opacity;
}

void QSvgFillStyle::setFillStyle(QSvgFillStyleProperty* style)
{
    m_style = style;
    m_fillSet = true;
}

void QSvgFillStyle::setBrush(QBrush brush)
{
    m_fill = brush;
    m_style = 0;
    m_fillSet = true;
}

void QSvgFillStyle::apply(QPainter *p, const QSvgNode *, QSvgExtraStates &states)
{
    m_oldFill = p->brush();
    m_oldFillRule = states.fillRule;
    m_oldFillOpacity = states.fillOpacity;

    if (m_fillRuleSet)
        states.fillRule = m_fillRule;
    if (m_fillSet) {
        if (m_style)
            p->setBrush(m_style->brush(p, states));
        else
            p->setBrush(m_fill);
    }
    if (m_fillOpacitySet)
        states.fillOpacity = m_fillOpacity;
}

void QSvgFillStyle::revert(QPainter *p, QSvgExtraStates &states)
{
    if (m_fillOpacitySet)
        states.fillOpacity = m_oldFillOpacity;
    if (m_fillSet)
        p->setBrush(m_oldFill);
    if (m_fillRuleSet)
        states.fillRule = m_oldFillRule;
}

QSvgViewportFillStyle::QSvgViewportFillStyle(const QBrush &brush)
    : m_viewportFill(brush)
{
}

void QSvgViewportFillStyle::apply(QPainter *p, const QSvgNode *, QSvgExtraStates &)
{
    m_oldFill = p->brush();
    p->setBrush(m_viewportFill);
}

void QSvgViewportFillStyle::revert(QPainter *p, QSvgExtraStates &)
{
    p->setBrush(m_oldFill);
}

QSvgFontStyle::QSvgFontStyle(QSvgFont *font, QSvgTinyDocument *doc)
    : m_svgFont(font)
    , m_doc(doc)
    , m_familySet(false)
    , m_sizeSet(false)
    , m_styleSet(false)
    , m_weightSet(false)
    , m_textAnchorSet(false)
{
}

QSvgFontStyle::QSvgFontStyle()
    : m_svgFont(0)
    , m_doc(0)
    , m_familySet(false)
    , m_sizeSet(false)
    , m_styleSet(false)
    , m_weightSet(false)
    , m_textAnchorSet(false)
{
}

int QSvgFontStyle::SVGToQtWeight(int weight) {
    switch (weight) {
    case 100:
    case 200:
        return QFont::Light;
    case 300:
    case 400:
        return QFont::Normal;
    case 500:
    case 600:
        return QFont::DemiBold;
    case 700:
    case 800:
        return QFont::Bold;
    case 900:
        return QFont::Black;
    }
    return QFont::Normal;
}

void QSvgFontStyle::apply(QPainter *p, const QSvgNode *, QSvgExtraStates &states)
{
    m_oldQFont = p->font();
    m_oldSvgFont = states.svgFont;
    m_oldTextAnchor = states.textAnchor;
    m_oldWeight = states.fontWeight;

    if (m_textAnchorSet)
        states.textAnchor = m_textAnchor;

    QFont font = m_oldQFont;
    if (m_familySet) {
        states.svgFont = m_svgFont;
        font.setFamily(m_qfont.family());
    }

    if (m_sizeSet)
        font.setPointSizeF(m_qfont.pointSizeF());

    if (m_styleSet)
        font.setStyle(m_qfont.style());

    if (m_weightSet) {
        if (m_weight == BOLDER) {
            states.fontWeight = qMin(states.fontWeight + 100, 900);
        } else if (m_weight == LIGHTER) {
            states.fontWeight = qMax(states.fontWeight - 100, 100);
        } else {
            states.fontWeight = m_weight;
        }
        font.setWeight(SVGToQtWeight(states.fontWeight));
    }

    p->setFont(font);
}

void QSvgFontStyle::revert(QPainter *p, QSvgExtraStates &states)
{
    p->setFont(m_oldQFont);
    states.svgFont = m_oldSvgFont;
    states.textAnchor = m_oldTextAnchor;
    states.fontWeight = m_oldWeight;
}

QSvgStrokeStyle::QSvgStrokeStyle()
    : m_strokeOpacity(1.0)
    , m_oldStrokeOpacity(0.0)
    , m_strokeDashOffset(0)
    , m_oldStrokeDashOffset(0)
    , m_style(0)
    , m_gradientResolved(true)
    , m_vectorEffect(false)
    , m_oldVectorEffect(false)
    , m_strokeSet(false)
    , m_strokeDashArraySet(false)
    , m_strokeDashOffsetSet(false)
    , m_strokeLineCapSet(false)
    , m_strokeLineJoinSet(false)
    , m_strokeMiterLimitSet(false)
    , m_strokeOpacitySet(false)
    , m_strokeWidthSet(false)
    , m_vectorEffectSet(false)
{
}

void QSvgStrokeStyle::apply(QPainter *p, const QSvgNode *, QSvgExtraStates &states)
{
    m_oldStroke = p->pen();
    m_oldStrokeOpacity = states.strokeOpacity;
    m_oldStrokeDashOffset = states.strokeDashOffset;
    m_oldVectorEffect = states.vectorEffect;

    QPen pen = p->pen();

    qreal oldWidth = pen.widthF();
    qreal width = m_stroke.widthF();
    if (oldWidth == 0)
        oldWidth = 1;
    if (width == 0)
        width = 1;
    qreal scale = oldWidth / width;

    if (m_strokeOpacitySet)
        states.strokeOpacity = m_strokeOpacity;

    if (m_vectorEffectSet)
        states.vectorEffect = m_vectorEffect;

    if (m_strokeSet) {
        if (m_style)
            pen.setBrush(m_style->brush(p, states));
        else
            pen.setBrush(m_stroke.brush());
    }

    if (m_strokeWidthSet)
        pen.setWidthF(m_stroke.widthF());

    bool setDashOffsetNeeded = false;

    if (m_strokeDashOffsetSet) {
        states.strokeDashOffset = m_strokeDashOffset;
        setDashOffsetNeeded = true;
    }

    if (m_strokeDashArraySet) {
        if (m_stroke.style() == Qt::SolidLine) {
            pen.setStyle(Qt::SolidLine);
        } else if (m_strokeWidthSet || oldWidth == 1) {
            // If both width and dash array was set, the dash array is already scaled correctly.
            pen.setDashPattern(m_stroke.dashPattern());
            setDashOffsetNeeded = true;
        } else {
            // If dash array was set, but not the width, the dash array has to be scaled with respect to the old width.
            QVector<qreal> dashes = m_stroke.dashPattern();
            for (int i = 0; i < dashes.size(); ++i)
                dashes[i] /= oldWidth;
            pen.setDashPattern(dashes);
            setDashOffsetNeeded = true;
        }
    } else if (m_strokeWidthSet && pen.style() != Qt::SolidLine && scale != 1) {
        // If the width was set, but not the dash array, the old dash array must be scaled with respect to the new width.
        QVector<qreal> dashes = pen.dashPattern();
        for (int i = 0; i < dashes.size(); ++i)
            dashes[i] *= scale;
        pen.setDashPattern(dashes);
        setDashOffsetNeeded = true;
    }

    if (m_strokeLineCapSet)
        pen.setCapStyle(m_stroke.capStyle());
    if (m_strokeLineJoinSet)
        pen.setJoinStyle(m_stroke.joinStyle());
    if (m_strokeMiterLimitSet)
        pen.setMiterLimit(m_stroke.miterLimit());

    // You can have dash offset on solid strokes in SVG files, but not in Qt.
    // QPen::setDashOffset() will set the pen style to Qt::CustomDashLine,
    // so don't call the method if the pen is solid.
    if (setDashOffsetNeeded && pen.style() != Qt::SolidLine) {
        qreal currentWidth = pen.widthF();
        if (currentWidth == 0)
            currentWidth = 1;
        pen.setDashOffset(states.strokeDashOffset / currentWidth);
    }

    pen.setCosmetic(states.vectorEffect);

    p->setPen(pen);
}

void QSvgStrokeStyle::revert(QPainter *p, QSvgExtraStates &states)
{
    p->setPen(m_oldStroke);
    states.strokeOpacity = m_oldStrokeOpacity;
    states.strokeDashOffset = m_oldStrokeDashOffset;
    states.vectorEffect = m_oldVectorEffect;
}

void QSvgStrokeStyle::setDashArray(const QVector<qreal> &dashes)
{
    if (m_strokeWidthSet) {
        QVector<qreal> d = dashes;
        qreal w = m_stroke.widthF();
        if (w != 0 && w != 1) {
            for (int i = 0; i < d.size(); ++i)
                d[i] /= w;
        }
        m_stroke.setDashPattern(d);
    } else {
        m_stroke.setDashPattern(dashes);
    }
    m_strokeDashArraySet = true;
}

QSvgSolidColorStyle::QSvgSolidColorStyle(const QColor &color)
    : m_solidColor(color)
{
}

QSvgGradientStyle::QSvgGradientStyle(QGradient *grad)
    : m_gradient(grad), m_gradientStopsSet(false)
{
}

QBrush QSvgGradientStyle::brush(QPainter *, QSvgExtraStates &)
{
    if (!m_link.isEmpty()) {
        resolveStops();
    }

    // If the gradient is marked as empty, insert transparent black
    if (!m_gradientStopsSet) {
        m_gradient->setStops(QGradientStops() << QGradientStop(0.0, QColor(0, 0, 0, 0)));
        m_gradientStopsSet = true;
    }

    QBrush b(*m_gradient);

    if (!m_matrix.isIdentity())
        b.setMatrix(m_matrix);

    return b;
}


void QSvgGradientStyle::setMatrix(const QMatrix &mat)
{
    m_matrix = mat;
}

QSvgTransformStyle::QSvgTransformStyle(const QTransform &trans)
    : m_transform(trans)
{
}

void QSvgTransformStyle::apply(QPainter *p, const QSvgNode *, QSvgExtraStates &)
{
    m_oldWorldTransform = p->worldTransform();
    p->setWorldTransform(m_transform, true);
}

void QSvgTransformStyle::revert(QPainter *p, QSvgExtraStates &)
{
    p->setWorldTransform(m_oldWorldTransform, false /* don't combine */);
}

QSvgStyleProperty::Type QSvgFillStyle::type() const
{
    return FILL;
}

QSvgStyleProperty::Type QSvgViewportFillStyle::type() const
{
    return VIEWPORT_FILL;
}

QSvgStyleProperty::Type QSvgFontStyle::type() const
{
    return FONT;
}

QSvgStyleProperty::Type QSvgStrokeStyle::type() const
{
    return STROKE;
}

QSvgStyleProperty::Type QSvgSolidColorStyle::type() const
{
    return SOLID_COLOR;
}

QSvgStyleProperty::Type QSvgGradientStyle::type() const
{
    return GRADIENT;
}

QSvgStyleProperty::Type QSvgTransformStyle::type() const
{
    return TRANSFORM;
}


QSvgCompOpStyle::QSvgCompOpStyle(QPainter::CompositionMode mode)
    : m_mode(mode)
{

}

void QSvgCompOpStyle::apply(QPainter *p, const QSvgNode *, QSvgExtraStates &)
{
    m_oldMode = p->compositionMode();
    p->setCompositionMode(m_mode);
}

void QSvgCompOpStyle::revert(QPainter *p, QSvgExtraStates &)
{
    p->setCompositionMode(m_oldMode);
}

QSvgStyleProperty::Type QSvgCompOpStyle::type() const
{
    return COMP_OP;
}

QSvgStyle::~QSvgStyle()
{
}

void QSvgStyle::apply(QPainter *p, const QSvgNode *node, QSvgExtraStates &states)
{
    if (fill) {
        fill->apply(p, node, states);
    }

    if (viewportFill) {
        viewportFill->apply(p, node, states);
    }

    if (font) {
        font->apply(p, node, states);
    }

    if (stroke) {
        stroke->apply(p, node, states);
    }

    if (transform) {
        transform->apply(p, node, states);
    }

    if (opacity) {
        opacity->apply(p, node, states);
    }

    if (compop) {
        compop->apply(p, node, states);
    }
}

void QSvgStyle::revert(QPainter *p, QSvgExtraStates &states)
{
    if (fill) {
        fill->revert(p, states);
    }

    if (viewportFill) {
        viewportFill->revert(p, states);
    }

    if (font) {
        font->revert(p, states);
    }

    if (stroke) {
        stroke->revert(p, states);
    }

    if (transform) {
        transform->revert(p, states);
    }

    if (opacity) {
        opacity->revert(p, states);
    }

    if (compop) {
        compop->revert(p, states);
    }
}

QSvgOpacityStyle::QSvgOpacityStyle(qreal opacity)
    : m_opacity(opacity), m_oldOpacity(0)
{

}

void QSvgOpacityStyle::apply(QPainter *p, const QSvgNode *, QSvgExtraStates &)
{
    m_oldOpacity = p->opacity();
    p->setOpacity(m_opacity * m_oldOpacity);
}

void QSvgOpacityStyle::revert(QPainter *p, QSvgExtraStates &)
{
    p->setOpacity(m_oldOpacity);
}

QSvgStyleProperty::Type QSvgOpacityStyle::type() const
{
    return OPACITY;
}

void QSvgGradientStyle::setStopLink(const QString &link, QSvgTinyDocument *doc)
{
    m_link = link;
    m_doc  = doc;
}

void QSvgGradientStyle::resolveStops()
{
    if (!m_link.isEmpty() && m_doc) {
        QSvgStyleProperty *prop = m_doc->styleProperty(m_link);
        if (Q_LIKELY(prop && prop != this)) {
            if (prop->type() == QSvgStyleProperty::GRADIENT) {
                QSvgGradientStyle *st = static_cast<QSvgGradientStyle*>(prop);
                st->resolveStops();
                m_gradient->setStops(st->qgradient()->stops());
                m_gradientStopsSet = st->gradientStopsSet();
            }
        } else {
            qWarning("Could not resolve property : %s", qPrintable(m_link));
        }
        m_link = QString();
    }
}

QT_END_NAMESPACE
