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

#ifndef LAYOUTTASKMENU_H
#define LAYOUTTASKMENU_H

#include <QtDesigner/taskmenu.h>

#include "qlayout_widget_p.h"
#include <spacer_widget_p.h>
#include <extensionfactory_p.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {
    class FormLayoutMenu;
    class MorphMenu;
}

// Morph menu for QLayoutWidget.
class LayoutWidgetTaskMenu : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    explicit LayoutWidgetTaskMenu(QLayoutWidget *w, QObject *parent = nullptr);

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

private:
    QLayoutWidget *m_widget;
    qdesigner_internal::MorphMenu *m_morphMenu;
    qdesigner_internal::FormLayoutMenu *m_formLayoutMenu;
};

// Empty task menu for spacers.
class SpacerTaskMenu : public QObject, public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES(QDesignerTaskMenuExtension)
public:
    explicit SpacerTaskMenu(Spacer *bar, QObject *parent = nullptr);

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

};

typedef qdesigner_internal::ExtensionFactory<QDesignerTaskMenuExtension, QLayoutWidget, LayoutWidgetTaskMenu> LayoutWidgetTaskMenuFactory;
typedef qdesigner_internal::ExtensionFactory<QDesignerTaskMenuExtension, Spacer, SpacerTaskMenu> SpacerTaskMenuFactory;

QT_END_NAMESPACE

#endif // LAYOUTTASKMENU_H
