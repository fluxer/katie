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
#include "qharfbuzz_p.h"
#include "qdrawhelper_p.h"
#include "qcorecommon_p.h"
#include "qguicommon_p.h"

QT_BEGIN_NAMESPACE

// Harfbuzz helper functions

static HB_Bool hb_stringToGlyphs(HB_Font font, const HB_UChar16 *string, hb_uint32 length, HB_Glyph *glyphs, hb_uint32 *numGlyphs, HB_Bool rightToLeft)
{
    QFontEngine *fe = (QFontEngine *)font->userData;

    QVarLengthGlyphLayoutArray qglyphs(*numGlyphs);

    QTextEngine::ShaperFlags shaperFlags(QTextEngine::GlyphIndicesOnly);
    if (rightToLeft)
        shaperFlags |= QTextEngine::RightToLeft;

    int nGlyphs = *numGlyphs;
    bool result = fe->stringToCMap(reinterpret_cast<const QChar *>(string), length, &qglyphs, &nGlyphs, shaperFlags);
    *numGlyphs = nGlyphs;
    if (!result)
        return false;

    for (hb_uint32 i = 0; i < *numGlyphs; ++i)
        glyphs[i] = qglyphs.glyphs[i];

    return true;
}

static void hb_getAdvances(HB_Font font, const HB_Glyph *glyphs, hb_uint32 numGlyphs, HB_Fixed *advances, int flags)
{
    QFontEngine *fe = (QFontEngine *)font->userData;

    QVarLengthGlyphLayoutArray qglyphs(numGlyphs);

    for (hb_uint32 i = 0; i < numGlyphs; ++i)
        qglyphs.glyphs[i] = glyphs[i];

    fe->recalcAdvances(&qglyphs, flags & HB_ShaperFlag_UseDesignMetrics ? QFlags<QTextEngine::ShaperFlag>(QTextEngine::DesignMetrics) : QFlags<QTextEngine::ShaperFlag>(0));

    for (hb_uint32 i = 0; i < numGlyphs; ++i)
        advances[i] = qglyphs.advances_x[i].value();
}

static HB_Bool hb_canRender(HB_Font font, const HB_UChar16 *string, hb_uint32 length)
{
    QFontEngine *fe = (QFontEngine *)font->userData;
    return fe->canRender(reinterpret_cast<const QChar *>(string), length);
}

static void hb_getGlyphMetrics(HB_Font font, HB_Glyph glyph, HB_GlyphMetrics *metrics)
{
    QFontEngine *fe = (QFontEngine *)font->userData;
    glyph_metrics_t m = fe->boundingBox(glyph);
    metrics->x = m.x.value();
    metrics->y = m.y.value();
    metrics->width = m.width.value();
    metrics->height = m.height.value();
    metrics->xOffset = m.xoff.value();
    metrics->yOffset = m.yoff.value();
}

static HB_Fixed hb_getFontMetric(HB_Font font, HB_FontMetric metric)
{
    QFontEngine *fe = (QFontEngine *)font->userData;
    switch (metric) {
    case HB_FontAscent:
        return fe->ascent().value();
    default:
        return 0;
    }
}

HB_Error QFontEngine::getPointInOutline(HB_Glyph glyph, int flags, hb_uint32 point, HB_Fixed *xpos, HB_Fixed *ypos, hb_uint32 *nPoints)
{
    Q_UNUSED(glyph)
    Q_UNUSED(flags)
    Q_UNUSED(point)
    Q_UNUSED(xpos)
    Q_UNUSED(ypos)
    Q_UNUSED(nPoints)
    return HB_Err_Not_Covered;
}

static HB_Error hb_getPointInOutline(HB_Font font, HB_Glyph glyph, int flags, hb_uint32 point, HB_Fixed *xpos, HB_Fixed *ypos, hb_uint32 *nPoints)
{
    QFontEngine *fe = (QFontEngine *)font->userData;
    return fe->getPointInOutline(glyph, flags, point, xpos, ypos, nPoints);
}

static const HB_FontClass hb_fontClass = {
    hb_stringToGlyphs, hb_getAdvances, hb_canRender, hb_getPointInOutline,
    hb_getGlyphMetrics, hb_getFontMetric
};

static HB_Error hb_getSFntTable(void *font, HB_Tag tableTag, HB_Byte *buffer, HB_UInt *length)
{
    QFontEngine *fe = (QFontEngine *)font;
    if (!fe->getSfntTableData(tableTag, buffer, length))
        return HB_Err_Invalid_Argument;
    return HB_Err_Ok;
}

// QFontEngine

QFontEngine::QFontEngine()
    : QObject(),
    ref(0)
{
    fsType = 0;
    memset(&hbFont, 0, sizeof(hbFont));
    hbFont.klass = &hb_fontClass;
    hbFont.userData = this;

    hbFace = qHBNewFace(this, hb_getSFntTable);
}

QFontEngine::~QFontEngine()
{
    qHBFreeFace(hbFace);
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

HB_Font QFontEngine::harfbuzzFont() const
{
    if (!hbFont.x_ppem) {
        QFixed emSquare = emSquareSize();
        hbFont.x_ppem = fontDef.pixelSize;
        hbFont.y_ppem = fontDef.pixelSize * fontDef.stretch / 100;
        hbFont.x_scale = (QFixed(hbFont.x_ppem * (1 << 16)) / emSquare).value();
        hbFont.y_scale = (QFixed(hbFont.y_ppem * (1 << 16)) / emSquare).value();
    }
    return &hbFont;
}

HB_Face QFontEngine::harfbuzzFace() const
{
    return hbFace;
}

QFixed QFontEngine::xHeight() const
{
    QGlyphLayoutArray<8> glyphs;
    int nglyphs = 7;
    QChar x((ushort)'x');
    stringToCMap(&x, 1, &glyphs, &nglyphs, QTextEngine::GlyphIndicesOnly);

    glyph_metrics_t bb = boundingBox(glyphs.glyphs[0]);
    return bb.height;
}

QFixed QFontEngine::averageCharWidth() const
{
    QGlyphLayoutArray<8> glyphs;
    int nglyphs = 7;
    QChar x((ushort)'x');
    stringToCMap(&x, 1, &glyphs, &nglyphs, QTextEngine::GlyphIndicesOnly);

    glyph_metrics_t bb = boundingBox(glyphs.glyphs[0]);
    return bb.xoff;
}


void QFontEngine::getGlyphPositions(const QGlyphLayout &glyphs, const QTransform &matrix, QTextItem::RenderFlags flags,
                                    QVarLengthArray<glyph_t> &glyphs_out, QVarLengthArray<QFixedPoint> &positions)
{
    QFixed xpos;
    QFixed ypos;

    const bool transform = matrix.m11() != 1.
                           || matrix.m12() != 0.
                           || matrix.m21() != 0.
                           || matrix.m22() != 1.;
    if (!transform) {
        xpos = QFixed::fromReal(matrix.dx());
        ypos = QFixed::fromReal(matrix.dy());
    }

    int current = 0;
    if (flags & QTextItem::RightToLeft) {
        int i = glyphs.numGlyphs;
        int totalKashidas = 0;
        while(i--) {
            if (glyphs.attributes[i].dontPrint)
                continue;
            xpos += glyphs.advances_x[i] + QFixed::fromFixed(glyphs.justifications[i].space_18d6);
            ypos += glyphs.advances_y[i];
            totalKashidas += glyphs.justifications[i].nKashidas;
        }
        positions.resize(glyphs.numGlyphs+totalKashidas);
        glyphs_out.resize(glyphs.numGlyphs+totalKashidas);

        i = 0;
        while(i < glyphs.numGlyphs) {
            if (glyphs.attributes[i].dontPrint) {
                ++i;
                continue;
            }
            xpos -= glyphs.advances_x[i];
            ypos -= glyphs.advances_y[i];

            QFixed gpos_x = xpos + glyphs.offsets[i].x;
            QFixed gpos_y = ypos + glyphs.offsets[i].y;
            if (transform) {
                QPointF gpos(gpos_x.toReal(), gpos_y.toReal());
                gpos = gpos * matrix;
                gpos_x = QFixed::fromReal(gpos.x());
                gpos_y = QFixed::fromReal(gpos.y());
            }
            positions[current].x = gpos_x;
            positions[current].y = gpos_y;
            glyphs_out[current] = glyphs.glyphs[i];
            ++current;
            if (glyphs.justifications[i].nKashidas) {
                QChar ch(0x640); // Kashida character
                QGlyphLayoutArray<8> g;
                int nglyphs = 7;
                stringToCMap(&ch, 1, &g, &nglyphs, 0);
                for (uint k = 0; k < glyphs.justifications[i].nKashidas; ++k) {
                    xpos -= g.advances_x[0];
                    ypos -= g.advances_y[0];

                    QFixed gpos_x = xpos + glyphs.offsets[i].x;
                    QFixed gpos_y = ypos + glyphs.offsets[i].y;
                    if (transform) {
                        QPointF gpos(gpos_x.toReal(), gpos_y.toReal());
                        gpos = gpos * matrix;
                        gpos_x = QFixed::fromReal(gpos.x());
                        gpos_y = QFixed::fromReal(gpos.y());
                    }
                    positions[current].x = gpos_x;
                    positions[current].y = gpos_y;
                    glyphs_out[current] = g.glyphs[0];
                    ++current;
                }
            } else {
                xpos -= QFixed::fromFixed(glyphs.justifications[i].space_18d6);
            }
            ++i;
        }
    } else {
        positions.resize(glyphs.numGlyphs);
        glyphs_out.resize(glyphs.numGlyphs);
        int i = 0;
        if (!transform) {
            while (i < glyphs.numGlyphs) {
                if (!glyphs.attributes[i].dontPrint) {
                    positions[current].x = xpos + glyphs.offsets[i].x;
                    positions[current].y = ypos + glyphs.offsets[i].y;
                    glyphs_out[current] = glyphs.glyphs[i];
                    xpos += glyphs.advances_x[i] + QFixed::fromFixed(glyphs.justifications[i].space_18d6);
                    ypos += glyphs.advances_y[i];
                    ++current;
                }
                ++i;
            }
        } else {
            while (i < glyphs.numGlyphs) {
                if (!glyphs.attributes[i].dontPrint) {
                    QFixed gpos_x = xpos + glyphs.offsets[i].x;
                    QFixed gpos_y = ypos + glyphs.offsets[i].y;
                    QPointF gpos(gpos_x.toReal(), gpos_y.toReal());
                    gpos = gpos * matrix;
                    positions[current].x = QFixed::fromReal(gpos.x());
                    positions[current].y = QFixed::fromReal(gpos.y());
                    glyphs_out[current] = glyphs.glyphs[i];
                    xpos += glyphs.advances_x[i] + QFixed::fromFixed(glyphs.justifications[i].space_18d6);
                    ypos += glyphs.advances_y[i];
                    ++current;
                }
                ++i;
            }
        }
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

glyph_metrics_t QFontEngine::tightBoundingBox(const QGlyphLayout &glyphs)
{
    glyph_metrics_t overall;

    QFixed ymax = 0;
    QFixed xmax = 0;
    for (int i = 0; i < glyphs.numGlyphs; i++) {
        glyph_metrics_t bb = boundingBox(glyphs.glyphs[i]);
        QFixed x = overall.xoff + glyphs.offsets[i].x + bb.x;
        QFixed y = overall.yoff + glyphs.offsets[i].y + bb.y;
        overall.x = qMin(overall.x, x);
        overall.y = qMin(overall.y, y);
        xmax = qMax(xmax, x + bb.width);
        ymax = qMax(ymax, y + bb.height);
        overall.xoff += bb.xoff;
        overall.yoff += bb.yoff;
    }
    overall.height = qMax(overall.height, ymax - overall.y);
    overall.width = xmax - overall.x;

    return overall;
}


void QFontEngine::addOutlineToPath(qreal x, qreal y, const QGlyphLayout &glyphs, QPainterPath *path,
                                   QTextItem::RenderFlags flags)
{
    if (!glyphs.numGlyphs)
        return;

    QVarLengthArray<QFixedPoint> positions;
    QVarLengthArray<glyph_t> positioned_glyphs;
    QTransform matrix = QTransform::fromTranslate(x, y);
    getGlyphPositions(glyphs, matrix, flags, positioned_glyphs, positions);
    addGlyphsToPath(positioned_glyphs.data(), positions.data(), positioned_glyphs.size(), path, flags);
}

void QFontEngine::addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int nGlyphs,
                                  QPainterPath *path, QTextItem::RenderFlags flags)
{
    QVarLengthGlyphLayoutArray g(nGlyphs);

    for (int i = 0; i < nGlyphs; ++i) {
        g.glyphs[i] = glyphs[i];
        if (i < nGlyphs - 1) {
            g.advances_x[i] = positions[i+1].x - positions[i].x;
            g.advances_y[i] = positions[i+1].y - positions[i].y;
        } else {
            g.advances_x[i] = QFixed::fromReal(maxCharWidth());
            g.advances_y[i] = 0;
        }
    }

    // TODO what to do with 'flags' ??
    Q_UNUSED(flags);
    QFixed advanceX = positions[0].x;
    QFixed advanceY = positions[0].y;
    for (int i=0; i < g.numGlyphs; ++i) {
        glyph_metrics_t metrics = boundingBox(g.glyphs[i]);
        if (metrics.width.value() == 0 || metrics.height.value() == 0) {
            advanceX += g.advances_x[i];
            advanceY += g.advances_y[i];
            continue;
        }
        const QImage alphaMask = alphaMapForGlyph(g.glyphs[i]);

        const int w = alphaMask.width();
        const int h = alphaMask.height();
        const int srcBpl = alphaMask.bytesPerLine();
        Q_ASSERT(alphaMask.depth() == 1);
        const uchar *bitmap_data = alphaMask.constBits();
        QFixedPoint offset = g.offsets[i];
        advanceX += offset.x;
        advanceY += offset.y;

        path->moveTo((advanceX + metrics.x).toReal(), (advanceY + metrics.y).toReal());
        path->addRect(alphaMask.rect());
        path->closeSubpath();

        advanceX += g.advances_x[i];
        advanceY += g.advances_y[i];
    }
}

QImage QFontEngine::alphaMapForGlyph(glyph_t glyph)
{
    glyph_metrics_t gm = boundingBox(glyph);
    int glyph_x = qFloor(gm.x.toReal());
    int glyph_y = qFloor(gm.y.toReal());
    int glyph_width = qCeil((gm.x + gm.width).toReal()) -  glyph_x;
    int glyph_height = qCeil((gm.y + gm.height).toReal()) - glyph_y;

    if (glyph_width <= 0 || glyph_height <= 0)
        return QImage();
    QImage im(glyph_width, glyph_height, QImage::Format_Mono);
    im.fill(0);
    im.setColorTable(alphaColorTable());

    for (int i=2; i <= glyph_width-3; ++i) {
        im.setPixel(i, 2, 1);
        im.setPixel(i, glyph_width-3, 1);
        im.setPixel(2, i, 1);
        im.setPixel(glyph_width-3, i, 1);
    }
    return im;
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
    addGlyphsToPath(&glyph, &p, 1, path, QFlag(0));
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

void QFontEngine::doKerning(QGlyphLayout *glyphs, QTextEngine::ShaperFlags flags)
{
    int numPairs = kerning_pairs.size();
    if(!numPairs)
        return;

    const KernPair *pairs = kerning_pairs.constData();

    if(flags & QTextEngine::DesignMetrics) {
        for(int i = 0; i < glyphs->numGlyphs - 1; ++i)
            glyphs->advances_x[i] += kerning(glyphs->glyphs[i], glyphs->glyphs[i+1] , pairs, numPairs);
    } else {
        for(int i = 0; i < glyphs->numGlyphs - 1; ++i)
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
void QFontEngine::doKerning(QGlyphLayout *, QTextEngine::ShaperFlags)
{
}
#endif

int QFontEngine::glyphCount() const
{
    QByteArray maxpTable = getSfntTable(MAKE_TAG('m', 'a', 'x', 'p'));
    if (maxpTable.size() < 6)
        return 0;
    return qFromBigEndian<quint16>(reinterpret_cast<const uchar *>(maxpTable.constData() + 4));
}

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
        glyphs->advances_y[i] = 0;
    }

    *nglyphs = len;
    glyphs->numGlyphs = len;
    return true;
}

void QFontEngineBox::recalcAdvances(QGlyphLayout *glyphs, QTextEngine::ShaperFlags) const
{
    for (int i = 0; i < glyphs->numGlyphs; i++) {
        glyphs->advances_x[i] = _size;
        glyphs->advances_y[i] = 0;
    }
}

void QFontEngineBox::addOutlineToPath(qreal x, qreal y, const QGlyphLayout &glyphs, QPainterPath *path, QTextItem::RenderFlags flags)
{
    if (!glyphs.numGlyphs)
        return;

    QVarLengthArray<QFixedPoint> positions;
    QVarLengthArray<glyph_t> positioned_glyphs;
    const QTransform matrix = QTransform::fromTranslate(x, y - _size);
    getGlyphPositions(glyphs, matrix, flags, positioned_glyphs, positions);

    QSize s(_size - 3, _size - 3);
    for (int k = 0; k < positions.size(); k++)
        path->addRect(QRectF(positions[k].toPointF(), s));
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
    return glyph_metrics_t(0, -_size, _size, _size, _size, 0);
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

#ifdef Q_WS_X11
int QFontEngineBox::cmap() const
{
    return -1;
}
#endif

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

QImage QFontEngineBox::alphaMapForGlyph(glyph_t)
{
    QImage image(_size, _size, QImage::Format_Mono);
    image.fill(0);
    image.setColorTable(alphaColorTable());

    for (int i=2; i <= _size-3; ++i) {
        image.setPixel(i, 2, 1);
        image.setPixel(i, _size-3, 1);
        image.setPixel(2, i, 1);
        image.setPixel(_size-3, i, 1);
    }
    return image;
}

QT_END_NAMESPACE
