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

#include "qdesigner_integration_p.h"
#include "qdesigner_propertycommand_p.h"
#include "qdesigner_propertyeditor_p.h"
#include "qdesigner_objectinspector_p.h"
#include "widgetdatabase_p.h"
#include "pluginmanager_p.h"
#include "widgetfactory_p.h"
#include "qdesigner_widgetbox_p.h"

// sdk
#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/abstractformwindow.h>
#include <QtDesigner/abstractformwindowmanager.h>
#include <QtDesigner/abstractformwindowcursor.h>
#include <QtDesigner/abstractactioneditor.h>
#include <QtDesigner/abstractwidgetbox.h>
#include <QtDesigner/QExtensionManager>
#include <QtDesigner/propertysheet.h>

#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QDir>

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// ---------------- DesignerIntegrationPrivate
class QDesignerIntegrationPrivate {
public:
    QDesignerIntegrationPrivate()
        : m_slotNavigationEnabled(false)
    {}

    bool m_slotNavigationEnabled;
};

// -------------- QDesignerIntegration
// As of 4.4, the header will be distributed with the Eclipse plugin.

QDesignerIntegration::QDesignerIntegration(QDesignerFormEditorInterface *core, QObject *parent) :
    QDesignerIntegrationInterface(core, parent),
    m_d(new QDesignerIntegrationPrivate)
{
    initialize();
}

QDesignerIntegration::~QDesignerIntegration()
{
    delete m_d;
}

void QDesignerIntegration::initialize()
{
    //
    // integrate the `Form Editor component'
    //

    // Extensions
    if (QDesignerPropertyEditor *designerPropertyEditor= qobject_cast<QDesignerPropertyEditor *>(core()->propertyEditor())) {
        connect(designerPropertyEditor, SIGNAL(propertyValueChanged(QString,QVariant,bool)), this, SLOT(updateProperty(QString,QVariant,bool)));
        connect(designerPropertyEditor, SIGNAL(resetProperty(QString)), this, SLOT(resetProperty(QString)));
        connect(designerPropertyEditor, SIGNAL(addDynamicProperty(QString,QVariant)),
                this, SLOT(addDynamicProperty(QString,QVariant)));
        connect(designerPropertyEditor, SIGNAL(removeDynamicProperty(QString)),
                this, SLOT(removeDynamicProperty(QString)));
    } else {
        connect(core()->propertyEditor(), SIGNAL(propertyChanged(QString,QVariant)),
                this, SLOT(updatePropertyPrivate(QString,QVariant)));
    }

    connect(core()->formWindowManager(), SIGNAL(formWindowAdded(QDesignerFormWindowInterface*)),
            this, SLOT(setupFormWindow(QDesignerFormWindowInterface*)));

    connect(core()->formWindowManager(), SIGNAL(activeFormWindowChanged(QDesignerFormWindowInterface*)),
            this, SLOT(updateActiveFormWindow(QDesignerFormWindowInterface*)));

    if (WidgetDataBase *widgetDataBase = qobject_cast<WidgetDataBase*>(core()->widgetDataBase()))
        widgetDataBase->grabStandardWidgetBoxIcons();
}

void QDesignerIntegration::updateProperty(const QString &name, const QVariant &value, bool enableSubPropertyHandling)
{
    QDesignerFormWindowInterface *formWindow = core()->formWindowManager()->activeFormWindow();
    if (!formWindow)
        return;

    Selection selection;
    getSelection(selection);
    if (selection.empty())
        return;

    SetPropertyCommand *cmd = new SetPropertyCommand(formWindow);
    // find a reference object to compare to and to find the right group
    if (cmd->init(selection.selection(), name, value, propertyEditorObject(), enableSubPropertyHandling)) {
        formWindow->commandHistory()->push(cmd);
    } else {
        delete cmd;
        qDebug() << "Unable to set  property " << name << '.';
    }

    emit propertyChanged(formWindow, name, value);
}

void QDesignerIntegration::updatePropertyPrivate(const QString &name, const QVariant &value)
{
    updateProperty(name, value, true);
}

void QDesignerIntegration::resetProperty(const QString &name)
{
    QDesignerFormWindowInterface *formWindow = core()->formWindowManager()->activeFormWindow();
    if (!formWindow)
        return;

    Selection selection;
    getSelection(selection);
    if (selection.empty())
        return;


    ResetPropertyCommand *cmd = new ResetPropertyCommand(formWindow);
    // find a reference object to find the right group
    if (cmd->init(selection.selection(), name, propertyEditorObject())) {
        formWindow->commandHistory()->push(cmd);
    } else {
        delete cmd;
        qDebug() << "** WARNING Unable to reset property " << name << '.';
    }
}

void QDesignerIntegration::addDynamicProperty(const QString &name, const QVariant &value)
{
    QDesignerFormWindowInterface *formWindow = core()->formWindowManager()->activeFormWindow();
    if (!formWindow)
        return;

    Selection selection;
    getSelection(selection);
    if (selection.empty())
        return;

    AddDynamicPropertyCommand *cmd = new AddDynamicPropertyCommand(formWindow);
    if (cmd->init(selection.selection(), propertyEditorObject(), name, value)) {
        formWindow->commandHistory()->push(cmd);
    } else {
        delete cmd;
        qDebug() <<  "** WARNING Unable to add dynamic property " << name << '.';
    }
}

void QDesignerIntegration::removeDynamicProperty(const QString &name)
{
    QDesignerFormWindowInterface *formWindow = core()->formWindowManager()->activeFormWindow();
    if (!formWindow)
        return;

    Selection selection;
    getSelection(selection);
    if (selection.empty())
        return;

    RemoveDynamicPropertyCommand *cmd = new RemoveDynamicPropertyCommand(formWindow);
    if (cmd->init(selection.selection(), propertyEditorObject(), name)) {
        formWindow->commandHistory()->push(cmd);
    } else {
        delete cmd;
        qDebug() << "** WARNING Unable to remove dynamic property " << name << '.';
    }

}


void QDesignerIntegration::updateActiveFormWindow(QDesignerFormWindowInterface *formWindow)
{
    Q_UNUSED(formWindow);
    updateSelection();
}

void QDesignerIntegration::setupFormWindow(QDesignerFormWindowInterface *formWindow)
{
    connect(formWindow, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
    connect(formWindow, SIGNAL(activated(QWidget*)), this, SLOT(activateWidget(QWidget*)));
}

void QDesignerIntegration::updateGeometry()
{
}

void QDesignerIntegration::updateSelection()
{
    QDesignerFormWindowInterface *formWindow = core()->formWindowManager()->activeFormWindow();
    QWidget *selection = 0;

    if (formWindow) {
        selection = formWindow->cursor()->current();
    }

    if (QDesignerActionEditorInterface *actionEditor = core()->actionEditor())
        actionEditor->setFormWindow(formWindow);

    if (QDesignerPropertyEditorInterface *propertyEditor = core()->propertyEditor())
        propertyEditor->setObject(selection);

    if (QDesignerObjectInspectorInterface *objectInspector = core()->objectInspector())
        objectInspector->setFormWindow(formWindow);

}

void QDesignerIntegration::activateWidget(QWidget *widget)
{
    Q_UNUSED(widget);
}

QWidget *QDesignerIntegration::containerWindow(QWidget *widget) const
{
    // Find the parent window to apply a geometry to.
    while (widget) {
        if (widget->isWindow())
            break;
        if (qstrcmp(widget->metaObject()->className(), "QMdiSubWindow") == 0)
            break;

        widget = widget->parentWidget();
    }

    return widget;
}

void QDesignerIntegration::getSelection(Selection &s)
{
    // Get multiselection from object inspector
    if (QDesignerObjectInspector *designerObjectInspector = qobject_cast<QDesignerObjectInspector *>(core()->objectInspector())) {
        designerObjectInspector->getSelection(s);
        // Action editor puts actions that are not on the form yet
        // into the property editor only.
        if (s.empty())
            if (QObject *object = core()->propertyEditor()->object())
                s.objects.push_back(object);

    } else {
        // Just in case someone plugs in an old-style object inspector: Emulate selection
        s.clear();
        QDesignerFormWindowInterface *formWindow = core()->formWindowManager()->activeFormWindow();
        if (!formWindow)
            return;

        QObject *object = core()->propertyEditor()->object();
        if (object->isWidgetType()) {
            QWidget *widget = static_cast<QWidget*>(object);
            QDesignerFormWindowCursorInterface *cursor = formWindow->cursor();
            if (cursor->isWidgetSelected(widget)) {
                s.managed.push_back(widget);
            } else {
                s.unmanaged.push_back(widget);
            }
        } else {
            s.objects.push_back(object);
        }
    }
}

QObject *QDesignerIntegration::propertyEditorObject()
{
    QDesignerPropertyEditorInterface *propertyEditor = core()->propertyEditor();
    if (!propertyEditor)
        return 0;
    return propertyEditor->object();
}

// Load plugins into widget database and factory.
void QDesignerIntegration::initializePlugins(QDesignerFormEditorInterface *formEditor)
{
    // load the plugins
    WidgetDataBase *widgetDataBase = qobject_cast<WidgetDataBase*>(formEditor->widgetDataBase());
    if (widgetDataBase) {
        widgetDataBase->loadPlugins();
    }

    if (WidgetFactory *widgetFactory = qobject_cast<WidgetFactory*>(formEditor->widgetFactory())) {
        widgetFactory->loadPlugins();
    }

    if (widgetDataBase) {
        widgetDataBase->grabDefaultPropertyValues();
    }
}

void QDesignerIntegration::updateCustomWidgetPlugins()
{
    QDesignerFormEditorInterface *formEditor = core();
    if (QDesignerPluginManager *pm = formEditor->pluginManager())
        pm->registerNewPlugins();

    initializePlugins(formEditor);

    // Do not just reload the last file as the WidgetBox merges the compiled-in resources
    // and $HOME/.designer/widgetbox.xml. This would also double the scratchpad.
    if (QDesignerWidgetBox *wb = qobject_cast<QDesignerWidgetBox*>(formEditor->widgetBox())) {
        const QDesignerWidgetBox::LoadMode oldLoadMode = wb->loadMode();
        wb->setLoadMode(QDesignerWidgetBox::LoadCustomWidgetsOnly);
        wb->load();
        wb->setLoadMode(oldLoadMode);
    }
}

void QDesignerIntegration::emitObjectNameChanged(QDesignerFormWindowInterface *formWindow, QObject *object, const QString &newName, const QString &oldName)
{
    emit objectNameChanged(formWindow, object, newName, oldName);
}

void QDesignerIntegration::emitNavigateToSlot(const QString &objectName,
                                              const QString &signalSignature,
                                              const QStringList &parameterNames)
{
     emit navigateToSlot(objectName, signalSignature, parameterNames);
}

void QDesignerIntegration::emitNavigateToSlot(const QString &slotSignature)
{
    emit navigateToSlot(slotSignature);
}

void QDesignerIntegration::requestHelp(const QDesignerFormEditorInterface *core, const QString &manual, const QString &document)
{
    if (QDesignerIntegration *di = qobject_cast<QDesignerIntegration *>(core->integration()))
        emit di->helpRequested(manual, document);
}

void QDesignerIntegration::setSlotNavigationEnabled(bool enable)
{
    m_d->m_slotNavigationEnabled = enable;
}

bool QDesignerIntegration::isSlotNavigationEnabled() const
{
    return m_d->m_slotNavigationEnabled;
}

static QString fixHelpClassName(const QString &className)
{
    // ### generalize using the Widget Data Base
    if (className == QLatin1String("Line"))
        return QLatin1String("QFrame");
    if (className == QLatin1String("Spacer"))
        return QLatin1String("QSpacerItem");
    if (className == QLatin1String("QLayoutWidget"))
        return QLatin1String("QLayout");
    return className;
}

// Return class in which the property is defined
static QString classForProperty(QDesignerFormEditorInterface *core,
                                QObject *object,
                                const QString &property)
{
    if (const QDesignerPropertySheetExtension *ps = qt_extension<QDesignerPropertySheetExtension *>(core->extensionManager(), object)) {
        const int index = ps->indexOf(property);
        if (index >= 0)
            return ps->propertyGroup(index);
    }
    return QString();
}

QString QDesignerIntegration::contextHelpId() const
{
    QObject *currentObject = core()->propertyEditor()->object();
    if (!currentObject)
        return QString();
    // Return a help index id consisting of "class::property"
    QString className;
    QString currentPropertyName = core()->propertyEditor()->currentPropertyName();    
    if (!currentPropertyName.isEmpty())
        className = classForProperty(core(), currentObject, currentPropertyName);
    if (className.isEmpty()) {
        currentPropertyName.clear(); // We hit on some fake property.
        className = WidgetFactory::classNameOf(core(), currentObject);
    }
    QString helpId = fixHelpClassName(className);
    if (!currentPropertyName.isEmpty()) {
        helpId += QLatin1String("::");
        helpId += currentPropertyName;
    }
    return helpId;
}

} // namespace qdesigner_internal

QT_END_NAMESPACE

#include "moc_qdesigner_integration_p.h"
