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

#ifndef QRUBBERBAND_H
#define QRUBBERBAND_H

#include <QtGui/qwidget.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_RUBBERBAND

class QRubberBandPrivate;
class QStyleOptionRubberBand;

class Q_GUI_EXPORT QRubberBand : public QWidget
{
    Q_OBJECT

public:
    enum Shape { Line, Rectangle };
    explicit QRubberBand(Shape, QWidget *parent = Q_NULLPTR);
    ~QRubberBand();

    Shape shape() const;

    inline void move(int x, int y);
    inline void move(const QPoint &p)
    { move(p.x(), p.y()); }
    inline void resize(int w, int h)
    { setGeometry(geometry().x(), geometry().y(), w, h); }
    inline void resize(const QSize &s)
    { resize(s.width(), s.height()); }

protected:
    void paintEvent(QPaintEvent *);
    void changeEvent(QEvent *);
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
    void moveEvent(QMoveEvent *);
    void initStyleOption(QStyleOptionRubberBand *option) const;

private:
    Q_DECLARE_PRIVATE(QRubberBand)
};

inline void QRubberBand::move(int ax, int ay)
{ setGeometry(ax, ay, width(), height()); }

#endif // QT_NO_RUBBERBAND

QT_END_NAMESPACE


#endif // QRUBBERBAND_H
