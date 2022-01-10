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
#include "qfontdatabase.h"
#include "qfontengine_p.h"
#include "qfontengine_ft_p.h"
#include "qx11info_x11.h"

QT_BEGIN_NAMESPACE

double qt_pixelSize(double pointSize, int dpi)
{
    if (pointSize < 0)
        return -1.;
    if (dpi == 75) // the stupid 75 dpi setting on X11
        dpi = 72;
    return ((pointSize * dpi) / 72.0);
}

double qt_pointSize(double pixelSize, int dpi)
{
    if (pixelSize < 0)
        return -1.;
    if (dpi == 75) // the stupid 75 dpi setting on X11
        dpi = 72;
    return (pixelSize * 72. / double(dpi));
}

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

Qt::HANDLE QFont::handle() const
{
    return 0;
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
    return QString::fromLatin1("Helvetica");
}

QT_END_NAMESPACE


