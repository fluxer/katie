/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the test suite of the Katie Toolkit.
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

#ifndef QSTATICTEXT_P_H
#define QSTATICTEXT_P_H

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

#include "qstatictext.h"

#include "qfontengine_p.h"
#include <QtGui/qcolor.h>

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QStaticTextItem
{
public:
    QStaticTextItem() : chars(nullptr), numChars(0), m_fontEngine(nullptr) {}

    QStaticTextItem(const QStaticTextItem &other)
    {
        operator=(other);
    }

    void operator=(const QStaticTextItem &other)
    {
        glyphPositions = other.glyphPositions;
        glyphs = other.glyphs;
        chars = other.chars;
        numGlyphs = other.numGlyphs;
        numChars = other.numChars;
        font = other.font;
        color = other.color;

        m_fontEngine = nullptr;
        setFontEngine(other.fontEngine());
    }

    ~QStaticTextItem();

    void setFontEngine(QFontEngine *fe);
    QFontEngine *fontEngine() const { return m_fontEngine; }

    union {
        QFixedPoint *glyphPositions;             // 8 bytes per glyph
        int positionOffset;
    };
    union {
        glyph_t *glyphs;                         // 4 bytes per glyph
        int glyphOffset;
    };
    union {
        QChar *chars;                            // 2 bytes per glyph
        int charOffset;
    };
                                                 // =================
                                                 // 14 bytes per glyph

                                                 // 12 bytes for pointers
    int numGlyphs;                               // 4 bytes per item
    int numChars;                                // 4 bytes per item
    QFont font;                                  // 8 bytes per item
    QColor color;                                // 10 bytes per item
                                                 // ================
                                                 // 51 bytes per item

private: // Needs special handling in setters, so private to avoid abuse
    QFontEngine *m_fontEngine;                     // 4 bytes per item
};

class QStaticText;
class Q_AUTOTEST_EXPORT QStaticTextPrivate
{
public:
    QStaticTextPrivate();
    QStaticTextPrivate(const QStaticTextPrivate &other);
    ~QStaticTextPrivate();

    void init();
    void paintText(const QPointF &pos, QPainter *p);

    void invalidate()
    {
        needsRelayout = true;
    }

    QAtomicInt ref;                      // 4 bytes per text

    QString text;                        // 4 bytes per text
    QFont font;                          // 8 bytes per text
    qreal textWidth;                     // 8 bytes per text
    QSizeF actualSize;                   // 16 bytes per text
    QPointF position;                    // 16 bytes per text

    QTransform matrix;                   // 80 bytes per text
    QStaticTextItem *items;              // 4 bytes per text
    int itemCount;                       // 4 bytes per text

    glyph_t *glyphPool;                  // 4 bytes per text
    QFixedPoint *positionPool;           // 4 bytes per text
    QChar *charPool;                     // 4 bytes per text

    QTextOption textOption;              // 28 bytes per text

    bool needsRelayout;                  // 1 byte per text
    Qt::TextFormat textFormat;
    bool untransformedCoordinates;
                                         // ================
                                         // 195 bytes per text

    static QStaticTextPrivate *get(const QStaticText *q);
};

QT_END_NAMESPACE

#endif // QSTATICTEXT_P_H
