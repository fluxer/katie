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

#include <QtGui/QAction>

#include "tabordereditor_plugin.h"
#include "tabordereditor_tool.h"

#include <QtDesigner/abstractformeditor.h>
#include <QtDesigner/abstractformwindow.h>
#include <QtDesigner/abstractformwindowmanager.h>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

TabOrderEditorPlugin::TabOrderEditorPlugin()
    : m_initialized(false)
{
}

TabOrderEditorPlugin::~TabOrderEditorPlugin()
{
}

bool TabOrderEditorPlugin::isInitialized() const
{
    return m_initialized;
}

void TabOrderEditorPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_ASSERT(!isInitialized());

    m_action = new QAction(tr("Edit Tab Order"), this);
    m_action->setObjectName(QLatin1String("_qt_edit_tab_order_action"));
    QIcon icon = QIcon::fromTheme(QLatin1String("designer-edit-tabs"),
                                  QIcon(core->resourceLocation() + QLatin1String("/tabordertool.png")));
    m_action->setIcon(icon);
    m_action->setEnabled(false);

    setParent(core);
    m_core = core;
    m_initialized = true;

    connect(core->formWindowManager(), SIGNAL(formWindowAdded(QDesignerFormWindowInterface*)),
            this, SLOT(addFormWindow(QDesignerFormWindowInterface*)));

    connect(core->formWindowManager(), SIGNAL(formWindowRemoved(QDesignerFormWindowInterface*)),
            this, SLOT(removeFormWindow(QDesignerFormWindowInterface*)));

    connect(core->formWindowManager(), SIGNAL(activeFormWindowChanged(QDesignerFormWindowInterface*)),
                this, SLOT(activeFormWindowChanged(QDesignerFormWindowInterface*)));
}

void TabOrderEditorPlugin::activeFormWindowChanged(QDesignerFormWindowInterface *formWindow)
{
    m_action->setEnabled(formWindow != 0);
}

QDesignerFormEditorInterface *TabOrderEditorPlugin::core() const
{
    return m_core;
}

void TabOrderEditorPlugin::addFormWindow(QDesignerFormWindowInterface *formWindow)
{
    Q_ASSERT(formWindow != 0);
    Q_ASSERT(m_tools.contains(formWindow) == false);

    TabOrderEditorTool *tool = new TabOrderEditorTool(formWindow, this);
    m_tools[formWindow] = tool;
    connect(m_action, SIGNAL(triggered()), tool->action(), SLOT(trigger()));
    formWindow->registerTool(tool);
}

void TabOrderEditorPlugin::removeFormWindow(QDesignerFormWindowInterface *formWindow)
{
    Q_ASSERT(formWindow != 0);
    Q_ASSERT(m_tools.contains(formWindow) == true);

    TabOrderEditorTool *tool = m_tools.value(formWindow);
    m_tools.remove(formWindow);
    disconnect(m_action, SIGNAL(triggered()), tool->action(), SLOT(trigger()));
    // ### FIXME disable the tool

    delete tool;
}

QAction *TabOrderEditorPlugin::action() const
{
    return m_action;
}

QT_END_NAMESPACE
#include "moc_tabordereditor_plugin.h"
