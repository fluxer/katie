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

#include "layouttaskmenu.h"
#include <formlayoutmenu_p.h>
#include <morphmenu_p.h>

#include <QtDesigner/abstractformwindow.h>

#include <QtGui/QAction>
#include <QtCore/QDebug>

QT_BEGIN_NAMESPACE

// ------------ LayoutWidgetTaskMenu
LayoutWidgetTaskMenu::LayoutWidgetTaskMenu(QLayoutWidget *lw, QObject *parent) :
   QObject(parent),
   m_widget(lw),
   m_morphMenu(new qdesigner_internal::MorphMenu(this)),
   m_formLayoutMenu(new qdesigner_internal::FormLayoutMenu(this))
{
}

QAction *LayoutWidgetTaskMenu::preferredEditAction() const
{
    return m_formLayoutMenu->preferredEditAction(m_widget, m_widget->formWindow());
}

QList<QAction*> LayoutWidgetTaskMenu::taskActions() const
{
    QList<QAction*> rc;
    QDesignerFormWindowInterface *fw = m_widget->formWindow();
    m_morphMenu->populate(m_widget, fw, rc);
    m_formLayoutMenu->populate(m_widget, fw, rc);
    return rc;
}

// ------------- SpacerTaskMenu
SpacerTaskMenu::SpacerTaskMenu(Spacer *, QObject *parent) :
    QObject(parent)
{
}

QAction *SpacerTaskMenu::preferredEditAction() const
{
    return 0;
}

QList<QAction*> SpacerTaskMenu::taskActions() const
{
    return QList<QAction*>();
}

QT_END_NAMESPACE

#include "moc_layouttaskmenu.h"
