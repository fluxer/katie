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

#ifndef ABSTRACTFORMEDITORPLUGIN_H
#define ABSTRACTFORMEDITORPLUGIN_H

#include <QtCore/QObject>


QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QAction;

class Q_DESIGNER_EXPORT QDesignerFormEditorPluginInterface
{
public:
    virtual ~QDesignerFormEditorPluginInterface() {}

    virtual bool isInitialized() const = 0;
    virtual void initialize(QDesignerFormEditorInterface *core) = 0;
    virtual QAction *action() const = 0;

    virtual QDesignerFormEditorInterface *core() const = 0;
};

QT_END_NAMESPACE

Q_DECLARE_INTERFACE(QDesignerFormEditorPluginInterface, "Katie.Designer.QDesignerFormEditorPluginInterface")


#endif // ABSTRACTFORMEDITORPLUGIN_H
