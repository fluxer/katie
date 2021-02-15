/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
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

#include "qaccessibleplugin.h"

#ifndef QT_NO_ACCESSIBILITY

#include "qaccessible.h"

QT_BEGIN_NAMESPACE

/*!
    \class QAccessiblePlugin
    \brief The QAccessiblePlugin class provides an abstract base for
    accessibility plugins.

    \ingroup plugins
    \ingroup accessibility

    Writing an accessibility plugin is achieved by subclassing this
    base class, reimplementing the pure virtual functions keys() and
    create(), and exporting the class with the Q_EXPORT_PLUGIN2()
    macro.

    \sa QAccessibleBridgePlugin, {How to Create Qt Plugins}
*/

/*!
    Constructs an accessibility plugin with the given \a parent. This
    is invoked automatically by the Q_EXPORT_PLUGIN2() macro.
*/
QAccessiblePlugin::QAccessiblePlugin(QObject *parent)
    : QObject(parent)
{
}

/*!
    Destroys the accessibility plugin.

    You never have to call this explicitly. Qt destroys a plugin
    automatically when it is no longer used.
*/
QAccessiblePlugin::~QAccessiblePlugin()
{
}

/*!
    \fn QStringList QAccessiblePlugin::keys() const

    Returns the list of keys this plugin supports.

    These keys must be the class names that this plugin provides
    an accessibility implementation for.

    \sa create()
*/

/*!
    \fn QAccessibleInterface *QAccessiblePlugin::create(const QString &key, QObject *object)

    Creates and returns a QAccessibleInterface implementation for the
    class \a key and the object \a object. Keys are case sensitive.

    \sa keys()
*/

QT_END_NAMESPACE


#include "moc_qaccessibleplugin.h"

#endif // QT_NO_ACCESSIBILITY
