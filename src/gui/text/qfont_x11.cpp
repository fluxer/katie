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

#include "qfont.h"
#include "qapplication.h"
#include "qfontengine_p.h"
#include "qfontengine_ft_p.h"
#include "qx11info_x11.h"

QT_BEGIN_NAMESPACE

/*!
  \internal
  X11 Only: Returns the screen with which this font is associated.
*/
int QFont::x11Screen() const
{
    return d->screen;
}

/*! \internal
    X11 Only: Associate the font with the specified \a screen.
*/
void QFont::x11SetScreen(int screen)
{
    if (screen < 0) // assume default
        screen = QX11Info::appScreen();

    if (screen == d->screen)
        return; // nothing to do

    detach();
    d->screen = screen;
}

FT_Face QFont::freetypeFace() const
{
    QFontEngine *engine = d->engineForScript(QUnicodeTables::Common);
    if (engine->type() == QFontEngine::Freetype) {
        const QFontEngineFT *ft = static_cast<const QFontEngineFT *>(engine);
        return ft->getFace();
    }
    return 0;
}

QString QFont::lastResortFamily()
{
    return QString::fromLatin1("FreeSans");
}

QT_END_NAMESPACE


