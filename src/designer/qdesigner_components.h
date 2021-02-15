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

#ifndef QDESIGNER_COMPONENTS_H
#define QDESIGNER_COMPONENTS_H

#include <QtCore/QObject>
#include <QtDesigner/qdesigner_components_global.h>


QT_BEGIN_NAMESPACE

class QObject;
class QWidget;

class QDesignerFormEditorInterface;
class QDesignerWidgetBoxInterface;
class QDesignerPropertyEditorInterface;
class QDesignerObjectInspectorInterface;
class QDesignerActionEditorInterface;

class QDESIGNER_COMPONENTS_EXPORT QDesignerComponents
{
public:
    static void initializeResources();
    static QObjectList initializePlugins(QDesignerFormEditorInterface *core);

    static QDesignerFormEditorInterface *createFormEditor(QObject *parent);
    static QDesignerWidgetBoxInterface *createWidgetBox(QDesignerFormEditorInterface *core, QWidget *parent);
    static QDesignerPropertyEditorInterface *createPropertyEditor(QDesignerFormEditorInterface *core, QWidget *parent);
    static QDesignerObjectInspectorInterface *createObjectInspector(QDesignerFormEditorInterface *core, QWidget *parent);
    static QDesignerActionEditorInterface *createActionEditor(QDesignerFormEditorInterface *core, QWidget *parent);

    static QObject *createTaskMenu(QDesignerFormEditorInterface *core, QObject *parent);
    static QWidget *createResourceEditor(QDesignerFormEditorInterface *core, QWidget *parent);
    static QWidget *createSignalSlotEditor(QDesignerFormEditorInterface *core, QWidget *parent);
};

QT_END_NAMESPACE


#endif // QDESIGNER_COMPONENTS_H
