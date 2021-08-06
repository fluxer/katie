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

#ifndef QMOTIFSTYLE_H
#define QMOTIFSTYLE_H

#include <QtGui/qcommonstyle.h>


QT_BEGIN_NAMESPACE


#if !defined(QT_NO_STYLE_MOTIF)

class QPalette;
class QFocusFrame;

class QMotifStylePrivate;
class Q_GUI_EXPORT QMotifStyle : public QCommonStyle
{
    Q_OBJECT
public:
    explicit QMotifStyle(bool useHighlightCols = false);

    void setUseHighlightColors(bool);
    bool useHighlightColors() const;

    void polish(QPalette&);
    void polish(QWidget*);
    void unpolish(QWidget*);
    void polish(QApplication*);
    void unpolish(QApplication*);

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p,
                        const QWidget *w = nullptr) const;

    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                      const QWidget *w = nullptr) const;

    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p,
                            const QWidget *w = nullptr) const;

    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget = nullptr) const;

    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr,
                     const QWidget *widget = nullptr) const;

    QSize sizeFromContents(ContentsType ct, const QStyleOption *opt,
                           const QSize &contentsSize, const QWidget *widget = nullptr) const;

    QRect subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr) const;

    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = nullptr) const;

    int styleHint(StyleHint hint, const QStyleOption *opt = nullptr, const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const;

    bool event(QEvent *);
    QPalette standardPalette() const;

protected:
    QMotifStyle(QMotifStylePrivate &dd);
    void timerEvent(QTimerEvent *event);
    bool eventFilter(QObject *o, QEvent *e);

private:
    Q_DECLARE_PRIVATE(QMotifStyle)
    Q_DISABLE_COPY(QMotifStyle)
};

#endif // QT_NO_STYLE_MOTIF

QT_END_NAMESPACE


#endif // QMOTIFSTYLE_H
