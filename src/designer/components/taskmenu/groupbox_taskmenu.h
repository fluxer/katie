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

#ifndef GROUPBOX_TASKMENU_H
#define GROUPBOX_TASKMENU_H

#include <QtGui/QGroupBox>
#include <QtCore/QPointer>

#include "qdesigner_taskmenu_p.h"
#include <extensionfactory_p.h>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

namespace qdesigner_internal {
class InPlaceEditor;

class GroupBoxTaskMenu: public QDesignerTaskMenu
{
    Q_OBJECT
public:
    explicit GroupBoxTaskMenu(QGroupBox *groupbox, QObject *parent = nullptr);

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

private:
    QAction *m_editTitleAction;
    QList<QAction*> m_taskActions;
};

typedef ExtensionFactory<QDesignerTaskMenuExtension, QGroupBox, GroupBoxTaskMenu>  GroupBoxTaskMenuFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // GROUPBOX_TASKMENU_H
