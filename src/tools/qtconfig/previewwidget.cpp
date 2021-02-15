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

#include "previewwidget.h"
#include "ui_previewwidget.h"
#include <QCloseEvent>

QT_BEGIN_NAMESPACE

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PreviewWidget)
{
    ui->setupUi(this);

    // install event filter on child widgets
    QList<QWidget *> l = findChildren<QWidget *>();
    foreach(QWidget *w, l) {
        w->installEventFilter(this);
        w->setFocusPolicy(Qt::NoFocus);
    }
}

PreviewWidget::~PreviewWidget()
{
  delete ui;
}

bool PreviewWidget::eventFilter(QObject *, QEvent *e)
{
    switch (e->type()) {
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::Enter:
    case QEvent::Leave:
        return true; // ignore;
    default:
        break;
    }
    return false;
}

void PreviewWidget::closeEvent(QCloseEvent *e)
{
    e->ignore();
}

#include "moc_previewwidget.h"

QT_END_NAMESPACE
