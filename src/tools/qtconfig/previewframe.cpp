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

#include "previewframe.h"
#include "previewwidget.h"

#include <QBoxLayout>
#include <QPainter>
#include <QPen>
#include <QMdiSubWindow>

QT_BEGIN_NAMESPACE

PreviewFrame::PreviewFrame(QWidget *parent)
    : QFrame(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    setLineWidth(1);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->setMargin(0);
    previewWidget = new PreviewWidget;
    workspace = new Workspace(this);
    vbox->addWidget(workspace);
    previewWidget->setAutoFillBackground(true);
}

void PreviewFrame::setPreviewPalette(const QPalette &pal)
{
    previewWidget->setPalette(pal);
}

QString PreviewFrame::previewText() const
{
    return m_previewWindowText;
}

void PreviewFrame::setPreviewVisible(bool visible)
{
    previewWidget->parentWidget()->setVisible(visible);
    if (visible)
        m_previewWindowText = QLatin1String("The moose in the noose\nate the goose who was loose.");
    else
        m_previewWindowText = tr("Desktop settings will only take effect after an application restart.");
    workspace->viewport()->update();
}

Workspace::Workspace(PreviewFrame *parent)
    : QMdiArea(parent)
{
    previewFrame = parent;
    PreviewWidget *previewWidget = previewFrame->widget();
    QMdiSubWindow *frame = addSubWindow(previewWidget, Qt::Window);
    frame->move(10, 10);
    frame->show();
}

void Workspace::paintEvent(QPaintEvent *)
{
    QPainter p(viewport());
    p.fillRect(rect(), palette().color(backgroundRole()).dark());
    p.setPen(QPen(Qt::white));
    p.drawText(0, height() / 2,  width(), height(), Qt::AlignHCenter, previewFrame->previewText());
}

#include "moc_previewframe.h"

QT_END_NAMESPACE
