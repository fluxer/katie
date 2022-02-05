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

#include "qsvgtinydocument_p.h"

#ifndef QT_NO_SVG

#include "qsvghandler_p.h"
#include "qsvgfont_p.h"
#include "qplatformdefs.h"
#include "qpainter.h"
#include "qfile.h"
#include "qbuffer.h"
#include "qbytearray.h"
#include "qqueue.h"
#include "qstack.h"
#include "qdebug.h"
#include "qcorecommon_p.h"

QT_BEGIN_NAMESPACE

QSvgTinyDocument::QSvgTinyDocument()
    : QSvgStructureNode(0)
    , m_animated(false)
    , m_animationDuration(0)
    , m_fps(30)
{
}

QSvgTinyDocument::~QSvgTinyDocument()
{
}

QSvgTinyDocument * QSvgTinyDocument::load(const QString &fileName)
{
    QFile file(fileName);
    if (Q_UNLIKELY(!file.open(QFile::ReadOnly))) {
        qWarning("Cannot open file '%s', because: %s",
                 qPrintable(fileName), qPrintable(file.errorString()));
        return nullptr;
    }

    return load(file.readAll());
}

QSvgTinyDocument * QSvgTinyDocument::load(const QByteArray &contents)
{
    // Check for gzip magic number and inflate if appropriate
    if (contents.startsWith("\x1f\x8b")) {
        return load(qUncompress(contents.constData(), contents.size()));
    }

    QSvgHandler handler(contents);

    QSvgTinyDocument *doc = nullptr;
    if (Q_UNLIKELY(!handler.ok())) {
        qWarning("Cannot read SVG, because: %s (line %d)",
                 qPrintable(handler.errorString()), handler.lineNumber());
    } else {
        doc = handler.document();
        doc->m_animationDuration = handler.animationDuration();
    }
    return doc;
}

QSvgTinyDocument * QSvgTinyDocument::load(QXmlStreamReader *contents)
{
    QSvgHandler handler(contents);

    QSvgTinyDocument *doc = nullptr;
    if (Q_UNLIKELY(!handler.ok())) {
        qWarning("Cannot read SVG, because: %s (line %d)",
                 qPrintable(handler.errorString()), handler.lineNumber());
    } else {
        doc = handler.document();
        doc->m_animationDuration = handler.animationDuration();
    }
    return doc;
}

void QSvgTinyDocument::draw(QPainter *p, const QRectF &bounds)
{
    if (m_time.isNull()) {
        m_time.start();
    }

    if (displayMode() == QSvgNode::NoneMode)
        return;

    p->save();
    //sets default style on the painter
    //### not the most optimal way
    mapSourceToTarget(p, bounds, QRectF());
    QPen pen(Qt::NoBrush, 1, Qt::SolidLine, Qt::FlatCap, Qt::SvgMiterJoin);
    pen.setMiterLimit(4);
    p->setPen(pen);
    p->setBrush(Qt::black);
    QList<QSvgNode*>::iterator itr = m_renderers.begin();
    applyStyle(p, m_states);
    while (itr != m_renderers.end()) {
        QSvgNode *node = *itr;
        if ((node->isVisible()) && (node->displayMode() != QSvgNode::NoneMode))
            node->draw(p, m_states);
        ++itr;
    }
    revertStyle(p, m_states);
    p->restore();
}


void QSvgTinyDocument::draw(QPainter *p, const QString &id,
                            const QRectF &bounds)
{
    QSvgNode *node = scopeNode(id);

    if (Q_UNLIKELY(!node)) {
        qDebug("Couldn't find node %s. Skipping rendering.", qPrintable(id));
        return;
    }
    if (m_time.isNull()) {
        m_time.start();
    }

    if (node->displayMode() == QSvgNode::NoneMode)
        return;

    p->save();

    mapSourceToTarget(p, bounds, node->transformedBounds());
    QTransform originalTransform = p->worldTransform();

    //XXX set default style on the painter
    QPen pen(Qt::NoBrush, 1, Qt::SolidLine, Qt::FlatCap, Qt::SvgMiterJoin);
    pen.setMiterLimit(4);
    p->setPen(pen);
    p->setBrush(Qt::black);

    QStack<QSvgNode*> parentApplyStack;
    QSvgNode *parent = node->parent();
    while (parent) {
        parentApplyStack.push(parent);
        parent = parent->parent();
    }

    for (int i = parentApplyStack.size() - 1; i >= 0; --i)
        parentApplyStack[i]->applyStyle(p, m_states);

    // Reset the world transform so that our parents don't affect
    // the position
    QTransform currentTransform = p->worldTransform();
    p->setWorldTransform(originalTransform);

    node->draw(p, m_states);

    p->setWorldTransform(currentTransform);

    for (int i = 0; i < parentApplyStack.size(); ++i)
        parentApplyStack[i]->revertStyle(p, m_states);

    //p->fillRect(bounds.adjusted(-5, -5, 5, 5), QColor(0, 0, 255, 100));

    p->restore();
}


QSvgNode::Type QSvgTinyDocument::type() const
{
    return DOC;
}

void QSvgTinyDocument::setWidth(int len)
{
    m_size.setWidth(len);
}

void QSvgTinyDocument::setHeight(int len)
{
    m_size.setHeight(len);
}

void QSvgTinyDocument::setViewBox(const QRectF &rect)
{
    m_viewBox = rect;
}

void QSvgTinyDocument::addSvgFont(QSvgFont *font)
{
    m_fonts.insert(font->familyName(), font);
}

QSvgFont * QSvgTinyDocument::svgFont(const QString &family) const
{
    return m_fonts[family];
}

void QSvgTinyDocument::addNamedNode(const QString &id, QSvgNode *node)
{
    m_namedNodes.insert(id, node);
}

QSvgNode *QSvgTinyDocument::namedNode(const QString &id) const
{
    return m_namedNodes.value(id);
}

void QSvgTinyDocument::addNamedStyle(const QString &id, QSvgFillStyleProperty *style)
{
    m_namedStyles.insert(id, style);
}

QSvgFillStyleProperty *QSvgTinyDocument::namedStyle(const QString &id) const
{
    return m_namedStyles.value(id);
}

void QSvgTinyDocument::restartAnimation()
{
    m_time.restart();
}

bool QSvgTinyDocument::animated() const
{
    return m_animated;
}

void QSvgTinyDocument::setAnimated(bool a)
{
    m_animated = a;
}

void QSvgTinyDocument::draw(QPainter *p, QSvgExtraStates &)
{
    draw(p, QRectF());
}

void QSvgTinyDocument::mapSourceToTarget(QPainter *p, const QRectF &targetRect, const QRectF &sourceRect)
{
    QRectF target = targetRect;
    if (target.isNull()) {
        QPaintDevice *dev = p->device();
        QRectF deviceRect(0, 0, dev->width(), dev->height());
        if (deviceRect.isNull()) {
            if (sourceRect.isNull())
                target = QRectF(QPointF(0, 0), size());
            else
                target = QRectF(QPointF(0, 0), sourceRect.size());
        } else {
            target = deviceRect;
        }
    }

    QRectF source = sourceRect;
    if (source.isNull())
        source = viewBox();

    if (source != target && !source.isNull()) {
        QTransform transform;
        transform.scale(target.width() / source.width(),
                  target.height() / source.height());
        QRectF c2 = transform.mapRect(source);
        p->translate(target.x() - c2.x(),
                     target.y() - c2.y());
        p->scale(target.width() / source.width(),
                 target.height() / source.height());
    }
}

QRectF QSvgTinyDocument::boundsOnElement(const QString &id) const
{
    const QSvgNode *node = scopeNode(id);
    if (!node)
        return transformedBounds();
    return node->transformedBounds();
}

bool QSvgTinyDocument::elementExists(const QString &id) const
{
    QSvgNode *node = scopeNode(id);

    return (node!=0);
}

QMatrix QSvgTinyDocument::matrixForElement(const QString &id) const
{
    QSvgNode *node = scopeNode(id);

    if (Q_UNLIKELY(!node)) {
        qDebug("Couldn't find node %s. Skipping rendering.", qPrintable(id));
        return QMatrix();
    }

    QTransform t;

    node = node->parent();
    while (node) {
        if (node->m_style.transform)
            t *= node->m_style.transform->qtransform();
        node = node->parent();
    }
    
    return t.toAffine();
}

int QSvgTinyDocument::currentFrame() const
{
    double runningPercentage = qMin(m_time.elapsed()/double(m_animationDuration), 1.);

    int totalFrames = m_fps * m_animationDuration;

    return int(runningPercentage * totalFrames);
}

void QSvgTinyDocument::setCurrentFrame(int frame)
{
    const int totalFrames = m_fps * m_animationDuration;
    const int framePercentage = frame / totalFrames;
    const int timeForFrame = m_animationDuration * framePercentage * 1000; //in ms
    const int timeToAdd = timeForFrame - m_time.elapsed();
    m_time = m_time.addMSecs(timeToAdd);
}

void QSvgTinyDocument::setFramesPerSecond(int num)
{
    m_fps = num;
}

QT_END_NAMESPACE

#endif // QT_NO_SVG




