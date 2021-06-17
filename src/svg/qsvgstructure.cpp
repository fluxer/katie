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

#include "qsvgstructure_p.h"

#ifndef QT_NO_SVG

#include "qsvgnode_p.h"
#include "qsvgstyle_p.h"
#include "qsvgtinydocument_p.h"

#include "qpainter.h"
#include "qlocale.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

QSvgG::QSvgG(QSvgNode *parent)
    : QSvgStructureNode(parent)
{

}

QSvgStructureNode::~QSvgStructureNode()
{
    qDeleteAll(m_renderers);
}

void QSvgG::draw(QPainter *p, QSvgExtraStates &states)
{
    QList<QSvgNode*>::iterator itr = m_renderers.begin();
    applyStyle(p, states);

    while (itr != m_renderers.end()) {
        QSvgNode *node = *itr;
        if ((node->isVisible()) && (node->displayMode() != QSvgNode::NoneMode))
            node->draw(p, states);
        ++itr;
    }
    revertStyle(p, states);
}

QSvgNode::Type QSvgG::type() const
{
    return G;
}

QSvgStructureNode::QSvgStructureNode(QSvgNode *parent)
    :QSvgNode(parent)
{

}

QSvgNode * QSvgStructureNode::scopeNode(const QString &id) const
{
    QSvgTinyDocument *doc = document();
    return doc ? doc->namedNode(id) : 0;
}

void QSvgStructureNode::addChild(QSvgNode *child, const QString &id)
{
    m_renderers.append(child);

    if (id.isEmpty())
        return; //we can't add it to scope without id

    QSvgTinyDocument *doc = document();
    if (doc)
        doc->addNamedNode(id, child);
}

QSvgDefs::QSvgDefs(QSvgNode *parent)
    : QSvgStructureNode(parent)
{
}

void QSvgDefs::draw(QPainter *, QSvgExtraStates &)
{
    //noop
}

QSvgNode::Type QSvgDefs::type() const
{
    return DEFS;
}

inline static bool isSupportedSvgFeature(const QString &str)
{
    static const QLatin1String svgFeatureTbl[] = {
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Text"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Shape"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#SVG"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Structure"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#SolidColor"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Hyperlinking"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#CoreAttribute"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#XlinkAttribute"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#SVG-static"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#OpacityAttribute"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Gradient"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Font"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Image"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#ConditionalProcessing"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Extensibility"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#GraphicsAttribute"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#Prefetch"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#PaintAttribute"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#ConditionalProcessingAttribute"),
        QLatin1String("http://www.w3.org/Graphics/SVG/feature/1.2/#ExternalResourcesRequiredAttribute")
    };

    for (qint16 i = 0; i < 20; i++) {
        if (str == svgFeatureTbl[i])
            return true;
    }
    return false;
}

QSvgSwitch::QSvgSwitch(QSvgNode *parent)
    : QSvgStructureNode(parent)
{
    QLocale locale;
    m_systemLanguage = locale.name().replace(QLatin1Char('_'), QLatin1Char('-'));
    int idx = m_systemLanguage.indexOf(QLatin1Char('-'));
    m_systemLanguagePrefix = m_systemLanguage.mid(0, idx);
}

void QSvgSwitch::draw(QPainter *p, QSvgExtraStates &states)
{
    QList<QSvgNode*>::iterator itr = m_renderers.begin();
    applyStyle(p, states);

    while (itr != m_renderers.end()) {
        QSvgNode *node = *itr;
        if (node->isVisible() && (node->displayMode() != QSvgNode::NoneMode)) {
            const QStringList &features  = node->requiredFeatures();
            const QStringList &extensions = node->requiredExtensions();
            const QStringList &languages = node->requiredLanguages();
            const QStringList &formats = node->requiredFormats();
            const QStringList &fonts = node->requiredFonts();

            bool okToRender = true;
            if (!features.isEmpty()) {
                QStringList::const_iterator sitr = features.constBegin();
                for (; sitr != features.constEnd(); ++sitr) {
                    if (!isSupportedSvgFeature(*sitr)) {
                        okToRender = false;
                        break;
                    }
                }
            }

            if (okToRender && !extensions.isEmpty()) {
                okToRender = false;
            }

            if (okToRender && !languages.isEmpty()) {
                QStringList::const_iterator sitr = languages.constBegin();
                okToRender = false;
                for (; sitr != languages.constEnd(); ++sitr) {
                    if ((*sitr).startsWith(m_systemLanguagePrefix)) {
                        okToRender = true;
                        break;
                    }
                }
            }

            if (okToRender && !formats.isEmpty()) {
                okToRender = false;
            }

            if (okToRender && !fonts.isEmpty()) {
                okToRender = false;
            }

            if (okToRender) {
                node->draw(p, states);
                break;
            }
        }
        ++itr;
    }
    revertStyle(p, states);
}

QSvgNode::Type QSvgSwitch::type() const
{
    return SWITCH;
}

QRectF QSvgStructureNode::bounds(QPainter *p, QSvgExtraStates &states) const
{
    QRectF bounds;
    foreach(const QSvgNode *node, m_renderers) {
        bounds |= node->transformedBounds(p, states);
    }
    return bounds;
}

QSvgNode * QSvgStructureNode::previousSiblingNode(QSvgNode *n) const
{
    QSvgNode *prev = 0;
    foreach(QSvgNode *node, m_renderers) {
        if (node == n)
            return prev;
        prev = node;
    }
    return prev;
}

QT_END_NAMESPACE

#endif // QT_NO_SVG




