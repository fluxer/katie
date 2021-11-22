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

#include "qplatformdefs.h"
#include "qdebug.h"
#include "qpaintdevice.h"
#include "qelapsedtimer.h"
#include "qfile.h"
#include "qtemporaryfile.h"
#include "qabstractfileengine.h"
#include "qmath.h"

#include "qx11info_x11.h"
#include "qfontengine_x11_p.h"

#ifndef QT_NO_FONTCONFIG
#include <ft2build.h>
#include FT_FREETYPE_H

#include <fontconfig/fcfreetype.h>
#endif

QT_BEGIN_NAMESPACE

// from qfont_x11.cpp
extern double qt_pointSize(double pixelSize, int dpi);
extern double qt_pixelSize(double pointSize, int dpi);

// generated via util/unicode/makeencodings
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
    { "fontspecific-*", 36, 0 },
};

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
    return XlfdTbl[id].name;
}
#endif

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
            dpi = QX11Info::appDpiY();
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

// values are from likelySubtags.xml distributed with Unicode CLDR data
static const char *specialLanguagesTbl[] = {
    "en", // Common
    "la", // Latin
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
    0, // Ethiopic
    "chr-us", // Cherokee
    "en-ca", // CanadianAboriginal
    "sga-ie", // Ogham
    "non-se", // Runic
    "km", // Khmer
    "mn", // Mongolian
    "ja-jp", // Hiragana
    "ja-jp", // Katakana
    "zh-tw", // Bopomofo
    "zh-tw", // Han
    "yi", // Yi
    "ett-it", // OldItalic
    "got-ua", // Gothic
    0, // Deseret
    0, // Inherited
    "tl-ph", // Tagalog
    "hnn-ph", // Hanunoo
    "bku-ph", // Buhid
    "tbw-ph", // Tagbanwa
    "lif", // Limbu
    "tdd-cn", // TaiLe
    "lab-gr", // LinearB
    "uga-sy", // Ugaritic
    "en-gb", // Shavian
    "so-so", // Osmanya
    "grc-cy", // Cypriot
    "fr-fr", // Braille
    "bug-id", // Buginese
    "cop-eg", // Coptic
    "khb-cn", // NewTaiLue
    "cu-bg", // Glagolitic
    0, // Tifinagh
    "syl-bd", // SylotiNagri
    "peo-ir", // OldPersian
    "pra-pk", // Kharoshthi
    "ban-id", // Balinese
    "akk-iq", // Cuneiform
    "phn-lb", // Phoenician
    "lzh-cn", // PhagsPa
    "nqo-gn", // Nko
    "su-id", // Sundanese
    "lep-in", // Lepcha
    "sat-in", // OlChiki
    "vai-lr", // Vai
    "saz-in", // Saurashtra
    "eky-mm", // KayahLi
    "rej-id", // Rejang
    "xlc-tr", // Lycian
    "xcr-tr", // Carian
    "xld-tr", // Lydian
    "cjm-vn", // Cham
    0, // TaiTham
    "blt-vn", // TaiViet
    "ae-ir", // Avestan
    "ar-eg", // EgyptianHieroglyphs
    "smp-il", // Samaritan
    "lis-cn", // Lisu
    "bax-cm", // Bamum
    "jv-id", // Javanese
    "mni-in", // MeeteiMayek
    "arc-ir", // ImperialAramaic
    "ar", // OldSouthArabian
    "xpr-ir", // InscriptionalParthian
    "pal-ir", // InscriptionalPahlavi
    "otk-mn", // OldTurkic
    "bho-in", // Kaithi
    "bbc-id", // Batak
    "pka-in", // Brahmi
    "myz-ir", // Mandaic
    "ccp-bd", // Chakma
    "xmr-sd", // MeroiticCursive
    "xmr-sd", // MeroiticHieroglyphs
    "hmd-cn", // Miao
    "sa-in", // Sharada
    "srb-in", // SoraSompeng
    "doi-in", // Takri
    "sq", // CaucasianAlbanian
    "bsq-lr", // BassaVah
    "fr-fr", // Duployan
    "sq-al", // Elbasan
    "sa-in", // Grantha
    "hnj-la", // PahawhHmong
    "sd-in", // Khojki
    "lab-gr", // LinearA
    "hi-in", // Mahajani
    "xmn-cn", // Manichaean
    0, // MendeKikakui
    "mr-in", // Modi
    "mro-bd", // Mro
    "ar", // OldNorthArabian
    "arc-jo", // Nabataean
    "arc-sy", // Palmyrene
    "ctd-mm", // PauCinHau
    "kv-ru", // OldPermic
    "pal-cn", // PsalterPahlavi
    "sa-in", // Siddham
    "sd-in", // Khudawadi
    "mai-in", // Tirhuta
    0, // WarangCiti
    "aho-in", // Ahom
    "hlu-tr", // AnatolianHieroglyphs
    "mis-iq", // Hatran
    "skr-pk", // Multani
    "hu-hu", // OldHungarian
    "ase-us", // SignWriting
    "ff-gn", // Adlam
    "sa-in", // Bhaiksuki
    "bo-cn", // Marchen
    "new-np", // Newa
    "osa-us", // Osage
    "txg-cn", // Tangut
    "esg-in", // MasaramGondi
    "zhx-cn", // Nushu
    "cmg-mn", // Soyombo
    "cmg-mn", // ZanabazarSquare
    "doi-in", // Dogra
    "wsg-in", // GunjalaGondi
    "mak-id", // Makasar
    "mis-ng", // Medefaidrin
    "rhg-mm", // HanifiRohingya
    "sog-uz", // Sogdian
    "sog-uz", // OldSogdian
    "arc-ir", // Elymaic
    "sa-in", // Nandinagari
    "mww-us", // NyiakengPuachueHmong
    "npp-in", // Wancho
    "xco-uz", // Chorasmian
    "dv-mv", // DivesAkuru
    "zkt-cn", // KhitanSmallScript
    "ku-ge", // Yezidi
    0, // CyproMinoan
    "ug-cn", // OldUyghur
    "ntm-in", // Tangsa
    "txo-in", // Toto
    "sq-al", // Vithkuqi
};
enum { SpecialLanguageCount = sizeof(specialLanguagesTbl) / sizeof(const char *) };

// values obtained via genutf script
static const uint specialCharsTbl[] = {
    0x0, // Common
    0x0041, // Latin
    0x0370, // Greek
    0x0400, // Cyrillic
    0x0531, // Armenian
    0x0591, // Hebrew
    0x0600, // Arabic
    0x0700, // Syriac
    0x0780, // Thaana
    0x0900, // Devanagari
    0x0980, // Bengali
    0x0A01, // Gurmukhi
    0x0A81, // Gujarati
    0x0B01, // Oriya
    0x0B82, // Tamil
    0x0C00, // Telugu
    0x0C80, // Kannada
    0x0D00, // Malayalam
    0x0D81, // Sinhala
    0x0E01, // Thai
    0x0E81, // Lao
    0x0F00, // Tibetan
    0x1000, // Myanmar
    0x10A0, // Georgian
    0x1100, // Hangul
    0x1200, // Ethiopic
    0x13A0, // Cherokee
    0x1400, // CanadianAboriginal
    0x1680, // Ogham
    0x16A0, // Runic
    0x1780, // Khmer
    0x1800, // Mongolian
    0x3041, // Hiragana
    0x30A1, // Katakana
    0x02EA, // Bopomofo
    0x2E80, // Han
    0xA000, // Yi
    0x10300, // OldItalic
    0x10330, // Gothic
    0x10400, // Deseret
    0x0, // Inherited
    0x1700, // Tagalog
    0x1720, // Hanunoo
    0x1740, // Buhid
    0x1760, // Tagbanwa
    0x1900, // Limbu
    0x1950, // TaiLe
    0x10000, // LinearB
    0x10380, // Ugaritic
    0x10450, // Shavian
    0x10480, // Osmanya
    0x10800, // Cypriot
    0x2800, // Braille
    0x1A00, // Buginese
    0x03E2, // Coptic
    0x1980, // NewTaiLue
    0x2C00, // Glagolitic
    0x2D30, // Tifinagh
    0xA800, // SylotiNagri
    0x103A0, // OldPersian
    0x10A00, // Kharoshthi
    0x1B00, // Balinese
    0x12000, // Cuneiform
    0x10900, // Phoenician
    0xA840, // PhagsPa
    0x07C0, // Nko
    0x1B80, // Sundanese
    0x1C00, // Lepcha
    0x1C50, // OlChiki
    0xA500, // Vai
    0xA880, // Saurashtra
    0xA900, // KayahLi
    0xA930, // Rejang
    0x10280, // Lycian
    0x102A0, // Carian
    0x10920, // Lydian
    0xAA00, // Cham
    0x1A20, // TaiTham
    0xAA80, // TaiViet
    0x10B00, // Avestan
    0x13000, // EgyptianHieroglyphs
    0x0800, // Samaritan
    0xA4D0, // Lisu
    0xA6A0, // Bamum
    0xA980, // Javanese
    0xAAE0, // MeeteiMayek
    0x10840, // ImperialAramaic
    0x10A60, // OldSouthArabian
    0x10B40, // InscriptionalParthian
    0x10B60, // InscriptionalPahlavi
    0x10C00, // OldTurkic
    0x11080, // Kaithi
    0x1BC0, // Batak
    0x11000, // Brahmi
    0x0840, // Mandaic
    0x11100, // Chakma
    0x109A0, // MeroiticCursive
    0x10980, // MeroiticHieroglyphs
    0x16F00, // Miao
    0x11180, // Sharada
    0x110D0, // SoraSompeng
    0x11680, // Takri
    0x10530, // CaucasianAlbanian
    0x16AD0, // BassaVah
    0x1BC00, // Duployan
    0x10500, // Elbasan
    0x11300, // Grantha
    0x16B00, // PahawhHmong
    0x11200, // Khojki
    0x10600, // LinearA
    0x11150, // Mahajani
    0x10AC0, // Manichaean
    0x1E800, // MendeKikakui
    0x11600, // Modi
    0x16A40, // Mro
    0x10A80, // OldNorthArabian
    0x10880, // Nabataean
    0x10860, // Palmyrene
    0x11AC0, // PauCinHau
    0x10350, // OldPermic
    0x10B80, // PsalterPahlavi
    0x11580, // Siddham
    0x112B0, // Khudawadi
    0x11480, // Tirhuta
    0x118A0, // WarangCiti
    0x11700, // Ahom
    0x14400, // AnatolianHieroglyphs
    0x108E0, // Hatran
    0x11280, // Multani
    0x10C80, // OldHungarian
    0x1D800, // SignWriting
    0x1E900, // Adlam
    0x11C00, // Bhaiksuki
    0x11C70, // Marchen
    0x11400, // Newa
    0x104B0, // Osage
    0x16FE0, // Tangut
    0x11D00, // MasaramGondi
    0x16FE1, // Nushu
    0x11A50, // Soyombo
    0x11A00, // ZanabazarSquare
    0x11800, // Dogra
    0x11D60, // GunjalaGondi
    0x11EE0, // Makasar
    0x16E40, // Medefaidrin
    0x10D00, // HanifiRohingya
    0x10F30, // Sogdian
    0x10F00, // OldSogdian
    0x10FE0, // Elymaic
    0x119A0, // Nandinagari
    0x1E100, // NyiakengPuachueHmong
    0x1E2C0, // Wancho
    0x10FB0, // Chorasmian
    0x11900, // DivesAkuru
    0x16FE4, // KhitanSmallScript
    0x10E80, // Yezidi
    0x12F90, // CyproMinoan
    0x10F70, // OldUyghur
    0x16A70, // Tangsa
    0x1E290, // Toto
    0x10570, // Vithkuqi
};
enum { SpecialCharCount = sizeof(specialCharsTbl) / sizeof(uint) };

static const struct DefaultFontTblData {
    const QLatin1String name;
    const bool fixedpitch;
} DefaultFontTbl[] = {
    { QLatin1String("Serif"), false },
    { QLatin1String("Sans Serif"), false },
    { QLatin1String("Monospace"), true },
};
static const qint16 DefaultFontTblSize = sizeof(DefaultFontTbl) / sizeof(DefaultFontTblData);

static const char* PatternPropertiesTbl[] = {
    FC_FAMILY,
    FC_STYLE,
    FC_WEIGHT,
    FC_SLANT,
    FC_SPACING,
    FC_LANG,
    FC_CHARSET,
    FC_FOUNDRY,
    FC_SCALABLE,
    FC_PIXEL_SIZE,
    FC_WIDTH,
};
static const qint16 PatternPropertiesTblSize = 11;

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
    FcChar8 *foundry_value;
    FcChar8 *style_value;
    FcBool scalable;

    {
        FcObjectSet *os = FcObjectSetCreate();
        FcPattern *pattern = FcPatternCreate();
        for (qint16 i = 0; i < PatternPropertiesTblSize; i++) {
            FcObjectSetAdd(os, PatternPropertiesTbl[i]);
        }
        fonts = FcFontList(0, pattern, os);
        FcObjectSetDestroy(os);
        FcPatternDestroy(pattern);
    }

    for (int i = 0; i < fonts->nfont; i++) {
        if (FcPatternGetString(fonts->fonts[i], FC_FAMILY, 0, &value) != FcResultMatch)
            continue;
        familyName = QString::fromUtf8((const char *)value);
        slant_value = FC_SLANT_ROMAN;
        weight_value = FC_WEIGHT_MEDIUM;
        spacing_value = FC_PROPORTIONAL;
        scalable = FcTrue;

        if (FcPatternGetInteger (fonts->fonts[i], FC_SLANT, 0, &slant_value) != FcResultMatch)
            slant_value = FC_SLANT_ROMAN;
        if (FcPatternGetInteger (fonts->fonts[i], FC_WEIGHT, 0, &weight_value) != FcResultMatch)
            weight_value = FC_WEIGHT_MEDIUM;
        if (FcPatternGetInteger (fonts->fonts[i], FC_SPACING, 0, &spacing_value) != FcResultMatch)
            spacing_value = FC_PROPORTIONAL;
        if (FcPatternGetBool(fonts->fonts[i], FC_SCALABLE, 0, &scalable) != FcResultMatch)
            scalable = FcTrue;
        if (FcPatternGetString(fonts->fonts[i], FC_FOUNDRY, 0, &foundry_value) != FcResultMatch)
            foundry_value = 0;
        if (FcPatternGetString(fonts->fonts[i], FC_STYLE, 0, &style_value) != FcResultMatch)
            style_value = 0;
        QtFontFamily *family = db->family(familyName, true);

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
        QtFontEncoding *enc = size->encodingID(-1, true);
        enc->pitch = (spacing_value >= FC_CHARCELL ? 'c' :
                      (spacing_value >= FC_MONO ? 'm' : 'p'));
    }

    FcFontSetDestroy (fonts);

    for (qint16 i = 0; i < DefaultFontTblSize; i++) {
        QtFontFamily *family = db->family(DefaultFontTbl[i].name, true);
        family->fixedPitch = DefaultFontTbl[i].fixedpitch;
        QtFontFoundry *foundry = family->foundry(QString(), true);

        QtFontStyle::Key styleKey;
        for (int j = 0; j < 4; j++) {
            styleKey.style = (j%2) ? QFont::StyleNormal : QFont::StyleItalic;
            styleKey.weight = (j > 1) ? QFont::Bold : QFont::Normal;
            QtFontStyle *style = foundry->style(styleKey, QString(), true);
            style->smoothScalable = true;
            QtFontSize *size = style->pixelSize(SMOOTH_SCALABLE, true);
            QtFontEncoding *enc = size->encodingID(-1, true);
            enc->pitch = (DefaultFontTbl[i].fixedpitch ? 'm' : 'p');
        }
    }
}
#endif // QT_NO_FONTCONFIG

static void registerFont(QFontDatabasePrivate::ApplicationFont *fnt);
static QString styleStringHelper(int weight, QFont::Style style);

static void initializeFontDb()
{
    QFontDatabasePrivate *db = privateDb();
    if (!db || db->count)
        return;

#ifndef QT_NO_FONTCONFIG
#ifdef QFONTDATABASE_DEBUG
    QElapsedTimer elapsedtimer;
    elapsedtimer.start();
#endif
    if (db->reregisterAppFonts) {
        db->reregisterAppFonts = false;
        for (int i = 0; i < db->applicationFonts.count(); ++i)
            if (!db->applicationFonts.at(i).families.isEmpty()) {
                registerFont(&db->applicationFonts[i]);
            }
    }

    loadFontConfig();
#ifdef QFONTDATABASE_DEBUG
    FD_DEBUG("QFontDatabase: loaded FontConfig: %d ms", int(elapsedtimer.elapsed()));
#endif
#endif

#ifndef QT_NO_FONTCONFIG
    for (int i = 0; i < db->count; i++) {
        for (int j = 0; j < db->families[i]->count; ++j) {        // each foundry
            QtFontFoundry *foundry = db->families[i]->foundries[j];
            for (int k = 0; k < foundry->count; ++k) {
                QtFontStyle *style = foundry->styles[k];
                if (style->key.style != QFont::StyleNormal) continue;

                QtFontSize *size = style->pixelSize(SMOOTH_SCALABLE);
                if (! size) continue; // should not happen
                QtFontEncoding *enc = size->encodingID(-1, true);
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
                QtFontEncoding *equiv_enc = equiv_size->encodingID(-1, true);

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
        FD_DEBUG("'%s' %s  fixed=%s", family->name.toLatin1().constData(), (family->fixedPitch ? "fixed" : ""),
                 (family->fixedPitch ? "yes" : "no"));

        for (int fd = 0; fd < family->count; fd++) {
            QtFontFoundry *foundry = family->foundries[fd];
            FD_DEBUG("\t\t'%s'", foundry->name.toLatin1().constData());
            for (int s = 0; s < foundry->count; s++) {
                QtFontStyle *style = foundry->styles[s];
                FD_DEBUG("\t\t\tstyle: style=%d weight=%d stretch=%d",
                         style->key.style, style->key.weight, style->key.stretch);
                if (style->smoothScalable)
                    FD_DEBUG("\t\t\t\tsmooth scalable");
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

#ifndef QT_NO_FONTCONFIG
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

static void qt_addPatternProps(FcPattern *pattern, int screen, QUnicodeTables::Script script, const QFontDef &request)
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

    if (script != QUnicodeTables::Common && specialLanguagesTbl[script]) {
        Q_ASSERT(script < QUnicodeTables::ScriptCount);
        FcLangSet *ls = FcLangSetCreate();
        FcLangSetAdd(ls, (const FcChar8*)specialLanguagesTbl[script]);
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


static FcPattern *getFcPattern(const QFontPrivate *fp, QUnicodeTables::Script script, const QFontDef &request)
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
            QT_PREPEND_NAMESPACE(match)(script, request, family, foundry, &desc);
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
                                   const QFontDef &request,
                                   QUnicodeTables::Script script)
{
#ifdef FONT_MATCH_DEBUG
    FcChar8 *fam;
    FcPatternGetString(match, FC_FAMILY, 0, &fam);
    FM_DEBUG("==== trying %s\n", fam);
#endif
    FM_DEBUG("passes charset test\n");

    if (!match) // probably no fonts available.
        return nullptr;

    if (script != QUnicodeTables::Common) {
        // skip font if it doesn't support the language we want
        if (specialLanguagesTbl[script]){
            FcLangSet *langSet = 0;
            if (FcPatternGetLangSet(match, FC_LANG, 0, &langSet) != FcResultMatch)
                goto special_char;
            if (FcLangSetHasLang(langSet, (const FcChar8*)specialLanguagesTbl[script]) != FcLangEqual)
                goto special_char;
        }
special_char:
        if (specialCharsTbl[script]) {
            // need to check the charset, as the langset doesn't work for some scripts
            FcCharSet *cs;
            if (FcPatternGetCharSet(match, FC_CHARSET, 0, &cs) != FcResultMatch)
                return nullptr;
            if (!FcCharSetHasChar(cs, specialCharsTbl[script]))
                return nullptr;
        }
    }

    // enforce non-antialiasing if requested. the ft font engine looks at this property.
    if (request.styleStrategy & QFont::NoAntialias) {
        FcPatternDel(match, FC_ANTIALIAS);
        FcPatternAddBool(match, FC_ANTIALIAS, false);
    }

    QFontEngineX11FT *engine = new QFontEngineX11FT(match, qt_FcPatternToQFontDef(match, request), screen);
    if (engine->invalid()) {
        FM_DEBUG("   --> invalid!\n");
        delete engine;
        engine = nullptr;
    }
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

static QFontEngine *loadFc(const QFontPrivate *fp, QUnicodeTables::Script script, const QFontDef &request)
{
    FM_DEBUG("===================== loadFc: script=%d family='%s'\n", script, request.family.toLatin1().constData());
    FcPattern *pattern = getFcPattern(fp, script, request);

#ifdef FONT_MATCH_DEBUG
    FM_DEBUG("\n\nfinal FcPattern contains:\n");
    FcPatternPrint(pattern);
#endif

    FcResult res;
    FcPattern *match = FcFontMatch(0, pattern, &res);
    QFontEngine *fe = tryPatternLoad(match, fp->screen, request, script);
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
        FM_DEBUG("engine for script %d is %s\n", script, fe ? fe->fontDef.family.toLatin1().constData(): "(null)");
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

    FcPattern *pattern = nullptr;

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
        std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);
        if (!privateDb()->count)
            initializeFontDb();

#ifndef QT_NO_FONTCONFIG
        if (qt_x11Data->has_fontconfig) {
            fe = loadFc(d, static_cast<QUnicodeTables::Script>(script), req);
        }
#endif
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

    int id = 0;
    FcBlanks *blanks = FcConfigGetBlanks(0);
    int count = 0;

    QStringList families;
    QFontDatabasePrivate *db = privateDb();

    do {
        FcPattern *pattern = queryFont((const FcChar8 *)QFile::encodeName(fnt->fileName).constData(),
                            fnt->data, id, blanks, &count);
        if (!pattern)
            return;

        FcPatternDel(pattern, FC_FILE);
        QByteArray cs = fnt->fileName.toUtf8();
        FcPatternAddString(pattern, FC_FILE, (const FcChar8 *) cs.constData());

        int n = 0;
        for (int i = 0; ; i++) {
            FcChar8 *familylang = nullptr;
            if (FcPatternGetString(pattern, FC_FAMILYLANG, i, &familylang) != FcResultMatch)
                break;
            QString familyLang = QString::fromUtf8((const char *) familylang);
            if (familyLang.compare(db->systemLang, Qt::CaseInsensitive) == 0) {
                n = i;
                break;
            }
        }

        FcChar8 *fam = nullptr;
        if (FcPatternGetString(pattern, FC_FAMILY, n, &fam) == FcResultMatch) {
            families << QString::fromUtf8(reinterpret_cast<const char *>(fam));
        }

        if (!FcFontSetAdd(set, pattern))
            return;

        ++id;
    } while (id < count);

    fnt->families = families;
#endif
}

bool QFontDatabase::removeApplicationFont(int handle)
{
#if defined(QT_NO_FONTCONFIG)
    return false;
#else
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

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
    std::lock_guard<std::recursive_mutex> locker(qGlobalFontDatabaseMutex);

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

