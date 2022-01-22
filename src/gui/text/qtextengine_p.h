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

#include "QtCore/qvarlengtharray.h"
#include "qfont_p.h"
#include "QtGui/qpaintengine.h"
#include "qtextdocument_p.h"

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
Q_DECLARE_TYPEINFO(glyph_metrics_t, Q_PRIMITIVE_TYPE);

typedef uint32_t HB_Glyph;

#define QSPACEFORGLYPHS(__glyphcount) \
    (__glyphcount * (sizeof(HB_Glyph) + sizeof(QFixed) + sizeof(bool)))

struct QGlyphLayout
{
    // init to 0 not needed, done when shaping
    HB_Glyph *glyphs; // 4 bytes per element
    QFixed *advances_x; // 4 bytes per element
    bool *dontPrint;

    int numGlyphs;

    inline QGlyphLayout() : numGlyphs(0) {}

    inline explicit QGlyphLayout(char *address, int totalGlyphs)
    {
        glyphs = reinterpret_cast<HB_Glyph *>(address);
        int offset = totalGlyphs * sizeof(HB_Glyph);
        advances_x = reinterpret_cast<QFixed *>(address + offset);
        offset += totalGlyphs * sizeof(QFixed);
        dontPrint = reinterpret_cast<bool *>(address + offset);
        offset += totalGlyphs * sizeof(bool);
        numGlyphs = totalGlyphs;
    }

    inline char *data() {
        return reinterpret_cast<char *>(glyphs);
    }

    void grow(char *address, int totalGlyphs);
};

class QVarLengthGlyphLayoutArray : private QVarLengthArray<void *>, public QGlyphLayout
{
private:
    typedef QVarLengthArray<void *> Array;
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
    QTextItemInt();
    QTextItemInt(const QTextItemInt &other);

    RenderFlags flags;
    const QFont *f;
    QString text;

    QGlyphLayout glyphs;
    QFontEngine *fontEngine;

    int position;
    int size;
    QFixed x;
    QFixed y;
    QFixed width;
    QFixed height;
    QFixed ascent;
    QFixed descent;
    QFixed leading;
    bool leadingincluded;

    QTextCharFormat::UnderlineStyle underlineStyle;
    QTextCharFormat charFormat;
};

class Q_GUI_EXPORT QTextEngine {
public:
    enum LayoutState {
        LayoutEmpty,
        InLayout,
        LayoutFailed
    };

    QTextEngine();
    QTextEngine(const QString &str, const QFont &f);
    ~QTextEngine();

    enum ShaperFlag {
        DesignMetrics = 0x0002,
        GlyphIndicesOnly = 0x0004
    };
    Q_DECLARE_FLAGS(ShaperFlags, ShaperFlag)

    void invalidate();
    void updateLine(const int line);
    void createLinesForBlock();

    QVector<QTextItemInt> lines;

    QString text;
    QFont fnt;
    QTextOption option;
    QTextBlock block;
    QPointF position;
    LayoutState state;
    QList<QTextLayout::FormatRange> formats;

    bool atWordSeparator(int position) const;
    bool atSpace(int position) const;

    QFixed width(int charFrom, int numChars) const;
    glyph_metrics_t boundingBox(int from,  int len) const;
    QString elidedText(Qt::TextElideMode mode, const QFixed &width, int flags = 0) const;

    QString linesText() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QTextEngine::ShaperFlags)

QT_END_NAMESPACE

#endif // QTEXTENGINE_P_H
