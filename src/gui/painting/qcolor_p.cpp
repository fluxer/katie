/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
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

#include "qrgb.h"
#include "qstringlist.h"

QT_BEGIN_NAMESPACE

static inline int h2i(char hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    return -1;
}

static inline int hex2int(const char *s)
{
    return (h2i(s[0]) << 4) | h2i(s[1]);
}

static inline int hex2int(char s)
{
    int h = h2i(s);
    return (h << 4) | h;
}

bool qt_get_hex_rgb(const char *name, QRgb *rgb)
{
    if(name[0] != '#')
        return false;
    name++;
    int len = qstrlen(name);
    int r, g, b;
    if (len == 12) {
        r = hex2int(name);
        g = hex2int(name + 4);
        b = hex2int(name + 8);
    } else if (len == 9) {
        r = hex2int(name);
        g = hex2int(name + 3);
        b = hex2int(name + 6);
    } else if (len == 6) {
        r = hex2int(name);
        g = hex2int(name + 2);
        b = hex2int(name + 4);
    } else if (len == 3) {
        r = hex2int(name[0]);
        g = hex2int(name[1]);
        b = hex2int(name[2]);
    } else {
        r = g = b = -1;
    }
    if ((uint)r > 255 || (uint)g > 255 || (uint)b > 255) {
        *rgb = 0;
        return false;
    }
    *rgb = qRgb(r, g ,b);
    return true;
}

#ifndef QT_NO_COLORNAMES

/*
  CSS color names = SVG 1.0 color names + transparent (rgba(0,0,0,0))
*/
#define RGBCOLOR(r,g,b) (0xff000000 | (r << 16) |  (g << 8) | b)
static const struct RGBData {
    const char *name;
    const uint value;
} rgbTbl[] = {
    { "aliceblue", RGBCOLOR(240, 248, 255) },
    { "antiquewhite", RGBCOLOR(250, 235, 215) },
    { "aqua", RGBCOLOR( 0, 255, 255) },
    { "aquamarine", RGBCOLOR(127, 255, 212) },
    { "azure", RGBCOLOR(240, 255, 255) },
    { "beige", RGBCOLOR(245, 245, 220) },
    { "bisque", RGBCOLOR(255, 228, 196) },
    { "black", RGBCOLOR(0, 0, 0) },
    { "blanchedalmond", RGBCOLOR(255, 235, 205) },
    { "blue", RGBCOLOR(0, 0, 255) },
    { "blueviolet", RGBCOLOR(138, 43, 226) },
    { "brown", RGBCOLOR(165, 42, 42) },
    { "burlywood", RGBCOLOR(222, 184, 135) },
    { "cadetblue", RGBCOLOR( 95, 158, 160) },
    { "chartreuse", RGBCOLOR(127, 255, 0) },
    { "chocolate", RGBCOLOR(210, 105, 30) },
    { "coral", RGBCOLOR(255, 127, 80) },
    { "cornflowerblue", RGBCOLOR(100, 149, 237) },
    { "cornsilk", RGBCOLOR(255, 248, 220) },
    { "crimson", RGBCOLOR(220, 20, 60) },
    { "cyan", RGBCOLOR(0, 255, 255) },
    { "darkblue", RGBCOLOR(0, 0, 139) },
    { "darkcyan", RGBCOLOR(0, 139, 139) },
    { "darkgoldenrod", RGBCOLOR(184, 134, 11) },
    { "darkgray", RGBCOLOR(169, 169, 169) },
    { "darkgreen", RGBCOLOR(0, 100, 0) },
    { "darkgrey", RGBCOLOR(169, 169, 169) },
    { "darkkhaki", RGBCOLOR(189, 183, 107) },
    { "darkmagenta", RGBCOLOR(139, 0, 139) },
    { "darkolivegreen", RGBCOLOR( 85, 107, 47) },
    { "darkorange", RGBCOLOR(255, 140, 0) },
    { "darkorchid", RGBCOLOR(153, 50, 204) },
    { "darkred", RGBCOLOR(139, 0, 0) },
    { "darksalmon", RGBCOLOR(233, 150, 122) },
    { "darkseagreen", RGBCOLOR(143, 188, 143) },
    { "darkslateblue", RGBCOLOR(72, 61, 139) },
    { "darkslategray", RGBCOLOR(47, 79, 79) },
    { "darkslategrey", RGBCOLOR(47, 79, 79) },
    { "darkturquoise", RGBCOLOR(0, 206, 209) },
    { "darkviolet", RGBCOLOR(148, 0, 211) },
    { "deeppink", RGBCOLOR(255, 20, 147) },
    { "deepskyblue", RGBCOLOR( 0, 191, 255) },
    { "dimgray", RGBCOLOR(105, 105, 105) },
    { "dimgrey", RGBCOLOR(105, 105, 105) },
    { "dodgerblue", RGBCOLOR( 30, 144, 255) },
    { "firebrick", RGBCOLOR(178, 34, 34) },
    { "floralwhite", RGBCOLOR(255, 250, 240) },
    { "forestgreen", RGBCOLOR(34, 139, 34) },
    { "fuchsia", RGBCOLOR(255, 0, 255) },
    { "gainsboro", RGBCOLOR(220, 220, 220) },
    { "ghostwhite", RGBCOLOR(248, 248, 255) },
    { "gold", RGBCOLOR(255, 215, 0) },
    { "goldenrod", RGBCOLOR(218, 165, 32) },
    { "gray", RGBCOLOR(128, 128, 128) },
    { "green", RGBCOLOR(0, 128, 0) },
    { "greenyellow", RGBCOLOR(173, 255, 47) },
    { "grey", RGBCOLOR(128, 128, 128) },
    { "honeydew", RGBCOLOR(240, 255, 240) },
    { "hotpink", RGBCOLOR(255, 105, 180) },
    { "indianred", RGBCOLOR(205, 92, 92) },
    { "indigo", RGBCOLOR(75, 0, 130) },
    { "ivory", RGBCOLOR(255, 255, 240) },
    { "khaki", RGBCOLOR(240, 230, 140) },
    { "lavender", RGBCOLOR(230, 230, 250) },
    { "lavenderblush", RGBCOLOR(255, 240, 245) },
    { "lawngreen", RGBCOLOR(124, 252, 0) },
    { "lemonchiffon", RGBCOLOR(255, 250, 205) },
    { "lightblue", RGBCOLOR(173, 216, 230) },
    { "lightcoral", RGBCOLOR(240, 128, 128) },
    { "lightcyan", RGBCOLOR(224, 255, 255) },
    { "lightgoldenrodyellow", RGBCOLOR(250, 250, 210) },
    { "lightgray", RGBCOLOR(211, 211, 211) },
    { "lightgreen", RGBCOLOR(144, 238, 144) },
    { "lightgrey", RGBCOLOR(211, 211, 211) },
    { "lightpink", RGBCOLOR(255, 182, 193) },
    { "lightsalmon", RGBCOLOR(255, 160, 122) },
    { "lightseagreen", RGBCOLOR( 32, 178, 170) },
    { "lightskyblue", RGBCOLOR(135, 206, 250) },
    { "lightslategray", RGBCOLOR(119, 136, 153) },
    { "lightslategrey", RGBCOLOR(119, 136, 153) },
    { "lightsteelblue", RGBCOLOR(176, 196, 222) },
    { "lightyellow", RGBCOLOR(255, 255, 224) },
    { "lime", RGBCOLOR(0, 255, 0) },
    { "limegreen", RGBCOLOR(50, 205, 50) },
    { "linen", RGBCOLOR(250, 240, 230) },
    { "magenta", RGBCOLOR(255, 0, 255) },
    { "maroon", RGBCOLOR(128, 0, 0) },
    { "mediumaquamarine", RGBCOLOR(102, 205, 170) },
    { "mediumblue", RGBCOLOR(0, 0, 205) },
    { "mediumorchid", RGBCOLOR(186, 85, 211) },
    { "mediumpurple", RGBCOLOR(147, 112, 219) },
    { "mediumseagreen", RGBCOLOR( 60, 179, 113) },
    { "mediumslateblue", RGBCOLOR(123, 104, 238) },
    { "mediumspringgreen", RGBCOLOR(0, 250, 154) },
    { "mediumturquoise", RGBCOLOR(72, 209, 204) },
    { "mediumvioletred", RGBCOLOR(199, 21, 133) },
    { "midnightblue", RGBCOLOR(25, 25, 112) },
    { "mintcream", RGBCOLOR(245, 255, 250) },
    { "mistyrose", RGBCOLOR(255, 228, 225) },
    { "moccasin", RGBCOLOR(255, 228, 181) },
    { "navajowhite", RGBCOLOR(255, 222, 173) },
    { "navy", RGBCOLOR(0, 0, 128) },
    { "oldlace", RGBCOLOR(253, 245, 230) },
    { "olive", RGBCOLOR(128, 128, 0) },
    { "olivedrab", RGBCOLOR(107, 142, 35) },
    { "orange", RGBCOLOR(255, 165, 0) },
    { "orangered", RGBCOLOR(255, 69, 0) },
    { "orchid", RGBCOLOR(218, 112, 214) },
    { "palegoldenrod", RGBCOLOR(238, 232, 170) },
    { "palegreen", RGBCOLOR(152, 251, 152) },
    { "paleturquoise", RGBCOLOR(175, 238, 238) },
    { "palevioletred", RGBCOLOR(219, 112, 147) },
    { "papayawhip", RGBCOLOR(255, 239, 213) },
    { "peachpuff", RGBCOLOR(255, 218, 185) },
    { "peru", RGBCOLOR(205, 133, 63) },
    { "pink", RGBCOLOR(255, 192, 203) },
    { "plum", RGBCOLOR(221, 160, 221) },
    { "powderblue", RGBCOLOR(176, 224, 230) },
    { "purple", RGBCOLOR(128, 0, 128) },
    { "red", RGBCOLOR(255, 0, 0) },
    { "rosybrown", RGBCOLOR(188, 143, 143) },
    { "royalblue", RGBCOLOR(65, 105, 225) },
    { "saddlebrown", RGBCOLOR(139, 69, 19) },
    { "salmon", RGBCOLOR(250, 128, 114) },
    { "sandybrown", RGBCOLOR(244, 164, 96) },
    { "seagreen", RGBCOLOR(46, 139, 87) },
    { "seashell", RGBCOLOR(255, 245, 238) },
    { "sienna", RGBCOLOR(160, 82, 45) },
    { "silver", RGBCOLOR(192, 192, 192) },
    { "skyblue", RGBCOLOR(135, 206, 235) },
    { "slateblue", RGBCOLOR(106, 90, 205) },
    { "slategray", RGBCOLOR(112, 128, 144) },
    { "slategrey", RGBCOLOR(112, 128, 144) },
    { "snow", RGBCOLOR(255, 250, 250) },
    { "springgreen", RGBCOLOR( 0, 255, 127) },
    { "steelblue", RGBCOLOR(70, 130, 180) },
    { "tan", RGBCOLOR(210, 180, 140) },
    { "teal", RGBCOLOR(0, 128, 128) },
    { "thistle", RGBCOLOR(216, 191, 216) },
    { "tomato", RGBCOLOR(255, 99, 71) },
    { "transparent", 0 },
    { "turquoise", RGBCOLOR(64, 224, 208) },
    { "violet", RGBCOLOR(238, 130, 238) },
    { "wheat", RGBCOLOR(245, 222, 179) },
    { "white", RGBCOLOR(255, 255, 255) },
    { "whitesmoke", RGBCOLOR(245, 245, 245) },
    { "yellow", RGBCOLOR(255, 255, 0) },
    { "yellowgreen", RGBCOLOR(154, 205, 50) }
};
#undef RGBCOLOR

static const int rgbTblSize = sizeof(rgbTbl) / sizeof(RGBData);

inline bool operator<(const char *name, const RGBData &data)
{ return qstrcmp(name, data.name) < 0; }
inline bool operator<(const RGBData &data, const char *name)
{ return qstrcmp(data.name, name) < 0; }

bool qt_get_named_rgb(const char *name, QRgb* rgb)
{
    const RGBData *r = qBinaryFind(rgbTbl, rgbTbl + rgbTblSize, name);
    if (r != rgbTbl + rgbTblSize) {
        *rgb = r->value;
        return true;
    }
    return false;
}

QStringList qt_get_colornames()
{
    static QStringList lst;
    if (lst.isEmpty()) {
        for (int i = 0; i < rgbTblSize; i++)
            lst << QString::fromLatin1(rgbTbl[i].name);
    }
    return lst;
}

#else

bool qt_get_named_rgb(const char *, QRgb*)
{
    return false;
}

QStringList qt_get_colornames()
{
    return QStringList();
}
#endif // QT_NO_COLORNAMES

QT_END_NAMESPACE


