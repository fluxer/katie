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

#ifndef TASKMENU_H
#define TASKMENU_H

#include <QtDesigner/extension.h>


QT_BEGIN_NAMESPACE

class QAction;

class QDesignerTaskMenuExtension
{
public:
    virtual ~QDesignerTaskMenuExtension() {}

    virtual QAction *preferredEditAction() const;

    virtual QList<QAction*> taskActions() const = 0;
};

inline QAction *QDesignerTaskMenuExtension::preferredEditAction() const
{ return 0; }

QT_END_NAMESPACE

Q_DECLARE_EXTENSION_INTERFACE(QDesignerTaskMenuExtension, "Katie.Designer.TaskMenu")


#endif // TASKMENU_H
