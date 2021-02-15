/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the tools applications of the Katie Toolkit.
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

#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QAbstractButton>

QT_BEGIN_NAMESPACE

class ColorButton : public QAbstractButton
{
    Q_OBJECT

public:
    ColorButton(QWidget *);
    ColorButton(const QColor &, QWidget *);

    const QColor &color() const { return col; }

    void setColor(const QColor &);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

signals:
    void colorChanged(const QColor &);

protected:
    void paintEvent(QPaintEvent *);
    void drawButton(QPainter *);
    void drawButtonLabel(QPainter *);

private slots:
    void changeColor();


private:
    QColor col;
    QPoint presspos;
    bool mousepressed;
};

QT_END_NAMESPACE

#endif // COLORBUTTON_H
