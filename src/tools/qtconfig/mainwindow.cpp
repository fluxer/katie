/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "colorbutton.h"
#include "previewframe.h"
#include "paletteeditoradvanced.h"

#include <QLabel>
#include <QApplication>
#include <QComboBox>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QAction>
#include <QStatusBar>
#include <QSettings>
#include <QMessageBox>
#include <QStyle>
#include <QCloseEvent>
#include <QDebug>
#include <QPixmap>

#include <stdlib.h>

QT_BEGIN_NAMESPACE

// from qapplication.cpp and qapplication_x11.cpp - These are NOT for
// external use ignore them
// extern bool Q_CORE_EXPORT qt_resolve_symlinks;

static const uchar* qrc_qtconfig()
{
    static const uchar static_qrc_data[] = {
        0x89, 0x50, 0x4e, 0x47, 0xd, 0xa, 0x1a, 0xa, 0x0, 0x0, 0x0, 0xd, 0x49, 0x48, 0x44, 0x52, 0x0, 0x0, 0x0, 0x40,
        0x0, 0x0, 0x0, 0x40, 0x8, 0x4, 0x0, 0x0, 0x0, 0x0, 0x60, 0xb9, 0x55, 0x0, 0x0, 0x0, 0x4, 0x67, 0x41, 0x4d,
        0x41, 0x0, 0x0, 0xb1, 0x8f, 0xb, 0xfc, 0x61, 0x5, 0x0, 0x0, 0x0, 0x20, 0x63, 0x48, 0x52, 0x4d, 0x0, 0x0, 0x7a,
        0x26, 0x0, 0x0, 0x80, 0x84, 0x0, 0x0, 0xfa, 0x0, 0x0, 0x0, 0x80, 0xe8, 0x0, 0x0, 0x75, 0x30, 0x0, 0x0, 0xea,
        0x60, 0x0, 0x0, 0x3a, 0x98, 0x0, 0x0, 0x17, 0x70, 0x9c, 0xba, 0x51, 0x3c, 0x0, 0x0, 0x0, 0x2, 0x62, 0x4b, 0x47,
        0x44, 0x0, 0x0, 0xaa, 0x8d, 0x23, 0x32, 0x0, 0x0, 0x0, 0x9, 0x70, 0x48, 0x59, 0x73, 0x0, 0x0, 0xe, 0xc4, 0x0,
        0x0, 0xe, 0xc4, 0x1, 0x95, 0x2b, 0xe, 0x1b, 0x0, 0x0, 0x0, 0x7, 0x74, 0x49, 0x4d, 0x45, 0x7, 0xe3, 0xb, 0x1d,
        0x2, 0x3b, 0x3, 0x2f, 0x98, 0x17, 0x33, 0x0, 0x0, 0x4, 0xd, 0x49, 0x44, 0x41, 0x54, 0x68, 0xde, 0xc5, 0x98, 0x5b,
        0x6c, 0x14, 0x55, 0x18, 0xc7, 0x7f, 0xdb, 0xd2, 0x92, 0x2c, 0xd0, 0xae, 0x5c, 0x34, 0x95, 0x36, 0x68, 0x83, 0x1a, 0x20,
        0xc5, 0x44, 0xc2, 0x45, 0x1, 0x25, 0xc6, 0x7, 0xa3, 0x1, 0x41, 0x51, 0x91, 0x90, 0x48, 0x42, 0x2, 0x89, 0xd7, 0x98,
        0x48, 0xe0, 0x41, 0x8, 0x88, 0x97, 0xc0, 0xb, 0x1a, 0x5e, 0x30, 0x5e, 0x1a, 0x13, 0xd, 0x11, 0x45, 0xd2, 0x6a, 0x42,
        0x2c, 0xd1, 0x20, 0x4, 0x88, 0x4, 0x4c, 0xa0, 0x60, 0x42, 0xa0, 0x9, 0x97, 0x56, 0x8c, 0xb4, 0x5c, 0xb6, 0xd0, 0x16,
        0xba, 0x74, 0x78, 0x98, 0xdd, 0x6f, 0xbf, 0xd9, 0x99, 0xd9, 0x9d, 0xd9, 0xd9, 0x99, 0xfd, 0xe6, 0x61, 0xff, 0x33, 0xdf,
        0x99, 0xf3, 0xff, 0xcd, 0x99, 0xb3, 0x67, 0xce, 0x39, 0x10, 0x34, 0x86, 0xb3, 0x86, 0x7f, 0xb8, 0xcd, 0x4d, 0x2e, 0xd0,
        0xc6, 0x7, 0x4c, 0xe, 0x5c, 0xa3, 0xaf, 0x18, 0xc9, 0x1, 0x8c, 0x9c, 0xa3, 0x8d, 0xa6, 0xe8, 0x0, 0x76, 0xd9, 0xec,
        0xd, 0xc, 0x6e, 0xb1, 0x3c, 0x1a, 0xfb, 0x5, 0x8e, 0xf6, 0x6, 0x6, 0x77, 0x58, 0x1c, 0x5, 0xc0, 0x41, 0xf5, 0xcc,
        0xb7, 0x72, 0x10, 0xae, 0x72, 0x5f, 0xd8, 0xf6, 0x13, 0x18, 0x12, 0xbb, 0x85, 0xc0, 0x68, 0x16, 0xb2, 0x5f, 0x21, 0x6c,
        0xd, 0x1b, 0x60, 0xb9, 0x58, 0xed, 0x93, 0x6b, 0x31, 0xbe, 0x91, 0xab, 0x57, 0xa8, 0xe, 0x17, 0xe0, 0x6b, 0xb1, 0x7a,
        0x4f, 0x5d, 0xad, 0xa1, 0x47, 0xae, 0xcf, 0x29, 0x5c, 0x49, 0x45, 0x0, 0x80, 0x27, 0x45, 0xed, 0x51, 0x57, 0x93, 0xb4,
        0x8a, 0x9e, 0x15, 0x26, 0x40, 0x1d, 0x13, 0xd3, 0xea, 0x7f, 0x4e, 0x5b, 0x32, 0x87, 0x44, 0x35, 0x86, 0x9, 0x30, 0x4f,
        0xd4, 0x7e, 0xc, 0x4b, 0xe6, 0x8c, 0xa8, 0xf1, 0x61, 0x2, 0x3c, 0xab, 0x0, 0xac, 0x71, 0x5d, 0x54, 0x3c, 0x3c, 0x80,
        0x51, 0x3c, 0xe7, 0xa, 0x90, 0x12, 0x55, 0x15, 0x1e, 0xc0, 0x87, 0x8c, 0x4d, 0xab, 0xe, 0x4e, 0xe4, 0xe4, 0xb2, 0xd,
        0x7f, 0x2d, 0x2c, 0x80, 0x57, 0x78, 0x57, 0xf4, 0x97, 0x39, 0x3d, 0x40, 0xf7, 0xfd, 0x8b, 0x45, 0x3e, 0x5e, 0x81, 0x98,
        0x4e, 0x9f, 0x1a, 0x70, 0xc7, 0xd9, 0xf2, 0x27, 0x24, 0xbb, 0x22, 0xc, 0xfb, 0x7a, 0xba, 0xd4, 0x70, 0xbb, 0xcc, 0x96,
        0x9f, 0xa9, 0xb2, 0x1e, 0xfe, 0x86, 0x7e, 0x23, 0xce, 0x31, 0x65, 0xb0, 0xc5, 0xa1, 0xc4, 0x57, 0x92, 0x3d, 0x5e, 0x7a,
        0xfb, 0x18, 0x3f, 0x29, 0xfb, 0xdd, 0xe, 0x3d, 0xa8, 0x86, 0x5e, 0xc9, 0xbf, 0xed, 0xa5, 0xca, 0x61, 0xbe, 0x0, 0xd6,
        0xf3, 0x92, 0xe8, 0xbf, 0x59, 0xc6, 0x90, 0xad, 0xc4, 0x6b, 0x8c, 0x4c, 0x2b, 0x83, 0x3e, 0x5e, 0x96, 0xeb, 0x29, 0x2e,
        0xd3, 0xc9, 0xb9, 0x60, 0xcf, 0xbf, 0x48, 0x7d, 0x7e, 0xbb, 0x5c, 0x46, 0x39, 0xfd, 0x82, 0xec, 0xc7, 0x59, 0x36, 0x91,
        0x28, 0xd6, 0xbe, 0x49, 0x35, 0xee, 0x20, 0x73, 0x1d, 0xcb, 0x4c, 0xcb, 0x6b, 0x6f, 0x1e, 0xdd, 0xaa, 0x15, 0x7d, 0xc4,
        0x38, 0x3a, 0x54, 0x25, 0xef, 0xbb, 0x94, 0xfa, 0xc8, 0x3, 0x80, 0xc1, 0x10, 0xab, 0xfd, 0xda, 0x27, 0x2c, 0x4d, 0xbb,
        0x8b, 0x98, 0x4b, 0xb9, 0xbd, 0x9e, 0x0, 0xc, 0x86, 0xb2, 0x7f, 0x5f, 0x5d, 0xd5, 0x28, 0xe6, 0x52, 0x47, 0xa5, 0x43,
        0xc5, 0x6f, 0xa9, 0x89, 0x76, 0x3b, 0x4f, 0x70, 0xc3, 0x5, 0x60, 0x1d, 0xf5, 0x2e, 0x99, 0xa, 0xea, 0x78, 0x4a, 0x3a,
        0x28, 0xf4, 0x33, 0x89, 0xf3, 0xb9, 0x26, 0x49, 0xf, 0xec, 0x5d, 0x4c, 0x28, 0xe6, 0xd, 0x2, 0x50, 0xcb, 0x76, 0x55,
        0xd3, 0xb7, 0xd6, 0xe4, 0x6a, 0x4f, 0x4d, 0xd7, 0x13, 0x78, 0xc9, 0xb1, 0x45, 0xea, 0xea, 0x57, 0xed, 0x41, 0x3, 0x3,
        0x1e, 0xec, 0xff, 0x2d, 0xc1, 0x8a, 0xa7, 0x8a, 0x33, 0x52, 0xdf, 0x2, 0xf3, 0xdd, 0x0, 0xcc, 0x67, 0x78, 0xc1, 0x5b,
        0x7f, 0x67, 0x16, 0xed, 0x81, 0x1, 0x6, 0xd9, 0x29, 0xba, 0x11, 0x32, 0x23, 0x61, 0x7d, 0xde, 0x9b, 0xbe, 0xe3, 0x3c,
        0x47, 0xf8, 0xc5, 0xf6, 0xd9, 0x2d, 0x2e, 0x3a, 0x45, 0x8d, 0xc9, 0x2, 0x54, 0xe6, 0xbd, 0x65, 0x3b, 0x7, 0x4b, 0x62,
        0x6d, 0x46, 0x83, 0xa8, 0x24, 0x4, 0x9b, 0x96, 0x17, 0x17, 0x4f, 0x8b, 0xba, 0x50, 0xe, 0x80, 0xc9, 0xcc, 0x14, 0x7d,
        0x38, 0x7a, 0x80, 0x4a, 0x3e, 0x13, 0xfd, 0x97, 0xd9, 0x2, 0xfe, 0x3e, 0xc7, 0xc1, 0x22, 0xc1, 0x6f, 0xcc, 0x90, 0xb3,
        0x6d, 0xe6, 0x4f, 0x94, 0x2d, 0x70, 0x4d, 0x2d, 0x59, 0x8e, 0xb3, 0x23, 0x7a, 0x0, 0x68, 0x16, 0x35, 0x98, 0x99, 0xcc,
        0x44, 0xb, 0x70, 0x48, 0xe6, 0x50, 0x99, 0x75, 0x65, 0xc4, 0x0, 0xfd, 0xdc, 0x4c, 0x2b, 0xf9, 0xe, 0x44, 0xb, 0x30,
        0x42, 0x8c, 0x93, 0xe5, 0x1, 0x78, 0x46, 0xe6, 0x1f, 0xd2, 0x1d, 0xa3, 0x4, 0x88, 0xb1, 0x56, 0xb4, 0xc, 0xee, 0x51,
        0x2, 0x6c, 0x52, 0xab, 0xc6, 0x1f, 0xca, 0x1, 0x90, 0xdd, 0xd2, 0x39, 0xc0, 0x91, 0x72, 0x0, 0xfc, 0x28, 0x4a, 0x2d,
        0xda, 0xa2, 0x4, 0xc8, 0x4e, 0x42, 0x6b, 0xcb, 0x3, 0x70, 0x8f, 0xa8, 0xec, 0x26, 0x4e, 0xa4, 0x0, 0x8f, 0x8b, 0xfa,
        0xaf, 0x1c, 0x0, 0x9, 0xb5, 0x54, 0x3d, 0x9c, 0xf, 0xa0, 0x95, 0x17, 0x78, 0x9d, 0x8e, 0x12, 0xdb, 0x57, 0xb0, 0x8d,
        0xd1, 0x69, 0x7d, 0x45, 0xed, 0x24, 0xa6, 0x63, 0xb3, 0xda, 0xf5, 0x35, 0xc7, 0xaa, 0x6, 0xb5, 0x14, 0x9d, 0x5d, 0x2,
        0x80, 0x2f, 0xd4, 0xf4, 0xfe, 0x53, 0x2b, 0x99, 0x35, 0x5a, 0xd2, 0x73, 0xdf, 0x8b, 0x1c, 0x2d, 0x69, 0xb, 0x64, 0x77,
        0x92, 0x2e, 0x59, 0xf7, 0x55, 0x72, 0x1, 0x32, 0xbb, 0x3a, 0x95, 0x1, 0x96, 0x60, 0x4e, 0x71, 0x59, 0xd4, 0x46, 0xae,
        0xe6, 0x3, 0x58, 0xc9, 0x22, 0xa0, 0x9a, 0xcf, 0x79, 0xb0, 0xa4, 0x0, 0x23, 0x44, 0xe5, 0xf4, 0x2e, 0x73, 0x4e, 0x38,
        0x20, 0xe7, 0xd5, 0xfc, 0x4c, 0x37, 0x71, 0xcb, 0x26, 0xeb, 0x4e, 0xae, 0x73, 0x89, 0x93, 0xfc, 0xc9, 0x5e, 0x7a, 0x8b,
        0x4, 0x78, 0x58, 0x54, 0xb7, 0x53, 0xfa, 0x55, 0x8f, 0xeb, 0xfa, 0x1, 0x5a, 0x59, 0xaa, 0x9e, 0xc6, 0x6b, 0xdc, 0x4f,
        0x4a, 0x6a, 0x70, 0xbc, 0x3b, 0x4e, 0xa7, 0x47, 0x4, 0x3, 0x83, 0x5e, 0xbe, 0x67, 0xb1, 0x2f, 0x8c, 0x66, 0xb9, 0xf7,
        0xf, 0xb7, 0x22, 0xb3, 0x3d, 0xed, 0xe, 0xe8, 0xa3, 0x8f, 0x16, 0x56, 0xf1, 0x48, 0x41, 0xf3, 0x61, 0x7c, 0xac, 0xee,
        0x5a, 0xe9, 0x5e, 0x70, 0x12, 0xbb, 0xe9, 0xf7, 0x9, 0x61, 0x2e, 0xda, 0x77, 0xb0, 0x96, 0xf9, 0x34, 0xea, 0xf5, 0x7e,
        0xda, 0xba, 0x89, 0x77, 0x38, 0xa5, 0xca, 0x76, 0xd8, 0xf7, 0xcf, 0x63, 0x39, 0x67, 0x9, 0x47, 0xb8, 0x38, 0x8f, 0x31,
        0x8f, 0x17, 0x79, 0xa0, 0xc0, 0xd3, 0xa6, 0x54, 0x27, 0xad, 0xb2, 0x1, 0xa5, 0x78, 0x9e, 0x36, 0x1f, 0x2f, 0xce, 0x21,
        0xa6, 0xf1, 0x9, 0xa7, 0x8b, 0x68, 0x25, 0xf3, 0x78, 0x23, 0x98, 0x79, 0x36, 0xa6, 0xb0, 0x81, 0xa3, 0x3e, 0xcd, 0x93,
        0x2c, 0x29, 0x95, 0x7d, 0x26, 0x1e, 0xe2, 0x4d, 0x5a, 0x3c, 0x75, 0xdd, 0xdb, 0x34, 0xbb, 0xef, 0x9b, 0xc7, 0xfc, 0x78,
        0x3a, 0x44, 0x15, 0x8f, 0x32, 0x95, 0xa9, 0x34, 0x71, 0x2f, 0xb5, 0xd4, 0x52, 0x23, 0x99, 0x14, 0x3d, 0xf4, 0xd0, 0xce,
        0x3e, 0x7e, 0x55, 0xbb, 0x22, 0xb6, 0xb8, 0xb, 0x80, 0x9c, 0x7f, 0x17, 0xe4, 0xd1, 0x5b, 0x68, 0x0, 0x0, 0x0, 0x25,
        0x74, 0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3a, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x0, 0x32, 0x30, 0x31, 0x39,
        0x2d, 0x31, 0x31, 0x2d, 0x32, 0x39, 0x54, 0x30, 0x32, 0x3a, 0x35, 0x39, 0x3a, 0x30, 0x33, 0x2b, 0x30, 0x30, 0x3a, 0x30,
        0x30, 0xb3, 0x26, 0x10, 0x56, 0x0, 0x0, 0x0, 0x25, 0x74, 0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3a, 0x6d, 0x6f,
        0x64, 0x69, 0x66, 0x79, 0x0, 0x32, 0x30, 0x31, 0x39, 0x2d, 0x31, 0x31, 0x2d, 0x32, 0x39, 0x54, 0x30, 0x32, 0x3a, 0x35,
        0x39, 0x3a, 0x30, 0x33, 0x2b, 0x30, 0x30, 0x3a, 0x30, 0x30, 0xc2, 0x7b, 0xa8, 0xea, 0x0, 0x0, 0x0, 0x19, 0x74, 0x45,
        0x58, 0x74, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65, 0x0, 0x77, 0x77, 0x77, 0x2e, 0x69, 0x6e, 0x6b, 0x73, 0x63,
        0x61, 0x70, 0x65, 0x2e, 0x6f, 0x72, 0x67, 0x9b, 0xee, 0x3c, 0x1a, 0x0, 0x0, 0x0, 0x0, 0x49, 0x45, 0x4e, 0x44, 0xae,
        0x42, 0x60, 0x82
    };
    return static_qrc_data;
};
static const qint64 qrc_qtconfig_size = 1343;

static const char *appearance_text = QT_TRANSLATE_NOOP("MainWindow",
"<p><b><font size+=2>Appearance</font></b></p>"
"<hr>"
"<p>Use this tab to customize the appearance of your Katie applications.</p>"
"<p>You can select the default GUI Style from the drop down list and "
"customize the colors.</p>"
"<p>Any GUI Style plugins in your plugin path will automatically be added "
"to the list of built-in Katie styles. (See the Library Paths tab for "
"information on adding new plugin paths.)</p>"
"<p>When you choose 3-D Effects and Window Background colors, the Katie "
"Configuration program will automatically generate a palette for you. "
"To customize colors further, press the Tune Palette button to open "
"the advanced palette editor.</p>"
"<p>The Preview Window shows what the selected Style and colors look "
"like.</p>"
);

static const char *font_text = QT_TRANSLATE_NOOP("MainWindow",
"<p><b><font size+=2>Fonts</font></b></p>"
"<hr>"
"<p>Use this tab to select the default font for your Katie applications. "
"The selected font is shown (initially as 'Sample Text') in the line "
"edit below the Family, Style and Point Size drop down lists.</p>"
"<p>You can specify if Katie should try to embed fonts into its generated output "
"when printing. If you enable font embedding, the resulting postscript will be "
"more portable and will more accurately reflect the "
"visual output on the screen; however the resulting postscript file "
"size will be bigger.</p>"
);

static const char *interface_text = QT_TRANSLATE_NOOP("MainWindow",
"<p><b><font size+=2>Interface</font></b></p>"
"<hr>"
"<p>Use this tab to customize the feel of your Katie applications.</p>"
"<p>If the Resolve Symlinks checkbox is checked Katie will follow symlinks "
"when handling URLs. For example, in the file dialog, if this setting is turned "
"on and /usr/tmp is a symlink to /var/tmp, entering the /usr/tmp directory "
"will cause the file dialog to change to /var/tmp.  With this setting turned "
"off, symlinks are not resolved or followed.</p>"
"<p>The Global Strut setting is useful for people who require a "
"minimum size for all widgets (e.g. when using a touch panel or for users "
"who are visually impaired).  Leaving the Global Strut width and height "
"at 0 will disable the Global Strut feature.</p>"
);
// ### What does the 'Enhanced support for languages written R2L do?

QPalette::ColorGroup MainWindow::groupFromIndex(int item)
{
    switch (item) {
    case 0:
    default:
        return QPalette::Active;
    case 1:
        return QPalette::Inactive;
    case 2:
        return QPalette::Disabled;
    }
}

static void setStyleHelper(QWidget *w, QStyle *s)
{
    const QObjectList children = w->children();
    for (int i = 0; i < children.size(); ++i) {
        QObject *child = children.at(i);
        if (child->isWidgetType())
            setStyleHelper((QWidget *) child, s);
    }
    w->setStyle(s);
}

MainWindow::MainWindow()
    : ui(new Ui::MainWindow),
      editPalette(palette()),
      previewPalette(palette()),
      previewstyle(0)
{
    ui->setupUi(this);
    statusBar();

    // signals and slots connections
    connect(ui->fontStyleCombo, SIGNAL(activated(int)), SLOT(buildFont()));
    connect(ui->pointSizeCombo, SIGNAL(activated(int)), SLOT(buildFont()));
    connect(ui->fontFamilyCombo, SIGNAL(activated(QString)), SLOT(familySelected(QString)));
    connect(ui->fileExitAction, SIGNAL(triggered()), SLOT(fileExit()));
    connect(ui->fileSaveAction, SIGNAL(triggered()), SLOT(fileSave()));
    connect(ui->helpAboutAction, SIGNAL(triggered()), SLOT(helpAbout()));
    connect(ui->helpAboutQtAction, SIGNAL(triggered()), SLOT(helpAboutQt()));
    connect(ui->mainTabWidget, SIGNAL(currentChanged(int)), SLOT(pageChanged(int)));
    connect(ui->paletteCombo, SIGNAL(activated(int)), SLOT(paletteSelected(int)));
    connect(ui->toolBoxEffectCombo, SIGNAL(activated(int)), SLOT(somethingModified()));
    connect(ui->doubleClickIntervalSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->cursorFlashTimeSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->wheelScrollLinesSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->menuEffectCombo, SIGNAL(activated(int)), SLOT(somethingModified()));
    connect(ui->comboEffectCombo, SIGNAL(activated(int)), SLOT(somethingModified()));
    connect(ui->toolTipEffectCombo, SIGNAL(activated(int)), SLOT(somethingModified()));
    connect(ui->strutWidthSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->strutHeightSpinBox, SIGNAL(valueChanged(int)), SLOT(somethingModified()));
    connect(ui->effectsCheckBox, SIGNAL(toggled(bool)), SLOT(effectsToggled(bool)));
    connect(ui->resolveLinksCheckBox, SIGNAL(toggled(bool)), SLOT(somethingModified()));
    connect(ui->fontEmbeddingCheckBox, SIGNAL(clicked()), SLOT(somethingModified()));
    connect(ui->rtlExtensionsCheckBox, SIGNAL(toggled(bool)), SLOT(somethingModified()));
    connect(ui->guiStyleCombo, SIGNAL(activated(QString)), SLOT(styleSelected(QString)));
    connect(ui->tunePaletteButton, SIGNAL(clicked()), SLOT(tunePalette()));

    modified = true;
    desktopThemeName = tr("Desktop Settings (Default)");
    QPixmap windowIconPixmap;
    windowIconPixmap.loadFromData(qrc_qtconfig(), qrc_qtconfig_size, "PNG");
    setWindowIcon(windowIconPixmap);
    QStringList gstyles = QStyleFactory::keys();
    gstyles.sort();
    ui->guiStyleCombo->addItem(desktopThemeName);
    ui->guiStyleCombo->setItemData(ui->guiStyleCombo->findText(desktopThemeName),
                                   tr("Choose style and palette based on your desktop settings."),
                                   Qt::ToolTipRole);
    ui->guiStyleCombo->addItems(gstyles);

    QSettings settings(QLatin1String("Katie"), QSettings::NativeFormat);

    QString currentstyle = settings.value(QLatin1String("Qt/style")).toString();
    if (currentstyle.isEmpty()) {
        ui->guiStyleCombo->setCurrentIndex(ui->guiStyleCombo->findText(desktopThemeName));
        currentstyle = QApplication::style()->objectName();
    } else {
        int index = ui->guiStyleCombo->findText(currentstyle, Qt::MatchFixedString);
        if (index != -1) {
            ui->guiStyleCombo->setCurrentIndex(index);
        } else { // we give up
            ui->guiStyleCombo->addItem(tr("Unknown"));
            ui->guiStyleCombo->setCurrentIndex(ui->guiStyleCombo->count() - 1);
        }
    }
    ui->buttonMainColor->setColor(palette().color(QPalette::Active, QPalette::Button));
    ui->buttonWindowColor->setColor(palette().color(QPalette::Active, QPalette::Window));
    connect(ui->buttonMainColor, SIGNAL(colorChanged(QColor)), SLOT(buildPalette()));
    connect(ui->buttonWindowColor, SIGNAL(colorChanged(QColor)), SLOT(buildPalette()));

#ifndef Q_WS_X11
    ui->colorConfig->hide();
#endif

    QFontDatabase db;
    QStringList families = db.families();
    ui->fontFamilyCombo->addItems(families);

    QList<int> sizes = db.standardSizes();
    foreach(int i, sizes)
        ui->pointSizeCombo->addItem(QString::number(i));

    ui->doubleClickIntervalSpinBox->setValue(QApplication::doubleClickInterval());
    ui->cursorFlashTimeSpinBox->setValue(QApplication::cursorFlashTime());
    ui->wheelScrollLinesSpinBox->setValue(QApplication::wheelScrollLines());
    // #############
    // resolveLinksCheckBox->setChecked(qt_resolve_symlinks);

    ui->effectsCheckBox->setChecked(QApplication::isEffectEnabled(Qt::UI_General));
    ui->effectsFrame->setEnabled(ui->effectsCheckBox->isChecked());

    if (QApplication::isEffectEnabled(Qt::UI_FadeMenu))
        ui->menuEffectCombo->setCurrentIndex(2);
    else if (QApplication::isEffectEnabled(Qt::UI_AnimateMenu))
        ui->menuEffectCombo->setCurrentIndex(1);

    if (QApplication::isEffectEnabled(Qt::UI_AnimateCombo))
        ui->comboEffectCombo->setCurrentIndex(1);

    if (QApplication::isEffectEnabled(Qt::UI_FadeTooltip))
        ui->toolTipEffectCombo->setCurrentIndex(2);
    else if (QApplication::isEffectEnabled(Qt::UI_AnimateTooltip))
        ui->toolTipEffectCombo->setCurrentIndex(1);

    if (QApplication::isEffectEnabled(Qt::UI_AnimateToolBox))
        ui->toolBoxEffectCombo->setCurrentIndex(1);

    QSize globalStrut = QApplication::globalStrut();
    ui->strutWidthSpinBox->setValue(globalStrut.width());
    ui->strutHeightSpinBox->setValue(globalStrut.height());

    // find the default family
    QStringList::Iterator sit = families.begin();
    int i = 0, possible = -1;
    while (sit != families.end()) {
        if (*sit == QApplication::font().family())
            break;
        if ((*sit).contains(QApplication::font().family()))
            possible = i;

        i++;
        sit++;
    }
    if (sit == families.end())
        i = possible;
    if (i == -1) // no clue about the current font
        i = 0;

    ui->fontFamilyCombo->setCurrentIndex(i);

    QStringList styles = db.styles(ui->fontFamilyCombo->currentText());
    ui->fontStyleCombo->addItems(styles);

    QString stylestring = db.styleString(QApplication::font());
    sit = styles.begin();
    i = 0;
    possible = -1;
    while (sit != styles.end()) {
        if (*sit == stylestring)
            break;
        if ((*sit).contains(stylestring))
            possible = i;

        i++;
        sit++;
    }
    if (sit == styles.end())
        i = possible;
    if (i == -1) // no clue about the current font
        i = 0;
    ui->fontStyleCombo->setCurrentIndex(i);

    i = 0;
    for (int psize = QApplication::font().pointSize(); i < ui->pointSizeCombo->count(); ++i) {
        const int sz = ui->pointSizeCombo->itemText(i).toInt();
        if (sz == psize) {
            ui->pointSizeCombo->setCurrentIndex(i);
            break;
        } else if(sz > psize) {
            ui->pointSizeCombo->insertItem(i, QString::number(psize));
            ui->pointSizeCombo->setCurrentIndex(i);
            break;
        }
    }

    ui->rtlExtensionsCheckBox->setChecked(settings.value(QLatin1String("Qt/useRtlExtensions"), false)
                                          .toBool());

    ui->fontEmbeddingCheckBox->setChecked(settings.value(QLatin1String("Qt/embedFonts"), true)
                                          .toBool());

    ui->helpView->setText(tr(appearance_text));

    setModified(false);
    updateStyleLayout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

#ifdef Q_WS_X11
extern void qt_x11_apply_settings_in_all_apps();
#endif

void MainWindow::fileSave()
{
    if (! modified) {
        statusBar()->showMessage(tr("No changes to be saved."), 2000);
        return;
    }

    statusBar()->showMessage(tr("Saving changes..."));

    {
        QSettings settings(QLatin1String("Katie"), QSettings::NativeFormat);
        QFontDatabase db;
        QFont font = db.font(ui->fontFamilyCombo->currentText(),
                             ui->fontStyleCombo->currentText(),
                             ui->pointSizeCombo->currentText().toInt());

        QStringList actcg, inactcg, discg;
        bool overrideDesktopSettings = (ui->guiStyleCombo->currentText() != desktopThemeName);
        if (overrideDesktopSettings) {
            int i;
            for (i = 0; i < QPalette::NColorRoles; i++)
                actcg << editPalette.color(QPalette::Active,
                                           QPalette::ColorRole(i)).name();
            for (i = 0; i < QPalette::NColorRoles; i++)
                inactcg << editPalette.color(QPalette::Inactive,
                                             QPalette::ColorRole(i)).name();
            for (i = 0; i < QPalette::NColorRoles; i++)
                discg << editPalette.color(QPalette::Disabled,
                                           QPalette::ColorRole(i)).name();
        }

        settings.setValue(QLatin1String("Qt/font"), font.toString());
        settings.setValue(QLatin1String("Qt/Palette/active"), actcg);
        settings.setValue(QLatin1String("Qt/Palette/inactive"), inactcg);
        settings.setValue(QLatin1String("Qt/Palette/disabled"), discg);

        settings.setValue(QLatin1String("Qt/embedFonts"), ui->fontEmbeddingCheckBox->isChecked());
        settings.setValue(QLatin1String("Qt/style"),
                          overrideDesktopSettings ? ui->guiStyleCombo->currentText() : QString());

        settings.setValue(QLatin1String("Qt/doubleClickInterval"), ui->doubleClickIntervalSpinBox->value());
        settings.setValue(QLatin1String("Qt/cursorFlashTime"),
                          ui->cursorFlashTimeSpinBox->value() == 9 ? 0 : ui->cursorFlashTimeSpinBox->value());
        settings.setValue(QLatin1String("Qt/wheelScrollLines"), ui->wheelScrollLinesSpinBox->value());
        settings.setValue(QLatin1String("Qt/resolveSymlinks"), ui->resolveLinksCheckBox->isChecked());

        QSize strut(ui->strutWidthSpinBox->value(), ui->strutHeightSpinBox->value());
        settings.setValue(QLatin1String("Qt/globalStrut/width"), strut.width());
        settings.setValue(QLatin1String("Qt/globalStrut/height"), strut.height());

        settings.setValue(QLatin1String("Qt/useRtlExtensions"), ui->rtlExtensionsCheckBox->isChecked());

        QStringList effects;
        if (ui->effectsCheckBox->isChecked()) {
            effects << QLatin1String("general");

            switch (ui->menuEffectCombo->currentIndex()) {
            case 1: effects << QLatin1String("animatemenu"); break;
            case 2: effects << QLatin1String("fademenu"); break;
            }

            switch (ui->comboEffectCombo->currentIndex()) {
            case 1: effects << QLatin1String("animatecombo"); break;
            }

            switch (ui->toolTipEffectCombo->currentIndex()) {
            case 1: effects << QLatin1String("animatetooltip"); break;
            case 2: effects << QLatin1String("fadetooltip"); break;
            }

            switch (ui->toolBoxEffectCombo->currentIndex()) {
            case 1: effects << QLatin1String("animatetoolbox"); break;
            }
        } else
            effects << QLatin1String("none");
        settings.setValue(QLatin1String("Qt/GUIEffects"), effects);
    }

#if defined(Q_WS_X11)
    qt_x11_apply_settings_in_all_apps();
#endif // Q_WS_X11

    setModified(false);
    statusBar()->showMessage(tr("Saved changes."));
}

void MainWindow::fileExit()
{
    qApp->closeAllWindows();
}

void MainWindow::setModified(bool m)
{
    if (modified == m)
        return;

    modified = m;
    ui->fileSaveAction->setEnabled(m);
}

void MainWindow::buildPalette()
{
    QPalette temp(ui->buttonMainColor->color(), ui->buttonWindowColor->color());
    for (int i = 0; i < QPalette::NColorGroups; i++)
        temp = PaletteEditorAdvanced::buildEffect(QPalette::ColorGroup(i), temp);

    editPalette = temp;
    setPreviewPalette(editPalette);
    updateColorButtons();

    setModified(true);
}

void MainWindow::setPreviewPalette(const QPalette &pal)
{
    QPalette::ColorGroup colorGroup = groupFromIndex(ui->paletteCombo->currentIndex());

    for (int i = 0; i < QPalette::NColorGroups; i++) {
        for (int j = 0; j < QPalette::NColorRoles; j++) {
            QPalette::ColorGroup targetGroup = QPalette::ColorGroup(i);
            QPalette::ColorRole targetRole = QPalette::ColorRole(j);
            previewPalette.setColor(targetGroup, targetRole, pal.color(colorGroup, targetRole));
        }
    }

    ui->previewFrame->setPreviewPalette(previewPalette);
}

void MainWindow::updateColorButtons()
{
    ui->buttonMainColor->setColor(editPalette.color(QPalette::Active, QPalette::Button));
    ui->buttonWindowColor->setColor(editPalette.color(QPalette::Active, QPalette::Window));
}

void MainWindow::tunePalette()
{
    bool ok;
    QPalette pal = PaletteEditorAdvanced::getPalette(&ok, editPalette,
                                                     backgroundRole(), this);
    if (!ok)
        return;

    editPalette = pal;
    setPreviewPalette(editPalette);
    setModified(true);
}

void MainWindow::paletteSelected(int)
{
    setPreviewPalette(editPalette);
}

void MainWindow::updateStyleLayout()
{
    QString currentStyle = ui->guiStyleCombo->currentText();
    bool autoStyle = (currentStyle == desktopThemeName);
    ui->previewFrame->setPreviewVisible(!autoStyle);
    ui->buildPaletteGroup->setEnabled(!autoStyle);
}

void MainWindow::styleSelected(const QString &stylename)
{
    if (stylename == desktopThemeName) {
        setModified(true);
    } else {
        QStyle *style = QStyleFactory::create(stylename);
        if (!style)
            return;
        setStyleHelper(ui->previewFrame, style);
        delete previewstyle;
        previewstyle = style;
        setModified(true);
    }
    updateStyleLayout();
}

void MainWindow::familySelected(const QString &family)
{
    QFontDatabase db;
    QStringList styles = db.styles(family);
    ui->fontStyleCombo->clear();
    ui->fontStyleCombo->addItems(styles);
    buildFont();
}

void MainWindow::buildFont()
{
    QFontDatabase db;
    QFont font = db.font(ui->fontFamilyCombo->currentText(),
                         ui->fontStyleCombo->currentText(),
                         ui->pointSizeCombo->currentText().toInt());
    ui->sampleLineEdit->setFont(font);
    setModified(true);
}

void MainWindow::somethingModified()
{
    setModified(true);
}

void MainWindow::helpAbout()
{
    QMessageBox box(this);
    box.setText(tr("<h3>%1</h3>"
                   "<br/>Version %2"
                   "<br/><br/>Copyright (C) 2015 The Qt Company Ltd."
                   "<br/><br/>Copyright (C) 2016 Ivailo Monev"
                    )
                   .arg(tr("Katie Configuration")).arg(QLatin1String(QT_VERSION_STR)));
    box.setWindowTitle(tr("Katie Configuration"));
    box.setIcon(QMessageBox::NoIcon);
    box.exec();
}

void MainWindow::helpAboutQt()
{
    QMessageBox::aboutQt(this, tr("Katie Configuration"));
}

void MainWindow::pageChanged(int pageNumber)
{
    QWidget *page = ui->mainTabWidget->widget(pageNumber);
    if (page == ui->interfaceTab)
        ui->helpView->setText(tr(interface_text));
    else if (page == ui->appearanceTab)
        ui->helpView->setText(tr(appearance_text));
    else if (page == ui->fontsTab)
        ui->helpView->setText(tr(font_text));
}

void MainWindow::effectsToggled(bool toggled)
{
    ui->effectsFrame->setEnabled(toggled);
    setModified(true);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (modified) {
        switch (QMessageBox::warning(this, tr("Save Changes"),
                                     tr("Save changes to settings?"),
                                     (QMessageBox::Yes | QMessageBox::No
                                     | QMessageBox::Cancel))) {
        case QMessageBox::Yes: // save
            qApp->processEvents();
            fileSave();

            // fall through intended
        case QMessageBox::No: // don't save
            e->accept();
            break;

        case QMessageBox::Cancel: // cancel
            e->ignore();
            break;

        default: break;
        }
    } else
        e->accept();
}

#include "moc_mainwindow.h"

QT_END_NAMESPACE
