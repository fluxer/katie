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

#ifndef QCHECKBOX_H
#define QCHECKBOX_H

#include <QtGui/qabstractbutton.h>


QT_BEGIN_NAMESPACE


class QCheckBoxPrivate;
class QStyleOptionButton;

class Q_GUI_EXPORT QCheckBox : public QAbstractButton
{
    Q_OBJECT

    Q_PROPERTY(bool tristate READ isTristate WRITE setTristate)

public:
    explicit QCheckBox(QWidget *parent = nullptr);
    explicit QCheckBox(const QString &text, QWidget *parent = nullptr);


    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setTristate(bool y = true);
    bool isTristate() const;

    Qt::CheckState checkState() const;
    void setCheckState(Qt::CheckState state);

Q_SIGNALS:
    void stateChanged(int);

protected:
    bool event(QEvent *e);
    bool hitButton(const QPoint &pos) const;
    void checkStateSet();
    void nextCheckState();
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void initStyleOption(QStyleOptionButton *option) const;


private:
    Q_DECLARE_PRIVATE(QCheckBox)
    Q_DISABLE_COPY(QCheckBox)
};

QT_END_NAMESPACE


#endif // QCHECKBOX_H
