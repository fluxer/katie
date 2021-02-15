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

#ifndef QDBUSBUS_H
#define QDBUSBUS_H

#include <QtCore/qstringlist.h>

#include <QtDBus/qdbusabstractinterface.h>
#include <QtDBus/qdbusreply.h>



QT_BEGIN_NAMESPACE


class QDBusConnection;
class QString;
class QByteArray;

/*
 * Proxy class for interface org.freedesktop.DBus
 */
class Q_DBUS_EXPORT QDBusConnectionInterface: public QDBusAbstractInterface
{
    Q_OBJECT
    Q_ENUMS(ServiceQueueOptions ServiceReplacementOptions RegisterServiceReply)
    friend class QDBusConnectionPrivate;
    static inline const char *staticInterfaceName();

    explicit QDBusConnectionInterface(const QDBusConnection &connection, QObject *parent);
    ~QDBusConnectionInterface();

    Q_PROPERTY(QDBusReply<QStringList> registeredServiceNames READ registeredServiceNames)

public:
    enum ServiceQueueOptions {
        DontQueueService,
        QueueService,
        ReplaceExistingService
    };
    enum ServiceReplacementOptions {
        DontAllowReplacement,
        AllowReplacement
    };
    enum RegisterServiceReply {
        ServiceNotRegistered = 0,
        ServiceRegistered,
        ServiceQueued
    };

public Q_SLOTS:
    QDBusReply<QStringList> registeredServiceNames() const;
    QDBusReply<bool> isServiceRegistered(const QString &serviceName) const;
    QDBusReply<QString> serviceOwner(const QString &name) const;
    QDBusReply<bool> unregisterService(const QString &serviceName);
    QDBusReply<QDBusConnectionInterface::RegisterServiceReply> registerService(const QString &serviceName,
                                                     ServiceQueueOptions qoption = DontQueueService,
                                                     ServiceReplacementOptions roption = DontAllowReplacement);

    QDBusReply<uint> servicePid(const QString &serviceName) const;
    QDBusReply<uint> serviceUid(const QString &serviceName) const;

    QDBusReply<void> startService(const QString &name);

Q_SIGNALS:
    void serviceRegistered(const QString &service);
    void serviceUnregistered(const QString &service);
    void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);
    void callWithCallbackFailed(const QDBusError &error, const QDBusMessage &call);

    // internal signals
    // do not use
    void NameAcquired(const QString &);
    void NameLost(const QString &);
    void NameOwnerChanged(const QString &, const QString &, const QString &);
protected:
    void connectNotify(const char *);
    void disconnectNotify(const char *);
};

QT_END_NAMESPACE

Q_DECLARE_BUILTIN_METATYPE(QDBusConnectionInterface::RegisterServiceReply, UInt)


#endif
