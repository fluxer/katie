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

#ifndef QFONT_P_H
#define QFONT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of internal files.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include "QtGui/qfont.h"
#include "QtCore/qmap.h"
#include "QtCore/qobject.h"
#include "qunicodetables_p.h"
#include <QtGui/qfontdatabase.h>
#include "qfixed_p.h"

QT_BEGIN_NAMESPACE

// forwards
class QFontCache;
class QFontEngine;

struct QFontDef
{
    inline QFontDef()
        : pointSize(-1.0), pixelSize(-1),
          styleStrategy(QFont::PreferDefault),
          fixedPitch(false), style(QFont::StyleNormal), weight(50), stretch(100),
          ignorePitch(true), hintingPreference(QFont::PreferDefaultHinting)
    {
    }

    QString family;
    QString styleName;

    qreal pointSize;
    qreal pixelSize;

    QFont::StyleStrategy styleStrategy;

    bool fixedPitch;
    QFont::Style style;
    int weight; // 0-99
    int stretch; // 0-4000

    bool ignorePitch;
    QFont::HintingPreference hintingPreference;

    bool exactMatch(const QFontDef &other) const;
    bool operator==(const QFontDef &other) const
    {
        return pointSize == other.pointSize
                    && pixelSize == other.pixelSize
                    && weight == other.weight
                    && style == other.style
                    && stretch == other.stretch
                    && styleStrategy == other.styleStrategy
                    && ignorePitch == other.ignorePitch && fixedPitch == other.fixedPitch
                    && family == other.family
                    && (styleName.isEmpty() || other.styleName.isEmpty() || styleName == other.styleName)
                    && hintingPreference == other.hintingPreference;
    }
    inline bool operator<(const QFontDef &other) const
    {
        if (pointSize != other.pointSize) return pointSize < other.pointSize;
        if (pixelSize != other.pixelSize) return pixelSize < other.pixelSize;
        if (weight != other.weight) return weight < other.weight;
        if (style != other.style) return style < other.style;
        if (stretch != other.stretch) return stretch < other.stretch;
        if (styleStrategy != other.styleStrategy) return styleStrategy < other.styleStrategy;
        if (family != other.family) return family < other.family;
        if (!styleName.isEmpty() && !other.styleName.isEmpty() && styleName != other.styleName)
            return styleName < other.styleName;
        if (hintingPreference != other.hintingPreference) return hintingPreference < other.hintingPreference;
        if (ignorePitch != other.ignorePitch) return ignorePitch < other.ignorePitch;
        if (fixedPitch != other.fixedPitch) return fixedPitch < other.fixedPitch;
        return false;
    }
};

class Q_GUI_EXPORT QFontPrivate
{
public:
    QFontPrivate();
    QFontPrivate(const QFontPrivate &other);
    ~QFontPrivate();

    QFontEngine *engineForScript(QUnicodeTables::Script script) const;

    QAtomicInt ref;
    QFontDef request;
    int dpi;
    int screen;

    bool underline;
    bool overline;
    bool strikeOut;
    bool kerning;

    static QFontPrivate *get(const QFont &font)
    {
        return font.d.data();
    }

    void resolve(uint mask, const QFontPrivate *other);
private:
    QFontPrivate &operator=(const QFontPrivate &) { return *this; }
};


class QFontCache
{
public:
    // note: these static functions work on a per-thread basis
    static QFontCache *instance();

    QFontCache();
    ~QFontCache();

    void clear();
    // QFontEngines are cached using the same keys
    struct Key {
        Key() : script(0), screen(0) { }
        Key(const QFontDef &d, int c, int s = 0)
            : def(d), script(c), screen(s) { }

        QFontDef def;
        int script;
        int screen;

        inline bool operator<(const Key &other) const
        {
            if (script != other.script) return script < other.script;
            if (screen != other.screen) return screen < other.screen;
            return def < other.def;
        }
        inline bool operator==(const Key &other) const
        { return def == other.def && script == other.script && screen == other.screen; }
    };

    // QFontEngine cache
    typedef QMap<Key,QFontEngine *> EngineCache;
    EngineCache engineCache;

    QFontEngine *findEngine(const Key &key);
    void insertEngine(const Key &key, QFontEngine *engine);
};

QT_END_NAMESPACE

#endif // QFONT_P_H
