/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDBus module of the Katie Toolkit.
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

#ifndef QDBUSSERVICEWATCHER_H
#define QDBUSSERVICEWATCHER_H

#include <QtCore/qobject.h>



QT_BEGIN_NAMESPACE


class QDBusConnection;

class QDBusServiceWatcherPrivate;
class Q_DBUS_EXPORT QDBusServiceWatcher: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList watchedServices READ watchedServices WRITE setWatchedServices)
    Q_PROPERTY(WatchMode watchMode READ watchMode WRITE setWatchMode)
public:
    enum WatchModeFlag {
        WatchForRegistration = 0x01,
        WatchForUnregistration = 0x02,
        WatchForOwnerChange = 0x03
    };
    Q_DECLARE_FLAGS(WatchMode, WatchModeFlag)

    explicit QDBusServiceWatcher(QObject *parent = nullptr);
    QDBusServiceWatcher(const QString &service, const QDBusConnection &connection,
                        WatchMode watchMode = WatchForOwnerChange, QObject *parent = nullptr);
    ~QDBusServiceWatcher();

    QStringList watchedServices() const;
    void setWatchedServices(const QStringList &services);
    void addWatchedService(const QString &newService);
    bool removeWatchedService(const QString &service);

    WatchMode watchMode() const;
    void setWatchMode(WatchMode mode);

    QDBusConnection connection() const;
    void setConnection(const QDBusConnection &connection);

Q_SIGNALS:
    void serviceRegistered(const QString &service);
    void serviceUnregistered(const QString &service);
    void serviceOwnerChanged(const QString &service, const QString &oldOwner, const QString &newOwner);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_serviceOwnerChanged(QString,QString,QString))
    Q_DISABLE_COPY(QDBusServiceWatcher)
    Q_DECLARE_PRIVATE(QDBusServiceWatcher)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDBusServiceWatcher::WatchMode)

QT_END_NAMESPACE


#endif // QDBUSSERVICEWATCHER_H
