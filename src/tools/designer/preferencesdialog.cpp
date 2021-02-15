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

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "qdesigner_appearanceoptions.h"
#include "abstractoptionspage_p.h"
#include "abstractformeditor.h"

#include <QtGui/QFileDialog>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

PreferencesDialog::PreferencesDialog(QDesignerFormEditorInterface *core, QWidget *parentWidget) :
    QDialog(parentWidget),
    m_ui(new Ui_PreferencesDialog())
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    m_ui->setupUi(this);

    m_optionsPages = core->optionsPages();

    m_ui->m_optionTabWidget->clear();
    foreach (QDesignerOptionsPageInterface *optionsPage, m_optionsPages) {
        QWidget *page = optionsPage->createPage(this);
        m_ui->m_optionTabWidget->addTab(page, optionsPage->name());
        if (QDesignerAppearanceOptionsWidget *appearanceWidget = qobject_cast<QDesignerAppearanceOptionsWidget *>(page))
            connect(appearanceWidget, SIGNAL(uiModeChanged(bool)), this, SLOT(slotUiModeChanged(bool)));
    }

    connect(m_ui->m_dialogButtonBox, SIGNAL(rejected()), this, SLOT(slotRejected()));
    connect(m_ui->m_dialogButtonBox, SIGNAL(accepted()), this, SLOT(slotAccepted()));
    connect(applyButton(), SIGNAL(clicked()), this, SLOT(slotApply()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete m_ui;
}

QPushButton *PreferencesDialog::applyButton() const
{
    return m_ui->m_dialogButtonBox->button(QDialogButtonBox::Apply);
}

void PreferencesDialog::slotApply()
{
    foreach (QDesignerOptionsPageInterface *optionsPage, m_optionsPages)
        optionsPage->apply();
}

void PreferencesDialog::slotAccepted()
{
    slotApply();
    closeOptionPages();
    accept();
}

void PreferencesDialog::slotRejected()
{
    closeOptionPages();
    reject();
}

void PreferencesDialog::slotUiModeChanged(bool modified)
{
    // Cannot "apply" a ui mode change (destroy the dialogs parent)
    applyButton()->setEnabled(!modified);
}

void PreferencesDialog::closeOptionPages()
{
    foreach (QDesignerOptionsPageInterface *optionsPage, m_optionsPages)
        optionsPage->finish();
}

QT_END_NAMESPACE
#include "moc_preferencesdialog.h"
