/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the plugins of the Katie Toolkit.
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

#ifndef QNETWORKMANAGERENGINE_P_H
#define QNETWORKMANAGERENGINE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "../qbearerengine_impl.h"

#include "qnetworkmanagerservice.h"

#include <QMap>
#include <QVariant>

#ifndef QT_NO_BEARERMANAGEMENT
#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

class QNetworkManagerEngine : public QBearerEngineImpl
{
    Q_OBJECT

public:
    QNetworkManagerEngine(QObject *parent = Q_NULLPTR);
    ~QNetworkManagerEngine();

    bool networkManagerAvailable() const;

    QString getInterfaceFromId(const QString &id);
    bool hasIdentifier(const QString &id) const;

    void connectToId(const QString &id);
    void disconnectFromId(const QString &id);

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void requestUpdate();

    QNetworkSession::State sessionStateForId(const QString &id);

    quint64 bytesWritten(const QString &id);
    quint64 bytesReceived(const QString &id);
    quint64 startTime(const QString &id);

    QNetworkConfigurationManager::Capabilities capabilities() const;

    QNetworkSessionPrivate *createSessionBackend();

    QNetworkConfigurationPrivatePointer defaultConfiguration();

private Q_SLOTS:
    void interfacePropertiesChanged(const QString &path,
                                    const QMap<QString, QVariant> &properties);
    void activeConnectionPropertiesChanged(const QString &path,
                                           const QMap<QString, QVariant> &properties);
    void devicePropertiesChanged(const QString &path,
                                 const QMap<QString, QVariant> &properties);

    void deviceAdded(const QDBusObjectPath &path);
    void deviceRemoved(const QDBusObjectPath &path);

    void newConnection(const QDBusObjectPath &path, QNetworkManagerSettings *settings = 0);
    void removeConnection(const QString &path);
    void updateConnection(const QNmSettingsMap &settings);
    void activationFinished(QDBusPendingCallWatcher *watcher);

    void newAccessPoint(const QString &path, const QDBusObjectPath &objectPath);
    void removeAccessPoint(const QString &path, const QDBusObjectPath &objectPath);
    void updateAccessPoint(const QMap<QString, QVariant> &map);

private:
    QNetworkConfigurationPrivate *parseConnection(const QString &service,
                                                  const QString &settingsPath,
                                                  const QNmSettingsMap &map);
    QNetworkManagerSettingsConnection *connectionFromId(const QString &id) const;

private:
    QNetworkManagerInterface *interface;
    QNetworkManagerSettings *systemSettings;
    QNetworkManagerSettings *userSettings;
    QHash<QString, QNetworkManagerInterfaceDeviceWireless *> wirelessDevices;
    QHash<QString, QNetworkManagerConnectionActive *> activeConnections;
    QList<QNetworkManagerSettingsConnection *> connections;
    QList<QNetworkManagerInterfaceAccessPoint *> accessPoints;
    QList<QNetworkManagerInterfaceAccessPoint *> configuredAccessPoints;
};

QT_END_NAMESPACE

#endif // QT_NO_DBUS
#endif // QT_NO_BEARERMANAGEMENT

#endif

