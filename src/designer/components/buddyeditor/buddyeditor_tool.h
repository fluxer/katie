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

#ifndef BUDDYEDITOR_TOOL_H
#define BUDDYEDITOR_TOOL_H

#include "buddyeditor_global.h"

#include <QtCore/QPointer>

#include <QtDesigner/abstractformwindowtool.h>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QDesignerFormWindowInterface;
class QAction;

namespace qdesigner_internal {

class BuddyEditor;

class QT_BUDDYEDITOR_EXPORT BuddyEditorTool: public QDesignerFormWindowToolInterface
{
    Q_OBJECT
public:
    explicit BuddyEditorTool(QDesignerFormWindowInterface *formWindow, QObject *parent = nullptr);
    virtual ~BuddyEditorTool();

    virtual QDesignerFormEditorInterface *core() const;
    virtual QDesignerFormWindowInterface *formWindow() const;

    virtual QWidget *editor() const;
    virtual QAction *action() const;

    virtual void activated();
    virtual void deactivated();

    virtual bool handleEvent(QWidget *widget, QWidget *managedWidget, QEvent *event);

private:
    QDesignerFormWindowInterface *m_formWindow;
    mutable QPointer<BuddyEditor> m_editor;
    QAction *m_action;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // BUDDYEDITOR_TOOL_H
