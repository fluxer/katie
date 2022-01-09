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
#include "qtextformat.h"
#include "qtextformat_p.h"
#include "qtextengine_p.h"
#include "qabstracttextdocumentlayout.h"
#include "qtextlayout.h"
#include "qvarlengtharray.h"
#include "qfont.h"
#include "qfontengine_p.h"
#include "qunicodetables_p.h"
#include "qtextdocument_p.h"
#include "qapplication.h"
#include "qcorecommon_p.h"
#include "qx11info_x11.h"

#if defined(Q_WS_X11)
#   include "qfontengine_ft_p.h"
#endif

QT_BEGIN_NAMESPACE

namespace {
// Helper class used in QTextEngine::itemize
// keep it out here to allow us to keep supporting various compilers.
class Itemizer {
public:
    Itemizer(const QString &string, const QScriptAnalysis *analysis, QScriptItemArray &items)
        : m_string(string),
        m_analysis(analysis),
        m_items(items)
    {
    }

    /// generate the script items
    void generate(int start, int length)
    {
        if (!length)
            return;
        const int end = start + length;
        for (int i = start + 1; i < end; ++i) {
            // According to the unicode spec we should be treating characters in the Common script
            // (punctuation, spaces, etc) as being the same script as the surrounding text for the
            // purpose of splitting up text. This is important because, for example, a fullstop
            // (0x2E) can be used to indicate an abbreviation and so must be treated as part of a
            // word.  Thus it must be passed along with the word in languages that have to calculate
            // word breaks.  For example the thai word "ครม." has no word breaks but the word "ครม"
            // does.
            // Unfortuntely because we split up the strings for both wordwrapping and for setting
            // the font and because Japanese and Chinese are also aliases of the script "Common",
            // doing this would break too many things.  So instead we only pass the full stop
            // along, and nothing else.
            if (m_analysis[i].flags == m_analysis[start].flags
                && (m_analysis[i].script == m_analysis[start].script || m_string[i] == QLatin1Char('.'))
                && m_analysis[i].flags < QScriptAnalysis::SpaceTabOrObject
                && i - start < MaxItemLength)
                continue;
            m_items.append(QScriptItem(start, m_analysis[start]));
            start = i;
        }
        m_items.append(QScriptItem(start, m_analysis[start]));
    }

private:
    enum { MaxItemLength = 4096 };

    const QString &m_string;
    const QScriptAnalysis * const m_analysis;
    QScriptItemArray &m_items;
};
}

// ask the font engine to find out which glyphs (as an index in the specific font) to use for the text in one item.
static bool stringToGlyphs(HB_ShaperItem *item, QGlyphLayout *glyphs, QFontEngine *fontEngine)
{
    int nGlyphs = item->num_glyphs;

    QTextEngine::ShaperFlags shaperFlags(QTextEngine::GlyphIndicesOnly);

    bool result = fontEngine->stringToCMap(reinterpret_cast<const QChar *>(item->string + item->item.pos), item->item.length, glyphs, &nGlyphs, shaperFlags);
    item->num_glyphs = nGlyphs;
    glyphs->numGlyphs = nGlyphs;
    return result;
}

// shape all the items that intersect with the line, taking tab widths into account to find out what text actually fits in the line.
void QTextEngine::shapeLine(const QScriptLine &line)
{
    QFixed x;
    bool first = true;
    const int end = findItem(line.from + line.length - 1);
    int item = findItem(line.from);
    if (item == -1)
        return;
    for (item = findItem(line.from); item <= end; ++item) {
        QScriptItem &si = layoutData->items[item];
        if (si.analysis.flags == QScriptAnalysis::Tab) {
            ensureSpace(1);
            si.width = calculateTabWidth(item, x);
        } else {
            shape(item);
        }
        if (first && si.position != line.from) { // that means our x position has to be offset
            QGlyphLayout glyphs = shapedGlyphs(&si);
            Q_ASSERT(line.from > si.position);
            for (int i = line.from - si.position - 1; i >= 0; i--) {
                x -= glyphs.effectiveAdvance(i);
            }
        }
        first = false;

        x += si.width;
    }
}

void QTextEngine::shapeText(int item) const
{
    Q_ASSERT(item < layoutData->items.size());
    QScriptItem &si = layoutData->items[item];

    if (si.num_glyphs)
        return;

    shapeTextWithHarfbuzz(item);

    si.width = 0;

    if (!si.num_glyphs)
        return;
    QGlyphLayout glyphs = shapedGlyphs(&si);

    QFont font = this->font(si);
    bool letterSpacingIsAbsolute = font.d->letterSpacingIsAbsolute;
    QFixed letterSpacing = font.d->letterSpacing;
    QFixed wordSpacing = font.d->wordSpacing;

    if (letterSpacingIsAbsolute && letterSpacing.value())
        letterSpacing *= font.d->dpi / QX11Info::appDpiY();

    if (letterSpacing != 0) {
        for (int i = 1; i < si.num_glyphs; ++i) {
            if (glyphs.attributes[i].clusterStart) {
                if (letterSpacingIsAbsolute)
                    glyphs.advances_x[i-1] += letterSpacing;
                else {
                    QFixed &advance = glyphs.advances_x[i-1];
                    advance += (letterSpacing - 100) * advance / 100;
                }
            }
        }
        if (letterSpacingIsAbsolute)
            glyphs.advances_x[si.num_glyphs-1] += letterSpacing;
        else {
            QFixed &advance = glyphs.advances_x[si.num_glyphs-1];
            advance += (letterSpacing - 100) * advance / 100;
        }
    }
    if (wordSpacing != 0) {
        for (int i = 0; i < si.num_glyphs; ++i) {
            if (glyphs.attributes[i].justification == HB_Space) {
                // word spacing only gets added once to a consecutive run of spaces (see CSS spec)
                if (i + 1 == si.num_glyphs
                    || glyphs.attributes[i+1].justification != HB_Space)
                    glyphs.advances_x[i] += wordSpacing;
            }
        }
    }

    for (int i = 0; i < si.num_glyphs; ++i)
        si.width += glyphs.advances_x[i] * !glyphs.attributes[i].dontPrint;
}

/// take the item from layoutData->items and
void QTextEngine::shapeTextWithHarfbuzz(int item) const
{
    Q_ASSERT(sizeof(HB_Fixed) == sizeof(QFixed));
    Q_ASSERT(sizeof(HB_FixedPoint) == sizeof(QFixedPoint));

    QScriptItem &si = layoutData->items[item];

    si.glyph_data_offset = layoutData->used;

    QFontEngine *font = fontEngine(si, &si.ascent, &si.descent, &si.leading);

    bool kerningEnabled = this->font(si).d->kerning;

    HB_ShaperItem shaper_item;
    memset(&shaper_item, 0, sizeof(shaper_item));
    shaper_item.string = reinterpret_cast<const HB_UChar16 *>(layoutData->string.constData());
    shaper_item.stringLength = layoutData->string.length();
    shaper_item.item.script = (HB_Script)si.analysis.script;
    shaper_item.item.pos = si.position;
    shaper_item.item.length = length(item);

    shaper_item.shaperFlags = 0;
    if (!kerningEnabled)
        shaper_item.shaperFlags |= HB_ShaperFlag_NoKerning;
    if (option.useDesignMetrics())
        shaper_item.shaperFlags |= HB_ShaperFlag_UseDesignMetrics;

    shaper_item.num_glyphs = qMax(layoutData->glyphLayout.numGlyphs - layoutData->used, int(shaper_item.item.length));
    if (! ensureSpace(shaper_item.num_glyphs)) {
        return;
    }
    QGlyphLayout initialGlyphs = availableGlyphs(&si).mid(0, shaper_item.num_glyphs);

    if (!stringToGlyphs(&shaper_item, &initialGlyphs, font)) {
        if (! ensureSpace(shaper_item.num_glyphs)) {
            return;
        }
        initialGlyphs = availableGlyphs(&si).mid(0, shaper_item.num_glyphs);

        if (!stringToGlyphs(&shaper_item, &initialGlyphs, font)) {
            return;
        }
    }

    shaper_item.initialGlyphCount = shaper_item.num_glyphs;
    if (shaper_item.num_glyphs < shaper_item.item.length)
        shaper_item.num_glyphs = shaper_item.item.length;

    si.ascent = qMax(font->ascent(), si.ascent);
    si.descent = qMax(font->descent(), si.descent);
    si.leading = qMax(font->leading(), si.leading);

    shaper_item.font = font;

    shaper_item.glyphIndicesPresent = true;

    do {
        if (! ensureSpace(shaper_item.num_glyphs)) {
            return;
        }

        const QGlyphLayout g = availableGlyphs(&si);

        shaper_item.glyphs = g.glyphs;
        shaper_item.attributes = g.attributes;
        shaper_item.advances = reinterpret_cast<HB_Fixed *>(g.advances_x);
        shaper_item.offsets = reinterpret_cast<HB_FixedPoint *>(g.offsets);

        if (shaper_item.glyphIndicesPresent) {
            for (uint32_t i = 0; i < shaper_item.initialGlyphCount; ++i)
                shaper_item.glyphs[i] &= 0x00ffffff;
        }

        shaper_item.log_clusters = logClusters(&si);

        // qDebug("    .. num_glyphs=%d, used=%d, item.num_glyphs=%d", num_glyphs, used, shaper_item.num_glyphs);
    } while (!qHB_BasicShape(&shaper_item)); // this does the actual shaping via harfbuzz.

    QGlyphLayout g = availableGlyphs(&si).mid(0, shaper_item.num_glyphs);

    if (kerningEnabled)
        font->doKerning(&g, option.useDesignMetrics() ? QFlag(QTextEngine::DesignMetrics) : QFlag(0));

    // qDebug("    -> item: script=%d num_glyphs=%d", shaper_item.script, shaper_item.num_glyphs);
    si.num_glyphs = shaper_item.num_glyphs;

    layoutData->used += si.num_glyphs;
}

static void init(QTextEngine *e)
{
    e->cacheGlyphs = false;
    e->forceJustification = false;
    e->visualMovement = false;

    e->layoutData = 0;

    e->minWidth = 0;
    e->maxWidth = 0;

    e->underlinePositions = 0;
    e->specialData = 0;
}

QTextEngine::QTextEngine()
{
    init(this);
}

QTextEngine::QTextEngine(const QString &str, const QFont &f)
    : text(str),
      fnt(f)
{
    init(this);
}

QTextEngine::~QTextEngine()
{
    delete layoutData;
    delete specialData;
}

const HB_CharAttributes *QTextEngine::attributes() const
{
    if (layoutData && layoutData->haveCharAttributes)
        return (HB_CharAttributes *) layoutData->memory;

    itemize();
    if (! ensureSpace(layoutData->string.length()))
        return NULL;

    qHB_GetCharAttributes(reinterpret_cast<const HB_UChar16 *>(layoutData->string.constData()),
                          layoutData->string.length(), (HB_CharAttributes *)layoutData->memory);


    layoutData->haveCharAttributes = true;
    return (HB_CharAttributes *) layoutData->memory;
}

void QTextEngine::shape(int item) const
{
    if (layoutData->items[item].analysis.flags == QScriptAnalysis::Object) {
        ensureSpace(1);
        if (block.docHandle()) {
            QTextFormat format = formats()->format(formatIndex(&layoutData->items[item]));
            docLayout()->resizeInlineObject(QTextInlineObject(item, const_cast<QTextEngine *>(this)),
                                            layoutData->items[item].position + block.position(), format);
        }
    } else if (layoutData->items[item].analysis.flags == QScriptAnalysis::Tab) {
        // set up at least the ascent/descent/leading of the script item for the tab
        fontEngine(layoutData->items[item],
                   &layoutData->items[item].ascent,
                   &layoutData->items[item].descent,
                   &layoutData->items[item].leading);
    } else {
        shapeText(item);
    }
}

void QTextEngine::invalidate()
{
    freeMemory();
    minWidth = 0;
    maxWidth = 0;
    if (specialData)
        specialData->resolvedFormatIndices.clear();
}

void QTextEngine::clearLineData()
{
    lines.clear();
}

void QTextEngine::validate() const
{
    if (layoutData)
        return;
    layoutData = new LayoutData();
    if (block.docHandle()) {
        layoutData->string = block.text();
        if (option.flags() & QTextOption::ShowLineAndParagraphSeparators)
            layoutData->string += QLatin1Char(block.next().isValid() ? 0xb6 : 0x20);
    } else {
        layoutData->string = text;
    }
    if (specialData && specialData->preeditPosition != -1)
        layoutData->string.insert(specialData->preeditPosition, specialData->preeditText);
}

void QTextEngine::itemize() const
{
    validate();
    if (layoutData->items.size())
        return;

    int length = layoutData->string.length();
    if (!length)
        return;

    QSTACKARRAY(QScriptAnalysis, scriptAnalysis, length);
    QScriptAnalysis *analysis = scriptAnalysis;

    const ushort *uc = reinterpret_cast<const ushort *>(layoutData->string.unicode());
    const ushort *e = uc + length;
    QUnicodeTables::Script lastScript = QUnicodeTables::Common;
    while (uc < e) {
        switch (*uc) {
        case QChar::ObjectReplacementCharacter:
            analysis->script = QUnicodeTables::Common;
            analysis->flags = QScriptAnalysis::Object;
            break;
        case QChar::LineSeparator:
            analysis->script = QUnicodeTables::Common;
            analysis->flags = QScriptAnalysis::LineOrParagraphSeparator;
            if (option.flags() & QTextOption::ShowLineAndParagraphSeparators)
                *const_cast<ushort*>(uc) = 0x21B5; // visual line separator
            break;
        case 9: // Tab
            analysis->script = QUnicodeTables::Common;
            analysis->flags = QScriptAnalysis::Tab;
            break;
        case 32: // Space
        case QChar::Nbsp:
            if (option.flags() & QTextOption::ShowTabsAndSpaces) {
                analysis->script = QUnicodeTables::Common;
                analysis->flags = QScriptAnalysis::Space;
                break;
            }
        // fall through
        default:
            QUnicodeTables::Script script = QUnicodeTables::script(*uc);
            analysis->script = script == QUnicodeTables::Inherited ? lastScript : script;
            analysis->flags = QScriptAnalysis::None;
            break;
        }
        lastScript = analysis->script;
        ++uc;
        ++analysis;
    }
    if (option.flags() & QTextOption::ShowLineAndParagraphSeparators) {
        (analysis-1)->flags = QScriptAnalysis::LineOrParagraphSeparator; // to exclude it from width
    }

    Itemizer itemizer(layoutData->string, scriptAnalysis, layoutData->items);

    const QTextDocumentPrivate *p = block.docHandle();
    if (p) {
        SpecialData *s = specialData;

        QTextDocumentPrivate::FragmentIterator it = p->find(block.position());
        QTextDocumentPrivate::FragmentIterator end = p->find(block.position() + block.length() - 1); // -1 to omit the block separator char
        int format = it.value()->format;

        int prevPosition = 0;
        int position = prevPosition;
        while (1) {
            const QTextFragmentData * const frag = it.value();
            if (it == end || format != frag->format) {
                if (s && position >= s->preeditPosition) {
                    position += s->preeditText.length();
                    s = 0;
                }
                Q_ASSERT(position <= length);
                itemizer.generate(prevPosition, position - prevPosition);
                if (it == end) {
                    if (position < length)
                        itemizer.generate(position, length - position);
                    break;
                }
                format = frag->format;
                prevPosition = position;
            }
            position += frag->size_array[0];
            ++it;
        }
    } else {
        itemizer.generate(0, length);
    }

    addRequiredBoundaries();
    resolveAdditionalFormats();
}

int QTextEngine::findItem(int strPos) const
{
    itemize();

    int left = 1;
    int right = layoutData->items.size()-1;
    while (left <= right) {
        int middle = ((right-left)/2)+left;
        if (strPos > layoutData->items[middle].position)
            left = middle+1;
        else if (strPos < layoutData->items[middle].position)
            right = middle-1;
        else {
            return middle;
        }
    }
    return right;
}

QFixed QTextEngine::width(int from, int len) const
{
    itemize();

    QFixed w = 0;

//     qDebug("QTextEngine::width(from = %d, len = %d), numItems=%d, strleng=%d", from,  len, items.size(), string.length());
    for (int i = 0; i < layoutData->items.size(); i++) {
        const QScriptItem *si = layoutData->items.constData() + i;
        int pos = si->position;
        int ilen = length(i);
//          qDebug("item %d: from %d len %d", i, pos, ilen);
        if (pos >= from + len)
            break;
        if (pos + ilen > from) {
            if (!si->num_glyphs)
                shape(i);

            if (si->analysis.flags == QScriptAnalysis::Object) {
                w += si->width;
                continue;
            } else if (si->analysis.flags == QScriptAnalysis::Tab) {
                w += calculateTabWidth(i, w);
                continue;
            }


            QGlyphLayout glyphs = shapedGlyphs(si);
            unsigned short *logClusters = this->logClusters(si);

//             fprintf(stderr, "  logclusters:");
//             for (int k = 0; k < ilen; k++)
//                 fprintf(stderr, " %d", logClusters[k]);
//             fprintf(stderr, "\n");
            // do the simple thing for now and give the first glyph in a cluster the full width, all other ones 0.
            int charFrom = from - pos;
            if (charFrom < 0)
                charFrom = 0;
            int glyphStart = logClusters[charFrom];
            if (charFrom > 0 && logClusters[charFrom-1] == glyphStart)
                while (charFrom < ilen && logClusters[charFrom] == glyphStart)
                    charFrom++;
            if (charFrom < ilen) {
                glyphStart = logClusters[charFrom];
                int charEnd = from + len - 1 - pos;
                if (charEnd >= ilen)
                    charEnd = ilen-1;
                int glyphEnd = logClusters[charEnd];
                while (charEnd < ilen && logClusters[charEnd] == glyphEnd)
                    charEnd++;
                glyphEnd = (charEnd == ilen) ? si->num_glyphs : logClusters[charEnd];

//                 qDebug("char: start=%d end=%d / glyph: start = %d, end = %d", charFrom, charEnd, glyphStart, glyphEnd);
                for (int i = glyphStart; i < glyphEnd; i++)
                    w += glyphs.advances_x[i] * !glyphs.attributes[i].dontPrint;
            }
        }
    }
//     qDebug("   --> w= %d ", w);
    return w;
}

glyph_metrics_t QTextEngine::boundingBox() const
{
    itemize();

    const int len = layoutData->string.length();
    glyph_metrics_t gm;

    for (int i = 0; i < layoutData->items.size(); i++) {
        const QScriptItem *si = layoutData->items.constData() + i;

        int pos = si->position;
        int ilen = length(i);
        if (pos > len)
            break;
        if (pos + ilen > 0) {
            if (!si->num_glyphs)
                shape(i);

            if (si->analysis.flags == QScriptAnalysis::Object) {
                gm.width += si->width;
                continue;
            } else if (si->analysis.flags == QScriptAnalysis::Tab) {
                gm.width += calculateTabWidth(i, gm.width);
                continue;
            }

            unsigned short *logClusters = this->logClusters(si);
            QGlyphLayout glyphs = shapedGlyphs(si);

            // do the simple thing for now and give the first glyph in a cluster the full width, all other ones 0.
            int charFrom = pos;
            if (charFrom < 0)
                charFrom = 0;
            int glyphStart = logClusters[charFrom];
            if (charFrom > 0 && logClusters[charFrom-1] == glyphStart)
                while (charFrom < ilen && logClusters[charFrom] == glyphStart)
                    charFrom++;
            if (charFrom < ilen) {
                QFontEngine *fe = fontEngine(*si);
                glyphStart = logClusters[charFrom];
                int charEnd = len - 1 - pos;
                if (charEnd >= ilen)
                    charEnd = ilen-1;
                int glyphEnd = logClusters[charEnd];
                while (charEnd < ilen && logClusters[charEnd] == glyphEnd)
                    charEnd++;
                glyphEnd = (charEnd == ilen) ? si->num_glyphs : logClusters[charEnd];
                if (glyphStart <= glyphEnd ) {
                    glyph_metrics_t m = fe->boundingBox(glyphs.mid(glyphStart, glyphEnd - glyphStart));
                    gm.x = qMin(gm.x, m.x + gm.xoff);
                    gm.y = qMin(gm.y, m.y + gm.yoff);
                    gm.width = qMax(gm.width, m.width+gm.xoff);
                    gm.height = qMax(gm.height, m.height+gm.yoff);
                    gm.xoff += m.xoff;
                    gm.yoff += m.yoff;
                }
            }
        }
    }
    return gm;
}

QFont QTextEngine::font(const QScriptItem &si) const
{
    QFont font = fnt;
    if (hasFormats()) {
        QTextCharFormat f = format(&si);
        font = f.font();

        if (block.docHandle() && block.docHandle()->layout()) {
            // Make sure we get the right dpi on printers
            QPaintDevice *pdev = block.docHandle()->layout()->paintDevice();
            if (pdev)
                font = QFont(font, pdev);
        } else {
            font = font.resolve(fnt);
        }
        QTextCharFormat::VerticalAlignment valign = f.verticalAlignment();
        if (valign == QTextCharFormat::AlignSuperScript || valign == QTextCharFormat::AlignSubScript) {
            if (font.pointSize() != -1)
                font.setPointSize((font.pointSize() * 2) / 3);
            else
                font.setPixelSize((font.pixelSize() * 2) / 3);
        }
    }

    return font;
}

QFontEngine *QTextEngine::fontEngine(const QScriptItem &si, QFixed *ascent, QFixed *descent, QFixed *leading) const
{
    QFontEngine *engine = nullptr;
    QUnicodeTables::Script script = si.analysis.script;

    QFont font = fnt;
    if (hasFormats()) {
        QTextCharFormat f = format(&si);
        font = f.font();

        if (block.docHandle() && block.docHandle()->layout()) {
            // Make sure we get the right dpi on printers
            QPaintDevice *pdev = block.docHandle()->layout()->paintDevice();
            if (pdev)
                font = QFont(font, pdev);
        } else {
            font = font.resolve(fnt);
        }
        QTextCharFormat::VerticalAlignment valign = f.verticalAlignment();
        if (valign == QTextCharFormat::AlignSuperScript || valign == QTextCharFormat::AlignSubScript) {
            if (font.pointSize() != -1)
                font.setPointSize((font.pointSize() * 2) / 3);
            else
                font.setPixelSize((font.pixelSize() * 2) / 3);
            engine = font.d->engineForScript(script);
        } else {
            engine = font.d->engineForScript(script);
        }
    } else {
        engine = font.d->engineForScript(script);
    }

    if (ascent) {
        *ascent = engine->ascent();
        *descent = engine->descent();
        *leading = engine->leading();
    }

    return engine;
}

struct QJustificationPoint {
    int type;
    QGlyphLayout glyph;
    QFontEngine *fontEngine;
};

Q_DECLARE_TYPEINFO(QJustificationPoint, Q_PRIMITIVE_TYPE);

static void set(QJustificationPoint *point, int type, const QGlyphLayout &glyph, QFontEngine *fe)
{
    point->type = type;
    point->glyph = glyph;
    point->fontEngine = fe;
}


void QTextEngine::justify(const QScriptLine &line)
{
//     qDebug("justify: line.gridfitted = %d, line.justified=%d", line.gridfitted, line.justified);
    if (line.gridfitted && line.justified)
        return;

    if (!line.gridfitted) {
        // redo layout in device metrics, then adjust
        const_cast<QScriptLine &>(line).gridfitted = true;
    }

    if ((option.alignment() & Qt::AlignHorizontal_Mask) != Qt::AlignJustify)
        return;

    itemize();

    if (!forceJustification) {
        int end = line.from + (int)line.length;
        if (end == layoutData->string.length())
            return; // no justification at end of paragraph
        if (end && layoutData->items[findItem(end-1)].analysis.flags == QScriptAnalysis::LineOrParagraphSeparator)
            return; // no justification at the end of an explicitly separated line
    }

    // justify line
    int maxJustify = 0;

    // don't include trailing white spaces when doing justification
    int line_length = line.length;
    const HB_CharAttributes *a = attributes();
    if (! a)
        return;
    a += line.from;
    while (line_length && a[line_length-1].whiteSpace)
        --line_length;
    // subtract one char more, as we can't justfy after the last character
    --line_length;

    if (!line_length)
        return;

    int firstItem = findItem(line.from);
    int nItems = findItem(line.from + line_length - 1) - firstItem + 1;

    QVarLengthArray<QJustificationPoint> justificationPoints;
    int nPoints = 0;
    // qDebug("justifying from %d len %d, firstItem=%d, nItems=%d (%s)", line.from, line_length, firstItem, nItems, layoutData->string.mid(line.from, line_length).toUtf8().constData());

    // we need to do all shaping before we go into the next loop, as we there
    // store pointers to the glyph data that could get reallocated by the shaping
    // process.
    for (int i = 0; i < nItems; ++i) {
        QScriptItem &si = layoutData->items[firstItem + i];
        if (!si.num_glyphs)
            shape(firstItem + i);
    }

    for (int i = 0; i < nItems; ++i) {
        QScriptItem &si = layoutData->items[firstItem + i];

        int start = qMax(line.from - si.position, 0);
        int end = qMin(line.from + line_length - (int)si.position, length(firstItem+i));

        unsigned short *log_clusters = logClusters(&si);

        int gs = log_clusters[start];
        int ge = (end == length(firstItem+i) ? si.num_glyphs : log_clusters[end]);

        const QGlyphLayout g = shapedGlyphs(&si);

        for (int i = gs; i < ge; ++i) {
            g.justifications[i].type = QGlyphJustification::JustifyNone;
            g.justifications[i].space_18d6 = 0;

            justificationPoints.resize(nPoints+3);
            int justification = g.attributes[i].justification;

            switch(justification) {
                case HB_NoJustification: {
                    break;
                }
                case HB_Space:
                case HB_Character: {
                    set(&justificationPoints[nPoints++], justification, g.mid(i), fontEngine(si));
                    maxJustify = qMax(maxJustify, justification);
                    break;
                }
            }
        }
    }

    QFixed need = line.width - line.textWidth;
    if (need < 0) {
        // line overflows already!
        const_cast<QScriptLine &>(line).justified = true;
        return;
    }

    // qDebug("doing justification: textWidth=%x, requested=%x, maxJustify=%d", line.textWidth.value(), line.width.value(), maxJustify);

    Q_ASSERT(need >= 0);
    if (!need)
        goto end;

    maxJustify = qMin(maxJustify, (int)HB_Space);
    for (int type = maxJustify; need != 0 && type > 0; --type) {
        int n = 0;
        for (int i = 0; i < nPoints; ++i) {
            if (justificationPoints[i].type == type)
                ++n;
        }
//          qDebug("number of points for justification type %d: %d", type, n);


        if (!n)
            continue;

        for (int i = 0; i < nPoints; ++i) {
            if (justificationPoints[i].type == type) {
                QFixed add = need/n;
//                  qDebug("adding %x to glyph %x", add.value(), justificationPoints[i].glyph->glyph);
                justificationPoints[i].glyph.justifications[0].space_18d6 = add.value();
                need -= add;
                --n;
            }
        }

        Q_ASSERT(!need);
    }
 end:
    const_cast<QScriptLine &>(line).justified = true;
}

void QScriptLine::setDefaultHeight(QTextEngine *eng)
{
    QFont f;
    QFontEngine *e;

    if (eng->block.docHandle() && eng->block.docHandle()->layout()) {
        f = eng->block.charFormat().font();
        // Make sure we get the right dpi on printers
        QPaintDevice *pdev = eng->block.docHandle()->layout()->paintDevice();
        if (pdev)
            f = QFont(f, pdev);
        e = f.d->engineForScript(QUnicodeTables::Common);
    } else {
        e = eng->fnt.d->engineForScript(QUnicodeTables::Common);
    }

    QFixed other_ascent = e->ascent();
    QFixed other_descent = e->descent();
    QFixed other_leading = e->leading();
    leading = qMax(leading + ascent, other_leading + other_ascent) - qMax(ascent, other_ascent);
    ascent = qMax(ascent, other_ascent);
    descent = qMax(descent, other_descent);
}

QTextEngine::LayoutData::LayoutData()
{
    memory = 0;
    allocated = 0;
    used = 0;
    layoutState = LayoutEmpty;
    haveCharAttributes = false;
    logClustersPtr = 0;
}

QTextEngine::LayoutData::~LayoutData()
{
    ::free(memory);
    memory = 0;
}

bool QTextEngine::LayoutData::reallocate(int totalGlyphs)
{
    Q_ASSERT(totalGlyphs >= glyphLayout.numGlyphs);

    int space_charAttributes = sizeof(HB_CharAttributes) * string.length() / QT_POINTER_SIZE + 1;
    int space_logClusters = sizeof(unsigned short) * string.length() / QT_POINTER_SIZE + 1;
    int space_glyphs = QGlyphLayout::spaceNeededForGlyphLayout(totalGlyphs) / QT_POINTER_SIZE + 2;

    int newAllocated = space_charAttributes + space_glyphs + space_logClusters;
    // These values can be negative if the length of string/glyphs causes overflow,
    // we can't layout such a long string all at once, so return false here to
    // indicate there is a failure
    if (space_charAttributes < 0 || space_logClusters < 0 || space_glyphs < 0 || newAllocated < allocated) {
        layoutState = LayoutFailed;
        return false;
    }

    void **newMem = (void **)::realloc(memory, newAllocated * QT_POINTER_SIZE);
    if (!newMem) {
        layoutState = LayoutFailed;
        return false;
    }
    memory = newMem;

    void **m = memory;
    m += space_charAttributes;
    logClustersPtr = (unsigned short *) m;
    m += space_logClusters;

    const int space_preGlyphLayout = space_charAttributes + space_logClusters;
    if (allocated < space_preGlyphLayout)
        memset(memory + allocated, 0, (space_preGlyphLayout - allocated) * QT_POINTER_SIZE);

    glyphLayout.grow(reinterpret_cast<char *>(m), totalGlyphs);

    allocated = newAllocated;
    return true;
}

// grow to the new size, copying the existing data to the new layout
void QGlyphLayout::grow(char *address, int totalGlyphs)
{
    QGlyphLayout oldLayout(address, numGlyphs);
    QGlyphLayout newLayout(address, totalGlyphs);

    if (numGlyphs) {
        // move the existing data
        memmove(newLayout.attributes, oldLayout.attributes, numGlyphs * sizeof(HB_GlyphAttributes));
        memmove(newLayout.justifications, oldLayout.justifications, numGlyphs * sizeof(QGlyphJustification));
        memmove(newLayout.advances_y, oldLayout.advances_y, numGlyphs * sizeof(QFixed));
        memmove(newLayout.advances_x, oldLayout.advances_x, numGlyphs * sizeof(QFixed));
        memmove(newLayout.glyphs, oldLayout.glyphs, numGlyphs * sizeof(HB_Glyph));
    }

    // clear the new data
    newLayout.clear(numGlyphs);

    *this = newLayout;
}

void QTextEngine::freeMemory()
{
    delete layoutData;
    layoutData = 0;
    for (int i = 0; i < lines.size(); ++i) {
        lines[i].justified = 0;
        lines[i].gridfitted = 0;
    }
}

int QTextEngine::formatIndex(const QScriptItem *si) const
{
    if (specialData && !specialData->resolvedFormatIndices.isEmpty())
        return specialData->resolvedFormatIndices.at(si - &layoutData->items[0]);
    QTextDocumentPrivate *p = block.docHandle();
    if (!p)
        return -1;
    int pos = si->position;
    if (specialData && si->position >= specialData->preeditPosition) {
        if (si->position < specialData->preeditPosition + specialData->preeditText.length())
            pos = qMax(specialData->preeditPosition - 1, 0);
        else
            pos -= specialData->preeditText.length();
    }
    QTextDocumentPrivate::FragmentIterator it = p->find(block.position() + pos);
    return it.value()->format;
}


QTextCharFormat QTextEngine::format(const QScriptItem *si) const
{
    QTextCharFormat format;
    const QTextFormatCollection *formats = 0;
    if (block.docHandle()) {
        formats = this->formats();
        format = formats->charFormat(formatIndex(si));
    }
    if (specialData && specialData->resolvedFormatIndices.isEmpty()) {
        int end = si->position + length(si);
        for (int i = 0; i < specialData->addFormats.size(); ++i) {
            const QTextLayout::FormatRange &r = specialData->addFormats.at(i);
            if (r.start <= si->position && r.start + r.length >= end) {
                if (!specialData->addFormatIndices.isEmpty())
                    format.merge(formats->format(specialData->addFormatIndices.at(i)));
                else
                    format.merge(r.format);
            }
        }
    }
    return format;
}

void QTextEngine::addRequiredBoundaries() const
{
    if (specialData) {
        for (int i = 0; i < specialData->addFormats.size(); ++i) {
            const QTextLayout::FormatRange &r = specialData->addFormats.at(i);
            setBoundary(r.start);
            setBoundary(r.start + r.length);
            //qDebug("adding boundaries %d %d", r.start, r.start+r.length);
        }
    }
}

bool QTextEngine::atWordSeparator(int position) const
{
    const QChar c = layoutData->string.at(position);
    switch (c.toLatin1()) {
    case '.':
    case ',':
    case '?':
    case '!':
    case '@':
    case '#':
    case '$':
    case ':':
    case ';':
    case '-':
    case '<':
    case '>':
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case '=':
    case '/':
    case '+':
    case '%':
    case '&':
    case '^':
    case '*':
    case '\'':
    case '"':
    case '`':
    case '~':
    case '|':
        return true;
    default:
        return false;
    }
}

bool QTextEngine::atSpace(int position) const
{
    const QChar c = layoutData->string.at(position);

    return c == QLatin1Char(' ')
        || c == QChar::Nbsp
        || c == QChar::LineSeparator
        || c == QLatin1Char('\t')
        ;
}


void QTextEngine::indexAdditionalFormats()
{
    if (!block.docHandle())
        return;

    specialData->addFormatIndices.resize(specialData->addFormats.count());
    QTextFormatCollection * const formats = this->formats();

    for (int i = 0; i < specialData->addFormats.count(); ++i) {
        specialData->addFormatIndices[i] = formats->indexForFormat(specialData->addFormats.at(i).format);
        specialData->addFormats[i].format = QTextCharFormat();
    }
}

/* These two helper functions are used to determine whether we need to insert a ZWJ character
   between the text that gets truncated and the ellipsis. This is important to get
   correctly shaped results for arabic text.
*/
static inline bool nextCharJoins(const QString &string, int pos)
{
    while (pos < string.length() && string.at(pos).category() == QChar::Mark_NonSpacing)
        ++pos;
    if (pos == string.length())
        return false;
    return string.at(pos).joining() != QChar::OtherJoining;
}

static inline bool prevCharJoins(const QString &string, int pos)
{
    while (pos > 0 && string.at(pos - 1).category() == QChar::Mark_NonSpacing)
        --pos;
    if (pos == 0)
        return false;
    QChar::Joining joining = string.at(pos - 1).joining();
    return (joining == QChar::Dual);
}

QString QTextEngine::elidedText(Qt::TextElideMode mode, const QFixed &width, int flags) const
{
//    qDebug() << "elidedText; available width" << width.toReal() << "text width:" << this->width(0, layoutData->string.length()).toReal();

    if (flags & Qt::TextShowMnemonic) {
        itemize();
        HB_CharAttributes *attributes = const_cast<HB_CharAttributes *>(this->attributes());
        if (!attributes)
            return QString();
        for (int i = 0; i < layoutData->items.size(); ++i) {
            QScriptItem &si = layoutData->items[i];
            if (!si.num_glyphs)
                shape(i);

            unsigned short *logClusters = this->logClusters(&si);
            QGlyphLayout glyphs = shapedGlyphs(&si);

            const int end = si.position + length(&si);
            for (int i = si.position; i < end - 1; ++i) {
                if (layoutData->string.at(i) == QLatin1Char('&')) {
                    const int gp = logClusters[i - si.position];
                    glyphs.attributes[gp].dontPrint = true;
                    attributes[i + 1].charStop = false;
                    attributes[i + 1].whiteSpace = false;
                    attributes[i + 1].lineBreakType = HB_NoBreak;
                    if (layoutData->string.at(i + 1) == QLatin1Char('&'))
                        ++i;
                }
            }
        }
    }

    validate();

    if (mode == Qt::ElideNone
        || this->width(0, layoutData->string.length()) <= width
        || layoutData->string.length() <= 1)
        return layoutData->string;

    QFixed ellipsisWidth;
    QString ellipsisText;
    {
        QChar ellipsisChar(0x2026);

        QFontEngine *fe = fnt.d->engineForScript(QUnicodeTables::Common);

        QGlyphLayoutArray<1> ellipsisGlyph;
        {
            if (fe->canRender(&ellipsisChar, 1)) {
                int nGlyphs = 1;
                fe->stringToCMap(&ellipsisChar, 1, &ellipsisGlyph, &nGlyphs, 0);
            }
        }

        if (ellipsisGlyph.glyphs[0]) {
            ellipsisWidth = ellipsisGlyph.advances_x[0];
            ellipsisText = ellipsisChar;
        } else {
            QString dotDotDot(QLatin1String("..."));

            QGlyphLayoutArray<3> glyphs;
            int nGlyphs = 3;
            if (!fe->stringToCMap(dotDotDot.constData(), 3, &glyphs, &nGlyphs, 0))
                // should never happen...
                return layoutData->string;
            for (int i = 0; i < nGlyphs; ++i)
                ellipsisWidth += glyphs.advances_x[i];
            ellipsisText = dotDotDot;
        }
    }

    const QFixed availableWidth = width - ellipsisWidth;
    if (availableWidth < 0)
        return QString();

    const HB_CharAttributes *attributes = this->attributes();
    if (!attributes)
        return QString();

    if (mode == Qt::ElideRight) {
        QFixed currentWidth;
        int pos;
        int nextBreak = 0;

        do {
            pos = nextBreak;

            ++nextBreak;
            while (nextBreak < layoutData->string.length() && !attributes[nextBreak].charStop)
                ++nextBreak;

            currentWidth += this->width(pos, nextBreak - pos);
        } while (nextBreak < layoutData->string.length()
                 && currentWidth < availableWidth);

        if (nextCharJoins(layoutData->string, pos))
            ellipsisText.prepend(QChar(0x200d) /* ZWJ */);

        return layoutData->string.left(pos) + ellipsisText;
    } else if (mode == Qt::ElideLeft) {
        QFixed currentWidth;
        int pos;
        int nextBreak = layoutData->string.length();

        do {
            pos = nextBreak;

            --nextBreak;
            while (nextBreak > 0 && !attributes[nextBreak].charStop)
                --nextBreak;

            currentWidth += this->width(nextBreak, pos - nextBreak);
        } while (nextBreak > 0
                 && currentWidth < availableWidth);

        if (prevCharJoins(layoutData->string, pos))
            ellipsisText.append(QChar(0x200d) /* ZWJ */);

        return ellipsisText + layoutData->string.mid(pos);
    } else if (mode == Qt::ElideMiddle) {
        QFixed leftWidth;
        QFixed rightWidth;

        int leftPos = 0;
        int nextLeftBreak = 0;

        int rightPos = layoutData->string.length();
        int nextRightBreak = layoutData->string.length();

        do {
            leftPos = nextLeftBreak;
            rightPos = nextRightBreak;

            ++nextLeftBreak;
            while (nextLeftBreak < layoutData->string.length() && !attributes[nextLeftBreak].charStop)
                ++nextLeftBreak;

            --nextRightBreak;
            while (nextRightBreak > 0 && !attributes[nextRightBreak].charStop)
                --nextRightBreak;

            leftWidth += this->width(leftPos, nextLeftBreak - leftPos);
            rightWidth += this->width(nextRightBreak, rightPos - nextRightBreak);
        } while (nextLeftBreak < layoutData->string.length()
                 && nextRightBreak > 0
                 && leftWidth + rightWidth < availableWidth);

        if (nextCharJoins(layoutData->string, leftPos))
            ellipsisText.prepend(QChar(0x200d) /* ZWJ */);
        if (prevCharJoins(layoutData->string, rightPos))
            ellipsisText.append(QChar(0x200d) /* ZWJ */);

        return layoutData->string.left(leftPos) + ellipsisText + layoutData->string.mid(rightPos);
    }

    return layoutData->string;
}

void QTextEngine::setBoundary(int strPos) const
{
    if (strPos <= 0 || strPos >= layoutData->string.length())
        return;
    const int item = findItem(strPos);
    if (item < 0)
        return;

    QScriptItem newItem = layoutData->items.at(item);
    if (newItem.position != strPos) {
        newItem.position = strPos;
        layoutData->items.insert(item + 1, newItem);
    }
}

QFixed QTextEngine::calculateTabWidth(int item, QFixed x) const
{
    QFixed dpiScale = 1;
    if (block.docHandle() && block.docHandle()->layout()) {
        QPaintDevice *pdev = block.docHandle()->layout()->paintDevice();
        if (pdev)
            dpiScale = QFixed::fromReal(pdev->logicalDpiY() / qreal(QX11Info::appDpiY()));
    } else {
        dpiScale = QFixed::fromReal(fnt.d->dpi / qreal(QX11Info::appDpiY()));
    }

    QFixed tab = QFixed::fromReal(option.tabStop());
    if (tab <= 0)
        tab = 80; // default
    tab *= dpiScale;
    QFixed nextTabPos = ((x / tab).truncate() + 1) * tab;
    QFixed tabWidth = nextTabPos - x;

    return tabWidth;
}

void QTextEngine::resolveAdditionalFormats() const
{
    if (!specialData || specialData->addFormats.isEmpty()
        || !block.docHandle()
        || !specialData->resolvedFormatIndices.isEmpty())
        return;

    QTextFormatCollection *collection = this->formats();

    specialData->resolvedFormatIndices.clear();
    QVector<int> indices(layoutData->items.count());
    for (int i = 0; i < layoutData->items.count(); ++i) {
        QTextCharFormat f = format(&layoutData->items.at(i));
        indices[i] = collection->indexForFormat(f);
    }
    specialData->resolvedFormatIndices = indices;
}

QFixed QTextEngine::alignLine(const QScriptLine &line)
{
    QFixed x = 0;
    justify(line);
    // if width is QFIXED_MAX that means we used setNumColumns() and that implicitly makes this line left aligned.
    if (!line.justified && line.width != QFIXED_MAX) {
        int align = option.alignment();
        if (align & Qt::AlignRight)
            x = line.width - (line.textAdvance);
        else if (align & Qt::AlignHCenter)
            x = (line.width - line.textAdvance)/2;
    }
    return x;
}

QFixed QTextEngine::offsetInLigature(const QScriptItem *si, int pos, int max, int glyph_pos)
{
    unsigned short *logClusters = this->logClusters(si);
    const QGlyphLayout &glyphs = shapedGlyphs(si);

    int offsetInCluster = 0;
    for (int i = pos - 1; i >= 0; i--) {
        if (logClusters[i] == glyph_pos)
            offsetInCluster++;
        else
            break;
    }

    // in the case that the offset is inside a (multi-character) glyph,
    // interpolate the position.
    if (offsetInCluster > 0) {
        int clusterLength = 0;
        for (int i = pos - offsetInCluster; i < max; i++) {
            if (logClusters[i] == glyph_pos)
                clusterLength++;
            else
                break;
        }
        if (clusterLength)
            return glyphs.advances_x[glyph_pos] * offsetInCluster / clusterLength;
    }

    return 0;
}

// Scan in logClusters[from..to-1] for glyph_pos
int QTextEngine::getClusterLength(unsigned short *logClusters,
                                  const HB_CharAttributes *attributes,
                                  int from, int to, int glyph_pos, int *start)
{
    int clusterLength = 0;
    for (int i = from; i < to; i++) {
        if (logClusters[i] == glyph_pos && attributes[i].charStop) {
            if (*start < 0)
                *start = i;
            clusterLength++;
        }
        else if (clusterLength)
            break;
    }
    return clusterLength;
}

int QTextEngine::positionInLigature(const QScriptItem *si, int end,
                                    QFixed x, QFixed edge, int glyph_pos,
                                    bool cursorOnCharacter)
{
    unsigned short *logClusters = this->logClusters(si);
    int clusterStart = -1;
    int clusterLength = 0;

    if (si->analysis.script != QUnicodeTables::Common &&
        si->analysis.script != QUnicodeTables::Greek) {
        if (glyph_pos == -1)
            return si->position + end;
        else {
            int i;
            for (i = 0; i < end; i++)
                if (logClusters[i] == glyph_pos)
                    break;
            return si->position + i;
        }
    }

    if (glyph_pos == -1 && end > 0)
        glyph_pos = logClusters[end - 1];
    else {
        if (x <= edge)
            glyph_pos--;
    }

    const HB_CharAttributes *attrs = attributes();
    logClusters = this->logClusters(si);
    clusterLength = getClusterLength(logClusters, attrs, 0, end, glyph_pos, &clusterStart);

    if (clusterLength) {
        const QGlyphLayout &glyphs = shapedGlyphs(si);
        QFixed glyphWidth = glyphs.effectiveAdvance(glyph_pos);
        // the approximate width of each individual element of the ligature
        QFixed perItemWidth = glyphWidth / clusterLength;
        if (perItemWidth <= 0)
            return si->position + clusterStart;
        QFixed left = x > edge ? edge : edge - glyphWidth;
        int n = ((x - left) / perItemWidth).floor().toInt();
        QFixed dist = x - left - n * perItemWidth;
        int closestItem = dist > (perItemWidth / 2) ? n + 1 : n;
        if (cursorOnCharacter && closestItem > 0)
            closestItem--;
        int pos = si->position + clusterStart + closestItem;
        // Jump to the next charStop
        while (pos < end && !attrs[pos].charStop)
            pos++;
        return pos;
    }
    return si->position + end;
}

int QTextEngine::previousLogicalPosition(int oldPos) const
{
    const HB_CharAttributes *attrs = attributes();
    if (!attrs || oldPos < 0)
        return oldPos;

    if (oldPos <= 0)
        return 0;
    oldPos--;
    while (oldPos && !attrs[oldPos].charStop)
        oldPos--;
    return oldPos;
}

int QTextEngine::nextLogicalPosition(int oldPos) const
{
    const HB_CharAttributes *attrs = attributes();
    int len = block.isValid() ? block.length() - 1
                              : layoutData->string.length();
    Q_ASSERT(len <= layoutData->string.length());
    if (!attrs || oldPos < 0 || oldPos >= len)
        return oldPos;

    oldPos++;
    while (oldPos < len && !attrs[oldPos].charStop)
        oldPos++;
    return oldPos;
}

int QTextEngine::lineNumberForTextPosition(int pos)
{
    if (!layoutData)
        itemize();
    if (pos == layoutData->string.length() && lines.size())
        return lines.size() - 1;
    for (int i = 0; i < lines.size(); ++i) {
        const QScriptLine& line = lines[i];
        if (line.from + line.length + line.trailingSpaces > pos)
            return i;
    }
    return -1;
}

int QTextEngine::positionAfterVisualMovement(int pos, QTextCursor::MoveOperation op)
{
    if (!layoutData)
        itemize();

    bool moveRight = (op == QTextCursor::Right);

    return (moveRight ? nextLogicalPosition(pos) : previousLogicalPosition(pos));
}

QTextItemInt::QTextItemInt(const QScriptItem &si, QFont *font, const QTextCharFormat &format)
    : justified(false), underlineStyle(QTextCharFormat::NoUnderline), charFormat(format),
      num_chars(0), chars(0), logClusters(0), f(0), fontEngine(0)
{
    f = font;
    fontEngine = f->d->engineForScript(si.analysis.script);
    Q_ASSERT(fontEngine);

    initWithScriptItem(si);
}

QTextItemInt::QTextItemInt(const QGlyphLayout &g, QFont *font, const QChar *chars_, int numChars, QFontEngine *fe, const QTextCharFormat &format)
    : flags(0), justified(false), underlineStyle(QTextCharFormat::NoUnderline), charFormat(format),
      num_chars(numChars), chars(chars_), logClusters(0), f(font),  glyphs(g), fontEngine(fe)
{
}

// Fix up flags and underlineStyle with given info
void QTextItemInt::initWithScriptItem(const QScriptItem &si)
{
    // explicitly initialize flags so that initFontAttributes can be called
    // multiple times on the same TextItem
    flags = 0;
    ascent = si.ascent;
    descent = si.descent;

    if (charFormat.hasProperty(QTextFormat::TextUnderlineStyle)) {
        underlineStyle = charFormat.underlineStyle();
    } else if (charFormat.boolProperty(QTextFormat::FontUnderline)
               || f->d->underline) {
        underlineStyle = QTextCharFormat::SingleUnderline;
    }

    // compat
    if (underlineStyle == QTextCharFormat::SingleUnderline)
        flags |= QTextItem::Underline;

    if (f->d->overline || charFormat.fontOverline())
        flags |= QTextItem::Overline;
    if (f->d->strikeOut || charFormat.fontStrikeOut())
        flags |= QTextItem::StrikeOut;
}

QTextLineItemIterator::QTextLineItemIterator(QTextEngine *_eng, int _lineNum, const QPointF &pos,
                                             const QTextLayout::FormatRange *_selection)
    : eng(_eng),
      line(eng->lines[_lineNum]),
      si(0),
      lineNum(_lineNum),
      lineEnd(line.from + line.length),
      firstItem(eng->findItem(line.from)),
      lastItem(eng->findItem(lineEnd - 1)),
      nItems((firstItem >= 0 && lastItem >= firstItem)? (lastItem-firstItem+1) : 0),
      logicalItem(-1),
      item(-1),
      selection(_selection)
{
    pos_x = x = QFixed::fromReal(pos.x());

    x += line.x;

    x += eng->alignLine(line);

    eng->shapeLine(line);
}

QScriptItem &QTextLineItemIterator::next()
{
    x += itemWidth;

    ++logicalItem;
    item = (logicalItem + firstItem);
    itemLength = eng->length(item);
    si = &eng->layoutData->items[item];
    if (!si->num_glyphs)
        eng->shape(item);

    if (si->analysis.flags >= QScriptAnalysis::TabOrObject) {
        itemWidth = si->width;
        return *si;
    }

    unsigned short *logClusters = eng->logClusters(si);
    QGlyphLayout glyphs = eng->shapedGlyphs(si);

    itemStart = qMax(line.from, si->position);
    glyphsStart = logClusters[itemStart - si->position];
    if (lineEnd < si->position + itemLength) {
        itemEnd = lineEnd;
        glyphsEnd = logClusters[itemEnd-si->position];
    } else {
        itemEnd = si->position + itemLength;
        glyphsEnd = si->num_glyphs;
    }
    // show soft-hyphen at line-break
    if (si->position + itemLength >= lineEnd
        && eng->layoutData->string.at(lineEnd - 1) == 0x00ad)
        glyphs.attributes[glyphsEnd - 1].dontPrint = false;

    itemWidth = 0;
    for (int g = glyphsStart; g < glyphsEnd; ++g)
        itemWidth += glyphs.effectiveAdvance(g);

    return *si;
}

bool QTextLineItemIterator::getSelectionBounds(QFixed *selectionX, QFixed *selectionWidth) const
{
    *selectionX = *selectionWidth = 0;

    if (!selection)
        return false;

    if (si->analysis.flags >= QScriptAnalysis::TabOrObject) {
        if (si->position >= selection->start + selection->length
            || si->position + itemLength <= selection->start)
            return false;

        *selectionX = x;
        *selectionWidth = itemWidth;
    } else {
        unsigned short *logClusters = eng->logClusters(si);
        QGlyphLayout glyphs = eng->shapedGlyphs(si);

        int from = qMax(itemStart, selection->start) - si->position;
        int to = qMin(itemEnd, selection->start + selection->length) - si->position;
        if (from >= to)
            return false;

        int start_glyph = logClusters[from];
        int end_glyph = (to == eng->length(item)) ? si->num_glyphs : logClusters[to];
        QFixed soff;
        QFixed swidth;
        for (int g = glyphsStart; g < start_glyph; ++g)
            soff += glyphs.effectiveAdvance(g);
        for (int g = start_glyph; g < end_glyph; ++g)
            swidth += glyphs.effectiveAdvance(g);

        // If the starting character is in the middle of a ligature,
        // selection should only contain the right part of that ligature
        // glyph, so we need to get the width of the left part here and
        // add it to *selectionX
        QFixed leftOffsetInLigature = eng->offsetInLigature(si, from, to, start_glyph);
        *selectionX = x + soff + leftOffsetInLigature;
        *selectionWidth = swidth - leftOffsetInLigature;
        // If the ending character is also part of a ligature, swidth does
        // not contain that part yet, we also need to find out the width of
        // that left part
        *selectionWidth += eng->offsetInLigature(si, to, eng->length(item), end_glyph);
    }
    return true;
}

QT_END_NAMESPACE




