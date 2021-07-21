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
#include "qfactoryinterface.h"
#include "qmap.h"
#include "qdir.h"
#include "qsettings.h"
#include "qdebug.h"
#include "qmutex.h"
#include "qplugin.h"
#include "qpluginloader.h"
#include "qlibraryinfo.h"
#include "qobject_p.h"
#include "qcoreapplication_p.h"

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QList<QFactoryLoader *>, qt_factory_loaders)

static std::recursive_mutex qGlobalFactoryLoaderMutex;

class QFactoryLoaderPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QFactoryLoader)
public:
    QFactoryLoaderPrivate(){}
    ~QFactoryLoaderPrivate();
    mutable QMutex mutex;
    QByteArray iid;
    QList<QLibraryPrivate*> libraryList;
    QMap<QString,QLibraryPrivate*> keyMap;
    QStringList keyList;
    QString suffix;
    Qt::CaseSensitivity cs;
    QStringList loadedPaths;
};

QFactoryLoaderPrivate::~QFactoryLoaderPrivate()
{
    foreach (QLibraryPrivate *library, libraryList)
        library->release();
}

QFactoryLoader::QFactoryLoader(const char *iid,
                               const QString &suffix,
                               Qt::CaseSensitivity cs)
    : QObject(*new QFactoryLoaderPrivate)
{
    Q_D(QFactoryLoader);
    d->iid = iid;
    d->cs = cs;
    d->suffix = suffix;

    std::lock_guard<std::recursive_mutex> locker(qGlobalFactoryLoaderMutex);
    update();
    qt_factory_loaders()->append(this);
}


void QFactoryLoader::updateDir(const QString &pluginDir)
{
    Q_D(QFactoryLoader);
    QString path = pluginDir + d->suffix;
    QDir pathdir(path);
    if (!pathdir.exists())
        return;

    QSettings *settings = QCoreApplicationPrivate::staticConf();
    foreach (const QString &plugin, pathdir.entryList(QDir::Files)) {
        QString fileName = QDir::cleanPath(path + QLatin1Char('/') + plugin);

        if (qt_debug_component()) {
            qDebug() << "QFactoryLoader::QFactoryLoader() looking at" << fileName;
        }
        QLibraryPrivate *library = QLibraryPrivate::findOrCreate(QFileInfo(fileName).canonicalFilePath());
        if (!library->isPlugin()) {
            if (qt_debug_component()) {
                qDebug() << library->errorString;
                qDebug() << "         not a plugin";
            }
            library->release();
            continue;
        }
        QString regkey = QString::fromLatin1("Katie Factory Cache %1/%2:/%3")
                         .arg(QT_VERSION_HEX_STR)
                         .arg(QString::fromLatin1(d->iid.constData()))
                         .arg(fileName);
        QStringList keys;
        QStringList reg = settings->value(regkey).toStringList();
        if (reg.count() && library->lastModified == reg[0]) {
            keys = reg;
            keys.removeFirst();
        } else {
            if (!library->loadPlugin()) {
                if (qt_debug_component()) {
                    qDebug() << library->errorString;
                    qDebug() << "           could not load";
                }
                library->release();
                continue;
            }
            QObject *instance = library->instance();
            if (!instance) {
                library->release();
                // ignore plugins that have a valid signature but cannot be loaded.
                continue;
            }
            QFactoryInterface *factory = qobject_cast<QFactoryInterface*>(instance);
            if (instance && factory && instance->qt_metacast(d->iid.constData()))
                keys = factory->keys();
            if (keys.isEmpty())
                library->release();
            reg.clear();
            reg << library->lastModified;
            reg += keys;
            settings->setValue(regkey, reg);
        }
        if (qt_debug_component()) {
            qDebug() << "keys" << keys;
        }

        if (keys.isEmpty()) {
            library->release();
            continue;
        }

        int keysUsed = 0;
        for (int k = 0; k < keys.count(); ++k) {
            // first come first serve, unless the first
            // library was built with a future Qt version,
            // whereas the new one has a Qt version that fits
            // better
            QString key = keys.at(k);
            if (!d->cs)
                key = key.toLower();
            QLibraryPrivate *previous = d->keyMap.value(key);
            if (!previous || (previous->qt_version > QT_VERSION && library->qt_version <= QT_VERSION)) {
                d->keyMap[key] = library;
                d->keyList += keys.at(k);
                keysUsed++;
            }
        }
        if (keysUsed)
            d->libraryList += library;
        else
            library->release();
    }
}

void QFactoryLoader::update()
{
    Q_D(QFactoryLoader);
    QMutexLocker locker(&d->mutex);
    foreach (const QString &pluginDir, QCoreApplication::libraryPaths()) {
        // Already loaded, skip it...
        if (d->loadedPaths.contains(pluginDir))
            continue;
        d->loadedPaths << pluginDir;
        updateDir(pluginDir);
    }
}

QFactoryLoader::~QFactoryLoader()
{
    std::lock_guard<std::recursive_mutex> locker(qGlobalFactoryLoaderMutex);
    qt_factory_loaders()->removeAll(this);
}

QStringList QFactoryLoader::keys() const
{
    Q_D(const QFactoryLoader);
    return d->keyList;
}

QObject *QFactoryLoader::instance(const QString &key) const
{
    Q_D(const QFactoryLoader);
    QMutexLocker locker(&d->mutex);
    QString lowered = d->cs ? key : key.toLower();
    if (QLibraryPrivate* library = d->keyMap.value(lowered)) {
        if (library->instance || library->loadPlugin()) {
            return library->instance();
        }
    }
    return 0;
}

void QFactoryLoader::refreshAll()
{
    std::lock_guard<std::recursive_mutex> locker(qGlobalFactoryLoaderMutex);
    QList<QFactoryLoader *> *loaders = qt_factory_loaders();
    for (QList<QFactoryLoader *>::const_iterator it = loaders->constBegin();
         it != loaders->constEnd(); ++it) {
        (*it)->update();
    }
}

#include "moc_qfactoryloader_p.h"

QT_END_NAMESPACE

#endif // QT_NO_LIBRARY
