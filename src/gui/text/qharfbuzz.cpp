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
#include "qlibrary.h"
#include "qtextcodec.h"
#include "qharfbuzz_p.h"

QT_BEGIN_NAMESPACE

extern "C" {

HB_GraphemeClass HB_GetGraphemeClass(HB_UChar32 ch)
{
    return (HB_GraphemeClass) QUnicodeTables::graphemeBreak(ch);
}

HB_WordClass HB_GetWordClass(HB_UChar32 ch)
{
    return (HB_WordClass) QUnicodeTables::wordBreak(ch);
}

HB_SentenceClass HB_GetSentenceClass(HB_UChar32 ch)
{
    return (HB_SentenceClass) QUnicodeTables::sentenceBreak(ch);
}

HB_LineBreakClass HB_GetLineBreakClass(HB_UChar32 ch)
{
    return (HB_LineBreakClass)QUnicodeTables::lineBreakClass(ch);
}


void HB_GetGraphemeAndLineBreakClass(HB_UChar32 ch, HB_GraphemeClass *grapheme, HB_LineBreakClass *lineBreak)
{
    *grapheme = (HB_GraphemeClass) QUnicodeTables::graphemeBreak(ch);
    *lineBreak = (HB_LineBreakClass) QUnicodeTables::lineBreakClass(ch);
}

void HB_GetUnicodeCharProperties(HB_UChar32 ch, HB_CharCategory *category, int *combiningClass)
{
    *category = (HB_CharCategory)QChar::category(ch);
    *combiningClass = QUnicodeTables::combiningClass(ch);
}

HB_CharCategory HB_GetUnicodeCharCategory(HB_UChar32 ch)
{
    return (HB_CharCategory)QChar::category(ch);
}

int HB_GetUnicodeCharCombiningClass(HB_UChar32 ch)
{
    return QUnicodeTables::combiningClass(ch);
}

HB_UChar16 HB_GetMirroredChar(HB_UChar16 ch)
{
    return QChar::mirroredChar(ch);
}

void *HB_Library_Resolve(const char *library, int version, const char *symbol)
{
#ifdef QT_NO_LIBRARY
    return 0;
#else
    return QLibrary::resolve(QString::fromLatin1(library), version, symbol);
#endif
}

} // extern "C"

HB_Bool qShapeItem(HB_ShaperItem *item)
{
    return HB_ShapeItem(item);
}

HB_Face qHBNewFace(void *font, HB_GetFontTableFunc tableFunc)
{
    return HB_NewFace(font, tableFunc);
}

void qHBFreeFace(HB_Face face)
{
    HB_FreeFace(face);
}

void qGetCharAttributes(const HB_UChar16 *string, hb_uint32 stringLength,
                        const HB_ScriptItem *items, hb_uint32 numItems,
                        HB_CharAttributes *attributes)
{
    HB_GetCharAttributes(string, stringLength, items, numItems, attributes);
}

QT_END_NAMESPACE
