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

#ifndef QCOMMONSTYLE_H
#define QCOMMONSTYLE_H

#include <QtGui/qstyle.h>


QT_BEGIN_NAMESPACE

class QCommonStylePrivate;

class Q_GUI_EXPORT QCommonStyle: public QStyle
{
    Q_OBJECT
    
public:
    QCommonStyle();

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p,
                       const QWidget *w = nullptr) const;
    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                     const QWidget *w = nullptr) const;
    QRect subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr) const;
    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p,
                            const QWidget *w = nullptr) const;
    SubControl hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *opt,
                                     const QPoint &pt, const QWidget *w = nullptr) const;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc,
                         const QWidget *w = nullptr) const;
    QSize sizeFromContents(ContentsType ct, const QStyleOption *opt,
                           const QSize &contentsSize, const QWidget *widget = nullptr) const;

    int pixelMetric(PixelMetric m, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr) const;

    int styleHint(StyleHint sh, const QStyleOption *opt = nullptr, const QWidget *w = nullptr,
                  QStyleHintReturn *shret = nullptr) const;

    QPixmap standardPixmap(StandardPixmap sp, const QStyleOption *opt = nullptr,
                           const QWidget *widget = nullptr) const;

    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap,
                                const QStyleOption *opt) const;

    void unpolish(QWidget *widget);
    void unpolish(QApplication *application);

    virtual QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *opt = nullptr,
                       const QWidget *widget = nullptr) const;

protected:
    QCommonStyle(QCommonStylePrivate &dd);

private:
    Q_DECLARE_PRIVATE(QCommonStyle)
    Q_DISABLE_COPY(QCommonStyle)
};

QT_END_NAMESPACE


#endif // QCOMMONSTYLE_H
