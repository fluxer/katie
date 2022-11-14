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
#ifndef QFONTENGINE_FT_P_H
#define QFONTENGINE_FT_P_H
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

#include "qfontengine_p.h"
#include "qstdcontainers_p.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#if defined(Q_WS_X11)
#include "qt_x11_p.h"
#endif

#ifndef QT_NO_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif

QT_BEGIN_NAMESPACE


struct QFontGlyph {
    int left;
    int right;
    int top;
    int bottom;
    FT_Pos horiadvance;
    FT_Pos advancex;

    FT_Outline outline;
};

/*
 * This struct represents one font file on disk.
 */
class QFreetypeFace
{
public:
    QFreetypeFace(const QFontEngine::FaceId &face_id);
    ~QFreetypeFace();

    FT_Face face;
    FT_Library library;

    static void addGlyphToPath(FT_Outline outline, const QFixedPoint &point, QPainterPath *path);

private:
    Q_DISABLE_COPY(QFreetypeFace);
};

class Q_GUI_EXPORT QFontEngineFT : public QFontEngine
{
public:
#ifndef QT_NO_FONTCONFIG
    QFontEngineFT(const QFontDef &fd, FcPattern *pattern);
#endif
    QFontEngineFT(const QFontDef &fd);
    virtual ~QFontEngineFT();

    virtual QFontEngine::FaceId faceId() const;
    virtual QFontEngine::Properties properties() const;

    virtual bool getSfntTableData(uint tag, uchar *buffer, uint *length) const;
    virtual int synthesized() const;

    virtual QFixed ascent() const;
    virtual QFixed descent() const;
    virtual QFixed leading() const;
    virtual QFixed xHeight() const;
    virtual QFixed averageCharWidth() const;

    virtual qreal maxCharWidth() const;
    virtual qreal minLeftBearing() const;
    virtual qreal minRightBearing() const;
    virtual QFixed lineThickness() const;
    virtual QFixed underlinePosition() const;

    void doKerning(QGlyphLayout *);

    inline virtual Type type() const
    { return QFontEngine::Freetype; }
    inline virtual const char *name() const
    { return "freetype"; }

    virtual void getUnscaledGlyph(glyph_t glyph, QPainterPath *path, glyph_metrics_t *metrics);

    virtual bool canRender(const QChar *string, int len);

    virtual void addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int nglyphs,
                         QPainterPath *path);

    virtual bool stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs,
                      QTextEngine::ShaperFlags flags) const;

    virtual glyph_metrics_t boundingBox(const QGlyphLayout &glyphs) const;
    virtual glyph_metrics_t boundingBox(glyph_t glyph) const;

    virtual void recalcAdvances(QGlyphLayout *glyphs) const;

    enum Scaling {
        Scaled,
        Unscaled
    };
    void setFace(Scaling scale);
    FT_Face getFace() const;

    inline bool invalid() const { return xsize == 0 && ysize == 0; }

protected:
    int default_load_flags;

private:
    void init();
    bool loadGlyph(glyph_t glyph) const;
    QFontGlyph* getGlyph(glyph_t glyph) const;

    QFreetypeFace *freetype;
    QFontEngine::FaceId face_id;

    int xsize;
    int ysize;

    mutable QFixed lbearing;
    mutable QFixed rbearing;
    QFixed line_thickness;
    QFixed underline_position;

    bool kerning_pairs_loaded;

    typedef QStdMap<uint, glyph_t> CharCache;
    mutable CharCache charcache;

    typedef QStdMap<glyph_t, QFontGlyph*> GlyphCache;
    mutable GlyphCache glyphcache;
};


QT_END_NAMESPACE

#endif // QFONTENGINE_FT_P_H
