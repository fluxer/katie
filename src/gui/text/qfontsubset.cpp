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
#include "qdebug.h"
#include "qfontsubset_p.h"
#include "qendian.h"
#include "qpainterpath.h"
#include "qpdf_p.h"
#include "qcorecommon_p.h"

#ifdef Q_WS_X11
#include "qfontengine_ft_p.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#ifndef QT_NO_PRINTER

QT_BEGIN_NAMESPACE

static const char * const agl =
".notdef\0space\0exclam\0quotedbl\0numbersign\0dollar\0percent\0ampersand\0"
"quotesingle\0parenleft\0parenright\0asterisk\0plus\0comma\0hyphen\0period\0"
"slash\0zero\0one\0two\0three\0four\0five\0six\0seven\0eight\0nine\0colon\0"
"semicolon\0less\0equal\0greater\0question\0at\0A\0B\0C\0D\0E\0F\0G\0H\0I\0J\0"
"K\0L\0M\0N\0O\0P\0Q\0R\0S\0T\0U\0V\0W\0X\0Y\0Z\0bracketleft\0backslash\0"
"bracketright\0asciicircum\0underscore\0grave\0a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0"
"k\0l\0m\0n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0braceleft\0bar\0braceright\0"
"asciitilde\0space\0exclamdown\0cent\0sterling\0currency\0yen\0brokenbar\0"
"section\0dieresis\0copyright\0ordfeminine\0guillemotleft\0logicalnot\0"
"hyphen\0registered\0macron\0degree\0plusminus\0twosuperior\0threesuperior\0"
"acute\0mu\0paragraph\0periodcentered\0cedilla\0onesuperior\0ordmasculine\0"
"guillemotright\0onequarter\0onehalf\0threequarters\0questiondown\0Agrave\0"
"Aacute\0Acircumflex\0Atilde\0Adieresis\0Aring\0AE\0Ccedilla\0Egrave\0Eacute\0"
"Ecircumflex\0Edieresis\0Igrave\0Iacute\0Icircumflex\0Idieresis\0Eth\0Ntilde\0"
"Ograve\0Oacute\0Ocircumflex\0Otilde\0Odieresis\0multiply\0Oslash\0Ugrave\0"
"Uacute\0Ucircumflex\0Udieresis\0Yacute\0Thorn\0germandbls\0agrave\0aacute\0"
"acircumflex\0atilde\0adieresis\0aring\0ae\0ccedilla\0egrave\0eacute\0"
"ecircumflex\0edieresis\0igrave\0iacute\0icircumflex\0idieresis\0eth\0ntilde\0"
"ograve\0oacute\0ocircumflex\0otilde\0odieresis\0divide\0oslash\0ugrave\0"
"uacute\0ucircumflex\0udieresis\0yacute\0thorn\0ydieresis\0Amacron\0amacron\0"
"Abreve\0abreve\0Aogonek\0aogonek\0Cacute\0cacute\0Ccircumflex\0ccircumflex\0"
"Cdotaccent\0cdotaccent\0Ccaron\0ccaron\0Dcaron\0dcaron\0Dcroat\0dcroat\0"
"Emacron\0emacron\0Ebreve\0ebreve\0Edotaccent\0edotaccent\0Eogonek\0eogonek\0"
"Ecaron\0ecaron\0Gcircumflex\0gcircumflex\0Gbreve\0gbreve\0Gdotaccent\0"
"gdotaccent\0Gcommaaccent\0gcommaaccent\0Hcircumflex\0hcircumflex\0Hbar\0"
"hbar\0Itilde\0itilde\0Imacron\0imacron\0Ibreve\0ibreve\0Iogonek\0iogonek\0"
"Idotaccent\0dotlessi\0IJ\0ij\0Jcircumflex\0jcircumflex\0Kcommaaccent\0"
"kcommaaccent\0kgreenlandic\0Lacute\0lacute\0Lcommaaccent\0lcommaaccent\0"
"Lcaron\0lcaron\0Ldot\0ldot\0Lslash\0lslash\0Nacute\0nacute\0Ncommaaccent\0"
"ncommaaccent\0Ncaron\0ncaron\0napostrophe\0Eng\0eng\0Omacron\0omacron\0"
"Obreve\0obreve\0Ohungarumlaut\0ohungarumlaut\0OE\0oe\0Racute\0racute\0"
"Rcommaaccent\0rcommaaccent\0Rcaron\0rcaron\0Sacute\0sacute\0Scircumflex\0"
"scircumflex\0Scedilla\0scedilla\0Scaron\0scaron\0Tcaron\0tcaron\0Tbar\0tbar\0"
"Utilde\0utilde\0Umacron\0umacron\0Ubreve\0ubreve\0Uring\0uring\0"
"Uhungarumlaut\0uhungarumlaut\0Uogonek\0uogonek\0Wcircumflex\0wcircumflex\0"
"Ycircumflex\0ycircumflex\0Ydieresis\0Zacute\0zacute\0Zdotaccent\0zdotaccent\0"
"Zcaron\0zcaron\0longs\0florin\0Ohorn\0ohorn\0Uhorn\0uhorn\0Gcaron\0gcaron\0"
"Aringacute\0aringacute\0AEacute\0aeacute\0Oslashacute\0oslashacute\0"
"Scommaaccent\0scommaaccent\0Tcommaaccent\0tcommaaccent\0afii57929\0"
"afii64937\0circumflex\0caron\0breve\0dotaccent\0ring\0ogonek\0tilde\0"
"hungarumlaut\0gravecomb\0acutecomb\0tildecomb\0hookabovecomb\0dotbelowcomb\0"
"tonos\0dieresistonos\0Alphatonos\0anoteleia\0Epsilontonos\0Etatonos\0"
"Iotatonos\0Omicrontonos\0Upsilontonos\0Omegatonos\0iotadieresistonos\0Alpha\0"
"Beta\0Gamma\0Delta\0Epsilon\0Zeta\0Eta\0Theta\0Iota\0Kappa\0Lambda\0Mu\0Nu\0"
"Xi\0Omicron\0Pi\0Rho\0Sigma\0Tau\0Upsilon\0Phi\0Chi\0Psi\0Omega\0"
"Iotadieresis\0Upsilondieresis\0alphatonos\0epsilontonos\0etatonos\0"
"iotatonos\0upsilondieresistonos\0alpha\0beta\0gamma\0delta\0epsilon\0zeta\0"
"eta\0theta\0iota\0kappa\0lambda\0mu\0nu\0xi\0omicron\0pi\0rho\0sigma1\0"
"sigma\0tau\0upsilon\0phi\0chi\0psi\0omega\0iotadieresis\0upsilondieresis\0"
;

static const struct UnicodeAglData {
    const quint16 u;
    const quint16 index;
} unicode_to_aglindex[] = {
    {0x0000, 0}, {0x0020, 8}, {0x0021, 14}, {0x0022, 21},
    {0x0023, 30}, {0x0024, 41}, {0x0025, 48}, {0x0026, 56},
    {0x0027, 66}, {0x0028, 78}, {0x0029, 88}, {0x002A, 99},
    {0x002B, 108}, {0x002C, 113}, {0x002D, 119}, {0x002E, 126},
    {0x002F, 133}, {0x0030, 139}, {0x0031, 144}, {0x0032, 148},
    {0x0033, 152}, {0x0034, 158}, {0x0035, 163}, {0x0036, 168},
    {0x0037, 172}, {0x0038, 178}, {0x0039, 184}, {0x003A, 189},
    {0x003B, 195}, {0x003C, 205}, {0x003D, 210}, {0x003E, 216},
    {0x003F, 224}, {0x0040, 233}, {0x0041, 236}, {0x0042, 238},
    {0x0043, 240}, {0x0044, 242}, {0x0045, 244}, {0x0046, 246},
    {0x0047, 248}, {0x0048, 250}, {0x0049, 252}, {0x004A, 254},
    {0x004B, 256}, {0x004C, 258}, {0x004D, 260}, {0x004E, 262},
    {0x004F, 264}, {0x0050, 266}, {0x0051, 268}, {0x0052, 270},
    {0x0053, 272}, {0x0054, 274}, {0x0055, 276}, {0x0056, 278},
    {0x0057, 280}, {0x0058, 282}, {0x0059, 284}, {0x005A, 286},
    {0x005B, 288}, {0x005C, 300}, {0x005D, 310}, {0x005E, 323},
    {0x005F, 335}, {0x0060, 346}, {0x0061, 352}, {0x0062, 354},
    {0x0063, 356}, {0x0064, 358}, {0x0065, 360}, {0x0066, 362},
    {0x0067, 364}, {0x0068, 366}, {0x0069, 368}, {0x006A, 370},
    {0x006B, 372}, {0x006C, 374}, {0x006D, 376}, {0x006E, 378},
    {0x006F, 380}, {0x0070, 382}, {0x0071, 384}, {0x0072, 386},
    {0x0073, 388}, {0x0074, 390}, {0x0075, 392}, {0x0076, 394},
    {0x0077, 396}, {0x0078, 398}, {0x0079, 400}, {0x007A, 402},
    {0x007B, 404}, {0x007C, 414}, {0x007D, 418}, {0x007E, 429},
    {0x00A0, 440}, {0x00A1, 446}, {0x00A2, 457}, {0x00A3, 462},
    {0x00A4, 471}, {0x00A5, 480}, {0x00A6, 484}, {0x00A7, 494},
    {0x00A8, 502}, {0x00A9, 511}, {0x00AA, 521}, {0x00AB, 533},
    {0x00AC, 547}, {0x00AD, 558}, {0x00AE, 565}, {0x00AF, 576},
    {0x00B0, 583}, {0x00B1, 590}, {0x00B2, 600}, {0x00B3, 612},
    {0x00B4, 626}, {0x00B5, 632}, {0x00B6, 635}, {0x00B7, 645},
    {0x00B8, 660}, {0x00B9, 668}, {0x00BA, 680}, {0x00BB, 693},
    {0x00BC, 708}, {0x00BD, 719}, {0x00BE, 727}, {0x00BF, 741},
    {0x00C0, 754}, {0x00C1, 761}, {0x00C2, 768}, {0x00C3, 780},
    {0x00C4, 787}, {0x00C5, 797}, {0x00C6, 803}, {0x00C7, 806},
    {0x00C8, 815}, {0x00C9, 822}, {0x00CA, 829}, {0x00CB, 841},
    {0x00CC, 851}, {0x00CD, 858}, {0x00CE, 865}, {0x00CF, 877},
    {0x00D0, 887}, {0x00D1, 891}, {0x00D2, 898}, {0x00D3, 905},
    {0x00D4, 912}, {0x00D5, 924}, {0x00D6, 931}, {0x00D7, 941},
    {0x00D8, 950}, {0x00D9, 957}, {0x00DA, 964}, {0x00DB, 971},
    {0x00DC, 983}, {0x00DD, 993}, {0x00DE, 1000}, {0x00DF, 1006},
    {0x00E0, 1017}, {0x00E1, 1024}, {0x00E2, 1031}, {0x00E3, 1043},
    {0x00E4, 1050}, {0x00E5, 1060}, {0x00E6, 1066}, {0x00E7, 1069},
    {0x00E8, 1078}, {0x00E9, 1085}, {0x00EA, 1092}, {0x00EB, 1104},
    {0x00EC, 1114}, {0x00ED, 1121}, {0x00EE, 1128}, {0x00EF, 1140},
    {0x00F0, 1150}, {0x00F1, 1154}, {0x00F2, 1161}, {0x00F3, 1168},
    {0x00F4, 1175}, {0x00F5, 1187}, {0x00F6, 1194}, {0x00F7, 1204},
    {0x00F8, 1211}, {0x00F9, 1218}, {0x00FA, 1225}, {0x00FB, 1232},
    {0x00FC, 1244}, {0x00FD, 1254}, {0x00FE, 1261}, {0x00FF, 1267},
    {0x0100, 1277}, {0x0101, 1285}, {0x0102, 1293}, {0x0103, 1300},
    {0x0104, 1307}, {0x0105, 1315}, {0x0106, 1323}, {0x0107, 1330},
    {0x0108, 1337}, {0x0109, 1349}, {0x010A, 1361}, {0x010B, 1372},
    {0x010C, 1383}, {0x010D, 1390}, {0x010E, 1397}, {0x010F, 1404},
    {0x0110, 1411}, {0x0111, 1418}, {0x0112, 1425}, {0x0113, 1433},
    {0x0114, 1441}, {0x0115, 1448}, {0x0116, 1455}, {0x0117, 1466},
    {0x0118, 1477}, {0x0119, 1485}, {0x011A, 1493}, {0x011B, 1500},
    {0x011C, 1507}, {0x011D, 1519}, {0x011E, 1531}, {0x011F, 1538},
    {0x0120, 1545}, {0x0121, 1556}, {0x0122, 1567}, {0x0123, 1580},
    {0x0124, 1593}, {0x0125, 1605}, {0x0126, 1617}, {0x0127, 1622},
    {0x0128, 1627}, {0x0129, 1634}, {0x012A, 1641}, {0x012B, 1649},
    {0x012C, 1657}, {0x012D, 1664}, {0x012E, 1671}, {0x012F, 1679},
    {0x0130, 1687}, {0x0131, 1698}, {0x0132, 1707}, {0x0133, 1710},
    {0x0134, 1713}, {0x0135, 1725}, {0x0136, 1737}, {0x0137, 1750},
    {0x0138, 1763}, {0x0139, 1776}, {0x013A, 1783}, {0x013B, 1790},
    {0x013C, 1803}, {0x013D, 1816}, {0x013E, 1823}, {0x013F, 1830},
    {0x0140, 1835}, {0x0141, 1840}, {0x0142, 1847}, {0x0143, 1854},
    {0x0144, 1861}, {0x0145, 1868}, {0x0146, 1881}, {0x0147, 1894},
    {0x0148, 1901}, {0x0149, 1908}, {0x014A, 1920}, {0x014B, 1924},
    {0x014C, 1928}, {0x014D, 1936}, {0x014E, 1944}, {0x014F, 1951},
    {0x0150, 1958}, {0x0151, 1972}, {0x0152, 1986}, {0x0153, 1989},
    {0x0154, 1992}, {0x0155, 1999}, {0x0156, 2006}, {0x0157, 2019},
    {0x0158, 2032}, {0x0159, 2039}, {0x015A, 2046}, {0x015B, 2053},
    {0x015C, 2060}, {0x015D, 2072}, {0x015E, 2084}, {0x015F, 2093},
    {0x0160, 2102}, {0x0161, 2109}, {0x0164, 2116}, {0x0165, 2123},
    {0x0166, 2130}, {0x0167, 2135}, {0x0168, 2140}, {0x0169, 2147},
    {0x016A, 2154}, {0x016B, 2162}, {0x016C, 2170}, {0x016D, 2177},
    {0x016E, 2184}, {0x016F, 2190}, {0x0170, 2196}, {0x0171, 2210},
    {0x0172, 2224}, {0x0173, 2232}, {0x0174, 2240}, {0x0175, 2252},
    {0x0176, 2264}, {0x0177, 2276}, {0x0178, 2288}, {0x0179, 2298},
    {0x017A, 2305}, {0x017B, 2312}, {0x017C, 2323}, {0x017D, 2334},
    {0x017E, 2341}, {0x017F, 2348}, {0x0192, 2354}, {0x01A0, 2361},
    {0x01A1, 2367}, {0x01AF, 2373}, {0x01B0, 2379}, {0x01E6, 2385},
    {0x01E7, 2392}, {0x01FA, 2399}, {0x01FB, 2410}, {0x01FC, 2421},
    {0x01FD, 2429}, {0x01FE, 2437}, {0x01FF, 2449}, {0x0218, 2461},
    {0x0219, 2474}, {0x021A, 2487}, {0x021B, 2500}, {0x02BC, 2513},
    {0x02BD, 2523}, {0x02C6, 2533}, {0x02C7, 2544}, {0x02D8, 2550},
    {0x02D9, 2556}, {0x02DA, 2566}, {0x02DB, 2571}, {0x02DC, 2578},
    {0x02DD, 2584}, {0x0300, 2597}, {0x0301, 2607}, {0x0303, 2617},
    {0x0309, 2627}, {0x0323, 2641}, {0x0384, 2654}, {0x0385, 2660},
    {0x0386, 2674}, {0x0387, 2685}, {0x0388, 2695}, {0x0389, 2708},
    {0x038A, 2717}, {0x038C, 2727}, {0x038E, 2740}, {0x038F, 2753},
    {0x0390, 2764}, {0x0391, 2782}, {0x0392, 2788}, {0x0393, 2793},
    {0x0394, 2799}, {0x0395, 2805}, {0x0396, 2813}, {0x0397, 2818},
    {0x0398, 2822}, {0x0399, 2828}, {0x039A, 2833}, {0x039B, 2839},
    {0x039C, 2846}, {0x039D, 2849}, {0x039E, 2852}, {0x039F, 2855},
    {0x03A0, 2863}, {0x03A1, 2866}, {0x03A3, 2870}, {0x03A4, 2876},
    {0x03A5, 2880}, {0x03A6, 2888}, {0x03A7, 2892}, {0x03A8, 2896},
    {0x03A9, 2900}, {0x03AA, 2906}, {0x03AB, 2919}, {0x03AC, 2935},
    {0x03AD, 2946}, {0x03AE, 2959}, {0x03AF, 2968}, {0x03B0, 2978},
    {0x03B1, 2999}, {0x03B2, 3005}, {0x03B3, 3010}, {0x03B4, 3016},
    {0x03B5, 3022}, {0x03B6, 3030}, {0x03B7, 3035}, {0x03B8, 3039},
    {0x03B9, 3045}, {0x03BA, 3050}, {0x03BB, 3056}, {0x03BC, 3063},
    {0x03BD, 3066}, {0x03BE, 3069}, {0x03BF, 3072}, {0x03C0, 3080},
    {0x03C1, 3083}, {0x03C2, 3087}, {0x03C3, 3094}, {0x03C4, 3100},
    {0x03C5, 3104}, {0x03C6, 3112}, {0x03C7, 3116}, {0x03C8, 3120},
    {0x03C9, 3124}, {0x03CA, 3130}, {0x03CB, 3143}, {0x03CC, 3159},
    {0x03CD, 3172}, {0x03CE, 3185}, {0x03D1, 3196}, {0x03D2, 3203},
    {0x03D5, 3212}, {0x03D6, 3217}, {0xFFFF, 3224}
};

// ---------------------------- PS/PDF helper methods -----------------------------------

QByteArray QFontSubset::glyphName(unsigned short unicode)
{
    int l = 0;
    while(unicode_to_aglindex[l].u < unicode)
        l++;
    if (unicode_to_aglindex[l].u == unicode)
        return agl + unicode_to_aglindex[l].index;

    QSTACKARRAY(char, buffer, 8);
    buffer[0] = 'u';
    buffer[1] = 'n';
    buffer[2] = 'i';
    QPdf::toHex(unicode, buffer+3);
    return QByteArray(buffer);
}

static FT_Face ft_face(const QFontEngine *engine)
{
    if (engine->type() == QFontEngine::Freetype) {
        const QFontEngineFT *ft = static_cast<const QFontEngineFT *>(engine);
        return ft->getFace();
    }
    return 0;
}

QByteArray QFontSubset::glyphName(unsigned int glyph, const QVector<int> &reverseMap) const
{
    uint glyphIndex = glyph_indices[glyph];

    if (glyphIndex == 0)
        return "/.notdef";

    QByteArray ba;
    QPdf::ByteStream s(&ba);
    FT_Face face = ft_face(fontEngine);

    QSTACKARRAY(char, name, 32);
    if (face && FT_HAS_GLYPH_NAMES(face)) {
        FT_Get_Glyph_Name(face, glyphIndex, &name, sizeof(name));
        if (name[0] == '.') // fix broken PS fonts returning .notdef for many glyphs
            name[0] = 0;
    }
    if (name[0]) {
        s << '/' << name;
    } else if (reverseMap[glyphIndex] && reverseMap[glyphIndex] < 0x10000) {
        s << '/' << glyphName(reverseMap[glyphIndex]);
    } else {
        s << "/gl" << (int)glyphIndex;
    }
    return ba;
}


QByteArray QFontSubset::widthArray() const
{
    Q_ASSERT(!widths.isEmpty());

    QFontEngine::Properties properties = fontEngine->properties();

    QByteArray width;
    QPdf::ByteStream s(&width);
    static const QFixed scale = QFixed(1000)/QFixed(2048);

    QFixed defWidth = widths[0];
    //qDebug("defWidth=%d, scale=%f", defWidth.toInt(), scale.toReal());
    for (int i = 0; i < glyph_indices.size(); ++i) {
        if (defWidth != widths[i])
            defWidth = 0;
    }
    if (defWidth > 0) {
        s << "/DW " << (defWidth*scale).toInt();
    } else {
        s << "/W [";
        for (int g = 0; g < glyph_indices.size();) {
            QFixed w = widths[g];
            int start = g;
            int startLinear = 0;
            ++g;
            while (g < glyph_indices.size()) {
                QFixed nw = widths[g];
                if (nw == w) {
                if (!startLinear)
                    startLinear = g - 1;
                } else {
                    if (startLinear > 0 && g - startLinear >= 10)
                        break;
                    startLinear = 0;
                }
                w = nw;
                ++g;
            }
            // qDebug("start=%x startLinear=%x g-1=%x",start,startLinear,g-1);
            if (g - startLinear < 10)
                startLinear = 0;
            int endnonlinear = startLinear ? startLinear : g;
            // qDebug("    startLinear=%x endnonlinear=%x", startLinear,endnonlinear);
            if (endnonlinear > start) {
                s << start << '[';
                for (int i = start; i < endnonlinear; ++i)
                    s << (widths[i]*scale).toInt();
                s << "]\n";
            }
            if (startLinear)
                s << startLinear << g - 1 << (widths[startLinear]*scale).toInt() << '\n';
        }
        s << "]\n";
    }
    return width;
}

static void checkRanges(QPdf::ByteStream &ts, QByteArray &ranges, int &nranges)
{
    if (++nranges > 100) {
        ts << nranges << "beginbfrange\n"
           << ranges << "endbfrange\n";
        ranges = QByteArray();
        nranges = 0;
    }
}

QVector<int> QFontSubset::getReverseMap() const
{
    QVector<int> reverseMap;
    reverseMap.resize(0x10000);
    for (uint i = 0; i < 0x10000; ++i)
        reverseMap[i] = 0;
    QGlyphLayoutArray<2> glyphs;
    for (uint uc = 0; uc < 0x10000; ++uc) {
        QChar ch(uc);
        int nglyphs = 1;
        fontEngine->stringToCMap(&ch, 1, &glyphs, &nglyphs, QTextEngine::GlyphIndicesOnly);
        int idx = glyph_indices.indexOf(glyphs.glyphs[0]);
        if (idx >= 0 && !reverseMap.at(idx))
            reverseMap[idx] = uc;
    }
    return reverseMap;
}

QByteArray QFontSubset::createToUnicodeMap() const
{
    QVector<int> reverseMap = getReverseMap();

    QByteArray touc;
    QPdf::ByteStream ts(&touc);
    ts << "/CIDInit /ProcSet findresource begin\n"
        "12 dict begin\n"
        "begincmap\n"
        "/CIDSystemInfo << /Registry (Adobe) /Ordering (UCS) /Supplement 0 >> def\n"
        "/CMapName /Adobe-Identity-UCS def\n"
        "/CMapType 2 def\n"
        "1 begincodespacerange\n"
        "<0000> <FFFF>\n"
        "endcodespacerange\n";

    int nranges = 1;
    QByteArray ranges = "<0000> <0000> <0000>\n";
    QPdf::ByteStream s(&ranges);

    QSTACKARRAY(char, buf, 5);
    for (int g = 1; g < glyph_indices.size(); ) {
        int uc0 = reverseMap.at(g);
        if (!uc0) {
            ++g;
            continue;
        }
        int start = g;
        int startLinear = 0;
        ++g;
        while (g < glyph_indices.size()) {
            int uc = reverseMap[g];
            // cmaps can't have the high byte changing within one range, so we need to break on that as well
            if (!uc || (g>>8) != (start >> 8))
                break;
            if (uc == uc0 + 1) {
                if (!startLinear)
                    startLinear = g - 1;
            } else {
                if (startLinear > 0 && g - startLinear >= 10)
                    break;
                startLinear = 0;
            }
            uc0 = uc;
            ++g;
        }
        // qDebug("start=%x startLinear=%x g-1=%x",start,startLinear,g-1);
        if (g - startLinear < 10)
            startLinear = 0;
        int endnonlinear = startLinear ? startLinear : g;
        // qDebug("    startLinear=%x endnonlinear=%x", startLinear,endnonlinear);
        if (endnonlinear > start) {
            s << '<' << QPdf::toHex((ushort)start, buf) << "> <";
            s << QPdf::toHex((ushort)(endnonlinear - 1), buf) << "> ";
            if (endnonlinear == start + 1) {
                s << '<' << QPdf::toHex((ushort)reverseMap[start], buf) << ">\n";
            } else {
                s << '[';
                for (int i = start; i < endnonlinear; ++i) {
                    s << '<' << QPdf::toHex((ushort)reverseMap[i], buf) << "> ";
                }
                s << "]\n";
            }
            checkRanges(ts, ranges, nranges);
        }
        if (startLinear) {
            while (startLinear < g) {
                int len = g - startLinear;
                int uc_start = reverseMap[startLinear];
                int uc_end = uc_start + len - 1;
                if ((uc_end >> 8) != (uc_start >> 8))
                    len = 256 - (uc_start & 0xff);
                s << '<' << QPdf::toHex((ushort)startLinear, buf) << "> <";
                s << QPdf::toHex((ushort)(startLinear + len - 1), buf) << "> ";
                s << '<' << QPdf::toHex((ushort)reverseMap[startLinear], buf) << ">\n";
                checkRanges(ts, ranges, nranges);
                startLinear += len;
            }
        }
    }
    if (nranges) {
        ts << nranges << "beginbfrange\n"
           << ranges << "endbfrange\n";
    }
    ts << "endcmap\n"
        "CMapName currentdict /CMap defineresource pop\n"
        "end\n"
        "end\n";

    return touc;
}

int QFontSubset::addGlyph(int index)
{
    int idx = glyph_indices.indexOf(index);
    if (idx < 0) {
        idx = glyph_indices.size();
        glyph_indices.append(index);
    }
    return idx;
}

// ------------------ Type 1 generation ---------------------------

// needs at least 6 bytes of space in tmp
static const char *encodeNumber(int num, char *tmp)
{
    const char *ret = tmp;
    if(num >= -107 && num <= 107) {
        QPdf::toHex((uchar)(num + 139), tmp);
        tmp += 2;
    } else if (num > 107 && num <= 1131) {
        num -= 108;
        QPdf::toHex((uchar)((num >> 8) + 247), tmp);
        tmp += 2;
        QPdf::toHex((uchar)(num & 0xff), tmp);
        tmp += 2;
    } else if(num < - 107 && num >= -1131) {
        num += 108;
        num = -num;
        QPdf::toHex((uchar)((num >> 8) + 251), tmp);
        tmp += 2;
        QPdf::toHex((uchar)(num & 0xff), tmp);
        tmp += 2;
    } else {
        *tmp++ = 'f';
        *tmp++ = 'f';
        QPdf::toHex((uchar)(num >> 24), tmp);
        tmp += 2;
        QPdf::toHex((uchar)(num >> 16), tmp);
        tmp += 2;
        QPdf::toHex((uchar)(num >> 8), tmp);
        tmp += 2;
        QPdf::toHex((uchar)(num >> 0), tmp);
        tmp += 2;
    }
    *tmp = 0;
//     qDebug("encodeNumber: %d -> '%s'", num, ret);
    return ret;
}

static QByteArray charString(const QPainterPath &path, qreal advance, qreal lsb, qreal ppem)
{
    // the charstring commands we need
    static const char *hsbw = "0D";
    static const char *closepath = "09";
    static const char *moveto[3] = { "16", "04", "15" };
    static const char *lineto[3] = { "06", "07", "05" };
    static const char *rcurveto = "08";
    static const char *endchar = "0E";

    enum { horizontal = 1,  vertical = 2 };

    char tmp[16];

    qreal factor = 1000./ppem;

    int lsb_i = qRound(lsb*factor);
    int advance_i = qRound(advance*factor);
//     qDebug("--- charstring");

    // first of all add lsb and width to the charstring using the hsbw command
    QByteArray charstring;
    charstring += encodeNumber(lsb_i, tmp);
    charstring += encodeNumber(advance_i, tmp);
    charstring += hsbw;

    // add the path
    int xl = lsb_i;
    int yl = 0;
    bool openpath = false;
    for (int i = 0; i < path.elementCount(); ++i) {
        const QPainterPath::Element &elm = path.elementAt(i);
        int x = qRound(elm.x*factor);
        int y = -qRound(elm.y*factor);
        int dx = x - xl;
        int dy = y - yl;
        if (elm.type == QPainterPath::MoveToElement && openpath) {
//             qDebug("closepath %s", closepath);
            charstring += closepath;
        }
        if (elm.type == QPainterPath::MoveToElement ||
            elm.type == QPainterPath::LineToElement) {
            int type = -1;
            if (dx || !dy) {
                charstring += encodeNumber(dx, tmp);
                type += horizontal;
//                 qDebug("horizontal");
            }
            if (dy) {
                charstring += encodeNumber(dy, tmp);
                type += vertical;
//                 qDebug("vertical");
            }
//             qDebug("moveto/lineto %s", (elm.type == QPainterPath::MoveToElement ? moveto[type] : lineto[type]));
            charstring += (elm.type == QPainterPath::MoveToElement ? moveto[type] : lineto[type]);
            openpath = true;
            xl = x;
            yl = y;
        } else {
            Q_ASSERT(elm.type == QPainterPath::CurveToElement);
            const QPainterPath::Element &elm2 = path.elementAt(++i);
            const QPainterPath::Element &elm3 = path.elementAt(++i);
            int x2 = qRound(elm2.x*factor);
            int y2 = -qRound(elm2.y*factor);
            int x3 = qRound(elm3.x*factor);
            int y3 = -qRound(elm3.y*factor);
            charstring += encodeNumber(dx, tmp);
            charstring += encodeNumber(dy, tmp);
            charstring += encodeNumber(x2 - x, tmp);
            charstring += encodeNumber(y2 - y, tmp);
            charstring += encodeNumber(x3 - x2, tmp);
            charstring += encodeNumber(y3 - y2, tmp);
            charstring += rcurveto;
            openpath = true;
            xl = x3;
            yl = y3;
//             qDebug("rcurveto");
        }
    }
    if (openpath)
        charstring += closepath;
    charstring += endchar;
    if (charstring.length() > 240) {
        int pos = 240;
        while (pos < charstring.length()) {
            charstring.insert(pos, '\n');
            pos += 241;
        }
    }
    return charstring;
}

// for reference:
// https://www.adobe.com/content/dam/acom/en/devnet/actionscript/articles/PLRM.pdf
static const char *freesans_styles[4] = {
    "FreeSans",
    "FreeSans-Bold",
    "FreeSans-Oblique",
    "FreeSans-BoldOblique"
};

static const char *freeserif_styles[4] = {
    "FreeSerif",
    "FreeSerif-Bold",
    "FreeSerif-Oblique",
    "FreeSerif-BoldOblique"
};

static const char *freemono_styles[4] = {
    "FreeMono",
    "FreeMono-Bold",
    "FreeMono-Oblique",
    "FreeMono-BoldOblique"
};

QByteArray QFontSubset::toType1() const
{
    QFontEngine::Properties properties = fontEngine->properties();

    QByteArray font;
    QPdf::ByteStream s(&font);

    QByteArray id = QByteArray::number(object_id);
    QByteArray psname = properties.postscriptName;
    psname.replace(' ', "");

    standard_font = false;

    FT_Face face = ft_face(fontEngine);
    if (face && !FT_IS_SCALABLE(face)) {
        int style = 0;
        if (fontEngine->fontDef.style)
            style += 2;
        if (fontEngine->fontDef.weight >= QFont::Bold)
            style++;
        if (fontEngine->fontDef.family.contains(QLatin1String("FreeSans"), Qt::CaseInsensitive)) {
            psname = freesans_styles[style];
            standard_font = true;
        } else if (fontEngine->fontDef.family.contains(QLatin1String("FreeSerif"), Qt::CaseInsensitive)) {
            psname = freeserif_styles[style];
            standard_font = true;
        } else if (fontEngine->fontDef.family.contains(QLatin1String("FreeMono"), Qt::CaseInsensitive)) {
            psname = freemono_styles[style];
            standard_font = true;
        }
    }
    s << "/F" << id << "-Base\n";
    if (standard_font) {
            s << '/' << psname << " findfont\n"
                "0 dict copy dup /NumGlyphs 0 put dup /CMap 256 array put def\n";
    } else {
        s << "<<\n";
        if(!psname.isEmpty())
            s << "/FontName /" << psname << '\n';
        s << "/FontInfo <</FsType " << fontEngine->fsType << ">>\n"
            "/FontType 1\n"
            "/PaintType 0\n"
            "/FontMatrix [.001 0 0 .001 0 0]\n"
            "/FontBBox { 0 0 0 0 }\n"
            "/Private <<\n"
            "/password 5839\n"
            "/MinFeature {16 16}\n"
            "/BlueValues []\n"
            "/lenIV -1\n"
            ">>\n"
            "/CharStrings << >>\n"
            "/NumGlyphs 0\n"
            "/CMap 256 array\n"
            ">> def\n";
    }
    s << type1AddedGlyphs();
    downloaded_glyphs = glyph_indices.size();

    return font;
}

QByteArray QFontSubset::type1AddedGlyphs() const
{
    if (downloaded_glyphs == glyph_indices.size())
        return QByteArray();

    QFontEngine::Properties properties = fontEngine->properties();
    QVector<int> reverseMap = getReverseMap();
    QByteArray glyphs;
    QPdf::ByteStream s(&glyphs);

    int nGlyphs = glyph_indices.size();
    QByteArray id = QByteArray::number(object_id);

    s << 'F' << id << "-Base [\n";
    for (int i = downloaded_glyphs; i < nGlyphs; ++i) {
        glyph_t g = glyph_indices.at(i);
        QPainterPath path;
        glyph_metrics_t metric;
        fontEngine->getUnscaledGlyph(g, &path, &metric);
        QByteArray charstring = charString(path, metric.xoff.toReal(), metric.x.toReal(),
                                             properties.emSquare.toReal());
        s << glyphName(i, reverseMap);
        if (!standard_font)
          s << "\n<" << charstring << ">\n";
    }
    s << (standard_font ? "] T1AddMapping\n" : "] T1AddGlyphs\n");
    return glyphs;
}

QT_END_NAMESPACE

#endif // QT_NO_PRINTER




