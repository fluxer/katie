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

#include <QObject>
#include <QList>
#include <QDebug>
#include <QtDBus/qdbusmetatype.h>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusError>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>
#include <QtDBus/qdbuspendingcall.h>
#include <QtDBus/qdbusextratypes.h>
#include <QtDBus/QDBusPendingCall>

#include "qofonoservice_linux_p.h"

#ifndef QT_NO_BEARERMANAGEMENT
#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

QDBusArgument &operator<<(QDBusArgument &argument, const ObjectPathProperties &item)
{
    argument.beginStructure();
    argument << item.path << item.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ObjectPathProperties &item)
{
    argument.beginStructure();
    argument >> item.path >> item.properties;
    argument.endStructure();
    return argument;
}

static QDBusConnection ofonoConnection = QDBusConnection::systemBus();


QOfonoManagerInterface::QOfonoManagerInterface( QObject *parent)
        : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                                 QLatin1String(OFONO_MANAGER_PATH),
                                 OFONO_MANAGER_INTERFACE,
                                 QDBusConnection::systemBus(), parent)
{
    qDBusRegisterMetaType<ObjectPathProperties>();
    qDBusRegisterMetaType<PathPropertiesList>();
}

QOfonoManagerInterface::~QOfonoManagerInterface()
{
}

QList <QDBusObjectPath> QOfonoManagerInterface::getModems()
{
    QList <QDBusObjectPath> modemList;
    QList<QVariant> argumentList;
    QDBusReply<PathPropertiesList > reply = this->asyncCallWithArgumentList(QLatin1String("GetModems"), argumentList);
    if (reply.isValid()) {
        foreach (ObjectPathProperties modem, reply.value()) {
            modemList << modem.path;
        }
    }

    return modemList;
}

QDBusObjectPath QOfonoManagerInterface::currentModem()
{
    QList<QDBusObjectPath> modems = getModems();
    foreach (const QDBusObjectPath modem, modems) {
        QOfonoModemInterface device(modem.path());
        if (device.isPowered() && device.isOnline())
        return modem;;
    }
    return QDBusObjectPath();
}


void QOfonoManagerInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                               QLatin1String(OFONO_MANAGER_PATH),
                               QLatin1String(OFONO_MANAGER_INTERFACE),
                               QLatin1String("PropertyChanged"),
                               this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QOfonoDBusHelper *helper;
        helper = new QOfonoDBusHelper(this);

        ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                QLatin1String(OFONO_MANAGER_PATH),
                                QLatin1String(OFONO_MANAGER_INTERFACE),
                                QLatin1String("PropertyChanged"),
                                helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)));
    }
}

QVariant QOfonoManagerInterface::getProperty(const QString &property)
{
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        return map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return QVariant();
}

QVariantMap QOfonoManagerInterface::getProperties()
{
    QDBusReply<QVariantMap > reply = this->call(QLatin1String("GetProperties"));
    if (reply.isValid())
        return reply.value();
    else
        return QVariantMap();
}

QOfonoDBusHelper::QOfonoDBusHelper(QObject * parent)
        : QObject(parent)
{
}

QOfonoDBusHelper::~QOfonoDBusHelper()
{
}

void QOfonoDBusHelper::propertyChanged(const QString &item, const QDBusVariant &var)
{
    QDBusMessage msg = this->message();
    Q_EMIT propertyChangedContext(msg.path() ,item, var);
}


QOfonoModemInterface::QOfonoModemInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                             dbusPathName,
                             OFONO_MODEM_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QOfonoModemInterface::~QOfonoModemInterface()
{
}

bool QOfonoModemInterface::isPowered()
{
    QVariant var = getProperty(QLatin1String("Powered"));
    return qdbus_cast<bool>(var);
}

bool QOfonoModemInterface::isOnline()
{
    QVariant var = getProperty(QLatin1String("Online"));
    return qdbus_cast<bool>(var);
}

QString QOfonoModemInterface::getName()
{
    QVariant var = getProperty(QLatin1String("Name"));
    return qdbus_cast<QString>(var);
}

QString QOfonoModemInterface::getManufacturer()
{
    QVariant var = getProperty(QLatin1String("Manufacturer"));
    return qdbus_cast<QString>(var);

}

QString QOfonoModemInterface::getModel()
{

    QVariant var = getProperty(QLatin1String("Model"));
    return qdbus_cast<QString>(var);
}

QString QOfonoModemInterface::getRevision()
{
    QVariant var = getProperty(QLatin1String("Revision"));
    return qdbus_cast<QString>(var);

}
QString QOfonoModemInterface::getSerial()
{
    QVariant var = getProperty(QLatin1String("Serial"));
    return qdbus_cast<QString>(var);

}

QStringList QOfonoModemInterface::getFeatures()
{
    //sms, sim
    QVariant var = getProperty(QLatin1String("Features"));
    return qdbus_cast<QStringList>(var);
}

QStringList QOfonoModemInterface::getInterfaces()
{
    QVariant var = getProperty(QLatin1String("Interfaces"));
    return qdbus_cast<QStringList>(var);
}

QString QOfonoModemInterface::defaultInterface()
{
    foreach (const QString &modem,getInterfaces()) {
     return modem;
    }
    return QString();
}


void QOfonoModemInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
            if (!connection().connect(QLatin1String(OFONO_SERVICE),
                                   this->path(),
                                   QLatin1String(OFONO_MODEM_INTERFACE),
                                   QLatin1String("PropertyChanged"),
                                   this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
                qWarning() << "PropertyCHanged not connected";
            }
        }

        if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
            QOfonoDBusHelper *helper;
            helper = new QOfonoDBusHelper(this);

            ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                    this->path(),
                                    QLatin1String(OFONO_MODEM_INTERFACE),
                                    QLatin1String("PropertyChanged"),
                                    helper,SLOT(propertyChanged(QString,QDBusVariant)));


            QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                    this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
        }}

QVariantMap QOfonoModemInterface::getProperties()
{
    QDBusReply<QVariantMap > reply = this->call(QLatin1String("GetProperties"));
    return reply.value();
}

QVariant QOfonoModemInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return var;
}


QOfonoNetworkRegistrationInterface::QOfonoNetworkRegistrationInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                             dbusPathName,
                             OFONO_NETWORK_REGISTRATION_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QOfonoNetworkRegistrationInterface::~QOfonoNetworkRegistrationInterface()
{
}

QString QOfonoNetworkRegistrationInterface::getStatus()
{
    /*
                "unregistered"  Not registered to any network
                "registered"    Registered to home network
                "searching"     Not registered, but searching
                "denied"        Registration has been denied
                "unknown"       Status is unknown
                "roaming"       Registered, but roaming*/
    QVariant var = getProperty(QLatin1String("Status"));
    return qdbus_cast<QString>(var);
}

quint16 QOfonoNetworkRegistrationInterface::getLac()
{
    QVariant var = getProperty(QLatin1String("LocationAreaCode"));
    return var.value<quint16>();
}


quint32 QOfonoNetworkRegistrationInterface::getCellId()
{
    QVariant var = getProperty(QLatin1String("CellId"));
    return var.value<quint32>();
}

QString QOfonoNetworkRegistrationInterface::getTechnology()
{
    // "gsm", "edge", "umts", "hspa","lte"
    QVariant var = getProperty(QLatin1String("Technology"));
    return qdbus_cast<QString>(var);
}

QString QOfonoNetworkRegistrationInterface::getOperatorName()
{
    QVariant var = getProperty(QLatin1String("Name"));
    return qdbus_cast<QString>(var);
}

int QOfonoNetworkRegistrationInterface::getSignalStrength()
{
    QVariant var = getProperty(QLatin1String("Strength"));
    return qdbus_cast<int>(var);

}

QString QOfonoNetworkRegistrationInterface::getBaseStation()
{
    QVariant var = getProperty(QLatin1String("BaseStation"));
    return qdbus_cast<QString>(var);
}

QList <QDBusObjectPath> QOfonoNetworkRegistrationInterface::getOperators()
{
    QList <QDBusObjectPath> operatorList;
    QList<QVariant> argumentList;
    QDBusReply<PathPropertiesList > reply = this->asyncCallWithArgumentList(QLatin1String("GetOperators"),
                                                                                argumentList);
    if (reply.isValid()) {
        foreach (ObjectPathProperties netop, reply.value()) {
            operatorList << netop.path;
        }
    }
    return operatorList;
}

void QOfonoNetworkRegistrationInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                               this->path(),
                               QLatin1String(OFONO_NETWORK_REGISTRATION_INTERFACE),
                               QLatin1String("PropertyChanged"),
                               this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QOfonoDBusHelper *helper;
        helper = new QOfonoDBusHelper(this);

        ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                this->path(),
                                QLatin1String(OFONO_NETWORK_REGISTRATION_INTERFACE),
                                QLatin1String("PropertyChanged"),
                                helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariant QOfonoNetworkRegistrationInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return var;
}

QVariantMap QOfonoNetworkRegistrationInterface::getProperties()
{
    QDBusReply<QVariantMap > reply =  this->call(QLatin1String("GetProperties"));
    return reply.value();
}



QOfonoNetworkOperatorInterface::QOfonoNetworkOperatorInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                             dbusPathName,
                             OFONO_NETWORK_OPERATOR_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QOfonoNetworkOperatorInterface::~QOfonoNetworkOperatorInterface()
{
}

QString QOfonoNetworkOperatorInterface::getName()
{
    QVariant var = getProperty(QLatin1String("Name"));
    return qdbus_cast<QString>(var);
}

QString QOfonoNetworkOperatorInterface::getStatus()
{
    // "unknown", "available", "current" and "forbidden"
    QVariant var = getProperty(QLatin1String("Status"));
    return qdbus_cast<QString>(var);
}

QString QOfonoNetworkOperatorInterface::getMcc()
{
    QVariant var = getProperty(QLatin1String("MobileCountryCode"));
    return qdbus_cast<QString>(var);
}

QString QOfonoNetworkOperatorInterface::getMnc()
{
    QVariant var = getProperty(QLatin1String("MobileNetworkCode"));
    return qdbus_cast<QString>(var);
}

QStringList QOfonoNetworkOperatorInterface::getTechnologies()
{
    QVariant var = getProperty(QLatin1String("Technologies"));
    return qdbus_cast<QStringList>(var);
}

void QOfonoNetworkOperatorInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                               this->path(),
                               QLatin1String(OFONO_NETWORK_OPERATOR_INTERFACE),
                               QLatin1String("PropertyChanged"),
                               this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QOfonoDBusHelper *helper;
        helper = new QOfonoDBusHelper(this);

        ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                this->path(),
                                QLatin1String(OFONO_NETWORK_OPERATOR_INTERFACE),
                                QLatin1String("PropertyChanged"),
                                helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariant QOfonoNetworkOperatorInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return var;
}

QVariantMap QOfonoNetworkOperatorInterface::getProperties()
{
    QDBusReply<QVariantMap > reply =  this->call(QLatin1String("GetProperties"));
    return reply.value();
}

QOfonoSimInterface::QOfonoSimInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                             dbusPathName,
                             OFONO_SIM_MANAGER_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QOfonoSimInterface::~QOfonoSimInterface()
{
}

bool QOfonoSimInterface::isPresent()
{
    QVariant var = getProperty(QLatin1String("Present"));
    return qdbus_cast<bool>(var);
}

QString QOfonoSimInterface::getHomeMcc()
{
    QVariant var = getProperty(QLatin1String("MobileCountryCode"));
    return qdbus_cast<QString>(var);
}

QString QOfonoSimInterface::getHomeMnc()
{
    QVariant var = getProperty(QLatin1String("MobileNetworkCode"));
    return qdbus_cast<QString>(var);
}

//    QStringList subscriberNumbers();
//    QMap<QString,QString> serviceNumbers();
QString QOfonoSimInterface::pinRequired()
{
    QVariant var = getProperty(QLatin1String("PinRequired"));
    return qdbus_cast<QString>(var);
}

QString QOfonoSimInterface::lockedPins()
{
    QVariant var = getProperty(QLatin1String("LockedPins"));
    return qdbus_cast<QString>(var);
}

QString QOfonoSimInterface::cardIdentifier()
{
    QVariant var = getProperty(QLatin1String("CardIdentifier"));
    return qdbus_cast<QString>(var);
}

void QOfonoSimInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                               this->path(),
                               QLatin1String(OFONO_SIM_MANAGER_INTERFACE),
                               QLatin1String("PropertyChanged"),
                               this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QOfonoDBusHelper *helper;
        helper = new QOfonoDBusHelper(this);

        ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                this->path(),
                                QLatin1String(OFONO_SIM_MANAGER_INTERFACE),
                                QLatin1String("PropertyChanged"),
                                helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariant QOfonoSimInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return var;
}

QVariantMap QOfonoSimInterface::getProperties()
{
    QDBusReply<QVariantMap > reply =  this->call(QLatin1String("GetProperties"));
    return reply.value();
}

QOfonoDataConnectionManagerInterface::QOfonoDataConnectionManagerInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                             dbusPathName,
                             OFONO_DATA_CONNECTION_MANAGER_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QOfonoDataConnectionManagerInterface::~QOfonoDataConnectionManagerInterface()
{
}

QList<QDBusObjectPath> QOfonoDataConnectionManagerInterface::getPrimaryContexts()
{
    QList <QDBusObjectPath> contextList;
    QList<QVariant> argumentList;
    QDBusReply<PathPropertiesList > reply = this->asyncCallWithArgumentList(QLatin1String("GetContexts"),
                                                                         argumentList);
    if (reply.isValid()) {
        foreach (ObjectPathProperties context, reply.value()) {
            contextList << context.path;
        }
    }
    return contextList;
}

bool QOfonoDataConnectionManagerInterface::isAttached()
{
    QVariant var = getProperty(QLatin1String("Attached"));
    return qdbus_cast<bool>(var);
}

bool QOfonoDataConnectionManagerInterface::isRoamingAllowed()
{
    QVariant var = getProperty(QLatin1String("RoamingAllowed"));
    return qdbus_cast<bool>(var);
}

bool QOfonoDataConnectionManagerInterface::isPowered()
{
    QVariant var = getProperty(QLatin1String("Powered"));
    return qdbus_cast<bool>(var);
}

void QOfonoDataConnectionManagerInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                               this->path(),
                               QLatin1String(OFONO_DATA_CONNECTION_MANAGER_INTERFACE),
                               QLatin1String("PropertyChanged"),
                               this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QOfonoDBusHelper *helper;
        helper = new QOfonoDBusHelper(this);

        ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                this->path(),
                                QLatin1String(OFONO_DATA_CONNECTION_MANAGER_INTERFACE),
                                QLatin1String("PropertyChanged"),
                                helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariant QOfonoDataConnectionManagerInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return var;
}

QVariantMap QOfonoDataConnectionManagerInterface::getProperties()
{
    QDBusReply<QVariantMap > reply =  this->call(QLatin1String("GetProperties"));
    return reply.value();
}

QOfonoPrimaryDataContextInterface::QOfonoPrimaryDataContextInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                             dbusPathName,
                             OFONO_DATA_CONTEXT_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QOfonoPrimaryDataContextInterface::~QOfonoPrimaryDataContextInterface()
{
}

bool QOfonoPrimaryDataContextInterface::isActive()
{
    QVariant var = getProperty(QLatin1String("Active"));
    return qdbus_cast<bool>(var);
}

QString QOfonoPrimaryDataContextInterface::getApName()
{
    QVariant var = getProperty(QLatin1String("AccessPointName"));
    return qdbus_cast<QString>(var);
}

QString QOfonoPrimaryDataContextInterface::getType()
{
    QVariant var = getProperty(QLatin1String("Type"));
    return qdbus_cast<QString>(var);
}

QString QOfonoPrimaryDataContextInterface::getName()
{
    QVariant var = getProperty(QLatin1String("Name"));
    return qdbus_cast<QString>(var);
}

QVariantMap QOfonoPrimaryDataContextInterface::getSettings()
{
    QVariant var = getProperty(QLatin1String("Settings"));
    return qdbus_cast<QVariantMap>(var);
}

QString QOfonoPrimaryDataContextInterface::getInterface()
{
    QVariant var = getProperty(QLatin1String("Interface"));
    return qdbus_cast<QString>(var);
}

QString QOfonoPrimaryDataContextInterface::getAddress()
{
    QVariant var = getProperty(QLatin1String("Address"));
    return qdbus_cast<QString>(var);
}

bool QOfonoPrimaryDataContextInterface::setActive(bool on)
{
//    this->setProperty("Active", QVariant(on));

    return setProp(QLatin1String("Active"), qVariantFromValue(on));
}

bool QOfonoPrimaryDataContextInterface::setApn(const QString &name)
{
    return setProp(QLatin1String("AccessPointName"), QVariant::fromValue(name));
}

void QOfonoPrimaryDataContextInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                               this->path(),
                               QLatin1String(OFONO_DATA_CONTEXT_INTERFACE),
                               QLatin1String("PropertyChanged"),
                               this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QOfonoDBusHelper *helper;
        helper = new QOfonoDBusHelper(this);

        ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                this->path(),
                                QLatin1String(OFONO_DATA_CONTEXT_INTERFACE),
                                QLatin1String("PropertyChanged"),
                                helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariant QOfonoPrimaryDataContextInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return var;
}

QVariantMap QOfonoPrimaryDataContextInterface::getProperties()
{
    QDBusReply<QVariantMap > reply =  this->call(QLatin1String("GetProperties"));
    return reply.value();
}

bool QOfonoPrimaryDataContextInterface::setProp(const QString &property, const QVariant &var)
{
    QList<QVariant> args;
    args << qVariantFromValue(property) << qVariantFromValue(QDBusVariant(var));

    QDBusMessage reply = this->callWithArgumentList(QDBus::AutoDetect,
                                                    QLatin1String("SetProperty"),
                                                    args);
    bool ok = true;
    if (reply.type() != QDBusMessage::ReplyMessage) {
        qWarning() << reply.errorMessage();
        ok = false;
    }
    qWarning() << reply.errorMessage();
    return ok;
}

QOfonoSmsInterface::QOfonoSmsInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(OFONO_SERVICE),
                             dbusPathName,
                             OFONO_SMS_MANAGER_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QOfonoSmsInterface::~QOfonoSmsInterface()
{
}

void QOfonoSmsInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                                 this->path(),
                                 QLatin1String(OFONO_SMS_MANAGER_INTERFACE),
                                 QLatin1String("PropertyChanged"),
                                 this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QOfonoDBusHelper *helper;
        helper = new QOfonoDBusHelper(this);

        ofonoConnection.connect(QLatin1String(OFONO_SERVICE),
                                this->path(),
                                QLatin1String(OFONO_SMS_MANAGER_INTERFACE),
                                QLatin1String("PropertyChanged"),
                                helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                         this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)));
    }

    if (strcmp(signal + 1, "immediateMessage(QString,QVariantMap)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                                 this->path(),
                                 QLatin1String(OFONO_SMS_MANAGER_INTERFACE),
                                 QLatin1String("ImmediateMessage"),
                                 this,SIGNAL(immediateMessage(QString,QVariantMap)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "incomingMessage(QString,QVariantMap)") == 0) {
        if (!connection().connect(QLatin1String(OFONO_SERVICE),
                                 this->path(),
                                 QLatin1String(OFONO_SMS_MANAGER_INTERFACE),
                                 QLatin1String("IncomingMessage"),
                                 this,SIGNAL(incomingMessage(QString,QVariantMap)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }
}

QVariant QOfonoSmsInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << Q_FUNC_INFO << "does not contain" << property;
    }
    return var;
}

QVariantMap QOfonoSmsInterface::getProperties()
{
    QDBusReply<QVariantMap > reply = this->call(QLatin1String("GetProperties"));
    return reply.value();
}

void QOfonoSmsInterface::sendMessage(const QString &to, const QString &message)
{
    QDBusReply<QString> reply =  this->call(QLatin1String("SendMessage"),
                                            QVariant::fromValue(to),
                                            QVariant::fromValue(message));
    if (reply.error().type() == QDBusError::InvalidArgs)
        qWarning() << reply.error().message();
}

QT_END_NAMESPACE

#endif // QT_NO_DBUS
#endif // QT_NO_BEARERMANAGEMENT
#include "moc_qofonoservice_linux_p.h"
