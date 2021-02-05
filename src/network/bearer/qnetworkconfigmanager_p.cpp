/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtNetwork module of the Katie Toolkit.
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

#include "qnetworkconfigmanager_p.h"
#include "qbearerplugin_p.h"
#include "qfactoryloader_p.h"
#include "qdebug.h"
#include "qtimer.h"
#include "qstringlist.h"

#ifndef QT_NO_BEARERMANAGEMENT

QT_BEGIN_NAMESPACE

#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader,
                          (QBearerEngineFactoryInterface_iid, QLatin1String("/bearer")))
#endif

QNetworkConfigurationManagerPrivate::QNetworkConfigurationManagerPrivate()
    : QObject(), pollTimer(0), mutex(QMutex::Recursive), forcedPolling(0), firstUpdate(true)
{
    qRegisterMetaType<QNetworkConfiguration>("QNetworkConfiguration");
    qRegisterMetaType<QNetworkConfigurationPrivatePointer>("QNetworkConfigurationPrivatePointer");
}

void QNetworkConfigurationManagerPrivate::initialize()
{
    updateConfigurations();
}

QNetworkConfigurationManagerPrivate::~QNetworkConfigurationManagerPrivate()
{
    QMutexLocker locker(&mutex);

    qDeleteAll(sessionEngines);
}

void QNetworkConfigurationManagerPrivate::cleanup()
{
    deleteLater();
}

QNetworkConfiguration QNetworkConfigurationManagerPrivate::defaultConfiguration() const
{
    QMutexLocker locker(&mutex);

    foreach (QBearerEngine *engine, sessionEngines) {
        QNetworkConfigurationPrivatePointer ptr = engine->defaultConfiguration();
        if (ptr) {
            QNetworkConfiguration config;
            config.d = ptr;
            return config;
        }
    }

    // Engines don't have a default configuration.
    QNetworkConfigurationPrivatePointer defaultConfiguration;

    /*
        Find the perferred access point. The following priority order is used:

            1. Active Ethernet
            2. Active WLAN
            3. Active Other
            4. Discovered Ethernet
            5. Discovered WLAN
            6. Discovered Other
    */

    foreach (QBearerEngine *engine, sessionEngines) {
        QHash<QString, QNetworkConfigurationPrivatePointer>::Iterator it;
        QHash<QString, QNetworkConfigurationPrivatePointer>::Iterator end;

        QMutexLocker locker(&engine->mutex);

        for (it = engine->accessPointConfigurations.begin(),
             end = engine->accessPointConfigurations.end(); it != end; ++it) {
            QNetworkConfigurationPrivatePointer ptr = it.value();

            QMutexLocker configLocker(&ptr->mutex);
            QNetworkConfiguration::BearerType bearerType = ptr->bearerType;

            if ((ptr->state & QNetworkConfiguration::Discovered) == QNetworkConfiguration::Discovered) {
                if (!defaultConfiguration) {
                    defaultConfiguration = ptr;
                } else {
                    QMutexLocker defaultConfigLocker(&defaultConfiguration->mutex);

                    if (defaultConfiguration->state == ptr->state) {
                        switch (defaultConfiguration->bearerType) {
                        case QNetworkConfiguration::BearerEthernet:
                            // do nothing
                            break;
                        case QNetworkConfiguration::BearerWLAN:
                            // Ethernet beats WLAN
                            defaultConfiguration = ptr;
                            break;
                        default:
                            // Ethernet and WLAN beats other
                            if (bearerType == QNetworkConfiguration::BearerEthernet ||
                                bearerType == QNetworkConfiguration::BearerWLAN) {
                                defaultConfiguration = ptr;
                            }
                        }
                    } else {
                        // active beats discovered
                        if ((defaultConfiguration->state & QNetworkConfiguration::Active) !=
                            QNetworkConfiguration::Active) {
                            defaultConfiguration = ptr;
                        }
                    }
                }
            }
        }
    }

    // No Active InternetAccessPoint return first Discovered InternetAccessPoint.
    if (defaultConfiguration) {
        QNetworkConfiguration config;
        config.d = defaultConfiguration;
        return config;
    }

    return QNetworkConfiguration();
}

QList<QNetworkConfiguration> QNetworkConfigurationManagerPrivate::allConfigurations(QNetworkConfiguration::StateFlags filter) const
{
    QList<QNetworkConfiguration> result;

    QMutexLocker locker(&mutex);

    foreach (QBearerEngine *engine, sessionEngines) {
        QHash<QString, QNetworkConfigurationPrivatePointer>::Iterator it;
        QHash<QString, QNetworkConfigurationPrivatePointer>::Iterator end;

        QMutexLocker locker(&engine->mutex);

        //find all InternetAccessPoints
        for (it = engine->accessPointConfigurations.begin(),
             end = engine->accessPointConfigurations.end(); it != end; ++it) {
            QNetworkConfigurationPrivatePointer ptr = it.value();

            QMutexLocker configLocker(&ptr->mutex);

            if ((ptr->state & filter) == filter) {
                QNetworkConfiguration pt;
                pt.d = ptr;
                result << pt;
            }
        }
    }

    return result;
}

QNetworkConfiguration QNetworkConfigurationManagerPrivate::configurationFromIdentifier(const QString &identifier) const
{
    QNetworkConfiguration item;

    QMutexLocker locker(&mutex);

    foreach (QBearerEngine *engine, sessionEngines) {
        QMutexLocker locker(&engine->mutex);

        if (engine->accessPointConfigurations.contains(identifier)) {
            item.d = engine->accessPointConfigurations[identifier];
            return item;
        }
    }

    return item;
}

bool QNetworkConfigurationManagerPrivate::isOnline() const
{
    return !onlineConfigurations.isEmpty();
}

QNetworkConfigurationManager::Capabilities QNetworkConfigurationManagerPrivate::capabilities() const
{
    QNetworkConfigurationManager::Capabilities capFlags;

    foreach (QBearerEngine *engine, sessionEngines)
        capFlags |= engine->capabilities();

    return capFlags;
}

void QNetworkConfigurationManagerPrivate::configurationAdded(QNetworkConfigurationPrivatePointer ptr)
{
    QMutexLocker locker(&mutex);

    if (!firstUpdate) {
        QNetworkConfiguration item;
        item.d = ptr;
        emit configurationAdded(item);
    }

    ptr->mutex.lock();
    if (ptr->state == QNetworkConfiguration::Active) {
        ptr->mutex.unlock();
        onlineConfigurations.insert(ptr->id);
        if (!firstUpdate && onlineConfigurations.count() == 1)
            emit onlineStateChanged(true);
    } else {
        ptr->mutex.unlock();
    }
}

void QNetworkConfigurationManagerPrivate::configurationRemoved(QNetworkConfigurationPrivatePointer ptr)
{
    QMutexLocker locker(&mutex);

    ptr->mutex.lock();
    ptr->isValid = false;
    ptr->mutex.unlock();

    if (!firstUpdate) {
        QNetworkConfiguration item;
        item.d = ptr;
        emit configurationRemoved(item);
    }

    onlineConfigurations.remove(ptr->id);
    if (!firstUpdate && onlineConfigurations.isEmpty())
        emit onlineStateChanged(false);
}

void QNetworkConfigurationManagerPrivate::configurationChanged(QNetworkConfigurationPrivatePointer ptr)
{
    QMutexLocker locker(&mutex);

    if (!firstUpdate) {
        QNetworkConfiguration item;
        item.d = ptr;
        emit configurationChanged(item);
    }

    bool previous = !onlineConfigurations.isEmpty();

    ptr->mutex.lock();
    if (ptr->state == QNetworkConfiguration::Active)
        onlineConfigurations.insert(ptr->id);
    else
        onlineConfigurations.remove(ptr->id);
    ptr->mutex.unlock();

    bool online = !onlineConfigurations.isEmpty();

    if (!firstUpdate && online != previous)
        emit onlineStateChanged(online);
}

void QNetworkConfigurationManagerPrivate::updateConfigurations()
{
    QMutexLocker locker(&mutex);

    if (firstUpdate) {
        if (qobject_cast<QBearerEngine *>(sender()))
            return;

        updating = false;

#ifndef QT_NO_LIBRARY
        QBearerEngine *generic = 0;

        QFactoryLoader *l = loader();
        foreach (const QString &key, l->keys()) {
            QBearerEnginePlugin *plugin = qobject_cast<QBearerEnginePlugin *>(l->instance(key));
            if (plugin) {
                QBearerEngine *engine = plugin->create(key);
                if (!engine)
                    continue;

                if (key == QLatin1String("generic"))
                    generic = engine;
                else
                    sessionEngines.append(engine);

                connect(engine, SIGNAL(updateCompleted()),
                        this, SLOT(updateConfigurations()),
                        Qt::QueuedConnection);
                connect(engine, SIGNAL(configurationAdded(QNetworkConfigurationPrivatePointer)),
                        this, SLOT(configurationAdded(QNetworkConfigurationPrivatePointer)),
                        Qt::QueuedConnection);
                connect(engine, SIGNAL(configurationRemoved(QNetworkConfigurationPrivatePointer)),
                        this, SLOT(configurationRemoved(QNetworkConfigurationPrivatePointer)),
                        Qt::QueuedConnection);
                connect(engine, SIGNAL(configurationChanged(QNetworkConfigurationPrivatePointer)),
                        this, SLOT(configurationChanged(QNetworkConfigurationPrivatePointer)),
                        Qt::QueuedConnection);
            }
        }

        if (generic)
            sessionEngines.append(generic);
#endif // QT_NO_LIBRARY
    }

    QBearerEngine *engine = qobject_cast<QBearerEngine *>(sender());
    if (engine && !updatingEngines.isEmpty())
        updatingEngines.remove(engine);

    if (updating && updatingEngines.isEmpty()) {
        updating = false;
        emit configurationUpdateComplete();
    }

    if (engine && !pollingEngines.isEmpty()) {
        pollingEngines.remove(engine);
        if (pollingEngines.isEmpty())
            startPolling();
    }

    if (firstUpdate) {
        firstUpdate = false;
        foreach (QBearerEngine* engine, sessionEngines) {
            QMetaObject::invokeMethod(engine, "initialize");
        }
    }
}

void QNetworkConfigurationManagerPrivate::performAsyncConfigurationUpdate()
{
    QMutexLocker locker(&mutex);

    if (sessionEngines.isEmpty()) {
        emit configurationUpdateComplete();
        return;
    }

    updating = true;

    foreach (QBearerEngine *engine, sessionEngines) {
        updatingEngines.insert(engine);
        QMetaObject::invokeMethod(engine, "requestUpdate");
    }
}

QList<QBearerEngine *> QNetworkConfigurationManagerPrivate::engines() const
{
    return sessionEngines;
}

void QNetworkConfigurationManagerPrivate::startPolling()
{
    QMutexLocker locker(&mutex);

    if(!pollTimer) {
        pollTimer = new QTimer(this);
        pollTimer->setInterval(10000);
        pollTimer->setSingleShot(true);
        connect(pollTimer, SIGNAL(timeout()), this, SLOT(pollEngines()));
    }

    if(pollTimer->isActive())
        return;

    foreach (QBearerEngine *engine, sessionEngines) {
        if (engine->requiresPolling() && (forcedPolling || engine->configurationsInUse())) {
            pollTimer->start();
            break;
        }
    }
}

void QNetworkConfigurationManagerPrivate::pollEngines()
{
    QMutexLocker locker(&mutex);

    foreach (QBearerEngine *engine, sessionEngines) {
        if (engine->requiresPolling() && (forcedPolling || engine->configurationsInUse())) {
            pollingEngines.insert(engine);
            QMetaObject::invokeMethod(engine, "requestUpdate");
        }
    }
}

void QNetworkConfigurationManagerPrivate::enablePolling()
{
    QMutexLocker locker(&mutex);

    ++forcedPolling;

    if (forcedPolling == 1)
        QMetaObject::invokeMethod(this, "startPolling");
}

void QNetworkConfigurationManagerPrivate::disablePolling()
{
    QMutexLocker locker(&mutex);

    --forcedPolling;
}

QT_END_NAMESPACE

#include "moc_qnetworkconfigmanager_p.h"

#endif // QT_NO_BEARERMANAGEMENT


