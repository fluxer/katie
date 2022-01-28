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

#include "qmath.h"
#include "qfontengine_ft_p.h"
#include "qcorecommon_p.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_TRUETYPE_TABLES_H
#include FT_TYPE1_TABLES_H
#include FT_GLYPH_H

#if defined(FT_CONFIG_OPTIONS_H)
#include FT_CONFIG_OPTIONS_H
#endif

#if defined(FT_ERRORS_H)
#include FT_ERRORS_H
#endif

#if defined(FT_LCD_FILTER_H)
#include FT_LCD_FILTER_H
#endif

QT_BEGIN_NAMESPACE

#define FLOOR(x)    ((x) & -64)
#define CEIL(x)     (((x)+63) & -64)
#define TRUNC(x)    ((x) >> 6)
#define ROUND(x)    (((x)+32) & -64)

// failsafe in case Freetype breaks ABI
#define QT_MEMCPY_FT_OUTLINE

// -------------------------- Freetype support ------------------------------

QFreetypeFace::QFreetypeFace(const QFontEngine::FaceId &face_id)
    : face(nullptr),
    library(nullptr)
{
    FT_Init_FreeType(&library);

    if (face_id.filename.isEmpty()) {
        return;
    }

    if (FT_New_Face(library, face_id.filename, face_id.index, &face) != 0) {
        return;
    }

    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
}

QFreetypeFace::~QFreetypeFace()
{
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

void QFreetypeFace::addGlyphToPath(FT_Outline outline, const QFixedPoint &point, QPainterPath *path)
{
    static const qreal factor = (1.0 / 64.0);

    QPointF cp = point.toPointF();

    // convert the outline to a painter path
    int i = 0;
    for (int j = 0; j < outline.n_contours; ++j) {
        int last_point = outline.contours[j];
        QPointF start = cp + QPointF(outline.points[i].x*factor, -outline.points[i].y*factor);
        if(!(outline.tags[i] & 1)) {
            start += cp + QPointF(outline.points[last_point].x*factor, -outline.points[last_point].y*factor);
            start /= 2;
        }
        // qDebug("contour: %d -- %d", i, outline.contours[j]);
        // qDebug("first point at %f %f", start.x(), start.y());
        path->moveTo(start);

        QPointF c[4];
        c[0] = start;
        int n = 1;
        while (i < last_point) {
            ++i;
            c[n] = cp + QPointF(outline.points[i].x*factor, -outline.points[i].y*factor);
            // qDebug() << "    i=" << i << " flag=" << (int)outline.tags[i] << "point=" << c[n];
            ++n;
            switch (outline.tags[i] & 3) {
                case 2: {
                    // cubic bezier element
                    if (n < 4)
                        continue;
                    c[3] = (c[3] + c[2])/2;
                    --i;
                    break;
                }
                case 0: {
                    // quadratic bezier element
                    if (n < 3)
                        continue;
                    c[3] = (c[1] + c[2])/2;
                    c[2] = (2*c[1] + c[3])/3;
                    c[1] = (2*c[1] + c[0])/3;
                    --i;
                    break;
                }
                case 1:
                case 3: {
                    if (n == 2) {
                        // qDebug() << "lineTo" << c[1];
                        path->lineTo(c[1]);
                        c[0] = c[1];
                        n = 1;
                        continue;
                    } else if (n == 3) {
                        c[3] = c[2];
                        c[2] = (2*c[1] + c[3])/3;
                        c[1] = (2*c[1] + c[0])/3;
                    }
                    break;
                }
            }
            // qDebug() << "cubicTo" << c[1] << c[2] << c[3];
            path->cubicTo(c[1], c[2], c[3]);
            c[0] = c[3];
            n = 1;
        }
        if (n == 1) {
            // qDebug() << "closeSubpath";
            path->closeSubpath();
        } else {
            c[3] = start;
            if (n == 2) {
                c[2] = (2*c[1] + c[3])/3;
                c[1] = (2*c[1] + c[0])/3;
            }
            // qDebug() << "cubicTo" << c[1] << c[2] << c[3];
            path->cubicTo(c[1], c[2], c[3]);
        }
        ++i;
    }
}

#ifndef QT_NO_FONTCONFIG
QFontEngineFT::QFontEngineFT(const QFontDef &fd, FcPattern *pattern)
    : default_load_flags(FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH | FT_LOAD_NO_BITMAP),
    default_hint_style(HintNone),
    freetype(nullptr),
    xsize(0),
    ysize(0),
    line_thickness(QFixed::fromFixed(1)),
    underline_position(QFixed::fromReal(0.8)),
    kerning_pairs_loaded(false)
{
    fontDef = fd;

    // FcPatternPrint(pattern);
    FcChar8 *fileName;

    FcPatternGetString(pattern, FC_FILE, 0, &fileName);
    face_id.filename = (const char *)fileName;

    if (!FcPatternGetInteger(pattern, FC_INDEX, 0, &face_id.index)) {
        face_id.index = 0;
    }

#if defined(FC_AUTOHINT) && defined(FT_LOAD_FORCE_AUTOHINT)
    bool autohint = false;
    FcBool b = FcFalse;
    if (FcPatternGetBool(pattern, FC_AUTOHINT, 0, &b) == FcResultMatch) {
        autohint = b;
    }
    if (autohint) {
        default_load_flags |= FT_LOAD_FORCE_AUTOHINT;
    }
#endif

    init();
}
#endif // QT_NO_FONTCONFIG

QFontEngineFT::QFontEngineFT(const QFontDef &fd)
    : default_load_flags(FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH | FT_LOAD_NO_BITMAP),
    default_hint_style(HintNone),
    freetype(nullptr),
    xsize(0),
    ysize(0),
    line_thickness(QFixed::fromFixed(1)),
    underline_position(QFixed::fromReal(0.8)),
    kerning_pairs_loaded(false)
{
    fontDef = fd;

    face_id.filename = fd.family.toUtf8();
    face_id.index = 0;

    init();
}

void QFontEngineFT::init()
{
    freetype = new QFreetypeFace(face_id);
    if (!freetype->face) {
        return;
    } else if (!FT_IS_SCALABLE(freetype->face)) {
        return;
    }

    lbearing = rbearing = SHRT_MIN;
    ysize = qRound(fontDef.pixelSize * 64);
    xsize = (ysize * fontDef.stretch / 100);

    setFace(QFontEngineFT::Scaled);
    FT_Face face = getFace();

    // underline metrics
    line_thickness = QFixed::fromFixed(FT_MulFix(face->underline_thickness, face->size->metrics.y_scale));
    underline_position = QFixed::fromFixed(-FT_MulFix(face->underline_position, face->size->metrics.y_scale));
    if (line_thickness < 1) {
        line_thickness = 1;
    }

    fontDef.styleName = QString::fromUtf8(face->style_name);

    TT_OS2 *os2 = (TT_OS2 *)FT_Get_Sfnt_Table(face, ft_sfnt_os2);
    if (os2)
        fsType = os2->fsType;

    switch (fontDef.hintingPreference) {
        case QFont::PreferNoHinting: {
            default_hint_style = HintNone;
            break;
        }
        case QFont::PreferVerticalHinting: {
            default_hint_style = HintLight;
            break;
        }
        case QFont::PreferFullHinting:
        default: {
            default_hint_style = HintFull;
            break;
        }
    }
}

QFontEngineFT::~QFontEngineFT()
{
    if (freetype) {
        GlyphCache::const_iterator iter = glyphcache.begin();
        GlyphCache::const_iterator iterend = glyphcache.end();
        while (iter != iterend) {
#ifdef QT_MEMCPY_FT_OUTLINE
            QFontGlyph* gcache = iter->second;
            ::free(gcache->outline.contours);
            ::free(gcache->outline.points);
            ::free(gcache->outline.tags);
#else
            FT_Outline_Done(freetype->library, &(iter->second->outline));
#endif
            delete iter->second;
            iter++;
        }

        delete freetype;
    }
}

bool QFontEngineFT::loadGlyph(glyph_t glyph) const
{
    int load_flags = default_load_flags;

    if (default_hint_style == HintNone) {
        load_flags |= FT_LOAD_NO_HINTING;
    } else if (default_hint_style == HintLight) {
        load_flags |= FT_LOAD_TARGET_LIGHT;
    } else if (default_hint_style == HintMedium) {
        load_flags |= FT_LOAD_TARGET_LCD;
    } else if (default_hint_style == HintFull) {
        load_flags |= FT_LOAD_TARGET_LCD_V;
    } else {
        load_flags |= FT_LOAD_TARGET_MONO;
    }

    FT_Face face = freetype->face;
    FT_Error err = FT_Load_Glyph(face, glyph, load_flags);
    if (Q_UNLIKELY(err != FT_Err_Ok)) {
        qWarning("load glyph failed err=%x face=%p, glyph=%d", err, face, glyph);
        return false;
    }

    FT_GlyphSlot slot = face->glyph;
    if (Q_UNLIKELY(slot->format != FT_GLYPH_FORMAT_OUTLINE)) {
        qWarning("non-outline format is not supported format=%d face=%p, glyph=%d", slot->format, face, glyph);
        return false;
    }

    return true;
}

QFontGlyph* QFontEngineFT::getGlyph(glyph_t glyph) const
{
    GlyphCache::const_iterator iter = glyphcache.find(glyph);
    if (iter != glyphcache.end()) {
        return iter->second;
    }

    loadGlyph(glyph);
    FT_Face face = getFace();

    QFontGlyph* gcache = new QFontGlyph();
    gcache->left = FLOOR(face->glyph->metrics.horiBearingX);
    gcache->right = CEIL(face->glyph->metrics.horiBearingX + face->glyph->metrics.width);
    gcache->top = CEIL(face->glyph->metrics.horiBearingY);
    gcache->bottom = FLOOR(face->glyph->metrics.horiBearingY - face->glyph->metrics.height);
    gcache->linearhoriadvance = (face->glyph->linearHoriAdvance >> 10);
    gcache->horiadvance = face->glyph->metrics.horiAdvance;
    gcache->advancex = ROUND(face->glyph->advance.x);

#ifdef QT_MEMCPY_FT_OUTLINE
    short n_contours = face->glyph->outline.n_contours;
    gcache->outline.n_contours = n_contours;
    gcache->outline.contours = static_cast<short*>(::malloc(sizeof(short) * n_contours));
    ::memcpy(gcache->outline.contours, face->glyph->outline.contours, sizeof(short) * n_contours);
    short n_points = face->glyph->outline.n_points;
    gcache->outline.points = static_cast<FT_Vector*>(::malloc(sizeof(FT_Vector) * n_points));
    ::memcpy(gcache->outline.points, face->glyph->outline.points, sizeof(FT_Vector) * n_points);
    short n_tags = n_points;
    gcache->outline.tags = static_cast<char*>(::malloc(sizeof(char) * n_tags));
    ::memcpy(gcache->outline.tags, face->glyph->outline.tags, sizeof(char) * n_tags);
#else
    FT_Outline_New(freetype->library, face->glyph->outline.n_points,
                   face->glyph->outline.n_contours, &gcache->outline);
    FT_Outline_Copy(&face->glyph->outline, &gcache->outline);
#endif

    glyphcache.insert({glyph, gcache});

    return gcache;
}

QFontEngine::FaceId QFontEngineFT::faceId() const
{
    return face_id;
}

QFontEngine::Properties QFontEngineFT::properties() const
{
    FT_Face face = getFace();

    QFontEngine::Properties p;
    p.postscriptName = FT_Get_Postscript_Name(face);
    PS_FontInfoRec font_info;
    if (FT_Get_PS_Font_Info(face, &font_info) == 0) {
        p.copyright = font_info.notice;
    }
    p.ascent = face->ascender;
    p.descent = -face->descender;
    p.leading = face->height - face->ascender + face->descender;
    p.emSquare = face->units_per_EM;
    p.boundingBox = QRectF(face->bbox.xMin, -face->bbox.yMax,
                           face->bbox.xMax - face->bbox.xMin,
                           face->bbox.yMax - face->bbox.yMin);
    p.italicAngle = 0;
    p.capHeight = p.ascent;
    p.lineWidth = face->underline_thickness;

    if (p.postscriptName.isEmpty()) {
        p.postscriptName = QFontEngine::convertToPostscriptFontFamilyName(fontDef.family.toUtf8());
    }

    return p;
}

bool QFontEngineFT::getSfntTableData(uint tag, uchar *buffer, uint *length) const
{
    bool result = false;
#if (FREETYPE_MAJOR*10000 + FREETYPE_MINOR*100 + FREETYPE_PATCH) > 20103
    if (FT_IS_SFNT(freetype->face)) {
        FT_ULong len = *length;
        result = FT_Load_Sfnt_Table(freetype->face, tag, 0, buffer, &len) == FT_Err_Ok;
        *length = len;
    }
#endif
    return result;
}

int QFontEngineFT::synthesized() const
{
    int result = 0;
    if (fontDef.stretch != 100) {
        result |= SynthesizedStretch;
    }
    return result;
}

QFixed QFontEngineFT::ascent() const
{
    return QFixed::fromFixed(freetype->face->size->metrics.ascender);
}

QFixed QFontEngineFT::descent() const
{
    // subtract a pixel to work around QFontMetrics's built-in + 1
    return QFixed::fromFixed(-(freetype->face->size->metrics.descender) - 64);
}

QFixed QFontEngineFT::leading() const
{
    return QFixed::fromFixed(freetype->face->size->metrics.height - freetype->face->size->metrics.ascender + freetype->face->size->metrics.descender);
}

QFixed QFontEngineFT::xHeight() const
{
    TT_OS2 *os2 = (TT_OS2 *)FT_Get_Sfnt_Table(freetype->face, ft_sfnt_os2);
    if (os2 && os2->sxHeight) {
        return QFixed(os2->sxHeight*freetype->face->size->metrics.y_ppem)/freetype->face->units_per_EM;
    }
    return QFontEngine::xHeight();
}

QFixed QFontEngineFT::averageCharWidth() const
{
    TT_OS2 *os2 = (TT_OS2 *)FT_Get_Sfnt_Table(freetype->face, ft_sfnt_os2);
    if (os2 && os2->xAvgCharWidth) {
        return QFixed(os2->xAvgCharWidth*freetype->face->size->metrics.x_ppem)/freetype->face->units_per_EM;
    }
    return QFontEngine::averageCharWidth();
}

qreal QFontEngineFT::maxCharWidth() const
{
    return freetype->face->size->metrics.max_advance >> 6;
}

static const ushort char_table[] = {
        40,
        67,
        70,
        75,
        86,
        88,
        89,
        91,
        102,
        114,
        124,
        127,
        205,
        645,
        884,
        922,
        1070,
        12386
};

static const int char_table_entries = sizeof(char_table)/sizeof(ushort);


qreal QFontEngineFT::minLeftBearing() const
{
    if (lbearing == SHRT_MIN)
        (void) minRightBearing(); // calculates both
    return lbearing.toReal();
}

qreal QFontEngineFT::minRightBearing() const
{
    if (rbearing == SHRT_MIN) {
        lbearing = rbearing = 0;
        const QChar *ch = reinterpret_cast<const QChar *>(char_table);
        QGlyphLayoutArray<char_table_entries> glyphs;
        int ng = char_table_entries;
        stringToCMap(ch, char_table_entries, &glyphs, &ng, QTextEngine::GlyphIndicesOnly);
        while (--ng) {
            if (glyphs.glyphs[ng]) {
                glyph_metrics_t gi = boundingBox(glyphs.glyphs[ng]);
                lbearing = qMin(lbearing, gi.x);
                rbearing = qMin(rbearing, (gi.xoff - gi.x - gi.width));
            }
        }
    }
    return rbearing.toReal();
}

QFixed QFontEngineFT::lineThickness() const
{
    return line_thickness;
}

QFixed QFontEngineFT::underlinePosition() const
{
    return underline_position;
}

void QFontEngineFT::doKerning(QGlyphLayout *g, QTextEngine::ShaperFlags flags)
{
    if (!kerning_pairs_loaded) {
        kerning_pairs_loaded = true;
        if (freetype->face->size->metrics.x_ppem != 0) {
            QFixed scalingFactor(freetype->face->units_per_EM/freetype->face->size->metrics.x_ppem);
            loadKerningPairs(scalingFactor);
        }
    }
    QFontEngine::doKerning(g, flags);
}

void QFontEngineFT::getUnscaledGlyph(glyph_t glyph, QPainterPath *path, glyph_metrics_t *metrics)
{
    setFace(QFontEngineFT::Unscaled);

    loadGlyph(glyph);
    FT_Face face = getFace();

    int left  = face->glyph->metrics.horiBearingX;
    int right = face->glyph->metrics.horiBearingX + face->glyph->metrics.width;
    int top    = face->glyph->metrics.horiBearingY;
    int bottom = face->glyph->metrics.horiBearingY - face->glyph->metrics.height;

    QFixedPoint p;
    p.x = 0;
    p.y = 0;

    metrics->width = QFixed::fromFixed(right-left);
    metrics->height = QFixed::fromFixed(top-bottom);
    metrics->x = QFixed::fromFixed(left);
    metrics->y = QFixed::fromFixed(-top);
    metrics->xoff = QFixed::fromFixed(face->glyph->advance.x);

    QFreetypeFace::addGlyphToPath(face->glyph->outline, p, path);

    setFace(QFontEngineFT::Scaled);
}

static inline unsigned int getChar(const QChar *str, int &i, const int len)
{
    uint ucs4 = str[i].unicode();
    if (str[i].isHighSurrogate() && i < len-1 && str[i+1].isLowSurrogate()) {
        ++i;
        ucs4 = QChar::surrogateToUcs4(ucs4, str[i].unicode());
    }
    return ucs4;
}

bool QFontEngineFT::canRender(const QChar *string, int len)
{
    FT_Face face = freetype->face;
    for (int i = 0; i < len; i++ ) {
        unsigned int uc = getChar(string, i, len);
        const CharCache::const_iterator iter = charcache.find(uc);
        if (iter == charcache.end()) {
            glyph_t glyph = FT_Get_Char_Index(face, uc);
            if (glyph == 0) {
                return false;
            }
            charcache.insert({uc, glyph});
        }
    }
    return true;
}

void QFontEngineFT::addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int numGlyphs,
                                    QPainterPath *path)
{
    for (int gl = 0; gl < numGlyphs; gl++) {
        QFontGlyph* gcache = getGlyph(glyphs[gl]);
        Q_ASSERT(gcache);

        QFreetypeFace::addGlyphToPath(gcache->outline, positions[gl], path);
    }
}

bool QFontEngineFT::stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs,
                                 QTextEngine::ShaperFlags flags) const
{
    if (*nglyphs < len) {
        *nglyphs = len;
        return false;
    }

    int glyph_pos = 0;
    for (int i = 0; i < len; ++i) {
        unsigned int uc = getChar(str, i, len);
        const CharCache::const_iterator iter = charcache.find(uc);
        glyph_t glyph = 0;
        if (iter == charcache.end()) {
            glyph = FT_Get_Char_Index(freetype->face, uc);
            charcache.insert({uc, glyph});
        } else {
            glyph = iter->second;
        }
        glyphs->glyphs[glyph_pos] = glyph;
        ++glyph_pos;
    }

    *nglyphs = glyph_pos;
    glyphs->numGlyphs = glyph_pos;

    if (flags & QTextEngine::GlyphIndicesOnly) {
        return true;
    }

    recalcAdvances(glyphs, flags);

    return true;
}

void QFontEngineFT::recalcAdvances(QGlyphLayout *glyphs, QTextEngine::ShaperFlags flags) const
{
    bool design = (default_hint_style == HintNone ||
                   default_hint_style == HintLight ||
                   (flags & QTextEngine::DesignMetrics));
    for (int i = 0; i < glyphs->numGlyphs; i++) {
        QFontGlyph* gcache = getGlyph(glyphs->glyphs[i]);
        Q_ASSERT(gcache);

        glyphs->advances_x[i] = design ? QFixed::fromFixed(gcache->linearhoriadvance)
                                       : QFixed::fromFixed(gcache->horiadvance);
    }
}

glyph_metrics_t QFontEngineFT::boundingBox(const QGlyphLayout &glyphs) const
{
    glyph_metrics_t overall;
    // initialize with line height, we get the same behaviour on all platforms
    overall.y = -ascent();
    overall.height = ascent() + descent() + 1;

    for (int i = 0; i < glyphs.numGlyphs; i++) {
        glyph_metrics_t gm = boundingBox(glyphs.glyphs[i]);

        overall.height = qMax(overall.height, gm.height);
        overall.width = qMax(overall.width, gm.width);
        overall.x = qMin(overall.x, gm.x);
        overall.y = qMin(overall.y, gm.y);
        overall.xoff += gm.xoff;
    }

    return overall;
}

glyph_metrics_t QFontEngineFT::boundingBox(glyph_t glyph) const
{
    glyph_metrics_t overall;

    QFontGlyph* gcache = getGlyph(glyph);
    Q_ASSERT(gcache);

    overall.width = TRUNC(gcache->right - gcache->left);
    overall.height = TRUNC(gcache->top - gcache->bottom);
    overall.x = TRUNC(gcache->left);
    overall.y = -TRUNC(gcache->top);
    overall.xoff = TRUNC(gcache->advancex);
    return overall;
}

FT_Face QFontEngineFT::getFace() const
{
    return freetype->face;
}

void QFontEngineFT::setFace(Scaling scale)
{
    FT_Face face = freetype->face;
    if (scale == Unscaled) {
        FT_Set_Char_Size(face, face->units_per_EM << 6, face->units_per_EM << 6, 0, 0);
    } else {
        FT_Set_Char_Size(face, xsize, ysize, 0, 0);
    }
}

QT_END_NAMESPACE
