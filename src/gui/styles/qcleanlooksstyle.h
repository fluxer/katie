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

#ifndef QCLEANLOOKSSTYLE_H
#define QCLEANLOOKSSTYLE_H

#include <QtGui/qwindowsstyle.h>


QT_BEGIN_NAMESPACE


#if !defined(QT_NO_STYLE_CLEANLOOKS)

class QCleanlooksStylePrivate;
class Q_GUI_EXPORT QCleanlooksStyle : public QWindowsStyle
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCleanlooksStyle)

public:
    QCleanlooksStyle();

    QPalette standardPalette () const;
    void drawPrimitive(PrimitiveElement elem,
                        const QStyleOption *option,
                        QPainter *painter, const QWidget *widget = nullptr) const;
    void drawControl(ControlElement ce, const QStyleOption *option, QPainter *painter,
                                const QWidget *widget) const;
    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const;
    QRect subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr) const;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const;
    int styleHint(StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const;
    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = nullptr) const;
    void drawItemText(QPainter *painter, const QRect &rect,
                              int flags, const QPalette &pal, bool enabled,
                              const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const;
    void polish(QWidget *widget);
    void polish(QApplication *app);
    void polish(QPalette &pal);
    void unpolish(QWidget *widget);
    void unpolish(QApplication *app);

protected:
    QCleanlooksStyle(QCleanlooksStylePrivate &dd);
};

#endif // QT_NO_STYLE_CLEANLOOKS

QT_END_NAMESPACE


#endif // QCLEANLOOKSSTYLE_H
