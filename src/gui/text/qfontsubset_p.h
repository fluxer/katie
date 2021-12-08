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

#ifndef QFONTSUBSET_P_H
#define QFONTSUBSET_P_H

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

#include "qfontengine_p.h"

#ifndef QT_NO_PRINTER

QT_BEGIN_NAMESPACE

class QFontSubset
{
public:
    QFontSubset(QFontEngine *fe, int obj_id)
        : object_id(obj_id), noEmbed(false), fontEngine(fe), downloaded_glyphs(0), standard_font(false)
        { fontEngine->ref.ref(); addGlyph(0); }
    ~QFontSubset() {
        if (!fontEngine->ref.deref() && fontEngine->cache_count == 0)
            delete fontEngine;
    }

    QByteArray toTruetype() const;
    QByteArray toType1() const;
    QByteArray type1AddedGlyphs() const;
    QByteArray widthArray() const;
    QByteArray createToUnicodeMap() const;
    QVector<int> getReverseMap() const;
    QByteArray glyphName(unsigned int glyph, const QVector<int> &reverseMap) const;

    static QByteArray glyphName(unsigned short unicode);

    int addGlyph(int index);
    const int object_id;
    bool noEmbed;
    QFontEngine *fontEngine;
    QList<int> glyph_indices;
    mutable int downloaded_glyphs;
    mutable bool standard_font;
    mutable QVector<QFixed> widths;
};

QT_END_NAMESPACE

#endif // QT_NO_PRINTER

#endif // QFONTSUBSET_P_H
