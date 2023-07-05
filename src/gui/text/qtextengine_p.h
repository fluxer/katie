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

#ifndef QTEXTENGINE_P_H
#define QTEXTENGINE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qstdcontainers_p.h"
#include "qfont_p.h"
#include "qpaintengine.h"
#include "qtextdocument_p.h"
#include "qharfbuzz_p.h"

QT_BEGIN_NAMESPACE

class QFontPrivate;
class QFontEngine;

class QString;
class QPainter;

class QAbstractTextDocumentLayout;


// this uses the same coordinate system as Qt, but a different one to freetype.
// the characters bounding rect is given by QRect(x,y,width,height), its
// advance by xoff
struct glyph_metrics_t
{
    inline glyph_metrics_t()
        : x(100000),  y(100000) {}
    inline glyph_metrics_t(QFixed _x, QFixed _y, QFixed _width, QFixed _height, QFixed _xoff)
        : x(_x),
          y(_y),
          width(_width),
          height(_height),
          xoff(_xoff)
        {}
    QFixed x;
    QFixed y;
    QFixed width;
    QFixed height;
    QFixed xoff;

    inline bool isValid() const {return x != 100000 && y != 100000;}
};


struct Q_AUTOTEST_EXPORT QScriptAnalysis
{
    enum Flags {
        None = 0,
        LineOrParagraphSeparator = 1,
        Space = 2,
        SpaceTabOrObject = Space,
        Tab = 3,
        TabOrObject = Tab,
        Object = 4
    };

    inline QScriptAnalysis()
        : script(QUnicodeTables::Common),  flags(None) {}

    QUnicodeTables::Script script;
    Flags flags;
    inline bool operator == (const QScriptAnalysis &other) const {
        return script == other.script && flags == other.flags;
    }
};

struct QGlyphJustification
{
    enum JustificationType {
        JustifyNone,
        JustifySpace
    };

    inline QGlyphJustification()
        : type(JustifyNone), space_18d6(0)
    {}

    JustificationType type;
    uint space_18d6;
};

#define QSPACEFORGLYPHS(__glyphcount) \
    (__glyphcount * (sizeof(HB_Glyph) + sizeof(HB_GlyphAttributes) \
        + sizeof(QFixed) + sizeof(QGlyphJustification)))

struct QGlyphLayout
{
    // init to 0 not needed, done when shaping
    HB_Glyph *glyphs; // 4 bytes per element
    QFixed *advances_x; // 4 bytes per element
    QGlyphJustification *justifications; // 4 bytes per element
    HB_GlyphAttributes *attributes; // 2 bytes per element

    int numGlyphs;

    inline QGlyphLayout() : numGlyphs(0) {}

    inline explicit QGlyphLayout(char *address, int totalGlyphs)
    {
        int offset = totalGlyphs * sizeof(HB_Glyph);
        glyphs = reinterpret_cast<HB_Glyph *>(address);
        advances_x = reinterpret_cast<QFixed *>(address + offset);
        offset += totalGlyphs * sizeof(QFixed);
        justifications = reinterpret_cast<QGlyphJustification *>(address + offset);
        offset += totalGlyphs * sizeof(QGlyphJustification);
        attributes = reinterpret_cast<HB_GlyphAttributes *>(address + offset);
        numGlyphs = totalGlyphs;
    }

    inline QGlyphLayout mid(int position, int n = -1) const {
        QGlyphLayout copy = *this;
        copy.glyphs += position;
        copy.advances_x += position;
        copy.justifications += position;
        copy.attributes += position;
        if (n == -1)
            copy.numGlyphs -= position;
        else
            copy.numGlyphs = n;
        return copy;
    }

    inline QFixed effectiveAdvance(int item) const
    { return (advances_x[item] + QFixed::fromFixed(justifications[item].space_18d6)) * !attributes[item].dontPrint; }

    inline void clear(int first = 0, int last = -1) {
        if (last == -1)
            last = numGlyphs;
        if (first == 0 && last == numGlyphs) {
            memset(glyphs, 0, QSPACEFORGLYPHS(numGlyphs));
        } else {
            const int num = last - first;
            memset(glyphs + first, 0, num * sizeof(HB_Glyph));
            memset(advances_x + first, 0, num * sizeof(QFixed));
            memset(justifications + first, 0, num * sizeof(QGlyphJustification));
            memset(attributes + first, 0, num * sizeof(HB_GlyphAttributes));
        }
    }

    inline char *data() {
        return reinterpret_cast<char *>(glyphs);
    }

    void grow(char *address, int totalGlyphs);
};

class QVarLengthGlyphLayoutArray : private QStdVector<void *>, public QGlyphLayout
{
private:
    typedef QStdVector<void *> Array;
public:
    QVarLengthGlyphLayoutArray(int totalGlyphs)
        : Array(QSPACEFORGLYPHS(totalGlyphs) / QT_POINTER_SIZE + 1)
        , QGlyphLayout(reinterpret_cast<char *>(Array::data()), totalGlyphs)
    {
        memset(Array::data(), 0, Array::size() * QT_POINTER_SIZE);
    }
};

template <int N> struct QGlyphLayoutArray : public QGlyphLayout
{
public:
    QGlyphLayoutArray()
        : QGlyphLayout(reinterpret_cast<char *>(buffer), N)
    {
        memset(buffer, 0, sizeof(buffer));
    }

private:
    void *buffer[QSPACEFORGLYPHS(N) / QT_POINTER_SIZE + 1];
};

struct QScriptItem;
/// Internal QTextItem
class QTextItemInt : public QTextItem
{
public:
    inline QTextItemInt()
        : justified(false), underlineStyle(QTextCharFormat::NoUnderline), num_chars(0), chars(0),
          logClusters(0), f(0), fontEngine(0)
    {}
    QTextItemInt(const QScriptItem &si, QFont *font, const QTextCharFormat &format = QTextCharFormat());
    QTextItemInt(const QGlyphLayout &g, QFont *font, const QChar *chars, int numChars, QFontEngine *fe,
                 const QTextCharFormat &format = QTextCharFormat());

    /// copy the structure items, adjusting the glyphs arrays to the right subarrays.
    /// the width of the returned QTextItemInt is not adjusted, for speed reasons
    void initWithScriptItem(const QScriptItem &si);

    QFixed descent;
    QFixed ascent;
    QFixed width;

    RenderFlags flags;
    bool justified;
    QTextCharFormat::UnderlineStyle underlineStyle;
    const QTextCharFormat charFormat;
    int num_chars;
    const QChar *chars;
    const unsigned short *logClusters;
    QFont *f;

    QGlyphLayout glyphs;
    QFontEngine *fontEngine;
};

struct Q_AUTOTEST_EXPORT QScriptItem
{
    inline QScriptItem()
        : position(0),
          num_glyphs(0), descent(-1), ascent(-1), leading(-1), width(-1),
          glyph_data_offset(0) {}
    inline QScriptItem(int p, const QScriptAnalysis &a)
        : position(p), analysis(a),
          num_glyphs(0), descent(-1), ascent(-1), leading(-1), width(-1),
          glyph_data_offset(0) {}

    int position;
    QScriptAnalysis analysis;
    unsigned short num_glyphs;
    QFixed descent;
    QFixed ascent;
    QFixed leading;
    QFixed width;
    int glyph_data_offset;
    QFixed height() const { return ascent + descent + 1; }
};

typedef QVector<QScriptItem> QScriptItemArray;

struct Q_AUTOTEST_EXPORT QScriptLine
{
    // created and filled in QTextLine::layout_helper
    QScriptLine()
        : from(0), trailingSpaces(0), length(0),
        justified(false), gridfitted(false),
        leadingIncluded(false) {}
    QFixed descent;
    QFixed ascent;
    QFixed leading;
    QFixed x;
    QFixed y;
    QFixed width;
    QFixed textWidth;
    QFixed textAdvance;
    int from;
    ushort trailingSpaces;
    int length;
    mutable bool justified;
    mutable bool gridfitted;
    bool leadingIncluded;
    QFixed height() const { return (ascent + descent).ceil() + 1
                            + (leadingIncluded?  qMax(QFixed(),leading) : QFixed()); }
    QFixed base() const { return ascent
                          + (leadingIncluded ? qMax(QFixed(),leading) : QFixed()); }
    void setDefaultHeight(QTextEngine *eng);
    void operator+=(const QScriptLine &other);
};


inline void QScriptLine::operator+=(const QScriptLine &other)
{
    leading= qMax(leading + ascent, other.leading + other.ascent) - qMax(ascent, other.ascent);
    descent = qMax(descent, other.descent);
    ascent = qMax(ascent, other.ascent);
    textWidth += other.textWidth;
    length += other.length;
}

typedef QStdVector<QScriptLine> QScriptLineArray;

class QFontPrivate;
class QTextFormatCollection;

class Q_GUI_EXPORT QTextEngine {
public:
    enum LayoutState {
        LayoutEmpty,
        InLayout,
        LayoutFailed
    };
    struct LayoutData {
        LayoutData();
        ~LayoutData();
        mutable QScriptItemArray items;
        int allocated;
        void **memory;
        unsigned short *logClustersPtr;
        QGlyphLayout glyphLayout;
        mutable int used;
        LayoutState layoutState;
        bool haveCharAttributes;
        QString string;
        bool reallocate(int totalGlyphs);
    };

    QTextEngine(LayoutData *data);
    QTextEngine();
    QTextEngine(const QString &str, const QFont &f);
    ~QTextEngine();

    // keep in sync with QAbstractFontEngine::TextShapingFlag!!
    enum ShaperFlag {
        GlyphIndicesOnly = 0x0004
    };
    Q_DECLARE_FLAGS(ShaperFlags, ShaperFlag)

    void invalidate();
    void clearLineData();

    void validate() const;
    void itemize() const;

    const HB_CharAttributes *attributes() const;

    void shape(int item) const;

    void justify(const QScriptLine &si);
    QFixed alignLine(const QScriptLine &line);

    QFixed width(int charFrom, int numChars) const;
    glyph_metrics_t boundingBox(int from,  int len) const;

    int length(int item) const {
        int from = layoutData->items[item].position;
        item++;
        return (item < layoutData->items.size() ? layoutData->items[item].position : layoutData->string.length()) - from;
    }
    int length(const QScriptItem *si) const {
        int end;
        if (si + 1 < layoutData->items.constData()+ layoutData->items.size())
            end = (si+1)->position;
        else
            end = layoutData->string.length();
        return end - si->position;
    }

    QFontEngine *fontEngine(const QScriptItem &si, QFixed *ascent = 0, QFixed *descent = 0, QFixed *leading = 0) const;
    QFont font(const QScriptItem &si) const;
    inline QFont font() const { return fnt; }

    /**
     * Returns a pointer to an array of log clusters, offset at the script item.
     * Each item in the array is a unsigned short.  For each character in the original string there is an entry in the table
     * so there is a one to one correlation in indexes between the original text and the index in the logcluster.
     * The value of each item is the position in the glyphs array. Multiple similar pointers in the logclusters array imply
     * that one glyph is used for more than one character.
     * \sa glyphs()
     */
    inline unsigned short *logClusters(const QScriptItem *si) const
        { return layoutData->logClustersPtr+si->position; }
    /**
     * Returns an array of QGlyphLayout items, offset at the script item.
     * Each item in the array matches one glyph in the text, storing the advance, position etc.
     * The returned item's length equals to the number of available glyphs. This may be more
     * than what was actually shaped.
     * \sa logClusters()
     */
    inline QGlyphLayout availableGlyphs(const QScriptItem *si) const {
        return layoutData->glyphLayout.mid(si->glyph_data_offset);
    }
    /**
     * Returns an array of QGlyphLayout items, offset at the script item.
     * Each item in the array matches one glyph in the text, storing the advance, position etc.
     * The returned item's length equals to the number of shaped glyphs.
     * \sa logClusters()
     */
    inline QGlyphLayout shapedGlyphs(const QScriptItem *si) const {
        return layoutData->glyphLayout.mid(si->glyph_data_offset, si->num_glyphs);
    }

    inline bool ensureSpace(int nGlyphs) const {
        if (layoutData->glyphLayout.numGlyphs - layoutData->used < nGlyphs)
            return layoutData->reallocate((((layoutData->used + nGlyphs)*3/2 + 15) >> 4) << 4);
        return true;
    }

    void freeMemory();

    int findItem(int strPos) const;
    inline QTextFormatCollection *formats() const {
        return block.docHandle()->formatCollection();
    }
    QTextCharFormat format(const QScriptItem *si) const;
    inline QAbstractTextDocumentLayout *docLayout() const {
        return block.docHandle()->document()->documentLayout();
    }
    int formatIndex(const QScriptItem *si) const;

    /// returns the width of tab at index (in the tabs array) with the tab-start at position x
    QFixed calculateTabWidth(int index, QFixed x) const;

    mutable QScriptLineArray lines;

    QString text;
    QFont fnt;
    QTextBlock block;

    QTextOption option;

    QFixed minWidth;
    QFixed maxWidth;
    QPointF position;
    bool cacheGlyphs;

    mutable LayoutData *layoutData;

    inline bool hasFormats() const { return (block.docHandle() || specialData); }

    struct SpecialData {
        QList<QTextLayout::FormatRange> addFormats;
        QStdVector<int> addFormatIndices;
        QStdVector<int> resolvedFormatIndices;
    };
    SpecialData *specialData;

    bool atWordSeparator(int position) const;
    bool atSpace(int position) const;
    void indexAdditionalFormats();

    QString elidedText(Qt::TextElideMode mode, const QFixed &width, int flags) const;

    void shapeLine(const QScriptLine &line);

    QFixed offsetInLigature(const QScriptItem *si, int pos, int max, int glyph_pos);
    int positionInLigature(const QScriptItem *si, int end, QFixed x, QFixed edge, int glyph_pos, bool cursorOnCharacter);
    int lineNumberForTextPosition(int pos);

private:
    void setBoundary(int strPos) const;
    void addRequiredBoundaries() const;
    void shapeText(int item) const;
    void shapeTextWithHarfbuzz(int item) const;

    void resolveAdditionalFormats() const;
    int getClusterLength(unsigned short *logClusters, const HB_CharAttributes *attributes, int to, int glyph_pos, int *start);
};

struct QTextLineItemIterator
{
    QTextLineItemIterator(QTextEngine *eng, int lineNum, const QPointF &pos = QPointF(),
                          const QTextLayout::FormatRange *_selection = 0);

    inline bool atEnd() const { return logicalItem >= nItems - 1; }
    inline bool atBeginning() const { return logicalItem <= 0; }
    QScriptItem &next();

    bool getSelectionBounds(QFixed *selectionX, QFixed *selectionWidth) const;
    inline bool isOutsideSelection() const {
        QFixed tmp1, tmp2;
        return !getSelectionBounds(&tmp1, &tmp2);
    }

    QTextEngine *eng;

    QFixed x;
    const QScriptLine &line;
    QScriptItem *si;

    int lineNum;
    int lineEnd;
    int firstItem;
    int lastItem;
    int nItems;
    int logicalItem;
    int item;
    int itemLength;

    int glyphsStart;
    int glyphsEnd;
    int itemStart;
    int itemEnd;

    QFixed itemWidth;

    const QTextLayout::FormatRange *selection;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QTextEngine::ShaperFlags)

QT_END_NAMESPACE

#endif // QTEXTENGINE_P_H
