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

#include "previewconfigurationwidget_p.h"
#include "ui_previewconfigurationwidget.h"
#include "previewmanager_p.h"
#include "abstractsettings_p.h"
#include "shared_settings_p.h"

#include <iconloader_p.h>
#include <stylesheeteditor_p.h>

#include <QtGui/QFileDialog>
#include <QtGui/QStyleFactory>
#include <QtGui/QMessageBox>
#include <QtCore/QPair>
#include <QtCore/QList>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QSharedData>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// ------------- PreviewConfigurationWidgetPrivate
class PreviewConfigurationWidget::PreviewConfigurationWidgetPrivate {
public:
    PreviewConfigurationWidgetPrivate(QDesignerFormEditorInterface *core, QGroupBox *g);

    void slotEditAppStyleSheet();

    void retrieveSettings();
    void storeSettings() const;

    QAbstractButton *appStyleSheetChangeButton() const { return m_ui.m_appStyleSheetChangeButton; }

    QDesignerFormEditorInterface *m_core;

private:
    PreviewConfiguration previewConfiguration() const;
    void setPreviewConfiguration(const PreviewConfiguration &pc);

    const QString m_defaultStyle;
    QGroupBox *m_parent;
    Ui_PreviewConfigurationWidget m_ui;
};

PreviewConfigurationWidget::PreviewConfigurationWidgetPrivate::PreviewConfigurationWidgetPrivate(
        QDesignerFormEditorInterface *core, QGroupBox *g) :
    m_core(core),
    m_defaultStyle(PreviewConfigurationWidget::tr("Default")),
    m_parent(g)
{
    m_ui.setupUi(g);
    // styles
    m_ui.m_styleCombo->setEditable(false);
    QStringList styleItems(m_defaultStyle);
    styleItems += QStyleFactory::keys();
    m_ui.m_styleCombo->addItems(styleItems);

    // sheet
    m_ui.m_appStyleSheetLineEdit->setTextPropertyValidationMode(qdesigner_internal::ValidationStyleSheet);
    m_ui.m_appStyleSheetClearButton->setIcon(qdesigner_internal::createIconSet(QString::fromUtf8("resetproperty.png")));
    QObject::connect(m_ui.m_appStyleSheetClearButton, SIGNAL(clicked()), m_ui.m_appStyleSheetLineEdit, SLOT(clear()));

    retrieveSettings();
}

PreviewConfiguration PreviewConfigurationWidget::PreviewConfigurationWidgetPrivate::previewConfiguration() const
{
    PreviewConfiguration  rc;
    QString style = m_ui.m_styleCombo->currentText();
    if (style == m_defaultStyle)
        style.clear();
    const QString applicationStyleSheet = m_ui.m_appStyleSheetLineEdit->text();
    return PreviewConfiguration(style, applicationStyleSheet);
}

void PreviewConfigurationWidget::PreviewConfigurationWidgetPrivate::setPreviewConfiguration(const PreviewConfiguration &pc)
{
    int styleIndex = m_ui.m_styleCombo->findText(pc.style());
    if (styleIndex == -1)
        styleIndex = m_ui.m_styleCombo->findText(m_defaultStyle);
    m_ui.m_styleCombo->setCurrentIndex(styleIndex);
    m_ui.m_appStyleSheetLineEdit->setText(pc.applicationStyleSheet());
}

void  PreviewConfigurationWidget::PreviewConfigurationWidgetPrivate::slotEditAppStyleSheet()
{
    StyleSheetEditorDialog dlg(m_core, m_parent, StyleSheetEditorDialog::ModeGlobal);
    dlg.setText(m_ui.m_appStyleSheetLineEdit->text());
    if (dlg.exec() == QDialog::Accepted)
        m_ui.m_appStyleSheetLineEdit->setText(dlg.text());
}

void PreviewConfigurationWidget::PreviewConfigurationWidgetPrivate::retrieveSettings()
{
    QDesignerSharedSettings settings(m_core);
    m_parent->setChecked(settings.isCustomPreviewConfigurationEnabled());
    setPreviewConfiguration(settings.customPreviewConfiguration());
}

void PreviewConfigurationWidget::PreviewConfigurationWidgetPrivate::storeSettings() const
{
    QDesignerSharedSettings settings(m_core);
    settings.setCustomPreviewConfigurationEnabled(m_parent->isChecked());
    settings.setCustomPreviewConfiguration(previewConfiguration());
}

// ------------- PreviewConfigurationWidget
PreviewConfigurationWidget::PreviewConfigurationWidget(QDesignerFormEditorInterface *core,
                                                       QWidget *parent) :
    QGroupBox(parent),
    m_impl(new PreviewConfigurationWidgetPrivate(core, this))
{
    connect(m_impl->appStyleSheetChangeButton(), SIGNAL(clicked()), this, SLOT(slotEditAppStyleSheet()));

    m_impl->retrieveSettings();
}

PreviewConfigurationWidget::~PreviewConfigurationWidget()
{
    delete m_impl;
}

void PreviewConfigurationWidget::saveState()
{
    m_impl->storeSettings();
}

void PreviewConfigurationWidget::slotEditAppStyleSheet()
{
    m_impl->slotEditAppStyleSheet();
}

}

QT_END_NAMESPACE
#include "moc_previewconfigurationwidget_p.h"
