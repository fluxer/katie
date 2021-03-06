/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the Katie Designer of the Katie Toolkit.
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

#include "gridpanel_p.h"
#include "ui_gridpanel.h"
#include "grid_p.h"

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

GridPanel::GridPanel(QWidget *parentWidget) :
    QWidget(parentWidget)
{
    m_ui = new Ui_GridPanel;
    m_ui->setupUi(this);

    connect(m_ui->m_resetButton, SIGNAL(clicked()), this, SLOT(reset()));
}

GridPanel::~GridPanel()
{
    delete m_ui;
}

void GridPanel::setGrid(const Grid &g)
{
    m_ui->m_deltaXSpinBox->setValue(g.deltaX());
    m_ui->m_deltaYSpinBox->setValue(g.deltaY());
    m_ui->m_visibleCheckBox->setCheckState(g.visible() ? Qt::Checked : Qt::Unchecked);
    m_ui->m_snapXCheckBox->setCheckState(g.snapX()  ? Qt::Checked : Qt::Unchecked);
    m_ui->m_snapYCheckBox->setCheckState(g.snapY()  ? Qt::Checked : Qt::Unchecked);
}

void GridPanel::setTitle(const QString &title)
{
    m_ui->m_gridGroupBox->setTitle(title);
}

Grid GridPanel::grid() const
{
    Grid rc;
    rc.setDeltaX(m_ui->m_deltaXSpinBox->value());
    rc.setDeltaY(m_ui->m_deltaYSpinBox->value());
    rc.setSnapX(m_ui->m_snapXCheckBox->checkState() == Qt::Checked);
    rc.setSnapY(m_ui->m_snapYCheckBox->checkState() == Qt::Checked);
    rc.setVisible(m_ui->m_visibleCheckBox->checkState() == Qt::Checked);
    return rc;
}

void GridPanel::reset()
{
    setGrid(Grid());
}

void GridPanel::setCheckable (bool c)
{
    m_ui->m_gridGroupBox->setCheckable(c);
}

bool GridPanel::isCheckable () const
{
    return m_ui->m_gridGroupBox->isCheckable ();
}

bool GridPanel::isChecked () const
{
    return m_ui->m_gridGroupBox->isChecked ();
}

void GridPanel::setChecked(bool c)
{
    m_ui->m_gridGroupBox->setChecked(c);
}

void GridPanel::setResetButtonVisible(bool v)
{
    m_ui->m_resetButton->setVisible(v);
}

}

QT_END_NAMESPACE
#include "moc_gridpanel_p.h"
