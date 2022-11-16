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

#include "qdebug.h"
#include "qfontengine_p.h"

#include "qbitmap.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "qvarlengtharray.h"
#include "qmath.h"
#include "qendian.h"
#include "qdrawhelper_p.h"
#include "qcorecommon_p.h"
#include "qguicommon_p.h"

QT_BEGIN_NAMESPACE

// QFontEngine

QFontEngine::QFontEngine()
    : QObject(),
    ref(1)
{
    fsType = 0;
}

QFontEngine::~QFontEngine()
{
}

QFixed QFontEngine::lineThickness() const
{
    // ad hoc algorithm
    int score = fontDef.weight * fontDef.pixelSize;
    int lw = score / 700;

    // looks better with thicker line for small pointsizes
    if (lw < 2 && score >= 1050) lw = 2;
    if (lw == 0) lw = 1;

    return lw;
}

QFixed QFontEngine::underlinePosition() const
{
    return ((lineThickness() * 2) + 3) / 6;
}

QFixed QFontEngine::xHeight() const
{
    QGlyphLayoutArray glyphs(2);
    int nglyphs = 1;
    QChar x((ushort)'x');
    stringToCMap(&x, 1, &glyphs, &nglyphs, QTextEngine::GlyphIndicesOnly);

    glyph_metrics_t bb = boundingBox(glyphs.glyphs[0]);
    return bb.height;
}

QFixed QFontEngine::averageCharWidth() const
{
    QGlyphLayoutArray glyphs(2);
    int nglyphs = 1;
    QChar x((ushort)'x');
    stringToCMap(&x, 1, &glyphs, &nglyphs, QTextEngine::GlyphIndicesOnly);

    glyph_metrics_t bb = boundingBox(glyphs.glyphs[0]);
    return bb.xoff;
}


void QFontEngine::getGlyphPositions(const QGlyphLayout &glyphs, const QPointF &point,
                                    QVarLengthArray<glyph_t> &glyphs_out, QVarLengthArray<QFixedPoint> &positions)
{
    QFixed xpos = QFixed::fromReal(point.x());
    QFixed ypos = QFixed::fromReal(point.y());

    int current = 0;
    positions.resize(glyphs.numGlyphs);
    glyphs_out.resize(glyphs.numGlyphs);
    int i = 0;
    while (i < glyphs.numGlyphs) {
        if (!glyphs.attributes[i].dontPrint) {
            positions[current].x = xpos;
            positions[current].y = ypos;
            glyphs_out[current] = glyphs.glyphs[i];
            xpos += glyphs.advances_x[i];
            ++current;
        }
        ++i;
    }
    positions.resize(current);
    glyphs_out.resize(current);
    Q_ASSERT(positions.size() == glyphs_out.size());
}

void QFontEngine::getGlyphBearings(glyph_t glyph, qreal *leftBearing, qreal *rightBearing)
{
    glyph_metrics_t gi = boundingBox(glyph);
    bool isValid = gi.isValid();
    if (leftBearing != 0)
        *leftBearing = isValid ? gi.x.toReal() : qreal(0.0);
    if (rightBearing != 0)
        *rightBearing = isValid ? (gi.xoff - gi.x - gi.width).toReal() : qreal(0.0);
}

void QFontEngine::addOutlineToPath(qreal x, qreal y, const QGlyphLayout &glyphs, QPainterPath *path)
{
    if (!glyphs.numGlyphs)
        return;

    QVarLengthArray<QFixedPoint> positions;
    QVarLengthArray<glyph_t> positioned_glyphs;
    getGlyphPositions(glyphs, QPointF(x, y), positioned_glyphs, positions);
    addGlyphsToPath(positioned_glyphs.data(), positions.data(), positioned_glyphs.size(), path);
}

QFontEngine::Properties QFontEngine::properties() const
{
    Properties p;
    QByteArray psname = QFontEngine::convertToPostscriptFontFamilyName(fontDef.family.toUtf8());
    psname += '-';
    psname += QByteArray::number(fontDef.style);
    psname += '-';
    psname += QByteArray::number(fontDef.weight);

    p.postscriptName = psname;
    p.ascent = ascent();
    p.descent = descent();
    p.leading = leading();
    p.emSquare = p.ascent;
    p.boundingBox = QRectF(0, -p.ascent.toReal(), maxCharWidth(), (p.ascent + p.descent).toReal());
    p.italicAngle = 0;
    p.capHeight = p.ascent;
    p.lineWidth = lineThickness();
    return p;
}

void QFontEngine::getUnscaledGlyph(glyph_t glyph, QPainterPath *path, glyph_metrics_t *metrics)
{
    *metrics = boundingBox(glyph);
    QFixedPoint p;
    p.x = 0;
    p.y = 0;
    addGlyphsToPath(&glyph, &p, 1, path);
}

QByteArray QFontEngine::getSfntTable(uint tag) const
{
    QByteArray table;
    uint len = 0;
    if (!getSfntTableData(tag, 0, &len))
        return table;
    if (!len)
        return table;
    table.resize(len);
    if (!getSfntTableData(tag, reinterpret_cast<uchar *>(table.data()), &len))
        return QByteArray();
    return table;
}

#if defined(Q_WS_X11)
static inline QFixed kerning(int left, int right, const QFontEngine::KernPair *pairs, int numPairs)
{
    uint left_right = (left << 16) + right;

    left = 0, right = numPairs - 1;
    while (left <= right) {
        int middle = left + ( ( right - left ) >> 1 );

        if(pairs[middle].left_right == left_right)
            return pairs[middle].adjust;

        if (pairs[middle].left_right < left_right)
            left = middle + 1;
        else
            right = middle - 1;
    }
    return 0;
}

void QFontEngine::doKerning(QGlyphLayout *glyphs)
{
    int numPairs = kerning_pairs.size();
    if (!numPairs)
        return;

    const KernPair *pairs = kerning_pairs.constData();

    for (int i = 0; i < glyphs->numGlyphs - 1; ++i) {
        glyphs->advances_x[i] += qRound(kerning(glyphs->glyphs[i], glyphs->glyphs[i+1] , pairs, numPairs));
    }
}

void QFontEngine::loadKerningPairs(QFixed scalingFactor)
{
    kerning_pairs.clear();

    QByteArray tab = getSfntTable(MAKE_TAG('k', 'e', 'r', 'n'));
    if (tab.isEmpty())
        return;

    const uchar *table = reinterpret_cast<const uchar *>(tab.constData());

    unsigned short version = qFromBigEndian<quint16>(table);
    if (version != 0) {
//        qDebug("wrong version");
       return;
    }

    unsigned short numTables = qFromBigEndian<quint16>(table + 2);
    {
        int offset = 4;
        for(int i = 0; i < numTables; ++i) {
            if (offset + 6 > tab.size()) {
//                qDebug("offset out of bounds");
                goto end;
            }
            const uchar *header = table + offset;

            ushort version = qFromBigEndian<quint16>(header);
            ushort length = qFromBigEndian<quint16>(header+2);
            ushort coverage = qFromBigEndian<quint16>(header+4);
//            qDebug("subtable: version=%d, coverage=%x",version, coverage);
            if(version == 0 && coverage == 0x0001) {
                if (offset + length > tab.size()) {
//                    qDebug("length ouf ot bounds");
                    goto end;
                }
                const uchar *data = table + offset + 6;

                ushort nPairs = qFromBigEndian<quint16>(data);
                if(nPairs * 6 + 8 > length - 6) {
//                    qDebug("corrupt table!");
                    // corrupt table
                    goto end;
                }

                int off = 8;
                for(int i = 0; i < nPairs; ++i) {
                    QFontEngine::KernPair p;
                    p.left_right = (((uint)qFromBigEndian<quint16>(data+off)) << 16) + qFromBigEndian<quint16>(data+off+2);
                    p.adjust = QFixed(((int)(short)qFromBigEndian<quint16>(data+off+4))) / scalingFactor;
                    kerning_pairs.append(p);
                    off += 6;
                }
            }
            offset += length;
        }
    }
end:
    qSort(kerning_pairs);
//    for (int i = 0; i < kerning_pairs.count(); ++i)
//        qDebug() << 'i' << i << "left_right" << hex << kerning_pairs.at(i).left_right;
}

#else
void QFontEngine::doKerning(QGlyphLayout *)
{
}
#endif

QByteArray QFontEngine::convertToPostscriptFontFamilyName(const QByteArray &family)
{
    QByteArray f = family;
    f.replace(' ', "");
    f.replace('(', "");
    f.replace(')', "");
    f.replace('<', "");
    f.replace('>', "");
    f.replace('[', "");
    f.replace(']', "");
    f.replace('{', "");
    f.replace('}', "");
    f.replace('/', "");
    f.replace('%', "");
    return f;
}

// ------------------------------------------------------------------
// The box font engine
// ------------------------------------------------------------------

QFontEngineBox::QFontEngineBox(int size)
    : _size(size)
{
}

QFontEngineBox::~QFontEngineBox()
{
}

bool QFontEngineBox::stringToCMap(const QChar *, int len, QGlyphLayout *glyphs, int *nglyphs, QTextEngine::ShaperFlags) const
{
    if (*nglyphs < len) {
        *nglyphs = len;
        return false;
    }

    for (int i = 0; i < len; i++) {
        glyphs->glyphs[i] = 0;
        glyphs->advances_x[i] = _size;
    }

    *nglyphs = len;
    glyphs->numGlyphs = len;
    return true;
}

void QFontEngineBox::recalcAdvances(QGlyphLayout *glyphs) const
{
    for (int i = 0; i < glyphs->numGlyphs; i++) {
        glyphs->advances_x[i] = _size;
    }
}

void QFontEngineBox::addOutlineToPath(qreal x, qreal y, const QGlyphLayout &glyphs, QPainterPath *path)
{
    if (!glyphs.numGlyphs)
        return;

    QVarLengthArray<QFixedPoint> positions;
    QVarLengthArray<glyph_t> positioned_glyphs;
    getGlyphPositions(glyphs, QPointF(x, y - _size), positioned_glyphs, positions);

    const QSize s(_size - 3, _size - 3);
    for (int k = 0; k < positions.size(); k++) {
        path->addRect(QRectF(positions[k].toPointF(), s));
    }
}

glyph_metrics_t QFontEngineBox::boundingBox(const QGlyphLayout &glyphs) const
{
    glyph_metrics_t overall;
    overall.width = _size*glyphs.numGlyphs;
    overall.height = _size;
    overall.xoff = overall.width;
    return overall;
}

glyph_metrics_t QFontEngineBox::boundingBox(glyph_t) const
{
    return glyph_metrics_t(0, -_size, _size, _size, _size);
}

QFixed QFontEngineBox::ascent() const
{
    return _size;
}

QFixed QFontEngineBox::descent() const
{
    return 0;
}

QFixed QFontEngineBox::leading() const
{
    QFixed l = _size * QFixed::fromReal(qreal(0.15));
    return l.ceil();
}

qreal QFontEngineBox::maxCharWidth() const
{
    return _size;
}

const char *QFontEngineBox::name() const
{
    return "null";
}

bool QFontEngineBox::canRender(const QChar *, int)
{
    return true;
}

QFontEngine::Type QFontEngineBox::type() const
{
    return Box;
}


void QFontEngineBox::addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int nGlyphs,
                                  QPainterPath *path)
{
    QGlyphLayoutArray g(nGlyphs);

    for (int i = 0; i < nGlyphs; ++i) {
        g.glyphs[i] = glyphs[i];
        if (i < nGlyphs - 1) {
            g.advances_x[i] = positions[i+1].x - positions[i].x;
        } else {
            g.advances_x[i] = QFixed::fromReal(maxCharWidth());
        }
    }

    QFixed advanceX = positions[0].x;
    QFixed advanceY = positions[0].y;
    for (int i=0; i < g.numGlyphs; ++i) {
        glyph_metrics_t metrics = boundingBox(g.glyphs[i]);
        if (metrics.width.value() == 0 || metrics.height.value() == 0) {
            advanceX += g.advances_x[i];
            continue;
        }

        path->moveTo((advanceX + metrics.x).toReal(), (advanceY + metrics.y).toReal());
        path->addRect(QRectF(0.0, 0.0, _size, _size));
        path->closeSubpath();

        advanceX += g.advances_x[i];
    }
}

QT_END_NAMESPACE
