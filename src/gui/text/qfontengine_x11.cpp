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
// Multi FT engine
// ------------------------------------------------------------------

static QFontEngine *engineForPattern(FcPattern *match, const QFontDef &request, int screen)
{
    QFontEngineX11FT *engine = new QFontEngineX11FT(match, request, screen);
    if (!engine->invalid())
        return engine;

    delete engine;
    QFontEngine *fe = new QFontEngineBox(request.pixelSize);
    fe->fontDef = request;
    return fe;
}

QFontEngineMultiFT::QFontEngineMultiFT(QFontEngine *fe, FcPattern *matchedPattern, FcPattern *p, int s, const QFontDef &req)
    : QFontEngineMulti(2), request(req), pattern(p), fontSet(0), screen(s)
{
    firstEnginePattern = FcPatternDuplicate(matchedPattern);
    engines[0] = fe;
    engines.at(0)->ref.ref();
    fontDef = engines[0]->fontDef;
    cache_cost = 100;
    firstFontIndex = 1;
}

QFontEngineMultiFT::~QFontEngineMultiFT()
{
    extern std::recursive_mutex& qt_fontdatabase_mutex();
    std::lock_guard<std::recursive_mutex> locker(qt_fontdatabase_mutex());

    FcPatternDestroy(pattern);
    if (firstEnginePattern)
        FcPatternDestroy(firstEnginePattern);
    if (fontSet)
        FcFontSetDestroy(fontSet);
}


void QFontEngineMultiFT::loadEngine(int at)
{
    extern std::recursive_mutex& qt_fontdatabase_mutex();
    std::lock_guard<std::recursive_mutex> locker(qt_fontdatabase_mutex());

    extern QFontDef qt_FcPatternToQFontDef(FcPattern *pattern, const QFontDef &);
    extern FcFontSet *qt_fontSetForPattern(FcPattern *pattern, const QFontDef &request);

    Q_ASSERT(at > 0);
    if (!fontSet) {
        fontSet = qt_fontSetForPattern(pattern, request);

        // it may happen that the fontset of fallbacks consists of only one font. In this case we
        // have to fall back to the box fontengine as we cannot render the glyph.
        if (fontSet->nfont == 1 && at == 1 && engines.size() == 2) {
            Q_ASSERT(engines.at(at) == 0);
            QFontEngine *fe = new QFontEngineBox(request.pixelSize);
            fe->fontDef = request;
            engines[at] = fe;
            return;
        }

        if (firstEnginePattern) {

            if (!FcPatternEqual(firstEnginePattern, fontSet->fonts[0]))
                firstFontIndex = 0;

            FcPatternDestroy(firstEnginePattern);
            firstEnginePattern = 0;
        }

        engines.resize(fontSet->nfont + 1 - firstFontIndex);
    }
    Q_ASSERT(at < engines.size());
    Q_ASSERT(engines.at(at) == 0);

    FcPattern *match = FcFontRenderPrepare(NULL, pattern, fontSet->fonts[at + firstFontIndex - 1]);
    QFontDef fontDef = qt_FcPatternToQFontDef(match, this->request);

    // note: we use -1 for the script to make sure that we keep real
    // FT engines separate from Multi engines in the font cache
    QFontCache::Key key(fontDef, -1, screen);
    QFontEngine *fontEngine = QFontCache::instance()->findEngine(key);
    if (!fontEngine) {
        fontEngine = engineForPattern(match, request, screen);
        QFontCache::instance()->insertEngine(key, fontEngine);
    }
    FcPatternDestroy(match);
    fontEngine->ref.ref();
    engines[at] = fontEngine;
}

// ------------------------------------------------------------------
// X11 FT engine
// ------------------------------------------------------------------
QFontEngineX11FT::QFontEngineX11FT(FcPattern *pattern, const QFontDef &fd, int screen)
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

    if (!init(face_id))
        return;
}

QFontEngineX11FT::~QFontEngineX11FT()
{
}

#endif // QT_NO_FONTCONFIG

QT_END_NAMESPACE



