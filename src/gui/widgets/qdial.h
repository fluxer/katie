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


#ifndef QDIAL_H
#define QDIAL_H

#include <QtGui/qabstractslider.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_DIAL

class QDialPrivate;
class QStyleOptionSlider;

class Q_GUI_EXPORT QDial: public QAbstractSlider
{
    Q_OBJECT

    Q_PROPERTY(bool wrapping READ wrapping WRITE setWrapping)
    Q_PROPERTY(int notchSize READ notchSize)
    Q_PROPERTY(qreal notchTarget READ notchTarget WRITE setNotchTarget)
    Q_PROPERTY(bool notchesVisible READ notchesVisible WRITE setNotchesVisible)
public:
    explicit QDial(QWidget *parent = nullptr);

    ~QDial();

    bool wrapping() const;

    int notchSize() const;

    void setNotchTarget(double target);
    qreal notchTarget() const;
    bool notchesVisible() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public Q_SLOTS:
    void setNotchesVisible(bool visible);
    void setWrapping(bool on);

protected:
    void paintEvent(QPaintEvent *pe);

    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);

    void initStyleOption(QStyleOptionSlider *option) const;


private:
    Q_DECLARE_PRIVATE(QDial)
    Q_DISABLE_COPY(QDial)
};

#endif  // QT_NO_DIAL

QT_END_NAMESPACE


#endif // QDIAL_H
