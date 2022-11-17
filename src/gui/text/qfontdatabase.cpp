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

#include "qfontdatabase.h"
#include "qfontdatabase_p.h"
#include "qdebug.h"
#include "qalgorithms.h"
#include "qapplication.h"
#include "qunicodetables_p.h"
#include "qfontengine_p.h"
#include "qfontinfo.h"
#include "qfontengine_ft_p.h"
#include "qx11info_x11.h"

#ifndef QT_NO_FONTCONFIG
#  include <fontconfig/fcfreetype.h>
#endif

#include <stdlib.h>
#include <limits.h>

// #define QFONTDATABASE_DEBUG
#ifdef QFONTDATABASE_DEBUG
#  define FD_DEBUG qDebug
#else
#  define FD_DEBUG if (false) qDebug
#endif

// #define FONT_MATCH_DEBUG
#ifdef FONT_MATCH_DEBUG
#  define FM_DEBUG qDebug
#else
#  define FM_DEBUG if (false) qDebug
#endif

QT_BEGIN_NAMESPACE

static const QLatin1String normalfontstyle("Normal");
static const QLatin1String regularfontstyle("Regular");
static const QLatin1String bookfontstyle("Book");
static const QLatin1String romanfontstyle("Roman");

static inline bool isStyleMatch(const QString &style, const QString &otherstyle)
{
    Q_ASSERT(!style.isEmpty()) // no font style in the db should be empty
    const QString lowerstyle(style.toLower());
    const QString lowerother(otherstyle.toLower());
    if (lowerstyle == lowerother) {
        return true;
    }
    // compat, most likely going away with the next release
    if ((lowerstyle == QLatin1String("normal") && otherstyle == QLatin1String("regular"))
        || (lowerstyle == QLatin1String("regular") || otherstyle == QLatin1String("normal"))) {
        return true;
    }
    return false;
}


static double qt_pixelSize(double pointSize, int dpi)
{
    if (pointSize < 0)
        return -1.;
    if (dpi == 75) // the stupid 75 dpi setting on X11
        dpi = 72;
    return ((pointSize * dpi) / 72.0);
}

static inline double qt_pointSize(double pixelSize, int dpi)
{
    if (pixelSize < 0)
        return -1.;
    if (dpi == 75) // the stupid 75 dpi setting on X11
        dpi = 72;
    return (pixelSize * 72. / double(dpi));
}

#ifndef QT_NO_FONTCONFIG
static inline int getFCWeight(int fc_weight)
{
    if (fc_weight <= (FC_WEIGHT_LIGHT + FC_WEIGHT_MEDIUM) / 2)
        return QFont::Light;
    else if (fc_weight <= (FC_WEIGHT_MEDIUM + FC_WEIGHT_DEMIBOLD) / 2)
        return QFont::Normal;
    else if (fc_weight <= (FC_WEIGHT_DEMIBOLD + FC_WEIGHT_BOLD) / 2)
        return QFont::DemiBold;
    else if (fc_weight <= (FC_WEIGHT_BOLD + FC_WEIGHT_BLACK) / 2)
        return QFont::Bold;

    return QFont::Black;
}

static QFontDef qt_FcPatternToQFontDef(FcPattern *pattern, const QFontDef &request)
{
    QFontDef fontDef;

    fontDef.hintingPreference = request.hintingPreference;
#ifdef FC_HINT_STYLE
    if (fontDef.hintingPreference == QFont::PreferDefaultHinting) {
        int hint_style = 0;
        if (FcPatternGetInteger(pattern, FC_HINT_STYLE, 0, &hint_style) == FcResultNoMatch
            && qt_x11Data->fc_hint_style > -1) {
            hint_style = qt_x11Data->fc_hint_style;
        }

        switch (hint_style) {
            case FC_HINT_NONE: {
                fontDef.hintingPreference = QFont::PreferNoHinting;
                break;
            }
            case FC_HINT_SLIGHT: {
                fontDef.hintingPreference = QFont::PreferVerticalHinting;
                break;
            }
            // no enum for FC_HINT_MEDIUM
            default: {
                fontDef.hintingPreference = QFont::PreferFullHinting;
                break;
            }
        }
    }
#endif // FC_HINT_STYLE

    FcChar8 *value = 0;
    if (FcPatternGetString(pattern, FC_FAMILY, 0, &value) == FcResultMatch) {
        fontDef.family = QString::fromUtf8(reinterpret_cast<const char *>(value));
    }

    double dpi;
    if (FcPatternGetDouble(pattern, FC_DPI, 0, &dpi) != FcResultMatch) {
        dpi = QX11Info::appDpiY();
    }

    double size;
    if (FcPatternGetDouble(pattern, FC_PIXEL_SIZE, 0, &size) == FcResultMatch)
        fontDef.pixelSize = size;
    else
        fontDef.pixelSize = 12;

    fontDef.pointSize = qt_pointSize(fontDef.pixelSize, qRound(dpi));

    int weight;
    if (FcPatternGetInteger(pattern, FC_WEIGHT, 0, &weight) != FcResultMatch)
        weight = FC_WEIGHT_MEDIUM;
    fontDef.weight = getFCWeight(weight);

    int slant;
    if (FcPatternGetInteger(pattern, FC_SLANT, 0, &slant) != FcResultMatch)
        slant = FC_SLANT_ROMAN;
    fontDef.style = (slant == FC_SLANT_ITALIC)
                    ? QFont::StyleItalic
                    : ((slant == FC_SLANT_OBLIQUE)
                       ? QFont::StyleOblique
                       : QFont::StyleNormal);


    fontDef.stretch = request.stretch;
    fontDef.style = request.style;

    int spacing = FC_PROPORTIONAL;
    if (FcPatternGetInteger(pattern, FC_SPACING, 0, &spacing) == FcResultMatch) {
        fontDef.fixedPitch = (spacing >= FC_MONO);
        fontDef.ignorePitch = false;
    } else {
        fontDef.ignorePitch = true;
    }

    return fontDef;
}

// values obtained via genutf script
static const uint specialCharsTbl[] = {
    0x0, // Common
    0x1E900, // Adlam
    0x11700, // Ahom
    0x14400, // AnatolianHieroglyphs
    0x0600, // Arabic
    0x0531, // Armenian
    0x10B00, // Avestan
    0x1B00, // Balinese
    0xA6A0, // Bamum
    0x16AD0, // BassaVah
    0x1BC0, // Batak
    0x0980, // Bengali
    0x11C00, // Bhaiksuki
    0x02EA, // Bopomofo
    0x11000, // Brahmi
    0x2800, // Braille
    0x1A00, // Buginese
    0x1740, // Buhid
    0x1400, // CanadianAboriginal
    0x102A0, // Carian
    0x10530, // CaucasianAlbanian
    0x11100, // Chakma
    0xAA00, // Cham
    0x13A0, // Cherokee
    0x10FB0, // Chorasmian
    0x03E2, // Coptic
    0x12000, // Cuneiform
    0x10800, // Cypriot
    0x12F90, // CyproMinoan
    0x0400, // Cyrillic
    0x10400, // Deseret
    0x0900, // Devanagari
    0x11900, // DivesAkuru
    0x11800, // Dogra
    0x1BC00, // Duployan
    0x13000, // EgyptianHieroglyphs
    0x10500, // Elbasan
    0x10FE0, // Elymaic
    0x1200, // Ethiopic
    0x10A0, // Georgian
    0x2C00, // Glagolitic
    0x10330, // Gothic
    0x11300, // Grantha
    0x0370, // Greek
    0x0A81, // Gujarati
    0x11D60, // GunjalaGondi
    0x0A01, // Gurmukhi
    0x2E80, // Han
    0x1100, // Hangul
    0x10D00, // HanifiRohingya
    0x1720, // Hanunoo
    0x108E0, // Hatran
    0x0591, // Hebrew
    0x3041, // Hiragana
    0x10840, // ImperialAramaic
    0x0, // Inherited
    0x10B60, // InscriptionalPahlavi
    0x10B40, // InscriptionalParthian
    0xA980, // Javanese
    0x11080, // Kaithi
    0x0C80, // Kannada
    0x30A1, // Katakana
    0x11F00, // Kawi
    0xA900, // KayahLi
    0x10A00, // Kharoshthi
    0x16FE4, // KhitanSmallScript
    0x1780, // Khmer
    0x11200, // Khojki
    0x112B0, // Khudawadi
    0x0E81, // Lao
    0x0041, // Latin
    0x1C00, // Lepcha
    0x1900, // Limbu
    0x10600, // LinearA
    0x10000, // LinearB
    0xA4D0, // Lisu
    0x10280, // Lycian
    0x10920, // Lydian
    0x11150, // Mahajani
    0x11EE0, // Makasar
    0x0D00, // Malayalam
    0x0840, // Mandaic
    0x10AC0, // Manichaean
    0x11C70, // Marchen
    0x11D00, // MasaramGondi
    0x16E40, // Medefaidrin
    0xAAE0, // MeeteiMayek
    0x1E800, // MendeKikakui
    0x109A0, // MeroiticCursive
    0x10980, // MeroiticHieroglyphs
    0x16F00, // Miao
    0x11600, // Modi
    0x1800, // Mongolian
    0x16A40, // Mro
    0x11280, // Multani
    0x1000, // Myanmar
    0x10880, // Nabataean
    0x1E4D0, // NagMundari
    0x119A0, // Nandinagari
    0x1980, // NewTaiLue
    0x11400, // Newa
    0x07C0, // Nko
    0x16FE1, // Nushu
    0x1E100, // NyiakengPuachueHmong
    0x1680, // Ogham
    0x1C50, // OlChiki
    0x10C80, // OldHungarian
    0x10300, // OldItalic
    0x10A80, // OldNorthArabian
    0x10350, // OldPermic
    0x103A0, // OldPersian
    0x10F00, // OldSogdian
    0x10A60, // OldSouthArabian
    0x10C00, // OldTurkic
    0x10F70, // OldUyghur
    0x0B01, // Oriya
    0x104B0, // Osage
    0x10480, // Osmanya
    0x16B00, // PahawhHmong
    0x10860, // Palmyrene
    0x11AC0, // PauCinHau
    0xA840, // PhagsPa
    0x10900, // Phoenician
    0x10B80, // PsalterPahlavi
    0xA930, // Rejang
    0x16A0, // Runic
    0x0800, // Samaritan
    0xA880, // Saurashtra
    0x11180, // Sharada
    0x10450, // Shavian
    0x11580, // Siddham
    0x1D800, // SignWriting
    0x0D81, // Sinhala
    0x10F30, // Sogdian
    0x110D0, // SoraSompeng
    0x11A50, // Soyombo
    0x1B80, // Sundanese
    0xA800, // SylotiNagri
    0x0700, // Syriac
    0x1700, // Tagalog
    0x1760, // Tagbanwa
    0x1950, // TaiLe
    0x1A20, // TaiTham
    0xAA80, // TaiViet
    0x11680, // Takri
    0x0B82, // Tamil
    0x16A70, // Tangsa
    0x16FE0, // Tangut
    0x0C00, // Telugu
    0x0780, // Thaana
    0x0E01, // Thai
    0x0F00, // Tibetan
    0x2D30, // Tifinagh
    0x11480, // Tirhuta
    0x1E290, // Toto
    0x10380, // Ugaritic
    0xA500, // Vai
    0x10570, // Vithkuqi
    0x1E2C0, // Wancho
    0x118A0, // WarangCiti
    0x10E80, // Yezidi
    0xA000, // Yi
    0x11A00, // ZanabazarSquare
};
enum { SpecialCharCount = sizeof(specialCharsTbl) / sizeof(uint) };

// --------------------------------------------------------------------------------------
// font loader
// --------------------------------------------------------------------------------------
static FcPattern *getFcPattern(const QFontPrivate *fp, const QFontDef &request)
{
    if (!qt_x11Data->has_fontconfig)
        return nullptr;

    FcPattern *pattern = FcPatternCreate();
    if (Q_UNLIKELY(!pattern)) {
        return nullptr;
    }

    QString family, foundry;
    QFontDatabasePrivate::parseFontName(request.family, foundry, family);

    if (!family.isEmpty()) {
        QByteArray cs = family.toUtf8();
        FcPatternAddString(pattern, FC_FAMILY, (const FcChar8 *)cs.constData());
    }
    if (!foundry.isEmpty()) {
        QByteArray cs = foundry.toUtf8();
        FcPatternAddString(pattern, FC_FOUNDRY, (const FcChar8 *)cs.constData());
    }

    if (!request.ignorePitch) {
        int pitch_value = FC_PROPORTIONAL;
        if (request.fixedPitch)
            pitch_value = FC_MONO;
        FcPatternAddInteger(pattern, FC_SPACING, pitch_value);
    }

    FcPatternAddBool(pattern, FC_OUTLINE, true);
    FcPatternAddBool(pattern, FC_SCALABLE, true);

    const double size_value = qMax(qreal(1.), request.pixelSize);
    FcPatternAddDouble(pattern, FC_PIXEL_SIZE, size_value);

    if (!request.styleName.isEmpty()) {
        QByteArray cs = request.styleName.toUtf8();
        FcPatternAddString(pattern, FC_STYLE, (const FcChar8 *)cs.constData());
    } else {
        int weight_value = FC_WEIGHT_BLACK;
        if (request.weight == 0)
            weight_value = FC_WEIGHT_MEDIUM;
        else if (request.weight < (QFont::Light + QFont::Normal) / 2)
            weight_value = FC_WEIGHT_LIGHT;
        else if (request.weight < (QFont::Normal + QFont::DemiBold) / 2)
            weight_value = FC_WEIGHT_MEDIUM;
        else if (request.weight < (QFont::DemiBold + QFont::Bold) / 2)
            weight_value = FC_WEIGHT_DEMIBOLD;
        else if (request.weight < (QFont::Bold + QFont::Black) / 2)
            weight_value = FC_WEIGHT_BOLD;
        FcPatternAddInteger(pattern, FC_WEIGHT, weight_value);

        int slant_value = FC_SLANT_ROMAN;
        if (request.style == QFont::StyleItalic)
            slant_value = FC_SLANT_ITALIC;
        else if (request.style == QFont::StyleOblique)
            slant_value = FC_SLANT_OBLIQUE;
        FcPatternAddInteger(pattern, FC_SLANT, slant_value);

        int stretch = request.stretch;
        if (!stretch)
            stretch = QFont::Unstretched;
        FcPatternAddInteger(pattern, FC_WIDTH, stretch);
    }

    FcConfigSubstitute(0, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    // add the application instance family
    QString defaultFamily = QApplication::font().family();
    QByteArray cs = defaultFamily.toUtf8();
    FcPatternAddString(pattern, FC_FAMILY, (const FcChar8 *)cs.constData());

    // add QFont::lastResortFamily() to the list, for compatibility with
    // previous versions
    defaultFamily = QFont::lastResortFamily();
    cs = defaultFamily.toUtf8();
    FcPatternAddString(pattern, FC_FAMILY, (const FcChar8 *)cs.constData());

    return pattern;
}

static QFontEngine *tryPatternLoad(FcPattern *match,
                                   const QFontDef &request,
                                   QUnicodeTables::Script script)
{
#ifdef FONT_MATCH_DEBUG
    FcChar8 *fam;
    FcPatternGetString(match, FC_FAMILY, 0, &fam);
    FM_DEBUG("==== trying %s\n", fam);
#endif

    if (!match) {
        // probably no fonts available
        return nullptr;
    }

    if (script != QUnicodeTables::Common) {
        // need to check the charset, as the langset doesn't work for some scripts
        if (specialCharsTbl[script]) {
            FcCharSet *cs = nullptr;
            if (FcPatternGetCharSet(match, FC_CHARSET, 0, &cs) != FcResultMatch) {
                return nullptr;
            }
            if (!FcCharSetHasChar(cs, specialCharsTbl[script])) {
                return nullptr;
            }
        }
    }

    FM_DEBUG("passes charset test\n");
    QFontEngineFT *engine = new QFontEngineFT(qt_FcPatternToQFontDef(match, request), match);
    if (engine->invalid()) {
        FM_DEBUG("   --> invalid!\n");
        delete engine;
        engine = nullptr;
    }
    return engine;
}

static QFontEngine *loadFc(const QFontPrivate *fp, QUnicodeTables::Script script, const QFontDef &request)
{
    FM_DEBUG("===================== loadFc: script=%d family='%s'\n", script, request.family.toLatin1().constData());
    FcPattern *pattern = getFcPattern(fp, request);

#ifdef FONT_MATCH_DEBUG
    FM_DEBUG("\n\nfinal FcPattern contains:\n");
    FcPatternPrint(pattern);
#endif

    FcResult unused;
    FcPattern *match = FcFontMatch(0, pattern, &unused);
    QFontEngine *fe = tryPatternLoad(match, request, script);
    if (!fe) {
        FcFontSet *fs = FcFontSort(0, pattern, FcTrue, 0, &unused);

        if (match) {
            FcPatternDestroy(match);
            match = 0;
        }

        if (fs) {
            for (int i = 0; !fe && i < fs->nfont; ++i) {
                match = FcFontRenderPrepare(NULL, pattern, fs->fonts[i]);
                fe = tryPatternLoad(match, request, script);
                if (fe)
                    break;
                FcPatternDestroy(match);
                match = 0;
            }
            FcFontSetDestroy(fs);
        }
    }
    FM_DEBUG("engine for script %d is %s\n", script, fe ? fe->fontDef.family.toLatin1().constData(): "(null)");
    FcPatternDestroy(pattern);
    if (match) {
        FcPatternDestroy(match);
    }
    return fe;
}
#endif // QT_NO_FONTCONFIG

static inline QString styleStringHelper(const QFontDatabasePrivate *d_ptr,
                                        const QString &family, int weight, QFont::Style style)
{
    QString result;

    if (weight >= QFont::Black) {
        result = QApplication::translate("QFontDatabase", "Black");
    } else if (weight >= QFont::Bold) {
        result = QApplication::translate("QFontDatabase", "Bold");
    } else if (weight >= QFont::DemiBold) {
        result = QApplication::translate("QFontDatabase", "Demi Bold");
    } else if (weight < QFont::Normal) {
        result = QApplication::translate("QFontDatabase", "Light");
    }

    if (style == QFont::StyleItalic) {
        result += QLatin1Char(' ') + QApplication::translate("QFontDatabase", "Italic");
    } else if (style == QFont::StyleOblique) {
        result += QLatin1Char(' ') + QApplication::translate("QFontDatabase", "Oblique");
    }

    if (result.isEmpty()) {
        foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
            if (fontfamily.family.compare(family, Qt::CaseInsensitive) != 0) {
                continue;
            }
            result = fontfamily.style;
            break;
        }
    }

    return result.simplified();
}

/*!
    \class QFontDatabase
    \threadsafe

    \brief The QFontDatabase class provides information about the fonts available on the system.

    \ingroup appearance

    The most common uses of this class are to query the database for
    the list of font families() and for the pointSizes() and styles()
    that are available for each family. An alternative to pointSizes()
    is smoothSizes() which returns the sizes at which a given family
    and style will look attractive.

    If the font family is available from two or more foundries the
    foundry name is included in the family name; for example:
    "Helvetica [Adobe]" and "Helvetica [Cronyx]". When you specify a
    family, you can use "family [foundry]" format; for example:
    "Helvetica [Cronyx]". If the family has a foundry it is always
    returned using the bracketed format, as is the case with the
    value returned by families().

    The font() function returns a QFont given a family, style and
    point size.

    A family and style combination can be checked to see if it is
    italic() or bold(), and to retrieve its weight(). Similarly we can
    call isSmoothlyScalable(), isScalable() and isFixedPitch().

    Use the styleString() to obtain a text version of a style.

    The QFontDatabase class also supports some static functions, for
    example, standardSizes().

    Example:

    \snippet doc/src/snippets/qfontdatabase/main.cpp 0
    \snippet doc/src/snippets/qfontdatabase/main.cpp 1

    This example gets the list of font families, the list of
    styles for each family, and the point sizes that are available for
    each combination of family and style, displaying this information
    in a tree view.

    \sa QFont, QFontInfo, QFontMetrics, QFontComboBox, {Character Map Example}
*/

/*!
    Creates a font database object.
*/
QFontDatabase::QFontDatabase()
    : d_ptr(new QFontDatabasePrivate())
{
#ifndef QT_NO_FONTCONFIG
#ifdef QFONTDATABASE_DEBUG
    QElapsedTimer elapsedtimer;
    elapsedtimer.start();
#endif

    Q_ASSERT_X(qt_x11Data, "QFontDatabase",
               "A QApplication object needs to be constructed before FontConfig is used.");
    if (!qt_x11Data->has_fontconfig)
        return;

    Q_ASSERT_X(int(QUnicodeTables::ScriptCount) == SpecialCharCount,
               "QFontDatabase", "New scripts have been added.");

    FcObjectSet *os = FcObjectSetCreate();
    if (Q_UNLIKELY(!os)) {
        return;
    }
    FcPattern *pattern = FcPatternCreate();
    if (Q_UNLIKELY(!pattern)) {
        return;
    }
    FcObjectSetAdd(os, FC_FAMILY);
    FcObjectSetAdd(os, FC_SCALABLE);
    FcObjectSetAdd(os, FC_FOUNDRY);
    FcObjectSetAdd(os, FC_WEIGHT);
    FcObjectSetAdd(os, FC_SPACING);
    FcObjectSetAdd(os, FC_STYLE);
    FcObjectSetAdd(os, FC_SLANT);
    FcObjectSetAdd(os, FC_PIXEL_SIZE);
    FcFontSet *fonts = FcFontList(0, pattern, os);
    FcObjectSetDestroy(os);
    FcPatternDestroy(pattern);
    if (Q_UNLIKELY(!fonts)) {
        return;
    }

    for (int i = 0; i < fonts->nfont; i++) {
        FcChar8 *family_value = nullptr;
        FcChar8 *style_value = nullptr;
        FcChar8 *foundry_value = nullptr;
        int weight_value = FC_WEIGHT_MEDIUM;
        int spacing_value = FC_PROPORTIONAL;
        FcBool scalable_value = FcFalse;
        int slant_value = FC_SLANT_ROMAN;
        double pixelsize_value = -1;

        if (FcPatternGetString(fonts->fonts[i], FC_FAMILY, 0, &family_value) != FcResultMatch)
            continue;
        if (FcPatternGetString(fonts->fonts[i], FC_STYLE, 0, &style_value) != FcResultMatch)
            continue;
        FcPatternGetBool(fonts->fonts[i], FC_SCALABLE, 0, &scalable_value);
        if (scalable_value != FcTrue)
            continue;

        if (FcPatternGetString(fonts->fonts[i], FC_FOUNDRY, 0, &foundry_value) != FcResultMatch)
            foundry_value = nullptr;
        if (FcPatternGetInteger(fonts->fonts[i], FC_WEIGHT, 0, &weight_value) != FcResultMatch)
            weight_value = FC_WEIGHT_MEDIUM;
        if (FcPatternGetInteger(fonts->fonts[i], FC_SPACING, 0, &spacing_value) != FcResultMatch)
            spacing_value = FC_PROPORTIONAL;
        if (FcPatternGetInteger(fonts->fonts[i], FC_SLANT, 0, &slant_value) != FcResultMatch)
            slant_value = FC_SLANT_ROMAN;
        if (FcPatternGetDouble(fonts->fonts[i], FC_PIXEL_SIZE, 0, &pixelsize_value) != FcResultMatch)
            pixelsize_value = -1;

        QFontFamily fontfamily;
        fontfamily.family = QString::fromUtf8((const char *)family_value);
        fontfamily.foundry = QString::fromUtf8((const char *)foundry_value);
        fontfamily.style = QString::fromUtf8((const char *)style_value);
        fontfamily.fixedpitch = (spacing_value >= FC_MONO);
        fontfamily.italic = (slant_value >= FC_SLANT_ITALIC);
        fontfamily.bold = (weight_value >= FC_WEIGHT_BOLD); // or FC_WEIGHT_DEMIBOLD?
        fontfamily.weight = weight_value;
        fontfamily.pixelsize = pixelsize_value;
        fontfamily.preference = -weight_value;

        if (fontfamily.style == normalfontstyle || fontfamily.style == regularfontstyle) {
            fontfamily.preference += 1000;
        } else if (fontfamily.style == bookfontstyle) {
            fontfamily.preference += 900;
        } else if (fontfamily.style == romanfontstyle) {
            fontfamily.preference += 800;
        }

        d_ptr->fontfamilies.append(fontfamily);
    }

    FcFontSetDestroy(fonts);

    qStableSort(d_ptr->fontfamilies);

#ifdef QFONTDATABASE_DEBUG
    FD_DEBUG("QFontDatabase: loaded FontConfig: %d ms", int(elapsedtimer.elapsed()));
#endif
#endif // QT_NO_FONTCONFIG
}

/*!
    Destroys the font database object and frees all allocated resources.
*/
QFontDatabase::~QFontDatabase()
{
    delete d_ptr;
}

/*!
    Returns a string that describes the style of the \a font. For
    example, "Bold Italic", "Bold", "Italic" or "Regular". An empty
    string may be returned.
*/
QString QFontDatabase::styleString(const QFont &font)
{
    QString result = font.styleName();
    if (result.isEmpty()) {
        result = styleStringHelper(d_ptr, font.family(), font.weight(), font.style());
    }
    return result;
}

/*!
    Returns a string that describes the style of the \a fontInfo. For
    example, "Bold Italic", "Bold", "Italic" or "Regular". An empty
    string may be returned.
*/
QString QFontDatabase::styleString(const QFontInfo &fontInfo)
{
    QString result = fontInfo.styleName();
    if (result.isEmpty()) {
        result = styleStringHelper(d_ptr, fontInfo.family(), fontInfo.weight(), fontInfo.style());
    }
    return result;
}

/*!
    Returns a sorted list of the available font families.

    If a family exists in several foundries, the returned name for
    that font is in the form "family [foundry]". Examples: "Times
    [Adobe]", "Times [Cronyx]", "Palatino".
*/
QStringList QFontDatabase::families() const
{
    QStringList result;
    QMap<QString,QStringList> familieswithfoundry;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        familieswithfoundry[fontfamily.family].append(fontfamily.foundry);
    }
    QMap<QString,QStringList>::const_iterator familieswithfoundryit = familieswithfoundry.constBegin();
    while (familieswithfoundryit != familieswithfoundry.constEnd()) {
        QStringList foundries = familieswithfoundryit.value();
        foundries.removeDuplicates();
        if (foundries.size() > 1) {
            foreach (const QString &foundry, foundries) {
                result.append(QString::fromLatin1("%1 [%2]").arg(familieswithfoundryit.key(), foundry));
            }
        } else {
            result.append(familieswithfoundryit.key());
        }
        familieswithfoundryit++;
    }
    qSort(result);
    return result;
}

/*!
    Returns a list of the styles available for the font \a family.
    Some example styles: "Light", "Light Italic", "Bold",
    "Oblique", "Demi". The list may be empty.

    If the font family styles includes "Regular", "Book" or "Roman"
    those styles will appear first in the list (in that order).
    Other styles appear in the list according to their weight.

    \sa families()
*/
QStringList QFontDatabase::styles(const QString &family) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    QStringList result;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)) {
            continue;
        }
        result.append(fontfamily.style);
    }

    return result;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is fixed pitch; otherwise returns false.
*/

bool QFontDatabase::isFixedPitch(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    bool result = false;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = fontfamily.fixedpitch;
        break;
    }
    return result;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is smoothly scalable; otherwise returns false. If this
    function returns true, it's safe to scale this font to any size,
    and the result will always look attractive.

    \sa isScalable()
*/
bool QFontDatabase::isSmoothlyScalable(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    bool result = false;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = true;
        break;
    }
    return result;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is scalable; otherwise returns false.

    \sa isSmoothlyScalable()
*/
bool QFontDatabase::isScalable(const QString &family, const QString &style) const
{
    return isSmoothlyScalable(family, style);
}

/*!
    \fn QList<int> QFontDatabase::pointSizes(const QString &family, const QString &style)
    Returns a list of the point sizes available for the font with the
    given \a family and \a style. The list may be empty.

    \sa smoothSizes(), standardSizes()
*/
QList<int> QFontDatabase::pointSizes(const QString &family, const QString &style)
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    QList<int> result;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = standardSizes();
        break;
    }
    return result;
}

/*!
    Returns a QFont object that has family \a family, style \a style
    and point size \a pointSize. If no matching font could be created,
    a QFont object that uses the application's default font is
    returned.
*/
QFont QFontDatabase::font(const QString &family, const QString &style,
                          int pointSize) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    QFont result = QApplication::font();
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }

        result = QFont(fontfamily.family, pointSize, fontfamily.weight, fontfamily.italic);
        result.setStyleName(fontfamily.style);
        result.setBold(fontfamily.bold);
        result.setFixedPitch(fontfamily.fixedpitch);
        if (fontfamily.pixelsize != -1) {
            result.setPixelSize(fontfamily.pixelsize);
        }
        break;
    }
    return result;
}

/*!
    \fn QList<int> QFontDatabase::smoothSizes(const QString &family, const QString &style)
    Returns the point sizes of a font with the given \a family and \a style
    that will look attractive. The list may be empty.
    For non-scalable fonts and bitmap scalable fonts, this function
    is equivalent to pointSizes().

  \sa pointSizes(), standardSizes()
*/
QList<int> QFontDatabase::smoothSizes(const QString &family, const QString &style)
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    QList<int> result;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = standardSizes();
        break;
    }
    return result;
}

/*!
    Returns true if the font that has family \a family and style \a
    style is italic; otherwise returns false.

    \sa weight(), bold()
*/
bool QFontDatabase::italic(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    bool result = false;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = fontfamily.italic;
        break;
    }
    return result;
}


/*!
    Returns true if the font that has family \a family and style \a
    style is bold; otherwise returns false.

    \sa italic(), weight()
*/
bool QFontDatabase::bold(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    bool result = false;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = fontfamily.bold;
        break;
    }
    return result;
}


/*!
    Returns the weight of the font that has family \a family and style
    \a style. If there is no such family and style combination,
    returns -1.

    \sa italic(), bold()
*/
int QFontDatabase::weight(const QString &family, const QString &style) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    int result = -1;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)
            || (!style.isEmpty() && !isStyleMatch(fontfamily.style, style))) {
            continue;
        }
        result = fontfamily.weight;
        break;
    }
    return result;
}


/*!
    Returns true if font \a family is available, false otherwise. Aliases
    such as "Sans Serif" are resolved.
*/
bool QFontDatabase::hasFamily(const QString &family) const
{
    QString parsedfamily, parsedfoundry;
    QFontDatabasePrivate::parseFontName(family, parsedfoundry, parsedfamily);

    const QString familyalias = QFontDatabasePrivate::resolveFontFamilyAlias(parsedfamily);

    bool result = false;
    foreach (const QFontFamily &fontfamily, d_ptr->fontfamilies) {
        if (fontfamily.family.compare(familyalias, Qt::CaseInsensitive) != 0
            || (!parsedfoundry.isEmpty() && fontfamily.foundry.compare(parsedfoundry, Qt::CaseInsensitive) != 0)) {
            continue;
        }
        result = true;
        break;
    }
    return result;
}

/*!
    Returns a list of standard font sizes.

    \sa smoothSizes(), pointSizes()
*/
QList<int> QFontDatabase::standardSizes()
{
    static const QList<int> sizes = QList<int>()
        << 6
        << 7
        << 8
        << 9
        << 10
        << 11
        << 12
        << 14
        << 16
        << 18
        << 20
        << 22
        << 24
        << 26
        << 28
        << 36
        << 48
        << 72;
    return sizes;
}

/*!
    \since 4.4

    Returns true if font rendering is supported outside the GUI
    thread, false otherwise. In other words, a return value of false
    means that all QPainter::drawText() calls outside the GUI thread
    will not produce readable output.

    \sa {Thread-Support in Qt Modules#Painting In Threads}{Painting In Threads}
*/
bool QFontDatabase::supportsThreadedFontRendering()
{
#if defined(QT_NO_FONTCONFIG)
    return false;
#else
    return qt_x11Data->has_fontconfig;
#endif
}

/*!
    \internal

    This makes sense of the font family name:

    if the family name contains a '[' and a ']', then we take the text
    between the square brackets as the foundry, and the text before the
    square brackets as the family (ie. "FreeSans [GNU]")
*/
void QFontDatabasePrivate::parseFontName(const QString &name, QString &foundry, QString &family)
{
    int i = name.indexOf(QLatin1Char('['));
    int li = name.lastIndexOf(QLatin1Char(']'));
    if (i >= 0 && li >= 0 && i < li) {
        foundry = name.mid(i + 1, li - i - 1);
        if (i > 0 && name[i - 1] == QLatin1Char(' '))
            i--;
        family = name.left(i);
    } else {
        foundry.clear();
        family = name;
    }

    // capitalize the family/foundry names
    bool space = true;
    QChar *s = family.data();
    int len = family.length();
    while(len--) {
        if (space) *s = s->toUpper();
        space = s->isSpace();
        ++s;
    }

    space = true;
    s = foundry.data();
    len = foundry.length();
    while(len--) {
        if (space) *s = s->toUpper();
        space = s->isSpace();
        ++s;
    }
}

/*!
    \internal
*/
QString QFontDatabasePrivate::resolveFontFamilyAlias(const QString &family)
{
#if defined(QT_NO_FONTCONFIG)
    return family;
#else
    if (!qt_x11Data->has_fontconfig)
        return family;

    FcPattern *pattern = FcPatternCreate();
    if (Q_UNLIKELY(!pattern)) {
        return family;
    }

    QByteArray cs = family.toUtf8();
    FcPatternAddString(pattern, FC_FAMILY, (const FcChar8 *) cs.constData());
    FcConfigSubstitute(0, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    QString resolved;
    FcChar8 *familyAfterSubstitution = nullptr;
    FcResult unused;
    FcPattern *match = FcFontMatch(0, pattern, &unused);
    if (match) {
        FcPatternGetString(match, FC_FAMILY, 0, &familyAfterSubstitution);
        resolved = QString::fromUtf8((const char *) familyAfterSubstitution);
        FcPatternDestroy(match);
    } else {
        FcPatternGetString(pattern, FC_FAMILY, 0, &familyAfterSubstitution);
        resolved = QString::fromUtf8((const char *) familyAfterSubstitution);
    }
    FcPatternDestroy(pattern);

    return resolved;
#endif
}

/*!
    \internal

    Loads a QFontEngine for the specified \a script that matches the
    QFontDef \e request member variable.
*/
QFontEngine* QFontDatabasePrivate::load(const QFontPrivate *d, int script)
{
    Q_ASSERT(script >= 0 && script < QUnicodeTables::ScriptCount);

    // normalize the request to get better caching
    QFontDef req = d->request;
    if (req.pixelSize <= 0) {
        req.pixelSize = qt_pixelSize(req.pointSize, d->dpi);
    }
    if (req.pixelSize < 1) {
        req.pixelSize = 1;
    }

    if (req.weight == 0) {
        req.weight = QFont::Normal;
    }
    if (req.stretch == 0) {
        req.stretch = QFont::Unstretched;
    }

    QFontCache::Key key(req, script, d->screen);
    // set it to the actual pointsize, so QFontInfo will do the right thing
    if (req.pointSize < 0) {
        req.pointSize = qt_pointSize(req.pixelSize, d->dpi);
    }

    QFontEngine *fe = QFontCache::instance()->findEngine(key);
    if (fe) {
        return fe;
    }

    const ushort firstchar = (req.family.isEmpty() ? 0 : req.family.at(0).unicode());
    // relative or absolute path
    if (firstchar == '.' || firstchar == '/') {
        QFontEngineFT* ftfe = new QFontEngineFT(req);
        if (ftfe->invalid()) {
            delete ftfe;
        } else {
            fe = ftfe;
        }
    }

#ifndef QT_NO_FONTCONFIG
    if (!fe && qt_x11Data->has_fontconfig) {
        fe = loadFc(d, static_cast<QUnicodeTables::Script>(script), req);
    }
#endif

    if (!fe) {
        fe = new QFontEngineBox(req.pixelSize);
    }
    QFontCache::instance()->insertEngine(key, fe);
    return fe;
}

QT_END_NAMESPACE
