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

#include "qunicodetables_p.h"
#include "qfontengine_p.h"
#include "qharfbuzz_p.h"

QT_BEGIN_NAMESPACE

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

static void qHB_GetGraphemeAndLineBreakClass(HB_UChar32 ch, HB_GraphemeClass *grapheme, HB_LineBreakClass *lineBreak)
{
    *grapheme = (HB_GraphemeClass) QUnicodeTables::graphemeBreak(ch);
    *lineBreak = (HB_LineBreakClass) QUnicodeTables::lineBreakClass(ch);
}

static void qHB_GetUnicodeCharProperties(HB_UChar32 ch, HB_CharCategory *category)
{
    *category = (HB_CharCategory)QChar::category(ch);
}

static void qHB_GetGlyphAdvances(QFontEngine* fe, const HB_Glyph *glyphs, uint32_t numGlyphs, HB_Fixed *advances, int flags)
{
    QGlyphLayoutArray qglyphs(numGlyphs);

    for (uint32_t i = 0; i < numGlyphs; ++i)
        qglyphs.glyphs[i] = glyphs[i];

    fe->recalcAdvances(&qglyphs);

    for (uint32_t i = 0; i < numGlyphs; ++i)
        advances[i] = qglyphs.advances_x[i].value();
}

// set the glyph attributes heuristically. Assumes a 1 to 1 relationship between chars and glyphs
// and no reordering.
// also computes logClusters heuristically
static void qHB_HeuristicSetGlyphAttributes(HB_ShaperItem *item)
{
    const HB_UChar16 *uc = item->string + item->item.pos;
    uint32_t length = item->item.length;

    assert(item->num_glyphs <= length);

    // qDebug("qHB_HeuristicSetGlyphAttributes: num_glyphs=%d", item->num_glyphs);
    HB_GlyphAttributes *attributes = item->attributes;
    unsigned short *logClusters = item->log_clusters;

    uint32_t glyph_pos = 0;
    uint32_t i;
    for (i = 0; i < length; i++) {
        if (HB_IsHighSurrogate(uc[i]) && i < length - 1
            && HB_IsLowSurrogate(uc[i + 1])) {
            logClusters[i] = glyph_pos;
            logClusters[++i] = glyph_pos;
        } else {
            logClusters[i] = glyph_pos;
        }
        ++glyph_pos;
    }
    assert(glyph_pos == item->num_glyphs);

    // first char in a run is never (treated as) a mark
    int cStart = 0;
    const bool symbolFont = false; // ###: item->font->isSymbolFont;
    attributes[0].mark = false;
    attributes[0].clusterStart = true;
    attributes[0].dontPrint = (!symbolFont && uc[0] == 0x00ad) || HB_IsControlChar(uc[0]);

    int pos = 0;
    HB_CharCategory lastCat;
    qHB_GetUnicodeCharProperties(uc[0], &lastCat);
    for (i = 1; i < length; ++i) {
        if (logClusters[i] == pos)
            // same glyph
            continue;
        ++pos;
        while (pos < logClusters[i]) {
            attributes[pos] = attributes[pos-1];
            ++pos;
        }
        // hide soft-hyphens by default
        attributes[pos].dontPrint = false;
        if ((!symbolFont && uc[i] == 0x00ad) || HB_IsControlChar(uc[i]))
            attributes[pos].dontPrint = true;
        HB_CharCategory cat;
        qHB_GetUnicodeCharProperties(uc[i], &cat);
        if (cat != HB_Mark_NonSpacing) {
            attributes[pos].mark = false;
            attributes[pos].clusterStart = true;
            cStart = logClusters[i];
        } else {
            attributes[pos].mark = true;
            attributes[pos].clusterStart = false;
            logClusters[i] = cStart;
        }
        // one gets an inter character justification point if the current char is not a non spacing mark.
        // as then the current char belongs to the last one and one gets a space justification point
        // after the space char.
        if (lastCat == HB_Separator_Space)
            attributes[pos-1].justification = HB_Space;
        else if (cat != HB_Mark_NonSpacing)
            attributes[pos-1].justification = HB_Character;
        else
            attributes[pos-1].justification = HB_NoJustification;

        lastCat = cat;
    }
    pos = logClusters[length-1];
    if (lastCat == HB_Separator_Space)
        attributes[pos].justification = HB_Space;
    else
        attributes[pos].justification = HB_Character;
}

static inline void positionCluster(HB_ShaperItem *item, int gfrom,  int glast)
{
    int nmarks = glast - gfrom;
    assert(nmarks > 0);

    // qDebug("---> positionCluster: cluster from %d to %d", gfrom, glast);
    // qDebug("baseInfo: %f/%f (%f/%f) off=%f/%f", baseInfo.x, baseInfo.y, baseInfo.width, baseInfo.height, baseInfo.xoff, baseInfo.yoff);

    for(int i = 1; i <= nmarks; i++) {
        item->advances[gfrom+i] = 0;
    }
}

static void qHB_HeuristicPosition(HB_ShaperItem *item)
{
    qHB_GetGlyphAdvances(item->font, item->glyphs, item->num_glyphs,
                         item->advances, item->shaperFlags);
    HB_GlyphAttributes *attributes = item->attributes;

    int cEnd = -1;
    int i = item->num_glyphs;
    while (i--) {
        if (cEnd == -1 && attributes[i].mark) {
            cEnd = i;
        } else if (cEnd != -1 && !attributes[i].mark) {
            positionCluster(item, i, cEnd);
            cEnd = -1;
        }
    }
}

void qHB_BasicShape(HB_ShaperItem *shaper_item)
{
    qHB_HeuristicSetGlyphAttributes(shaper_item);

    qHB_HeuristicPosition(shaper_item);
}


// The following line break classes are not treated by the table:
//  AI, BK, CB, CR, LF, NL, SA, SG, SP, XX

enum break_class {
    // the first 4 values have to agree with the enum in QCharAttributes
    ProhibitedBreak,            // PB in table
    DirectBreak,                // DB in table
    IndirectBreak,              // IB in table
    CombiningIndirectBreak,     // CI in table
    CombiningProhibitedBreak    // CP in table
};
#define DB DirectBreak
#define IB IndirectBreak
#define CI CombiningIndirectBreak
#define CP CombiningProhibitedBreak
#define PB ProhibitedBreak

static const uint8_t breakTable[HB_LineBreak_JT+1][HB_LineBreak_JT+1] =
{
/*          OP  CL  QU  GL  NS  EX  SY  IS  PR  PO  NU  AL  ID  IN  HY  BA  BB  B2  ZW  CM  WJ  H2  H3  JL  JV  JT */
/* OP */ { PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, PB, CP, PB, PB, PB, PB, PB, PB },
/* CL */ { DB, PB, IB, IB, PB, PB, PB, PB, IB, IB, IB, IB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* QU */ { PB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, IB, IB, IB, IB, IB, IB, PB, CI, PB, IB, IB, IB, IB, IB },
/* GL */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, IB, IB, IB, IB, IB, IB, PB, CI, PB, IB, IB, IB, IB, IB },
/* NS */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, DB, DB, DB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* EX */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, IB, DB, IB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* SY */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* IS */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, DB, IB, IB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* PR */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, IB, DB, IB, IB, DB, DB, PB, CI, PB, IB, IB, IB, IB, IB },
/* PO */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* NU */ { IB, PB, IB, IB, IB, IB, PB, PB, IB, IB, IB, IB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* AL */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* ID */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, IB, DB, DB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* IN */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, DB, DB, DB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* HY */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, DB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* BA */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, DB, DB, DB, DB, DB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* BB */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, IB, IB, IB, IB, IB, IB, PB, CI, PB, IB, IB, IB, IB, IB },
/* B2 */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, DB, DB, DB, DB, DB, IB, IB, DB, PB, PB, CI, PB, DB, DB, DB, DB, DB },
/* ZW */ { DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, DB, PB, DB, DB, DB, DB, DB, DB, DB },
/* CM */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, DB, IB, IB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, DB },
/* WJ */ { IB, PB, IB, IB, IB, PB, PB, PB, IB, IB, IB, IB, IB, IB, IB, IB, IB, IB, PB, CI, PB, IB, IB, IB, IB, IB },
/* H2 */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, IB, DB, DB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, IB, IB },
/* H3 */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, IB, DB, DB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, IB },
/* JL */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, IB, DB, DB, DB, IB, IB, IB, DB, DB, PB, CI, PB, IB, IB, IB, IB, DB },
/* JV */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, IB, DB, DB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, IB, IB },
/* JT */ { DB, PB, IB, IB, IB, PB, PB, PB, DB, IB, DB, DB, DB, IB, IB, IB, DB, DB, PB, CI, PB, DB, DB, DB, DB, IB }
};
#undef DB
#undef IB
#undef CI
#undef CP
#undef PB

static const uint8_t graphemeTable[HB_Grapheme_LVT + 1][HB_Grapheme_LVT + 1] =
{
//      Other, CR,    LF,    Control,Extend,L,    V,     T,     LV,    LVT
    { true , true , true , true , true , true , true , true , true , true  }, // Other, 
    { true , true , true , true , true , true , true , true , true , true  }, // CR,
    { true , false, true , true , true , true , true , true , true , true  }, // LF,
    { true , true , true , true , true , true , true , true , true , true  }, // Control,
    { false, true , true , true , false, false, false, false, false, false }, // Extend,
    { true , true , true , true , true , false, true , true , true , true  }, // L, 
    { true , true , true , true , true , false, false, true , false, true  }, // V, 
    { true , true , true , true , true , true , false, false, false, false }, // T, 
    { true , true , true , true , true , false, true , true , true , true  }, // LV, 
    { true , true , true , true , true , false, true , true , true , true  }, // LVT
};

void qHB_GetCharAttributes(const HB_UChar16 *string, uint32_t stringLength,
                           HB_CharAttributes *attributes)
{
    if (!stringLength)
        return;

    // ##### can this fail if the first char is a surrogate?
    HB_LineBreakClass cls;
    HB_GraphemeClass grapheme;
    qHB_GetGraphemeAndLineBreakClass(*string, &grapheme, &cls);
    // handle case where input starts with an LF
    if (cls == HB_LineBreak_LF)
        cls = HB_LineBreak_BK;

    attributes[0].whiteSpace = (cls == HB_LineBreak_SP || cls == HB_LineBreak_BK);
    attributes[0].charStop = true;

    int lcls = cls;
    for (uint32_t i = 1; i < stringLength; ++i) {
        attributes[i].whiteSpace = false;

        HB_UChar32 code = string[i];
        HB_GraphemeClass ngrapheme;
        HB_LineBreakClass ncls;
        qHB_GetGraphemeAndLineBreakClass(code, &ngrapheme, &ncls);
        attributes[i].charStop = graphemeTable[ngrapheme][grapheme];
        // handle surrogates
        if (ncls == HB_LineBreak_SG) {
            if (HB_IsHighSurrogate(string[i]) && i < stringLength - 1 && HB_IsLowSurrogate(string[i+1])) {
                continue;
            } else if (HB_IsLowSurrogate(string[i]) && HB_IsHighSurrogate(string[i-1])) {
                code = HB_SurrogateToUcs4(string[i-1], string[i]);
                qHB_GetGraphemeAndLineBreakClass(code, &ngrapheme, &ncls);
                attributes[i].charStop = false;
            } else {
                ncls = HB_LineBreak_AL;
            }
        }

        // set white space and char stop flag
        if (ncls >= HB_LineBreak_SP)
            attributes[i].whiteSpace = true;

        HB_LineBreakType lineBreakType = HB_NoBreak;
        if (cls >= HB_LineBreak_LF) {
            lineBreakType = HB_ForcedBreak;
        } else if(cls == HB_LineBreak_CR) {
            lineBreakType = (ncls == HB_LineBreak_LF) ? HB_NoBreak : HB_ForcedBreak;
        }

        if (ncls == HB_LineBreak_SP)
            goto next_no_cls_update;
        if (ncls >= HB_LineBreak_CR)
            goto next;

        {
            int tcls = ncls;
            // for south east asian chars that require a complex (dictionary analysis), the unicode
            // standard recommends to treat them as AL. thai_attributes and other attribute methods that
            // do dictionary analysis can override
            if (tcls >= HB_LineBreak_SA)
                tcls = HB_LineBreak_AL;
            if (cls >= HB_LineBreak_SA)
                cls = HB_LineBreak_AL;

            int brk = breakTable[cls][tcls];
            switch (brk) {
            case DirectBreak:
                lineBreakType = HB_Break;
                if (string[i-1] == 0xad) // soft hyphen
                    lineBreakType = HB_SoftHyphen;
                break;
            case IndirectBreak:
                lineBreakType = (lcls == HB_LineBreak_SP) ? HB_Break : HB_NoBreak;
                break;
            case CombiningIndirectBreak:
                lineBreakType = HB_NoBreak;
                if (lcls == HB_LineBreak_SP){
                    if (i > 1)
                        attributes[i-2].lineBreakType = HB_Break;
                } else {
                    goto next_no_cls_update;
                }
                break;
            case CombiningProhibitedBreak:
                lineBreakType = HB_NoBreak;
                if (lcls != HB_LineBreak_SP)
                    goto next_no_cls_update;
            case ProhibitedBreak:
            default:
                break;
            }
        }
    next:
        cls = ncls;
    next_no_cls_update:
        lcls = ncls;
        grapheme = ngrapheme;
        attributes[i-1].lineBreakType = lineBreakType;
    }
    attributes[stringLength-1].lineBreakType = HB_ForcedBreak;
}

QT_END_NAMESPACE
