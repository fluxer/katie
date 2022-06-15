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

#ifndef QCOLOR_P_H
#define QCOLOR_P_H

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

#include "QtGui/qrgb.h"

QT_BEGIN_NAMESPACE

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

static const qint16 rgbTblSize = sizeof(rgbTbl) / sizeof(RGBData);
#endif // QT_NO_COLORNAMES

bool qt_get_hex_rgb(const char *, const int, QRgb *);

QT_END_NAMESPACE

#endif // QCOLOR_P_H
