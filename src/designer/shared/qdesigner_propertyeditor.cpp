/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2020 Ivailo Monev
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

#include "qdesigner_propertyeditor_p.h"
#include "pluginmanager_p.h"

#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/dynamicpropertysheet.h>
#include <QtDesigner/propertysheet.h>
#include <QtDesigner/QExtensionManager>
#include <widgetfactory_p.h>
#include <QtGui/QAction>
#include <QtGui/QLineEdit>
#include <QtGui/QAbstractButton>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {
typedef QDesignerPropertyEditor::StringPropertyParameters StringPropertyParameters;
// A map of property name to type
typedef QHash<QString, StringPropertyParameters> PropertyNameTypeMap;

// Compile a map of hard-coded string property types
static const PropertyNameTypeMap &stringPropertyTypes()
{
    static PropertyNameTypeMap propertyNameTypeMap;
    if (propertyNameTypeMap.empty()) {
        const StringPropertyParameters richtext(ValidationRichText, true);
        // Accessibility. Both are texts the narrator reads
        propertyNameTypeMap.insert(QLatin1String("accessibleDescription"), richtext);
        propertyNameTypeMap.insert(QLatin1String("accessibleName"), richtext);
        // object names
        const StringPropertyParameters objectName(ValidationObjectName, false);
        propertyNameTypeMap.insert(QLatin1String("buddy"), objectName);
        propertyNameTypeMap.insert(QLatin1String("currentItemName"), objectName);
        propertyNameTypeMap.insert(QLatin1String("currentPageName"), objectName);
        propertyNameTypeMap.insert(QLatin1String("currentTabName"), objectName);
        propertyNameTypeMap.insert(QLatin1String("layoutName"), objectName);
        propertyNameTypeMap.insert(QLatin1String("spacerName"), objectName);
        // Style sheet
        propertyNameTypeMap.insert(QLatin1String("styleSheet"), StringPropertyParameters(ValidationStyleSheet, false));
        // Buttons/  QCommandLinkButton
        const StringPropertyParameters multiline(ValidationMultiLine, true);
        propertyNameTypeMap.insert(QLatin1String("description"), multiline);
        propertyNameTypeMap.insert(QLatin1String("iconText"), multiline);
        // Tooltips, etc.
        propertyNameTypeMap.insert(QLatin1String("toolTip"), richtext);
        propertyNameTypeMap.insert(QLatin1String("whatsThis"), richtext);
        propertyNameTypeMap.insert(QLatin1String("windowIconText"), richtext);
        propertyNameTypeMap.insert(QLatin1String("html"), richtext);
        //  A QWizard page id
        propertyNameTypeMap.insert(QLatin1String("pageId"), StringPropertyParameters(ValidationSingleLine, false));
        // QPlainTextEdit
        propertyNameTypeMap.insert(QLatin1String("plainText"), StringPropertyParameters(ValidationMultiLine, true));
    }
    return propertyNameTypeMap;
}

QDesignerPropertyEditor::QDesignerPropertyEditor(QWidget *parent, Qt::WindowFlags flags) :
    QDesignerPropertyEditorInterface(parent, flags),
    m_propertyChangedForwardingBlocked(false)
{
    // Make old signal work for  compatibility
    connect(this, SIGNAL(propertyChanged(QString,QVariant)), this, SLOT(slotPropertyChanged(QString,QVariant)));
}

static inline bool isDynamicProperty(QDesignerFormEditorInterface *core, QObject *object,
                                     const QString &propertyName)
{
    if (const QDesignerDynamicPropertySheetExtension *dynamicSheet = qt_extension<QDesignerDynamicPropertySheetExtension*>(core->extensionManager(), object)) {
        if (dynamicSheet->dynamicPropertiesAllowed()) {
            if (QDesignerPropertySheetExtension *propertySheet = qt_extension<QDesignerPropertySheetExtension*>(core->extensionManager(), object)) {
                const int index = propertySheet->indexOf(propertyName);
                return index >= 0 && dynamicSheet->isDynamicProperty(index);
            }
        }
    }
    return false;
}

QDesignerPropertyEditor::StringPropertyParameters QDesignerPropertyEditor::textPropertyValidationMode(
        QDesignerFormEditorInterface *core, const QObject *object,
        const QString &propertyName, bool isMainContainer)
{
    // object name - no comment
    if (propertyName == QLatin1String("objectName")) {
        const TextPropertyValidationMode vm =  isMainContainer ? ValidationObjectNameScope : ValidationObjectName;
        return StringPropertyParameters(vm, false);
    }

    // Check custom widgets by class.
    const QString className = WidgetFactory::classNameOf(core, object);
    const QDesignerCustomWidgetData customData = core->pluginManager()->customWidgetData(className);
    if (!customData.isNull()) {
        StringPropertyParameters customType;
        if (customData.xmlStringPropertyType(propertyName, &customType))
            return customType;
    }

    if (isDynamicProperty(core, const_cast<QObject *>(object), propertyName))
        return StringPropertyParameters(ValidationMultiLine, true);

    // Check hardcoded property ames
   const PropertyNameTypeMap::const_iterator hit = stringPropertyTypes().constFind(propertyName);
   if (hit != stringPropertyTypes().constEnd())
       return hit.value();

    // text: Check according to widget type.
    if (propertyName == QLatin1String("text")) {
        if (qobject_cast<const QAction *>(object) || qobject_cast<const QLineEdit *>(object))
            return StringPropertyParameters(ValidationSingleLine, true);
        if (qobject_cast<const QAbstractButton *>(object))
            return StringPropertyParameters(ValidationMultiLine, true);
        return StringPropertyParameters(ValidationRichText, true);
    }

   // Fuzzy matching
    if (propertyName.endsWith(QLatin1String("Name")))
        return StringPropertyParameters(ValidationSingleLine, true);

    if (propertyName.endsWith(QLatin1String("ToolTip")))
        return StringPropertyParameters(ValidationRichText, true);

    // default to single
    return StringPropertyParameters(ValidationSingleLine, true);
}

void QDesignerPropertyEditor::emitPropertyValueChanged(const QString &name, const QVariant &value, bool enableSubPropertyHandling)
{
    // Avoid duplicate signal emission - see below
    m_propertyChangedForwardingBlocked = true;
    emit propertyValueChanged(name, value, enableSubPropertyHandling);
    emit propertyChanged(name, value);
    m_propertyChangedForwardingBlocked = false;
}

void QDesignerPropertyEditor::slotPropertyChanged(const QString &name, const QVariant &value)
{
    // Forward signal from Integration using the old interfaces.
    if (!m_propertyChangedForwardingBlocked)
        emit propertyValueChanged(name, value, true);
}

}

QT_END_NAMESPACE
#include <moc_qdesigner_propertyeditor_p.h>
