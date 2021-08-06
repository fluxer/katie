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

#ifndef QPLASTIQUESTYLE_H
#define QPLASTIQUESTYLE_H

#include <QtGui/qwindowsstyle.h>


QT_BEGIN_NAMESPACE


#if !defined(QT_NO_STYLE_PLASTIQUE)

class QPlastiqueStylePrivate;
class Q_GUI_EXPORT QPlastiqueStyle : public QWindowsStyle
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPlastiqueStyle)
public:
    QPlastiqueStyle();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = nullptr) const;
    void drawControl(ControlElement element, const QStyleOption *option,
                     QPainter *painter, const QWidget *widget) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const;

    QRect subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const;

    int styleHint(StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const;
    SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                                     const QPoint &pos, const QWidget *widget = nullptr) const;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const;

    void polish(QWidget *widget);
    void unpolish(QWidget *widget);

    QPalette standardPalette() const;

    int layoutSpacing(QSizePolicy::ControlType control1,
                      QSizePolicy::ControlType control2, 
                      Qt::Orientation orientation,
                      const QStyleOption *option = nullptr, 
                      const QWidget *widget = nullptr) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    Q_DISABLE_COPY(QPlastiqueStyle)
};

#endif // QT_NO_STYLE_PLASTIQUE

QT_END_NAMESPACE


#endif // QPLASTIQUESTYLE_H
