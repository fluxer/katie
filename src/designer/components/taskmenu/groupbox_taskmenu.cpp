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

#include "groupbox_taskmenu.h"
#include "inplace_editor.h"

#include <QtDesigner/abstractformwindow.h>

#include <QtGui/QAction>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

// -------- GroupBoxTaskMenuInlineEditor
class GroupBoxTaskMenuInlineEditor : public  TaskMenuInlineEditor
{
public:
    GroupBoxTaskMenuInlineEditor(QGroupBox *button, QObject *parent);

protected:
    virtual QRect editRectangle() const;
};

GroupBoxTaskMenuInlineEditor::GroupBoxTaskMenuInlineEditor(QGroupBox *w, QObject *parent) :
      TaskMenuInlineEditor(w, ValidationSingleLine, QLatin1String("title"), parent)
{
}

QRect GroupBoxTaskMenuInlineEditor::editRectangle() const
{
    QWidget *w = widget();
    QStyleOption opt; // ## QStyleOptionGroupBox
    opt.init(w);
    return QRect(QPoint(), QSize(w->width(),20));
}

// --------------- GroupBoxTaskMenu

GroupBoxTaskMenu::GroupBoxTaskMenu(QGroupBox *groupbox, QObject *parent)
    : QDesignerTaskMenu(groupbox, parent),
      m_editTitleAction(new QAction(tr("Change title..."), this))

{
    TaskMenuInlineEditor *editor = new GroupBoxTaskMenuInlineEditor(groupbox, this);
    connect(m_editTitleAction, SIGNAL(triggered()), editor, SLOT(editText()));
    m_taskActions.append(m_editTitleAction);

    QAction *sep = new QAction(this);
    sep->setSeparator(true);
    m_taskActions.append(sep);
}

QList<QAction*> GroupBoxTaskMenu::taskActions() const
{
    return m_taskActions + QDesignerTaskMenu::taskActions();
}

QAction *GroupBoxTaskMenu::preferredEditAction() const
{
    return m_editTitleAction;
}

}
QT_END_NAMESPACE
#include "moc_groupbox_taskmenu.h"
