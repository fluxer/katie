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

#ifndef QFONTENGINE_X11_P_H
#define QFONTENGINE_X11_P_H

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

#include "qfontengine_ft_p.h"

QT_BEGIN_NAMESPACE

class QFreetypeFace;

#ifndef QT_NO_FONTCONFIG

class Q_GUI_EXPORT QFontEngineMultiFT : public QFontEngineMulti
{
public:
    QFontEngineMultiFT(QFontEngine *fe, FcPattern *firstEnginePattern, FcPattern *p, int s, const QFontDef &request);
    ~QFontEngineMultiFT();

    void loadEngine(int at);

private:
    QFontDef request;
    FcPattern *pattern;
    FcPattern *firstEnginePattern;
    FcFontSet *fontSet;
    int screen;
    int firstFontIndex; // first font in fontset
};

class Q_GUI_EXPORT QFontEngineX11FT : public QFontEngineFT
{
public:
    explicit QFontEngineX11FT(const QFontDef &fontDef) : QFontEngineFT(fontDef) {}
    explicit QFontEngineX11FT(FcPattern *pattern, const QFontDef &fd, int screen);
    ~QFontEngineX11FT();

    QFontEngine *cloneWithSize(qreal pixelSize) const;

#ifndef QT_NO_XRENDER
    int xglyph_format;
#endif

protected:
    virtual bool uploadGlyphToServer(QGlyphSet *set, uint glyphid, Glyph *g, GlyphInfo *info, int glyphDataSize) const;
    virtual unsigned long allocateServerGlyphSet();
    virtual void freeServerGlyphSet(unsigned long id);
};

#endif // QT_NO_FONTCONFIG

QT_END_NAMESPACE

#endif // QFONTENGINE_X11_P_H
