/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QSTATUSBAR_H
#define QSTATUSBAR_H

#include <QtGui/qwidget.h>


QT_BEGIN_NAMESPACE


#ifndef QT_NO_STATUSBAR

class QStatusBarPrivate;

class Q_GUI_EXPORT QStatusBar: public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool sizeGripEnabled READ isSizeGripEnabled WRITE setSizeGripEnabled)

public:
    explicit QStatusBar(QWidget* parent = Q_NULLPTR);
    virtual ~QStatusBar();

    void addWidget(QWidget *widget, int stretch = 0);
    int insertWidget(int index, QWidget *widget, int stretch = 0);
    void addPermanentWidget(QWidget *widget, int stretch = 0);
    int insertPermanentWidget(int index, QWidget *widget, int stretch = 0);
    void removeWidget(QWidget *widget);

    void setSizeGripEnabled(bool);
    bool isSizeGripEnabled() const;

    QString currentMessage() const;

public Q_SLOTS:
    void showMessage(const QString &text, int timeout = 0);
    void clearMessage();


Q_SIGNALS:
    void messageChanged(const QString &text);

protected:
    void showEvent(QShowEvent *);
    void paintEvent(QPaintEvent *);

    // ### Qt 5: consider making reformat() and hideOrShow() private
    void reformat();
    void hideOrShow();
    bool event(QEvent *);

private:
    Q_DISABLE_COPY(QStatusBar)
    Q_DECLARE_PRIVATE(QStatusBar)
};

#endif // QT_NO_STATUSBAR

QT_END_NAMESPACE


#endif // QSTATUSBAR_H
