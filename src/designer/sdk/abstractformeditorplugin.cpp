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

#include <QtDesigner/abstractformeditorplugin.h>

QT_BEGIN_NAMESPACE

/*!
    \internal
    \class QDesignerFormEditorPluginInterface
    \brief The QDesignerFormEditorPluginInterface class provides an interface that is used to
    manage plugins for Katie Designer's form editor component.
    \inmodule QtDesigner

    \sa QDesignerFormEditorInterface
*/

/*!
    \fn virtual QDesignerFormEditorPluginInterface::~QDesignerFormEditorPluginInterface()

    Destroys the plugin interface.
*/

/*!
    \fn virtual bool QDesignerFormEditorPluginInterface::isInitialized() const = 0

    Returns true if the plugin interface is initialized; otherwise returns false.
*/

/*!
    \fn virtual void QDesignerFormEditorPluginInterface::initialize(QDesignerFormEditorInterface *core) = 0

    Initializes the plugin interface for the specified \a core interface.
*/

/*!
    \fn virtual QAction *QDesignerFormEditorPluginInterface::action() const = 0

    Returns the action associated with this interface.
*/

/*!
    \fn virtual QDesignerFormEditorInterface *QDesignerFormEditorPluginInterface::core() const = 0

    Returns the core form editor interface associated with this component.
*/

QT_END_NAMESPACE
