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
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
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
    explicit GroupBoxTaskMenu(QGroupBox *groupbox, QObject *parent = Q_NULLPTR);

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
