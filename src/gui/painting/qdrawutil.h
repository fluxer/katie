/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDRAWUTIL_H
#define QDRAWUTIL_H

#include <QtCore/qnamespace.h>
#include <QtCore/qmargins.h>
#include <QtGui/qpixmap.h>


QT_BEGIN_NAMESPACE

class QPainter;
class QPalette;
class QPoint;
class QColor;
class QBrush;
class QRect;

//
// Standard shade drawing
//

Q_GUI_EXPORT void qDrawShadeLine(QPainter *p, int x1, int y1, int x2, int y2,
                              const QPalette &pal, bool sunken = true,
                              int lineWidth = 1, int midLineWidth = 0);

inline void qDrawShadeLine(QPainter *p, const QPoint &p1, const QPoint &p2,
                              const QPalette &pal, bool sunken = true,
                              int lineWidth = 1, int midLineWidth = 0)
{
    qDrawShadeLine(p, p1.x(), p1.y(), p2.x(), p2.y(), pal, sunken,
                    lineWidth, midLineWidth);
}

Q_GUI_EXPORT void qDrawShadeRect(QPainter *p, int x, int y, int w, int h,
                              const QPalette &pal, bool sunken = false,
                              int lineWidth = 1, int midLineWidth = 0,
                              const QBrush *fill = Q_NULLPTR);

inline void qDrawShadeRect(QPainter *p, const QRect &r,
                              const QPalette &pal, bool sunken = false,
                              int lineWidth = 1, int midLineWidth = 0,
                              const QBrush *fill = Q_NULLPTR)
{
    qDrawShadeRect(p, r.x(), r.y(), r.width(), r.height(), pal, sunken,
                    lineWidth, midLineWidth, fill);
}

Q_GUI_EXPORT void qDrawShadePanel(QPainter *p, int x, int y, int w, int h,
                               const QPalette &pal, bool sunken = false,
                               int lineWidth = 1, const QBrush *fill = Q_NULLPTR);

inline void qDrawShadePanel(QPainter *p, const QRect &r,
                               const QPalette &pal, bool sunken = false,
                               int lineWidth = 1, const QBrush *fill = Q_NULLPTR)
{
    qDrawShadePanel(p, r.x(), r.y(), r.width(), r.height(), pal, sunken,
                     lineWidth, fill);
}

Q_GUI_EXPORT void qDrawWinButton(QPainter *p, int x, int y, int w, int h,
                              const QPalette &pal, bool sunken = false,
                              const QBrush *fill = Q_NULLPTR);

inline void qDrawWinButton(QPainter *p, const QRect &r,
                              const QPalette &pal, bool sunken = false,
                              const QBrush *fill = Q_NULLPTR)
{
    qDrawWinButton(p, r.x(), r.y(), r.width(), r.height(), pal, sunken, fill);
}

Q_GUI_EXPORT void qDrawWinPanel(QPainter *p, int x, int y, int w, int h,
                              const QPalette &pal, bool sunken = false,
                             const QBrush *fill = Q_NULLPTR);

inline void qDrawWinPanel(QPainter *p, const QRect &r,
                              const QPalette &pal, bool sunken = false,
                             const QBrush *fill = Q_NULLPTR)
{
    qDrawWinPanel(p, r.x(), r.y(), r.width(), r.height(), pal, sunken, fill);
}

Q_GUI_EXPORT void qDrawPlainRect(QPainter *p, int x, int y, int w, int h,
                              const QColor &c,
                              int lineWidth = 1, const QBrush *fill = Q_NULLPTR);

inline void qDrawPlainRect(QPainter *p, const QRect &r, const QColor &c,
                              int lineWidth = 1, const QBrush *fill = Q_NULLPTR)
{
    qDrawPlainRect(p, r.x(), r.y(), r.width(), r.height(), c, lineWidth, fill);
}


struct QTileRules
{
    inline QTileRules(Qt::TileRule horizontalRule, Qt::TileRule verticalRule)
            : horizontal(horizontalRule), vertical(verticalRule) {}
    inline QTileRules(Qt::TileRule rule = Qt::StretchTile)
            : horizontal(rule), vertical(rule) {}
    Qt::TileRule horizontal;
    Qt::TileRule vertical;
};

Q_GUI_EXPORT void qDrawBorderPixmap(QPainter *painter,
                                    const QRect &targetRect,
                                    const QMargins &targetMargins,
                                    const QPixmap &pixmap,
                                    const QRect &sourceRect,
                                    const QMargins &sourceMargins,
                                    const QTileRules &rules = QTileRules()
                                    );

inline void qDrawBorderPixmap(QPainter *painter,
                                           const QRect &target,
                                           const QMargins &margins,
                                           const QPixmap &pixmap)
{
    qDrawBorderPixmap(painter, target, margins, pixmap, pixmap.rect(), margins);
}

QT_END_NAMESPACE


#endif // QDRAWUTIL_H
