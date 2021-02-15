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

#include "abstractformwindow.h"
#include "inplace_editor.h"

#include <QtDesigner/abstractformwindow.h>
#include <QtDesigner/abstractformwindowcursor.h>
#include <QtDesigner/propertysheet.h>
#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/abstractlanguage.h>
#include <QtDesigner/QExtensionManager>

#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// ----------------- InPlaceEditor

InPlaceEditor::InPlaceEditor(QWidget *widget,
                             TextPropertyValidationMode validationMode,
                             QDesignerFormWindowInterface *fw,
                             const QString& text,
                             const QRect& r) :
    TextPropertyEditor(widget, EmbeddingInPlace, validationMode),
    m_InPlaceWidgetHelper(this, widget, fw)
{
    setAlignment(m_InPlaceWidgetHelper.alignment());
    setObjectName(QLatin1String("__qt__passive_m_editor"));

    setText(text);
    selectAll();

    setGeometry(QRect(widget->mapTo(widget->window(), r.topLeft()), r.size()));
    setFocus();
    show();

    connect(this, SIGNAL(editingFinished()),this, SLOT(close()));
}


// -------------- TaskMenuInlineEditor

TaskMenuInlineEditor::TaskMenuInlineEditor(QWidget *w, TextPropertyValidationMode vm,
                                           const QString &property, QObject *parent) :
    QObject(parent),
    m_vm(vm),
    m_property(property),
    m_widget(w),
    m_managed(true)
{
}

void TaskMenuInlineEditor::editText()
{
    m_formWindow = QDesignerFormWindowInterface::findFormWindow(m_widget);
    if (m_formWindow.isNull())
        return;
    m_managed = m_formWindow->isManaged(m_widget);
    // Close as soon as a different widget is selected
    connect(m_formWindow, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

    // get old value
    QDesignerFormEditorInterface *core = m_formWindow->core();
    const QDesignerPropertySheetExtension *sheet = qt_extension<QDesignerPropertySheetExtension*>(core->extensionManager(), m_widget);
    const int index = sheet->indexOf(m_property);
    if (index == -1)
        return;
    m_value = qvariant_cast<PropertySheetStringValue>(sheet->property(index));
    const QString oldValue = m_value.value();

    m_editor = new InPlaceEditor(m_widget, m_vm, m_formWindow, oldValue, editRectangle());
    connect(m_editor, SIGNAL(textChanged(QString)), this, SLOT(updateText(QString)));
}

void TaskMenuInlineEditor::updateText(const QString &text)
{
    // In the [rare] event we are invoked on an unmanaged widget,
    // do not use the cursor selection
    m_value.setValue(text);
    if (m_managed) {
        m_formWindow->cursor()->setProperty(m_property, QVariant::fromValue(m_value));
    } else {
        m_formWindow->cursor()->setWidgetProperty(m_widget, m_property, QVariant::fromValue(m_value));
    }
}

void TaskMenuInlineEditor::updateSelection()
{
    if (m_editor)
        m_editor->deleteLater();
}

}

QT_END_NAMESPACE
#include "moc_inplace_editor.h"
