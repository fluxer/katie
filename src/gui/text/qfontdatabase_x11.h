/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qplatformdefs.h>
#include "qdebug.h"
#include "qpaintdevice.h"
#include "qelapsedtimer.h"
#include "qfile.h"
#include "qtemporaryfile.h"
#include "qabstractfileengine.h"
#include "qmath.h"

#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <qt_x11_p.h>
#include "qx11info_x11.h"
#include <qfontengine_x11_p.h>

#ifndef QT_NO_FONTCONFIG
#include <ft2build.h>
#include FT_FREETYPE_H

#include <fontconfig/fcfreetype.h>
#endif

QT_BEGIN_NAMESPACE

// from qfont_x11.cpp
extern double qt_pointSize(double pixelSize, int dpi);
extern double qt_pixelSize(double pointSize, int dpi);

// from qapplication.cpp
extern bool qt_is_gui_used;

static inline void capitalize (char *s)
{
    bool space = true;
    while(*s) {
        if (space)
            *s = toupper(*s);
        space = (*s == ' ');
        ++s;
    }
}


/*
  To regenerate the writingSystems_for_xlfd_encoding table, run
  'util/unicode/x11/makeencodings' and paste the generated
  'encodings.c' here.
*/
// ----- begin of generated code -----

static const struct XlfdTblData {
    const char *name;
    const int id;
    const int mib;
} XlfdTbl[] = {
    { "iso8859-1", 0, 4 },
    { "iso8859-2", 1, 5 },
    { "iso8859-3", 2, 6 },
    { "iso8859-4", 3, 7 },
    { "iso8859-9", 4, 12 },
    { "iso8859-10", 5, 13 },
    { "iso8859-13", 6, 109 },
    { "iso8859-14", 7, 110 },
    { "iso8859-15", 8, 111 },
    { "hp-roman8", 9, 2004 },
    { "iso8859-5", 10, 8 },
    { "*-cp1251", 11, 2251 },
    { "koi8-ru", 12, 2084 },
    { "koi8-u", 13, 2088 },
    { "koi8-r", 14, 2084 },
    { "iso8859-7", 15, 10 },
    { "iso8859-8", 16, 85 },
    { "gb18030-0", 17, -114 },
    { "gb18030.2000-0", 18, -113 },
    { "gbk-0", 19, -113 },
    { "gb2312.*-0", 20, 57 },
    { "jisx0201*-0", 21, 15 },
    { "jisx0208*-0", 22, 63 },
    { "ksc5601*-*", 23, 36 },
    { "big5hkscs-0", 24, -2101 },
    { "hkscs-1", 25, -2101 },
    { "big5*-*", 26, -2026 },
    { "tscii-*", 27, 2028 },
    { "tis620*-*", 28, 2259 },
    { "iso8859-11", 29, 2259 },
    { "mulelao-1", 30, -4242 },
    { "ethiopic-unicode", 31, 0 },
    { "iso10646-1", 32, 0 },
    { "unicode-*", 33, 0 },
    { "*-symbol", 34, 0 },
    { "*-fontspecific", 35, 0 },
    { "fontspecific-*", 36, 0 }
};

// ----- end of generated code -----


const qint16 XlfdTblSize = sizeof(XlfdTbl) / sizeof(XlfdTblData);

int qt_encoding_id_for_mib(int mib)
{
    for (qint16 i = 0; i < XlfdTblSize; i++) {
        if (XlfdTbl[i].mib == mib)
            return XlfdTbl[i].id;
    }
    return -1;
}


#ifdef QFONTDATABASE_DEBUG
static const char * xlfd_for_id(int id)
{
    // special case: -1 returns the "*-*" encoding, allowing us to do full
    // database population in a single X server round trip.
    if (id < 0 || id > XlfdTblSize)
        return "*-*";
    return XlfdTbl[i].name;
}
#endif

#ifndef QT_NO_FONTCONFIG

#ifndef FC_WIDTH
#define FC_WIDTH "width"
#endif

static int getFCWeight(int fc_weight)
{
    int qtweight = QFont::Black;
    if (fc_weight <= (FC_WEIGHT_LIGHT + FC_WEIGHT_MEDIUM) / 2)
        qtweight = QFont::Light;
    else if (fc_weight <= (FC_WEIGHT_MEDIUM + FC_WEIGHT_DEMIBOLD) / 2)
        qtweight = QFont::Normal;
    else if (fc_weight <= (FC_WEIGHT_DEMIBOLD + FC_WEIGHT_BOLD) / 2)
        qtweight = QFont::DemiBold;
    else if (fc_weight <= (FC_WEIGHT_BOLD + FC_WEIGHT_BLACK) / 2)
        qtweight = QFont::Bold;

    return qtweight;
}

QFontDef qt_FcPatternToQFontDef(FcPattern *pattern, const QFontDef &request)
{
    QFontDef fontDef;
    fontDef.styleStrategy = request.styleStrategy;

    fontDef.hintingPreference = request.hintingPreference;
    FcChar8 *value = 0;
    if (FcPatternGetString(pattern, FC_FAMILY, 0, &value) == FcResultMatch) {
        fontDef.family = QString::fromUtf8(reinterpret_cast<const char *>(value));
    }

    double dpi;
    if (FcPatternGetDouble(pattern, FC_DPI, 0, &dpi) != FcResultMatch) {
        if (qt_x11Data->display)
            dpi = QX11Info::appDpiY();
        else
            dpi = qt_defaultDpiY();
    }

    double size;
    if (FcPatternGetDouble(pattern, FC_PIXEL_SIZE, 0, &size) == FcResultMatch)
        fontDef.pixelSize = size;
    else
        fontDef.pixelSize = 12;

    fontDef.pointSize = qt_pointSize(fontDef.pixelSize, qRound(dpi));

    /* ###
       fontDef.styleHint
    */

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


    FcBool scalable;
    if (FcPatternGetBool(pattern, FC_SCALABLE, 0, &scalable) != FcResultMatch)
        scalable = false;
    if (scalable) {
        fontDef.stretch = request.stretch;
        fontDef.style = request.style;
    } else {
        int width;
        if (FcPatternGetInteger(pattern, FC_WIDTH, 0, &width) == FcResultMatch)
            fontDef.stretch = width;
        else
            fontDef.stretch = 100;
    }

    int spacing;
    if (FcPatternGetInteger(pattern, FC_SPACING, 0, &spacing) == FcResultMatch) {
        fontDef.fixedPitch = (spacing >= FC_MONO);
        fontDef.ignorePitch = false;
    } else {
        fontDef.ignorePitch = true;
    }

    return fontDef;
}

static const char *specialLanguages[] = {
    "en", // Common
    "el", // Greek
    "ru", // Cyrillic
    "hy", // Armenian
    "he", // Hebrew
    "ar", // Arabic
    "syr", // Syriac
    "div", // Thaana
    "hi", // Devanagari
    "bn", // Bengali
    "pa", // Gurmukhi
    "gu", // Gujarati
    "or", // Oriya
    "ta", // Tamil
    "te", // Telugu
    "kn", // Kannada
    "ml", // Malayalam
    "si", // Sinhala
    "th", // Thai
    "lo", // Lao
    "bo", // Tibetan
    "my", // Myanmar
    "ka", // Georgian
    "ko", // Hangul
    "", // Ogham
    "", // Runic
    "km", // Khmer
    "" // N'Ko
};
enum { SpecialLanguageCount = sizeof(specialLanguages) / sizeof(const char *) };

static const ushort specialChars[] = {
    0, // English
    0, // Greek
    0, // Cyrillic
    0, // Armenian
    0, // Hebrew
    0, // Arabic
    0, // Syriac
    0, // Thaana
    0, // Devanagari
    0, // Bengali
    0, // Gurmukhi
    0, // Gujarati
    0, // Oriya
    0, // Tamil
    0xc15, // Telugu
    0xc95, // Kannada
    0xd15, // Malayalam
    0xd9a, // Sinhala
    0, // Thai
    0, // Lao
    0, // Tibetan
    0x1000, // Myanmar
    0, // Georgian
    0, // Hangul
    0x1681, // Ogham
    0x16a0, // Runic
    0,  // Khmer
    0x7ca // N'Ko
};
enum { SpecialCharCount = sizeof(specialChars) / sizeof(ushort) };

// this could become a list of all languages used for each writing
// system, instead of using the single most common language.
static const char *languageForWritingSystem[] = {
    0,     // Any
    "en",  // Latin
    "el",  // Greek
    "ru",  // Cyrillic
    "hy",  // Armenian
    "he",  // Hebrew
    "ar",  // Arabic
    "syr", // Syriac
    "div", // Thaana
    "hi",  // Devanagari
    "bn",  // Bengali
    "pa",  // Gurmukhi
    "gu",  // Gujarati
    "or",  // Oriya
    "ta",  // Tamil
    "te",  // Telugu
    "kn",  // Kannada
    "ml",  // Malayalam
    "si",  // Sinhala
    "th",  // Thai
    "lo",  // Lao
    "bo",  // Tibetan
    "my",  // Myanmar
    "ka",  // Georgian
    "km",  // Khmer
    "zh-cn", // SimplifiedChinese
    "zh-tw", // TraditionalChinese
    "ja",  // Japanese
    "ko",  // Korean
    "vi",  // Vietnamese
    0, // Symbol
    0, // Ogham
    0, // Runic
    0 // N'Ko
};
enum { LanguageCount = sizeof(languageForWritingSystem) / sizeof(const char *) };

// Unfortunately FontConfig doesn't know about some languages. We have to test these through the
// charset. The lists below contain the systems where we need to do this.
static const ushort sampleCharForWritingSystem[] = {
    0,     // Any
    0,  // Latin
    0,  // Greek
    0,  // Cyrillic
    0,  // Armenian
    0,  // Hebrew
    0,  // Arabic
    0, // Syriac
    0, // Thaana
    0,  // Devanagari
    0,  // Bengali
    0,  // Gurmukhi
    0,  // Gujarati
    0,  // Oriya
    0,  // Tamil
    0xc15,  // Telugu
    0xc95,  // Kannada
    0xd15,  // Malayalam
    0xd9a,  // Sinhala
    0,  // Thai
    0,  // Lao
    0,  // Tibetan
    0x1000,  // Myanmar
    0,  // Georgian
    0,  // Khmer
    0, // SimplifiedChinese
    0, // TraditionalChinese
    0,  // Japanese
    0,  // Korean
    0,  // Vietnamese
    0, // Symbol
    0x1681, // Ogham
    0x16a0, // Runic
    0x7ca // N'Ko
};
enum { SampleCharCount = sizeof(sampleCharForWritingSystem) / sizeof(ushort) };

// Newer FontConfig let's us sort out fonts that contain certain glyphs, but no
// open type tables for is directly. Do this so we don't pick some strange
// pseudo unicode font
static const char *openType[] = {
    0,     // Any
    0,  // Latin
    0,  // Greek
    0,  // Cyrillic
    0,  // Armenian
    0,  // Hebrew
    0,  // Arabic
    "syrc",  // Syriac
    "thaa",  // Thaana
    "deva",  // Devanagari
    "beng",  // Bengali
    "guru",  // Gurmukhi
    "gurj",  // Gujarati
    "orya",  // Oriya
    "taml",  // Tamil
    "telu",  // Telugu
    "knda",  // Kannada
    "mlym",  // Malayalam
    "sinh",  // Sinhala
    0,  // Thai
    0,  // Lao
    "tibt",  // Tibetan
    "mymr",  // Myanmar
    0,  // Georgian
    "khmr",  // Khmer
    0, // SimplifiedChinese
    0, // TraditionalChinese
    0,  // Japanese
    0,  // Korean
    0,  // Vietnamese
    0, // Symbol
    0, // Ogham
    0, // Runic
    "nko " // N'Ko
};
enum { OpenTypeCount = sizeof(openType) / sizeof(const char *) };


static void loadFontConfig()
{
    Q_ASSERT_X(qt_x11Data, "QFontDatabase",
               "A QApplication object needs to be constructed before FontConfig is used.");
    if (!qt_x11Data->has_fontconfig)
        return;

    Q_ASSERT_X(int(QUnicodeTables::ScriptCount) == SpecialLanguageCount,
               "QFontDatabase", "New scripts have been added.");
    Q_ASSERT_X(int(QUnicodeTables::ScriptCount) == SpecialCharCount,
               "QFontDatabase", "New scripts have been added.");
    Q_ASSERT_X(int(QFontDatabase::WritingSystemsCount) == LanguageCount,
               "QFontDatabase", "New writing systems have been added.");
    Q_ASSERT_X(int(QFontDatabase::WritingSystemsCount) == SampleCharCount,
               "QFontDatabase", "New writing systems have been added.");
    Q_ASSERT_X(int(QFontDatabase::WritingSystemsCount) == OpenTypeCount,
               "QFontDatabase", "New writing systems have been added.");

    QFontDatabasePrivate *db = privateDb();
    FcFontSet  *fonts;

    FcPattern *pattern = FcPatternCreate();
    FcDefaultSubstitute(pattern);
    FcChar8 *lang = 0;
    if (FcPatternGetString(pattern, FC_LANG, 0, &lang) == FcResultMatch)
        db->systemLang = QString::fromUtf8((const char *) lang);
    FcPatternDestroy(pattern);

    QString familyName;
    FcChar8 *value = 0;
    int weight_value;
    int slant_value;
    int spacing_value;
    FcChar8 *file_value;
    int index_value;
    FcChar8 *foundry_value;
    FcChar8 *style_value;
    FcBool scalable;

    {
        FcObjectSet *os = FcObjectSetCreate();
        FcPattern *pattern = FcPatternCreate();
        const char *properties [] = {
            FC_FAMILY, FC_STYLE, FC_WEIGHT, FC_SLANT,
            FC_SPACING, FC_FILE, FC_INDEX,
            FC_LANG, FC_CHARSET, FC_FOUNDRY, FC_SCALABLE, FC_PIXEL_SIZE, FC_WEIGHT,
            FC_WIDTH,
            FC_CAPABILITY,
            (const char *)0
        };
        const char **p = properties;
        while (*p) {
            FcObjectSetAdd(os, *p);
            ++p;
        }
        fonts = FcFontList(0, pattern, os);
        FcObjectSetDestroy(os);
        FcPatternDestroy(pattern);
    }

    for (int i = 0; i < fonts->nfont; i++) {
        if (FcPatternGetString(fonts->fonts[i], FC_FAMILY, 0, &value) != FcResultMatch)
            continue;
        //         capitalize(value);
        familyName = QString::fromUtf8((const char *)value);
        slant_value = FC_SLANT_ROMAN;
        weight_value = FC_WEIGHT_MEDIUM;
        spacing_value = FC_PROPORTIONAL;
        file_value = 0;
        index_value = 0;
        scalable = FcTrue;

        if (FcPatternGetInteger (fonts->fonts[i], FC_SLANT, 0, &slant_value) != FcResultMatch)
            slant_value = FC_SLANT_ROMAN;
        if (FcPatternGetInteger (fonts->fonts[i], FC_WEIGHT, 0, &weight_value) != FcResultMatch)
            weight_value = FC_WEIGHT_MEDIUM;
        if (FcPatternGetInteger (fonts->fonts[i], FC_SPACING, 0, &spacing_value) != FcResultMatch)
            spacing_value = FC_PROPORTIONAL;
        if (FcPatternGetString (fonts->fonts[i], FC_FILE, 0, &file_value) != FcResultMatch)
            file_value = 0;
        if (FcPatternGetInteger (fonts->fonts[i], FC_INDEX, 0, &index_value) != FcResultMatch)
            index_value = 0;
        if (FcPatternGetBool(fonts->fonts[i], FC_SCALABLE, 0, &scalable) != FcResultMatch)
            scalable = FcTrue;
        if (FcPatternGetString(fonts->fonts[i], FC_FOUNDRY, 0, &foundry_value) != FcResultMatch)
            foundry_value = 0;
        if (FcPatternGetString(fonts->fonts[i], FC_STYLE, 0, &style_value) != FcResultMatch)
            style_value = 0;
        QtFontFamily *family = db->family(familyName, true);

        FcLangSet *langset = 0;
        FcResult res = FcPatternGetLangSet(fonts->fonts[i], FC_LANG, 0, &langset);
        if (res == FcResultMatch) {
            for (int i = 1; i < LanguageCount; ++i) {
                const FcChar8 *lang = (const FcChar8*) languageForWritingSystem[i];
                if (!lang) {
                    family->writingSystems[i] |= QtFontFamily::UnsupportedFT;
                } else {
                    FcLangResult langRes = FcLangSetHasLang(langset, lang);
                    if (langRes != FcLangDifferentLang)
                        family->writingSystems[i] = QtFontFamily::Supported;
                    else
                        family->writingSystems[i] |= QtFontFamily::UnsupportedFT;
                }
            }
            family->writingSystems[QFontDatabase::Other] = QtFontFamily::UnsupportedFT;
            family->ftWritingSystemCheck = true;
        } else {
            // we set Other to supported for symbol fonts. It makes no
            // sense to merge these with other ones, as they are
            // special in a way.
            for (int i = 1; i < LanguageCount; ++i)
                family->writingSystems[i] |= QtFontFamily::UnsupportedFT;
            family->writingSystems[QFontDatabase::Other] = QtFontFamily::Supported;
        }

        FcCharSet *cs = 0;
        res = FcPatternGetCharSet(fonts->fonts[i], FC_CHARSET, 0, &cs);
        if (res == FcResultMatch) {
            // some languages are not supported by FontConfig, we rather check the
            // charset to detect these
            for (int i = 1; i < SampleCharCount; ++i) {
                if (!sampleCharForWritingSystem[i])
                    continue;
                if (FcCharSetHasChar(cs, sampleCharForWritingSystem[i]))
                    family->writingSystems[i] = QtFontFamily::Supported;
            }
        }

        for (int j = 1; j < LanguageCount; ++j) {
            if (family->writingSystems[j] == QtFontFamily::Supported && requiresOpenType(j) && openType[j]) {
                FcChar8 *cap;
                res = FcPatternGetString (fonts->fonts[i], FC_CAPABILITY, 0, &cap);
                if (res != FcResultMatch || !strstr((const char *)cap, openType[j]))
                    family->writingSystems[j] = QtFontFamily::UnsupportedFT;
            }
        }

        QByteArray file((const char *)file_value);
        family->fontFilename = file;
        family->fontFileIndex = index_value;

        QtFontStyle::Key styleKey;
        QString styleName = style_value ? QString::fromUtf8((const char *) style_value) : QString();
        styleKey.style = (slant_value == FC_SLANT_ITALIC)
                         ? QFont::StyleItalic
                         : ((slant_value == FC_SLANT_OBLIQUE)
                            ? QFont::StyleOblique
                            : QFont::StyleNormal);
        styleKey.weight = getFCWeight(weight_value);
        if (!scalable) {
            int width = 100;
            FcPatternGetInteger (fonts->fonts[i], FC_WIDTH, 0, &width);
            styleKey.stretch = width;
        }

        QtFontFoundry *foundry
            = family->foundry(foundry_value ? QString::fromUtf8((const char *)foundry_value) : QString(), true);
        QtFontStyle *style = foundry->style(styleKey, styleName, true);

        if (spacing_value < FC_MONO)
            family->fixedPitch = false;

        QtFontSize *size;
        if (scalable) {
            style->smoothScalable = true;
            size = style->pixelSize(SMOOTH_SCALABLE, true);
        } else {
            double pixel_size = 0;
            FcPatternGetDouble (fonts->fonts[i], FC_PIXEL_SIZE, 0, &pixel_size);
            size = style->pixelSize((int)pixel_size, true);
        }
        QtFontEncoding *enc = size->encodingID(-1, 0, 0, 0, 0, true);
        enc->pitch = (spacing_value >= FC_CHARCELL ? 'c' :
                      (spacing_value >= FC_MONO ? 'm' : 'p'));
    }

    FcFontSetDestroy (fonts);

    struct FcDefaultFont {
        const char *qtname;
        bool fixed;
    };
    const FcDefaultFont defaults[] = {
        { "Serif", false },
        { "Sans Serif", false },
        { "Monospace", true },
        { 0, false }
    };
    const FcDefaultFont *f = defaults;
    while (f->qtname) {
        QtFontFamily *family = db->family(QLatin1String(f->qtname), true);
        family->fixedPitch = f->fixed;
        family->synthetic = true;
        QtFontFoundry *foundry = family->foundry(QString(), true);

        // aliases only make sense for 'common', not for any of the specials
        for (int i = 1; i < LanguageCount; ++i) {
            if (requiresOpenType(i))
                family->writingSystems[i] = QtFontFamily::UnsupportedFT;
            else
                family->writingSystems[i] = QtFontFamily::Supported;
        }
        family->writingSystems[QFontDatabase::Other] = QtFontFamily::UnsupportedFT;

        QtFontStyle::Key styleKey;
        for (int i = 0; i < 4; ++i) {
            styleKey.style = (i%2) ? QFont::StyleNormal : QFont::StyleItalic;
            styleKey.weight = (i > 1) ? QFont::Bold : QFont::Normal;
            QtFontStyle *style = foundry->style(styleKey, QString(), true);
            style->smoothScalable = true;
            QtFontSize *size = style->pixelSize(SMOOTH_SCALABLE, true);
            QtFontEncoding *enc = size->encodingID(-1, 0, 0, 0, 0, true);
            enc->pitch = (f->fixed ? 'm' : 'p');
        }
        ++f;
    }
}
#endif // QT_NO_FONTCONFIG

static void checkSymbolFont(QtFontFamily *family)
{
#ifndef QT_NO_FONTCONFIG
    if (!family || family->symbol_checked || family->fontFilename.isEmpty())
        return;
    family->symbol_checked = true;

    QFontEngine::FaceId id;
    id.filename = family->fontFilename;
    id.index = family->fontFileIndex;
    QFreetypeFace *f = QFreetypeFace::getFace(id);
    if (!f) {
        qWarning("checkSymbolFonts: Couldn't open face %s (%s/%d)",
                 qPrintable(family->name), family->fontFilename.data(), family->fontFileIndex);
        return;
    }
    for (int i = 0; i < f->face->num_charmaps; ++i) {
        FT_CharMap cm = f->face->charmaps[i];
        if (cm->encoding == FT_ENCODING_ADOBE_CUSTOM
            || cm->encoding == FT_ENCODING_MS_SYMBOL) {
            for (int x = QFontDatabase::Latin; x < QFontDatabase::Other; ++x)
                family->writingSystems[x] = QtFontFamily::Unsupported;
            family->writingSystems[QFontDatabase::Other] = QtFontFamily::Supported;
            break;
        }
    }
    f->release(id);
#else
    Q_UNUSED(family);
#endif
}

static void checkSymbolFonts(const QString &family = QString())
{
#ifndef QT_NO_FONTCONFIG
    QFontDatabasePrivate *d = privateDb();

    if (family.isEmpty()) {
        for (int i = 0; i < d->count; ++i)
            checkSymbolFont(d->families[i]);
    } else {
        checkSymbolFont(d->family(family));
    }
#else
    Q_UNUSED(family);
#endif
}

static void registerFont(QFontDatabasePrivate::ApplicationFont *fnt);

static void initializeFontDb()
{
    QFontDatabasePrivate *db = privateDb();
    if (!db || db->count)
        return;

    QElapsedTimer t;
    t.start();

#ifndef QT_NO_FONTCONFIG
    if (db->reregisterAppFonts) {
        db->reregisterAppFonts = false;
        for (int i = 0; i < db->applicationFonts.count(); ++i)
            if (!db->applicationFonts.at(i).families.isEmpty()) {
                registerFont(&db->applicationFonts[i]);
            }
    }

    loadFontConfig();
    FD_DEBUG("QFontDatabase: loaded FontConfig: %d ms", int(t.elapsed()));
#endif

    t.start();

#ifndef QT_NO_FONTCONFIG
    for (int i = 0; i < db->count; i++) {
        for (int j = 0; j < db->families[i]->count; ++j) {        // each foundry
            QtFontFoundry *foundry = db->families[i]->foundries[j];
            for (int k = 0; k < foundry->count; ++k) {
                QtFontStyle *style = foundry->styles[k];
                if (style->key.style != QFont::StyleNormal) continue;

                QtFontSize *size = style->pixelSize(SMOOTH_SCALABLE);
                if (! size) continue; // should not happen
                QtFontEncoding *enc = size->encodingID(-1, 0, 0, 0, 0, true);
                if (! enc) continue; // should not happen either

                QtFontStyle::Key key = style->key;

                // does this style have an italic equivalent?
                key.style = QFont::StyleItalic;
                QtFontStyle *equiv = foundry->style(key);
                if (equiv) continue;

                // does this style have an oblique equivalent?
                key.style = QFont::StyleOblique;
                equiv = foundry->style(key);
                if (equiv) continue;

                // let's fake one...
                equiv = foundry->style(key, QString(), true);
                equiv->styleName = styleStringHelper(key.weight, QFont::Style(key.style));
                equiv->smoothScalable = true;

                QtFontSize *equiv_size = equiv->pixelSize(SMOOTH_SCALABLE, true);
                QtFontEncoding *equiv_enc = equiv_size->encodingID(-1, 0, 0, 0, 0, true);

                // keep the same pitch
                equiv_enc->pitch = enc->pitch;
            }
        }
    }
#endif


#ifdef QFONTDATABASE_DEBUG
    // print the database
    for (int f = 0; f < db->count; f++) {
        QtFontFamily *family = db->families[f];
        FD_DEBUG("'%s' %s  fixed=%s", family->name.latin1(), (family->fixedPitch ? "fixed" : ""),
                 (family->fixedPitch ? "yes" : "no"));
        for (int i = 0; i < QFontDatabase::WritingSystemsCount; ++i) {
            QFontDatabase::WritingSystem ws = QFontDatabase::WritingSystem(i);
            FD_DEBUG("\t%s: %s", QFontDatabase::writingSystemName(ws).toLatin1().constData(),
                     ((family->writingSystems[i] & QtFontFamily::Supported) ? "Supported" :
                      (family->writingSystems[i] & QtFontFamily::Unsupported) == QtFontFamily::Unsupported ?
                      "Unsupported" : "Unknown"));
        }

        for (int fd = 0; fd < family->count; fd++) {
            QtFontFoundry *foundry = family->foundries[fd];
            FD_DEBUG("\t\t'%s'", foundry->name.latin1());
            for (int s = 0; s < foundry->count; s++) {
                QtFontStyle *style = foundry->styles[s];
                FD_DEBUG("\t\t\tstyle: style=%d weight=%d (%s)\n"
                         "\t\t\tstretch=%d (%s)",
                         style->key.style, style->key.weight,
                         style->weightName, style->key.stretch,
                         style->setwidthName ? style->setwidthName : "nil");
                if (style->smoothScalable)
                    FD_DEBUG("\t\t\t\tsmooth scalable");
                else if (style->bitmapScalable)
                    FD_DEBUG("\t\t\t\tbitmap scalable");
                if (style->pixelSizes) {
                    qDebug("\t\t\t\t%d pixel sizes", style->count);
                    for (int z = 0; z < style->count; ++z) {
                        QtFontSize *size = style->pixelSizes + z;
                        for (int e = 0; e < size->count; ++e) {
                            FD_DEBUG("\t\t\t\t  size %5d pitch %c encoding %s",
                                     size->pixelSize,
                                     size->encodings[e].pitch,
                                     xlfd_for_id(size->encodings[e].encoding));
                        }
                    }
                }
            }
        }
    }
#endif // QFONTDATABASE_DEBUG
}


// --------------------------------------------------------------------------------------
// font loader
// --------------------------------------------------------------------------------------

static const char *styleHint(const QFontDef &request)
{
    const char *stylehint = 0;
    switch (request.styleHint) {
    case QFont::SansSerif:
        stylehint = "sans-serif";
        break;
    case QFont::Serif:
        stylehint = "serif";
        break;
    case QFont::TypeWriter:
        stylehint = "monospace";
        break;
    default:
        if (request.fixedPitch)
            stylehint = "monospace";
        break;
    }
    return stylehint;
}

#ifndef QT_NO_FONTCONFIG

void qt_addPatternProps(FcPattern *pattern, int screen, int script, const QFontDef &request)
{
    double size_value = qMax(qreal(1.), request.pixelSize);
    FcPatternDel(pattern, FC_PIXEL_SIZE);
    FcPatternAddDouble(pattern, FC_PIXEL_SIZE, size_value);

    if (qt_x11Data->display && QX11Info::appDepth(screen) <= 8) {
        FcPatternDel(pattern, FC_ANTIALIAS);
        // can't do antialiasing on 8bpp
        FcPatternAddBool(pattern, FC_ANTIALIAS, false);
    } else if (request.styleStrategy & (QFont::PreferAntialias|QFont::NoAntialias)) {
        FcPatternDel(pattern, FC_ANTIALIAS);
        FcPatternAddBool(pattern, FC_ANTIALIAS,
                         !(request.styleStrategy & QFont::NoAntialias));
    }

    if (script != QUnicodeTables::Common && *specialLanguages[script] != '\0') {
        Q_ASSERT(script < QUnicodeTables::ScriptCount);
        FcLangSet *ls = FcLangSetCreate();
        FcLangSetAdd(ls, (const FcChar8*)specialLanguages[script]);
        FcPatternDel(pattern, FC_LANG);
        FcPatternAddLangSet(pattern, FC_LANG, ls);
        FcLangSetDestroy(ls);
    }

    if (!request.styleName.isEmpty()) {
        QByteArray cs = request.styleName.toUtf8();
        FcPatternAddString(pattern, FC_STYLE, (const FcChar8 *) cs.constData());
        return;
    }

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
    FcPatternDel(pattern, FC_WEIGHT);
    FcPatternAddInteger(pattern, FC_WEIGHT, weight_value);

    int slant_value = FC_SLANT_ROMAN;
    if (request.style == QFont::StyleItalic)
        slant_value = FC_SLANT_ITALIC;
    else if (request.style == QFont::StyleOblique)
        slant_value = FC_SLANT_OBLIQUE;
    FcPatternDel(pattern, FC_SLANT);
    FcPatternAddInteger(pattern, FC_SLANT, slant_value);

    int stretch = request.stretch;
    if (!stretch)
        stretch = 100;
    FcPatternDel(pattern, FC_WIDTH);
    FcPatternAddInteger(pattern, FC_WIDTH, stretch);
}

static bool preferScalable(const QFontDef &request)
{
    return request.styleStrategy & (QFont::PreferOutline|QFont::ForceOutline|QFont::PreferQuality|QFont::PreferAntialias);
}


static FcPattern *getFcPattern(const QFontPrivate *fp, int script, const QFontDef &request)
{
    if (!qt_x11Data->has_fontconfig)
        return 0;

    FcPattern *pattern = FcPatternCreate();
    if (!pattern)
        return 0;

    FcValue value;
    value.type = FcTypeString;

    QtFontDesc desc;
    QStringList families_and_foundries = familyList(request);
    for (int i = 0; i < families_and_foundries.size(); ++i) {
        QString family, foundry;
        parseFontName(families_and_foundries.at(i), foundry, family);
        if (!family.isEmpty()) {
            QByteArray cs = family.toUtf8();
            value.u.s = (const FcChar8 *)cs.data();
            FcPatternAdd(pattern, FC_FAMILY, value, FcTrue);
        }
        if (i == 0) {
            QT_PREPEND_NAMESPACE(match)(script, request, family, foundry, -1, &desc);
            if (!foundry.isEmpty()) {
                QByteArray cs = foundry.toUtf8();
                value.u.s = (const FcChar8 *)cs.data();
                FcPatternAddWeak(pattern, FC_FOUNDRY, value, FcTrue);
            }
        }
    }

    const char *stylehint = styleHint(request);
    if (stylehint) {
        value.u.s = (const FcChar8 *)stylehint;
        FcPatternAddWeak(pattern, FC_FAMILY, value, FcTrue);
    }

    if (!request.ignorePitch) {
        char pitch_value = FC_PROPORTIONAL;
        if (request.fixedPitch || (desc.family && desc.family->fixedPitch))
            pitch_value = FC_MONO;
        FcPatternAddInteger(pattern, FC_SPACING, pitch_value);
    }
    FcPatternAddBool(pattern, FC_OUTLINE, !(request.styleStrategy & QFont::PreferBitmap));
    if (preferScalable(request) || (desc.style && desc.style->smoothScalable))
        FcPatternAddBool(pattern, FC_SCALABLE, true);

    qt_addPatternProps(pattern, fp->screen, script, request);

    FcConfigSubstitute(0, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    // these should only get added to the pattern _after_ substitution
    // append the default fallback font for the specified script
    extern QString qt_fallback_font_family(int);
    QString fallback = qt_fallback_font_family(script);
    if (!fallback.isEmpty()) {
        QByteArray cs = fallback.toUtf8();
        value.u.s = (const FcChar8 *)cs.data();
        FcPatternAddWeak(pattern, FC_FAMILY, value, FcTrue);
    }

    // add the default family
    QString defaultFamily = QApplication::font().family();
    QByteArray cs = defaultFamily.toUtf8();
    value.u.s = (const FcChar8 *)cs.data();
    FcPatternAddWeak(pattern, FC_FAMILY, value, FcTrue);

    // add QFont::defaultFamily() to the list, for compatibility with
    // previous versions
    defaultFamily = QApplication::font().defaultFamily();
    cs = defaultFamily.toUtf8();
    value.u.s = (const FcChar8 *)cs.data();
    FcPatternAddWeak(pattern, FC_FAMILY, value, FcTrue);

    return pattern;
}


static void FcFontSetRemove(FcFontSet *fs, int at)
{
    Q_ASSERT(at < fs->nfont);
    FcPatternDestroy(fs->fonts[at]);
    int len = (--fs->nfont - at) * sizeof(FcPattern *);;
    if (len > 0)
        memmove(fs->fonts + at, fs->fonts + at + 1, len);
}

static QFontEngine *tryPatternLoad(FcPattern *match, int screen,
                                   const QFontDef &request, int script)
{
#ifdef FONT_MATCH_DEBUG
    FcChar8 *fam;
    FcPatternGetString(match, FC_FAMILY, 0, &fam);
    FM_DEBUG("==== trying %s\n", fam);
#endif
    FM_DEBUG("passes charset test\n");

    QFontEngineX11FT *engine = 0;
    if (!match) // probably no fonts available.
        goto done;

    if (script != QUnicodeTables::Common) {
        // skip font if it doesn't support the language we want
        if (specialChars[script]) {
            // need to check the charset, as the langset doesn't work for these scripts
            FcCharSet *cs;
            if (FcPatternGetCharSet(match, FC_CHARSET, 0, &cs) != FcResultMatch)
                goto done;
            if (!FcCharSetHasChar(cs, specialChars[script]))
                goto done;
        } else if (*specialLanguages[script] != '\0'){
            FcLangSet *langSet = 0;
            if (FcPatternGetLangSet(match, FC_LANG, 0, &langSet) != FcResultMatch)
                goto done;
            if (FcLangSetHasLang(langSet, (const FcChar8*)specialLanguages[script]) != FcLangEqual)
                goto done;
        }
    }

    // enforce non-antialiasing if requested. the ft font engine looks at this property.
    if (request.styleStrategy & QFont::NoAntialias) {
        FcPatternDel(match, FC_ANTIALIAS);
        FcPatternAddBool(match, FC_ANTIALIAS, false);
    }

    engine = new QFontEngineX11FT(match, qt_FcPatternToQFontDef(match, request), screen);
    if (engine->invalid()) {
        FM_DEBUG("   --> invalid!\n");
        delete engine;
        engine = 0;
    } else if (scriptRequiresOpenType(script)) {
        HB_Face hbFace = engine->harfbuzzFace();
        if (!hbFace || !hbFace->supported_scripts[script]) {
            FM_DEBUG("  OpenType support missing for script\n");
            delete engine;
            engine = 0;
        }
    }
done:
    return engine;
}

FcFontSet *qt_fontSetForPattern(FcPattern *pattern, const QFontDef &request)
{
    FcResult result;
    FcFontSet *fs = FcFontSort(0, pattern, FcTrue, 0, &result);
#ifdef FONT_MATCH_DEBUG
    FM_DEBUG("first font in fontset:\n");
    FcPatternPrint(fs->fonts[0]);
#endif

    FcBool forceScalable = request.styleStrategy & QFont::ForceOutline;

    // remove fonts if they are not scalable (and should be)
    if (forceScalable && fs) {
        for (int i = 0; i < fs->nfont; ++i) {
            FcPattern *font = fs->fonts[i];
            FcResult res;
            FcBool scalable;
            res = FcPatternGetBool(font, FC_SCALABLE, 0, &scalable);
            if (res != FcResultMatch || !scalable) {
                FcFontSetRemove(fs, i);
#ifdef FONT_MATCH_DEBUG
                FM_DEBUG("removing pattern:");
                FcPatternPrint(font);
#endif
                --i; // go back one
            }
        }
    }

    FM_DEBUG("final pattern contains %d fonts\n", fs->nfont);

    return fs;
}

static QFontEngine *loadFc(const QFontPrivate *fp, int script, const QFontDef &request)
{
    FM_DEBUG("===================== loadFc: script=%d family='%s'\n", script, request.family.toLatin1().data());
    FcPattern *pattern = getFcPattern(fp, script, request);

#ifdef FONT_MATCH_DEBUG
    FM_DEBUG("\n\nfinal FcPattern contains:\n");
    FcPatternPrint(pattern);
#endif

    QFontEngine *fe = 0;
    FcResult res;
    FcPattern *match = FcFontMatch(0, pattern, &res);
    fe = tryPatternLoad(match, fp->screen, request, script);
    if (!fe) {
        FcFontSet *fs = qt_fontSetForPattern(pattern, request);

        if (match) {
            FcPatternDestroy(match);
            match = 0;
        }

        if (fs) {
            for (int i = 0; !fe && i < fs->nfont; ++i) {
                match = FcFontRenderPrepare(NULL, pattern, fs->fonts[i]);
                fe = tryPatternLoad(match, fp->screen, request, script);
                if (fe)
                    break;
                FcPatternDestroy(match);
                match = 0;
            }
            FcFontSetDestroy(fs);
        }
        FM_DEBUG("engine for script %d is %s\n", script, fe ? fe->fontDef.family.toLatin1().data(): "(null)");
    }
    if (fe
        && script == QUnicodeTables::Common
        && !(request.styleStrategy & QFont::NoFontMerging) && !fe->symbol) {
        fe = new QFontEngineMultiFT(fe, match, pattern, fp->screen, request);
    } else {
        FcPatternDestroy(pattern);
    }
    if (match)
        FcPatternDestroy(match);
    return fe;
}

static FcPattern *queryFont(const FcChar8 *file, const QByteArray &data, int id, FcBlanks *blanks, int *count)
{
    if (data.isEmpty())
        return FcFreeTypeQuery(file, id, blanks, count);

    extern FT_Library qt_getFreetype();
    FT_Library lib = qt_getFreetype();

    FcPattern *pattern = 0;

    FT_Face face;
    if (!FT_New_Memory_Face(lib, (const FT_Byte *)data.constData(), data.size(), id, &face)) {
        *count = face->num_faces;

        pattern = FcFreeTypeQueryFace(face, file, id, blanks);

        FT_Done_Face(face);
    }

    return pattern;
}
#endif // QT_NO_FONTCONFIG


/*! \internal
  Loads a QFontEngine for the specified \a script that matches the
  QFontDef \e request member variable.
*/
void QFontDatabase::load(const QFontPrivate *d, int script)
{
    Q_ASSERT(script >= 0 && script < QUnicodeTables::ScriptCount);

    // normalize the request to get better caching
    QFontDef req = d->request;
    if (req.pixelSize <= 0)
        req.pixelSize = qFloor(qt_pixelSize(req.pointSize, d->dpi) * 100.0 + 0.5) * 0.01;
    if (req.pixelSize < 1)
        req.pixelSize = 1;

    if (req.weight == 0)
        req.weight = QFont::Normal;
    if (req.stretch == 0)
        req.stretch = 100;

    QFontCache::Key key(req, script, d->screen);
    if (!d->engineData)
        getEngineData(d, key);

    // the cached engineData could have already loaded the engine we want
    if (d->engineData->engines[script])
        return;

    // set it to the actual pointsize, so QFontInfo will do the right thing
    if (req.pointSize < 0)
        req.pointSize = qt_pointSize(req.pixelSize, d->dpi);


    QFontEngine *fe = QFontCache::instance()->findEngine(key);

    if (!fe) {
        QMutexLocker locker(fontDatabaseMutex());
        if (!privateDb()->count)
            initializeFontDb();

#ifdef QT_BUILD_INTERNAL
        if (qt_enable_test_font && req.family == QLatin1String("__Qt__Box__Engine__")) {
            fe = new QTestFontEngine(req.pixelSize);
            fe->fontDef = req;
        } else
#endif
#ifndef QT_NO_FONTCONFIG
        if (qt_x11Data->has_fontconfig) {
            fe = loadFc(d, script, req);
#endif
        }
        if (!fe) {
            fe = new QFontEngineBox(req.pixelSize);
            fe->fontDef = QFontDef();
        }
    }
    if (fe->symbol || (d->request.styleStrategy & QFont::NoFontMerging)) {
        for (int i = 0; i < QUnicodeTables::ScriptCount; ++i) {
            if (!d->engineData->engines[i]) {
                d->engineData->engines[i] = fe;
                fe->ref.ref();
            }
        }
    } else {
        d->engineData->engines[script] = fe;
        fe->ref.ref();
    }
    QFontCache::instance()->insertEngine(key, fe);
}

// Needed for fontconfig version < 2.2.97
#ifndef FC_FAMILYLANG
#define FC_FAMILYLANG "familylang"
#endif

static void registerFont(QFontDatabasePrivate::ApplicationFont *fnt)
{
#if defined(QT_NO_FONTCONFIG)
    return;
#else
    if (!qt_x11Data->has_fontconfig)
        return;

    FcConfig *config = FcConfigGetCurrent();
    if (!config)
        return;

    FcFontSet *set = FcConfigGetFonts(config, FcSetApplication);
    if (!set) {
        FcConfigAppFontAddFile(config, (const FcChar8 *)":/non-existent");
        set = FcConfigGetFonts(config, FcSetApplication); // try again
        if (!set)
            return;
    }

    QString fileNameForQuery = fnt->fileName;
    int id = 0;
    FcBlanks *blanks = FcConfigGetBlanks(0);
    int count = 0;

    QStringList families;
    QFontDatabasePrivate *db = privateDb();

    FcPattern *pattern = Q_NULLPTR;
    do {
        pattern = queryFont((const FcChar8 *)QFile::encodeName(fileNameForQuery).constData(),
                            fnt->data, id, blanks, &count);
        if (!pattern)
            return;

        FcPatternDel(pattern, FC_FILE);
        QByteArray cs = fnt->fileName.toUtf8();
        FcPatternAddString(pattern, FC_FILE, (const FcChar8 *) cs.constData());

        FcChar8 *fam = 0, *familylang = 0;
        int i, n = 0;
        for (i = 0; ; i++) {
            if (FcPatternGetString(pattern, FC_FAMILYLANG, i, &familylang) != FcResultMatch)
                break;
            QString familyLang = QString::fromUtf8((const char *) familylang);
            if (familyLang.compare(db->systemLang, Qt::CaseInsensitive) == 0) {
                n = i;
                break;
            }
        }

        if (FcPatternGetString(pattern, FC_FAMILY, n, &fam) == FcResultMatch) {
            QString family = QString::fromUtf8(reinterpret_cast<const char *>(fam));
            families << family;
        }

        if (!FcFontSetAdd(set, pattern))
            return;

        ++id;
    } while (pattern && id < count);

    fnt->families = families;
#endif
}

bool QFontDatabase::removeApplicationFont(int handle)
{
#if defined(QT_NO_FONTCONFIG)
    return false;
#else
    QMutexLocker locker(fontDatabaseMutex());

    QFontDatabasePrivate *db = privateDb();
    if (handle < 0 || handle >= db->applicationFonts.count())
        return false;

    FcConfigAppFontClear(0);

    db->applicationFonts[handle] = QFontDatabasePrivate::ApplicationFont();

    db->reregisterAppFonts = true;
    db->invalidate();
    return true;
#endif
}

bool QFontDatabase::removeAllApplicationFonts()
{
#if defined(QT_NO_FONTCONFIG)
    return false;
#else
    QMutexLocker locker(fontDatabaseMutex());

    QFontDatabasePrivate *db = privateDb();
    if (db->applicationFonts.isEmpty())
        return false;

    FcConfigAppFontClear(0);
    db->applicationFonts.clear();
    db->invalidate();
    return true;
#endif
}

bool QFontDatabase::supportsThreadedFontRendering()
{
#if defined(QT_NO_FONTCONFIG)
    return false;
#else
    return qt_x11Data->has_fontconfig;
#endif
}

QString QFontDatabase::resolveFontFamilyAlias(const QString &family)
{
#if defined(QT_NO_FONTCONFIG)
    return family;
#else
    FcPattern *pattern = FcPatternCreate();
    if (!pattern)
        return family;

    QByteArray cs = family.toUtf8();
    FcPatternAddString(pattern, FC_FAMILY, (const FcChar8 *) cs.constData());
    FcConfigSubstitute(0, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcChar8 *familyAfterSubstitution;
    FcPatternGetString(pattern, FC_FAMILY, 0, &familyAfterSubstitution);
    QString resolved = QString::fromUtf8((const char *) familyAfterSubstitution);
    FcPatternDestroy(pattern);

    return resolved;
#endif
}

QT_END_NAMESPACE

