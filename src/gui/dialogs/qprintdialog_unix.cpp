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

#include "qplatformdefs.h"

#ifndef QT_NO_PRINTDIALOG

#include "qabstractprintdialog_p.h"
#include "qprintdialog.h"
#include "qprintdialog_p.h"
#include "qfiledialog.h"
#include "qcompleter.h"
#include <QtCore/qdir.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qevent.h>
#include <QtGui/qfilesystemmodel.h>
#include <QtGui/qstyleditemdelegate.h>
#include <QtGui/qprinter.h>
#include "qprinterinfo_unix_p.h"
#include "qcore_unix_p.h"

#include "ui_qprintpropertieswidget.h"
#include "ui_qprintsettingsoutput.h"
#include "ui_qprintwidget.h"

#if !defined(QT_NO_CUPS)
#  include "qcups_p.h"
#  include <cups/cups.h>
#  include "qpdf_p.h"
#endif

QT_BEGIN_NAMESPACE

static const uchar* qrc_status_color()
{
    static const uchar static_qrc_data[] = {
        0x89, 0x50, 0x4e, 0x47, 0xd, 0xa, 0x1a, 0xa, 0x0, 0x0, 0x0, 0xd, 0x49, 0x48, 0x44, 0x52, 0x0, 0x0, 0x0, 0x20,
        0x0, 0x0, 0x0, 0x20, 0x8, 0x6, 0x0, 0x0, 0x0, 0x73, 0x7a, 0x7a, 0xf4, 0x0, 0x0, 0x5, 0x8a, 0x49, 0x44, 0x41,
        0x54, 0x58, 0xc3, 0xc5, 0x97, 0x59, 0x6f, 0x13, 0x67, 0x14, 0x86, 0xd3, 0xfe, 0x11, 0x7e, 0x45, 0x7b, 0x9, 0xb7, 0x24,
        0x71, 0xda, 0x5f, 0x40, 0x55, 0x15, 0xda, 0x82, 0x12, 0x28, 0x69, 0x91, 0x10, 0x15, 0x20, 0x55, 0xbd, 0x40, 0x2d, 0x82,
        0x26, 0x40, 0xc3, 0x5, 0x4, 0x35, 0x2a, 0x6a, 0x55, 0x54, 0xba, 0x92, 0xb0, 0x84, 0x90, 0x44, 0x25, 0x24, 0x36, 0x21,
        0x4, 0x67, 0xb7, 0x93, 0xd8, 0xf1, 0x36, 0xde, 0x97, 0x19, 0x8f, 0x3d, 0x9e, 0xf1, 0xcc, 0xdb, 0x73, 0x3e, 0xc7, 0xc6,
        0xd8, 0xc1, 0x76, 0x2e, 0x10, 0x91, 0x5e, 0x79, 0x89, 0x3d, 0xcf, 0x7b, 0xd6, 0xf9, 0xdc, 0x6, 0xa0, 0xed, 0x6d, 0xea,
        0xd5, 0x17, 0x6d, 0x6d, 0x7b, 0x48, 0xfb, 0xde, 0xb0, 0xf6, 0x34, 0x32, 0xb0, 0xb7, 0x50, 0x28, 0x20, 0x93, 0x91, 0x91,
        0x91, 0x65, 0xc8, 0xb2, 0x2, 0x59, 0x51, 0xa0, 0x28, 0x59, 0x28, 0xd9, 0x2c, 0xb2, 0x2c, 0x55, 0x85, 0x5a, 0x56, 0x2e,
        0x87, 0x5c, 0xb5, 0xf2, 0xf9, 0x8a, 0xf2, 0x3b, 0xc8, 0x30, 0xc, 0x86, 0xec, 0x6d, 0x64, 0x60, 0x1f, 0xc3, 0x3, 0xc1,
        0x10, 0x42, 0x92, 0x4, 0x29, 0x1c, 0x46, 0x38, 0x12, 0x41, 0x24, 0x1a, 0x45, 0x34, 0x16, 0x43, 0x2c, 0x1e, 0x47, 0x9c,
        0x95, 0x48, 0x20, 0x91, 0x4c, 0xa, 0x25, 0x53, 0x29, 0xa1, 0x14, 0x2b, 0x9d, 0x16, 0x4a, 0xb3, 0x32, 0x99, 0x3a, 0xe5,
        0x35, 0x8d, 0x21, 0xfb, 0x1a, 0x1a, 0x90, 0x29, 0xf2, 0x97, 0xf0, 0x28, 0xc1, 0x4b, 0xe0, 0xd8, 0xe, 0xe0, 0x32, 0xb4,
        0x1a, 0x98, 0x61, 0xc9, 0x72, 0x45, 0x72, 0x95, 0xb4, 0x96, 0xc, 0x50, 0xca, 0xa5, 0x70, 0x4d, 0xd4, 0xc, 0xde, 0x86,
        0x97, 0xc1, 0x99, 0x95, 0x15, 0x68, 0xdd, 0xdd, 0x42, 0xf2, 0xea, 0xea, 0x4b, 0x98, 0x52, 0x2e, 0x5b, 0xbd, 0xb8, 0xbc,
        0x4d, 0xd, 0x70, 0xbd, 0xeb, 0xe0, 0x4, 0x8e, 0x3c, 0xf3, 0x61, 0xe3, 0xc4, 0x7f, 0xf0, 0x7e, 0xe7, 0x40, 0x32, 0x96,
        0x42, 0xee, 0xca, 0x15, 0x18, 0xfb, 0xf7, 0xb, 0xe5, 0x7, 0x6, 0xa0, 0x24, 0x63, 0xd0, 0xee, 0x5f, 0x80, 0x7e, 0xfb,
        0x14, 0x54, 0xff, 0x4a, 0xa5, 0x5f, 0xaa, 0x55, 0xd0, 0xf5, 0xe6, 0x6, 0xf8, 0x8b, 0xb5, 0x70, 0x8e, 0x7a, 0xf3, 0xd4,
        0x14, 0x56, 0xf6, 0xff, 0x2b, 0x14, 0xbe, 0xe3, 0x86, 0xfa, 0xe8, 0x11, 0x8c, 0xf6, 0x76, 0xa1, 0xdc, 0xf8, 0x38, 0xf2,
        0xce, 0xfb, 0x28, 0xf6, 0xb7, 0xb, 0xe9, 0x7f, 0x9e, 0xae, 0x6f, 0x50, 0x92, 0xde, 0x92, 0x1, 0x72, 0x5a, 0xb, 0xe7,
        0x3a, 0xfb, 0x7e, 0x7c, 0x5e, 0x31, 0x90, 0x70, 0x4a, 0x22, 0xa5, 0xaa, 0xc7, 0x3, 0xd5, 0xeb, 0x15, 0x13, 0x91, 0xf7,
        0xcc, 0xa3, 0x78, 0xc9, 0x56, 0x32, 0x30, 0x79, 0xad, 0xd1, 0x14, 0x34, 0x36, 0xc0, 0x17, 0x2b, 0xc3, 0x37, 0x17, 0x82,
        0xf8, 0x9d, 0x22, 0x1f, 0xbb, 0x3a, 0x4f, 0x8d, 0x96, 0x41, 0x74, 0xd2, 0x8b, 0xe4, 0x92, 0x24, 0x46, 0x52, 0x9e, 0x76,
        0x23, 0xbb, 0x15, 0x41, 0x3e, 0xe4, 0x83, 0xe6, 0x9c, 0x11, 0x0, 0xcd, 0xbf, 0x8c, 0x82, 0x6b, 0x4a, 0x34, 0x9b, 0xb1,
        0x74, 0x1d, 0x86, 0xfd, 0xc, 0xf4, 0xe4, 0xba, 0xa8, 0x3d, 0xab, 0x58, 0x2c, 0xb6, 0x60, 0x80, 0x52, 0x55, 0x8e, 0xfc,
        0xd6, 0xc9, 0xc7, 0xe8, 0xa3, 0x88, 0x59, 0xcb, 0x4, 0xe7, 0xa8, 0x93, 0xfe, 0x24, 0xb2, 0x9, 0x32, 0xd3, 0x71, 0x9,
        0xa9, 0xef, 0xef, 0xc2, 0x18, 0x3a, 0xf, 0xf3, 0x8b, 0x4e, 0x68, 0x72, 0x1a, 0x5a, 0x5a, 0x12, 0x20, 0x3d, 0xf8, 0x4,
        0xd6, 0x48, 0xa7, 0x90, 0xe9, 0x38, 0x23, 0x52, 0xcf, 0x6a, 0xc9, 0x0, 0xd7, 0xaa, 0x9c, 0xf6, 0x91, 0xf3, 0xb3, 0x2,
        0xde, 0xdf, 0x39, 0xc, 0xef, 0x82, 0x24, 0xe0, 0xc3, 0xf4, 0x7c, 0xf1, 0xc6, 0x22, 0x54, 0xfb, 0x6, 0x72, 0xfe, 0x18,
        0xa, 0xe1, 0x0, 0xf4, 0x45, 0x7, 0x8c, 0xb9, 0x9f, 0x60, 0xde, 0xb4, 0x41, 0xcf, 0x84, 0x61, 0x24, 0xd6, 0x60, 0xde,
        0xed, 0x12, 0x6, 0x8a, 0x2f, 0xfa, 0x76, 0x69, 0x80, 0x52, 0x99, 0xda, 0x9e, 0xeb, 0x78, 0x34, 0x85, 0xf9, 0x7b, 0x1b,
        0xd8, 0x78, 0x1e, 0x12, 0x5d, 0x9d, 0x4a, 0xc8, 0x2, 0x1e, 0x5d, 0x8e, 0x8a, 0x34, 0x97, 0x53, 0xcb, 0x17, 0x37, 0x62,
        0x6b, 0x28, 0x3e, 0x1f, 0x82, 0xae, 0xc9, 0x25, 0x58, 0x7c, 0x9, 0x45, 0x3f, 0x35, 0xaa, 0xa6, 0x88, 0xda, 0xb3, 0x4c,
        0xd3, 0x6c, 0x6e, 0x80, 0x6b, 0xc9, 0x8b, 0x65, 0x63, 0x45, 0xc2, 0x6f, 0xd4, 0x78, 0x8e, 0x71, 0xaf, 0x80, 0x2f, 0x4c,
        0x7, 0x70, 0xb2, 0x63, 0x18, 0x4b, 0xf6, 0x50, 0x5, 0x5e, 0x8e, 0xac, 0x12, 0x61, 0x72, 0x16, 0xd6, 0x8c, 0x4d, 0x3c,
        0x32, 0xb0, 0x98, 0x99, 0x82, 0x19, 0xb9, 0x6, 0x43, 0xf5, 0xee, 0xce, 0x0, 0x6f, 0xb3, 0x73, 0xdd, 0x13, 0xe8, 0xa1,
        0xf4, 0x1f, 0x23, 0x68, 0xc0, 0x9b, 0x40, 0xd8, 0x97, 0xc2, 0xaf, 0xe7, 0xe7, 0x10, 0xd, 0x64, 0xea, 0xe0, 0xe5, 0x8,
        0xd, 0x35, 0x4, 0x73, 0xfd, 0x2, 0x8a, 0x39, 0x9, 0x45, 0x2d, 0x4, 0xcb, 0x65, 0x23, 0x51, 0x2f, 0xf8, 0x8e, 0x8b,
        0xf4, 0xb7, 0x66, 0x80, 0xa2, 0xe3, 0xad, 0x76, 0xae, 0x67, 0x12, 0x47, 0xdb, 0xef, 0x8, 0x3, 0xc1, 0xad, 0x24, 0x82,
        0x41, 0x19, 0x3, 0x3, 0x2e, 0x84, 0x42, 0xca, 0xce, 0x70, 0x8e, 0x58, 0x97, 0x60, 0xca, 0x17, 0xc4, 0x63, 0xad, 0x1,
        0x86, 0xb7, 0x64, 0x80, 0xd3, 0xcb, 0xab, 0xd4, 0xe3, 0x8a, 0xe2, 0xd6, 0xd5, 0x17, 0x98, 0x9d, 0xf4, 0x89, 0x11, 0x9b,
        0x9b, 0x8b, 0xe1, 0xc0, 0x81, 0x69, 0xcc, 0xcf, 0x27, 0xea, 0xc1, 0x14, 0x9d, 0x88, 0xb0, 0xf0, 0xc, 0x56, 0xdc, 0x26,
        0x1e, 0x5, 0x50, 0xa6, 0x69, 0x88, 0x5e, 0x87, 0x99, 0xf7, 0x89, 0xd7, 0x96, 0x65, 0xb5, 0x66, 0x80, 0xc7, 0x8d, 0xeb,
        0x9e, 0x48, 0x2a, 0x78, 0x30, 0x93, 0x80, 0xd3, 0x55, 0x4a, 0xbb, 0xa2, 0x68, 0x18, 0x7a, 0xe2, 0xc5, 0x9a, 0x94, 0xa9,
        0x83, 0xab, 0x45, 0x17, 0x62, 0xd6, 0xcf, 0x4, 0x52, 0xb7, 0xa3, 0x5d, 0x26, 0xe0, 0x4, 0x29, 0x27, 0xc0, 0x65, 0x35,
        0x37, 0xc0, 0x20, 0x82, 0xf3, 0x42, 0xba, 0x78, 0x33, 0x80, 0x8e, 0x63, 0x6b, 0xe8, 0x3a, 0xee, 0xc2, 0xaa, 0x47, 0x41,
        0x38, 0x95, 0x85, 0xed, 0xf2, 0x8c, 0x30, 0x61, 0x8f, 0x6f, 0x20, 0x9c, 0x4b, 0x23, 0x5e, 0xc, 0x63, 0xd1, 0x74, 0x8,
        0xb8, 0xdb, 0xea, 0x42, 0xc1, 0x8c, 0x91, 0x21, 0x5e, 0x3e, 0x1f, 0x92, 0x6c, 0x64, 0xf2, 0xd2, 0xee, 0xc, 0x70, 0xa4,
        0xd9, 0x6d, 0x3, 0x5f, 0x5f, 0xd9, 0x12, 0x6, 0x58, 0xd3, 0xce, 0xb4, 0x48, 0x7d, 0x24, 0xad, 0x42, 0x29, 0xe4, 0x61,
        0x9b, 0xb9, 0x8c, 0x1f, 0xd6, 0x47, 0xf1, 0x8b, 0xd5, 0x87, 0xe3, 0x96, 0xd, 0x1a, 0x45, 0xae, 0x5b, 0x71, 0x1, 0xd1,
        0x75, 0x7, 0x65, 0xcb, 0x26, 0x94, 0xcb, 0x9d, 0xad, 0xd4, 0xdf, 0x6c, 0xd9, 0x0, 0xc1, 0x79, 0x21, 0xb9, 0xbc, 0x32,
        0xce, 0xc, 0xf8, 0x30, 0xf8, 0x57, 0x44, 0xc0, 0x67, 0xe8, 0x9c, 0xe0, 0xa2, 0x2d, 0xc9, 0x29, 0x77, 0x24, 0x36, 0x45,
        0x6, 0x12, 0x66, 0x44, 0x64, 0x80, 0xc1, 0x19, 0xcb, 0x3, 0xc9, 0x70, 0xa0, 0x68, 0x1a, 0x74, 0x6b, 0x1e, 0xa4, 0x71,
        0x3e, 0x4b, 0xd7, 0x71, 0x57, 0xee, 0x5, 0x5a, 0x41, 0x6f, 0x7e, 0x22, 0x62, 0x3, 0xea, 0xb6, 0x81, 0xea, 0x79, 0x1f,
        0x5c, 0x5e, 0x46, 0xe7, 0xc8, 0x8, 0x3e, 0xb8, 0x77, 0xf, 0x4b, 0x74, 0xb3, 0x62, 0x13, 0x52, 0x41, 0x12, 0x62, 0x78,
        0xda, 0x72, 0xe3, 0x6f, 0xd5, 0x86, 0x5b, 0xd1, 0x4e, 0x3c, 0x8d, 0x5f, 0x2b, 0x2d, 0x2e, 0xda, 0xa8, 0x7c, 0x82, 0x8a,
        0xf0, 0xa9, 0x8a, 0xe4, 0xf1, 0x7, 0x15, 0x62, 0xbc, 0xdf, 0xdc, 0x0, 0xc1, 0x85, 0xe3, 0x2a, 0x3, 0x67, 0x1d, 0xe,
        0x61, 0x80, 0x35, 0x1e, 0x8, 0xe0, 0x9, 0x75, 0xb8, 0x8d, 0xc6, 0xac, 0xcb, 0xdd, 0x85, 0x69, 0x65, 0x1a, 0x3e, 0x7d,
        0x4c, 0xc0, 0x6f, 0x7a, 0x3b, 0x30, 0xec, 0x3e, 0x45, 0x63, 0x1b, 0xc4, 0xe2, 0xd2, 0x12, 0xec, 0x76, 0x7, 0xec, 0xf4,
        0xdd, 0x15, 0xf7, 0xa6, 0xfe, 0xf1, 0xa7, 0x7, 0xbf, 0x25, 0xc6, 0xbb, 0x8d, 0xd, 0x10, 0x2c, 0x57, 0x65, 0xa0, 0x3c,
        0xef, 0x1b, 0x14, 0xcd, 0x69, 0xbb, 0x1d, 0xfd, 0x4e, 0x27, 0xb2, 0x64, 0xea, 0x3a, 0x8d, 0x57, 0x27, 0xcd, 0xb8, 0xcd,
        0x6d, 0x13, 0xcf, 0xb, 0x46, 0xe, 0x53, 0x91, 0x8b, 0xf8, 0xc7, 0x75, 0x2, 0x33, 0xb, 0xf7, 0xf1, 0x70, 0x6c, 0xc,
        0x8f, 0x1f, 0x4f, 0xc1, 0xed, 0x5e, 0x87, 0xc7, 0xe3, 0xb5, 0xe, 0x1d, 0x3e, 0xd2, 0x5f, 0xb, 0xdf, 0xd1, 0x80, 0x5e,
        0x65, 0xa0, 0x7a, 0xe3, 0xd5, 0x8e, 0x9d, 0x8f, 0x66, 0xbb, 0xd7, 0xdf, 0x2b, 0xe4, 0xd7, 0xfc, 0xe2, 0x33, 0xbc, 0x41,
        0x3d, 0x74, 0x46, 0x78, 0xf0, 0x60, 0x54, 0x34, 0x9d, 0xae, 0x17, 0xb0, 0xb5, 0xb5, 0x85, 0xcf, 0x8f, 0xf6, 0xdc, 0xd8,
        0x9, 0xde, 0xd0, 0x40, 0xed, 0xbe, 0x7f, 0x65, 0xe1, 0x6c, 0x77, 0x75, 0x79, 0xb4, 0xf8, 0x3d, 0x36, 0xcc, 0xf5, 0x7e,
        0xfa, 0x74, 0x16, 0x13, 0x13, 0x93, 0x15, 0xf8, 0xd1, 0x2f, 0x7b, 0xff, 0xa0, 0xeb, 0xbe, 0xd3, 0xea, 0xf, 0x93, 0x92,
        0x81, 0x9a, 0xfa, 0x37, 0x33, 0xc0, 0xff, 0xe3, 0xe5, 0xc5, 0xd1, 0x8f, 0xd1, 0x51, 0x6d, 0x65, 0x75, 0xad, 0x4, 0xff,
        0xaa, 0xf7, 0x76, 0x23, 0x78, 0xcb, 0x6, 0xea, 0x56, 0xee, 0xe, 0x6, 0x38, 0xfd, 0xab, 0x74, 0x3a, 0x1e, 0x1d, 0x7d,
        0x88, 0x4d, 0x8f, 0xc7, 0xfa, 0xac, 0xa7, 0x7b, 0xb0, 0x19, 0xfc, 0xb5, 0x6, 0xf2, 0xbb, 0x30, 0xc0, 0x7f, 0xfc, 0x19,
        0xbe, 0x85, 0x73, 0xd7, 0xbf, 0x58, 0x5e, 0x33, 0x3e, 0x39, 0x7c, 0xa4, 0xaf, 0x15, 0xf8, 0x8e, 0x3f, 0xcd, 0x44, 0xf3,
        0xbc, 0x6, 0x58, 0x1b, 0x79, 0x25, 0x3, 0xf4, 0x99, 0xb4, 0x9c, 0x85, 0x73, 0x75, 0x5d, 0xf9, 0xe8, 0xd0, 0xc1, 0x6f,
        0x5a, 0x85, 0xbf, 0x89, 0x1f, 0xa7, 0xef, 0xed, 0x6, 0x5e, 0x67, 0xe0, 0x6d, 0xe8, 0x7f, 0x90, 0x59, 0x7c, 0x75, 0x27,
        0x34, 0x83, 0x2, 0x0, 0x0, 0x0, 0x0, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
    };
    return static_qrc_data;
};
static const qint64 qrc_status_color_size = 1475;

static const uchar* qrc_status_gray_scale()
{
    static const uchar static_qrc_data[] = {
        0x89, 0x50, 0x4e, 0x47, 0xd, 0xa, 0x1a, 0xa, 0x0, 0x0, 0x0, 0xd, 0x49, 0x48, 0x44, 0x52, 0x0, 0x0, 0x0, 0x20,
        0x0, 0x0, 0x0, 0x20, 0x8, 0x3, 0x0, 0x0, 0x0, 0x44, 0xa4, 0x8a, 0xc6, 0x0, 0x0, 0x2, 0x25, 0x50, 0x4c, 0x54,
        0x45, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xdf,
        0xe3, 0xe5, 0x0, 0x0, 0x0, 0xd7, 0xdb, 0xde, 0x60, 0x65, 0x63, 0xdf, 0xdf, 0xe0, 0x64, 0x68, 0x69, 0x67, 0x6d, 0x6c,
        0x6d, 0x72, 0x71, 0x6d, 0x73, 0x71, 0xde, 0xdf, 0xdf, 0x66, 0x6c, 0x6b, 0xd1, 0xd6, 0xd9, 0x62, 0x68, 0x69, 0xd2, 0xd8,
        0xdc, 0x5e, 0x64, 0x63, 0x0, 0x0, 0x0, 0xfa, 0xfa, 0xfa, 0xf9, 0xf9, 0xf9, 0xfb, 0xfb, 0xfb, 0xfd, 0xfd, 0xfd, 0xf1,
        0xf2, 0xf2, 0xfc, 0xfc, 0xfc, 0xf7, 0xf7, 0xf7, 0xfe, 0xfe, 0xfe, 0xf6, 0xf6, 0xf6, 0xf8, 0xf8, 0xf8, 0xf3, 0xf3, 0xf3,
        0xef, 0xef, 0xef, 0xf4, 0xf4, 0xf4, 0xf5, 0xf5, 0xf5, 0xec, 0xec, 0xec, 0xf1, 0xf1, 0xf1, 0xee, 0xee, 0xee, 0xf0, 0xf0,
        0xf0, 0xe1, 0xe1, 0xe1, 0xd8, 0xd8, 0xd8, 0x90, 0x90, 0x90, 0x6f, 0x6f, 0x6f, 0xd5, 0xd5, 0xd5, 0xe4, 0xe4, 0xe4, 0xf4,
        0xf5, 0xf5, 0xf6, 0xf7, 0xf7, 0xef, 0xf0, 0xf0, 0xf2, 0xf3, 0xf3, 0xe5, 0xe5, 0xe5, 0xbd, 0xbd, 0xbd, 0xf2, 0xf2, 0xf2,
        0xa2, 0xa2, 0xa2, 0xe3, 0xe3, 0xe3, 0xe9, 0xe9, 0xe9, 0xe6, 0xe6, 0xe6, 0xed, 0xee, 0xee, 0xec, 0xed, 0xed, 0xea, 0xea,
        0xea, 0xee, 0xef, 0xef, 0xdf, 0xdf, 0xdf, 0xdd, 0xdd, 0xdd, 0xb5, 0xb5, 0xb5, 0xb4, 0xb4, 0xb4, 0xd3, 0xd3, 0xd3, 0xdb,
        0xdb, 0xdb, 0xf5, 0xf6, 0xf6, 0xa0, 0xa0, 0xa0, 0xd6, 0xd6, 0xd6, 0xb1, 0xb2, 0xb2, 0xb3, 0xb3, 0xb3, 0xf1, 0xf1, 0xf2,
        0xc3, 0xc3, 0xc3, 0xe6, 0xe7, 0xe7, 0xf9, 0xfa, 0xfa, 0xe7, 0xe8, 0xe8, 0x9b, 0x9b, 0x9b, 0xe2, 0xe2, 0xe2, 0xf3, 0xf3,
        0xf4, 0x87, 0x87, 0x87, 0xde, 0xde, 0xde, 0xa1, 0xa1, 0xa1, 0xcd, 0xcd, 0xcd, 0xd9, 0xd9, 0xd9, 0xcf, 0xcf, 0xcf, 0x96,
        0x96, 0x96, 0xca, 0xca, 0xca, 0xcc, 0xcc, 0xcc, 0xbc, 0xbc, 0xbc, 0xce, 0xce, 0xce, 0xa7, 0xa7, 0xa7, 0xd4, 0xd5, 0xd5,
        0xc5, 0xc5, 0xc5, 0xfa, 0xfa, 0xf9, 0xe7, 0xe7, 0xe7, 0xdc, 0xdc, 0xdc, 0xc7, 0xc7, 0xc7, 0x8a, 0x8a, 0x8a, 0x89, 0x89,
        0x89, 0xf7, 0xf8, 0xf8, 0xe8, 0xe8, 0xe8, 0xeb, 0xec, 0xec, 0xf3, 0xf4, 0xf4, 0xed, 0xed, 0xed, 0xa3, 0xa3, 0xa3, 0xc6,
        0xc6, 0xc6, 0x58, 0x58, 0x58, 0xdf, 0xe0, 0xe0, 0x86, 0x86, 0x86, 0xea, 0xeb, 0xeb, 0xb2, 0xb2, 0xb2, 0xa5, 0xa5, 0xa5,
        0xbe, 0xbe, 0xbe, 0xf0, 0xf1, 0xf1, 0xc4, 0xc4, 0xc4, 0x6e, 0x6e, 0x6e, 0xda, 0xda, 0xda, 0xe0, 0xe0, 0xe0, 0xcc, 0xcd,
        0xcd, 0xf2, 0xf2, 0xf3, 0x94, 0x94, 0x94, 0xac, 0xac, 0xac, 0xd1, 0xd1, 0xd1, 0xa3, 0xa4, 0xa4, 0xa4, 0xa4, 0xa4, 0x6c,
        0x6c, 0x6c, 0xcb, 0xcb, 0xcb, 0x93, 0x93, 0x93, 0xbb, 0xbb, 0xbb, 0xf8, 0xf9, 0xf9, 0xe7, 0xe8, 0xe9, 0xb9, 0xb9, 0xb9,
        0xfd, 0xfe, 0xfe, 0xe8, 0xe9, 0xe9, 0xe4, 0xe5, 0xe5, 0xb7, 0xb7, 0xb7, 0xf8, 0xf9, 0xfa, 0xb0, 0xb0, 0xb0, 0x71, 0x71,
        0x71, 0xcd, 0xce, 0xce, 0xe9, 0xea, 0xea, 0xe3, 0xe4, 0xe4, 0x97, 0x97, 0x97, 0xa9, 0xa9, 0xa9, 0x8b, 0x8b, 0x8b, 0x95,
        0x95, 0x95, 0xab, 0xab, 0xab, 0x84, 0x84, 0x84, 0xc9, 0xc9, 0xca, 0xc9, 0xca, 0xca, 0xa8, 0xa8, 0xa8, 0xc2, 0xc2, 0xc2,
        0x87, 0x88, 0x88, 0x6d, 0x6d, 0x6d, 0xbc, 0xbd, 0xbd, 0xc4, 0xc4, 0xc5, 0xdb, 0xdb, 0xdc, 0xe9, 0xea, 0xeb, 0x92, 0x92,
        0x92, 0xeb, 0xeb, 0xeb, 0xba, 0xba, 0xbb, 0xb8, 0xb8, 0xb8, 0xe8, 0xe9, 0xea, 0xcb, 0xcb, 0xcc, 0xc0, 0xc0, 0xc1, 0xe4,
        0xe5, 0xe6, 0x5b, 0x5b, 0x5b, 0xbd, 0xbe, 0xbe, 0xd7, 0xd8, 0xd9, 0xb0, 0xb1, 0xb1, 0xbb, 0xbb, 0xbc, 0xaa, 0xab, 0xab,
        0xdb, 0xdc, 0xdc, 0xff, 0xff, 0xff, 0x59, 0x59, 0x59, 0xd2, 0xd2, 0xd2, 0xfa, 0xfb, 0xfb, 0xfb, 0xfc, 0xfc, 0xf0, 0xf2,
        0xf4, 0xdd, 0xde, 0xde, 0xbf, 0xc0, 0xc0, 0xc6, 0xc7, 0xc7, 0x26, 0x29, 0x10, 0x54, 0x0, 0x0, 0x0, 0x16, 0x74, 0x52,
        0x4e, 0x53, 0x0, 0x3f, 0x1, 0x3e, 0x2, 0x1f, 0xf3, 0x36, 0xfa, 0x83, 0xfe, 0x91, 0x9e, 0xae, 0xad, 0xfe, 0x9d, 0xfb,
        0x90, 0xf3, 0x81, 0x35, 0xe0, 0x72, 0x21, 0x46, 0x0, 0x0, 0x2, 0x5a, 0x49, 0x44, 0x41, 0x54, 0x78, 0x5e, 0x8d, 0xd3,
        0x53, 0x73, 0x25, 0x41, 0x0, 0x86, 0xe1, 0x24, 0x67, 0xb3, 0x36, 0xba, 0x87, 0xc6, 0xb1, 0xed, 0x13, 0xdb, 0xb6, 0x6d,
        0xae, 0x6d, 0xdb, 0xb6, 0x8d, 0xdf, 0xb7, 0xd3, 0xa9, 0x64, 0xe6, 0xdc, 0xec, 0x56, 0xbe, 0xdb, 0xf7, 0xe9, 0xaa, 0xee,
        0x8b, 0xce, 0x58, 0xcd, 0xb2, 0x33, 0xff, 0xb1, 0xec, 0x65, 0x60, 0x80, 0xc5, 0xc4, 0x3e, 0x77, 0xd5, 0x90, 0xe0, 0x10,
        0xc5, 0x3c, 0x86, 0x61, 0x9c, 0x4e, 0x27, 0x85, 0x86, 0x19, 0x96, 0x41, 0x66, 0xf1, 0x9b, 0xd2, 0xf2, 0x33, 0x97, 0x27,
        0x7, 0xa5, 0x48, 0x38, 0x1c, 0x8b, 0xf1, 0x2e, 0x97, 0x71, 0x16, 0xad, 0x3f, 0x73, 0x5, 0xf8, 0x4a, 0xcb, 0x5f, 0x3e,
        0x19, 0x1c, 0x44, 0x99, 0x47, 0x51, 0x26, 0x8, 0xc2, 0xe7, 0xf3, 0xb1, 0x1a, 0x70, 0x9f, 0x54, 0x8f, 0x47, 0x86, 0xd5,
        0x7c, 0xa5, 0xb0, 0xf0, 0x2c, 0xe1, 0x73, 0xbb, 0x39, 0x34, 0xa8, 0x81, 0x2a, 0xd4, 0xc3, 0xb9, 0x77, 0x93, 0x91, 0xe,
        0xbf, 0xff, 0xf, 0xbf, 0xb7, 0x23, 0x28, 0x38, 0xd4, 0x95, 0x69, 0x40, 0x40, 0x3d, 0x76, 0xc0, 0x62, 0x69, 0xad, 0xf7,
        0xfb, 0x17, 0x6a, 0xa6, 0xa6, 0x46, 0xd1, 0x5d, 0x19, 0xa0, 0x1, 0x7, 0xea, 0xae, 0x16, 0x8b, 0x65, 0x9c, 0x8b, 0x4e,
        0x8a, 0xa1, 0xd4, 0xd4, 0x37, 0xa, 0xd, 0xd3, 0x80, 0x18, 0x9, 0xff, 0x9a, 0x7e, 0x31, 0x5b, 0x50, 0x39, 0x34, 0x2e,
        0xf1, 0x1e, 0x2a, 0x6a, 0x67, 0xcd, 0xad, 0x3c, 0x84, 0xa4, 0x6, 0xf2, 0xc2, 0xb1, 0x43, 0x13, 0x13, 0x39, 0x5c, 0x54,
        0x1e, 0xb9, 0x57, 0x97, 0x12, 0x9, 0xd8, 0xe5, 0xf5, 0x36, 0x0, 0xa0, 0x3, 0x26, 0xe6, 0xfa, 0x30, 0xd1, 0x17, 0x8f,
        0x3e, 0x2d, 0xa9, 0x54, 0xe4, 0xae, 0x5, 0x2f, 0xc7, 0x7b, 0xbd, 0xb6, 0x74, 0xe0, 0x74, 0xcd, 0x46, 0x2e, 0x7d, 0x16,
        0xf8, 0x12, 0x3b, 0xb, 0x21, 0xe0, 0x73, 0x59, 0xa0, 0x84, 0x58, 0xc, 0xc3, 0x35, 0x40, 0x19, 0x13, 0xd7, 0x9a, 0x84,
        0x83, 0xf3, 0x39, 0x2c, 0x4, 0xea, 0x94, 0x26, 0x5, 0xf3, 0xd8, 0xf8, 0x74, 0x20, 0x57, 0x1f, 0x99, 0xb7, 0x87, 0x6a,
        0x7, 0x50, 0xc7, 0x30, 0x31, 0xc1, 0x70, 0xc9, 0xd4, 0x1c, 0xa9, 0x83, 0x7e, 0xe2, 0xfe, 0x83, 0x79, 0x8f, 0x7d, 0xbf,
        0x67, 0xa9, 0x63, 0x82, 0x4d, 0x40, 0x0, 0xd7, 0x1, 0xeb, 0x1e, 0xff, 0xd4, 0x4c, 0xd5, 0x7d, 0x6d, 0x40, 0x99, 0x24,
        0xa5, 0x19, 0x9, 0xf, 0x15, 0xc8, 0x38, 0xad, 0x3, 0x4e, 0x50, 0x7e, 0xdb, 0x20, 0x57, 0xb4, 0x88, 0xba, 0x62, 0x6,
        0x78, 0x30, 0xe, 0x68, 0x5a, 0x7, 0x27, 0x86, 0xc4, 0xc6, 0xb1, 0xb7, 0xb9, 0x92, 0xb5, 0x28, 0x61, 0xe4, 0x2, 0xe6,
        0x24, 0x23, 0x1d, 0xf6, 0x8f, 0xa6, 0x3, 0x28, 0x88, 0xb7, 0xc6, 0xc6, 0x8e, 0x82, 0x61, 0xe6, 0xd8, 0xf1, 0x9e, 0x69,
        0x48, 0xd1, 0x81, 0xfc, 0xfc, 0x53, 0x38, 0x7e, 0x5a, 0x7, 0xe, 0xa6, 0xbb, 0x7a, 0x1a, 0xe4, 0x98, 0xc8, 0x1a, 0xa3,
        0x10, 0xa0, 0x95, 0x0, 0xd6, 0x73, 0x6e, 0x80, 0xa2, 0xce, 0x1b, 0x34, 0x20, 0x32, 0x2c, 0x4, 0x17, 0xac, 0x17, 0x13,
        0x24, 0xcf, 0xd3, 0xc3, 0xb5, 0x15, 0x73, 0x2, 0x2f, 0x5, 0x83, 0x6b, 0xd7, 0x69, 0x20, 0x8f, 0x52, 0xc1, 0x55, 0xab,
        0xb5, 0xb9, 0xbe, 0xf7, 0xfa, 0x42, 0x77, 0xc5, 0x8d, 0x9b, 0x9e, 0xf6, 0xdb, 0x77, 0xd6, 0x6f, 0x58, 0xb3, 0x2, 0xca,
        0x18, 0x8a, 0x5, 0x20, 0xd0, 0xd8, 0x26, 0xce, 0xf4, 0xf5, 0xce, 0xc0, 0x96, 0xb6, 0xd1, 0x87, 0x8f, 0x1e, 0x6f, 0xdc,
        0xa4, 0xf6, 0x65, 0x0, 0x18, 0xa, 0x2, 0x80, 0x9e, 0xf8, 0x6c, 0x64, 0x44, 0x2, 0x72, 0xfc, 0x39, 0xde, 0x69, 0xda,
        0x8c, 0xba, 0x6, 0x58, 0x4, 0x48, 0x12, 0xc7, 0x69, 0x9a, 0xa4, 0xa4, 0x57, 0xaf, 0x3b, 0x4d, 0x5b, 0xb2, 0x32, 0xd2,
        0x80, 0x33, 0xd, 0x60, 0x5c, 0xfc, 0xdd, 0x7b, 0xd3, 0x56, 0xad, 0x6b, 0x0, 0x5b, 0x1, 0xb2, 0xf9, 0xe3, 0xb6, 0xed,
        0xa8, 0xa7, 0x1, 0x4a, 0x7, 0x5f, 0x80, 0xb1, 0x7d, 0xc7, 0x4e, 0xd4, 0xf5, 0x19, 0xf0, 0xef, 0x4b, 0x51, 0xcd, 0xea,
        0x7e, 0xfc, 0xdc, 0xb5, 0x3b, 0xeb, 0xff, 0x9f, 0x77, 0xf, 0xea, 0xab, 0xde, 0x5f, 0x6c, 0x30, 0x96, 0x1a, 0x23, 0x17,
        0x2a, 0xe1, 0x0, 0x0, 0x0, 0x0, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
    };
    return static_qrc_data;
};
static const qint64 qrc_status_gray_scale_size = 1254;

class QOptionTreeItem;
class QPPDOptionsModel;
class QUnixPrintWidget;

class QPrintPropertiesDialog : public QDialog
{
    Q_OBJECT
public:
    QPrintPropertiesDialog(QAbstractPrintDialog *parent = nullptr);
    ~QPrintPropertiesDialog();

#if !defined(QT_NO_CUPS)
    void setCups(QCUPSSupport *cups) { m_cups = cups; }
    void addItemToOptions(QOptionTreeItem *parent, QList<const ppd_option_t*>& options, QList<const char*>& markedOptions) const;
#endif

    void selectPrinter();
    void selectPdfPsPrinter(const QPrinter *p);

    /// copy printer properties to the widget
    void applyPrinterProperties(QPrinter *p);
    void setupPrinter() const;

protected:
    void showEvent(QShowEvent* event);

private:
    Ui::QPrintPropertiesWidget widget;
    QDialogButtonBox *m_buttons;
#if !defined(QT_NO_CUPS)
    QCUPSSupport *m_cups;
    QPPDOptionsModel *m_cupsOptionsModel;
#endif
};

class QUnixPrintWidgetPrivate
{
public:
    QUnixPrintWidgetPrivate(QUnixPrintWidget *q);
    ~QUnixPrintWidgetPrivate();

    /// copy printer properties to the widget
    void applyPrinterProperties(QPrinter *p);
    bool checkFields();
    void setupPrinter();
    void setOptionsPane(QPrintDialogPrivate *pane);
#if !defined(QT_NO_CUPS)
    void setCupsProperties();
#endif

// slots
    void _q_printerChanged(int index);
    void _q_btnPropertiesClicked();
    void _q_btnBrowseClicked();

    QUnixPrintWidget * const parent;
    QPrintPropertiesDialog *propertiesDialog;
    Ui::QPrintWidget widget;
    QAbstractPrintDialog * q;
    QPrinter *printer;
    void updateWidget();

private:
    QPrintDialogPrivate *optionsPane;
    bool filePrintersAdded;
#if !defined(QT_NO_CUPS)
    QCUPSSupport* cups;
    int cupsPrinterCount;
    const cups_dest_t* cupsPrinters;
    const ppd_file_t* cupsPPD;
#endif
};

class QUnixPrintWidget : public QWidget
{
    Q_OBJECT

public:
    QUnixPrintWidget(QPrinter *printer, QWidget *parent = nullptr);
    ~QUnixPrintWidget();

private:
    friend class QPrintDialogPrivate;
    friend class QUnixPrintWidgetPrivate;
    QUnixPrintWidgetPrivate *d;
    Q_PRIVATE_SLOT(d, void _q_printerChanged(int))
    Q_PRIVATE_SLOT(d, void _q_btnBrowseClicked())
    Q_PRIVATE_SLOT(d, void _q_btnPropertiesClicked())
};

#if !defined(QT_NO_CUPS)
class QOptionTreeItem
{
public:
    enum ItemType { Root, Group, Option, Choice };

    QOptionTreeItem(ItemType t, int i, const void* p, const char* desc, QOptionTreeItem* pi)
        : type(t),
          index(i),
          ptr(p),
          description(desc),
          selected(-1),
          selDescription(0),
          parentItem(pi) {}

    ~QOptionTreeItem() {
        while (!childItems.isEmpty())
            delete childItems.takeFirst();
    }

    ItemType type;
    int index;
    const void* ptr;
    const char* description;
    int selected;
    const char* selDescription;
    QOptionTreeItem* parentItem;
    QList<QOptionTreeItem*> childItems;
};

class QPPDOptionsModel : public QAbstractItemModel
{
    friend class QPPDOptionsEditor;
public:
    QPPDOptionsModel(QCUPSSupport *cups, QObject *parent = nullptr);
    ~QPPDOptionsModel();

    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    QOptionTreeItem* rootItem;
    QCUPSSupport *cups;
    const ppd_file_t* ppd;
    void parseItems();
    void parseGroups(QOptionTreeItem* parent);
    void parseOptions(QOptionTreeItem* parent);
    void parseChoices(QOptionTreeItem* parent);
};

class QPPDOptionsEditor : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QPPDOptionsEditor(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    ~QPPDOptionsEditor() {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

private slots:
    void cbChanged(int index);

};

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPrintPropertiesDialog::QPrintPropertiesDialog(QAbstractPrintDialog *parent)
    : QDialog(parent)
#if !defined(QT_NO_CUPS)
    , m_cups(0), m_cupsOptionsModel(0)
#endif
{
    QVBoxLayout *lay = new QVBoxLayout(this);
    this->setLayout(lay);
    QWidget *content = new QWidget(this);
    widget.setupUi(content);
    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    lay->addWidget(content);
    lay->addWidget(m_buttons);

    connect(m_buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
}

QPrintPropertiesDialog::~QPrintPropertiesDialog()
{
#if !defined(QT_NO_CUPS)
    delete m_cupsOptionsModel;
#else
    delete widget.cupsPropertiesPage;
#endif
}

void QPrintPropertiesDialog::applyPrinterProperties(QPrinter *p)
{
    widget.pageSetup->setPrinter(p);
}

void QPrintPropertiesDialog::setupPrinter() const
{
    widget.pageSetup->setupPrinter();

#if !defined(QT_NO_CUPS)
    QPPDOptionsModel* model = static_cast<QPPDOptionsModel*>(widget.treeView->model());
    if (model) {
        QOptionTreeItem* rootItem = model->rootItem;
        QList<const ppd_option_t*> options;
        QList<const char*> markedOptions;

        addItemToOptions(rootItem, options, markedOptions);
        model->cups->saveOptions(options, markedOptions);
    }
#endif
}

void QPrintPropertiesDialog::selectPrinter()
{
#if !defined(QT_NO_CUPS)
    widget.pageSetup->selectPrinter(m_cups);
    widget.treeView->setModel(0);
    if (m_cups && QCUPSSupport::isAvailable()) {

        if (m_cupsOptionsModel == 0) {
            m_cupsOptionsModel = new QPPDOptionsModel(m_cups);

            widget.treeView->setItemDelegate(new QPPDOptionsEditor(this));
        } else {
            // update the model
            m_cupsOptionsModel->parseItems();
        }

        if (m_cupsOptionsModel->rowCount() > 0) {
            widget.treeView->setModel(m_cupsOptionsModel);

            for (int i = 0; i < m_cupsOptionsModel->rowCount(); ++i)
                widget.treeView->expand(m_cupsOptionsModel->index(i,0));

            widget.tabs->setTabEnabled(1, true); // enable the advanced tab
        } else {
            widget.tabs->setTabEnabled(1, false);
        }

    } else
#endif
    {
        widget.cupsPropertiesPage->setEnabled(false);
        widget.pageSetup->selectPrinter(0);
    }
}

void QPrintPropertiesDialog::selectPdfPsPrinter(const QPrinter *p)
{
    widget.treeView->setModel(0);
    widget.pageSetup->selectPdfPsPrinter(p);
    widget.tabs->setTabEnabled(1, false); // disable the advanced tab
}

void QPrintPropertiesDialog::showEvent(QShowEvent* event)
{
    widget.treeView->resizeColumnToContents(0);
    event->accept();
}

#if !defined(QT_NO_CUPS)
void QPrintPropertiesDialog::addItemToOptions(QOptionTreeItem *parent, QList<const ppd_option_t*>& options, QList<const char*>& markedOptions) const
{
    for (int i = 0; i < parent->childItems.count(); ++i) {
        QOptionTreeItem *itm = parent->childItems.at(i);
        if (itm->type == QOptionTreeItem::Option) {
            const ppd_option_t* opt = reinterpret_cast<const ppd_option_t*>(itm->ptr);
            options << opt;
            if (qstrcmp(opt->defchoice, opt->choices[itm->selected].choice) != 0) {
                markedOptions << opt->keyword << opt->choices[itm->selected].choice;
            }
        } else {
            addItemToOptions(itm, options, markedOptions);
        }
    }
}
#endif

QPrintDialogPrivate::QPrintDialogPrivate()
    : top(0), bottom(0), buttons(0), collapseButton(0)
{
}

QPrintDialogPrivate::~QPrintDialogPrivate()
{
}

void QPrintDialogPrivate::init()
{
    Q_Q(QPrintDialog);

    top = new QUnixPrintWidget(0, q);
    bottom = new QWidget(q);
    options.setupUi(bottom);
    options.color->setIconSize(QSize(32, 32));
    QPixmap statusColorPixmap;
    statusColorPixmap.loadFromData(qrc_status_color(), qrc_status_color_size, "PNG");
    options.color->setIcon(QIcon(statusColorPixmap));
    options.grayscale->setIconSize(QSize(32, 32));
    QPixmap statusGrayScalePixmap;
    statusGrayScalePixmap.loadFromData(qrc_status_gray_scale(), qrc_status_gray_scale_size, "PNG");
    options.grayscale->setIcon(QIcon(statusGrayScalePixmap));
    top->d->setOptionsPane(this);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, q);
    collapseButton = new QPushButton(QPrintDialog::tr("&Options >>"), buttons);
    buttons->addButton(collapseButton, QDialogButtonBox::ResetRole);
    bottom->setVisible(false);

    QPushButton *printButton = buttons->button(QDialogButtonBox::Ok);
    printButton->setText(QPrintDialog::tr("&Print"));
    printButton->setDefault(true);

    QVBoxLayout *lay = new QVBoxLayout(q);
    q->setLayout(lay);
    lay->addWidget(top);
    lay->addWidget(bottom);
    lay->addWidget(buttons);

    QPrinter* p = q->printer();

    applyPrinterProperties(p);

#ifdef QT_NO_MESSAGEBOX
    QObject::connect(buttons, SIGNAL(accepted()), q, SLOT(accept()));
#else
    QObject::connect(buttons, SIGNAL(accepted()), q, SLOT(_q_checkFields()));
#endif
    QObject::connect(buttons, SIGNAL(rejected()), q, SLOT(reject()));

    QObject::connect(options.reverse, SIGNAL(toggled(bool)),
                     q, SLOT(_q_chbPrintLastFirstToggled(bool)));

    QObject::connect(collapseButton, SIGNAL(released()), q, SLOT(_q_collapseOrExpandDialog()));
}

void QPrintDialogPrivate::applyPrinterProperties(QPrinter *p)
{
    if (p->colorMode() == QPrinter::Color)
        options.color->setChecked(true);
    else
        options.grayscale->setChecked(true);

    switch(p->duplex()) {
    case QPrinter::DuplexNone:
        options.noDuplex->setChecked(true); break;
    case QPrinter::DuplexLongSide:
    case QPrinter::DuplexAuto:
        options.duplexLong->setChecked(true); break;
    case QPrinter::DuplexShortSide:
        options.duplexShort->setChecked(true); break;
    }
    options.copies->setValue(p->copyCount());
    options.collate->setChecked(p->collateCopies());
    options.reverse->setChecked(p->pageOrder() == QPrinter::LastPageFirst);
    top->d->applyPrinterProperties(p);
}

void QPrintDialogPrivate::_q_chbPrintLastFirstToggled(bool checked)
{
    Q_Q(QPrintDialog);
    if (checked)
        q->printer()->setPageOrder(QPrinter::LastPageFirst);
    else
        q->printer()->setPageOrder(QPrinter::FirstPageFirst);
}

void QPrintDialogPrivate::_q_collapseOrExpandDialog()
{
    int collapseHeight = 0;
    Q_Q(QPrintDialog);
    QWidget *widgetToHide = bottom;
    if (widgetToHide->isVisible()) {
        collapseButton->setText(QPrintDialog::tr("&Options >>"));
        collapseHeight = widgetToHide->y() + widgetToHide->height() - (top->y() + top->height());
    }
    else
        collapseButton->setText(QPrintDialog::tr("&Options <<"));
    widgetToHide->setVisible(! widgetToHide->isVisible());
    if (! widgetToHide->isVisible()) { // make it shrink
        q->layout()->activate();
        q->resize( QSize(q->width(), q->height() - collapseHeight) );
    }
}

#ifndef QT_NO_MESSAGEBOX
void QPrintDialogPrivate::_q_checkFields()
{
    Q_Q(QPrintDialog);
    if (top->d->checkFields())
        q->accept();
}
#endif // QT_NO_MESSAGEBOX

void QPrintDialogPrivate::setupPrinter()
{
    Q_Q(QPrintDialog);
    QPrinter* p = q->printer();

    if (options.duplex->isEnabled()) {
        if (options.noDuplex->isChecked())
            p->setDuplex(QPrinter::DuplexNone);
        else if (options.duplexLong->isChecked())
            p->setDuplex(QPrinter::DuplexLongSide);
        else
            p->setDuplex(QPrinter::DuplexShortSide);
    }

    p->setColorMode( options.color->isChecked() ? QPrinter::Color : QPrinter::GrayScale );

    // print range
    if (options.printAll->isChecked()) {
        p->setPrintRange(QPrinter::AllPages);
        p->setFromTo(0,0);
    } else if (options.printSelection->isChecked()) {
        p->setPrintRange(QPrinter::Selection);
        p->setFromTo(0,0);
    } else if (options.printCurrentPage->isChecked()) {
        p->setPrintRange(QPrinter::CurrentPage);
        p->setFromTo(0,0);
    } else if (options.printRange->isChecked()) {
        p->setPrintRange(QPrinter::PageRange);
        p->setFromTo(options.from->value(), qMax(options.from->value(), options.to->value()));
    }

    // copies
    p->setCopyCount(options.copies->value());
    p->setCollateCopies(options.collate->isChecked());

    top->d->setupPrinter();
}

void QPrintDialogPrivate::updateWidgets()
{
    Q_Q(QPrintDialog);
    options.gbPrintRange->setVisible(q->isOptionEnabled(QPrintDialog::PrintPageRange) ||
                                     q->isOptionEnabled(QPrintDialog::PrintSelection) ||
                                     q->isOptionEnabled(QPrintDialog::PrintCurrentPage));

    options.printRange->setEnabled(q->isOptionEnabled(QPrintDialog::PrintPageRange));
    options.printSelection->setVisible(q->isOptionEnabled(QPrintDialog::PrintSelection));
    options.printCurrentPage->setVisible(q->isOptionEnabled(QPrintDialog::PrintCurrentPage));
    options.collate->setVisible(q->isOptionEnabled(QPrintDialog::PrintCollateCopies));

    switch (q->printRange()) {
    case QPrintDialog::AllPages:
        options.printAll->setChecked(true);
        break;
    case QPrintDialog::Selection:
        options.printSelection->setChecked(true);
        break;
    case QPrintDialog::PageRange:
        options.printRange->setChecked(true);
        break;
    case QPrintDialog::CurrentPage:
        if (q->isOptionEnabled(QPrintDialog::PrintCurrentPage))
            options.printCurrentPage->setChecked(true);
        break;
    default:
        break;
    }
    const int minPage = qMax(1, qMin(q->minPage() , q->maxPage()));
    const int maxPage = qMax(1, q->maxPage() == INT_MAX ? 9999 : q->maxPage());

    options.from->setMinimum(minPage);
    options.to->setMinimum(minPage);
    options.from->setMaximum(maxPage);
    options.to->setMaximum(maxPage);

    options.from->setValue(q->fromPage());
    options.to->setValue(q->toPage());
    top->d->updateWidget();
}

void QPrintDialogPrivate::setTabs(const QList<QWidget*> &tabWidgets)
{
    while(options.tabs->count() > 2)
        delete options.tabs->widget(2);

    QList<QWidget*>::ConstIterator iter = tabWidgets.begin();
    while(iter != tabWidgets.constEnd()) {
        QWidget *tab = *iter;
        options.tabs->addTab(tab, tab->windowTitle());
        ++iter;
    }
}

#if !defined(QT_NO_CUPS)
void QPrintDialogPrivate::selectPrinter(QCUPSSupport *cups)
{
    options.duplex->setEnabled(cups && cups->ppdOption("Duplex"));
}
#endif

////////////////////////////////////////////////////////////////////////////////

QPrintDialog::QPrintDialog(QPrinter *printer, QWidget *parent)
    : QAbstractPrintDialog(*(new QPrintDialogPrivate), printer, parent)
{
    Q_D(QPrintDialog);
    d->init();
}

/*!
    Constructs a print dialog with the given \a parent.
*/
QPrintDialog::QPrintDialog(QWidget *parent)
    : QAbstractPrintDialog(*(new QPrintDialogPrivate), 0, parent)
{
    Q_D(QPrintDialog);
    d->init();
}

QPrintDialog::~QPrintDialog()
{
}

void QPrintDialog::setVisible(bool visible)
{
    Q_D(QPrintDialog);

    if (visible)
        d->updateWidgets();

    QAbstractPrintDialog::setVisible(visible);
}

int QPrintDialog::exec()
{
    return QDialog::exec();
}

void QPrintDialog::accept()
{
    Q_D(QPrintDialog);
    d->setupPrinter();
    QDialog::accept();
}


/*! \internal
*/
QUnixPrintWidgetPrivate::QUnixPrintWidgetPrivate(QUnixPrintWidget *p)
    : parent(p), propertiesDialog(0), printer(0), optionsPane(0), filePrintersAdded(false)
#if !defined(QT_NO_CUPS)
    , cups(0), cupsPrinterCount(0), cupsPrinters(0), cupsPPD(0)
#endif
{
    q = nullptr;
    if (parent)
        q = qobject_cast<QAbstractPrintDialog*> (parent->parent());

    widget.setupUi(parent);

    int currentPrinterIndex = 0;
#if !defined(QT_NO_CUPS)
    cups = new QCUPSSupport;
    if (QCUPSSupport::isAvailable()) {
        cupsPPD = cups->currentPPD();
        cupsPrinterCount = cups->availablePrintersCount();
        cupsPrinters = cups->availablePrinters();

        for (int i = 0; i < cupsPrinterCount; ++i) {
            QString printerName(QString::fromLocal8Bit(cupsPrinters[i].name));
            if (cupsPrinters[i].instance)
                printerName += QLatin1Char('/') + QString::fromLocal8Bit(cupsPrinters[i].instance);

            widget.printers->addItem(printerName);
            if (cupsPrinters[i].is_default)
                widget.printers->setCurrentIndex(i);
        }
        // the model depends on valid ppd. so before enabling the
        // properties button we make sure the ppd is in fact valid.
        if (cupsPrinterCount && cups->currentPPD()) {
            widget.properties->setEnabled(true);
        }
        currentPrinterIndex = cups->currentPrinterIndex();
    }
#endif

#if !defined(QT_NO_FILESYSTEMMODEL) && !defined(QT_NO_COMPLETER)
    QFileSystemModel *fsm = new QFileSystemModel(widget.filename);
    fsm->setRootPath(QDir::homePath());
    widget.filename->setCompleter(new QCompleter(fsm, widget.filename));
#endif
    _q_printerChanged(currentPrinterIndex);

    QObject::connect(widget.printers, SIGNAL(currentIndexChanged(int)),
                     parent, SLOT(_q_printerChanged(int)));
    QObject::connect(widget.fileBrowser, SIGNAL(clicked()), parent, SLOT(_q_btnBrowseClicked()));
    QObject::connect(widget.properties, SIGNAL(clicked()), parent, SLOT(_q_btnPropertiesClicked()));

    // disable features that QPrinter does not yet support.
    widget.preview->setVisible(false);
}

void QUnixPrintWidgetPrivate::updateWidget()
{
    const bool printToFile = q == 0 || q->isOptionEnabled(QPrintDialog::PrintToFile);
    if (printToFile && !filePrintersAdded) {
        if (widget.printers->count())
            widget.printers->insertSeparator(widget.printers->count());
        widget.printers->addItem(QPrintDialog::tr("Print to File (PDF)"));
        widget.printers->addItem(QPrintDialog::tr("Print to File (Postscript)"));
        filePrintersAdded = true;
    }
    if (!printToFile && filePrintersAdded) {
        widget.printers->removeItem(widget.printers->count()-1);
        widget.printers->removeItem(widget.printers->count()-1);
        if (widget.printers->count())
            widget.printers->removeItem(widget.printers->count()-1); // remove separator
        filePrintersAdded = false;
    }
    if (printer && filePrintersAdded && (printer->outputFormat() != QPrinter::NativeFormat
                                         || printer->printerName().isEmpty()))
    {
        if (printer->outputFormat() == QPrinter::PdfFormat)
            widget.printers->setCurrentIndex(widget.printers->count() - 2);
        else if (printer->outputFormat() == QPrinter::PostScriptFormat)
            widget.printers->setCurrentIndex(widget.printers->count() - 1);
        widget.filename->setEnabled(true);
        widget.lOutput->setEnabled(true);
    }

    widget.filename->setVisible(printToFile);
    widget.lOutput->setVisible(printToFile);
    widget.fileBrowser->setVisible(printToFile);

    if (q)
        widget.properties->setVisible(q->isOptionEnabled(QAbstractPrintDialog::PrintShowPageSize));
}

QUnixPrintWidgetPrivate::~QUnixPrintWidgetPrivate()
{
#if !defined(QT_NO_CUPS)
    delete cups;
#endif
}

void QUnixPrintWidgetPrivate::_q_printerChanged(int index)
{
    if (index < 0)
        return;
    const int printerCount = widget.printers->count();
    widget.filename->setEnabled(false);
    widget.lOutput->setEnabled(false);

    if (filePrintersAdded) {
        Q_ASSERT(index != printerCount - 3); // separator
        if (index > printerCount - 3) { // PDF or postscript
            bool pdfPrinter = (index == printerCount - 2);
            widget.location->setText(QPrintDialog::tr("Local file"));
            widget.type->setText(QPrintDialog::tr("Write %1 file").arg(pdfPrinter ? QString::fromLatin1("PDF")
                                                                       : QString::fromLatin1("PostScript")));
            widget.properties->setEnabled(true);
            widget.filename->setEnabled(true);
            QString filename = widget.filename->text();
            QString suffix = QFileInfo(filename).suffix();
            if (pdfPrinter && suffix == QLatin1String("ps"))
                filename = filename.replace(QLatin1String(".ps"), QLatin1String(".pdf"));
            if (!pdfPrinter && suffix == QLatin1String("pdf"))
                filename = filename.replace(QLatin1String(".pdf"), QLatin1String(".ps"));
            widget.filename->setText(filename);
            widget.lOutput->setEnabled(true);
            if (propertiesDialog)
                propertiesDialog->selectPdfPsPrinter(printer);
#if !defined(QT_NO_CUPS)
            if (optionsPane)
                optionsPane->selectPrinter(0);
#endif
            return;
        }
    }

    widget.location->setText(QString());
#if !defined(QT_NO_CUPS)
    if (QCUPSSupport::isAvailable()) {
        cups->setCurrentPrinter(index);

        const cups_option_t *opt = cups->printerOption(QString::fromLatin1("printer-location"));
        QString location;
        if (opt)
            location = QString::fromLocal8Bit(opt->value);
        widget.location->setText(location);

        cupsPPD = cups->currentPPD();
        // set printer type line
        QString type;
        if (cupsPPD)
            type = QString::fromLocal8Bit(cupsPPD->manufacturer) + QLatin1String(" - ") + QString::fromLocal8Bit(cupsPPD->modelname);
        widget.type->setText(type);
        if (propertiesDialog)
            propertiesDialog->selectPrinter();
        if (optionsPane)
            optionsPane->selectPrinter(cups);
    } else {
        if (optionsPane)
            optionsPane->selectPrinter(0);
    }
#endif
}

void QUnixPrintWidgetPrivate::setOptionsPane(QPrintDialogPrivate *pane)
{
    optionsPane = pane;
    if (optionsPane)
        _q_printerChanged(widget.printers->currentIndex());
}

void QUnixPrintWidgetPrivate::_q_btnBrowseClicked()
{
    QString filename = widget.filename->text();
#ifndef QT_NO_FILEDIALOG
    filename = QFileDialog::getSaveFileName(parent, QPrintDialog::tr("Print To File ..."), filename,
                                            QString(), 0, QFileDialog::DontConfirmOverwrite);
#else
    filename.clear();
#endif
    if (!filename.isEmpty()) {
        widget.filename->setText(filename);
        if (filename.endsWith(QString::fromLatin1(".ps"), Qt::CaseInsensitive))
            widget.printers->setCurrentIndex(widget.printers->count() - 1); // the postscript one
        else if (filename.endsWith(QString::fromLatin1(".pdf"), Qt::CaseInsensitive))
            widget.printers->setCurrentIndex(widget.printers->count() - 2); // the pdf one
        else if (widget.printers->currentIndex() != widget.printers->count() - 1) // if ps is not selected, pdf is default
            widget.printers->setCurrentIndex(widget.printers->count() - 2); // the pdf one
    }
}

void QUnixPrintWidgetPrivate::applyPrinterProperties(QPrinter *p)
{
    if (p == 0)
        return;
    printer = p;
    if (p->outputFileName().isEmpty()) {
        QString home = QDir::homePath();
        QString cur = QDir::currentPath();
        if (home.at(home.length()-1) != QLatin1Char('/'))
            home += QLatin1Char('/');
        if (cur.at(cur.length()-1) != QLatin1Char('/'))
            cur += QLatin1Char('/');
        if (cur.left(home.length()) != home)
            cur = home;
#ifdef Q_WS_X11
        if (p->docName().isEmpty()) {
            if (p->outputFormat() == QPrinter::PostScriptFormat)
                cur += QLatin1String("print.ps");
            else
                cur += QLatin1String("print.pdf");
        } else {
            QRegExp re(QString::fromLatin1("(.*)\\.\\S+"));
            if (re.exactMatch(p->docName()))
                cur += re.cap(1);
            else
                cur += p->docName();
            if (p->outputFormat() == QPrinter::PostScriptFormat)
                cur += QLatin1String(".ps");
            else
                cur += QLatin1String(".pdf");
        }
#endif
        widget.filename->setText(cur);
    }
    else
        widget.filename->setText( p->outputFileName() );
    QString printer = p->printerName();
    if (!printer.isEmpty()) {
        for (int i = 0; i < widget.printers->count(); ++i) {
            if (widget.printers->itemText(i) == printer) {
                widget.printers->setCurrentIndex(i);
                break;
            }
        }
    }
    // PDF and PS printers are not added to the dialog yet, we'll handle those cases in QUnixPrintWidgetPrivate::updateWidget

    if (propertiesDialog)
        propertiesDialog->applyPrinterProperties(p);
}

#ifndef QT_NO_MESSAGEBOX
bool QUnixPrintWidgetPrivate::checkFields()
{
    if (widget.filename->isEnabled()) {
        QString file = widget.filename->text();
        QFile f(file);
        QStatInfo si(file);
        bool exists = si.exists();
        bool opened = false;
        if (exists && si.isDir()) {
            QMessageBox::warning(q, q->windowTitle(),
                            QPrintDialog::tr("%1 is a directory.\nPlease choose a different file name.").arg(file));
            return false;
        } else if ((exists && !si.isWritable()) || !(opened = f.open(QFile::Append))) {
            QMessageBox::warning(q, q->windowTitle(),
                            QPrintDialog::tr("File %1 is not writable.\nPlease choose a different file name.").arg(file));
            return false;
        } else if (exists) {
            int ret = QMessageBox::question(q, q->windowTitle(),
                                            QPrintDialog::tr("%1 already exists.\nDo you want to overwrite it?").arg(file),
                                            QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
            if (ret == QMessageBox::No)
                return false;
        }
        if (opened) {
            f.close();
            if (!exists)
                f.remove();
        }
    }

    // Every test passed. Accept the dialog.
    return true;
}
#endif // QT_NO_MESSAGEBOX

void QUnixPrintWidgetPrivate::_q_btnPropertiesClicked()
{
    if (!propertiesDialog) {
        propertiesDialog = new QPrintPropertiesDialog(q);
        propertiesDialog->setResult(QDialog::Rejected);
    }

    if (propertiesDialog->result() == QDialog::Rejected) {
#if !defined(QT_NO_CUPS)
        propertiesDialog->setCups(cups);
#endif
        propertiesDialog->applyPrinterProperties(q->printer());

        if (q->isOptionEnabled(QPrintDialog::PrintToFile)
            && (widget.printers->currentIndex() > widget.printers->count() - 3)) // PDF or postscript
            propertiesDialog->selectPdfPsPrinter(q->printer());
        else
            propertiesDialog->selectPrinter();
    }
    propertiesDialog->exec();
}

#if !defined(QT_NO_CUPS)
void QUnixPrintWidgetPrivate::setCupsProperties()
{
    if (cups && QCUPSSupport::isAvailable() && cups->pageSizes()) {
        QPrintEngine *engine = printer->printEngine();
        const ppd_option_t* pageSizes = cups->pageSizes();
        QByteArray cupsPageSize;
        for (int i = 0; i < pageSizes->num_choices; ++i) {
            if (static_cast<int>(pageSizes->choices[i].marked) == 1)
                cupsPageSize = pageSizes->choices[i].choice;
        }
        engine->setProperty(PPK_CupsStringPageSize, QString::fromLatin1(cupsPageSize));
        engine->setProperty(PPK_CupsOptions, cups->options());

        QRect pageRect = cups->pageRect(cupsPageSize);
        engine->setProperty(PPK_CupsPageRect, pageRect);

        QRect paperRect = cups->paperRect(cupsPageSize);
        engine->setProperty(PPK_CupsPaperRect, paperRect);

        for (int ps = 0; ps < QPrinter::NPaperSize; ++ps) {
            QPdf::PaperSize size = QPdf::paperSize(QPrinter::PaperSize(ps));
            if (size.width == paperRect.width() && size.height == paperRect.height())
                printer->setPaperSize(static_cast<QPrinter::PaperSize>(ps));
        }
    }
}
#endif

void QUnixPrintWidgetPrivate::setupPrinter()
{
    const int printerCount = widget.printers->count();
    const int index = widget.printers->currentIndex();

    if (filePrintersAdded && index > printerCount - 3) { // PDF or postscript
        printer->setPrinterName(QString());
        Q_ASSERT(index != printerCount - 3); // separator
        if (index == printerCount - 2)
            printer->setOutputFormat(QPrinter::PdfFormat);
        else
            printer->setOutputFormat(QPrinter::PostScriptFormat);
        QString path = widget.filename->text();
        if (QDir::isRelativePath(path))
            path = QDir::homePath() + QDir::separator() + path;
        printer->setOutputFileName(path);
    }
    else {
        printer->setPrinterName(widget.printers->currentText());
        printer->setOutputFileName(QString());
    }

    if (propertiesDialog && propertiesDialog->result() == QDialog::Accepted)
        propertiesDialog->setupPrinter();
#if !defined(QT_NO_CUPS)
    if (!propertiesDialog)
        setCupsProperties();
#endif
}


/*! \internal
*/
QUnixPrintWidget::QUnixPrintWidget(QPrinter *printer, QWidget *parent)
    : QWidget(parent), d(new QUnixPrintWidgetPrivate(this))
{
    d->applyPrinterProperties(printer);
}

/*! \internal
*/
QUnixPrintWidget::~QUnixPrintWidget()
{
    delete d;
}

////////////////////////////////////////////////////////////////////////////////
#if !defined(QT_NO_CUPS)

QPPDOptionsModel::QPPDOptionsModel(QCUPSSupport *c, QObject *parent)
    : QAbstractItemModel(parent), rootItem(0), cups(c), ppd(c->currentPPD())
{
    parseItems();
}

QPPDOptionsModel::~QPPDOptionsModel()
{
}

int QPPDOptionsModel::columnCount(const QModelIndex&) const
{
    return 2;
}

int QPPDOptionsModel::rowCount(const QModelIndex& parent) const
{
    QOptionTreeItem* itm;
    if (!parent.isValid())
        itm = rootItem;
    else
        itm = reinterpret_cast<QOptionTreeItem*>(parent.internalPointer());

    if (itm->type == QOptionTreeItem::Option)
        return 0;

    return itm->childItems.count();
}

QVariant QPPDOptionsModel::data(const QModelIndex& index, int role) const
{
    switch(role) {
        case Qt::FontRole: {
            QOptionTreeItem* itm = reinterpret_cast<QOptionTreeItem*>(index.internalPointer());
            if (itm && itm->type == QOptionTreeItem::Group){
                QFont font = QApplication::font();
                font.setBold(true);
                return QVariant(font);
            }
            return QVariant();
        }

        case Qt::DisplayRole: {
            QOptionTreeItem* itm;
            if (!index.isValid())
                itm = rootItem;
            else
                itm = reinterpret_cast<QOptionTreeItem*>(index.internalPointer());

            if (index.column() == 0)
                return cups->unicodeString(itm->description);
            else if (itm->type == QOptionTreeItem::Option && itm->selected > -1)
                return cups->unicodeString(itm->selDescription);
            return QVariant();
        }

        default:
            return QVariant();
    }
    return QVariant();
}

QModelIndex QPPDOptionsModel::index(int row, int column, const QModelIndex& parent) const
{
    QOptionTreeItem* itm;
    if (!parent.isValid())
        itm = rootItem;
    else
        itm = reinterpret_cast<QOptionTreeItem*>(parent.internalPointer());

    return createIndex(row, column, itm->childItems.at(row));
}


QModelIndex QPPDOptionsModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    QOptionTreeItem* itm = reinterpret_cast<QOptionTreeItem*>(index.internalPointer());

    if (itm->parentItem && itm->parentItem != rootItem)
        return createIndex(itm->parentItem->index, 0, itm->parentItem);
    else
        return QModelIndex();
}

Qt::ItemFlags QPPDOptionsModel::flags(const QModelIndex& index) const
{
    if (!index.isValid() || reinterpret_cast<QOptionTreeItem*>(index.internalPointer())->type == QOptionTreeItem::Group)
        return Qt::ItemIsEnabled;

    if (index.column() == 1)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void QPPDOptionsModel::parseItems()
{
    emit layoutAboutToBeChanged();
    ppd = cups->currentPPD();
    delete rootItem;
    rootItem = new QOptionTreeItem(QOptionTreeItem::Root, 0, ppd, "Root Item", 0);
    parseGroups(rootItem);
    emit layoutChanged();
}

void QPPDOptionsModel::parseGroups(QOptionTreeItem* parent)
{
    if (parent->type == QOptionTreeItem::Root) {

        const ppd_file_t* ppdFile = reinterpret_cast<const ppd_file_t*>(parent->ptr);

        if (ppdFile) {
            for (int i = 0; i < ppdFile->num_groups; ++i) {
                QOptionTreeItem* group = new QOptionTreeItem(QOptionTreeItem::Group, i, &ppdFile->groups[i], ppdFile->groups[i].text, parent);
                parent->childItems.append(group);
                parseGroups(group); // parse possible subgroups
                parseOptions(group); // parse options
            }
        }
    } else if (parent->type == QOptionTreeItem::Group) {

        const ppd_group_t* group = reinterpret_cast<const ppd_group_t*>(parent->ptr);

        if (group) {
            for (int i = 0; i < group->num_subgroups; ++i) {
                QOptionTreeItem* subgroup = new QOptionTreeItem(QOptionTreeItem::Group, i, &group->subgroups[i], group->subgroups[i].text, parent);
                parent->childItems.append(subgroup);
                parseGroups(subgroup); // parse possible subgroups
                parseOptions(subgroup); // parse options
            }
        }
    }
}

void QPPDOptionsModel::parseOptions(QOptionTreeItem* parent)
{
    const ppd_group_t* group = reinterpret_cast<const ppd_group_t*>(parent->ptr);
    for (int i = 0; i < group->num_options; ++i) {
        QOptionTreeItem* opt = new QOptionTreeItem(QOptionTreeItem::Option, i, &group->options[i], group->options[i].text, parent);
        parent->childItems.append(opt);
        parseChoices(opt);
    }
}

void QPPDOptionsModel::parseChoices(QOptionTreeItem* parent)
{
    const ppd_option_t* option = reinterpret_cast<const ppd_option_t*>(parent->ptr);
    bool marked = false;
    for (int i = 0; i < option->num_choices; ++i) {
        QOptionTreeItem* choice = new QOptionTreeItem(QOptionTreeItem::Choice, i, &option->choices[i], option->choices[i].text, parent);
        if (static_cast<int>(option->choices[i].marked) == 1) {
            parent->selected = i;
            parent->selDescription = option->choices[i].text;
            marked = true;
        } else if (!marked && qstrcmp(option->choices[i].choice, option->defchoice) == 0) {
            parent->selected = i;
            parent->selDescription = option->choices[i].text;
        }
        parent->childItems.append(choice);
    }
}

QVariant QPPDOptionsModel::headerData(int section, Qt::Orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch(section){
        case 0:
            return QVariant(QApplication::translate("QPPDOptionsModel", "Name"));
        case 1:
            return QVariant(QApplication::translate("QPPDOptionsModel", "Value"));
        default:
            return QVariant();
    }
}

////////////////////////////////////////////////////////////////////////////////

QWidget* QPPDOptionsEditor::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const
{
    if (index.column() == 1 && reinterpret_cast<QOptionTreeItem*>(index.internalPointer())->type == QOptionTreeItem::Option)
        return new QComboBox(parent);
    else
        return 0;
}

void QPPDOptionsEditor::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (index.column() != 1)
        return;

    QComboBox* cb = static_cast<QComboBox*>(editor);
    QOptionTreeItem* itm = reinterpret_cast<QOptionTreeItem*>(index.internalPointer());

    if (itm->selected == -1)
        cb->addItem(QString());

    for (int i = 0; i < itm->childItems.count(); ++i)
        cb->addItem(QString::fromLocal8Bit(itm->childItems.at(i)->description));

    if (itm->selected > -1)
        cb->setCurrentIndex(itm->selected);

    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(cbChanged(int)));
}

void QPPDOptionsEditor::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* cb = static_cast<QComboBox*>(editor);
    QOptionTreeItem* itm = reinterpret_cast<QOptionTreeItem*>(index.internalPointer());

    if (itm->selected == cb->currentIndex())
        return;

    const ppd_option_t* opt = reinterpret_cast<const ppd_option_t*>(itm->ptr);
    QPPDOptionsModel* m = static_cast<QPPDOptionsModel*>(model);

    if (m->cups->markOption(opt->keyword, opt->choices[cb->currentIndex()].choice) == 0) {
        itm->selected = cb->currentIndex();
        itm->selDescription = reinterpret_cast<const ppd_option_t*>(itm->ptr)->choices[itm->selected].text;
    }
}

void QPPDOptionsEditor::cbChanged(int)
{
/*
    emit commitData(static_cast<QWidget*>(sender()));
*/
}

#endif

QT_END_NAMESPACE

#include "moc_qprintdialog.h"
#include "moc_qprintdialog_unix.cpp"

#endif // QT_NO_PRINTDIALOG

