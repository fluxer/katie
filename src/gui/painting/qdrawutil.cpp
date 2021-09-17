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

#include "qdrawutil.h"
#include "qbitmap.h"
#include "qpixmapcache.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qpalette.h"
#include "qpaintengineex_p.h"
#include "qmath.h"
#include "qstylehelper_p.h"

QT_BEGIN_NAMESPACE

/*!
    \headerfile <qdrawutil.h>
    \title Drawing Utility Functions

    \sa QPainter
*/

/*!
    \fn void qDrawShadeLine(QPainter *painter, int x1, int y1, int x2, int y2,
                     const QPalette &palette, bool sunken,
                     int lineWidth, int midLineWidth)
    \relates <qdrawutil.h>

    Draws a horizontal (\a y1 == \a y2) or vertical (\a x1 == \a x2)
    shaded line using the given \a painter.  Note that nothing is
    drawn if \a y1 != \a y2 and \a x1 != \a x2 (i.e. the line is
    neither horizontal nor vertical).

    The provided \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width. The given \a midLineWidth specifies the width of
    a middle line drawn in the QPalette::mid() color.

    The line appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style().  Use the drawing functions in QStyle to
    make widgets that follow the current GUI style.


    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded line:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 0

    \sa qDrawShadeRect(), qDrawShadePanel(), QStyle
*/

void qDrawShadeLine(QPainter *p, int x1, int y1, int x2, int y2,
                     const QPalette &pal, bool sunken,
                     int lineWidth, int midLineWidth)
{
    if (Q_UNLIKELY(!(p && lineWidth >= 0 && midLineWidth >= 0))) {
        qWarning("qDrawShadeLine: Invalid parameters");
        return;
    }
    int tlw = lineWidth*2 + midLineWidth;        // total line width
    QPen oldPen = p->pen();                        // save pen
    if (sunken)
        p->setPen(pal.color(QPalette::Dark));
    else
        p->setPen(pal.light().color());
    QPolygon a;
    int i;
    if (y1 == y2) {                                // horizontal line
        int y = y1 - tlw/2;
        if (x1 > x2) {                        // swap x1 and x2
            int t = x1;
            x1 = x2;
            x2 = t;
        }
        x2--;
        for (i=0; i<lineWidth; i++) {                // draw top shadow
            a.setPoints(3, x1+i, y+tlw-1-i,
                         x1+i, y+i,
                         x2-i, y+i);
            p->drawPolyline(a);
        }
        if (midLineWidth > 0) {
            p->setPen(pal.mid().color());
            for (i=0; i<midLineWidth; i++)        // draw lines in the middle
                p->drawLine(x1+lineWidth, y+lineWidth+i,
                             x2-lineWidth, y+lineWidth+i);
        }
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        for (i=0; i<lineWidth; i++) {                // draw bottom shadow
            a.setPoints(3, x1+i, y+tlw-i-1,
                         x2-i, y+tlw-i-1,
                         x2-i, y+i+1);
            p->drawPolyline(a);
        }
    }
    else if (x1 == x2) {                        // vertical line
        int x = x1 - tlw/2;
        if (y1 > y2) {                        // swap y1 and y2
            int t = y1;
            y1 = y2;
            y2 = t;
        }
        y2--;
        for (i=0; i<lineWidth; i++) {                // draw left shadow
            a.setPoints(3, x+i, y2,
                         x+i, y1+i,
                         x+tlw-1, y1+i);
            p->drawPolyline(a);
        }
        if (midLineWidth > 0) {
            p->setPen(pal.mid().color());
            for (i=0; i<midLineWidth; i++)        // draw lines in the middle
                p->drawLine(x+lineWidth+i, y1+lineWidth, x+lineWidth+i, y2);
        }
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        for (i=0; i<lineWidth; i++) {                // draw right shadow
            a.setPoints(3, x+lineWidth, y2-i,
                         x+tlw-i-1, y2-i,
                         x+tlw-i-1, y1+lineWidth);
            p->drawPolyline(a);
        }
    }
    p->setPen(oldPen);
}

/*!
    \fn void qDrawShadeRect(QPainter *painter, int x, int y, int width, int height,
                     const QPalette &palette, bool sunken,
                     int lineWidth, int midLineWidth,
                     const QBrush *fill)
    \relates <qdrawutil.h>

    Draws the shaded rectangle beginning at (\a x, \a y) with the
    given \a width and \a height using the provided \a painter.

    The provide \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors.  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width.  The \a midLineWidth specifies the width of a
    middle line drawn in the QPalette::mid() color.  The rectangle's
    interior is filled with the \a fill brush unless \a fill is 0.

    The rectangle appears sunken if \a sunken is true, otherwise
    raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded rectangle:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 1

    \sa qDrawShadeLine(), qDrawShadePanel(), qDrawPlainRect(), QStyle
*/

void qDrawShadeRect(QPainter *p, int x, int y, int w, int h,
                     const QPalette &pal, bool sunken,
                     int lineWidth, int midLineWidth,
                     const QBrush *fill)
{
    if (w == 0 || h == 0)
        return;
    if (Q_UNLIKELY(!(w > 0 && h > 0 && lineWidth >= 0 && midLineWidth >= 0))) {
        qWarning("qDrawShadeRect: Invalid parameters");
        return;
    }
    QPen oldPen = p->pen();
    if (sunken)
        p->setPen(pal.dark().color());
    else
        p->setPen(pal.light().color());
    int x1=x, y1=y, x2=x+w-1, y2=y+h-1;

    if (lineWidth == 1 && midLineWidth == 0) {// standard shade rectangle
        p->drawRect(x1, y1, w-2, h-2);
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        QLineF lines[4] = { QLineF(x1+1, y1+1, x2-2, y1+1),
                            QLineF(x1+1, y1+2, x1+1, y2-2),
                            QLineF(x1, y2, x2, y2),
                            QLineF(x2,y1, x2,y2-1) };
        p->drawLines(lines, 4);              // draw bottom/right lines
    } else {                                        // more complicated
        int m = lineWidth+midLineWidth;
        int i, j=0, k=m;
        for (i=0; i<lineWidth; i++) {                // draw top shadow
            QLineF lines[4] = { QLineF(x1+i, y2-i, x1+i, y1+i),
                                QLineF(x1+i, y1+i, x2-i, y1+i),
                                QLineF(x1+k, y2-k, x2-k, y2-k),
                                QLineF(x2-k, y2-k, x2-k, y1+k) };
            p->drawLines(lines, 4);
            k++;
        }
        p->setPen(pal.mid().color());
        j = lineWidth*2;
        for (i=0; i<midLineWidth; i++) {        // draw lines in the middle
            p->drawRect(x1+lineWidth+i, y1+lineWidth+i, w-j-1, h-j-1);
            j += 2;
        }
        if (sunken)
            p->setPen(pal.light().color());
        else
            p->setPen(pal.dark().color());
        k = m;
        for (i=0; i<lineWidth; i++) {                // draw bottom shadow
            QLineF lines[4] = { QLineF(x1+1+i, y2-i, x2-i, y2-i),
                                QLineF(x2-i, y2-i, x2-i, y1+i+1),
                                QLineF(x1+k, y2-k, x1+k, y1+k),
                                QLineF(x1+k, y1+k, x2-k, y1+k) };
            p->drawLines(lines, 4);
            k++;
        }
    }
    if (fill) {
        QBrush oldBrush = p->brush();
        int tlw = lineWidth + midLineWidth;
        p->setPen(Qt::NoPen);
        p->setBrush(*fill);
        p->drawRect(x+tlw, y+tlw, w-2*tlw, h-2*tlw);
        p->setBrush(oldBrush);
    }
    p->setPen(oldPen);                        // restore pen
}


/*!
    \fn void qDrawShadePanel(QPainter *painter, int x, int y, int width, int height,
                      const QPalette &palette, bool sunken,
                      int lineWidth, const QBrush *fill)
    \relates <qdrawutil.h>

    Draws the shaded panel beginning at (\a x, \a y) with the given \a
    width and \a height using the provided \a painter and the given \a
    lineWidth.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The panel's interior is filled
    with the \a fill brush unless \a fill is 0.

    The panel appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 2

    \sa qDrawWinPanel(), qDrawShadeLine(), qDrawShadeRect(), QStyle
*/

void qDrawShadePanel(QPainter *p, int x, int y, int w, int h,
                      const QPalette &pal, bool sunken,
                      int lineWidth, const QBrush *fill)
{
    if (w == 0 || h == 0)
        return;
    if (Q_UNLIKELY(!(w > 0 && h > 0 && lineWidth >= 0))) {
        qWarning("qDrawShadePanel: Invalid parameters");
    }
    QColor shade = pal.dark().color();
    QColor light = pal.light().color();
    if (fill) {
        if (fill->color() == shade)
            shade = pal.shadow().color();
        if (fill->color() == light)
            light = pal.midlight().color();
    }
    QPen oldPen = p->pen();                        // save pen
    QVector<QLineF> lines;
    lines.reserve(2*lineWidth);

    if (sunken)
        p->setPen(shade);
    else
        p->setPen(light);
    int x1, y1, x2, y2;
    int i;
    x1 = x;
    y1 = y2 = y;
    x2 = x+w-2;
    for (i=0; i<lineWidth; i++) {                // top shadow
        lines << QLineF(x1, y1++, x2--, y2++);
    }
    x2 = x1;
    y1 = y+h-2;
    for (i=0; i<lineWidth; i++) {                // left shado
        lines << QLineF(x1++, y1, x2++, y2--);
    }
    p->drawLines(lines);
    lines.clear();
    if (sunken)
        p->setPen(light);
    else
        p->setPen(shade);
    x1 = x;
    y1 = y2 = y+h-1;
    x2 = x+w-1;
    for (i=0; i<lineWidth; i++) {                // bottom shadow
        lines << QLineF(x1++, y1--, x2, y2--);
    }
    x1 = x2;
    y1 = y;
    y2 = y+h-lineWidth-1;
    for (i=0; i<lineWidth; i++) {                // right shadow
        lines << QLineF(x1--, y1++, x2--, y2);
    }
    p->drawLines(lines);
    if (fill)                                // fill with fill color
        p->fillRect(x+lineWidth, y+lineWidth, w-lineWidth*2, h-lineWidth*2, *fill);
    p->setPen(oldPen);                        // restore pen
}


/*!
  \internal
  This function draws a rectangle with two pixel line width.
  It is called from qDrawWinButton() and qDrawWinPanel().

  c1..c4 and fill are used:

    1 1 1 1 1 2
    1 3 3 3 4 2
    1 3 F F 4 2
    1 3 F F 4 2
    1 4 4 4 4 2
    2 2 2 2 2 2
*/

static void qDrawWinShades(QPainter *p,
                           int x, int y, int w, int h,
                           const QColor &c1, const QColor &c2,
                           const QColor &c3, const QColor &c4,
                           const QBrush *fill)
{
    if (w < 2 || h < 2)                        // can't do anything with that
        return;
    QPen oldPen = p->pen();
    QPoint a[3] = { QPoint(x, y+h-2), QPoint(x, y), QPoint(x+w-2, y) };
    p->setPen(c1);
    p->drawPolyline(a, 3);
    QPoint b[3] = { QPoint(x, y+h-1), QPoint(x+w-1, y+h-1), QPoint(x+w-1, y) };
    p->setPen(c2);
    p->drawPolyline(b, 3);
    if (w > 4 && h > 4) {
        QPoint c[3] = { QPoint(x+1, y+h-3), QPoint(x+1, y+1), QPoint(x+w-3, y+1) };
        p->setPen(c3);
        p->drawPolyline(c, 3);
        QPoint d[3] = { QPoint(x+1, y+h-2), QPoint(x+w-2, y+h-2), QPoint(x+w-2, y+1) };
        p->setPen(c4);
        p->drawPolyline(d, 3);
        if (fill)
            p->fillRect(QRect(x+2, y+2, w-4, h-4), *fill);
    }
    p->setPen(oldPen);
}


/*!
    \fn void qDrawWinButton(QPainter *painter, int x, int y, int width, int height,
                     const QPalette &palette, bool sunken,
                     const QBrush *fill)
    \relates <qdrawutil.h>

    Draws the Windows-style button specified by the given point (\a x,
    \a y}, \a width and \a height using the provided \a painter with a
    line width of 2 pixels. The button's interior is filled with the
    \a{fill} brush unless \a fill is 0.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).

    The button appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style()-> Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    \sa qDrawWinPanel(), QStyle
*/

void qDrawWinButton(QPainter *p, int x, int y, int w, int h,
                     const QPalette &pal, bool sunken,
                     const QBrush *fill)
{
    if (sunken)
        qDrawWinShades(p, x, y, w, h,
                       pal.shadow().color(), pal.light().color(), pal.dark().color(),
                       pal.button().color(), fill);
    else
        qDrawWinShades(p, x, y, w, h,
                       pal.light().color(), pal.shadow().color(), pal.button().color(),
                       pal.dark().color(), fill);
}

/*!
    \fn void qDrawWinPanel(QPainter *painter, int x, int y, int width, int height,
                    const QPalette &palette, bool        sunken,
                    const QBrush *fill)
    \relates <qdrawutil.h>

    Draws the Windows-style panel specified by the given point(\a x,
    \a y), \a width and \a height using the provided \a painter with a
    line width of 2 pixels. The button's interior is filled with the
    \a fill brush unless \a fill is 0.

    The given \a palette specifies the shading colors.  The panel
    appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 3

    \sa qDrawShadePanel(), qDrawWinButton(), QStyle
*/

void qDrawWinPanel(QPainter *p, int x, int y, int w, int h,
                    const QPalette &pal, bool        sunken,
                    const QBrush *fill)
{
    if (sunken)
        qDrawWinShades(p, x, y, w, h,
                        pal.dark().color(), pal.light().color(), pal.shadow().color(),
                       pal.midlight().color(), fill);
    else
        qDrawWinShades(p, x, y, w, h,
                       pal.light().color(), pal.shadow().color(), pal.midlight().color(),
                       pal.dark().color(), fill);
}

/*!
    \fn void qDrawPlainRect(QPainter *painter, int x, int y, int width, int height, const QColor &lineColor,
                     int lineWidth, const QBrush *fill)
    \relates <qdrawutil.h>

    Draws the plain rectangle beginning at (\a x, \a y) with the given
    \a width and \a height, using the specified \a painter, \a lineColor
    and \a lineWidth. The rectangle's interior is filled with the \a
    fill brush unless \a fill is 0.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a plain rectangle:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 4

    \sa qDrawShadeRect(), QStyle
*/

void qDrawPlainRect(QPainter *p, int x, int y, int w, int h, const QColor &c,
                     int lineWidth, const QBrush *fill)
{
    if (w == 0 || h == 0)
        return;
    if (Q_UNLIKELY(!(w > 0 && h > 0 && lineWidth >= 0))) {
        qWarning("qDrawPlainRect: Invalid parameters");
    }
    QPen   oldPen   = p->pen();
    QBrush oldBrush = p->brush();
    p->setPen(c);
    p->setBrush(Qt::NoBrush);
    for (int i=0; i<lineWidth; i++)
        p->drawRect(x+i, y+i, w-i*2 - 1, h-i*2 - 1);
    if (fill) {                                // fill with fill color
        p->setPen(Qt::NoPen);
        p->setBrush(*fill);
        p->drawRect(x+lineWidth, y+lineWidth, w-lineWidth*2, h-lineWidth*2);
    }
    p->setPen(oldPen);
    p->setBrush(oldBrush);
}

/*****************************************************************************
  Overloaded functions.
 *****************************************************************************/

/*!
    \fn void qDrawShadeLine(QPainter *painter, const QPoint &p1, const QPoint &p2,
             const QPalette &palette, bool sunken, int lineWidth, int midLineWidth)
    \relates <qdrawutil.h>
    \overload

    Draws a horizontal or vertical shaded line between \a p1 and \a p2
    using the given \a painter.  Note that nothing is drawn if the line
    between the points would be neither horizontal nor vertical.

    The provided \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width. The given \a midLineWidth specifies the width of
    a middle line drawn in the QPalette::mid() color.

    The line appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style().  Use the drawing functions in QStyle to
    make widgets that follow the current GUI style.


    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded line:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 5

    \sa qDrawShadeRect(), qDrawShadePanel(), QStyle
*/

/*!
    \fn void qDrawShadeRect(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, int lineWidth, int midLineWidth, const QBrush *fill)
    \relates <qdrawutil.h>
    \overload

    Draws the shaded rectangle specified by \a rect using the given \a painter.

    The provide \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors.  The given \a lineWidth
    specifies the line width for each of the lines; it is not the
    total line width.  The \a midLineWidth specifies the width of a
    middle line drawn in the QPalette::mid() color.  The rectangle's
    interior is filled with the \a fill brush unless \a fill is 0.

    The rectangle appears sunken if \a sunken is true, otherwise
    raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded rectangle:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 6

    \sa qDrawShadeLine(), qDrawShadePanel(), qDrawPlainRect(), QStyle
*/

/*!
    \fn void qDrawShadePanel(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, int lineWidth, const QBrush *fill)
    \relates <qdrawutil.h>
    \overload

    Draws the shaded panel at the rectangle specified by \a rect using the
    given \a painter and the given \a lineWidth.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).  The panel's interior is filled
    with the \a fill brush unless \a fill is 0.

    The panel appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 7

    \sa qDrawWinPanel(), qDrawShadeLine(), qDrawShadeRect(), QStyle
*/

/*!
    \fn void qDrawWinButton(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, const QBrush *fill)
    \relates <qdrawutil.h>
    \overload

    Draws the Windows-style button at the rectangle specified by \a rect using
    the given \a painter with a line width of 2 pixels. The button's interior
    is filled with the \a{fill} brush unless \a fill is 0.

    The given \a palette specifies the shading colors (\l
    {QPalette::light()}{light}, \l {QPalette::dark()}{dark} and \l
    {QPalette::mid()}{middle} colors).

    The button appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style()-> Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    \sa qDrawWinPanel(), QStyle
*/

/*!
    \fn void qDrawWinPanel(QPainter *painter, const QRect &rect, const QPalette &palette,
             bool sunken, const QBrush *fill)
    \overload

    Draws the Windows-style panel at the rectangle specified by \a rect using
    the given \a painter with a line width of 2 pixels. The button's interior
    is filled with the \a fill brush unless \a fill is 0.

    The given \a palette specifies the shading colors.  The panel
    appears sunken if \a sunken is true, otherwise raised.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a shaded panel:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 8

    \sa qDrawShadePanel(), qDrawWinButton(), QStyle
*/

/*!
    \fn void qDrawPlainRect(QPainter *painter, const QRect &rect, const QColor &lineColor, int lineWidth, const QBrush *fill)
    \relates <qdrawutil.h>
    \overload

    Draws the plain rectangle specified by \a rect using the given \a painter,
    \a lineColor and \a lineWidth. The rectangle's interior is filled with the
    \a fill brush unless \a fill is 0.

    \warning This function does not look at QWidget::style() or
    QApplication::style(). Use the drawing functions in QStyle to make
    widgets that follow the current GUI style.

    Alternatively you can use a QFrame widget and apply the
    QFrame::setFrameStyle() function to display a plain rectangle:

    \snippet doc/src/snippets/code/src_gui_painting_qdrawutil.cpp 9

    \sa qDrawShadeRect(), QStyle
*/

/*!
    \class QTileRules
    \since 4.6

    Holds the rules used to draw a pixmap or image split into nine segments,
    similar to \l{http://www.w3.org/TR/css3-background/}{CSS3 border-images}.

    \sa Qt::TileRule, QMargins
*/

/*! \fn QTileRules::QTileRules(Qt::TileRule horizontalRule, Qt::TileRule verticalRule)
  Constructs a QTileRules with the given \a horizontalRule and
  \a verticalRule.
 */

/*! \fn QTileRules::QTileRules(Qt::TileRule rule)
  Constructs a QTileRules with the given \a rule used for both
  the horizontal rule and the vertical rule.
 */

/*!
    \fn void qDrawBorderPixmap(QPainter *painter, const QRect &target, const QMargins &margins, const QPixmap &pixmap)
    \relates <qdrawutil.h>
    \since 4.6
    \overload

    \brief The qDrawBorderPixmap function is for drawing a pixmap into
    the margins of a rectangle.

    Draws the given \a pixmap into the given \a target rectangle, using the
    given \a painter. The pixmap will be split into nine segments and drawn
    according to the \a margins structure.
*/

/*!
    \since 4.6

    Draws the indicated \a sourceRect rectangle from the given \a pixmap into
    the given \a targetRect rectangle, using the given \a painter. The pixmap
    will be split into nine segments according to the given \a targetMargins
    and \a sourceMargins structures. Finally, the pixmap will be drawn
    according to the given \a rules.

    This function is used to draw a scaled pixmap, similar to
    \l{http://www.w3.org/TR/css3-background/}{CSS3 border-images}

    \sa Qt::TileRule, QTileRules, QMargins
*/

void qDrawBorderPixmap(QPainter *painter, const QRect &targetRect, const QMargins &targetMargins,
                       const QPixmap &pixmap, const QRect &sourceRect,const QMargins &sourceMargins,
                       const QTileRules &rules)
{
    if (Q_UNLIKELY(!painter->isActive())) {
        qWarning("qDrawBorderPixmap: Painter not active");
        return;
    } else if (pixmap.isNull()) {
        // there is nothing to draw to
        return;
    }

    // source center
    const int sourceCenterTop = sourceRect.top() + sourceMargins.top();
    const int sourceCenterLeft = sourceRect.left() + sourceMargins.left();
    const int sourceCenterBottom = sourceRect.bottom() - sourceMargins.bottom() + 1;
    const int sourceCenterRight = sourceRect.right() - sourceMargins.right() + 1;
    const int sourceCenterWidth = sourceCenterRight - sourceCenterLeft;
    const int sourceCenterHeight = sourceCenterBottom - sourceCenterTop;
    // target center
    const int targetCenterTop = targetRect.top() + targetMargins.top();
    const int targetCenterLeft = targetRect.left() + targetMargins.left();
    const int targetCenterBottom = targetRect.bottom() - targetMargins.bottom() + 1;
    const int targetCenterRight = targetRect.right() - targetMargins.right() + 1;
    const int targetCenterWidth = targetCenterRight - targetCenterLeft;
    const int targetCenterHeight = targetCenterBottom - targetCenterTop;

    int columns = 3;
    int rows = 3;
    if (rules.horizontal != Qt::StretchTile && sourceCenterWidth != 0)
        columns = qMax(3, 2 + qCeil(targetCenterWidth / qreal(sourceCenterWidth)));
    if (rules.vertical != Qt::StretchTile && sourceCenterHeight != 0)
        rows = qMax(3, 2 + qCeil(targetCenterHeight / qreal(sourceCenterHeight)));


    qreal xTarget[columns + 1]; // x-coordinates of target rectangles
    qreal yTarget[rows + 1]; // y-coordinates of target rectangles

    xTarget[0] = targetRect.left();
    xTarget[1] = targetCenterLeft;
    xTarget[columns - 1] = targetCenterRight;
    xTarget[columns] = targetRect.left() + targetRect.width();

    yTarget[0] = targetRect.top();
    yTarget[1] = targetCenterTop;
    yTarget[rows - 1] = targetCenterBottom;
    yTarget[rows] = targetRect.top() + targetRect.height();

    qreal dx = 0;
    qreal dy = 0;

    switch (rules.horizontal) {
    case Qt::StretchTile:
        dx = targetCenterWidth;
        break;
    case Qt::RepeatTile:
        dx = sourceCenterWidth;
        break;
    case Qt::RoundTile:
        dx = targetCenterWidth / qreal(columns - 2);
        break;
    }

    for (int i = 2; i < columns - 1; ++i)
        xTarget[i] = xTarget[i - 1] + dx;

    switch (rules.vertical) {
    case Qt::StretchTile:
        dy = targetCenterHeight;
        break;
    case Qt::RepeatTile:
        dy = sourceCenterHeight;
        break;
    case Qt::RoundTile:
        dy = targetCenterHeight / qreal(rows - 2);
        break;
    }

    for (int i = 2; i < rows - 1; ++i)
        yTarget[i] = yTarget[i - 1] + dy;

    // corners
    if (targetMargins.top() > 0 && targetMargins.left() > 0 && sourceMargins.top() > 0 && sourceMargins.left() > 0) { // top left
        const QRectF sourceData = QRectF(sourceRect.topLeft(), QSizeF(sourceMargins.left(), sourceMargins.top()));
        const QRectF targetData = QRectF(QPointF(xTarget[0], yTarget[0]), QPointF(xTarget[1], yTarget[1]));
        painter->drawPixmap(targetData, pixmap, sourceData);
    }
    if (targetMargins.top() > 0 && targetMargins.right() > 0 && sourceMargins.top() > 0 && sourceMargins.right() > 0) { // top right
        const QRectF sourceData = QRectF(QPointF(sourceCenterRight, sourceRect.top()), QSizeF(sourceMargins.right(), sourceMargins.top()));
        const QRectF targetData = QRectF(QPointF(xTarget[columns-1], yTarget[0]), QPointF(xTarget[columns], yTarget[1]));
        painter->drawPixmap(targetData, pixmap, sourceData);
    }
    if (targetMargins.bottom() > 0 && targetMargins.left() > 0 && sourceMargins.bottom() > 0 && sourceMargins.left() > 0) { // bottom left
        const QRectF sourceData = QRectF(QPointF(sourceRect.left(), sourceCenterBottom), QSizeF(sourceMargins.left(), sourceMargins.bottom()));
        const QRectF targetData = QRectF(QPointF(xTarget[0], yTarget[rows - 1]), QPointF(xTarget[1], yTarget[rows]));
        painter->drawPixmap(targetData, pixmap, sourceData);
    }
    if (targetMargins.bottom() > 0 && targetMargins.right() > 0 && sourceMargins.bottom() > 0 && sourceMargins.right() > 0) { // bottom right
        const QRectF sourceData = QRectF(QPointF(sourceCenterRight, sourceCenterBottom), QSizeF(sourceMargins.right(), sourceMargins.bottom()));
        const QRectF targetData = QRectF(QPointF(xTarget[columns - 1], yTarget[rows - 1]), QPointF(xTarget[columns], yTarget[rows]));
        painter->drawPixmap(targetData, pixmap, sourceData);
    }

    // horizontal edges
    if (targetCenterWidth > 0 && sourceCenterWidth > 0) {
        if (targetMargins.top() > 0 && sourceMargins.top() > 0) { // top
            for (int i = 1; i < columns - 1; ++i) {
                QRectF sourceData;
                if (rules.horizontal == Qt::RepeatTile && i == columns - 2) {
                    sourceData = QRectF(QPointF(sourceCenterLeft, sourceRect.top()), QSizeF(xTarget[i + 1] - xTarget[i], sourceMargins.top()));
                } else {
                    sourceData = QRectF(QPointF(sourceCenterLeft, sourceRect.top()), QSizeF(sourceCenterWidth, sourceMargins.top()));
                }
                const QRectF targetData = QRectF(QPointF(xTarget[i], yTarget[0]), QPointF(xTarget[i + 1], yTarget[1]));
                painter->drawPixmap(targetData, pixmap, sourceData);
            }
        }
        if (targetMargins.bottom() > 0 && sourceMargins.bottom() > 0) { // bottom
            for (int i = 1; i < columns - 1; ++i) {
                QRectF sourceData;
                if (rules.horizontal == Qt::RepeatTile && i == columns - 2) {
                    sourceData = QRectF(QPointF(sourceCenterLeft, sourceCenterBottom), QSizeF(xTarget[i + 1] - xTarget[i], sourceMargins.bottom()));
                } else {
                    sourceData = QRectF(QPointF(sourceCenterLeft, sourceCenterBottom), QSizeF(sourceCenterWidth, sourceMargins.bottom()));
                }
                const QRectF targetData = QRectF(QPointF(xTarget[i], yTarget[rows - 1]), QPointF(xTarget[i + 1], yTarget[rows]));
                painter->drawPixmap(targetData, pixmap, sourceData);
            }
        }
    }

    // vertical edges
    if (targetCenterHeight > 0 && sourceCenterHeight > 0) {
        if (targetMargins.left() > 0 && sourceMargins.left() > 0) { // left
            for (int i = 1; i < rows - 1; ++i) {
                QRectF sourceData;
                if (rules.vertical == Qt::RepeatTile && i == rows - 2) {
                    sourceData = QRectF(QPointF(sourceRect.left(), sourceCenterTop), QSizeF(sourceMargins.left(), yTarget[i + 1] - yTarget[i]));
                } else {
                    sourceData = QRectF(QPointF(sourceRect.left(), sourceCenterTop), QSizeF(sourceMargins.left(), sourceCenterHeight));
                }
                const QRectF targetData = QRectF(QPointF(xTarget[0], yTarget[i]), QPointF(xTarget[1], yTarget[i + 1]));
                painter->drawPixmap(targetData, pixmap, sourceData);
            }
        }
        if (targetMargins.right() > 0 && sourceMargins.right() > 0) { // right
            for (int i = 1; i < rows - 1; ++i) {
                QRectF sourceData;
                if (rules.vertical == Qt::RepeatTile && i == rows - 2) {
                    sourceData = QRectF(QPointF(sourceCenterRight, sourceCenterTop), QSizeF(sourceMargins.right(), yTarget[i + 1] - yTarget[i]));
                } else {
                    sourceData = QRectF(QPointF(sourceCenterRight, sourceCenterTop), QSizeF(sourceMargins.right(), sourceCenterHeight));
                }
                const QRectF targetData = QRectF(QPointF(xTarget[columns - 1], yTarget[i]), QPointF(xTarget[columns], yTarget[i + 1]));
                painter->drawPixmap(targetData, pixmap, sourceData);
            }
        }
    }

    // center
    if (targetCenterWidth > 0 && targetCenterHeight > 0 && sourceCenterWidth > 0 && sourceCenterHeight > 0) {
        for (int j = 1; j < rows - 1; ++j) {
            qreal sourceHeight = (rules.vertical == Qt::RepeatTile && j == rows - 2) ? yTarget[j + 1] - yTarget[j] : sourceCenterHeight;
            for (int i = 1; i < columns - 1; ++i) {
                qreal sourceWidth = (rules.horizontal == Qt::RepeatTile && i == columns - 2) ? xTarget[i + 1] - xTarget[i] : sourceCenterWidth;
                const QRectF sourceData = QRectF(QPointF(sourceCenterLeft, sourceCenterTop), QSizeF(sourceWidth, sourceHeight));
                const QRectF targetData = QRectF(QPointF(xTarget[i], yTarget[j]), QPointF(xTarget[i + 1], yTarget[j + 1]));
                painter->drawPixmap(targetData, pixmap, sourceData);
            }
        }
    }
}

QT_END_NAMESPACE




