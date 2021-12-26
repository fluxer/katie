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

#include "qbitmap.h"

// #define FONTENGINE_DEBUG

#include "qapplication.h"
#include "qbytearray.h"
#include "qdebug.h"
#include "qtextcodec.h"
#include "qthread.h"

#include "qfontdatabase.h"
#include "qpaintdevice.h"
#include "qpainter.h"
#include "qwidget.h"
#include "qsettings.h"
#include "qfile.h"

#include "qpaintengine_x11_p.h"
#include "qfont.h"
#include "qfont_p.h"
#include "qfontengine_p.h"
#include "qhash.h"

#include "qpainter_p.h"
#include "qunicodetables_p.h"

#include "qt_x11_p.h"
#include "qpixmap_x11_p.h"
#include "qx11info_x11.h"
#include "qfontengine_x11_p.h"

#include <limits.h>

#include <ft2build.h>
#if defined(FT_LCD_FILTER_H)
#include FT_LCD_FILTER_H
#endif

#if defined(FC_LCD_FILTER)

#ifndef FC_LCD_FILTER_NONE
#define FC_LCD_FILTER_NONE FC_LCD_NONE
#endif

#ifndef FC_LCD_FILTER_DEFAULT
#define FC_LCD_FILTER_DEFAULT FC_LCD_DEFAULT
#endif

#ifndef FC_LCD_FILTER_LIGHT
#define FC_LCD_FILTER_LIGHT FC_LCD_LIGHT
#endif

#ifndef FC_LCD_FILTER_LEGACY
#define FC_LCD_FILTER_LEGACY FC_LCD_LEGACY
#endif

#endif

QT_BEGIN_NAMESPACE

#ifndef QT_NO_FONTCONFIG

// ------------------------------------------------------------------
// X11 FT engine
// ------------------------------------------------------------------
QFontEngineX11FT::QFontEngineX11FT(FcPattern *pattern, const QFontDef &fd)
    : QFontEngineFT(fd)
{
//     FcPatternPrint(pattern);
    QFontEngine::FaceId face_id;
    FcChar8 *fileName;

    FcPatternGetString(pattern, FC_FILE, 0, &fileName);
    face_id.filename = (const char *)fileName;

    if (!FcPatternGetInteger(pattern, FC_INDEX, 0, &face_id.index))
        face_id.index = 0;

    if (fd.hintingPreference != QFont::PreferDefaultHinting) {
        switch (fd.hintingPreference) {
        case QFont::PreferNoHinting:
            default_hint_style = HintNone;
            break;
        case QFont::PreferVerticalHinting:
            default_hint_style = HintLight;
            break;
        case QFont::PreferFullHinting:
        default:
            default_hint_style = HintFull;
            break;
        }
    }
#ifdef FC_HINT_STYLE
    else {
        int hint_style = 0;
        if (FcPatternGetInteger (pattern, FC_HINT_STYLE, 0, &hint_style) == FcResultNoMatch
                 && qt_x11Data->fc_hint_style > -1)
            hint_style = qt_x11Data->fc_hint_style;

        switch (hint_style) {
        case FC_HINT_NONE:
            default_hint_style = HintNone;
            break;
        case FC_HINT_SLIGHT:
            default_hint_style = HintLight;
            break;
        case FC_HINT_MEDIUM:
            default_hint_style = HintMedium;
            break;
        default:
            default_hint_style = HintFull;
            break;
        }
    }
#endif

#if defined(FC_AUTOHINT) && defined(FT_LOAD_FORCE_AUTOHINT)
    {
        bool autohint = false;

        FcBool b;
        if (FcPatternGetBool(pattern, FC_AUTOHINT, 0, &b) == FcResultMatch)
            autohint = b;

        if (autohint)
            default_load_flags |= FT_LOAD_FORCE_AUTOHINT;
    }
#endif

    init(face_id);
}

QFontEngineX11FT::~QFontEngineX11FT()
{
}

#endif // QT_NO_FONTCONFIG

QT_END_NAMESPACE



