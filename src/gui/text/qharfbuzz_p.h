/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QHARFBUZZ_P_H
#define QHARFBUZZ_P_H

#include <QtCore/qglobal.h>

#include <cassert>

QT_BEGIN_NAMESPACE

class QFontEngine;

/*
 * This was part of HarfBuzz, an OpenType Layout engine library.
 *
 * Copyright (C) 1998-2004  David Turner and Werner Lemberg
 * Copyright (C) 2006  Behdad Esfahbod
 * Copyright (C) 2007  Red Hat, Inc.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#define HB_MIN(a, b) ((a) < (b) ? (a) : (b))
#define HB_MAX(a, b) ((a) > (b) ? (a) : (b))

#define HB_IsControlChar(uc) \
    ((uc >= 0x200b && uc <= 0x200f /* ZW Space, ZWNJ, ZWJ, LRM and RLM */) \
     || (uc >= 0x2028 && uc <= 0x202f /* LS, PS, LRE, RLE, PDF, LRO, RLO, NNBSP */) \
     || (uc >= 0x206a && uc <= 0x206f /* ISS, ASS, IAFS, AFS, NADS, NODS */))

#define HB_FIXED_CONSTANT(v) ((v) * 64)

#define HB_IsHighSurrogate(ucs) \
    (((ucs) & 0xfc00) == 0xd800)

#define HB_IsLowSurrogate(ucs) \
    (((ucs) & 0xfc00) == 0xdc00)

#define HB_SurrogateToUcs4(high, low) \
    (((HB_UChar32)(high))<<10) + (low) - 0x35fdc00;

typedef uint32_t HB_Glyph;
typedef uint16_t HB_UChar16;
typedef uint32_t HB_UChar32;
typedef int32_t HB_Fixed; /* 26.6 */

typedef enum {
    HB_FontAscent
} HB_FontMetric;

typedef enum {
    HB_Script_Common,
    HB_Script_Adlam,
    HB_Script_Ahom,
    HB_Script_AnatolianHieroglyphs,
    HB_Script_Arabic,
    HB_Script_Armenian,
    HB_Script_Avestan,
    HB_Script_Balinese,
    HB_Script_Bamum,
    HB_Script_BassaVah,
    HB_Script_Batak,
    HB_Script_Bengali,
    HB_Script_Bhaiksuki,
    HB_Script_Bopomofo,
    HB_Script_Brahmi,
    HB_Script_Braille,
    HB_Script_Buginese,
    HB_Script_Buhid,
    HB_Script_CanadianAboriginal,
    HB_Script_Carian,
    HB_Script_CaucasianAlbanian,
    HB_Script_Chakma,
    HB_Script_Cham,
    HB_Script_Cherokee,
    HB_Script_Chorasmian,
    HB_Script_Coptic,
    HB_Script_Cuneiform,
    HB_Script_Cypriot,
    HB_Script_CyproMinoan,
    HB_Script_Cyrillic,
    HB_Script_Deseret,
    HB_Script_Devanagari,
    HB_Script_DivesAkuru,
    HB_Script_Dogra,
    HB_Script_Duployan,
    HB_Script_EgyptianHieroglyphs,
    HB_Script_Elbasan,
    HB_Script_Elymaic,
    HB_Script_Ethiopic,
    HB_Script_Georgian,
    HB_Script_Glagolitic,
    HB_Script_Gothic,
    HB_Script_Grantha,
    HB_Script_Greek,
    HB_Script_Gujarati,
    HB_Script_GunjalaGondi,
    HB_Script_Gurmukhi,
    HB_Script_Han,
    HB_Script_Hangul,
    HB_Script_HanifiRohingya,
    HB_Script_Hanunoo,
    HB_Script_Hatran,
    HB_Script_Hebrew,
    HB_Script_Hiragana,
    HB_Script_ImperialAramaic,
    HB_Script_Inherited,
    HB_Script_InscriptionalPahlavi,
    HB_Script_InscriptionalParthian,
    HB_Script_Javanese,
    HB_Script_Kaithi,
    HB_Script_Kannada,
    HB_Script_Katakana,
    HB_Script_KayahLi,
    HB_Script_Kharoshthi,
    HB_Script_KhitanSmallScript,
    HB_Script_Khmer,
    HB_Script_Khojki,
    HB_Script_Khudawadi,
    HB_Script_Lao,
    HB_Script_Latin,
    HB_Script_Lepcha,
    HB_Script_Limbu,
    HB_Script_LinearA,
    HB_Script_LinearB,
    HB_Script_Lisu,
    HB_Script_Lycian,
    HB_Script_Lydian,
    HB_Script_Mahajani,
    HB_Script_Makasar,
    HB_Script_Malayalam,
    HB_Script_Mandaic,
    HB_Script_Manichaean,
    HB_Script_Marchen,
    HB_Script_MasaramGondi,
    HB_Script_Medefaidrin,
    HB_Script_MeeteiMayek,
    HB_Script_MendeKikakui,
    HB_Script_MeroiticCursive,
    HB_Script_MeroiticHieroglyphs,
    HB_Script_Miao,
    HB_Script_Modi,
    HB_Script_Mongolian,
    HB_Script_Mro,
    HB_Script_Multani,
    HB_Script_Myanmar,
    HB_Script_Nabataean,
    HB_Script_Nandinagari,
    HB_Script_Newa,
    HB_Script_NewTaiLue,
    HB_Script_Nko,
    HB_Script_Nushu,
    HB_Script_NyiakengPuachueHmong,
    HB_Script_Ogham,
    HB_Script_OlChiki,
    HB_Script_OldHungarian,
    HB_Script_OldItalic,
    HB_Script_OldNorthArabian,
    HB_Script_OldPermic,
    HB_Script_OldPersian,
    HB_Script_OldSogdian,
    HB_Script_OldSouthArabian,
    HB_Script_OldTurkic,
    HB_Script_OldUyghur,
    HB_Script_Oriya,
    HB_Script_Osage,
    HB_Script_Osmanya,
    HB_Script_PahawhHmong,
    HB_Script_Palmyrene,
    HB_Script_PauCinHau,
    HB_Script_PhagsPa,
    HB_Script_Phoenician,
    HB_Script_PsalterPahlavi,
    HB_Script_Rejang,
    HB_Script_Runic,
    HB_Script_Samaritan,
    HB_Script_Saurashtra,
    HB_Script_Sharada,
    HB_Script_Shavian,
    HB_Script_Siddham,
    HB_Script_SignWriting,
    HB_Script_Sinhala,
    HB_Script_Sogdian,
    HB_Script_SoraSompeng,
    HB_Script_Soyombo,
    HB_Script_Sundanese,
    HB_Script_SylotiNagri,
    HB_Script_Syriac,
    HB_Script_Tagalog,
    HB_Script_Tagbanwa,
    HB_Script_TaiLe,
    HB_Script_TaiTham,
    HB_Script_TaiViet,
    HB_Script_Takri,
    HB_Script_Tamil,
    HB_Script_Tangsa,
    HB_Script_Tangut,
    HB_Script_Telugu,
    HB_Script_Thaana,
    HB_Script_Thai,
    HB_Script_Tibetan,
    HB_Script_Tifinagh,
    HB_Script_Tirhuta,
    HB_Script_Toto,
    HB_Script_Ugaritic,
    HB_Script_Vai,
    HB_Script_Vithkuqi,
    HB_Script_Wancho,
    HB_Script_WarangCiti,
    HB_Script_Yezidi,
    HB_Script_Yi,
    HB_Script_ZanabazarSquare,
    HB_ScriptCount = HB_Script_ZanabazarSquare
} HB_Script;

typedef enum {
    HB_Combining_NotOrdered = 0,
    HB_Combining_AttachedBelow = 202,
    HB_Combining_AttachedAbove = 214,
    HB_Combining_AttachedAboveRight = 216,
    HB_Combining_BelowLeft = 218,
    HB_Combining_Below = 220,
    HB_Combining_BelowRight = 222,
    HB_Combining_Left = 224,
    HB_Combining_Right = 226,
    HB_Combining_AboveLeft = 228,
    HB_Combining_Above = 230,
    HB_Combining_AboveRight = 232,
    HB_Combining_DoubleBelow = 233,
    HB_Combining_DoubleAbove = 234,
    HB_Combining_IotaSubscript = 240
} HB_CombiningClass;

typedef enum {
    HB_NoBreak,
    HB_SoftHyphen,
    HB_Break,
    HB_ForcedBreak
} HB_LineBreakType;

typedef enum  {
    HB_Mark_NonSpacing,          /*   Mn */
    HB_Mark_SpacingCombining,    /*   Mc */
    HB_Mark_Enclosing,           /*   Me */

    HB_Number_DecimalDigit,      /*   Nd */
    HB_Number_Letter,            /*   Nl */
    HB_Number_Other,             /*   No */

    HB_Separator_Space,          /*   Zs */
    HB_Separator_Line,           /*   Zl */
    HB_Separator_Paragraph,      /*   Zp */

    HB_Other_Control,            /*   Cc */
    HB_Other_Format,             /*   Cf */
    HB_Other_Surrogate,          /*   Cs */
    HB_Other_PrivateUse,         /*   Co */
    HB_Other_NotAssigned,        /*   Cn */

    HB_Letter_Uppercase,         /*   Lu */
    HB_Letter_Lowercase,         /*   Ll */
    HB_Letter_Titlecase,         /*   Lt */
    HB_Letter_Modifier,          /*   Lm */
    HB_Letter_Other,             /*   Lo */

    HB_Punctuation_Connector,    /*   Pc */
    HB_Punctuation_Dash,         /*   Pd */
    HB_Punctuation_Open,         /*   Ps */
    HB_Punctuation_Close,        /*   Pe */
    HB_Punctuation_InitialQuote, /*   Pi */
    HB_Punctuation_FinalQuote,   /*   Pf */
    HB_Punctuation_Other,        /*   Po */

    HB_Symbol_Math,              /*   Sm */
    HB_Symbol_Currency,          /*   Sc */
    HB_Symbol_Modifier,          /*   Sk */
    HB_Symbol_Other              /*   So */
} HB_CharCategory;

/*
 see http://www.unicode.org/reports/tr14/tr14-19.html
 we don't use the XX, AI and CB properties and map them to AL instead.
 as we don't support any EBDIC based OS'es, NL is ignored and mapped to AL as well.
*/
typedef enum {
    HB_LineBreak_OP, HB_LineBreak_CL, HB_LineBreak_QU, HB_LineBreak_GL, HB_LineBreak_NS,
    HB_LineBreak_EX, HB_LineBreak_SY, HB_LineBreak_IS, HB_LineBreak_PR, HB_LineBreak_PO,
    HB_LineBreak_NU, HB_LineBreak_AL, HB_LineBreak_ID, HB_LineBreak_IN, HB_LineBreak_HY,
    HB_LineBreak_BA, HB_LineBreak_BB, HB_LineBreak_B2, HB_LineBreak_ZW, HB_LineBreak_CM,
    HB_LineBreak_WJ, HB_LineBreak_H2, HB_LineBreak_H3, HB_LineBreak_JL, HB_LineBreak_JV,
    HB_LineBreak_JT, HB_LineBreak_SA, HB_LineBreak_SG,
    HB_LineBreak_SP, HB_LineBreak_CR, HB_LineBreak_LF, HB_LineBreak_BK
} HB_LineBreakClass;

typedef enum {
    HB_Grapheme_Other, 
    HB_Grapheme_CR,
    HB_Grapheme_LF,
    HB_Grapheme_Control,
    HB_Grapheme_Extend,
    HB_Grapheme_L, 
    HB_Grapheme_V, 
    HB_Grapheme_T, 
    HB_Grapheme_LV, 
    HB_Grapheme_LVT
} HB_GraphemeClass;

typedef enum {
    HB_ShaperFlag_Default = 0,
    HB_ShaperFlag_NoKerning = 1,
    HB_ShaperFlag_UseDesignMetrics = 2
} HB_ShaperFlag;

/* 
   highest value means highest priority for justification. Justification is done by first inserting kashidas
   starting with the highest priority positions, then stretching spaces, afterwards extending inter char
   spacing, and last spacing between arabic words.
   NoJustification is for example set for arabic where no Kashida can be inserted or for diacritics.
*/
typedef enum {
    HB_NoJustification= 0,   /* Justification can't be applied after this glyph */
    HB_Arabic_Space   = 1,   /* This glyph represents a space inside arabic text */
    HB_Character      = 2,   /* Inter-character justification point follows this glyph */
    HB_Space          = 4,   /* This glyph represents a blank outside an Arabic run */
    HB_Arabic_Normal  = 7,   /* Normal Middle-Of-Word glyph that connects to the right (begin) */
    HB_Arabic_Waw     = 8,   /* Next character is final form of Waw/Ain/Qaf/Fa */
    HB_Arabic_BaRa    = 9,   /* Next two chars are Ba + Ra/Ya/AlefMaksura */
    HB_Arabic_Alef    = 10,  /* Next character is final form of Alef/Tah/Lam/Kaf/Gaf */
    HB_Arabic_HaaDal  = 11,  /* Next character is final form of Haa/Dal/Taa Marbutah */
    HB_Arabic_Seen    = 12,  /* Initial or Medial form Of Seen/Sad */
    HB_Arabic_Kashida = 13   /* Kashida(U+640) in middle of word */
} HB_JustificationClass;

typedef struct {
    HB_LineBreakType lineBreakType;
    bool whiteSpace; /* A unicode whitespace character, except NBSP, ZWNBSP */
    bool charStop; /* Valid cursor position (for left/right arrow) */
} HB_CharAttributes;

typedef struct {
    HB_Fixed x;
    HB_Fixed y;
} HB_FixedPoint;

typedef struct {
    HB_Fixed x, y;
    HB_Fixed width, height;
    HB_Fixed xOffset, yOffset;
} HB_GlyphMetrics;

typedef struct {
    uint32_t pos;
    uint32_t length;
    HB_Script script;
} HB_ScriptItem;

typedef struct {
    HB_JustificationClass justification;
    bool clusterStart;   /* First glyph of representation of cluster */
    bool mark;           /* needs to be positioned around base char */
    bool dontPrint;
    HB_CombiningClass combiningClass;
} HB_GlyphAttributes;

typedef struct {
    const HB_UChar16 *string;               /* input: the Unicode UTF16 text to be shaped */
    uint32_t stringLength;                 /* input: the length of the input in 16-bit words */
    HB_ScriptItem item;                     /* input: the current run to be shaped: a run of text all in the same script that is a substring of <string> */
    int shaperFlags;                        /* input (unused) should be set to 0; intended to support flags defined in HB_ShaperFlag */
    bool glyphIndicesPresent;            /* input: true if the <glyphs> array contains glyph indices ready to be shaped */
    uint32_t initialGlyphCount;            /* input: if glyphIndicesPresent is true, the number of glyph indices in the <glyphs> array */

    uint32_t num_glyphs;                   /* input: capacity of output arrays <glyphs>, <attributes>, <advances>, <offsets>, and <log_clusters>; */
                                            /* output: required capacity (may be larger than actual capacity) */

    HB_Glyph *glyphs;                       /* output: <num_glyphs> indices of shaped glyphs */
    HB_GlyphAttributes *attributes;         /* output: <num_glyphs> glyph attributes */
    HB_Fixed *advances;                     /* output: <num_glyphs> advances */
    HB_FixedPoint *offsets;                 /* output: <num_glyphs> offsets */
    unsigned short *log_clusters;           /* output: for each output glyph, the index in the input of the start of its logical cluster */

    QFontEngine* font;
} HB_ShaperItem;

bool qHB_BasicShape(HB_ShaperItem *shaper_item);

void qHB_GetCharAttributes(const HB_UChar16 *string, uint32_t stringLength,
                           HB_CharAttributes *attributes);

Q_DECLARE_TYPEINFO(HB_GlyphAttributes, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(HB_FixedPoint, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
