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

#include "qsvgfont_p.h"

#include "qpainter.h"
#include "qpen.h"
#include "qdebug.h"

QT_BEGIN_NAMESPACE

QSvgGlyph::QSvgGlyph(QChar unicode, const QPainterPath &path, qreal horizAdvX)
    : m_unicode(unicode), m_path(path), m_horizAdvX(horizAdvX)
{

}


QSvgFont::QSvgFont(qreal horizAdvX)
    : m_horizAdvX(horizAdvX)
{
}


QString QSvgFont::familyName() const
{
    return m_familyName;
}


void QSvgFont::addGlyph(QChar unicode, const QPainterPath &path, qreal horizAdvX )
{
    m_glyphs.insert(unicode, QSvgGlyph(unicode, path,
                                       (horizAdvX==-1)?m_horizAdvX:horizAdvX));
}


void QSvgFont::draw(QPainter *p, const QPointF &point, const QString &str, qreal pixelSize, Qt::Alignment alignment) const
{
    p->save();
    p->translate(point);
    p->scale(pixelSize / m_unitsPerEm, -pixelSize / m_unitsPerEm);

    // Calculate the text width to be used for alignment
    int textWidth = 0;
    QString::const_iterator itr = str.constBegin();
    for ( ; itr != str.constEnd(); ++itr) {
        QChar unicode = *itr;
        if (!m_glyphs.contains(*itr)) {
            unicode = 0;
            if (!m_glyphs.contains(unicode))
                continue;
        }
        textWidth += static_cast<int>(m_glyphs[unicode].m_horizAdvX);
    }

    QPoint alignmentOffset(0, 0);
    if (alignment == Qt::AlignHCenter) {
        alignmentOffset.setX(-textWidth / 2);
    } else if (alignment == Qt::AlignRight) {
        alignmentOffset.setX(-textWidth);
    }

    p->translate(alignmentOffset);

    // since in SVG the embedded font ain't really a path
    // the outline has got to stay untransformed...
    qreal penWidth = p->pen().widthF();
    penWidth /= (pixelSize/m_unitsPerEm);
    QPen pen = p->pen();
    pen.setWidthF(penWidth);
    p->setPen(pen);

    itr = str.constBegin();
    for ( ; itr != str.constEnd(); ++itr) {
        QChar unicode = *itr;
        if (!m_glyphs.contains(*itr)) {
            unicode = 0;
            if (!m_glyphs.contains(unicode))
                continue;
        }
        p->drawPath(m_glyphs[unicode].m_path);
        p->translate(m_glyphs[unicode].m_horizAdvX, 0);
    }

    p->restore();
}

void QSvgFont::setFamilyName(const QString &name)
{
    m_familyName = name;
}

void QSvgFont::setUnitsPerEm(qreal upem)
{
    m_unitsPerEm = upem;
}

QT_END_NAMESPACE
