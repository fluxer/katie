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

#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
  class PreviewWidget;
}


class PreviewWidget : public QWidget
{
    Q_OBJECT

public:
    PreviewWidget(QWidget *parent = nullptr);
    ~PreviewWidget();

    bool eventFilter(QObject *, QEvent *);
private:
    void closeEvent(QCloseEvent *);
    Ui::PreviewWidget *ui;
};

QT_END_NAMESPACE

#endif // PREVIEWWIDGET_H
