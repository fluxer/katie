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

#ifndef QFOCUSFRAME_H
#define QFOCUSFRAME_H

#include <QtGui/qwidget.h>


QT_BEGIN_NAMESPACE


class QFocusFramePrivate;
class QStyleOption;

class Q_GUI_EXPORT QFocusFrame : public QWidget
{
    Q_OBJECT
public:
    QFocusFrame(QWidget *parent = nullptr);
    ~QFocusFrame();

    void setWidget(QWidget *widget);
    QWidget *widget() const;

protected:
    bool eventFilter(QObject *, QEvent *);
    void paintEvent(QPaintEvent *);
    void initStyleOption(QStyleOption *option) const;

private:
    Q_DECLARE_PRIVATE(QFocusFrame)
    Q_DISABLE_COPY(QFocusFrame)
};

QT_END_NAMESPACE


#endif // QFOCUSFRAME_H
