/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#include "qplatformdefs.h"

#include "qplugin.h"
#include "qpluginloader.h"
#include "qlibrary_p.h"
#include "qdebug.h"
#include "qdir.h"


#ifndef QT_NO_LIBRARY

QT_BEGIN_NAMESPACE

/*!
    \class QPluginLoader
    \reentrant
    \brief The QPluginLoader class loads a plugin at run-time.


    \ingroup plugins

    QPluginLoader provides access to a \l{How to Create Qt
    Plugins}{Qt plugin}. A Katie plugin is stored in a shared library
    and offers these benefits over shared libraries accessed
    using QLibrary:

    \list
    \o QPluginLoader checks that a plugin is linked against the same
       version of Katie as the application.
    \o QPluginLoader provides direct access to a root component object
       (instance()), instead of forcing you to resolve a C function manually.
    \endlist

    An instance of a QPluginLoader object operates on a single shared
    library file, which we call a plugin. It provides access to the
    functionality in the plugin in a platform-independent way. To
    specify which plugin to load, either pass a file name in
    the constructor or set it with setFileName().

    The most important functions are load() to dynamically load the
    plugin file, isLoaded() to check whether loading was successful,
    and instance() to access the root component in the plugin. The
    instance() function implicitly tries to load the plugin if it has
    not been loaded yet. Multiple instances of QPluginLoader can be
    used to access the same physical plugin.

    Once loaded, plugins remain in memory until all instances of
    QPluginLoader has been unloaded, or until the application
    terminates. You can attempt to unload a plugin using unload(),
    but if other instances of QPluginLoader are using the same
    library, the call will fail, and unloading will only happen when
    every instance has called unload(). Right before the unloading
    happen, the root component will also be deleted.

    In order to speed up loading and validation of plugins, some of
    the information that is collected during loading is cached in
    persistent memory (through QSettings). For instance, the result
    of a load operation (e.g. succeeded or failed) is stored in the
    cache, so that subsequent load operations don't try to load an
    invalid plugin. However, if the "last modified" timestamp of
    a plugin has changed, the plugin's cache entry is invalidated
    and the plugin is reloaded regardless of the values in the cache
    entry. The cache entry is then updated with the new result of the
    load operation.

    This also means that the timestamp must be updated each time the
    plugin or any dependent resources (such as a shared library) is
    updated, since the dependent resources might influence the result
    of loading a plugin.

    See \l{How to Create Qt Plugins} for more information about
    how to make your application extensible through plugins.

    Note that the QPluginLoader cannot be used if your application is
    statically linked against Qt. You can use QLibrary if you need to
    load dynamic libraries in a statically linked application.

    \sa QLibrary, {Plug & Paint Example}
*/

/*!
    Constructs a plugin loader with the given \a parent.
*/
QPluginLoader::QPluginLoader(QObject *parent)
    : QLibrary(parent)
{
}

/*!
    Constructs a plugin loader with the given \a parent that will
    load the plugin specified by \a fileName.

    To be loadable, the file's suffix must be a valid suffix for a
    loadable library in accordance with the platform, e.g. \c .so on
    Unix. The suffix can be verified with QLibrary::isLibrary().

    \sa setFileName()
*/
QPluginLoader::QPluginLoader(const QString &fileName, QObject *parent)
    : QLibrary(fileName, parent)
{
}

/*!
    Destroys the QPluginLoader object.

    Unless unload() was called explicitly, the plugin stays in memory
    until the application terminates.

    \sa isLoaded(), unload()
*/
QPluginLoader::~QPluginLoader()
{
}

/*!
    Returns the root component object of the plugin. The plugin is
    loaded if necessary. The function returns 0 if the plugin could
    not be loaded or if the root component object could not be
    instantiated.

    If the root component object was destroyed, calling this function
    creates a new instance.

    The root component, returned by this function, is not deleted when
    the QPluginLoader is destroyed. If you want to ensure that the root
    component is deleted, you should call unload() as soon you don't
    need to access the core component anymore.  When the library is
    finally unloaded, the root component will automatically be deleted.

    The component object is a QObject. Use qobject_cast() to access
    interfaces you are interested in.

    \sa load()
*/
QObject *QPluginLoader::instance()
{
    if (!d_ptr->loadPlugin()) {
        return nullptr;
    }
    if (!d_ptr->inst && d_ptr->instance)
        d_ptr->inst = d_ptr->instance();
    return d_ptr->inst.data();
}

#include "moc_qpluginloader.h"

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY
