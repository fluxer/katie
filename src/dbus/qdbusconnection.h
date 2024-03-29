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

#ifndef QDBUSCONNECTION_H
#define QDBUSCONNECTION_H

#include <QtCore/qstring.h>
#include <QtCore/qobject.h>



QT_BEGIN_NAMESPACE


namespace QDBus
{
    enum CallMode {
        NoBlock,
        Block,
        BlockWithGui,
        AutoDetect
    };
}

class QDBusInterface;
class QDBusError;
class QDBusMessage;
class QDBusPendingCall;
class QDBusConnectionInterface;
class QObject;

class QDBusConnectionPrivate;
class Q_DBUS_EXPORT QDBusConnection
{
    Q_GADGET
    Q_ENUMS(BusType UnregisterMode)
    Q_FLAGS(RegisterOptions)
public:
    enum BusType { SessionBus, SystemBus, ActivationBus };
    enum RegisterOption {
        ExportAdaptors = 0x01,

        ExportScriptableSlots = 0x10,
        ExportScriptableSignals = 0x20,
        ExportScriptableProperties = 0x40,
        ExportScriptableInvokables = 0x80,
        ExportScriptableContents = 0xf0,

        ExportNonScriptableSlots = 0x100,
        ExportNonScriptableSignals = 0x200,
        ExportNonScriptableProperties = 0x400,
        ExportNonScriptableInvokables = 0x800,
        ExportNonScriptableContents = 0xf00,

        ExportAllSlots = ExportScriptableSlots|ExportNonScriptableSlots,
        ExportAllSignals = ExportScriptableSignals|ExportNonScriptableSignals,
        ExportAllProperties = ExportScriptableProperties|ExportNonScriptableProperties,
        ExportAllInvokables = ExportScriptableInvokables|ExportNonScriptableInvokables,
        ExportAllContents = ExportScriptableContents|ExportNonScriptableContents,

        ExportChildObjects = 0x1000
        // Reserved = 0xff000000
    };
    enum UnregisterMode {
        UnregisterNode,
        UnregisterTree
    };
    Q_DECLARE_FLAGS(RegisterOptions, RegisterOption)

    enum ConnectionCapability {
        UnixFileDescriptorPassing = 0x0001
    };
    Q_DECLARE_FLAGS(ConnectionCapabilities, ConnectionCapability)

    QDBusConnection(const QString &name);
    QDBusConnection(const QDBusConnection &other);
    ~QDBusConnection();

    QDBusConnection &operator=(const QDBusConnection &other);

    bool isConnected() const;
    QString baseService() const;
    QDBusError lastError() const;
    QString name() const;
    ConnectionCapabilities connectionCapabilities() const;

    bool send(const QDBusMessage &message) const;
    bool callWithCallback(const QDBusMessage &message, QObject *receiver,
                          const char *returnMethod, const char *errorMethod,
                          int timeout = -1) const;
    QT_DEPRECATED bool callWithCallback(const QDBusMessage &message, QObject *receiver,
                                        const char *slot, int timeout = -1) const;
    QDBusMessage call(const QDBusMessage &message, QDBus::CallMode mode = QDBus::Block,
                      int timeout = -1) const;
    QDBusPendingCall asyncCall(const QDBusMessage &message, int timeout = -1) const;

    bool connect(const QString &service, const QString &path, const QString &interface,
                 const QString &name, QObject *receiver, const char *slot);
    bool connect(const QString &service, const QString &path, const QString &interface,
                 const QString &name, const QString& signature,
                 QObject *receiver, const char *slot);
    bool connect(const QString &service, const QString &path, const QString &interface,
                 const QString &name, const QStringList &argumentMatch, const QString& signature,
                 QObject *receiver, const char *slot);

    bool disconnect(const QString &service, const QString &path, const QString &interface,
                    const QString &name, QObject *receiver, const char *slot);
    bool disconnect(const QString &service, const QString &path, const QString &interface,
                    const QString &name, const QString& signature,
                    QObject *receiver, const char *slot);
    bool disconnect(const QString &service, const QString &path, const QString &interface,
                    const QString &name, const QStringList &argumentMatch, const QString& signature,
                    QObject *receiver, const char *slot);

    bool registerObject(const QString &path, QObject *object,
                        RegisterOptions options = ExportAdaptors);
    void unregisterObject(const QString &path, UnregisterMode mode = UnregisterNode);
    QObject *objectRegisteredAt(const QString &path) const;

    bool registerService(const QString &serviceName);
    bool unregisterService(const QString &serviceName);

    QDBusConnectionInterface *interface() const;

    static QDBusConnection connectToBus(BusType type, const QString &name);
    static QDBusConnection connectToBus(const QString &address, const QString &name);
    static QDBusConnection connectToPeer(const QString &address, const QString &name);
    static void disconnectFromBus(const QString &name);
    static void disconnectFromPeer(const QString &name);

    static QByteArray localMachineId();

    static QDBusConnection sessionBus();
    static QDBusConnection systemBus();

    static QDBusConnection sender();

protected:
    explicit QDBusConnection(QDBusConnectionPrivate *dd);

private:
    friend class QDBusConnectionPrivate;
    QDBusConnectionPrivate *d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDBusConnection::RegisterOptions)

QT_END_NAMESPACE


#endif
