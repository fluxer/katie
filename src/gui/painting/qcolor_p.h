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
static const struct RGBData {
    const char *name;
    const QRgb value;
} RGBTbl[] = {
    { "aliceblue", qRgb(240, 248, 255) },
    { "antiquewhite", qRgb(250, 235, 215) },
    { "aqua", qRgb( 0, 255, 255) },
    { "aquamarine", qRgb(127, 255, 212) },
    { "azure", qRgb(240, 255, 255) },
    { "beige", qRgb(245, 245, 220) },
    { "bisque", qRgb(255, 228, 196) },
    { "black", qRgb(0, 0, 0) },
    { "blanchedalmond", qRgb(255, 235, 205) },
    { "blue", qRgb(0, 0, 255) },
    { "blueviolet", qRgb(138, 43, 226) },
    { "brown", qRgb(165, 42, 42) },
    { "burlywood", qRgb(222, 184, 135) },
    { "cadetblue", qRgb( 95, 158, 160) },
    { "chartreuse", qRgb(127, 255, 0) },
    { "chocolate", qRgb(210, 105, 30) },
    { "coral", qRgb(255, 127, 80) },
    { "cornflowerblue", qRgb(100, 149, 237) },
    { "cornsilk", qRgb(255, 248, 220) },
    { "crimson", qRgb(220, 20, 60) },
    { "cyan", qRgb(0, 255, 255) },
    { "darkblue", qRgb(0, 0, 139) },
    { "darkcyan", qRgb(0, 139, 139) },
    { "darkgoldenrod", qRgb(184, 134, 11) },
    { "darkgray", qRgb(169, 169, 169) },
    { "darkgreen", qRgb(0, 100, 0) },
    { "darkgrey", qRgb(169, 169, 169) },
    { "darkkhaki", qRgb(189, 183, 107) },
    { "darkmagenta", qRgb(139, 0, 139) },
    { "darkolivegreen", qRgb( 85, 107, 47) },
    { "darkorange", qRgb(255, 140, 0) },
    { "darkorchid", qRgb(153, 50, 204) },
    { "darkred", qRgb(139, 0, 0) },
    { "darksalmon", qRgb(233, 150, 122) },
    { "darkseagreen", qRgb(143, 188, 143) },
    { "darkslateblue", qRgb(72, 61, 139) },
    { "darkslategray", qRgb(47, 79, 79) },
    { "darkslategrey", qRgb(47, 79, 79) },
    { "darkturquoise", qRgb(0, 206, 209) },
    { "darkviolet", qRgb(148, 0, 211) },
    { "deeppink", qRgb(255, 20, 147) },
    { "deepskyblue", qRgb( 0, 191, 255) },
    { "dimgray", qRgb(105, 105, 105) },
    { "dimgrey", qRgb(105, 105, 105) },
    { "dodgerblue", qRgb( 30, 144, 255) },
    { "firebrick", qRgb(178, 34, 34) },
    { "floralwhite", qRgb(255, 250, 240) },
    { "forestgreen", qRgb(34, 139, 34) },
    { "fuchsia", qRgb(255, 0, 255) },
    { "gainsboro", qRgb(220, 220, 220) },
    { "ghostwhite", qRgb(248, 248, 255) },
    { "gold", qRgb(255, 215, 0) },
    { "goldenrod", qRgb(218, 165, 32) },
    { "gray", qRgb(128, 128, 128) },
    { "green", qRgb(0, 128, 0) },
    { "greenyellow", qRgb(173, 255, 47) },
    { "grey", qRgb(128, 128, 128) },
    { "honeydew", qRgb(240, 255, 240) },
    { "hotpink", qRgb(255, 105, 180) },
    { "indianred", qRgb(205, 92, 92) },
    { "indigo", qRgb(75, 0, 130) },
    { "ivory", qRgb(255, 255, 240) },
    { "khaki", qRgb(240, 230, 140) },
    { "lavender", qRgb(230, 230, 250) },
    { "lavenderblush", qRgb(255, 240, 245) },
    { "lawngreen", qRgb(124, 252, 0) },
    { "lemonchiffon", qRgb(255, 250, 205) },
    { "lightblue", qRgb(173, 216, 230) },
    { "lightcoral", qRgb(240, 128, 128) },
    { "lightcyan", qRgb(224, 255, 255) },
    { "lightgoldenrodyellow", qRgb(250, 250, 210) },
    { "lightgray", qRgb(211, 211, 211) },
    { "lightgreen", qRgb(144, 238, 144) },
    { "lightgrey", qRgb(211, 211, 211) },
    { "lightpink", qRgb(255, 182, 193) },
    { "lightsalmon", qRgb(255, 160, 122) },
    { "lightseagreen", qRgb( 32, 178, 170) },
    { "lightskyblue", qRgb(135, 206, 250) },
    { "lightslategray", qRgb(119, 136, 153) },
    { "lightslategrey", qRgb(119, 136, 153) },
    { "lightsteelblue", qRgb(176, 196, 222) },
    { "lightyellow", qRgb(255, 255, 224) },
    { "lime", qRgb(0, 255, 0) },
    { "limegreen", qRgb(50, 205, 50) },
    { "linen", qRgb(250, 240, 230) },
    { "magenta", qRgb(255, 0, 255) },
    { "maroon", qRgb(128, 0, 0) },
    { "mediumaquamarine", qRgb(102, 205, 170) },
    { "mediumblue", qRgb(0, 0, 205) },
    { "mediumorchid", qRgb(186, 85, 211) },
    { "mediumpurple", qRgb(147, 112, 219) },
    { "mediumseagreen", qRgb( 60, 179, 113) },
    { "mediumslateblue", qRgb(123, 104, 238) },
    { "mediumspringgreen", qRgb(0, 250, 154) },
    { "mediumturquoise", qRgb(72, 209, 204) },
    { "mediumvioletred", qRgb(199, 21, 133) },
    { "midnightblue", qRgb(25, 25, 112) },
    { "mintcream", qRgb(245, 255, 250) },
    { "mistyrose", qRgb(255, 228, 225) },
    { "moccasin", qRgb(255, 228, 181) },
    { "navajowhite", qRgb(255, 222, 173) },
    { "navy", qRgb(0, 0, 128) },
    { "oldlace", qRgb(253, 245, 230) },
    { "olive", qRgb(128, 128, 0) },
    { "olivedrab", qRgb(107, 142, 35) },
    { "orange", qRgb(255, 165, 0) },
    { "orangered", qRgb(255, 69, 0) },
    { "orchid", qRgb(218, 112, 214) },
    { "palegoldenrod", qRgb(238, 232, 170) },
    { "palegreen", qRgb(152, 251, 152) },
    { "paleturquoise", qRgb(175, 238, 238) },
    { "palevioletred", qRgb(219, 112, 147) },
    { "papayawhip", qRgb(255, 239, 213) },
    { "peachpuff", qRgb(255, 218, 185) },
    { "peru", qRgb(205, 133, 63) },
    { "pink", qRgb(255, 192, 203) },
    { "plum", qRgb(221, 160, 221) },
    { "powderblue", qRgb(176, 224, 230) },
    { "purple", qRgb(128, 0, 128) },
    { "red", qRgb(255, 0, 0) },
    { "rosybrown", qRgb(188, 143, 143) },
    { "royalblue", qRgb(65, 105, 225) },
    { "saddlebrown", qRgb(139, 69, 19) },
    { "salmon", qRgb(250, 128, 114) },
    { "sandybrown", qRgb(244, 164, 96) },
    { "seagreen", qRgb(46, 139, 87) },
    { "seashell", qRgb(255, 245, 238) },
    { "sienna", qRgb(160, 82, 45) },
    { "silver", qRgb(192, 192, 192) },
    { "skyblue", qRgb(135, 206, 235) },
    { "slateblue", qRgb(106, 90, 205) },
    { "slategray", qRgb(112, 128, 144) },
    { "slategrey", qRgb(112, 128, 144) },
    { "snow", qRgb(255, 250, 250) },
    { "springgreen", qRgb( 0, 255, 127) },
    { "steelblue", qRgb(70, 130, 180) },
    { "tan", qRgb(210, 180, 140) },
    { "teal", qRgb(0, 128, 128) },
    { "thistle", qRgb(216, 191, 216) },
    { "tomato", qRgb(255, 99, 71) },
    { "transparent", 0 },
    { "turquoise", qRgb(64, 224, 208) },
    { "violet", qRgb(238, 130, 238) },
    { "wheat", qRgb(245, 222, 179) },
    { "white", qRgb(255, 255, 255) },
    { "whitesmoke", qRgb(245, 245, 245) },
    { "yellow", qRgb(255, 255, 0) },
    { "yellowgreen", qRgb(154, 205, 50) }
};
static const qint16 RGBTblSize = sizeof(RGBTbl) / sizeof(RGBData);

// generated via genrgb.py
static const struct X11RGBTblData {
    const char *name;
    const QRgb value;
} X11RGBTbl[] = {
    { "aliceblue", qRgb(240, 248, 255) },
    { "antiquewhite", qRgb(250, 235, 215) },
    { "antiquewhite1", qRgb(255, 239, 219) },
    { "antiquewhite2", qRgb(238, 223, 204) },
    { "antiquewhite3", qRgb(205, 192, 176) },
    { "antiquewhite4", qRgb(139, 131, 120) },
    { "aquamarine", qRgb(127, 255, 212) },
    { "aquamarine1", qRgb(127, 255, 212) },
    { "aquamarine2", qRgb(118, 238, 198) },
    { "aquamarine3", qRgb(102, 205, 170) },
    { "aquamarine4", qRgb(69, 139, 116) },
    { "azure", qRgb(240, 255, 255) },
    { "azure1", qRgb(240, 255, 255) },
    { "azure2", qRgb(224, 238, 238) },
    { "azure3", qRgb(193, 205, 205) },
    { "azure4", qRgb(131, 139, 139) },
    { "beige", qRgb(245, 245, 220) },
    { "bisque", qRgb(255, 228, 196) },
    { "bisque1", qRgb(255, 228, 196) },
    { "bisque2", qRgb(238, 213, 183) },
    { "bisque3", qRgb(205, 183, 158) },
    { "bisque4", qRgb(139, 125, 107) },
    { "black", qRgb(0, 0, 0) },
    { "blanchedalmond", qRgb(255, 235, 205) },
    { "blue", qRgb(0, 0, 255) },
    { "blue1", qRgb(0, 0, 255) },
    { "blue2", qRgb(0, 0, 238) },
    { "blue3", qRgb(0, 0, 205) },
    { "blue4", qRgb(0, 0, 139) },
    { "blueviolet", qRgb(138, 43, 226) },
    { "brown", qRgb(165, 42, 42) },
    { "brown1", qRgb(255, 64, 64) },
    { "brown2", qRgb(238, 59, 59) },
    { "brown3", qRgb(205, 51, 51) },
    { "brown4", qRgb(139, 35, 35) },
    { "burlywood", qRgb(222, 184, 135) },
    { "burlywood1", qRgb(255, 211, 155) },
    { "burlywood2", qRgb(238, 197, 145) },
    { "burlywood3", qRgb(205, 170, 125) },
    { "burlywood4", qRgb(139, 115, 85) },
    { "cadetblue", qRgb(95, 158, 160) },
    { "cadetblue1", qRgb(152, 245, 255) },
    { "cadetblue2", qRgb(142, 229, 238) },
    { "cadetblue3", qRgb(122, 197, 205) },
    { "cadetblue4", qRgb(83, 134, 139) },
    { "chartreuse", qRgb(127, 255, 0) },
    { "chartreuse1", qRgb(127, 255, 0) },
    { "chartreuse2", qRgb(118, 238, 0) },
    { "chartreuse3", qRgb(102, 205, 0) },
    { "chartreuse4", qRgb(69, 139, 0) },
    { "chocolate", qRgb(210, 105, 30) },
    { "chocolate1", qRgb(255, 127, 36) },
    { "chocolate2", qRgb(238, 118, 33) },
    { "chocolate3", qRgb(205, 102, 29) },
    { "chocolate4", qRgb(139, 69, 19) },
    { "coral", qRgb(255, 127, 80) },
    { "coral1", qRgb(255, 114, 86) },
    { "coral2", qRgb(238, 106, 80) },
    { "coral3", qRgb(205, 91, 69) },
    { "coral4", qRgb(139, 62, 47) },
    { "cornflowerblue", qRgb(100, 149, 237) },
    { "cornsilk", qRgb(255, 248, 220) },
    { "cornsilk1", qRgb(255, 248, 220) },
    { "cornsilk2", qRgb(238, 232, 205) },
    { "cornsilk3", qRgb(205, 200, 177) },
    { "cornsilk4", qRgb(139, 136, 120) },
    { "cyan", qRgb(0, 255, 255) },
    { "cyan1", qRgb(0, 255, 255) },
    { "cyan2", qRgb(0, 238, 238) },
    { "cyan3", qRgb(0, 205, 205) },
    { "cyan4", qRgb(0, 139, 139) },
    { "darkblue", qRgb(0, 0, 139) },
    { "darkcyan", qRgb(0, 139, 139) },
    { "darkgoldenrod", qRgb(184, 134, 11) },
    { "darkgoldenrod1", qRgb(255, 185, 15) },
    { "darkgoldenrod2", qRgb(238, 173, 14) },
    { "darkgoldenrod3", qRgb(205, 149, 12) },
    { "darkgoldenrod4", qRgb(139, 101, 8) },
    { "darkgray", qRgb(169, 169, 169) },
    { "darkgreen", qRgb(0, 100, 0) },
    { "darkgrey", qRgb(169, 169, 169) },
    { "darkkhaki", qRgb(189, 183, 107) },
    { "darkmagenta", qRgb(139, 0, 139) },
    { "darkolivegreen", qRgb(85, 107, 47) },
    { "darkolivegreen1", qRgb(202, 255, 112) },
    { "darkolivegreen2", qRgb(188, 238, 104) },
    { "darkolivegreen3", qRgb(162, 205, 90) },
    { "darkolivegreen4", qRgb(110, 139, 61) },
    { "darkorange", qRgb(255, 140, 0) },
    { "darkorange1", qRgb(255, 127, 0) },
    { "darkorange2", qRgb(238, 118, 0) },
    { "darkorange3", qRgb(205, 102, 0) },
    { "darkorange4", qRgb(139, 69, 0) },
    { "darkorchid", qRgb(153, 50, 204) },
    { "darkorchid1", qRgb(191, 62, 255) },
    { "darkorchid2", qRgb(178, 58, 238) },
    { "darkorchid3", qRgb(154, 50, 205) },
    { "darkorchid4", qRgb(104, 34, 139) },
    { "darkred", qRgb(139, 0, 0) },
    { "darksalmon", qRgb(233, 150, 122) },
    { "darkseagreen", qRgb(143, 188, 143) },
    { "darkseagreen1", qRgb(193, 255, 193) },
    { "darkseagreen2", qRgb(180, 238, 180) },
    { "darkseagreen3", qRgb(155, 205, 155) },
    { "darkseagreen4", qRgb(105, 139, 105) },
    { "darkslateblue", qRgb(72, 61, 139) },
    { "darkslategray", qRgb(47, 79, 79) },
    { "darkslategray1", qRgb(151, 255, 255) },
    { "darkslategray2", qRgb(141, 238, 238) },
    { "darkslategray3", qRgb(121, 205, 205) },
    { "darkslategray4", qRgb(82, 139, 139) },
    { "darkslategrey", qRgb(47, 79, 79) },
    { "darkturquoise", qRgb(0, 206, 209) },
    { "darkviolet", qRgb(148, 0, 211) },
    { "debianred", qRgb(215, 7, 81) },
    { "deeppink", qRgb(255, 20, 147) },
    { "deeppink1", qRgb(255, 20, 147) },
    { "deeppink2", qRgb(238, 18, 137) },
    { "deeppink3", qRgb(205, 16, 118) },
    { "deeppink4", qRgb(139, 10, 80) },
    { "deepskyblue", qRgb(0, 191, 255) },
    { "deepskyblue1", qRgb(0, 191, 255) },
    { "deepskyblue2", qRgb(0, 178, 238) },
    { "deepskyblue3", qRgb(0, 154, 205) },
    { "deepskyblue4", qRgb(0, 104, 139) },
    { "dimgray", qRgb(105, 105, 105) },
    { "dimgrey", qRgb(105, 105, 105) },
    { "dodgerblue", qRgb(30, 144, 255) },
    { "dodgerblue1", qRgb(30, 144, 255) },
    { "dodgerblue2", qRgb(28, 134, 238) },
    { "dodgerblue3", qRgb(24, 116, 205) },
    { "dodgerblue4", qRgb(16, 78, 139) },
    { "firebrick", qRgb(178, 34, 34) },
    { "firebrick1", qRgb(255, 48, 48) },
    { "firebrick2", qRgb(238, 44, 44) },
    { "firebrick3", qRgb(205, 38, 38) },
    { "firebrick4", qRgb(139, 26, 26) },
    { "floralwhite", qRgb(255, 250, 240) },
    { "forestgreen", qRgb(34, 139, 34) },
    { "gainsboro", qRgb(220, 220, 220) },
    { "ghostwhite", qRgb(248, 248, 255) },
    { "gold", qRgb(255, 215, 0) },
    { "gold1", qRgb(255, 215, 0) },
    { "gold2", qRgb(238, 201, 0) },
    { "gold3", qRgb(205, 173, 0) },
    { "gold4", qRgb(139, 117, 0) },
    { "goldenrod", qRgb(218, 165, 32) },
    { "goldenrod1", qRgb(255, 193, 37) },
    { "goldenrod2", qRgb(238, 180, 34) },
    { "goldenrod3", qRgb(205, 155, 29) },
    { "goldenrod4", qRgb(139, 105, 20) },
    { "gray", qRgb(190, 190, 190) },
    { "gray0", qRgb(0, 0, 0) },
    { "gray1", qRgb(3, 3, 3) },
    { "gray10", qRgb(26, 26, 26) },
    { "gray100", qRgb(255, 255, 255) },
    { "gray11", qRgb(28, 28, 28) },
    { "gray12", qRgb(31, 31, 31) },
    { "gray13", qRgb(33, 33, 33) },
    { "gray14", qRgb(36, 36, 36) },
    { "gray15", qRgb(38, 38, 38) },
    { "gray16", qRgb(41, 41, 41) },
    { "gray17", qRgb(43, 43, 43) },
    { "gray18", qRgb(46, 46, 46) },
    { "gray19", qRgb(48, 48, 48) },
    { "gray2", qRgb(5, 5, 5) },
    { "gray20", qRgb(51, 51, 51) },
    { "gray21", qRgb(54, 54, 54) },
    { "gray22", qRgb(56, 56, 56) },
    { "gray23", qRgb(59, 59, 59) },
    { "gray24", qRgb(61, 61, 61) },
    { "gray25", qRgb(64, 64, 64) },
    { "gray26", qRgb(66, 66, 66) },
    { "gray27", qRgb(69, 69, 69) },
    { "gray28", qRgb(71, 71, 71) },
    { "gray29", qRgb(74, 74, 74) },
    { "gray3", qRgb(8, 8, 8) },
    { "gray30", qRgb(77, 77, 77) },
    { "gray31", qRgb(79, 79, 79) },
    { "gray32", qRgb(82, 82, 82) },
    { "gray33", qRgb(84, 84, 84) },
    { "gray34", qRgb(87, 87, 87) },
    { "gray35", qRgb(89, 89, 89) },
    { "gray36", qRgb(92, 92, 92) },
    { "gray37", qRgb(94, 94, 94) },
    { "gray38", qRgb(97, 97, 97) },
    { "gray39", qRgb(99, 99, 99) },
    { "gray4", qRgb(10, 10, 10) },
    { "gray40", qRgb(102, 102, 102) },
    { "gray41", qRgb(105, 105, 105) },
    { "gray42", qRgb(107, 107, 107) },
    { "gray43", qRgb(110, 110, 110) },
    { "gray44", qRgb(112, 112, 112) },
    { "gray45", qRgb(115, 115, 115) },
    { "gray46", qRgb(117, 117, 117) },
    { "gray47", qRgb(120, 120, 120) },
    { "gray48", qRgb(122, 122, 122) },
    { "gray49", qRgb(125, 125, 125) },
    { "gray5", qRgb(13, 13, 13) },
    { "gray50", qRgb(127, 127, 127) },
    { "gray51", qRgb(130, 130, 130) },
    { "gray52", qRgb(133, 133, 133) },
    { "gray53", qRgb(135, 135, 135) },
    { "gray54", qRgb(138, 138, 138) },
    { "gray55", qRgb(140, 140, 140) },
    { "gray56", qRgb(143, 143, 143) },
    { "gray57", qRgb(145, 145, 145) },
    { "gray58", qRgb(148, 148, 148) },
    { "gray59", qRgb(150, 150, 150) },
    { "gray6", qRgb(15, 15, 15) },
    { "gray60", qRgb(153, 153, 153) },
    { "gray61", qRgb(156, 156, 156) },
    { "gray62", qRgb(158, 158, 158) },
    { "gray63", qRgb(161, 161, 161) },
    { "gray64", qRgb(163, 163, 163) },
    { "gray65", qRgb(166, 166, 166) },
    { "gray66", qRgb(168, 168, 168) },
    { "gray67", qRgb(171, 171, 171) },
    { "gray68", qRgb(173, 173, 173) },
    { "gray69", qRgb(176, 176, 176) },
    { "gray7", qRgb(18, 18, 18) },
    { "gray70", qRgb(179, 179, 179) },
    { "gray71", qRgb(181, 181, 181) },
    { "gray72", qRgb(184, 184, 184) },
    { "gray73", qRgb(186, 186, 186) },
    { "gray74", qRgb(189, 189, 189) },
    { "gray75", qRgb(191, 191, 191) },
    { "gray76", qRgb(194, 194, 194) },
    { "gray77", qRgb(196, 196, 196) },
    { "gray78", qRgb(199, 199, 199) },
    { "gray79", qRgb(201, 201, 201) },
    { "gray8", qRgb(20, 20, 20) },
    { "gray80", qRgb(204, 204, 204) },
    { "gray81", qRgb(207, 207, 207) },
    { "gray82", qRgb(209, 209, 209) },
    { "gray83", qRgb(212, 212, 212) },
    { "gray84", qRgb(214, 214, 214) },
    { "gray85", qRgb(217, 217, 217) },
    { "gray86", qRgb(219, 219, 219) },
    { "gray87", qRgb(222, 222, 222) },
    { "gray88", qRgb(224, 224, 224) },
    { "gray89", qRgb(227, 227, 227) },
    { "gray9", qRgb(23, 23, 23) },
    { "gray90", qRgb(229, 229, 229) },
    { "gray91", qRgb(232, 232, 232) },
    { "gray92", qRgb(235, 235, 235) },
    { "gray93", qRgb(237, 237, 237) },
    { "gray94", qRgb(240, 240, 240) },
    { "gray95", qRgb(242, 242, 242) },
    { "gray96", qRgb(245, 245, 245) },
    { "gray97", qRgb(247, 247, 247) },
    { "gray98", qRgb(250, 250, 250) },
    { "gray99", qRgb(252, 252, 252) },
    { "green", qRgb(0, 255, 0) },
    { "green1", qRgb(0, 255, 0) },
    { "green2", qRgb(0, 238, 0) },
    { "green3", qRgb(0, 205, 0) },
    { "green4", qRgb(0, 139, 0) },
    { "greenyellow", qRgb(173, 255, 47) },
    { "grey", qRgb(190, 190, 190) },
    { "grey0", qRgb(0, 0, 0) },
    { "grey1", qRgb(3, 3, 3) },
    { "grey10", qRgb(26, 26, 26) },
    { "grey100", qRgb(255, 255, 255) },
    { "grey11", qRgb(28, 28, 28) },
    { "grey12", qRgb(31, 31, 31) },
    { "grey13", qRgb(33, 33, 33) },
    { "grey14", qRgb(36, 36, 36) },
    { "grey15", qRgb(38, 38, 38) },
    { "grey16", qRgb(41, 41, 41) },
    { "grey17", qRgb(43, 43, 43) },
    { "grey18", qRgb(46, 46, 46) },
    { "grey19", qRgb(48, 48, 48) },
    { "grey2", qRgb(5, 5, 5) },
    { "grey20", qRgb(51, 51, 51) },
    { "grey21", qRgb(54, 54, 54) },
    { "grey22", qRgb(56, 56, 56) },
    { "grey23", qRgb(59, 59, 59) },
    { "grey24", qRgb(61, 61, 61) },
    { "grey25", qRgb(64, 64, 64) },
    { "grey26", qRgb(66, 66, 66) },
    { "grey27", qRgb(69, 69, 69) },
    { "grey28", qRgb(71, 71, 71) },
    { "grey29", qRgb(74, 74, 74) },
    { "grey3", qRgb(8, 8, 8) },
    { "grey30", qRgb(77, 77, 77) },
    { "grey31", qRgb(79, 79, 79) },
    { "grey32", qRgb(82, 82, 82) },
    { "grey33", qRgb(84, 84, 84) },
    { "grey34", qRgb(87, 87, 87) },
    { "grey35", qRgb(89, 89, 89) },
    { "grey36", qRgb(92, 92, 92) },
    { "grey37", qRgb(94, 94, 94) },
    { "grey38", qRgb(97, 97, 97) },
    { "grey39", qRgb(99, 99, 99) },
    { "grey4", qRgb(10, 10, 10) },
    { "grey40", qRgb(102, 102, 102) },
    { "grey41", qRgb(105, 105, 105) },
    { "grey42", qRgb(107, 107, 107) },
    { "grey43", qRgb(110, 110, 110) },
    { "grey44", qRgb(112, 112, 112) },
    { "grey45", qRgb(115, 115, 115) },
    { "grey46", qRgb(117, 117, 117) },
    { "grey47", qRgb(120, 120, 120) },
    { "grey48", qRgb(122, 122, 122) },
    { "grey49", qRgb(125, 125, 125) },
    { "grey5", qRgb(13, 13, 13) },
    { "grey50", qRgb(127, 127, 127) },
    { "grey51", qRgb(130, 130, 130) },
    { "grey52", qRgb(133, 133, 133) },
    { "grey53", qRgb(135, 135, 135) },
    { "grey54", qRgb(138, 138, 138) },
    { "grey55", qRgb(140, 140, 140) },
    { "grey56", qRgb(143, 143, 143) },
    { "grey57", qRgb(145, 145, 145) },
    { "grey58", qRgb(148, 148, 148) },
    { "grey59", qRgb(150, 150, 150) },
    { "grey6", qRgb(15, 15, 15) },
    { "grey60", qRgb(153, 153, 153) },
    { "grey61", qRgb(156, 156, 156) },
    { "grey62", qRgb(158, 158, 158) },
    { "grey63", qRgb(161, 161, 161) },
    { "grey64", qRgb(163, 163, 163) },
    { "grey65", qRgb(166, 166, 166) },
    { "grey66", qRgb(168, 168, 168) },
    { "grey67", qRgb(171, 171, 171) },
    { "grey68", qRgb(173, 173, 173) },
    { "grey69", qRgb(176, 176, 176) },
    { "grey7", qRgb(18, 18, 18) },
    { "grey70", qRgb(179, 179, 179) },
    { "grey71", qRgb(181, 181, 181) },
    { "grey72", qRgb(184, 184, 184) },
    { "grey73", qRgb(186, 186, 186) },
    { "grey74", qRgb(189, 189, 189) },
    { "grey75", qRgb(191, 191, 191) },
    { "grey76", qRgb(194, 194, 194) },
    { "grey77", qRgb(196, 196, 196) },
    { "grey78", qRgb(199, 199, 199) },
    { "grey79", qRgb(201, 201, 201) },
    { "grey8", qRgb(20, 20, 20) },
    { "grey80", qRgb(204, 204, 204) },
    { "grey81", qRgb(207, 207, 207) },
    { "grey82", qRgb(209, 209, 209) },
    { "grey83", qRgb(212, 212, 212) },
    { "grey84", qRgb(214, 214, 214) },
    { "grey85", qRgb(217, 217, 217) },
    { "grey86", qRgb(219, 219, 219) },
    { "grey87", qRgb(222, 222, 222) },
    { "grey88", qRgb(224, 224, 224) },
    { "grey89", qRgb(227, 227, 227) },
    { "grey9", qRgb(23, 23, 23) },
    { "grey90", qRgb(229, 229, 229) },
    { "grey91", qRgb(232, 232, 232) },
    { "grey92", qRgb(235, 235, 235) },
    { "grey93", qRgb(237, 237, 237) },
    { "grey94", qRgb(240, 240, 240) },
    { "grey95", qRgb(242, 242, 242) },
    { "grey96", qRgb(245, 245, 245) },
    { "grey97", qRgb(247, 247, 247) },
    { "grey98", qRgb(250, 250, 250) },
    { "grey99", qRgb(252, 252, 252) },
    { "honeydew", qRgb(240, 255, 240) },
    { "honeydew1", qRgb(240, 255, 240) },
    { "honeydew2", qRgb(224, 238, 224) },
    { "honeydew3", qRgb(193, 205, 193) },
    { "honeydew4", qRgb(131, 139, 131) },
    { "hotpink", qRgb(255, 105, 180) },
    { "hotpink1", qRgb(255, 110, 180) },
    { "hotpink2", qRgb(238, 106, 167) },
    { "hotpink3", qRgb(205, 96, 144) },
    { "hotpink4", qRgb(139, 58, 98) },
    { "indianred", qRgb(205, 92, 92) },
    { "indianred1", qRgb(255, 106, 106) },
    { "indianred2", qRgb(238, 99, 99) },
    { "indianred3", qRgb(205, 85, 85) },
    { "indianred4", qRgb(139, 58, 58) },
    { "ivory", qRgb(255, 255, 240) },
    { "ivory1", qRgb(255, 255, 240) },
    { "ivory2", qRgb(238, 238, 224) },
    { "ivory3", qRgb(205, 205, 193) },
    { "ivory4", qRgb(139, 139, 131) },
    { "khaki", qRgb(240, 230, 140) },
    { "khaki1", qRgb(255, 246, 143) },
    { "khaki2", qRgb(238, 230, 133) },
    { "khaki3", qRgb(205, 198, 115) },
    { "khaki4", qRgb(139, 134, 78) },
    { "lavender", qRgb(230, 230, 250) },
    { "lavenderblush", qRgb(255, 240, 245) },
    { "lavenderblush1", qRgb(255, 240, 245) },
    { "lavenderblush2", qRgb(238, 224, 229) },
    { "lavenderblush3", qRgb(205, 193, 197) },
    { "lavenderblush4", qRgb(139, 131, 134) },
    { "lawngreen", qRgb(124, 252, 0) },
    { "lemonchiffon", qRgb(255, 250, 205) },
    { "lemonchiffon1", qRgb(255, 250, 205) },
    { "lemonchiffon2", qRgb(238, 233, 191) },
    { "lemonchiffon3", qRgb(205, 201, 165) },
    { "lemonchiffon4", qRgb(139, 137, 112) },
    { "lightblue", qRgb(173, 216, 230) },
    { "lightblue1", qRgb(191, 239, 255) },
    { "lightblue2", qRgb(178, 223, 238) },
    { "lightblue3", qRgb(154, 192, 205) },
    { "lightblue4", qRgb(104, 131, 139) },
    { "lightcoral", qRgb(240, 128, 128) },
    { "lightcyan", qRgb(224, 255, 255) },
    { "lightcyan1", qRgb(224, 255, 255) },
    { "lightcyan2", qRgb(209, 238, 238) },
    { "lightcyan3", qRgb(180, 205, 205) },
    { "lightcyan4", qRgb(122, 139, 139) },
    { "lightgoldenrod", qRgb(238, 221, 130) },
    { "lightgoldenrod1", qRgb(255, 236, 139) },
    { "lightgoldenrod2", qRgb(238, 220, 130) },
    { "lightgoldenrod3", qRgb(205, 190, 112) },
    { "lightgoldenrod4", qRgb(139, 129, 76) },
    { "lightgoldenrodyellow", qRgb(250, 250, 210) },
    { "lightgray", qRgb(211, 211, 211) },
    { "lightgreen", qRgb(144, 238, 144) },
    { "lightgrey", qRgb(211, 211, 211) },
    { "lightpink", qRgb(255, 182, 193) },
    { "lightpink1", qRgb(255, 174, 185) },
    { "lightpink2", qRgb(238, 162, 173) },
    { "lightpink3", qRgb(205, 140, 149) },
    { "lightpink4", qRgb(139, 95, 101) },
    { "lightsalmon", qRgb(255, 160, 122) },
    { "lightsalmon1", qRgb(255, 160, 122) },
    { "lightsalmon2", qRgb(238, 149, 114) },
    { "lightsalmon3", qRgb(205, 129, 98) },
    { "lightsalmon4", qRgb(139, 87, 66) },
    { "lightseagreen", qRgb(32, 178, 170) },
    { "lightskyblue", qRgb(135, 206, 250) },
    { "lightskyblue1", qRgb(176, 226, 255) },
    { "lightskyblue2", qRgb(164, 211, 238) },
    { "lightskyblue3", qRgb(141, 182, 205) },
    { "lightskyblue4", qRgb(96, 123, 139) },
    { "lightslateblue", qRgb(132, 112, 255) },
    { "lightslategray", qRgb(119, 136, 153) },
    { "lightslategrey", qRgb(119, 136, 153) },
    { "lightsteelblue", qRgb(176, 196, 222) },
    { "lightsteelblue1", qRgb(202, 225, 255) },
    { "lightsteelblue2", qRgb(188, 210, 238) },
    { "lightsteelblue3", qRgb(162, 181, 205) },
    { "lightsteelblue4", qRgb(110, 123, 139) },
    { "lightyellow", qRgb(255, 255, 224) },
    { "lightyellow1", qRgb(255, 255, 224) },
    { "lightyellow2", qRgb(238, 238, 209) },
    { "lightyellow3", qRgb(205, 205, 180) },
    { "lightyellow4", qRgb(139, 139, 122) },
    { "limegreen", qRgb(50, 205, 50) },
    { "linen", qRgb(250, 240, 230) },
    { "magenta", qRgb(255, 0, 255) },
    { "magenta1", qRgb(255, 0, 255) },
    { "magenta2", qRgb(238, 0, 238) },
    { "magenta3", qRgb(205, 0, 205) },
    { "magenta4", qRgb(139, 0, 139) },
    { "maroon", qRgb(176, 48, 96) },
    { "maroon1", qRgb(255, 52, 179) },
    { "maroon2", qRgb(238, 48, 167) },
    { "maroon3", qRgb(205, 41, 144) },
    { "maroon4", qRgb(139, 28, 98) },
    { "mediumaquamarine", qRgb(102, 205, 170) },
    { "mediumblue", qRgb(0, 0, 205) },
    { "mediumorchid", qRgb(186, 85, 211) },
    { "mediumorchid1", qRgb(224, 102, 255) },
    { "mediumorchid2", qRgb(209, 95, 238) },
    { "mediumorchid3", qRgb(180, 82, 205) },
    { "mediumorchid4", qRgb(122, 55, 139) },
    { "mediumpurple", qRgb(147, 112, 219) },
    { "mediumpurple1", qRgb(171, 130, 255) },
    { "mediumpurple2", qRgb(159, 121, 238) },
    { "mediumpurple3", qRgb(137, 104, 205) },
    { "mediumpurple4", qRgb(93, 71, 139) },
    { "mediumseagreen", qRgb(60, 179, 113) },
    { "mediumslateblue", qRgb(123, 104, 238) },
    { "mediumspringgreen", qRgb(0, 250, 154) },
    { "mediumturquoise", qRgb(72, 209, 204) },
    { "mediumvioletred", qRgb(199, 21, 133) },
    { "midnightblue", qRgb(25, 25, 112) },
    { "mintcream", qRgb(245, 255, 250) },
    { "mistyrose", qRgb(255, 228, 225) },
    { "mistyrose1", qRgb(255, 228, 225) },
    { "mistyrose2", qRgb(238, 213, 210) },
    { "mistyrose3", qRgb(205, 183, 181) },
    { "mistyrose4", qRgb(139, 125, 123) },
    { "moccasin", qRgb(255, 228, 181) },
    { "navajowhite", qRgb(255, 222, 173) },
    { "navajowhite1", qRgb(255, 222, 173) },
    { "navajowhite2", qRgb(238, 207, 161) },
    { "navajowhite3", qRgb(205, 179, 139) },
    { "navajowhite4", qRgb(139, 121, 94) },
    { "navy", qRgb(0, 0, 128) },
    { "navyblue", qRgb(0, 0, 128) },
    { "oldlace", qRgb(253, 245, 230) },
    { "olivedrab", qRgb(107, 142, 35) },
    { "olivedrab1", qRgb(192, 255, 62) },
    { "olivedrab2", qRgb(179, 238, 58) },
    { "olivedrab3", qRgb(154, 205, 50) },
    { "olivedrab4", qRgb(105, 139, 34) },
    { "orange", qRgb(255, 165, 0) },
    { "orange1", qRgb(255, 165, 0) },
    { "orange2", qRgb(238, 154, 0) },
    { "orange3", qRgb(205, 133, 0) },
    { "orange4", qRgb(139, 90, 0) },
    { "orangered", qRgb(255, 69, 0) },
    { "orangered1", qRgb(255, 69, 0) },
    { "orangered2", qRgb(238, 64, 0) },
    { "orangered3", qRgb(205, 55, 0) },
    { "orangered4", qRgb(139, 37, 0) },
    { "orchid", qRgb(218, 112, 214) },
    { "orchid1", qRgb(255, 131, 250) },
    { "orchid2", qRgb(238, 122, 233) },
    { "orchid3", qRgb(205, 105, 201) },
    { "orchid4", qRgb(139, 71, 137) },
    { "palegoldenrod", qRgb(238, 232, 170) },
    { "palegreen", qRgb(152, 251, 152) },
    { "palegreen1", qRgb(154, 255, 154) },
    { "palegreen2", qRgb(144, 238, 144) },
    { "palegreen3", qRgb(124, 205, 124) },
    { "palegreen4", qRgb(84, 139, 84) },
    { "paleturquoise", qRgb(175, 238, 238) },
    { "paleturquoise1", qRgb(187, 255, 255) },
    { "paleturquoise2", qRgb(174, 238, 238) },
    { "paleturquoise3", qRgb(150, 205, 205) },
    { "paleturquoise4", qRgb(102, 139, 139) },
    { "palevioletred", qRgb(219, 112, 147) },
    { "palevioletred1", qRgb(255, 130, 171) },
    { "palevioletred2", qRgb(238, 121, 159) },
    { "palevioletred3", qRgb(205, 104, 137) },
    { "palevioletred4", qRgb(139, 71, 93) },
    { "papayawhip", qRgb(255, 239, 213) },
    { "peachpuff", qRgb(255, 218, 185) },
    { "peachpuff1", qRgb(255, 218, 185) },
    { "peachpuff2", qRgb(238, 203, 173) },
    { "peachpuff3", qRgb(205, 175, 149) },
    { "peachpuff4", qRgb(139, 119, 101) },
    { "peru", qRgb(205, 133, 63) },
    { "pink", qRgb(255, 192, 203) },
    { "pink1", qRgb(255, 181, 197) },
    { "pink2", qRgb(238, 169, 184) },
    { "pink3", qRgb(205, 145, 158) },
    { "pink4", qRgb(139, 99, 108) },
    { "plum", qRgb(221, 160, 221) },
    { "plum1", qRgb(255, 187, 255) },
    { "plum2", qRgb(238, 174, 238) },
    { "plum3", qRgb(205, 150, 205) },
    { "plum4", qRgb(139, 102, 139) },
    { "powderblue", qRgb(176, 224, 230) },
    { "purple", qRgb(160, 32, 240) },
    { "purple1", qRgb(155, 48, 255) },
    { "purple2", qRgb(145, 44, 238) },
    { "purple3", qRgb(125, 38, 205) },
    { "purple4", qRgb(85, 26, 139) },
    { "red", qRgb(255, 0, 0) },
    { "red1", qRgb(255, 0, 0) },
    { "red2", qRgb(238, 0, 0) },
    { "red3", qRgb(205, 0, 0) },
    { "red4", qRgb(139, 0, 0) },
    { "rosybrown", qRgb(188, 143, 143) },
    { "rosybrown1", qRgb(255, 193, 193) },
    { "rosybrown2", qRgb(238, 180, 180) },
    { "rosybrown3", qRgb(205, 155, 155) },
    { "rosybrown4", qRgb(139, 105, 105) },
    { "royalblue", qRgb(65, 105, 225) },
    { "royalblue1", qRgb(72, 118, 255) },
    { "royalblue2", qRgb(67, 110, 238) },
    { "royalblue3", qRgb(58, 95, 205) },
    { "royalblue4", qRgb(39, 64, 139) },
    { "saddlebrown", qRgb(139, 69, 19) },
    { "salmon", qRgb(250, 128, 114) },
    { "salmon1", qRgb(255, 140, 105) },
    { "salmon2", qRgb(238, 130, 98) },
    { "salmon3", qRgb(205, 112, 84) },
    { "salmon4", qRgb(139, 76, 57) },
    { "sandybrown", qRgb(244, 164, 96) },
    { "seagreen", qRgb(46, 139, 87) },
    { "seagreen1", qRgb(84, 255, 159) },
    { "seagreen2", qRgb(78, 238, 148) },
    { "seagreen3", qRgb(67, 205, 128) },
    { "seagreen4", qRgb(46, 139, 87) },
    { "seashell", qRgb(255, 245, 238) },
    { "seashell1", qRgb(255, 245, 238) },
    { "seashell2", qRgb(238, 229, 222) },
    { "seashell3", qRgb(205, 197, 191) },
    { "seashell4", qRgb(139, 134, 130) },
    { "sienna", qRgb(160, 82, 45) },
    { "sienna1", qRgb(255, 130, 71) },
    { "sienna2", qRgb(238, 121, 66) },
    { "sienna3", qRgb(205, 104, 57) },
    { "sienna4", qRgb(139, 71, 38) },
    { "skyblue", qRgb(135, 206, 235) },
    { "skyblue1", qRgb(135, 206, 255) },
    { "skyblue2", qRgb(126, 192, 238) },
    { "skyblue3", qRgb(108, 166, 205) },
    { "skyblue4", qRgb(74, 112, 139) },
    { "slateblue", qRgb(106, 90, 205) },
    { "slateblue1", qRgb(131, 111, 255) },
    { "slateblue2", qRgb(122, 103, 238) },
    { "slateblue3", qRgb(105, 89, 205) },
    { "slateblue4", qRgb(71, 60, 139) },
    { "slategray", qRgb(112, 128, 144) },
    { "slategray1", qRgb(198, 226, 255) },
    { "slategray2", qRgb(185, 211, 238) },
    { "slategray3", qRgb(159, 182, 205) },
    { "slategray4", qRgb(108, 123, 139) },
    { "slategrey", qRgb(112, 128, 144) },
    { "snow", qRgb(255, 250, 250) },
    { "snow1", qRgb(255, 250, 250) },
    { "snow2", qRgb(238, 233, 233) },
    { "snow3", qRgb(205, 201, 201) },
    { "snow4", qRgb(139, 137, 137) },
    { "springgreen", qRgb(0, 255, 127) },
    { "springgreen1", qRgb(0, 255, 127) },
    { "springgreen2", qRgb(0, 238, 118) },
    { "springgreen3", qRgb(0, 205, 102) },
    { "springgreen4", qRgb(0, 139, 69) },
    { "steelblue", qRgb(70, 130, 180) },
    { "steelblue1", qRgb(99, 184, 255) },
    { "steelblue2", qRgb(92, 172, 238) },
    { "steelblue3", qRgb(79, 148, 205) },
    { "steelblue4", qRgb(54, 100, 139) },
    { "tan", qRgb(210, 180, 140) },
    { "tan1", qRgb(255, 165, 79) },
    { "tan2", qRgb(238, 154, 73) },
    { "tan3", qRgb(205, 133, 63) },
    { "tan4", qRgb(139, 90, 43) },
    { "thistle", qRgb(216, 191, 216) },
    { "thistle1", qRgb(255, 225, 255) },
    { "thistle2", qRgb(238, 210, 238) },
    { "thistle3", qRgb(205, 181, 205) },
    { "thistle4", qRgb(139, 123, 139) },
    { "tomato", qRgb(255, 99, 71) },
    { "tomato1", qRgb(255, 99, 71) },
    { "tomato2", qRgb(238, 92, 66) },
    { "tomato3", qRgb(205, 79, 57) },
    { "tomato4", qRgb(139, 54, 38) },
    { "turquoise", qRgb(64, 224, 208) },
    { "turquoise1", qRgb(0, 245, 255) },
    { "turquoise2", qRgb(0, 229, 238) },
    { "turquoise3", qRgb(0, 197, 205) },
    { "turquoise4", qRgb(0, 134, 139) },
    { "violet", qRgb(238, 130, 238) },
    { "violetred", qRgb(208, 32, 144) },
    { "violetred1", qRgb(255, 62, 150) },
    { "violetred2", qRgb(238, 58, 140) },
    { "violetred3", qRgb(205, 50, 120) },
    { "violetred4", qRgb(139, 34, 82) },
    { "wheat", qRgb(245, 222, 179) },
    { "wheat1", qRgb(255, 231, 186) },
    { "wheat2", qRgb(238, 216, 174) },
    { "wheat3", qRgb(205, 186, 150) },
    { "wheat4", qRgb(139, 126, 102) },
    { "white", qRgb(255, 255, 255) },
    { "whitesmoke", qRgb(245, 245, 245) },
    { "yellow", qRgb(255, 255, 0) },
    { "yellow1", qRgb(255, 255, 0) },
    { "yellow2", qRgb(238, 238, 0) },
    { "yellow3", qRgb(205, 205, 0) },
    { "yellow4", qRgb(139, 139, 0) },
    { "yellowgreen", qRgb(154, 205, 50) },
};
static const qint16 X11RGBTblSize = sizeof(X11RGBTbl) / sizeof(X11RGBTblData);

#endif // QT_NO_COLORNAMES

bool qt_get_hex_rgb(const char *, const int, QRgb *);

QT_END_NAMESPACE

#endif // QCOLOR_P_H
