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

#ifndef QFONTENGINE_P_H
#define QFONTENGINE_P_H

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

#include "QtCore/qatomic.h"
#include <QtCore/qvarlengtharray.h>
#include "qtextengine_p.h"
#include "qfont_p.h"

struct glyph_metrics_t;
typedef unsigned int glyph_t;

QT_BEGIN_NAMESPACE

class QChar;
class QPainterPath;

class QTextEngine;
struct QGlyphLayout;

#define MAKE_TAG(ch1, ch2, ch3, ch4) (\
    (((quint32)(ch1)) << 24) | \
    (((quint32)(ch2)) << 16) | \
    (((quint32)(ch3)) << 8) | \
    ((quint32)(ch4)) \
   )


class Q_GUI_EXPORT QFontEngine : public QObject
{
public:
    enum Type {
        Box,
        Freetype
    };

    QFontEngine();
    virtual ~QFontEngine();

    // all of these are in unscaled metrics if the engine supports uncsaled metrics,
    // otherwise in design metrics
    struct Properties {
        QByteArray postscriptName;
        QByteArray copyright;
        QRectF boundingBox;
        QFixed emSquare;
        QFixed ascent;
        QFixed descent;
        QFixed leading;
        QFixed italicAngle;
        QFixed capHeight;
        QFixed lineWidth;
    };
    virtual Properties properties() const;
    virtual void getUnscaledGlyph(glyph_t glyph, QPainterPath *path, glyph_metrics_t *metrics);
    QByteArray getSfntTable(uint /*tag*/) const;
    virtual bool getSfntTableData(uint /*tag*/, uchar * /*buffer*/, uint * /*length*/) const { return false; }

    struct FaceId {
        FaceId() : index(0) {}
        QByteArray filename;
        int index;
    };
    virtual FaceId faceId() const { return FaceId(); }
    enum SynthesizedFlags {
        SynthesizedItalic = 0x1,
        SynthesizedBold = 0x2,
        SynthesizedStretch = 0x4
    };
    virtual int synthesized() const { return 0; }

    virtual QFixed emSquareSize() const { return ascent(); }

    /* returns 0 as glyph index for non existent glyphs */
    virtual bool stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs, QTextEngine::ShaperFlags flags) const = 0;

    /**
     * This is a callback from harfbuzz. The font engine uses the font-system in use to find out the
     * advances of each glyph and set it on the layout.
     */
    virtual void recalcAdvances(QGlyphLayout *, QTextEngine::ShaperFlags) const {}
    virtual void doKerning(QGlyphLayout *, QTextEngine::ShaperFlags);

    virtual void addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int nglyphs,
                                 QPainterPath *path, QTextItem::RenderFlags flags) = 0;

    void getGlyphPositions(const QGlyphLayout &glyphs, const QTransform &matrix, QTextItem::RenderFlags flags,
                           QVarLengthArray<glyph_t> &glyphs_out, QVarLengthArray<QFixedPoint> &positions);

    virtual void addOutlineToPath(qreal, qreal, const QGlyphLayout &, QPainterPath *, QTextItem::RenderFlags flags);

    virtual glyph_metrics_t boundingBox(const QGlyphLayout &glyphs) const = 0;
    virtual glyph_metrics_t boundingBox(glyph_t glyph) const = 0;

    virtual QFixed ascent() const = 0;
    virtual QFixed descent() const = 0;
    virtual QFixed leading() const = 0;
    virtual QFixed xHeight() const;
    virtual QFixed averageCharWidth() const;

    virtual QFixed lineThickness() const;
    virtual QFixed underlinePosition() const;

    virtual qreal maxCharWidth() const = 0;
    virtual qreal minLeftBearing() const { return qreal(); }
    virtual qreal minRightBearing() const { return qreal(); }

    virtual void getGlyphBearings(glyph_t glyph, qreal *leftBearing = 0, qreal *rightBearing = 0);

    virtual const char *name() const = 0;

    virtual bool canRender(const QChar *string, int len) = 0;

    virtual Type type() const = 0;

    static QByteArray convertToPostscriptFontFamilyName(const QByteArray &fontFamily);

    QAtomicInt ref;
    QFontDef fontDef;
    int fsType;
#if defined(Q_WS_X11)
    struct KernPair {
        uint left_right;
        QFixed adjust;

        inline bool operator<(const KernPair &other) const
        {
            return left_right < other.left_right;
        }
    };
    QVector<KernPair> kerning_pairs;
    void loadKerningPairs(QFixed scalingFactor);
#endif
};

inline bool operator ==(const QFontEngine::FaceId &f1, const QFontEngine::FaceId &f2)
{
    return (f1.index == f2.index) && (f1.filename == f2.filename);
}

inline uint qHash(const QFontEngine::FaceId &f)
{
    return qHash(f.index << 16) + qHash(f.filename);
}


class QFontEngineBox : public QFontEngine
{
public:
    QFontEngineBox(int size);
    ~QFontEngineBox();

    virtual bool stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs, QTextEngine::ShaperFlags flags) const;
    virtual void recalcAdvances(QGlyphLayout *, QTextEngine::ShaperFlags) const;

    virtual void addOutlineToPath(qreal x, qreal y, const QGlyphLayout &glyphs, QPainterPath *path, QTextItem::RenderFlags flags);
    virtual void addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int nglyphs,
                                 QPainterPath *path, QTextItem::RenderFlags flags);

    virtual glyph_metrics_t boundingBox(const QGlyphLayout &glyphs) const;
    virtual glyph_metrics_t boundingBox(glyph_t glyph) const;

    virtual QFixed ascent() const;
    virtual QFixed descent() const;
    virtual QFixed leading() const;
    virtual qreal maxCharWidth() const;
    virtual qreal minLeftBearing() const { return 0; }
    virtual qreal minRightBearing() const { return 0; }

    virtual const char *name() const;

    virtual bool canRender(const QChar *string, int len);

    virtual Type type() const;
    inline int size() const { return _size; }

private:
    int _size;
};

QT_END_NAMESPACE

#endif // QFONTENGINE_P_H
