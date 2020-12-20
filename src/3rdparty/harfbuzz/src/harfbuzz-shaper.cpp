/*
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 * This is part of HarfBuzz, an OpenType Layout engine library.
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

#include "harfbuzz-shaper.h"
#include "harfbuzz-shaper-private.h"

#include "harfbuzz-stream-private.h"
#include <assert.h>
#include <stdio.h>

#define HB_MIN(a, b) ((a) < (b) ? (a) : (b))
#define HB_MAX(a, b) ((a) > (b) ? (a) : (b))

// -----------------------------------------------------------------------------------------------------
//
// The line break algorithm. See http://www.unicode.org/reports/tr14/tr14-13.html
//
// -----------------------------------------------------------------------------------------------------

/* The Unicode algorithm does in our opinion allow line breaks at some
   places they shouldn't be allowed. The following changes were thus
   made in comparison to the Unicode reference:

   EX->AL from DB to IB
   SY->AL from DB to IB
   SY->PO from DB to IB
   SY->PR from DB to IB
   SY->OP from DB to IB
   AL->PR from DB to IB
   AL->PO from DB to IB
   PR->PR from DB to IB
   PO->PO from DB to IB
   PR->PO from DB to IB
   PO->PR from DB to IB
   HY->PO from DB to IB
   HY->PR from DB to IB
   HY->OP from DB to IB
   NU->EX from PB to IB
   EX->PO from DB to IB
*/

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

static const hb_uint8 breakTable[HB_LineBreak_JT+1][HB_LineBreak_JT+1] =
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

static const hb_uint8 graphemeTable[HB_Grapheme_LVT + 1][HB_Grapheme_LVT + 1] =
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
    
static void calcLineBreaks(const HB_UChar16 *uc, hb_uint32 len, HB_CharAttributes *charAttributes)
{
    if (!len)
        return;

    // ##### can this fail if the first char is a surrogate?
    HB_LineBreakClass cls;
    HB_GraphemeClass grapheme;
    HB_GetGraphemeAndLineBreakClass(*uc, &grapheme, &cls);
    // handle case where input starts with an LF
    if (cls == HB_LineBreak_LF)
        cls = HB_LineBreak_BK;

    charAttributes[0].whiteSpace = (cls == HB_LineBreak_SP || cls == HB_LineBreak_BK);
    charAttributes[0].charStop = true;

    int lcls = cls;
    for (hb_uint32 i = 1; i < len; ++i) {
        charAttributes[i].whiteSpace = false;
        charAttributes[i].charStop = true;

        HB_UChar32 code = uc[i];
        HB_GraphemeClass ngrapheme;
        HB_LineBreakClass ncls;
        HB_GetGraphemeAndLineBreakClass(code, &ngrapheme, &ncls);
        charAttributes[i].charStop = graphemeTable[ngrapheme][grapheme];
        // handle surrogates
        if (ncls == HB_LineBreak_SG) {
            if (HB_IsHighSurrogate(uc[i]) && i < len - 1 && HB_IsLowSurrogate(uc[i+1])) {
                continue;
            } else if (HB_IsLowSurrogate(uc[i]) && HB_IsHighSurrogate(uc[i-1])) {
                code = HB_SurrogateToUcs4(uc[i-1], uc[i]);
                HB_GetGraphemeAndLineBreakClass(code, &ngrapheme, &ncls);
                charAttributes[i].charStop = false;
            } else {
                ncls = HB_LineBreak_AL;
            }
        }

        // set white space and char stop flag
        if (ncls >= HB_LineBreak_SP)
            charAttributes[i].whiteSpace = true;

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
                if (uc[i-1] == 0xad) // soft hyphen
                    lineBreakType = HB_SoftHyphen;
                break;
            case IndirectBreak:
                lineBreakType = (lcls == HB_LineBreak_SP) ? HB_Break : HB_NoBreak;
                break;
            case CombiningIndirectBreak:
                lineBreakType = HB_NoBreak;
                if (lcls == HB_LineBreak_SP){
                    if (i > 1)
                        charAttributes[i-2].lineBreakType = HB_Break;
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
        charAttributes[i-1].lineBreakType = lineBreakType;
    }
    charAttributes[len-1].lineBreakType = HB_ForcedBreak;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
//
// Basic processing
//
// --------------------------------------------------------------------------------------------------------------------------------------------

static inline void positionCluster(HB_ShaperItem *item, int gfrom,  int glast)
{
    int nmarks = glast - gfrom;
    assert(nmarks > 0);

    HB_Glyph *glyphs = item->glyphs;
    HB_GlyphAttributes *attributes = item->attributes;

    HB_GlyphMetrics baseMetrics;
    item->font->klass->getGlyphMetrics(item->font, glyphs[gfrom], &baseMetrics);

    if (item->item.script == HB_Script_Hebrew
        && (-baseMetrics.y) > baseMetrics.height)
        // we need to attach below the baseline, because of the hebrew iud.
        baseMetrics.height = -baseMetrics.y;

//     qDebug("---> positionCluster: cluster from %d to %d", gfrom, glast);
//     qDebug("baseInfo: %f/%f (%f/%f) off=%f/%f", baseInfo.x, baseInfo.y, baseInfo.width, baseInfo.height, baseInfo.xoff, baseInfo.yoff);

    HB_Fixed size = item->font->klass->getFontMetric(item->font, HB_FontAscent) / 10;
    HB_Fixed offsetBase = HB_FIXED_CONSTANT(1) + (size - HB_FIXED_CONSTANT(4)) / 4;
    if (size > HB_FIXED_CONSTANT(4))
        offsetBase += HB_FIXED_CONSTANT(4);
    else
        offsetBase += size;
    //qreal offsetBase = (size - 4) / 4 + qMin<qreal>(size, 4) + 1;
//     qDebug("offset = %f", offsetBase);

    bool rightToLeft = item->item.bidiLevel % 2;

    int i;
    unsigned char lastCmb = 0;
    HB_GlyphMetrics attachmentRect;
    memset(&attachmentRect, 0, sizeof(attachmentRect));

    for(i = 1; i <= nmarks; i++) {
        HB_Glyph mark = glyphs[gfrom+i];
        HB_GlyphMetrics markMetrics;
        item->font->klass->getGlyphMetrics(item->font, mark, &markMetrics);
        HB_FixedPoint p;
        p.x = p.y = 0;
//          qDebug("markInfo: %f/%f (%f/%f) off=%f/%f", markInfo.x, markInfo.y, markInfo.width, markInfo.height, markInfo.xoff, markInfo.yoff);

        HB_Fixed offset = offsetBase;
        unsigned char cmb = attributes[gfrom+i].combiningClass;

        // ### maybe the whole position determination should move down to heuristicSetGlyphAttributes. Would save some
        // bits  in the glyphAttributes structure.
        if (cmb < 200) {
            // fixed position classes. We approximate by mapping to one of the others.
            // currently I added only the ones for arabic, hebrew, lao and thai.

            // for Lao and Thai marks with class 0, see below (heuristicSetGlyphAttributes)

            // add a bit more offset to arabic, a bit hacky
            if (cmb >= 27 && cmb <= 36 && offset < 3)
                offset +=1;
            // below
            if ((cmb >= 10 && cmb <= 18) ||
                 cmb == 20 || cmb == 22 ||
                 cmb == 29 || cmb == 32)
                cmb = HB_Combining_Below;
            // above
            else if (cmb == 23 || cmb == 27 || cmb == 28 ||
                      cmb == 30 || cmb == 31 || (cmb >= 33 && cmb <= 36))
                cmb = HB_Combining_Above;
            //below-right
            else if (cmb == 9 || cmb == 103 || cmb == 118)
                cmb = HB_Combining_BelowRight;
            // above-right
            else if (cmb == 24 || cmb == 107 || cmb == 122)
                cmb = HB_Combining_AboveRight;
            else if (cmb == 25)
                cmb = HB_Combining_AboveLeft;
            // fixed:
            //  19 21

        }

        // combining marks of different class don't interact. Reset the rectangle.
        if (cmb != lastCmb) {
            //qDebug("resetting rect");
            attachmentRect = baseMetrics;
        }

        switch(cmb) {
        case HB_Combining_DoubleBelow:
                // ### wrong in rtl context!
        case HB_Combining_BelowLeft:
            p.y += offset;
        case HB_Combining_Below:
            p.y += offset;
        case HB_Combining_BelowRight:
            p.y += offset;
        case HB_Combining_Left:
            p.x -= offset;
        case HB_Combining_Right:
            p.x += offset;
        case HB_Combining_DoubleAbove:
            // ### wrong in RTL context!
        case HB_Combining_AboveLeft:
            p.y -= offset;
        case HB_Combining_Above:
            p.y -= offset;
        case HB_Combining_AboveRight:
            p.y -= offset;

        case HB_Combining_IotaSubscript:
            default:
                break;
        }
//          qDebug("char=%x combiningClass = %d offset=%f/%f", mark, cmb, p.x(), p.y());
        markMetrics.x += p.x;
        markMetrics.y += p.y;

        HB_GlyphMetrics unitedAttachmentRect = attachmentRect;
        unitedAttachmentRect.x = HB_MIN(attachmentRect.x, markMetrics.x);
        unitedAttachmentRect.y = HB_MIN(attachmentRect.y, markMetrics.y);
        unitedAttachmentRect.width = HB_MAX(attachmentRect.x + attachmentRect.width, markMetrics.x + markMetrics.width) - unitedAttachmentRect.x;
        unitedAttachmentRect.height = HB_MAX(attachmentRect.y + attachmentRect.height, markMetrics.y + markMetrics.height) - unitedAttachmentRect.y;
        attachmentRect = unitedAttachmentRect;

        lastCmb = cmb;
        if (rightToLeft) {
            item->offsets[gfrom+i].x = p.x;
            item->offsets[gfrom+i].y = p.y;
        } else {
            item->offsets[gfrom+i].x = p.x - baseMetrics.xOffset;
            item->offsets[gfrom+i].y = p.y - baseMetrics.yOffset;
        }
        item->advances[gfrom+i] = 0;
    }
}

void HB_HeuristicPosition(HB_ShaperItem *item)
{
    HB_GetGlyphAdvances(item);
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

// set the glyph attributes heuristically. Assumes a 1 to 1 relationship between chars and glyphs
// and no reordering.
// also computes logClusters heuristically
void HB_HeuristicSetGlyphAttributes(HB_ShaperItem *item)
{
    const HB_UChar16 *uc = item->string + item->item.pos;
    hb_uint32 length = item->item.length;

    // ### zeroWidth and justification are missing here!!!!!

    assert(item->num_glyphs <= length);

//     qDebug("QScriptEngine::heuristicSetGlyphAttributes, num_glyphs=%d", item->num_glyphs);
    HB_GlyphAttributes *attributes = item->attributes;
    unsigned short *logClusters = item->log_clusters;

    hb_uint32 glyph_pos = 0;
    hb_uint32 i;
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
    const bool symbolFont = item->face->isSymbolFont;
    attributes[0].mark = false;
    attributes[0].clusterStart = true;
    attributes[0].dontPrint = (!symbolFont && uc[0] == 0x00ad) || HB_IsControlChar(uc[0]);

    int pos = 0;
    HB_CharCategory lastCat;
    int dummy;
    HB_GetUnicodeCharProperties(uc[0], &lastCat, &dummy);
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
        if ((!symbolFont && uc[i] == 0x00ad) || HB_IsControlChar(uc[i]))
            attributes[pos].dontPrint = true;
        HB_CharCategory cat;
        int cmb;
        HB_GetUnicodeCharProperties(uc[i], &cat, &cmb);
        if (cat != HB_Mark_NonSpacing) {
            attributes[pos].mark = false;
            attributes[pos].clusterStart = true;
            attributes[pos].combiningClass = 0;
            cStart = logClusters[i];
        } else {
            if (cmb == 0) {
                // Fix 0 combining classes
                if ((uc[pos] & 0xff00) == 0x0e00) {
                    // thai or lao
                    if (uc[pos] == 0xe31 ||
                         uc[pos] == 0xe34 ||
                         uc[pos] == 0xe35 ||
                         uc[pos] == 0xe36 ||
                         uc[pos] == 0xe37 ||
                         uc[pos] == 0xe47 ||
                         uc[pos] == 0xe4c ||
                         uc[pos] == 0xe4d ||
                         uc[pos] == 0xe4e) {
                        cmb = HB_Combining_AboveRight;
                    } else if (uc[pos] == 0xeb1 ||
                                uc[pos] == 0xeb4 ||
                                uc[pos] == 0xeb5 ||
                                uc[pos] == 0xeb6 ||
                                uc[pos] == 0xeb7 ||
                                uc[pos] == 0xebb ||
                                uc[pos] == 0xecc ||
                                uc[pos] == 0xecd) {
                        cmb = HB_Combining_Above;
                    } else if (uc[pos] == 0xebc) {
                        cmb = HB_Combining_Below;
                    }
                }
            }

            attributes[pos].mark = true;
            attributes[pos].clusterStart = false;
            attributes[pos].combiningClass = cmb;
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

#ifndef NO_OPENTYPE
static const HB_OpenTypeFeature basic_features[] = {
    { HB_MAKE_TAG('c', 'c', 'm', 'p'), CcmpProperty },
    { HB_MAKE_TAG('l', 'i', 'g', 'a'), CcmpProperty },
    { HB_MAKE_TAG('c', 'l', 'i', 'g'), CcmpProperty },
    {0, 0}
};
#endif

HB_Bool HB_ConvertStringToGlyphIndices(HB_ShaperItem *shaper_item)
{
    if (shaper_item->glyphIndicesPresent) {
        shaper_item->num_glyphs = shaper_item->initialGlyphCount;
        shaper_item->glyphIndicesPresent = false;
        return true;
    }
    return shaper_item->font->klass
           ->convertStringToGlyphIndices(shaper_item->font,
                                         shaper_item->string + shaper_item->item.pos, shaper_item->item.length,
                                         shaper_item->glyphs, &shaper_item->num_glyphs,
                                         shaper_item->item.bidiLevel % 2);
}

HB_Bool HB_BasicShape(HB_ShaperItem *shaper_item)
{
#ifndef NO_OPENTYPE
    const int availableGlyphs = shaper_item->num_glyphs;
#endif

    if (!HB_ConvertStringToGlyphIndices(shaper_item))
        return false;

    HB_HeuristicSetGlyphAttributes(shaper_item);

#ifndef NO_OPENTYPE
    if (HB_SelectScript(shaper_item, basic_features)) {
        HB_OpenTypeShape(shaper_item, /*properties*/0);
        return HB_OpenTypePosition(shaper_item, availableGlyphs, /*doLogClusters*/true);
    }
#endif

    HB_HeuristicPosition(shaper_item);
    return true;
}

const HB_ScriptEngine HB_ScriptEngines[] = {
    // Common
    { HB_BasicShape, 0},
    // Adlam
    { HB_BasicShape, 0},
    // Afaka
    { HB_BasicShape, 0},
    // Ahom
    { HB_BasicShape, 0},
    // AnatolianHieroglyphs
    { HB_BasicShape, 0},
    // Arabic
    { HB_ArabicShape, 0},
    // Armenian
    { HB_BasicShape, 0},
    // Avestan
    { HB_BasicShape, 0},
    // Balinese
    { HB_BasicShape, 0},
    // Bamum
    { HB_BasicShape, 0},
    // Bangla
    { HB_BasicShape, 0},
    // BassaVah
    { HB_BasicShape, 0},
    // Batak
    { HB_BasicShape, 0},
    // Bhaiksuki
    { HB_BasicShape, 0},
    // Blissymbols
    { HB_BasicShape, 0},
    // BookPahlavi
    { HB_BasicShape, 0},
    // Bopomofo
    { HB_BasicShape, 0},
    // Brahmi
    { HB_BasicShape, 0},
    // Braille
    { HB_BasicShape, 0},
    // Buginese
    { HB_BasicShape, 0},
    // Buhid
    { HB_BasicShape, 0},
    // Carian
    { HB_BasicShape, 0},
    // CaucasianAlbanian
    { HB_BasicShape, 0},
    // Chakma
    { HB_BasicShape, 0},
    // Cham
    { HB_BasicShape, 0},
    // Cherokee
    { HB_BasicShape, 0},
    // Cirth
    { HB_BasicShape, 0},
    // Coptic
    { HB_BasicShape, 0},
    // Cypriot
    { HB_BasicShape, 0},
    // Cyrillic
    { HB_BasicShape, 0},
    // Deseret
    { HB_BasicShape, 0},
    // Devanagari
    { HB_IndicShape, HB_IndicAttributes },
    // Dogra
    { HB_BasicShape, 0},
    // Duployanshorthand
    { HB_BasicShape, 0},
    // EasternSyriac
    { HB_BasicShape, 0},
    // Egyptiandemotic
    { HB_BasicShape, 0},
    // Egyptianhieratic
    { HB_BasicShape, 0},
    // EgyptianHieroglyphs
    { HB_ArabicShape, 0},
    // Elbasan
    { HB_BasicShape, 0},
    // Elymaic
    { HB_BasicShape, 0},
    // Emoji
    { HB_BasicShape, 0},
    // EstrangeloSyriac
    { HB_BasicShape, 0},
    // Ethiopic
    { HB_BasicShape, 0},
    // FrakturLatin
    { HB_BasicShape, 0},
    // Fraser
    { HB_BasicShape, 0},
    // GaelicLatin
    { HB_BasicShape, 0},
    // GeorgianKhutsuri
    { HB_BasicShape, 0},
    // Georgian
    { HB_BasicShape, 0},
    // Glagolitic
    { HB_BasicShape, 0},
    // Gothic
    { HB_BasicShape, 0},
    // Grantha
    { HB_BasicShape, 0},
    // Greek
    { HB_GreekShape, 0},
    // Gujarati
    { HB_IndicShape, HB_IndicAttributes },
    // GunjalaGondi
    { HB_BasicShape, 0},
    // Gurmukhi
    { HB_IndicShape, HB_IndicAttributes },
    // Han
    { HB_BasicShape, 0},
    // Hangul
    { HB_HangulShape, 0},
    // HanifiRohingya
    { HB_BasicShape, 0},
    // Hanunoo
    { HB_BasicShape, 0},
    // HanwithBopomofo
    { HB_BasicShape, 0},
    // Hatran
    { HB_BasicShape, 0},
    // Hebrew
    { HB_HebrewShape, 0},
    // Hiragana
    { HB_BasicShape, 0},
    // ImperialAramaic
    { HB_BasicShape, 0},
    // Indus
    { HB_BasicShape, 0},
    // Inherited
    { HB_BasicShape, 0},
    // InscriptionalPahlavi
    { HB_BasicShape, 0},
    // InscriptionalParthian
    { HB_BasicShape, 0},
    // Jamo
    { HB_BasicShape, 0},
    // Japanese
    { HB_BasicShape, 0},
    // Japanesesyllabaries
    { HB_BasicShape, 0},
    // Javanese
    { HB_BasicShape, 0},
    // Jurchen
    { HB_BasicShape, 0},
    // Kaithi
    { HB_BasicShape, 0},
    // Kannada
    { HB_IndicShape, HB_IndicAttributes },
    // Katakana
    { HB_BasicShape, 0},
    // KayahLi
    { HB_BasicShape, 0},
    // Kharoshthi
    { HB_BasicShape, 0},
    // Khmer
    { HB_KhmerShape, HB_KhmerAttributes },
    // Khojki
    { HB_BasicShape, 0},
    // Khudawadi
    { HB_BasicShape, 0},
    // Korean
    { HB_BasicShape, 0},
    // Kpelle
    { HB_BasicShape, 0},
    // Lanna
    { HB_BasicShape, 0},
    // Lao
    { HB_BasicShape, 0},
    // Latin
    { HB_BasicShape, 0},
    // Lepcha
    { HB_BasicShape, 0},
    // Limbu
    { HB_BasicShape, 0},
    // LinearA
    { HB_BasicShape, 0},
    // LinearB
    { HB_BasicShape, 0},
    // Loma
    { HB_BasicShape, 0},
    // Lycian
    { HB_BasicShape, 0},
    // Lydian
    { HB_BasicShape, 0},
    // Mahajani
    { HB_IndicShape, HB_IndicAttributes },
    // Makasar
    { HB_BasicShape, 0},
    // Malayalam
    { HB_IndicShape, HB_IndicAttributes },
    // Mandaean
    { HB_BasicShape, 0},
    // Manichaean
    { HB_BasicShape, 0},
    // Marchen
    { HB_BasicShape, 0},
    // MasaramGondi
    { HB_BasicShape, 0},
    // MathematicalNotation
    { HB_BasicShape, 0},
    // Mayanhieroglyphs
    { HB_BasicShape, 0},
    // Medefaidrin
    { HB_BasicShape, 0},
    // MeiteiMayek
    { HB_BasicShape, 0},
    // Mende
    { HB_BasicShape, 0},
    // MeroiticCursive
    { HB_BasicShape, 0},
    // Meroitic
    { HB_BasicShape, 0},
    // Modi
    { HB_BasicShape, 0},
    // Mongolian
    { HB_BasicShape, 0},
    // Moon
    { HB_BasicShape, 0},
    // Mro
    { HB_BasicShape, 0},
    // Multani
    { HB_BasicShape, 0},
    // Myanmar
    { HB_MyanmarShape, HB_MyanmarAttributes },
    // Nko
    { HB_ArabicShape, 0},
    // Nabataean
    { HB_BasicShape, 0},
    // Nandinagari
    { HB_BasicShape, 0},
    // NaxiGeba
    { HB_BasicShape, 0},
    // NewTaiLue
    { HB_BasicShape, 0},
    // Newa
    { HB_BasicShape, 0},
    // Nushu
    { HB_BasicShape, 0},
    // NyiakengPuachueHmong
    { HB_BasicShape, 0},
    // Odia
    { HB_BasicShape, 0},
    // Ogham
    { HB_BasicShape, 0},
    // OlChiki
    { HB_BasicShape, 0},
    // OldChurchSlavonicCyrillic
    { HB_BasicShape, 0},
    // OldHungarian
    { HB_BasicShape, 0},
    // OldItalic
    { HB_BasicShape, 0},
    // OldNorthArabian
    { HB_ArabicShape, 0},
    // OldPermic
    { HB_BasicShape, 0},
    // OldPersian
    { HB_BasicShape, 0},
    // OldSogdian
    { HB_BasicShape, 0},
    // OldSouthArabian
    { HB_ArabicShape, 0},
    // Orkhon
    { HB_BasicShape, 0},
    // Osage
    { HB_BasicShape, 0},
    // Osmanya
    { HB_BasicShape, 0},
    // PahawhHmong
    { HB_BasicShape, 0},
    // Palmyrene
    { HB_BasicShape, 0},
    // PauCinHau
    { HB_BasicShape, 0},
    // Phagspa
    { HB_BasicShape, 0},
    // Phoenician
    { HB_BasicShape, 0},
    // PollardPhonetic
    { HB_BasicShape, 0},
    // PsalterPahlavi
    { HB_BasicShape, 0},
    // Rejang
    { HB_BasicShape, 0},
    // Rongorongo
    { HB_BasicShape, 0},
    // Runic
    { HB_BasicShape, 0},
    // SACuneiform
    { HB_BasicShape, 0},
    // Samaritan
    { HB_BasicShape, 0},
    // Sarati
    { HB_BasicShape, 0},
    // Saurashtra
    { HB_BasicShape, 0},
    // Sharada
    { HB_BasicShape, 0},
    // Shavian
    { HB_BasicShape, 0},
    // Siddham
    { HB_BasicShape, 0},
    // SignWriting
    { HB_BasicShape, 0},
    // SimplifiedHan
    { HB_BasicShape, 0},
    // Sinhala
    { HB_IndicShape, HB_IndicAttributes },
    // Sogdian
    { HB_BasicShape, 0},
    // SoraSompeng
    { HB_BasicShape, 0},
    // Soyombo
    { HB_BasicShape, 0},
    // Sundanese
    { HB_BasicShape, 0},
    // SylotiNagri
    { HB_BasicShape, 0},
    // Symbols
    { HB_BasicShape, 0},
    // Syriac
    { HB_ArabicShape, 0},
    // Tagalog
    { HB_BasicShape, 0},
    // Tagbanwa
    { HB_BasicShape, 0},
    // TaiLe
    { HB_BasicShape, 0},
    // TaiViet
    { HB_BasicShape, 0},
    // Takri
    { HB_BasicShape, 0},
    // Tamil
    { HB_IndicShape, HB_IndicAttributes },
    // Tangut
    { HB_BasicShape, 0},
    // Telugu
    { HB_IndicShape, HB_IndicAttributes },
    // Tengwar
    { HB_BasicShape, 0},
    // Thaana
    { HB_BasicShape, 0},
    // Thai
    { HB_BasicShape, HB_ThaiAttributes },
    // Tibetan
    { HB_TibetanShape, HB_TibetanAttributes },
    // Tifinagh
    { HB_BasicShape, 0},
    // Tirhuta
    { HB_BasicShape, 0},
    // TraditionalHan
    { HB_BasicShape, 0},
    // UCAS
    { HB_BasicShape, 0},
    // Ugaritic
    { HB_BasicShape, 0},
    // Unwritten
    { HB_BasicShape, 0},
    // Vai
    { HB_BasicShape, 0},
    // VarangKshiti
    { HB_BasicShape, 0},
    // VisibleSpeech
    { HB_BasicShape, 0},
    // Wancho
    { HB_BasicShape, 0},
    // WesternSyriac
    { HB_BasicShape, 0},
    // Woleai
    { HB_BasicShape, 0},
    // Yi
    { HB_BasicShape, 0},
    // ZanabazarSquare
    { HB_BasicShape, 0},
    // Zawgyi
    { HB_BasicShape, 0},
};
static const int NumEngines = sizeof(HB_ScriptEngines)/sizeof(HB_ScriptEngine);

void HB_GetCharAttributes(const HB_UChar16 *string, hb_uint32 stringLength,
                          const HB_ScriptItem *items, hb_uint32 numItems,
                          HB_CharAttributes *attributes)
{
    calcLineBreaks(string, stringLength, attributes);

    for (hb_uint32 i = 0; i < numItems; ++i) {
        HB_Script script = items[i].script;
        if (script == HB_Script_Inherited)
            script = HB_Script_Common;
        HB_AttributeFunction attributeFunction = HB_ScriptEngines[script].charAttributes;
        if (!attributeFunction)
            continue;
        attributeFunction(script, string, items[i].pos, items[i].length, attributes);
    }
}


enum BreakRule { NoBreak = 0, Break = 1, Middle = 2 };

static const hb_uint8 wordbreakTable[HB_Word_ExtendNumLet + 1][HB_Word_ExtendNumLet + 1] = {
//        Other    Format   Katakana ALetter  MidLetter MidNum  Numeric  ExtendNumLet
    {   Break,   Break,   Break,   Break,   Break,   Break,   Break,   Break }, // Other
    {   Break,   Break,   Break,   Break,   Break,   Break,   Break,   Break }, // Format 
    {   Break,   Break, NoBreak,   Break,   Break,   Break,   Break, NoBreak }, // Katakana
    {   Break,   Break,   Break, NoBreak,  Middle,   Break, NoBreak, NoBreak }, // ALetter
    {   Break,   Break,   Break,   Break,   Break,   Break,   Break,   Break }, // MidLetter
    {   Break,   Break,   Break,   Break,   Break,   Break,   Break,   Break }, // MidNum
    {   Break,   Break,   Break, NoBreak,   Break,  Middle, NoBreak, NoBreak }, // Numeric
    {   Break,   Break, NoBreak, NoBreak,   Break,   Break, NoBreak, NoBreak }, // ExtendNumLet
};

void HB_GetWordBoundaries(const HB_UChar16 *string, hb_uint32 stringLength,
                          const HB_ScriptItem * /*items*/, hb_uint32 /*numItems*/,
                          HB_CharAttributes *attributes)
{
    if (stringLength == 0)
        return;
    unsigned int brk = HB_GetWordClass(string[0]);
    attributes[0].wordBoundary = true;
    for (hb_uint32 i = 1; i < stringLength; ++i) {
        if (!attributes[i].charStop) {
            attributes[i].wordBoundary = false;
            continue;
        }
        hb_uint32 nbrk = HB_GetWordClass(string[i]);
        if (nbrk == HB_Word_Format) {
            attributes[i].wordBoundary = (HB_GetSentenceClass(string[i-1]) == HB_Sentence_Sep);
            continue;
        }
        BreakRule rule = (BreakRule)wordbreakTable[brk][nbrk];
        if (rule == Middle) {
            rule = Break;
            hb_uint32 lookahead = i + 1;
            while (lookahead < stringLength) {
                hb_uint32 testbrk = HB_GetWordClass(string[lookahead]);
                if (testbrk == HB_Word_Format && HB_GetSentenceClass(string[lookahead]) != HB_Sentence_Sep) {
                    ++lookahead;
                    continue;
                }
                if (testbrk == brk) {
                    rule = NoBreak;
                    while (i < lookahead)
                        attributes[i++].wordBoundary = false;
                    nbrk = testbrk;
                }
                break;
            }
        }
        attributes[i].wordBoundary = (rule == Break);
        brk = nbrk;
    }
}


enum SentenceBreakStates {
    SB_Initial,
    SB_Upper,
    SB_UpATerm, 
    SB_ATerm,
    SB_ATermC, 
    SB_ACS, 
    SB_STerm, 
    SB_STermC, 
    SB_SCS,
    SB_BAfter, 
    SB_Break,
    SB_Look
};

static const hb_uint8 sentenceBreakTable[HB_Sentence_Close + 1][HB_Sentence_Close + 1] = {
//        Other       Sep         Format      Sp          Lower       Upper       OLetter     Numeric     ATerm       STerm       Close
      { SB_Initial, SB_BAfter , SB_Initial, SB_Initial, SB_Initial, SB_Upper  , SB_Initial, SB_Initial, SB_ATerm  , SB_STerm  , SB_Initial }, // SB_Initial,
      { SB_Initial, SB_BAfter , SB_Upper  , SB_Initial, SB_Initial, SB_Upper  , SB_Initial, SB_Initial, SB_UpATerm, SB_STerm  , SB_Initial }, // SB_Upper
      
      { SB_Look   , SB_BAfter , SB_UpATerm, SB_ACS    , SB_Initial, SB_Upper  , SB_Break  , SB_Initial, SB_ATerm  , SB_STerm  , SB_ATermC  }, // SB_UpATerm
      { SB_Look   , SB_BAfter , SB_ATerm  , SB_ACS    , SB_Initial, SB_Break  , SB_Break  , SB_Initial, SB_ATerm  , SB_STerm  , SB_ATermC  }, // SB_ATerm
      { SB_Look   , SB_BAfter , SB_ATermC , SB_ACS    , SB_Initial, SB_Break  , SB_Break  , SB_Look   , SB_ATerm  , SB_STerm  , SB_ATermC  }, // SB_ATermC,
      { SB_Look   , SB_BAfter , SB_ACS    , SB_ACS    , SB_Initial, SB_Break  , SB_Break  , SB_Look   , SB_ATerm  , SB_STerm  , SB_Look    }, // SB_ACS,
      
      { SB_Break  , SB_BAfter , SB_STerm  , SB_SCS    , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_ATerm  , SB_STerm  , SB_STermC  }, // SB_STerm,
      { SB_Break  , SB_BAfter , SB_STermC , SB_SCS    , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_ATerm  , SB_STerm  , SB_STermC  }, // SB_STermC,
      { SB_Break  , SB_BAfter , SB_SCS    , SB_SCS    , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_ATerm  , SB_STerm  , SB_Break   }, // SB_SCS,
      { SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_Break  , SB_Break   }, // SB_BAfter,
};

void HB_GetSentenceBoundaries(const HB_UChar16 *string, hb_uint32 stringLength,
                              const HB_ScriptItem * /*items*/, hb_uint32 /*numItems*/,
                              HB_CharAttributes *attributes)
{
    if (stringLength == 0)
        return;
    hb_uint32 brk = sentenceBreakTable[SB_Initial][HB_GetSentenceClass(string[0])];
    attributes[0].sentenceBoundary = true;
    for (hb_uint32 i = 1; i < stringLength; ++i) {
        if (!attributes[i].charStop) {
            attributes[i].sentenceBoundary = false;
            continue;
        }
        brk = sentenceBreakTable[brk][HB_GetSentenceClass(string[i])];
        if (brk == SB_Look) {
            brk = SB_Break;
            hb_uint32 lookahead = i + 1;
            while (lookahead < stringLength) {
                hb_uint32 sbrk = HB_GetSentenceClass(string[lookahead]);
                if (sbrk != HB_Sentence_Other && sbrk != HB_Sentence_Numeric && sbrk != HB_Sentence_Close) {
                    break;
                } else if (sbrk == HB_Sentence_Lower) {
                    brk = SB_Initial;
                    break;
                }
                ++lookahead;
            }
            if (brk == SB_Initial) {
                while (i < lookahead)
                    attributes[i++].sentenceBoundary = false;
            }
        }
        if (brk == SB_Break) {
            attributes[i].sentenceBoundary = true;
            brk = sentenceBreakTable[SB_Initial][HB_GetSentenceClass(string[i])];
        } else {
            attributes[i].sentenceBoundary = false;
        }
    }
}


static inline char *tag_to_string(HB_UInt tag)
{
    static char string[5];
    string[0] = (tag >> 24)&0xff;
    string[1] = (tag >> 16)&0xff;
    string[2] = (tag >> 8)&0xff;
    string[3] = tag&0xff;
    string[4] = 0;
    return string;
}

#ifdef OT_DEBUG
static void dump_string(HB_Buffer buffer)
{
    for (uint i = 0; i < buffer->in_length; ++i) {
        qDebug("    %x: cluster=%d", buffer->in_string[i].gindex, buffer->in_string[i].cluster);
    }
}
#define DEBUG printf
#else
#define DEBUG if (1) ; else printf
#endif

#define DefaultLangSys 0xffff
#define DefaultScript HB_MAKE_TAG('D', 'F', 'L', 'T')

enum {
    RequiresGsub = 1,
    RequiresGpos = 2
};

struct OTScripts {
    unsigned int tag;
    int flags;
};
// values are from https://unicode.org/iso15924/iso15924-codes.html
// and https://docs.microsoft.com/en-us/typography/opentype/spec/scripttags
static const OTScripts ot_scripts [] = {
    // Common
    { DefaultScript, 0 },
    // Adlam
    { HB_MAKE_TAG('a', 'd', 'l', 'm'), 0 },
    // Afaka
    { HB_MAKE_TAG('a', 'f', 'a', 'k'), 0 },
    // Ahom
    { HB_MAKE_TAG('a', 'h', 'o', 'm'), 0 },
    // AnatolianHieroglyphs
    { HB_MAKE_TAG('h', 'l', 'u', 'w'), 0 },
    // Arabic
    { HB_MAKE_TAG('a', 'r', 'a', 'b'), 1 },
    // Armenian
    { HB_MAKE_TAG('a', 'r', 'm', 'n'), 0 },
    // Avestan
    { HB_MAKE_TAG('a', 'v', 's', 't'), 0 },
    // Balinese
    { HB_MAKE_TAG('b', 'a', 'l', 'i'), 0 },
    // Bamum
    { HB_MAKE_TAG('b', 'a', 'm', 'u'), 0 },
    // Bangla
    { HB_MAKE_TAG('b', 'e', 'n', 'g'), 0 },
    // BassaVah
    { HB_MAKE_TAG('b', 'a', 's', 's'), 0 },
    // Batak
    { HB_MAKE_TAG('b', 'a', 't', 'k'), 0 },
    // Bhaiksuki
    { HB_MAKE_TAG('b', 'h', 'k', 's'), 0 },
    // Blissymbols
    { HB_MAKE_TAG('b', 'l', 'i', 's'), 0 },
    // BookPahlavi
    { HB_MAKE_TAG('p', 'h', 'l', 'v'), 0 },
    // Bopomofo
    { HB_MAKE_TAG('b', 'o', 'p', 'o'), 0 },
    // Brahmi
    { HB_MAKE_TAG('b', 'r', 'a', 'h'), 0 },
    // Braille
    { HB_MAKE_TAG('b', 'r', 'a', 'i'), 0 },
    // Buginese
    { HB_MAKE_TAG('b', 'u', 'g', 'i'), 0 },
    // Buhid
    { HB_MAKE_TAG('b', 'u', 'h', 'd'), 0 },
    // Carian
    { HB_MAKE_TAG('c', 'a', 'r', 'i'), 0 },
    // CaucasianAlbanian
    { HB_MAKE_TAG('a', 'r', 'h', 'b'), 0 },
    // Chakma
    { HB_MAKE_TAG('c', 'a', 'k', 'm'), 0 },
    // Cham
    { HB_MAKE_TAG('c', 'h', 'a', 'm'), 0 },
    // Cherokee
    { HB_MAKE_TAG('c', 'h', 'e', 'r'), 0 },
    // Cirth
    { HB_MAKE_TAG('c', 'i', 'r', 't'), 0 },
    // Coptic
    { HB_MAKE_TAG('c', 'o', 'p', 't'), 0 },
    // Cypriot
    { HB_MAKE_TAG('c', 'p', 'r', 't'), 0 },
    // Cyrillic
    { HB_MAKE_TAG('c', 'y', 'r', 'l'), 0 },
    // Deseret
    { HB_MAKE_TAG('d', 's', 'r', 't'), 0 },
    // Devanagari
    { HB_MAKE_TAG('d', 'e', 'v', 'a'), 1 },
    // Dogra
    { HB_MAKE_TAG('d', 'o', 'g', 'r'), 0 },
    // Duployanshorthand
    { HB_MAKE_TAG('d', 'u', 'p', 'l'), 0 },
    // EasternSyriac
    { HB_MAKE_TAG('s', 'y', 'r', 'c'), 0 },
    // Egyptiandemotic
    { HB_MAKE_TAG('e', 'g', 'y', 'p'), 0 },
    // Egyptianhieratic
    { HB_MAKE_TAG('e', 'g', 'y', 'p'), 0 },
    // Egyptianhieroglyphs
    { HB_MAKE_TAG('e', 'g', 'y', 'p'), 0 },
    // Elbasan
    { HB_MAKE_TAG('e', 'l', 'b', 'a'), 0 },
    // Elymaic
    { HB_MAKE_TAG('e', 'l', 'y', 'm'), 0 },
    // Emoji
    { HB_MAKE_TAG('z', 's', 'y', 'e'), 0 },
    // EstrangeloSyriac
    { HB_MAKE_TAG('s', 'y', 'r', 'c'), 0 },
    // Ethiopic
    { HB_MAKE_TAG('e', 't', 'h', 'i'), 0 },
    // FrakturLatin
    { HB_MAKE_TAG('l', 'a', 't', 'n'), 0 },
    // Fraser
    { HB_MAKE_TAG('l', 'i', 's', 'u'), 0 },
    // GaelicLatin
    { HB_MAKE_TAG('l', 'a', 't', 'n'), 0 },
    // GeorgianKhutsuri
    { HB_MAKE_TAG('g', 'e', 'o', 'r'), 0 },
    // Georgian
    { HB_MAKE_TAG('g', 'e', 'o', 'r'), 0 },
    // Glagolitic
    { HB_MAKE_TAG('g', 'l', 'a', 'g'), 0 },
    // Gothic
    { HB_MAKE_TAG('g', 'o', 't', 'h'), 0 },
    // Grantha
    { HB_MAKE_TAG('g', 'r', 'a', 'n'), 0 },
    // Greek
    { HB_MAKE_TAG('g', 'r', 'e', 'k'), 0 },
    // Gujarati
    { HB_MAKE_TAG('g', 'u', 'j', 'r'), 1 },
    // GunjalaGondi
    { HB_MAKE_TAG('g', 'o', 'n', 'g'), 0 },
    // Gurmukhi
    { HB_MAKE_TAG('g', 'u', 'r', 'u'), 1 },
    // Han
    { HB_MAKE_TAG('h', 'a', 'n', 0), 0 },
    // Hangul
    { HB_MAKE_TAG('h', 'a', 'n', 'g'), 1 },
    // HanifiRohingya
    { HB_MAKE_TAG('r', 'o', 'h', 'g'), 0 },
    // Hanunoo
    { HB_MAKE_TAG('h', 'a', 'n', 'o'), 0 },
    // HanwithBopomofo
    { HB_MAKE_TAG('h', 'a', 'n', 'b'), 0 },
    // Hatran
    { HB_MAKE_TAG('h', 'a', 't', 'r'), 0 },
    // Hebrew
    { HB_MAKE_TAG('h', 'e', 'b', 'r'), 1 },
    // Hiragana
    { HB_MAKE_TAG('k', 'a', 'n', 'a'), 0 },
    // ImperialAramaic
    { HB_MAKE_TAG('a', 'r', 'm', 'i'), 0 },
    // Indus
    { HB_MAKE_TAG('i', 'n', 'd', 's'), 0 },
    // Inherited
    { HB_MAKE_TAG('z', 'i', 'n', 'h'), 0 },
    // InscriptionalPahlavi
    { HB_MAKE_TAG('p', 'h', 'l', 'i'), 0 },
    // InscriptionalParthian
    { HB_MAKE_TAG('p', 'r', 't', 'i'), 0 },
    // Jamo
    { HB_MAKE_TAG('j', 'a', 'm', 'o'), 0 },
    // Japanese
    { HB_MAKE_TAG('j', 'p', 'a', 'n'), 0 },
    // Japanesesyllabaries
    { HB_MAKE_TAG('h', 'r', 'k', 't'), 0 },
    // Javanese
    { HB_MAKE_TAG('j', 'a', 'v', 'a'), 0 },
    // Jurchen
    { HB_MAKE_TAG('j', 'u', 'r', 'c'), 0 },
    // Kaithi
    { HB_MAKE_TAG('k', 't', 'h', 'i'), 0 },
    // Kannada
    { HB_MAKE_TAG('k', 'n', 'd', 'a'), 1 },
    // Katakana
    { HB_MAKE_TAG('k', 'a', 'n', 'a'), 0 },
    // KayahLi
    { HB_MAKE_TAG('k', 'a', 'l', 'i'), 0 },
    // Kharoshthi
    { HB_MAKE_TAG('k', 'h', 'a', 'r'), 0 },
    // Khmer
    { HB_MAKE_TAG('k', 'h', 'm', 'r'), 1 },
    // Khojki
    { HB_MAKE_TAG('k', 'h', 'o', 'j'), 0 },
    // Khudawadi
    { HB_MAKE_TAG('s', 'i', 'n', 'd'), 0 },
    // Korean
    { HB_MAKE_TAG('k', 'o', 'r', 'e'), 0 },
    // Kpelle
    { HB_MAKE_TAG('k', 'p', 'e', 'l'), 0 },
    // Lanna
    { HB_MAKE_TAG('l', 'a', 'n', 'a'), 0 },
    // Lao
    { HB_MAKE_TAG('l', 'a', 'o', ' '), 1 },
    // Latin
    { HB_MAKE_TAG('l', 'a', 't', 'n'), 0 },
    // Lepcha
    { HB_MAKE_TAG('l', 'e', 'p', 'c'), 0 },
    // Limbu
    { HB_MAKE_TAG('l', 'i', 'm', 'b'), 0 },
    // LinearA
    { HB_MAKE_TAG('l', 'i', 'n', 'a'), 0 },
    // LinearB
    { HB_MAKE_TAG('l', 'i', 'n', 'b'), 0 },
    // Loma
    { HB_MAKE_TAG('l', 'o', 'm', 'a'), 0 },
    // Lycian
    { HB_MAKE_TAG('l', 'y', 'c', 'i'), 0 },
    // Lydian
    { HB_MAKE_TAG('l', 'y', 'd', 'i'), 0 },
    // Mahajani
    { HB_MAKE_TAG('m', 'a', 'h', 'j'), 0 },
    // Makasar
    { HB_MAKE_TAG('m', 'a', 'k', 'a'), 0 },
    // Malayalam
    { HB_MAKE_TAG('m', 'l', 'y', 'm'), 1 },
    // Mandaean
    { HB_MAKE_TAG('m', 'a', 'n', 'd'), 0 },
    // Manichaean
    { HB_MAKE_TAG('m', 'a', 'n', 'i'), 0 },
    // Marchen
    { HB_MAKE_TAG('m', 'a', 'r', 'c'), 0 },
    // MasaramGondi
    { HB_MAKE_TAG('g', 'o', 'n', 'm'), 0 },
    // MathematicalNotation
    { HB_MAKE_TAG('m', 'a', 't', 'h'), 0 },
    // Mayanhieroglyphs
    { HB_MAKE_TAG('m', 'a', 'y', 'a'), 0 },
    // Medefaidrin
    { HB_MAKE_TAG('m', 'e', 'd', 'f'), 0 },
    // MeiteiMayek
    { HB_MAKE_TAG('m', 't', 'e', 'i'), 0 },
    // Mende
    { HB_MAKE_TAG('m', 'e', 'n', 'd'), 0 },
    // MeroiticCursive
    { HB_MAKE_TAG('m', 'e', 'r', 'c'), 0 },
    // Meroitic
    { HB_MAKE_TAG('m', 'e', 'r', 'o'), 0 },
    // Modi
    { HB_MAKE_TAG('m', 'o', 'd', 'i'), 0 },
    // Mongolian
    { HB_MAKE_TAG('m', 'o', 'n', 'g'), 0 },
    // Moon
    { HB_MAKE_TAG('m', 'o', 'o', 'n'), 0 },
    // Mro
    { HB_MAKE_TAG('m', 'r', 'o', 'o'), 0 },
    // Multani
    { HB_MAKE_TAG('m', 'u', 'l', 't'), 0 },
    // Myanmar
    { HB_MAKE_TAG('m', 'y', 'm', 'r'), 1 },
    // NKo
    { HB_MAKE_TAG('n', 'k', 'o', ' '), 1 },
    // Nabataean
    { HB_MAKE_TAG('n', 'b', 'a', 't'), 0 },
    // Nandinagari
    { HB_MAKE_TAG('n', 'a', 'n', 'd'), 0 },
    // NaxiGeba
    { HB_MAKE_TAG('n', 'k', 'g', 'b'), 0 },
    // NewTaiLue
    { HB_MAKE_TAG('t', 'a', 'l', 'u'), 0 },
    // Newa
    { HB_MAKE_TAG('n', 'e', 'w', 'a'), 0 },
    // Nushu
    { HB_MAKE_TAG('n', 's', 'h', 'u'), 0 },
    // NyiakengPuachueHmong
    { HB_MAKE_TAG('h', 'm', 'n', 'g'), 0 },
    // Odia, v2 is ory2
    { HB_MAKE_TAG('o', 'r', 'y', 'a'), 0 },
    // Ogham
    { HB_MAKE_TAG('o', 'g', 'a', 'm'), 0 },
    // OlChiki
    { HB_MAKE_TAG('o', 'l', 'c', 'k'), 0 },
    // OldChurchSlavonicCyrillic
    { HB_MAKE_TAG('c', 'y', 'r', 'l'), 0 },
    // OldHungarian
    { HB_MAKE_TAG('h', 'u', 'n', 'g'), 0 },
    // OldItalic
    { HB_MAKE_TAG('i', 't', 'a', 'l'), 0 },
    // OldNorthArabian
    { HB_MAKE_TAG('n', 'a', 'r', 'b'), 1 },
    // OldPermic
    { HB_MAKE_TAG('p', 'e', 'r', 'm'), 0 },
    // OldPersian
    { HB_MAKE_TAG('x', 'p', 'e', 'o'), 0 },
    // OldSogdian
    { HB_MAKE_TAG('s', 'o', 'g', 'o'), 0 },
    // OldSouthArabian
    { HB_MAKE_TAG('s', 'a', 'r', 'b'), 1 },
    // Orkhon
    { HB_MAKE_TAG('o', 'r', 'k', 'h'), 0 },
    // Osage
    { HB_MAKE_TAG('o', 's', 'g', 'e'), 0 },
    // Osmanya
    { HB_MAKE_TAG('o', 's', 'm', 'a'), 0 },
    // PahawhHmong
    { HB_MAKE_TAG('h', 'm', 'n', 'g'), 0 },
    // Palmyrene
    { HB_MAKE_TAG('p', 'a', 'l', 'm'), 0 },
    // PauCinHau
    { HB_MAKE_TAG('p', 'a', 'u', 'c'), 0 },
    // Phagspa
    { HB_MAKE_TAG('p', 'h', 'a', 'g'), 0 },
    // Phoenician
    { HB_MAKE_TAG('p', 'h', 'n', 'x'), 0 },
    // PollardPhonetic
    { DefaultScript, 0 },
    // PsalterPahlavi
    { HB_MAKE_TAG('p', 'h', 'l', 'p'), 0 },
    // Rejang
    { HB_MAKE_TAG('r', 'j', 'n', 'g'), 0 },
    // Rongorongo
    { HB_MAKE_TAG('r', 'o', 'r', 'o'), 0 },
    // Runic
    { HB_MAKE_TAG('r', 'u', 'n', 'r'), 0 },
    // SACuneiform
    { HB_MAKE_TAG('x', 's', 'u', 'x'), 0 },
    // Samaritan
    { HB_MAKE_TAG('s', 'a', 'm', 'r'), 0 },
    // Sarati
    { HB_MAKE_TAG('s', 'a', 'r', 'a'), 0 },
    // Saurashtra
    { HB_MAKE_TAG('s', 'a', 'u', 'r'), 0 },
    // Sharada
    { HB_MAKE_TAG('s', 'h', 'r', 'd'), 0 },
    // Shavian
    { HB_MAKE_TAG('s', 'h', 'a', 'w'), 0 },
    // Siddham
    { HB_MAKE_TAG('s', 'i', 'd', 'd'), 0 },
    // SignWriting
    { HB_MAKE_TAG('s', 'g', 'n', 'w'), 0 },
    // SimplifiedHan
    { HB_MAKE_TAG('h', 'a', 'n', 's'), 0 },
    // Sinhala
    { HB_MAKE_TAG('s', 'i', 'n', 'h'), 1 },
    // Sogdian
    { HB_MAKE_TAG('s', 'o', 'g', 'd'), 0 },
    // SoraSompeng
    { HB_MAKE_TAG('s', 'o', 'r', 'a'), 0 },
    // Soyombo
    { HB_MAKE_TAG('s', 'o', 'y', 'o'), 0 },
    // Sundanese
    { HB_MAKE_TAG('s', 'u', 'n', 'd'), 0 },
    // SylotiNagri
    { HB_MAKE_TAG('s', 'y', 'l', 'o'), 0 },
    // Symbols
    { HB_MAKE_TAG('m', 'u', 's', 'c'), 0 },
    // Syriac
    { HB_MAKE_TAG('s', 'y', 'r', 'c'), 1 },
    // Tagalog
    { HB_MAKE_TAG('t', 'g', 'l', 'g'), 0 },
    // Tagbanwa
    { HB_MAKE_TAG('t', 'a', 'g', 'b'), 0 },
    // TaiLe
    { HB_MAKE_TAG('t', 'a', 'l', 'e'), 0 },
    // TaiViet
    { HB_MAKE_TAG('t', 'a', 'v', 't'), 0 },
    // Takri
    { HB_MAKE_TAG('t', 'a', 'k', 'r'), 0 },
    // Tamil
    { HB_MAKE_TAG('t', 'a', 'm', 'l'), 1 },
    // Tangut
    { HB_MAKE_TAG('t', 'a', 'n', 'g'), 0 },
    // Telugu
    { HB_MAKE_TAG('t', 'e', 'l', 'u'), 1 },
    // Tengwar
    { HB_MAKE_TAG('t', 'e', 'n', 'g'), 0 },
    // Thaana
    { HB_MAKE_TAG('t', 'h', 'a', 'a'), 1 },
    // Thai
    { HB_MAKE_TAG('t', 'h', 'a', 'i'), 1 },
    // Tibetan
    { HB_MAKE_TAG('t', 'i', 'b', 't'), 1 },
    // Tifinagh
    { HB_MAKE_TAG('t', 'f', 'n', 'g'), 0 },
    // Tirhuta
    { HB_MAKE_TAG('t', 'i', 'r', 'h'), 0 },
    // TraditionalHan
    { HB_MAKE_TAG('h', 'a', 'n', 't'), 0 },
    // UCAS
    { DefaultScript, 0 },
    // Ugaritic
    { HB_MAKE_TAG('u', 'g', 'a', 'r'), 0 },
    // Unwritten
    { HB_MAKE_TAG('z', 'x', 'x', 'x'), 0 },
    // Vai
    { HB_MAKE_TAG('v', 'a', 'i', 0), 0 },
    // VarangKshiti
    { HB_MAKE_TAG('w', 'a', 'r', 'a'), 0 },
    // VisibleSpeech
    { HB_MAKE_TAG('v', 'i', 's', 'p'), 0 },
    // Wancho
    { HB_MAKE_TAG('w', 'c', 'h', 'o'), 0 },
    // WesternSyriac
    { HB_MAKE_TAG('s', 'y', 'r', 'c'), 0 },
    // Woleai
    { HB_MAKE_TAG('w', 'o', 'l', 'e'), 0 },
    // Yi
    { HB_MAKE_TAG('y', 'i', 0, 0), 0 },
    // ZanabazarSquare
    { HB_MAKE_TAG('z', 'a', 'n', 'b'), 0 },
    // Zawgyi
    { DefaultScript, 0 },
};
static const int NumOTScripts = sizeof(ot_scripts)/sizeof(OTScripts);

static HB_Bool checkScript(HB_Face face, int script)
{
    assert(script <= HB_ScriptCount);
    // HB_ScriptCount is the last script, first script is 0
    assert((NumOTScripts - 1) == HB_ScriptCount);
    assert((NumEngines - 1) == HB_ScriptCount);

    if (!face->gsub && !face->gpos)
        return false;

    unsigned int tag = ot_scripts[script].tag;
    int requirements = ot_scripts[script].flags;

    if (requirements & RequiresGsub) {
        if (!face->gsub)
            return false;

        HB_UShort script_index;
        HB_Error error = HB_GSUB_Select_Script(face->gsub, tag, &script_index);
        if (error) {
            DEBUG("could not select script %d in GSub table: %d", (int)script, error);
            error = HB_GSUB_Select_Script(face->gsub, DefaultScript, &script_index);
            if (error)
                return false;
        }
    }

    if (requirements & RequiresGpos) {
        if (!face->gpos)
            return false;

        HB_UShort script_index;
        HB_Error error = HB_GPOS_Select_Script(face->gpos, script, &script_index);
        if (error) {
            DEBUG("could not select script in gpos table: %d", error);
            error = HB_GPOS_Select_Script(face->gpos, DefaultScript, &script_index);
            if (error)
                return false;
        }

    }
    return true;
}

static HB_Stream getTableStream(void *font, HB_GetFontTableFunc tableFunc, HB_Tag tag)
{
    HB_Error error;
    HB_UInt length = 0;
    HB_Stream stream = 0;

    if (!font)
        return 0;

    error = tableFunc(font, tag, 0, &length);
    if (error)
        return 0;
    stream = (HB_Stream)malloc(sizeof(HB_StreamRec));
    if (!stream)
        return 0;
    stream->base = (HB_Byte*)malloc(length*sizeof(HB_Byte));
    if (!stream->base) {
        _hb_close_stream(stream);
        return 0;
    }
    error = tableFunc(font, tag, stream->base, &length);
    if (error) {
        _hb_close_stream(stream);
        return 0;
    }
    stream->size = length;
    stream->pos = 0;
    stream->cursor = NULL;
    return stream;
}

HB_Face HB_NewFace(void *font, HB_GetFontTableFunc tableFunc)
{
    HB_Face face = (HB_Face )malloc(sizeof(HB_FaceRec));
    if (!face)
        return 0;

    face->isSymbolFont = false;
    face->gdef = 0;
    face->gpos = 0;
    face->gsub = 0;
    face->current_script = HB_ScriptCount;
    face->current_flags = HB_ShaperFlag_Default;
    face->has_opentype_kerning = false;
    face->tmpAttributes = 0;
    face->tmpLogClusters = 0;
    face->glyphs_substituted = false;
    face->buffer = 0;

    HB_Error error = HB_Err_Ok;
    HB_Stream stream;
    HB_Stream gdefStream;

    gdefStream = getTableStream(font, tableFunc, TTAG_GDEF);
    error = HB_Err_Not_Covered;
    if (!gdefStream || (error = HB_Load_GDEF_Table(gdefStream, &face->gdef))) {
        //DEBUG("error loading gdef table: %d", error);
        face->gdef = 0;
    }

    //DEBUG() << "trying to load gsub table";
    stream = getTableStream(font, tableFunc, TTAG_GSUB);
    error = HB_Err_Not_Covered;
    if (!stream || (error = HB_Load_GSUB_Table(stream, &face->gsub, face->gdef, gdefStream))) {
        face->gsub = 0;
        if (error != HB_Err_Not_Covered) {
            //DEBUG("error loading gsub table: %d", error);
        } else {
            //DEBUG("face doesn't have a gsub table");
        }
    }
    _hb_close_stream(stream);

    stream = getTableStream(font, tableFunc, TTAG_GPOS);
    error = HB_Err_Not_Covered;
    if (!stream || (error = HB_Load_GPOS_Table(stream, &face->gpos, face->gdef, gdefStream))) {
        face->gpos = 0;
        DEBUG("error loading gpos table: %d", error);
    }
    _hb_close_stream(stream);

    _hb_close_stream(gdefStream);

    for (unsigned int i = 0; i < HB_ScriptCount; ++i)
        face->supported_scripts[i] = checkScript(face, i);

    if (hb_buffer_new(&face->buffer) != HB_Err_Ok) {
        HB_FreeFace(face);
        return 0;
    }

    return face;
}

void HB_FreeFace(HB_Face face)
{
    if (!face)
        return;
    if (face->gpos)
        HB_Done_GPOS_Table(face->gpos);
    if (face->gsub)
        HB_Done_GSUB_Table(face->gsub);
    if (face->gdef)
        HB_Done_GDEF_Table(face->gdef);
    if (face->buffer)
        hb_buffer_free(face->buffer);
    if (face->tmpAttributes)
        free(face->tmpAttributes);
    if (face->tmpLogClusters)
        free(face->tmpLogClusters);
    free(face);
}

HB_Bool HB_SelectScript(HB_ShaperItem *shaper_item, const HB_OpenTypeFeature *features)
{
    HB_Script script = shaper_item->item.script;

    if (!shaper_item->face->supported_scripts[script])
        return false;

    HB_Face face = shaper_item->face;
    if (face->current_script == script && face->current_flags == shaper_item->shaperFlags)
        return true;

    face->current_script = script;
    face->current_flags = shaper_item->shaperFlags;

    assert(script <= HB_ScriptCount);
    // find script in our list of supported scripts.
    unsigned int tag = ot_scripts[script].tag;

    if (face->gsub && features) {
#ifdef OT_DEBUG
        {
            HB_FeatureList featurelist = face->gsub->FeatureList;
            int numfeatures = featurelist.FeatureCount;
            DEBUG("gsub table has %d features", numfeatures);
            for (int i = 0; i < numfeatures; i++) {
                HB_FeatureRecord *r = featurelist.FeatureRecord + i;
                DEBUG("   feature '%s'", tag_to_string(r->FeatureTag));
            }
        }
#endif
        HB_GSUB_Clear_Features(face->gsub);
        HB_UShort script_index;
        HB_Error error = HB_GSUB_Select_Script(face->gsub, tag, &script_index);
        if (!error) {
            DEBUG("script %s has script index %d", tag_to_string(script), script_index);
            while (features->tag) {
                HB_UShort feature_index;
                error = HB_GSUB_Select_Feature(face->gsub, features->tag, script_index, 0xffff, &feature_index);
                if (!error) {
                    DEBUG("  adding feature %s", tag_to_string(features->tag));
                    HB_GSUB_Add_Feature(face->gsub, feature_index, features->property);
                }
                ++features;
            }
        }
    }

    // reset
    face->has_opentype_kerning = false;

    if (face->gpos) {
        HB_GPOS_Clear_Features(face->gpos);
        HB_UShort script_index;
        HB_Error error = HB_GPOS_Select_Script(face->gpos, tag, &script_index);
        if (!error) {
#ifdef OT_DEBUG
            {
                HB_FeatureList featurelist = face->gpos->FeatureList;
                int numfeatures = featurelist.FeatureCount;
                DEBUG("gpos table has %d features", numfeatures);
                for(int i = 0; i < numfeatures; i++) {
                    HB_FeatureRecord *r = featurelist.FeatureRecord + i;
                    HB_UShort feature_index;
                    HB_GPOS_Select_Feature(face->gpos, r->FeatureTag, script_index, 0xffff, &feature_index);
                    DEBUG("   feature '%s'", tag_to_string(r->FeatureTag));
                }
            }
#endif
            HB_UInt *feature_tag_list_buffer;
            error = HB_GPOS_Query_Features(face->gpos, script_index, 0xffff, &feature_tag_list_buffer);
            if (!error) {
                HB_UInt *feature_tag_list = feature_tag_list_buffer;
                while (*feature_tag_list) {
                    HB_UShort feature_index;
                    if (*feature_tag_list == HB_MAKE_TAG('k', 'e', 'r', 'n')) {
                        if (face->current_flags & HB_ShaperFlag_NoKerning) {
                            ++feature_tag_list;
                            continue;
                        }
                        face->has_opentype_kerning = true;
                    }
                    error = HB_GPOS_Select_Feature(face->gpos, *feature_tag_list, script_index, 0xffff, &feature_index);
                    if (!error)
                        HB_GPOS_Add_Feature(face->gpos, feature_index, PositioningProperties);
                    ++feature_tag_list;
                }
                FREE(feature_tag_list_buffer);
            }
        }
    }

    return true;
}

HB_Bool HB_OpenTypeShape(HB_ShaperItem *item, const hb_uint32 *properties)
{
    HB_GlyphAttributes *tmpAttributes;
    unsigned int *tmpLogClusters;

    HB_Face face = item->face;

    face->length = item->num_glyphs;

    hb_buffer_clear(face->buffer);

    tmpAttributes = (HB_GlyphAttributes *) realloc(face->tmpAttributes, face->length*sizeof(HB_GlyphAttributes));
    if (!tmpAttributes)
        return false;
    face->tmpAttributes = tmpAttributes;

    tmpLogClusters = (unsigned int *) realloc(face->tmpLogClusters, face->length*sizeof(unsigned int));
    if (!tmpLogClusters)
        return false;
    face->tmpLogClusters = tmpLogClusters;

    for (int i = 0; i < face->length; ++i) {
        hb_buffer_add_glyph(face->buffer, item->glyphs[i], properties ? properties[i] : 0, i);
        face->tmpAttributes[i] = item->attributes[i];
        face->tmpLogClusters[i] = item->log_clusters[i];
    }

#ifdef OT_DEBUG
    DEBUG("-----------------------------------------");
//     DEBUG("log clusters before shaping:");
//     for (int j = 0; j < length; j++)
//         DEBUG("    log[%d] = %d", j, item->log_clusters[j]);
    DEBUG("original glyphs: %p", item->glyphs);
    for (int i = 0; i < length; ++i)
        DEBUG("   glyph=%4x", hb_buffer->in_string[i].gindex);
//     dump_string(hb_buffer);
#endif

    face->glyphs_substituted = false;
    if (face->gsub) {
        unsigned int error = HB_GSUB_Apply_String(face->gsub, face->buffer);
        if (error && error != HB_Err_Not_Covered)
            return false;
        face->glyphs_substituted = (error != HB_Err_Not_Covered);
    }

#ifdef OT_DEBUG
//     DEBUG("log clusters before shaping:");
//     for (int j = 0; j < length; j++)
//         DEBUG("    log[%d] = %d", j, item->log_clusters[j]);
    DEBUG("shaped glyphs:");
    for (int i = 0; i < length; ++i)
        DEBUG("   glyph=%4x", hb_buffer->in_string[i].gindex);
    DEBUG("-----------------------------------------");
//     dump_string(hb_buffer);
#endif

    return true;
}

HB_Bool HB_OpenTypePosition(HB_ShaperItem *item, int availableGlyphs, HB_Bool doLogClusters)
{
    HB_Face face = item->face;

    bool glyphs_positioned = false;
    if (face->gpos) {
        if (face->buffer->positions)
            memset(face->buffer->positions, 0, face->buffer->in_length*sizeof(HB_PositionRec));
        // #### check that passing "false,false" is correct
        glyphs_positioned = HB_GPOS_Apply_String(item->font, face->gpos, face->current_flags, face->buffer, false, false) != HB_Err_Not_Covered;
    }

    if (!face->glyphs_substituted && !glyphs_positioned) {
        HB_GetGlyphAdvances(item);
        return true; // nothing to do for us
    }

    // make sure we have enough space to write everything back
    if (availableGlyphs < (int)face->buffer->in_length) {
        item->num_glyphs = face->buffer->in_length;
        return false;
    }

    HB_Glyph *glyphs = item->glyphs;
    HB_GlyphAttributes *attributes = item->attributes;

    for (unsigned int i = 0; i < face->buffer->in_length; ++i) {
        glyphs[i] = face->buffer->in_string[i].gindex;
        attributes[i] = face->tmpAttributes[face->buffer->in_string[i].cluster];
        if (i && face->buffer->in_string[i].cluster == face->buffer->in_string[i-1].cluster)
            attributes[i].clusterStart = false;
    }
    item->num_glyphs = face->buffer->in_length;

    if (doLogClusters && face->glyphs_substituted) {
        // we can't do this for indic, as we pass the stuf in syllables and it's easier to do it in the shaper.
        unsigned short *logClusters = item->log_clusters;
        int clusterStart = 0;
        int oldCi = 0;
        // #### the reconstruction of the logclusters currently does not work if the original string
        // contains surrogate pairs
        for (unsigned int i = 0; i < face->buffer->in_length; ++i) {
            int ci = face->buffer->in_string[i].cluster;
            //         DEBUG("   ci[%d] = %d mark=%d, cmb=%d, cs=%d",
            //                i, ci, glyphAttributes[i].mark, glyphAttributes[i].combiningClass, glyphAttributes[i].clusterStart);
            if (!attributes[i].mark && attributes[i].clusterStart && ci != oldCi) {
                for (int j = oldCi; j < ci; j++)
                    logClusters[j] = clusterStart;
                clusterStart = i;
                oldCi = ci;
            }
        }
        for (int j = oldCi; j < face->length; j++)
            logClusters[j] = clusterStart;
    }

    // calulate the advances for the shaped glyphs
//     DEBUG("unpositioned: ");

    // positioning code:
    if (glyphs_positioned) {
        HB_GetGlyphAdvances(item);
        HB_Position positions = face->buffer->positions;
        HB_Fixed *advances = item->advances;

//         DEBUG("positioned glyphs:");
        for (unsigned int i = 0; i < face->buffer->in_length; i++) {
//             DEBUG("    %d:\t orig advance: (%d/%d)\tadv=(%d/%d)\tpos=(%d/%d)\tback=%d\tnew_advance=%d", i,
//                    glyphs[i].advance.x.toInt(), glyphs[i].advance.y.toInt(),
//                    (int)(positions[i].x_advance >> 6), (int)(positions[i].y_advance >> 6),
//                    (int)(positions[i].x_pos >> 6), (int)(positions[i].y_pos >> 6),
//                    positions[i].back, positions[i].new_advance);

            HB_Fixed adjustment = positions[i].x_advance;

            if (!(face->current_flags & HB_ShaperFlag_UseDesignMetrics))
                adjustment = HB_FIXED_ROUND(adjustment);

            if (positions[i].new_advance) {
                ; //advances[i] = adjustment;
            } else {
                advances[i] += adjustment;
            }

            int back = 0;
            HB_FixedPoint *offsets = item->offsets;
            offsets[i].x = positions[i].x_pos;
            offsets[i].y = positions[i].y_pos;
            while (positions[i - back].back) {
                back += positions[i - back].back;
                offsets[i].x += positions[i - back].x_pos;
                offsets[i].y += positions[i - back].y_pos;
            }
            offsets[i].y = -offsets[i].y;

            if (item->item.bidiLevel % 2) {
                // ### may need to go back multiple glyphs like in ltr
                back = positions[i].back;
                while (back--)
                    offsets[i].x -= advances[i-back];
            } else {
                back = 0;
                while (positions[i - back].back) {
                    back += positions[i - back].back;
                    offsets[i].x -= advances[i-back];
                }
            }
//             DEBUG("   ->\tadv=%d\tpos=(%d/%d)",
//                    glyphs[i].advance.x.toInt(), glyphs[i].offset.x.toInt(), glyphs[i].offset.y.toInt());
        }
        item->kerning_applied = face->has_opentype_kerning;
    } else {
        HB_HeuristicPosition(item);
    }

#ifdef OT_DEBUG
    if (doLogClusters) {
        DEBUG("log clusters after shaping:");
        for (int j = 0; j < length; j++)
            DEBUG("    log[%d] = %d", j, item->log_clusters[j]);
    }
    DEBUG("final glyphs:");
    for (int i = 0; i < (int)hb_buffer->in_length; ++i)
        DEBUG("   glyph=%4x char_index=%d mark: %d cmp: %d, clusterStart: %d advance=%d/%d offset=%d/%d",
               glyphs[i].glyph, hb_buffer->in_string[i].cluster, glyphs[i].attributes.mark,
               glyphs[i].attributes.combiningClass, glyphs[i].attributes.clusterStart,
               glyphs[i].advance.x.toInt(), glyphs[i].advance.y.toInt(),
               glyphs[i].offset.x.toInt(), glyphs[i].offset.y.toInt());
    DEBUG("-----------------------------------------");
#endif
    return true;
}

HB_Bool HB_ShapeItem(HB_ShaperItem *shaper_item)
{
    HB_Bool result = false;
    if (shaper_item->num_glyphs < shaper_item->item.length) {
        shaper_item->num_glyphs = shaper_item->item.length;
        return false;
    }
    assert(shaper_item->item.script <= HB_ScriptCount);
    result = HB_ScriptEngines[shaper_item->item.script].shape(shaper_item);
    shaper_item->glyphIndicesPresent = false;
    return result;
}

