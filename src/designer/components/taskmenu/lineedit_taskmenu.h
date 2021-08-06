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

#ifndef LINEEDIT_TASKMENU_H
#define LINEEDIT_TASKMENU_H

#include <QtGui/QLineEdit>
#include <QtCore/QPointer>

#include "qdesigner_taskmenu_p.h"
#include <extensionfactory_p.h>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

class LineEditTaskMenu: public QDesignerTaskMenu
{
    Q_OBJECT
public:
    explicit LineEditTaskMenu(QLineEdit *button, QObject *parent = nullptr);

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

private:
    QList<QAction*> m_taskActions;
    QAction *m_editTextAction;
};

typedef ExtensionFactory<QDesignerTaskMenuExtension, QLineEdit, LineEditTaskMenu> LineEditTaskMenuFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // LINEEDIT_TASKMENU_H
