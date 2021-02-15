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

#include "qconnmanservice_linux_p.h"

#ifndef QT_NO_BEARERMANAGEMENT
#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE


QDBusArgument &operator<<(QDBusArgument &argument, const ConnmanMap &map)
{
    argument.beginStructure();
    argument << map.objectPath << map.propertyMap;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ConnmanMap &map)
{
    argument.beginStructure();
    argument >> map.objectPath >> map.propertyMap;
    argument.endStructure();
    return argument;
}

static QDBusConnection connmanConnection = QDBusConnection::systemBus();


QConnmanManagerInterface::QConnmanManagerInterface( QObject *parent)
        : QDBusAbstractInterface(QLatin1String(CONNMAN_SERVICE),
                                 QLatin1String(CONNMAN_MANAGER_PATH),
                                 CONNMAN_MANAGER_INTERFACE,
                                 QDBusConnection::systemBus(), parent)
{
    qDBusRegisterMetaType<ConnmanMap>();
    qDBusRegisterMetaType<ConnmanMapList>();
    qRegisterMetaType<ConnmanMapList>("ConnmanMapList");
}

QConnmanManagerInterface::~QConnmanManagerInterface()
{
}

void QConnmanManagerInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        if(!connection().connect(QLatin1String(CONNMAN_SERVICE),
                               QLatin1String(CONNMAN_MANAGER_PATH),
                               QLatin1String(CONNMAN_MANAGER_INTERFACE),
                               QLatin1String("PropertyChanged"),
                               this,SIGNAL(propertyChanged(QString,QDBusVariant)))) {
            qWarning() << "PropertyCHanged not connected";
        }
    }

    if (strcmp(signal + 1, "stateChanged(QString)") == 0) {
        if (!connection().connect(QLatin1String(CONNMAN_SERVICE),
                                    QLatin1String(CONNMAN_MANAGER_PATH),
                                    QLatin1String(CONNMAN_MANAGER_INTERFACE),
                                    QLatin1String("StateChanged"),
                                    this,SIGNAL(stateChanged(QString)))) {
            qWarning() << "StateChanged not connected";

        }
    }
    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QConnmanDBusHelper *helper = new QConnmanDBusHelper(this);

        connmanConnection.connect(QLatin1String(CONNMAN_SERVICE),
                                 QLatin1String(CONNMAN_MANAGER_PATH),
                                 QLatin1String(CONNMAN_MANAGER_INTERFACE),
                                 QLatin1String("PropertyChanged"),
                                 helper,SLOT(propertyChanged(QString,QDBusVariant)));


        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariant QConnmanManagerInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    } else {
        qDebug() << "does not contain" << property;
    }
    return var;
}

QVariantMap QConnmanManagerInterface::getProperties()
{
    if(this->isValid()) {
        QDBusReply<QVariantMap > reply =  this->call(QLatin1String("GetProperties"));
        return reply.value();
    } else return QVariantMap();
}

QString QConnmanManagerInterface::getState()
{
    QDBusReply<QString > reply =  this->call(QLatin1String("GetState"));
    return reply.value();
}

bool QConnmanManagerInterface::setProperty(const QString &name, const QDBusVariant &value)
{
    Q_UNUSED(name);
    Q_UNUSED(value);
    return false;
}

QDBusObjectPath QConnmanManagerInterface::createProfile(const QString &/*name*/)
{
    return QDBusObjectPath();
}

bool QConnmanManagerInterface::removeProfile(QDBusObjectPath /*path*/)
{
    return false;
}

bool QConnmanManagerInterface::requestScan(const QString &type)
{
    QDBusReply<QString> reply =  this->call(QLatin1String("RequestScan"), QVariant::fromValue(type));

    bool ok = true;
    if(reply.error().type() == QDBusError::InvalidArgs) {
        qWarning() << reply.error().message();
        ok = false;
    }
    return ok;
}

bool QConnmanManagerInterface::enableTechnology(const QString &type)
{
    QDBusReply<QList<QDBusObjectPath> > reply =  this->call(QLatin1String("EnableTechnology"), QVariant::fromValue(type));
    bool ok = true;
    if(reply.error().type() == QDBusError::InvalidArgs) {
        qWarning() << reply.error().message();
        ok = false;
    }
    return ok;
}

bool QConnmanManagerInterface::disableTechnology(const QString &type)
{
    QDBusReply<QList<QDBusObjectPath> > reply =  this->call(QLatin1String("DisableTechnology"), QVariant::fromValue(type));
    bool ok = true;
    if(reply.error().type() == QDBusError::InvalidArgs) {
        qWarning() << reply.error().message();
        ok = false;
    }
    return ok;
}

QDBusObjectPath QConnmanManagerInterface::connectService(QVariantMap &map)
{
    QDBusReply<QDBusObjectPath > reply =  this->call(QLatin1String("ConnectService"), QVariant::fromValue(map));
    if(!reply.isValid()) {
        qDebug() << reply.error().message();

    }
    return reply;
}

void QConnmanManagerInterface::registerAgent(QDBusObjectPath &/*path*/)
{
}

void QConnmanManagerInterface::unregisterAgent(QDBusObjectPath /*path*/)
{
}

void QConnmanManagerInterface::registerCounter(const QString &path, quint32 interval)
{   QDBusReply<QList<QDBusObjectPath> > reply =  this->call(QLatin1String("RegisterCounter"),
                                                            QVariant::fromValue(path),
                                                            QVariant::fromValue(interval));
    if(reply.error().type() == QDBusError::InvalidArgs) {
        qWarning() << reply.error().message();
    }
}

void QConnmanManagerInterface::unregisterCounter(const QString &path)
{   QDBusReply<QList<QDBusObjectPath> > reply =  this->call(QLatin1String("UnregisterCounter"),
                                                            QVariant::fromValue(path));
    if(reply.error().type() == QDBusError::InvalidArgs) {
        qWarning() << reply.error().message();
    }
}

QString QConnmanManagerInterface::requestSession(const QString &bearerName)
{
    QDBusReply<QList<QDBusObjectPath> > reply =  this->call(QLatin1String("RequestSession"),
                                                            QVariant::fromValue(bearerName));
    return QString();
}

void QConnmanManagerInterface::releaseSession()
{
    QDBusReply<QList<QDBusObjectPath> > reply =  this->call(QLatin1String("ReleaseSession"));
}


QDBusObjectPath QConnmanManagerInterface::lookupService(const QString &service)
{
    QDBusReply<QDBusObjectPath > reply =  this->call(QLatin1String("LookupService"), QVariant::fromValue(service));
    if(!reply.isValid()) {
        qDebug() << reply.error().message();
    }
    return reply;
}

// properties

QStringList QConnmanManagerInterface::getAvailableTechnologies()
{
    QVariant var = getProperty(QLatin1String("AvailableTechnologies"));
    return qdbus_cast<QStringList>(var);
}

QStringList QConnmanManagerInterface::getEnabledTechnologies()
{
    QVariant var = getProperty(QLatin1String("EnabledTechnologies"));
    return qdbus_cast<QStringList>(var);
}

QStringList QConnmanManagerInterface::getConnectedTechnologies()
{
    QVariant var = getProperty(QLatin1String("ConnectedTechnologies"));
    return qdbus_cast<QStringList>(var);
}

QString QConnmanManagerInterface::getDefaultTechnology()
{
    QVariant var = getProperty(QLatin1String("DefaultTechnology"));
    return qdbus_cast<QString>(var);
}

bool QConnmanManagerInterface::getOfflineMode()
{
    QVariant var = getProperty(QLatin1String("OfflineMode"));
    return qdbus_cast<bool>(var);
}

QString QConnmanManagerInterface::getActiveProfile()
{
    QVariant var = getProperty(QLatin1String("ActiveProfile"));
    return qdbus_cast<QString>(var);
}

QStringList QConnmanManagerInterface::getProfiles()
{
    QVariant var = getProperty(QLatin1String("Profiles"));
    return qdbus_cast<QStringList>(var);
}

QStringList QConnmanManagerInterface::getTechnologies()
{
    QStringList list;
    QDBusReply<ConnmanMapList> replyList = this->call(QLatin1String("GetTechnologies"));
    if (replyList.isValid()) {
        Q_FOREACH (ConnmanMap map, replyList.value()) {
            list << map.objectPath.path();
        }
    } else {
        // try for older version
        QVariant var = getProperty(QLatin1String("Technologies"));
        if (!var.isNull()) {
            list = qdbus_cast<QStringList>(var);
        }
    }
    return list;
}

QStringList QConnmanManagerInterface::getServices()
{
    QStringList list;
    QDBusReply<ConnmanMapList> replyList = this->call(QLatin1String("GetServices"));
    if (replyList.isValid()) {
        Q_FOREACH (ConnmanMap map, replyList.value()) {
            list << map.objectPath.path();
        }
    } else {
        // try for older version
        QVariant var = getProperty(QLatin1String("Services"));
        if (!var.isNull()) {
            list = qdbus_cast<QStringList>(var);
        }
    }
    return list;
}

QString QConnmanManagerInterface::getPathForTechnology(const QString &name)
{
    foreach(const QString path, getTechnologies()) {
        if(path.contains(name)) {
            return path;
        }
    }
    return QLatin1String("");
}


//////////////////////////
QConnmanProfileInterface::QConnmanProfileInterface(const QString &dbusPathName,QObject *parent)
    : QDBusAbstractInterface(QLatin1String(CONNMAN_SERVICE),
                             dbusPathName,
                             CONNMAN_PROFILE_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QConnmanProfileInterface::~QConnmanProfileInterface()
{
}

void QConnmanProfileInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        connmanConnection.connect(QLatin1String(CONNMAN_SERVICE),
                                  this->path(),
                                  QLatin1String(CONNMAN_PROFILE_INTERFACE),
                                  QLatin1String("PropertyChanged"),
                                  this,SIGNAL(propertyChanged(QString,QDBusVariant)));
    }
}

QVariantMap QConnmanProfileInterface::getProperties()
{
    QDBusReply<QVariantMap > reply =  this->call(QLatin1String("GetProperties"));
    return reply.value();
}

QVariant QConnmanProfileInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    }
    return var;
}

// properties
QString QConnmanProfileInterface::getName()
{

    QVariant var = getProperty(QLatin1String("Name"));
    return qdbus_cast<QString>(var);
}

bool QConnmanProfileInterface::isOfflineMode()
{
    QVariant var = getProperty(QLatin1String("OfflineMode"));
    return qdbus_cast<bool>(var);
}

QStringList QConnmanProfileInterface::getServices()
{
    QVariant var = getProperty(QLatin1String("Services"));
    return qdbus_cast<QStringList>(var);
}


///////////////////////////
QConnmanServiceInterface::QConnmanServiceInterface(const QString &dbusPathName,QObject *parent)
    : QDBusAbstractInterface(QLatin1String(CONNMAN_SERVICE),
                             dbusPathName,
                             CONNMAN_SERVICE_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QConnmanServiceInterface::~QConnmanServiceInterface()
{
}

void QConnmanServiceInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        connmanConnection.connect(QLatin1String(CONNMAN_SERVICE),
                                  this->path(),
                                  QLatin1String(CONNMAN_SERVICE_INTERFACE),
                                  QLatin1String("PropertyChanged"),
                                  this,SIGNAL(propertyChanged(QString,QDBusVariant)));
    }
    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QConnmanDBusHelper *helper;
        helper = new QConnmanDBusHelper(this);

        connmanConnection.connect(QLatin1String(CONNMAN_SERVICE),
                                  this->path(),
                                  QLatin1String(CONNMAN_SERVICE_INTERFACE),
                                  QLatin1String("PropertyChanged"),
                                  helper,SLOT(propertyChanged(QString,QDBusVariant)));

        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariantMap QConnmanServiceInterface::getProperties()
{
    if(this->isValid()) {
        QDBusReply<QVariantMap> reply =  this->call(QLatin1String("GetProperties"));
        return reply.value();
    }
    else
        return QVariantMap();
}

QVariant QConnmanServiceInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    }
    return var;
}

void QConnmanServiceInterface::connect()
{
    this->asyncCall(QLatin1String("Connect"));
}

void QConnmanServiceInterface::disconnect()
{
    QDBusReply<QVariantMap> reply = this->call(QLatin1String("Disconnect"));
}

void QConnmanServiceInterface::remove()
{
    QDBusReply<QVariantMap> reply = this->call(QLatin1String("Remove"));
}

// void moveBefore(QDBusObjectPath &service);
// void moveAfter(QDBusObjectPath &service);

// properties
QString QConnmanServiceInterface::getState()
{
    QVariant var = getProperty(QLatin1String("State"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getError()
{
    QVariant var = getProperty(QLatin1String("Error"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getName()
{
    QVariant var = getProperty(QLatin1String("Name"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getType()
{
    QVariant var = getProperty(QLatin1String("Type"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getMode()
{
    QVariant var = getProperty(QLatin1String("Mode"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getSecurity()
{
    QVariant var = getProperty(QLatin1String("Security"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getPassphrase()
{
    QVariant var = getProperty(QLatin1String("Passphrase"));
    return qdbus_cast<QString>(var);
}

bool QConnmanServiceInterface::isPassphraseRequired()
{
    QVariant var = getProperty(QLatin1String("PassphraseRequired"));
    return qdbus_cast<bool>(var);
}

quint8 QConnmanServiceInterface::getSignalStrength()
{
    QVariant var = getProperty(QLatin1String("Strength"));
    return qdbus_cast<quint8>(var);
}

bool QConnmanServiceInterface::isFavorite()
{
    QVariant var = getProperty(QLatin1String("Favorite"));
    return qdbus_cast<bool>(var);
}

bool QConnmanServiceInterface::isImmutable()
{
    QVariant var = getProperty(QLatin1String("Immutable"));
    return qdbus_cast<bool>(var);
}

bool QConnmanServiceInterface::isAutoConnect()
{
    QVariant var = getProperty(QLatin1String("AutoConnect"));
    return qdbus_cast<bool>(var);
}

bool QConnmanServiceInterface::isSetupRequired()
{
    QVariant var = getProperty(QLatin1String("SetupRequired"));
    return qdbus_cast<bool>(var);
}

QString QConnmanServiceInterface::getAPN()
{
    QVariant var = getProperty(QLatin1String("APN"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getMCC()
{
    QVariant var = getProperty(QLatin1String("MCC"));
    return qdbus_cast<QString>(var);
}

QString QConnmanServiceInterface::getMNC()
{
    QVariant var = getProperty(QLatin1String("MNC"));
    return qdbus_cast<QString>(var);
}

bool QConnmanServiceInterface::isRoaming()
{
    QVariant var = getProperty(QLatin1String("Roaming"));
    return qdbus_cast<bool>(var);
}

QStringList QConnmanServiceInterface::getNameservers()
{
    QVariant var = getProperty(QLatin1String("NameServers"));
    return qdbus_cast<QStringList>(var);
}

QStringList QConnmanServiceInterface::getDomains()
{
    QVariant var = getProperty(QLatin1String("Domains"));
    return qdbus_cast<QStringList>(var);
}

QVariantMap QConnmanServiceInterface::getIPv4()
{
    QVariant var = getProperty(QLatin1String("IPv4"));
    return qdbus_cast<QVariantMap >(var);
}

QVariantMap QConnmanServiceInterface::getIPv4Configuration()
{
    QVariant var = getProperty(QLatin1String("IPv4.Configuration"));
    return qdbus_cast<QVariantMap >(var);
}

QVariantMap QConnmanServiceInterface::getProxy()
{
    QVariant var = getProperty(QLatin1String("Proxy"));
    return qdbus_cast<QVariantMap >(var);
}

QVariantMap QConnmanServiceInterface::getEthernet()
{
    QVariant var = getProperty(QLatin1String("Ethernet"));
    return qdbus_cast<QVariantMap >(var);
}

QString QConnmanServiceInterface::getMethod()
{
    QVariant var;
    QVariantMap map = getEthernet();
    QMapIterator<QString,QVariant> it(map);
    while(it.hasNext()) {
        it.next();
        if(it.key() == QLatin1String("Method")) {
            return it.value().toString();
        }
    }
 return QString();
}

QString QConnmanServiceInterface::getInterface()
{
    QVariant var;
    QVariantMap map = getEthernet();

    QMapIterator<QString,QVariant> it(map);
    while(it.hasNext()) {
        it.next();
        if(it.key() == QLatin1String("Interface")) {
            return it.value().toString();
        }
    }

    return QString();
}

QString QConnmanServiceInterface::getMacAddress()
{
    QVariant var;
    QVariantMap map = getEthernet();

    QMapIterator<QString,QVariant> it(map);
    while(it.hasNext()) {
        it.next();
        if(it.key() == QLatin1String("Address")) {
            return it.value().toString();
        }
    }
    return QString();
}

quint16 QConnmanServiceInterface::getMtu()
{
    quint16 mtu=0;
    QVariant var;
    QVariantMap map = getEthernet();

    QMapIterator<QString,QVariant> it(map);
    while(it.hasNext()) {
        it.next();
        if(it.key() == QLatin1String("MTU")) {
            return it.value().toUInt();
        }
    }
    return mtu;
}

quint16 QConnmanServiceInterface::getSpeed()
{
    quint16 speed=0;
    QVariant var;
    QVariantMap map = getEthernet();

    QMapIterator<QString,QVariant> it(map);
    while(it.hasNext()) {
        it.next();
        if(it.key() == QLatin1String("Speed")) {
            return it.value().toUInt();
        }
    }
    return speed;
}

QString QConnmanServiceInterface::getDuplex()
{
    QVariant var;
    QVariantMap map = getEthernet();

    QMapIterator<QString,QVariant> it(map);
    while(it.hasNext()) {
        it.next();
        if(it.key() == QLatin1String("Duplex")) {
            return it.value().toString();
        }
    }
    return QString();
}


bool QConnmanServiceInterface::isOfflineMode()
{
    QVariant var = getProperty(QLatin1String("OfflineMode"));
    return qdbus_cast<bool>(var);
}

QStringList QConnmanServiceInterface::getServices()
{
    QVariant var = getProperty(QLatin1String("Services"));
    return qdbus_cast<QStringList>(var);
}


//////////////////////////
QConnmanTechnologyInterface::QConnmanTechnologyInterface(const QString &dbusPathName,QObject *parent)
    : QDBusAbstractInterface(QLatin1String(CONNMAN_SERVICE),
                             dbusPathName,
                             CONNMAN_TECHNOLOGY_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QConnmanTechnologyInterface::~QConnmanTechnologyInterface()
{
}

void QConnmanTechnologyInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
        connmanConnection.connect(QLatin1String(CONNMAN_SERVICE),
                                  this->path(),
                                  QLatin1String(CONNMAN_TECHNOLOGY_INTERFACE),
                                  QLatin1String("PropertyChanged"),
                                  this,SIGNAL(propertyChanged(QString,QDBusVariant)));
    }
    if (strcmp(signal + 1, "propertyChangedContext(QString,QString,QDBusVariant)") == 0) {
        QConnmanDBusHelper *helper;
        helper = new QConnmanDBusHelper(this);

        connmanConnection.connect(QLatin1String(CONNMAN_SERVICE),
                                  this->path(),
                                  QLatin1String(CONNMAN_TECHNOLOGY_INTERFACE),
                                  QLatin1String("PropertyChanged"),
                                  helper,SLOT(propertyChanged(QString,QDBusVariant)));

        QObject::connect(helper,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)),
                this,SIGNAL(propertyChangedContext(QString,QString,QDBusVariant)), Qt::UniqueConnection);
    }
}

QVariantMap QConnmanTechnologyInterface::getProperties()
{
    QDBusReply<QVariantMap> reply =  this->call(QLatin1String("GetProperties"));
    return reply.value();
}

QVariant QConnmanTechnologyInterface::getProperty(const QString &property)
{
    QVariant var;
    QVariantMap map = getProperties();
    if (map.contains(property)) {
        var = map.value(property);
    }
    return var;
}

// properties
QString QConnmanTechnologyInterface::getState()
{
    QVariant var = getProperty(QLatin1String("State"));
    return qdbus_cast<QString>(var);
}

QString QConnmanTechnologyInterface::getName()
{
    QVariant var = getProperty(QLatin1String("Name"));
    return qdbus_cast<QString>(var);
}

QString QConnmanTechnologyInterface::getType()
{
    QVariant var = getProperty(QLatin1String("Type"));
    return qdbus_cast<QString>(var);
}


//////////////////////////////////
QConnmanAgentInterface::QConnmanAgentInterface(const QString &dbusPathName, QObject *parent)
    : QDBusAbstractInterface(QLatin1String(CONNMAN_SERVICE),
                             dbusPathName,
                             CONNMAN_AGENT_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QConnmanAgentInterface::~QConnmanAgentInterface()
{
}

void QConnmanAgentInterface::connectNotify(const char *signal)
{
    if (strcmp(signal + 1, "propertyChanged(QString,QDBusVariant)") == 0) {
//        connmanConnection.connect(QLatin1String(CONNMAN_SERVICE),
//                                  this->path(),
//                                  QLatin1String(CONNMAN_NETWORK_INTERFACE),
//                                  QLatin1String("PropertyChanged"),
//                                  this,SIGNAL(propertyChanged(QString,QVariant&)));
    }
}


void QConnmanAgentInterface::release()
{
}

void QConnmanAgentInterface::reportError(QDBusObjectPath &/*path*/, const QString &/*error*/)
{
}

//dict QConnmanAgentInterface::requestInput(QDBusObjectPath &path, dict fields)
//{
//}

void QConnmanAgentInterface::cancel()
{
}


/////////////////////////////////////////
QConnmanCounterInterface::QConnmanCounterInterface(const QString &dbusPathName,QObject *parent)
    : QDBusAbstractInterface(QLatin1String(CONNMAN_SERVICE),
                             dbusPathName,
                             CONNMAN_COUNTER_INTERFACE,
                             QDBusConnection::systemBus(), parent)
{
}

QConnmanCounterInterface::~QConnmanCounterInterface()
{
}

quint32 QConnmanCounterInterface::getReceivedByteCount()
{
    return 0;
}

quint32 QConnmanCounterInterface::getTransmittedByteCount()
{
    return 0;
}

quint64 QConnmanCounterInterface::getTimeOnline()
{
    return 0;
}

/////////////////////////////////////////
QConnmanDBusHelper::QConnmanDBusHelper(QObject * parent)
        : QObject(parent)
{
}

QConnmanDBusHelper::~QConnmanDBusHelper()
{
}

void QConnmanDBusHelper::propertyChanged(const QString &item, const QDBusVariant &var)
{
    QDBusMessage msg = this->message();
    Q_EMIT propertyChangedContext(msg.path() ,item, var);
}

/////////////////
QT_END_NAMESPACE

#endif // QT_NO_DBUS
#endif // QT_NO_BEARERMANAGEMENT

#include "moc_qconnmanservice_linux_p.h"
