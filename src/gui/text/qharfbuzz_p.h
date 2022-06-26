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
    HB_ShaperFlag_NoKerning = 1
} HB_ShaperFlag;

/* 
   highest value means highest priority for justification. Justification is done by first inserting kashidas
   starting with the highest priority positions, then stretching spaces, afterwards extending inter char
   spacing, and last spacing between arabic words.
   NoJustification is for example set for arabic where no Kashida can be inserted or for diacritics.
*/
typedef enum {
    HB_NoJustification= 0,   /* Justification can't be applied after this glyph */
    HB_Character      = 1,   /* Inter-character justification point follows this glyph */
    HB_Space          = 2,   /* This glyph represents a blank outside an Arabic run */
} HB_JustificationClass;

typedef struct {
    HB_LineBreakType lineBreakType;
    bool whiteSpace; /* A unicode whitespace character, except NBSP, ZWNBSP */
    bool charStop;   /* Valid cursor position (for left/right arrow) */
} HB_CharAttributes;

typedef struct {
    uint32_t pos;
    uint32_t length;
} HB_ScriptItem;

typedef struct {
    HB_JustificationClass justification;
    bool clusterStart;   /* First glyph of representation of cluster */
    bool mark;           /* needs to be positioned around base char */
    bool dontPrint;
} HB_GlyphAttributes;

typedef struct {
    const HB_UChar16 *string;               /* input: the Unicode UTF16 text to be shaped */
    uint32_t stringLength;                  /* input: the length of the input in 16-bit words */
    HB_ScriptItem item;                     /* input: the current run to be shaped: a run of text all in the same script that is a substring of <string> */
    int shaperFlags;                        /* input (unused) should be set to 0; intended to support flags defined in HB_ShaperFlag */

    uint32_t num_glyphs;                    /* input: capacity of output arrays <glyphs>, <attributes>, <advances>, <offsets>, and <log_clusters>; */
                                            /* output: required capacity (may be larger than actual capacity) */

    HB_Glyph *glyphs;                       /* output: <num_glyphs> indices of shaped glyphs */
    HB_GlyphAttributes *attributes;         /* output: <num_glyphs> glyph attributes */
    HB_Fixed *advances;                     /* output: <num_glyphs> advances */
    unsigned short *log_clusters;           /* output: for each output glyph, the index in the input of the start of its logical cluster */

    QFontEngine* font;
} HB_ShaperItem;

void qHB_BasicShape(HB_ShaperItem *shaper_item);

void qHB_GetCharAttributes(const HB_UChar16 *string, uint32_t stringLength,
                           HB_CharAttributes *attributes);

Q_DECLARE_TYPEINFO(HB_GlyphAttributes, Q_PRIMITIVE_TYPE);

QT_END_NAMESPACE

#endif
