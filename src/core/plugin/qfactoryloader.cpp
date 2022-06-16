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

#include "qfactoryloader_p.h"

#ifndef QT_NO_LIBRARY
#include "qhash.h"
#include "qdir.h"
#include "qdebug.h"
#include "qmutex.h"
#include "qplugin.h"
#include "qpluginloader.h"
#include "qcoreapplication_p.h"
#include "qlibrary_p.h"
#include "qstdcontainers_p.h"

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QMutex, qGlobalFactoryLoaderMutex);
Q_GLOBAL_STATIC(QStdVector<QFactoryLoader*>, qGlobalFactoryLoaders)

class QFactoryLoaderPrivate
{
public:
    QFactoryLoaderPrivate();
    ~QFactoryLoaderPrivate();

    QMutex mutex;
    QHash<QString,QPluginLoader*> pluginMap;
    QHash<QString,QString> keyMap;
    QString suffix;
};

QFactoryLoaderPrivate::QFactoryLoaderPrivate()
{
}

QFactoryLoaderPrivate::~QFactoryLoaderPrivate()
{
}

QFactoryLoader::QFactoryLoader(const QString &suffix)
    : d_ptr(new QFactoryLoaderPrivate())
{
    Q_D(QFactoryLoader);
    d->suffix = suffix;

    QMutexLocker locker(qGlobalFactoryLoaderMutex());
    update();
    qGlobalFactoryLoaders()->append(this);
}

void QFactoryLoader::update()
{
    Q_D(QFactoryLoader);
    QMutexLocker locker(&d->mutex);
    d->keyMap.clear();
    foreach (const QString &pluginDir, QCoreApplication::pluginPaths()) {
        const QString path = pluginDir + d->suffix;

        QDir pathdir(path);
        if (qt_debug_component()) {
            qDebug() << "QFactoryLoader: plugin path" << path;
        }
        foreach (const QString &plugin, pathdir.entryList(QDir::Files)) {
            const QString fileName = QDir::cleanPath(path + QLatin1Char('/') + plugin);

            if (qt_debug_component()) {
                qDebug() << "QFactoryLoader: looking at" << fileName;
            }
            if (QLibrary::isLibrary(fileName)) {
                d->keyMap.insert(QFileInfo(plugin).baseName().toLower(), fileName);
            }
            if (qt_debug_component()) {
                qDebug() << "QFactoryLoader: keys" << d->keyMap.keys();
            }
        }
    }
}

QFactoryLoader::~QFactoryLoader()
{
    QMutexLocker locker(qGlobalFactoryLoaderMutex());
    qGlobalFactoryLoaders()->removeAll(this);
    delete d_ptr;
}

QStringList QFactoryLoader::keys() const
{
    Q_D(const QFactoryLoader);
    return d->keyMap.keys();
}

QObject *QFactoryLoader::instance(const QString &key)
{
    Q_D(QFactoryLoader);
    QMutexLocker locker(&d->mutex);
    QString lowered = key.toLower();
    QPluginLoader* loader = d->pluginMap.value(lowered, nullptr);
    if (loader) {
        if (qt_debug_component()) {
            qDebug() << "QFactoryLoader: cached plugin" << lowered;
        }
        return loader->instance();
    }
    if (qt_debug_component()) {
        qDebug() << "QFactoryLoader: attempting to load plugin" << lowered << d->keyMap.value(lowered);
    }
    const QString pluginpath = d->keyMap.value(lowered);
    if (pluginpath.isEmpty()) {
        if (qt_debug_component()) {
            qDebug() << "QFactoryLoader: attempt to loaded invalid plugin" << lowered;
        }
        return nullptr;
    }
    loader = new QPluginLoader(pluginpath);
    if (loader->load()) {
        if (qt_debug_component()) {
            qDebug() << "QFactoryLoader: loaded plugin" << lowered;
        }
        d->pluginMap.insert(lowered, loader);
        return loader->instance();
    }
    delete loader;
    if (qt_debug_component()) {
        qDebug() << "QFactoryLoader: no plugin for" << lowered;
    }
    return nullptr;
}

void QFactoryLoader::refreshAll()
{
    QMutexLocker locker(qGlobalFactoryLoaderMutex());
    QStdVector<QFactoryLoader*> *loaders = qGlobalFactoryLoaders();
    QStdVector<QFactoryLoader *>::const_iterator it = loaders->constBegin();
    while (it != loaders->constEnd()) {
        (*it)->update();
        it++;
    }
}

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY
