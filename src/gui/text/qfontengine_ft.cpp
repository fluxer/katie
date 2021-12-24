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

#include "qdir.h"
#include "qmetatype.h"
#include "qtextstream.h"
#include "qvariant.h"
#include "qfile.h"
#include "qabstractfileengine.h"
#include "qmath.h"
#include "qharfbuzz_p.h"
#include "qfontengine_ft_p.h"
#include "qguicommon_p.h"
#include "qcorecommon_p.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_SYNTHESIS_H
#include FT_TRUETYPE_TABLES_H
#include FT_TYPE1_TABLES_H
#include FT_GLYPH_H

#if defined(FT_LCD_FILTER_H)
#include FT_LCD_FILTER_H
#endif

#if defined(FT_CONFIG_OPTIONS_H)
#include FT_CONFIG_OPTIONS_H
#endif

#if defined(FT_ERRORS_H)
#include FT_ERRORS_H
#endif

#if !defined(QT_MAX_CACHED_GLYPH_SIZE)
#  define QT_MAX_CACHED_GLYPH_SIZE 64
#endif

QT_BEGIN_NAMESPACE

#define FLOOR(x)    ((x) & -64)
#define CEIL(x)	    (((x)+63) & -64)
#define TRUNC(x)    ((x) >> 6)
#define ROUND(x)    (((x)+32) & -64)

// -------------------------- Freetype support ------------------------------

QFreetypeFace::QFreetypeFace(const QFontEngine::FaceId &face_id)
    : face(nullptr),
    xsize(0),
    ysize(0),
    unicode_map(nullptr),
    symbol_map(nullptr),
    library(nullptr)
{

    matrix.xx = 0x10000;
    matrix.yy = 0x10000;
    matrix.xy = 0;
    matrix.yx = 0;

    FT_Init_FreeType(&library);

    if (face_id.filename.isEmpty())
        return;

    if (!face_id.filename.isEmpty()) {
        QFile file(QString::fromUtf8(face_id.filename));
        if (face_id.filename.startsWith(":qmemoryfonts/")) {
            // from qfontdatabase.cpp
            extern QByteArray qt_fontdata_from_index(int);
            QByteArray idx = face_id.filename;
            idx.remove(0, 14); // remove ':qmemoryfonts/'
            bool ok = false;
            fontData = qt_fontdata_from_index(idx.toInt(&ok));
            if (!ok)
                fontData = QByteArray();
        } else if (!(file.fileEngine()->fileFlags(QAbstractFileEngine::FlagsMask) & QAbstractFileEngine::LocalDiskFlag)) {
            if (!file.open(QIODevice::ReadOnly)) {
                return;
            }
            fontData = file.readAll();
        }
    }
    if (!fontData.isEmpty()) {
        if (FT_New_Memory_Face(library, (const FT_Byte *)fontData.constData(), fontData.size(), face_id.index, &face)) {
            return;
        }
    } else if (FT_New_Face(library, face_id.filename, face_id.index, &face)) {
        return;
    }

    for (int i = 0; i < face->num_charmaps; ++i) {
        FT_CharMap cm = face->charmaps[i];
        switch(cm->encoding) {
            case FT_ENCODING_UNICODE: {
                unicode_map = cm;
                break;
            }
            case FT_ENCODING_APPLE_ROMAN:
            case FT_ENCODING_ADOBE_LATIN_1: {
                if (!unicode_map || unicode_map->encoding != FT_ENCODING_UNICODE)
                    unicode_map = cm;
                break;
            }
            case FT_ENCODING_ADOBE_CUSTOM:
            case FT_ENCODING_MS_SYMBOL: {
                if (!symbol_map)
                    symbol_map = cm;
                break;
            }
            default: {
                break;
            }
        }
    }

    if (!FT_IS_SCALABLE(face) && face->num_fixed_sizes == 1) {
        FT_Set_Char_Size (face, face->available_sizes[0].x_ppem,
            face->available_sizes[0].y_ppem, 0, 0);
    }

    FT_Set_Charmap(face, unicode_map);
}

QFreetypeFace::~QFreetypeFace()
{
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

int QFreetypeFace::fsType() const
{
    int fsType = 0;
    TT_OS2 *os2 = (TT_OS2 *)FT_Get_Sfnt_Table(face, ft_sfnt_os2);
    if (os2)
        fsType = os2->fsType;
    return fsType;
}

HB_Error QFreetypeFace::getPointInOutline(HB_Glyph glyph, int flags, hb_uint32 point, HB_Fixed *xpos, HB_Fixed *ypos, hb_uint32 *nPoints)
{
    if (HB_Error error = (HB_Error)FT_Load_Glyph(face, glyph, flags))
        return error;

    if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
        return HB_Err_Invalid_SubTable;

    *nPoints = face->glyph->outline.n_points;
    if (!(*nPoints))
        return HB_Err_Ok;

    if (point > *nPoints)
        return HB_Err_Invalid_SubTable;

    *xpos = face->glyph->outline.points[point].x;
    *ypos = face->glyph->outline.points[point].y;

    return HB_Err_Ok;
}

void QFreetypeFace::computeSize(const QFontDef &fontDef, int *xsize, int *ysize, bool *outline_drawing)
{
    *ysize = qRound(fontDef.pixelSize * 64);
    *xsize = *ysize * fontDef.stretch / 100;
    *outline_drawing = false;

    /*
     * Bitmap only faces must match exactly, so find the closest
     * one (height dominant search)
     */
    if (!(face->face_flags & FT_FACE_FLAG_SCALABLE)) {
        int best = 0;
        for (int i = 1; i < face->num_fixed_sizes; i++) {
            if (qAbs(*ysize -  face->available_sizes[i].y_ppem) <
                qAbs (*ysize - face->available_sizes[best].y_ppem) ||
                (qAbs (*ysize - face->available_sizes[i].y_ppem) ==
                 qAbs (*ysize - face->available_sizes[best].y_ppem) &&
                 qAbs (*xsize - face->available_sizes[i].x_ppem) <
                 qAbs (*xsize - face->available_sizes[best].x_ppem))) {
                best = i;
            }
        }
        if (FT_Set_Char_Size (face, face->available_sizes[best].x_ppem, face->available_sizes[best].y_ppem, 0, 0) == 0) {
            *xsize = face->available_sizes[best].x_ppem;
            *ysize = face->available_sizes[best].y_ppem;
        } else {
            int err = 1;
            if (!(face->face_flags & FT_FACE_FLAG_SCALABLE) && ysize == 0 && face->num_fixed_sizes >= 1) {
                // work around FT 2.1.10 problem with BDF without PIXEL_SIZE property
                err = FT_Set_Pixel_Sizes(face, face->available_sizes[0].width, face->available_sizes[0].height);
                if (err && face->num_fixed_sizes == 1)
                    err = 0; //even more of a workaround...
            }

            if (err)
                *xsize = *ysize = 0;
        }
    } else {
        *outline_drawing = (*xsize > (QT_MAX_CACHED_GLYPH_SIZE<<6) || *ysize > (QT_MAX_CACHED_GLYPH_SIZE<<6));
    }
}

QFontEngine::Properties QFreetypeFace::properties() const
{
    QFontEngine::Properties p;
    p.postscriptName = FT_Get_Postscript_Name(face);
    PS_FontInfoRec font_info;
    if (FT_Get_PS_Font_Info(face, &font_info) == 0)
        p.copyright = font_info.notice;
    if (FT_IS_SCALABLE(face)) {
        p.ascent = face->ascender;
        p.descent = -face->descender;
        p.leading = face->height - face->ascender + face->descender;
        p.emSquare = face->units_per_EM;
        p.boundingBox = QRectF(face->bbox.xMin, -face->bbox.yMax,
                               face->bbox.xMax - face->bbox.xMin,
                               face->bbox.yMax - face->bbox.yMin);
    } else {
        p.ascent = QFixed::fromFixed(face->size->metrics.ascender);
        p.descent = QFixed::fromFixed(-face->size->metrics.descender);
        p.leading = QFixed::fromFixed(face->size->metrics.height - face->size->metrics.ascender + face->size->metrics.descender);
        p.emSquare = face->size->metrics.y_ppem;
//        p.boundingBox = QRectF(-p.ascent.toReal(), 0, (p.ascent + p.descent).toReal(), face->size->metrics.max_advance/64.);
        p.boundingBox = QRectF(0, -p.ascent.toReal(),
                               face->size->metrics.max_advance/64, (p.ascent + p.descent).toReal() );
    }
    p.italicAngle = 0;
    p.capHeight = p.ascent;
    p.lineWidth = face->underline_thickness;
    return p;
}

/* Some fonts (such as MingLiu rely on hinting to scale different
   components to their correct sizes. While this is really broken (it
   should be done in the component glyph itself, not the hinter) we
   will have to live with it.

   This means we can not use FT_LOAD_NO_HINTING to get the glyph
   outline. All we can do is to load the unscaled glyph and scale it
   down manually when required.
*/
static void scaleOutline(FT_Face face, FT_GlyphSlot g, FT_Fixed x_scale, FT_Fixed y_scale)
{
    x_scale = FT_MulDiv(x_scale, 1 << 10, face->units_per_EM);
    y_scale = FT_MulDiv(y_scale, 1 << 10, face->units_per_EM);
    FT_Vector *p = g->outline.points;
    const FT_Vector *e = p + g->outline.n_points;
    while (p < e) {
        p->x = FT_MulFix(p->x, x_scale);
        p->y = FT_MulFix(p->y, y_scale);
        ++p;
    }
}

void QFreetypeFace::addGlyphToPath(FT_Face face, FT_GlyphSlot g, const QFixedPoint &point, QPainterPath *path, FT_Fixed x_scale, FT_Fixed y_scale)
{
    const qreal factor = 1/64.;
    scaleOutline(face, g, x_scale, y_scale);

    QPointF cp = point.toPointF();

    // convert the outline to a painter path
    int i = 0;
    for (int j = 0; j < g->outline.n_contours; ++j) {
        int last_point = g->outline.contours[j];
        QPointF start = cp + QPointF(g->outline.points[i].x*factor, -g->outline.points[i].y*factor);
        if(!(g->outline.tags[i] & 1)) {
            start += cp + QPointF(g->outline.points[last_point].x*factor, -g->outline.points[last_point].y*factor);
            start /= 2;
        }
//         qDebug("contour: %d -- %d", i, g->outline.contours[j]);
//         qDebug("first point at %f %f", start.x(), start.y());
        path->moveTo(start);

        QPointF c[4];
        c[0] = start;
        int n = 1;
        while (i < last_point) {
            ++i;
            c[n] = cp + QPointF(g->outline.points[i].x*factor, -g->outline.points[i].y*factor);
//             qDebug() << "    i=" << i << " flag=" << (int)g->outline.tags[i] << "point=" << c[n];
            ++n;
            switch (g->outline.tags[i] & 3) {
            case 2:
                // cubic bezier element
                if (n < 4)
                    continue;
                c[3] = (c[3] + c[2])/2;
                --i;
                break;
            case 0:
                // quadratic bezier element
                if (n < 3)
                    continue;
                c[3] = (c[1] + c[2])/2;
                c[2] = (2*c[1] + c[3])/3;
                c[1] = (2*c[1] + c[0])/3;
                --i;
                break;
            case 1:
            case 3:
                if (n == 2) {
//                     qDebug() << "lineTo" << c[1];
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
//             qDebug() << "cubicTo" << c[1] << c[2] << c[3];
            path->cubicTo(c[1], c[2], c[3]);
            c[0] = c[3];
            n = 1;
        }
        if (n == 1) {
//             qDebug() << "closeSubpath";
            path->closeSubpath();
        } else {
            c[3] = start;
            if (n == 2) {
                c[2] = (2*c[1] + c[3])/3;
                c[1] = (2*c[1] + c[0])/3;
            }
//             qDebug() << "cubicTo" << c[1] << c[2] << c[3];
            path->cubicTo(c[1], c[2], c[3]);
        }
        ++i;
    }
}

extern void qt_addBitmapToPath(qreal x0, qreal y0, const uchar *image_data, int bpl, int w, int h, QPainterPath *path);

void QFreetypeFace::addBitmapToPath(FT_GlyphSlot slot, const QFixedPoint &point, QPainterPath *path)
{
    if (slot->format != FT_GLYPH_FORMAT_BITMAP
        || slot->bitmap.pixel_mode != FT_PIXEL_MODE_MONO)
        return;

    QPointF cp = point.toPointF();
    qt_addBitmapToPath(cp.x() + TRUNC(slot->metrics.horiBearingX), cp.y() - TRUNC(slot->metrics.horiBearingY),
                       slot->bitmap.buffer, slot->bitmap.pitch, slot->bitmap.width, slot->bitmap.rows, path);
}

QFontEngineFT::Glyph::~Glyph()
{
    delete [] data;
}

static const uint subpixel_filter[3][3] = {
    { 180, 60, 16 },
    { 38, 180, 38 },
    { 16, 60, 180 }
};

static inline uint filterPixel(uint red, uint green, uint blue, bool legacyFilter)
{
    uint res;
    if (legacyFilter) {
        uint high = (red*subpixel_filter[0][0] + green*subpixel_filter[0][1] + blue*subpixel_filter[0][2]) >> 8;
        uint mid = (red*subpixel_filter[1][0] + green*subpixel_filter[1][1] + blue*subpixel_filter[1][2]) >> 8;
        uint low = (red*subpixel_filter[2][0] + green*subpixel_filter[2][1] + blue*subpixel_filter[2][2]) >> 8;
        res = (mid << 24) + (high << 16) + (mid << 8) + low;
    } else {
        uint alpha = green;
        res = (alpha << 24) + (red << 16) + (green << 8) + blue;
    }
    return res;
}

static void convertRGBToARGB(const uchar *src, uint *dst, int width, int height, int src_pitch, bool bgr, bool legacyFilter)
{
    int h = height;
    const int offs = bgr ? -1 : 1;
    const int w = width * 3;
    while (h--) {
        uint *dd = dst;
        for (int x = 0; x < w; x += 3) {
            uint red = src[x+1-offs];
            uint green = src[x+1];
            uint blue = src[x+1+offs];
            *dd = filterPixel(red, green, blue, legacyFilter);
            ++dd;
        }
        dst += width;
        src += src_pitch;
    }
}

static void convertRGBToARGB_V(const uchar *src, uint *dst, int width, int height, int src_pitch, bool bgr, bool legacyFilter)
{
    int h = height;
    const int offs = bgr ? -src_pitch : src_pitch;
    while (h--) {
        for (int x = 0; x < width; x++) {
            uint red = src[x+src_pitch-offs];
            uint green = src[x+src_pitch];
            uint blue = src[x+src_pitch+offs];
            dst[x] = filterPixel(red, green, blue, legacyFilter);
        }
        dst += width;
        src += 3*src_pitch;
    }
}

QFontEngineFT::QFontEngineFT(const QFontDef &fd)
{
    fontDef = fd;
    matrix.xx = 0x10000;
    matrix.yy = 0x10000;
    matrix.xy = 0;
    matrix.yx = 0;
    cache_cost = 100;
    kerning_pairs_loaded = false;
    embolden = false;
    freetype = 0;
    default_load_flags = FT_LOAD_IGNORE_GLOBAL_ADVANCE_WIDTH;
    default_hint_style = HintNone;
    subpixelType = Subpixel_None;
#if defined(FT_LCD_FILTER_H)
    lcdFilterType = (int)((quintptr) FT_LCD_FILTER_DEFAULT);
    antialias = true;
    defaultFormat = Format_A32;
#else
    antialias = false;
    defaultFormat = Format_Mono;
    lcdFilterType = 0;
#endif
    embeddedbitmap = false;
}

QFontEngineFT::~QFontEngineFT()
{
    delete freetype;
}

bool QFontEngineFT::init(FaceId faceId, bool antialias, GlyphFormat format)
{
    freetype = new QFreetypeFace(faceId);
    if (!freetype->face) {
        xsize = 0;
        ysize = 0;
        return false;
    }
#if defined(FT_LCD_FILTER_H)
    defaultFormat = format;
    this->antialias = antialias;
#endif

    face_id = faceId;

    symbol = freetype->symbol_map != 0;
    PS_FontInfoRec psrec;
    // don't assume that type1 fonts are symbol fonts by default
    if (FT_Get_PS_Font_Info(freetype->face, &psrec) == FT_Err_Ok) {
        symbol = bool(fontDef.family.contains(QLatin1String("symbol"), Qt::CaseInsensitive));
    }

    lbearing = rbearing = SHRT_MIN;
    freetype->computeSize(fontDef, &xsize, &ysize, &defaultGlyphSet.outline_drawing);

    FT_Face face = getFace();

    if (FT_IS_SCALABLE(face)) {
        bool fake_oblique = (fontDef.style != QFont::StyleNormal) && !(face->style_flags & FT_STYLE_FLAG_ITALIC);
        if (fake_oblique)
            matrix.xy = 0x10000*3/10;
        FT_Set_Transform(face, &matrix, 0);
        freetype->matrix = matrix;
        // fake bold
        if ((fontDef.weight == QFont::Bold) && !(face->style_flags & FT_STYLE_FLAG_BOLD) && !FT_IS_FIXED_WIDTH(face))
            embolden = true;
        // underline metrics
        line_thickness =  QFixed::fromFixed(FT_MulFix(face->underline_thickness, face->size->metrics.y_scale));
        underline_position = QFixed::fromFixed(-FT_MulFix(face->underline_position, face->size->metrics.y_scale));
    } else {
        // ad hoc algorithm
        int score = fontDef.weight * fontDef.pixelSize;
        line_thickness = score / 700;
        // looks better with thicker line for small pointsizes
        if (line_thickness < 2 && score >= 1050)
            line_thickness = 2;
        underline_position =  ((line_thickness * 2) + 3) / 6;
    }
    if (line_thickness < 1)
        line_thickness = 1;

    hbFont.x_ppem  = face->size->metrics.x_ppem;
    hbFont.y_ppem  = face->size->metrics.y_ppem;
    hbFont.x_scale = face->size->metrics.x_scale;
    hbFont.y_scale = face->size->metrics.y_scale;

    metrics = face->size->metrics;

    fontDef.styleName = QString::fromUtf8(face->style_name);

    fsType = freetype->fsType();

    return true;
}

int QFontEngineFT::loadFlags(QGlyphSet *set, GlyphFormat format, int flags,
                             bool &hsubpixel, int &vfactor) const
{
    int load_flags = FT_LOAD_DEFAULT | default_load_flags;
    int load_target = default_hint_style == HintLight
                      ? FT_LOAD_TARGET_LIGHT
                      : FT_LOAD_TARGET_NORMAL;

    if (format == Format_None) {
        format = defaultFormat;
    }

    if (format == Format_Mono) {
        load_target = FT_LOAD_TARGET_MONO;
    } else if (format == Format_A32) {
        if (subpixelType == QFontEngineFT::Subpixel_RGB || subpixelType == QFontEngineFT::Subpixel_BGR) {
            if (default_hint_style == HintFull)
                load_target = FT_LOAD_TARGET_LCD;
            hsubpixel = true;
        } else if (subpixelType == QFontEngineFT::Subpixel_VRGB || subpixelType == QFontEngineFT::Subpixel_VBGR) {
            if (default_hint_style == HintFull)
                load_target = FT_LOAD_TARGET_LCD_V;
            vfactor = 3;
        }
    }

    if (set && set->outline_drawing)
        load_flags = FT_LOAD_NO_BITMAP;

    if (default_hint_style == HintNone || (flags & HB_ShaperFlag_UseDesignMetrics) || (set && set->outline_drawing))
        load_flags |= FT_LOAD_NO_HINTING;
    else
        load_flags |= load_target;

    return load_flags;
}

QFontEngineFT::Glyph *QFontEngineFT::loadGlyph(glyph_t glyph,
                                               GlyphFormat format,
                                               bool fetchMetricsOnly) const
{
    if (format == Format_None) {
        format = defaultFormat;
    }

    Glyph *g = defaultGlyphSet.getGlyph(glyph);
    if (g && g->format == format) {
        return g;
    }

    Q_ASSERT(format != Format_None);
    bool hsubpixel = false;
    int vfactor = 1;
    int load_flags = loadFlags(&defaultGlyphSet, format, 0, hsubpixel, vfactor);

    if (format != Format_Mono && !embeddedbitmap)
        load_flags |= FT_LOAD_NO_BITMAP;

    FT_Matrix matrix = freetype->matrix;
    bool transform = matrix.xx != 0x10000
                     || matrix.yy != 0x10000
                     || matrix.xy != 0
                     || matrix.yx != 0;

    if (transform)
        load_flags |= FT_LOAD_NO_BITMAP;

    FT_Face face = freetype->face;

    FT_Set_Transform(face, &freetype->matrix, 0);

    FT_Error err = FT_Load_Glyph(face, glyph, load_flags);
    if (err && (load_flags & FT_LOAD_NO_BITMAP)) {
        load_flags &= ~FT_LOAD_NO_BITMAP;
        err = FT_Load_Glyph(face, glyph, load_flags);
    }
    if (err == FT_Err_Too_Few_Arguments) {
        // this is an error in the bytecode interpreter, just try to run without it
        load_flags |= FT_LOAD_FORCE_AUTOHINT;
        err = FT_Load_Glyph(face, glyph, load_flags);
    }
    if (err != FT_Err_Ok)
        qWarning("load glyph failed err=%x face=%p, glyph=%d", err, face, glyph);

    if (defaultGlyphSet.outline_drawing && fetchMetricsOnly)
        return 0;

    FT_GlyphSlot slot = face->glyph;
    if (embolden) FT_GlyphSlot_Embolden(slot);
    FT_Library library = freetype->library;

    FT_Render_Mode rendermode = FT_RENDER_MODE_MONO;
#if defined(FT_LCD_FILTER_H)
    if (antialias || hsubpixel) {
        rendermode = FT_RENDER_MODE_LCD;
    } else if (vfactor != 1) {
        rendermode = FT_RENDER_MODE_LCD_V;
    }

    if (slot->format == FT_GLYPH_FORMAT_OUTLINE && (hsubpixel || vfactor != 1)) {
        err = FT_Library_SetLcdFilter(library, (FT_LcdFilter)lcdFilterType);
    }
    if (err != FT_Err_Ok) {
        qWarning("setting render filter failed err=%x face=%p, glyph=%d", err, face, glyph);
    }
#endif

    err = FT_Render_Glyph(slot, rendermode);
    if (err != FT_Err_Ok) {
        qWarning("render glyph failed err=%x face=%p, glyph=%d", err, face, glyph);
    }

#if defined(FT_LCD_FILTER_H)
    FT_Library_SetLcdFilter(library, FT_LCD_FILTER_NONE);
#endif

    const unsigned short gheight = slot->bitmap.rows / vfactor;
    const unsigned short gwidth = ((rendermode == FT_RENDER_MODE_MONO) ? slot->bitmap.width : (slot->bitmap.width / 3));

    const int glyph_buffer_size = (rendermode == FT_RENDER_MODE_MONO ? (gwidth * gheight) : (gwidth * gheight * 4));
    if (!glyph_buffer_size) {
        // e.g. 3 or 2798
        // qWarning("glyph size is zero face=%p, glyph=%d", face, glyph);
        return 0;
    }
    Q_ASSERT(glyph_buffer_size >= 1);
    uchar *glyph_buffer = new uchar[glyph_buffer_size];

    bool useLegacyLcdFilter = false;
#if defined(FT_LCD_FILTER_H)
    useLegacyLcdFilter = (lcdFilterType == FT_LCD_FILTER_LEGACY);
#endif

#if defined(FT_LCD_FILTER_H)
    if (rendermode == FT_RENDER_MODE_LCD) {
        Q_ASSERT(slot->bitmap.pixel_mode == FT_PIXEL_MODE_LCD);
        convertRGBToARGB(slot->bitmap.buffer, (uint *)glyph_buffer, gwidth, gheight, slot->bitmap.pitch, subpixelType != QFontEngineFT::Subpixel_RGB, useLegacyLcdFilter);
    } else if (rendermode == FT_RENDER_MODE_LCD_V) {
        Q_ASSERT(slot->bitmap.pixel_mode == FT_PIXEL_MODE_LCD_V);
        convertRGBToARGB_V(slot->bitmap.buffer, (uint *)glyph_buffer, gwidth, gheight, slot->bitmap.pitch, subpixelType != QFontEngineFT::Subpixel_VRGB, useLegacyLcdFilter);
    } else
#endif
    {
        Q_ASSERT(slot->bitmap.pixel_mode == FT_PIXEL_MODE_MONO);
        const int bytes = ((((gwidth + 7) & ~7) >> 3) * slot->bitmap.rows);
        ::memcpy(glyph_buffer, slot->bitmap.buffer, bytes);
    }

    if (!g) {
        g = new Glyph;
        g->data = 0;
    }

    g->linearAdvance = slot->linearHoriAdvance >> 10;
    g->width = gwidth;
    g->height = gheight;
    g->x = slot->bitmap_left;
    g->y = slot->bitmap_top;
    g->advance = TRUNC(ROUND(slot->advance.x));
    g->format = format;
    delete [] g->data;
    g->data = glyph_buffer;

    defaultGlyphSet.setGlyph(glyph, g);

    return g;
}

QFontEngine::FaceId QFontEngineFT::faceId() const
{
    return face_id;
}

QFontEngine::Properties QFontEngineFT::properties() const
{
    Properties p = freetype->properties();
    if (p.postscriptName.isEmpty()) {
        p.postscriptName = QFontEngine::convertToPostscriptFontFamilyName(fontDef.family.toUtf8());
    }

    return freetype->properties();
}

QFixed QFontEngineFT::emSquareSize() const
{
    if (FT_IS_SCALABLE(freetype->face)) {
        return freetype->face->units_per_EM;
    }
    return freetype->face->size->metrics.y_ppem;
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
    int s = 0;
    if ((fontDef.style != QFont::StyleNormal) && !(freetype->face->style_flags & FT_STYLE_FLAG_ITALIC))
        s = SynthesizedItalic;
    if ((fontDef.weight == QFont::Bold) && !(freetype->face->style_flags & FT_STYLE_FLAG_BOLD))
        s |= SynthesizedBold;
    if (fontDef.stretch != 100 && FT_IS_SCALABLE(freetype->face))
        s |= SynthesizedStretch;
    return s;
}

QFixed QFontEngineFT::ascent() const
{
    return QFixed::fromFixed(metrics.ascender);
}

QFixed QFontEngineFT::descent() const
{
    // subtract a pixel to work around QFontMetrics's built-in + 1
    return QFixed::fromFixed(-metrics.descender - 64);
}

QFixed QFontEngineFT::leading() const
{
    return QFixed::fromFixed(metrics.height - metrics.ascender + metrics.descender);
}

QFixed QFontEngineFT::xHeight() const
{
    TT_OS2 *os2 = (TT_OS2 *)FT_Get_Sfnt_Table(freetype->face, ft_sfnt_os2);
    if (os2 && os2->sxHeight) {
        getFace();
        return QFixed(os2->sxHeight*freetype->face->size->metrics.y_ppem)/freetype->face->units_per_EM;
    }
    return QFontEngine::xHeight();
}

QFixed QFontEngineFT::averageCharWidth() const
{
    TT_OS2 *os2 = (TT_OS2 *)FT_Get_Sfnt_Table(freetype->face, ft_sfnt_os2);
    if (os2 && os2->xAvgCharWidth) {
        getFace();
        return QFixed(os2->xAvgCharWidth*freetype->face->size->metrics.x_ppem)/freetype->face->units_per_EM;
    }
    return QFontEngine::averageCharWidth();
}

qreal QFontEngineFT::maxCharWidth() const
{
    return metrics.max_advance >> 6;
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
        getFace();
        if (freetype->face->size->metrics.x_ppem != 0) {
            QFixed scalingFactor(freetype->face->units_per_EM/freetype->face->size->metrics.x_ppem);
            loadKerningPairs(scalingFactor);
        }
    }
    QFontEngine::doKerning(g, flags);
}

void QFontEngineFT::getUnscaledGlyph(glyph_t glyph, QPainterPath *path, glyph_metrics_t *metrics)
{
    FT_Face face = getFace(Unscaled);
    FT_Set_Transform(face, 0, 0);
    FT_Load_Glyph(face, glyph, FT_LOAD_NO_BITMAP);

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

    if (!FT_IS_SCALABLE(freetype->face))
        QFreetypeFace::addBitmapToPath(face->glyph, p, path);
    else
        QFreetypeFace::addGlyphToPath(face, face->glyph, p, path, face->units_per_EM << 6, face->units_per_EM << 6);

    FT_Set_Transform(face, &freetype->matrix, 0);
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
    for ( int i = 0; i < len; i++ ) {
        unsigned int uc = getChar(string, i, len);
        if (!FT_Get_Char_Index(face, uc))
            return false;
    }
    return true;
}

void QFontEngineFT::addOutlineToPath(qreal x, qreal y, const QGlyphLayout &glyphs, QPainterPath *path, QTextItem::RenderFlags flags)
{
    if (!glyphs.numGlyphs)
        return;

    if (FT_IS_SCALABLE(freetype->face)) {
        QFontEngine::addOutlineToPath(x, y, glyphs, path, flags);
    } else {
        QVarLengthArray<QFixedPoint> positions;
        QVarLengthArray<glyph_t> positioned_glyphs;
        const QTransform matrix = QTransform::fromTranslate(x, y);
        getGlyphPositions(glyphs, matrix, flags, positioned_glyphs, positions);

        FT_Face face = getFace(Unscaled);
        for (int gl = 0; gl < glyphs.numGlyphs; gl++) {
            FT_UInt glyph = positioned_glyphs[gl];
            FT_Load_Glyph(face, glyph, FT_LOAD_TARGET_MONO);
            freetype->addBitmapToPath(face->glyph, positions[gl], path);
        }
    }
}

void QFontEngineFT::addGlyphsToPath(glyph_t *glyphs, QFixedPoint *positions, int numGlyphs,
                                    QPainterPath *path, QTextItem::RenderFlags)
{
    FT_Face face = getFace(Unscaled);

    for (int gl = 0; gl < numGlyphs; gl++) {
        FT_UInt glyph = glyphs[gl];

        FT_Load_Glyph(face, glyph, FT_LOAD_NO_BITMAP);

        FT_GlyphSlot g = face->glyph;
        if (g->format != FT_GLYPH_FORMAT_OUTLINE)
            continue;
        QFreetypeFace::addGlyphToPath(face, g, positions[gl], path, xsize, ysize);
    }
}

bool QFontEngineFT::stringToCMap(const QChar *str, int len, QGlyphLayout *glyphs, int *nglyphs,
                                 QTextEngine::ShaperFlags flags) const
{
    if (*nglyphs < len) {
        *nglyphs = len;
        return false;
    }

    bool mirrored = flags & QTextEngine::RightToLeft;
    int glyph_pos = 0;
    if (freetype->symbol_map) {
        FT_Face face = freetype->face;
        for ( int i = 0; i < len; ++i ) {
            unsigned int uc = getChar(str, i, len);
            FT_Set_Charmap(face, freetype->symbol_map);
            glyph_t glyph = FT_Get_Char_Index(face, uc);
            FT_Set_Charmap(face, freetype->unicode_map);
            glyphs->glyphs[glyph_pos] = glyph;
            ++glyph_pos;
        }
    } else {
        FT_Face face = freetype->face;
        for (int i = 0; i < len; ++i) {
            unsigned int uc = getChar(str, i, len);
            if (mirrored) {
                uc = QChar::mirroredChar(uc);
            }
            glyph_t glyph = FT_Get_Char_Index(face, uc);
            glyphs->glyphs[glyph_pos] = glyph;
            ++glyph_pos;
        }
    }

    *nglyphs = glyph_pos;
    glyphs->numGlyphs = glyph_pos;

    if (flags & QTextEngine::GlyphIndicesOnly)
        return true;

    recalcAdvances(glyphs, flags);

    return true;
}

void QFontEngineFT::recalcAdvances(QGlyphLayout *glyphs, QTextEngine::ShaperFlags flags) const
{
    FT_Face face = getFace();
    bool design = (default_hint_style == HintNone ||
                   default_hint_style == HintLight ||
                   (flags & HB_ShaperFlag_UseDesignMetrics)) && FT_IS_SCALABLE(face);
    for (int i = 0; i < glyphs->numGlyphs; i++) {
        Glyph *g = loadGlyph(glyphs->glyphs[i], Format_None, true);
        if (g) {
            glyphs->advances_x[i] = design ? QFixed::fromFixed(g->linearAdvance) : QFixed(g->advance);
        } else {
            glyphs->advances_x[i] = design ? QFixed::fromFixed(face->glyph->linearHoriAdvance >> 10)
                                           : QFixed::fromFixed(face->glyph->metrics.horiAdvance).round();
        }
        if (fontDef.styleStrategy & QFont::ForceIntegerMetrics)
            glyphs->advances_x[i] = glyphs->advances_x[i].round();
        glyphs->advances_y[i] = 0;
    }
}

glyph_metrics_t QFontEngineFT::boundingBox(const QGlyphLayout &glyphs) const
{

    FT_Face face = getFace();

    glyph_metrics_t overall;
    // initialize with line height, we get the same behaviour on all platforms
    overall.y = -ascent();
    overall.height = ascent() + descent() + 1;

    QFixed ymax = 0;
    QFixed xmax = 0;
    for (int i = 0; i < glyphs.numGlyphs; i++) {
        Glyph *g = loadGlyph(glyphs.glyphs[i], Format_None, true);
        if (g) {
            QFixed x = overall.xoff + glyphs.offsets[i].x + g->x;
            QFixed y = overall.yoff + glyphs.offsets[i].y - g->y;
            overall.x = qMin(overall.x, x);
            overall.y = qMin(overall.y, y);
            xmax = qMax(xmax, x + g->width);
            ymax = qMax(ymax, y + g->height);
            overall.xoff += qRound(g->advance);
        } else {
            int left  = FLOOR(face->glyph->metrics.horiBearingX);
            int right = CEIL(face->glyph->metrics.horiBearingX + face->glyph->metrics.width);
            int top    = CEIL(face->glyph->metrics.horiBearingY);
            int bottom = FLOOR(face->glyph->metrics.horiBearingY - face->glyph->metrics.height);

            QFixed x = overall.xoff + glyphs.offsets[i].x - (-TRUNC(left));
            QFixed y = overall.yoff + glyphs.offsets[i].y - TRUNC(top);
            overall.x = qMin(overall.x, x);
            overall.y = qMin(overall.y, y);
            xmax = qMax(xmax, x + TRUNC(right - left));
            ymax = qMax(ymax, y + TRUNC(top - bottom));
            overall.xoff += qRound(TRUNC(ROUND(face->glyph->advance.x)));
        }
    }
    overall.height = qMax(overall.height, ymax - overall.y);
    overall.width = xmax - overall.x;

    return overall;
}

glyph_metrics_t QFontEngineFT::boundingBox(glyph_t glyph) const
{
    FT_Face face = getFace();
    glyph_metrics_t overall;
    Glyph *g = loadGlyph(glyph, Format_None, true);
    if (g) {
        overall.x = g->x;
        overall.y = -g->y;
        overall.width = g->width;
        overall.height = g->height;
        overall.xoff = g->advance;
        if (fontDef.styleStrategy & QFont::ForceIntegerMetrics)
            overall.xoff = overall.xoff.round();
    } else {
        int left  = FLOOR(face->glyph->metrics.horiBearingX);
        int right = CEIL(face->glyph->metrics.horiBearingX + face->glyph->metrics.width);
        int top    = CEIL(face->glyph->metrics.horiBearingY);
        int bottom = FLOOR(face->glyph->metrics.horiBearingY - face->glyph->metrics.height);

        overall.width = TRUNC(right-left);
        overall.height = TRUNC(top-bottom);
        overall.x = TRUNC(left);
        overall.y = -TRUNC(top);
        overall.xoff = TRUNC(ROUND(face->glyph->advance.x));
    }
    return overall;
}

QImage QFontEngineFT::alphaMapForGlyph(glyph_t g)
{
    getFace();

    Glyph *glyph = defaultGlyphSet.outline_drawing ? 0 : loadGlyph(g, Format_None);
    if (!glyph) {
        return QFontEngine::alphaMapForGlyph(g);
    }

    const int pitch = (antialias ? glyph->width * 4 : ((glyph->width + 31) & ~31) >> 3);

    QImage img(glyph->width, glyph->height, antialias ? QImage::Format_ARGB32 : QImage::Format_Mono);
    if (!antialias) {
        static const QVector<QRgb> colors = { qt_transparentrgba, qt_blackrgba };
        img.setColorTable(colors);
    }
    Q_ASSERT(img.bytesPerLine() == pitch);
    if (glyph->width) {
        for (int y = 0; y < glyph->height; ++y)
            memcpy(img.scanLine(y), &glyph->data[y * pitch], pitch);
    }

    return img;
}

int QFontEngineFT::glyphCount() const
{
    int count = 0;
    FT_Face face = getFace();
    if (face) {
        count = face->num_glyphs;
    }
    return count;
}

FT_Face QFontEngineFT::getFace(Scaling scale) const
{
    FT_Face face = freetype->face;
    if (scale == Unscaled) {
        FT_Set_Char_Size(face, face->units_per_EM << 6, face->units_per_EM << 6, 0, 0);
        freetype->xsize = face->units_per_EM << 6;
        freetype->ysize = face->units_per_EM << 6;
    } else if (freetype->xsize != xsize || freetype->ysize != ysize) {
        FT_Set_Char_Size(face, xsize, ysize, 0, 0);
        freetype->xsize = xsize;
        freetype->ysize = ysize;
    }
    if (freetype->matrix.xx != matrix.xx ||
        freetype->matrix.yy != matrix.yy ||
        freetype->matrix.xy != matrix.xy ||
        freetype->matrix.yx != matrix.yx) {
        freetype->matrix = matrix;
        FT_Set_Transform(face, &freetype->matrix, 0);
    }

    return face;
}


FT_Face QFontEngineFT::non_locked_face() const
{
    return freetype->face;
}


QFontEngineFT::QGlyphSet::QGlyphSet()
    : outline_drawing(false)
{
}

QFontEngineFT::QGlyphSet::~QGlyphSet()
{
    clear();
}

void QFontEngineFT::QGlyphSet::clear()
{
    qDeleteAll(glyph_data);
    glyph_data.clear();
}

void QFontEngineFT::QGlyphSet::setGlyph(glyph_t index, Glyph *glyph)
{
    glyph_data.insert(index, glyph);
}

HB_Error QFontEngineFT::getPointInOutline(HB_Glyph glyph, int flags, hb_uint32 point, HB_Fixed *xpos, HB_Fixed *ypos, hb_uint32 *nPoints)
{
    getFace();
    bool hsubpixel = true;
    int vfactor = 1;
    int load_flags = loadFlags(0, Format_None, flags, hsubpixel, vfactor);
    HB_Error result = freetype->getPointInOutline(glyph, load_flags, point, xpos, ypos, nPoints);
    return result;
}

QT_END_NAMESPACE
