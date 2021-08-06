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

#ifndef MENUTASKMENU_H
#define MENUTASKMENU_H

#include <QtDesigner/taskmenu.h>

#include "qdesigner_menu_p.h"
#include "qdesigner_menubar_p.h"
#include <extensionfactory_p.h>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

namespace qdesigner_internal {

    class PromotionTaskMenu;

// The QMenu task menu provides promotion and a remove option. The actual
// menu context options are not forwarded since they make only sense 
// when a menu is being edited/visible.

class MenuTaskMenu : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    explicit MenuTaskMenu(QDesignerMenu *menu, QObject *parent = nullptr);

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

private slots:
    void removeMenu();

private:
    QDesignerMenu *m_menu;
    QAction *m_removeAction;
    PromotionTaskMenu *m_promotionTaskMenu;
};

// The QMenuBar task menu forwards the actions of QDesignerMenuBar,
// making them available in the object inspector.

class MenuBarTaskMenu : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    explicit MenuBarTaskMenu(QDesignerMenuBar *bar, QObject *parent = nullptr);

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

private:
    QDesignerMenuBar *m_bar;
};

typedef ExtensionFactory<QDesignerTaskMenuExtension, QDesignerMenu, MenuTaskMenu> MenuTaskMenuFactory;
typedef ExtensionFactory<QDesignerTaskMenuExtension, QDesignerMenuBar, MenuBarTaskMenu> MenuBarTaskMenuFactory;

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // MENUTASKMENU_H
