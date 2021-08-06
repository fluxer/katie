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

#ifndef SIGNALSLOTEDITOR_TOOL_H
#define SIGNALSLOTEDITOR_TOOL_H

#include "signalsloteditor_global.h"
#include "signalsloteditor.h"

#include <QtCore/QPointer>
#include <QtDesigner/abstractformwindowtool.h>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QAction;

namespace qdesigner_internal {

class SignalSlotEditor;

class QT_SIGNALSLOTEDITOR_EXPORT SignalSlotEditorTool: public QDesignerFormWindowToolInterface
{
    Q_OBJECT
public:
    explicit SignalSlotEditorTool(QDesignerFormWindowInterface *formWindow, QObject *parent = nullptr);
    virtual ~SignalSlotEditorTool();

    virtual QDesignerFormEditorInterface *core() const;
    virtual QDesignerFormWindowInterface *formWindow() const;

    virtual QWidget *editor() const;

    QAction *action() const;

    virtual void activated();
    virtual void deactivated();

    virtual bool handleEvent(QWidget *widget, QWidget *managedWidget, QEvent *event);

    virtual void saveToDom(DomUI *ui, QWidget *mainContainer);
    virtual void loadFromDom(DomUI *ui, QWidget *mainContainer);

private:
    QDesignerFormWindowInterface *m_formWindow;
    mutable QPointer<qdesigner_internal::SignalSlotEditor> m_editor;
    QAction *m_action;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // SIGNALSLOTEDITOR_TOOL_H
