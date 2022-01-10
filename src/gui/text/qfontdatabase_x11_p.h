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
#include "qfile.h"
#include "qmath.h"
#include "qfontengine_ft_p.h"
#include "qx11info_x11.h"

#ifndef QT_NO_FONTCONFIG
#include <fontconfig/fcfreetype.h>
#endif

QT_BEGIN_NAMESPACE

// from qfont_x11.cpp
extern double qt_pointSize(double pixelSize, int dpi);
extern double qt_pixelSize(double pointSize, int dpi);

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
#endif

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
    "ff-gn", // Adlam
    "aho-in", // Ahom
    "hlu-tr", // AnatolianHieroglyphs
    "ar", // Arabic
    "hy", // Armenian
    "ae-ir", // Avestan
    "ban-id", // Balinese
    "bax-cm", // Bamum
    "bsq-lr", // BassaVah
    "bbc-id", // Batak
    "bn", // Bengali
    "sa-in", // Bhaiksuki
    "zh-tw", // Bopomofo
    "pka-in", // Brahmi
    "fr-fr", // Braille
    "bug-id", // Buginese
    "bku-ph", // Buhid
    "en-ca", // CanadianAboriginal
    "xcr-tr", // Carian
    "sq", // CaucasianAlbanian
    "ccp-bd", // Chakma
    "cjm-vn", // Cham
    "chr-us", // Cherokee
    "xco-uz", // Chorasmian
    "cop-eg", // Coptic
    "akk-iq", // Cuneiform
    "grc-cy", // Cypriot
    0, // CyproMinoan
    "ru", // Cyrillic
    0, // Deseret
    "hi", // Devanagari
    "dv-mv", // DivesAkuru
    "doi-in", // Dogra
    "fr-fr", // Duployan
    "ar-eg", // EgyptianHieroglyphs
    "sq-al", // Elbasan
    "arc-ir", // Elymaic
    0, // Ethiopic
    "ka", // Georgian
    "cu-bg", // Glagolitic
    "got-ua", // Gothic
    "sa-in", // Grantha
    "el", // Greek
    "gu", // Gujarati
    "wsg-in", // GunjalaGondi
    "pa", // Gurmukhi
    "zh-tw", // Han
    "ko", // Hangul
    "rhg-mm", // HanifiRohingya
    "hnn-ph", // Hanunoo
    "mis-iq", // Hatran
    "he", // Hebrew
    "ja-jp", // Hiragana
    "arc-ir", // ImperialAramaic
    0, // Inherited
    "pal-ir", // InscriptionalPahlavi
    "xpr-ir", // InscriptionalParthian
    "jv-id", // Javanese
    "bho-in", // Kaithi
    "kn", // Kannada
    "ja-jp", // Katakana
    "eky-mm", // KayahLi
    "pra-pk", // Kharoshthi
    "zkt-cn", // KhitanSmallScript
    "km", // Khmer
    "sd-in", // Khojki
    "sd-in", // Khudawadi
    "lo", // Lao
    "la", // Latin
    "lep-in", // Lepcha
    "lif", // Limbu
    "lab-gr", // LinearA
    "lab-gr", // LinearB
    "lis-cn", // Lisu
    "xlc-tr", // Lycian
    "xld-tr", // Lydian
    "hi-in", // Mahajani
    "mak-id", // Makasar
    "ml", // Malayalam
    "myz-ir", // Mandaic
    "xmn-cn", // Manichaean
    "bo-cn", // Marchen
    "esg-in", // MasaramGondi
    "mis-ng", // Medefaidrin
    "mni-in", // MeeteiMayek
    0, // MendeKikakui
    "xmr-sd", // MeroiticCursive
    "xmr-sd", // MeroiticHieroglyphs
    "hmd-cn", // Miao
    "mr-in", // Modi
    "mn", // Mongolian
    "mro-bd", // Mro
    "skr-pk", // Multani
    "my", // Myanmar
    "arc-jo", // Nabataean
    "sa-in", // Nandinagari
    "new-np", // Newa
    "khb-cn", // NewTaiLue
    "nqo-gn", // Nko
    "zhx-cn", // Nushu
    "mww-us", // NyiakengPuachueHmong
    "sga-ie", // Ogham
    "sat-in", // OlChiki
    "hu-hu", // OldHungarian
    "ett-it", // OldItalic
    "ar", // OldNorthArabian
    "kv-ru", // OldPermic
    "peo-ir", // OldPersian
    "sog-uz", // OldSogdian
    "ar", // OldSouthArabian
    "otk-mn", // OldTurkic
    "ug-cn", // OldUyghur
    "or", // Oriya
    "osa-us", // Osage
    "so-so", // Osmanya
    "hnj-la", // PahawhHmong
    "arc-sy", // Palmyrene
    "ctd-mm", // PauCinHau
    "lzh-cn", // PhagsPa
    "phn-lb", // Phoenician
    "pal-cn", // PsalterPahlavi
    "rej-id", // Rejang
    "non-se", // Runic
    "smp-il", // Samaritan
    "saz-in", // Saurashtra
    "sa-in", // Sharada
    "en-gb", // Shavian
    "sa-in", // Siddham
    "ase-us", // SignWriting
    "si", // Sinhala
    "sog-uz", // Sogdian
    "srb-in", // SoraSompeng
    "cmg-mn", // Soyombo
    "su-id", // Sundanese
    "syl-bd", // SylotiNagri
    "syr", // Syriac
    "tl-ph", // Tagalog
    "tbw-ph", // Tagbanwa
    "tdd-cn", // TaiLe
    0, // TaiTham
    "blt-vn", // TaiViet
    "doi-in", // Takri
    "ta", // Tamil
    "ntm-in", // Tangsa
    "txg-cn", // Tangut
    "te", // Telugu
    "div", // Thaana
    "th", // Thai
    "bo", // Tibetan
    0, // Tifinagh
    "mai-in", // Tirhuta
    "txo-in", // Toto
    "uga-sy", // Ugaritic
    "vai-lr", // Vai
    "sq-al", // Vithkuqi
    "npp-in", // Wancho
    0, // WarangCiti
    "ku-ge", // Yezidi
    "yi", // Yi
    "cmg-mn", // ZanabazarSquare
};
enum { SpecialLanguageCount = sizeof(specialLanguagesTbl) / sizeof(const char *) };

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

// --------------------------------------------------------------------------------------
// font loader
// --------------------------------------------------------------------------------------

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

static FcPattern *getFcPattern(const QFontPrivate *fp, QUnicodeTables::Script script, const QFontDef &request)
{
    if (!qt_x11Data->has_fontconfig)
        return 0;

    FcPattern *pattern = FcPatternCreate();
    if (!pattern)
        return 0;

    FcValue value;
    value.type = FcTypeString;

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
            if (!foundry.isEmpty()) {
                QByteArray cs = foundry.toUtf8();
                value.u.s = (const FcChar8 *)cs.data();
                FcPatternAddWeak(pattern, FC_FOUNDRY, value, FcTrue);
            }
        }
    }

    if (!request.ignorePitch) {
        char pitch_value = FC_PROPORTIONAL;
        if (request.fixedPitch)
            pitch_value = FC_MONO;
        FcPatternAddInteger(pattern, FC_SPACING, pitch_value);
    }
    FcPatternAddBool(pattern, FC_OUTLINE, !(request.styleStrategy & QFont::PreferBitmap));

    if (request.styleStrategy & (QFont::PreferOutline|QFont::PreferAntialias)) {
        FcPatternAddBool(pattern, FC_SCALABLE, true);
    }

    qt_addPatternProps(pattern, fp->screen, script, request);

    FcConfigSubstitute(0, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    // add the default family
    QString defaultFamily = QApplication::font().family();
    QByteArray cs = defaultFamily.toUtf8();
    value.u.s = (const FcChar8 *)cs.data();
    FcPatternAddWeak(pattern, FC_FAMILY, value, FcTrue);

    // add QFont::lastResortFamily() to the list, for compatibility with
    // previous versions
    defaultFamily = QFont::lastResortFamily();
    cs = defaultFamily.toUtf8();
    value.u.s = (const FcChar8 *)cs.data();
    FcPatternAddWeak(pattern, FC_FAMILY, value, FcTrue);

    return pattern;
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
    FcPattern *pattern = getFcPattern(fp, script, request);

#ifdef FONT_MATCH_DEBUG
    FM_DEBUG("\n\nfinal FcPattern contains:\n");
    FcPatternPrint(pattern);
#endif

    FcResult res;
    FcPattern *match = FcFontMatch(0, pattern, &res);
    QFontEngine *fe = tryPatternLoad(match, fp->screen, request, script);
    if (!fe) {
        FcResult unused;
        FcFontSet *fs = FcFontSort(0, pattern, FcTrue, 0, &unused);

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
    FcPatternDestroy(pattern);
    if (match) {
        FcPatternDestroy(match);
    }
    return fe;
}
#endif // QT_NO_FONTCONFIG


/*! \internal
  Loads a QFontEngine for the specified \a script that matches the
  QFontDef \e request member variable.
*/
QFontEngine* QFontDatabase::load(const QFontPrivate *d, int script)
{
    Q_ASSERT(script >= 0 && script < QUnicodeTables::ScriptCount);

    // normalize the request to get better caching
    QFontDef req = d->request;
    if (req.pixelSize <= 0) {
        req.pixelSize = qFloor(qt_pixelSize(req.pointSize, d->dpi) * 100.0 + 0.5) * 0.01;
    }
    if (req.pixelSize < 1) {
        req.pixelSize = 1;
    }

    if (req.weight == 0) {
        req.weight = QFont::Normal;
    }
    if (req.stretch == 0) {
        req.stretch = 100;
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

    if (QFile::exists(req.family)) {
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
        fe->fontDef = QFontDef();
    }
    QFontCache::instance()->insertEngine(key, fe);
    return fe;
}

QT_END_NAMESPACE

