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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the public API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QDBUSCONNECTION_P_H
#define QDBUSCONNECTION_P_H

#include "qdbuserror.h"
#include "qdbusconnection.h"
#include "qdbusmessage.h"

#include <QtCore/qatomic.h>
#include <QtCore/qhash.h>
#include <QtCore/qmutex.h>
#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qvector.h>

#include <dbus/dbus.h>

QT_BEGIN_NAMESPACE

class QDBusMessage;
class QSocketNotifier;
class QTimerEvent;
class QDBusObjectPrivate;
class QDBusCallDeliveryEvent;
class QDBusActivateObjectEvent;
class QMetaMethod;
class QDBusInterfacePrivate;
struct QDBusMetaObject;
class QDBusAbstractInterface;
class QDBusConnectionInterface;
class QDBusPendingCallPrivate;

class QDBusErrorInternal
{
    DBusError error;
    Q_DISABLE_COPY(QDBusErrorInternal)
public:
    inline QDBusErrorInternal() { dbus_error_init(&error); }
    inline ~QDBusErrorInternal() { dbus_error_free(&error); }
    inline bool operator !() const { return !dbus_error_is_set(&error); }
    inline operator DBusError *() { dbus_error_free(&error); return &error; }
    inline operator QDBusError() const { QDBusError err(&error); return err; }
};

// QDBusConnectionPrivate holds the DBusConnection and
// can have many QDBusConnection objects referring to it

class QDBusConnectionPrivate: public QObject
{
    Q_OBJECT
public:
    // structs and enums
    enum ConnectionMode { InvalidMode, ServerMode, ClientMode, PeerMode }; // LocalMode

    struct Watcher
    {
        Watcher(): watch(0), read(0), write(0) {}
        DBusWatch *watch;
        QSocketNotifier *read;
        QSocketNotifier *write;
    };

    struct SignalHook
    {
        inline SignalHook() : obj(0), midx(-1) { }
        QString service, path, signature;
        QObject* obj;
        int midx;
        QList<int> params;
        QStringList argumentMatch;
        QByteArray matchRule;
    };

    struct ObjectTreeNode
    {
        typedef QVector<ObjectTreeNode> DataList;

        inline ObjectTreeNode() : obj(0), flags(0) { }
        inline ObjectTreeNode(const QString &n) // intentionally implicit
            : name(n), obj(0), flags(0) { }
        inline ~ObjectTreeNode() { }
        inline bool operator<(const QString &other) const
            { return name < other; }
        inline bool operator<(const QStringRef &other) const
            { return QStringRef(&name) < other; }

        QString name;
        QObject *obj;
        int flags;

        DataList children;
    };

    // typedefs
    typedef QMultiHash<int, Watcher> WatcherHash;
    typedef QHash<int, DBusTimeout *> TimeoutHash;
    typedef QList<QPair<DBusTimeout *, int> > PendingTimeoutList;

    typedef QMultiHash<QString, SignalHook> SignalHookHash;
    typedef QHash<QString, QDBusMetaObject* > MetaObjectHash;
    typedef QHash<QByteArray, int> MatchRefCountHash;

    struct WatchedServiceData {
        WatchedServiceData() : refcount(0) {}
        WatchedServiceData(const QString &owner, int refcount = 0)
            : owner(owner), refcount(refcount)
        {}
        QString owner;
        int refcount;
    };
    typedef QHash<QString, WatchedServiceData> WatchedServicesHash;

    // public methods are entry points from other objects
    explicit QDBusConnectionPrivate(QObject *parent = nullptr);
    ~QDBusConnectionPrivate();

    void setBusService(const QDBusConnection &connection);
    void setPeer(DBusConnection *connection, const QDBusErrorInternal &error);
    void setConnection(DBusConnection *connection, const QDBusErrorInternal &error);
    void setServer(DBusServer *server, const QDBusErrorInternal &error);
    void closeConnection();

    QString getNameOwner(const QString &service);

    bool send(const QDBusMessage &message);
    QDBusMessage sendWithReply(const QDBusMessage &message, int mode, int timeout = -1);
    QDBusMessage sendWithReplyLocal(const QDBusMessage &message);
    QDBusPendingCallPrivate *sendWithReplyAsync(const QDBusMessage &message, QObject *receiver,
                                                const char *returnMethod, const char *errorMethod,int timeout = -1);
    bool connectSignal(const QString &service, const QString &path, const QString& interface,
                       const QString &name, const QStringList &argumentMatch, const QString &signature,
                       QObject *receiver, const char *slot);
    void connectSignal(const QString &key, const SignalHook &hook);
    SignalHookHash::Iterator disconnectSignal(SignalHookHash::Iterator &it);
    bool disconnectSignal(const QString &service, const QString &path, const QString& interface,
                          const QString &name, const QStringList &argumentMatch, const QString &signature,
                          QObject *receiver, const char *slot);
    void registerObject(const ObjectTreeNode *node);
    void connectRelay(const QString &service,
                      const QString &path, const QString &interface,
                      QDBusAbstractInterface *receiver, const char *signal);
    void disconnectRelay(const QString &service,
                         const QString &path, const QString &interface,
                         QDBusAbstractInterface *receiver, const char *signal);
    void registerService(const QString &serviceName);
    void unregisterService(const QString &serviceName);

    bool handleMessage(const QDBusMessage &msg);
    void waitForFinished(QDBusPendingCallPrivate *pcall);

    QDBusMetaObject *findMetaObject(const QString &service, const QString &path,
                                    const QString &interface, QDBusError &error);

    void postEventToThread(int action, QObject *target, QEvent *event);

    inline void serverConnection(const QDBusConnection &connection)
        { emit newServerConnection(connection); }

private:
    void checkThread();
    bool handleError(const QDBusErrorInternal &error);

    void handleSignal(const QString &key, const QDBusMessage &msg);
    void handleSignal(const QDBusMessage &msg);
    void handleObjectCall(const QDBusMessage &message);

    void activateSignal(const SignalHook& hook, const QDBusMessage &msg);
    void activateObject(ObjectTreeNode &node, const QDBusMessage &msg, int pathStartPos);
    bool activateInternalFilters(const ObjectTreeNode &node, const QDBusMessage &msg);
    bool activateCall(QObject *object, int flags, const QDBusMessage &msg);

    void sendError(const QDBusMessage &msg, QDBusError::ErrorType code);
    void deliverCall(QObject *object, const QDBusMessage &msg,
                     const QList<int> &metaTypes, int slotIdx);

    bool isServiceRegisteredByThread(const QString &serviceName);

    QString getNameOwnerNoCache(const QString &service);

protected:
    void customEvent(QEvent *e);
    void timerEvent(QTimerEvent *e);

public slots:
    // public slots
    void doDispatch();
    void socketRead(int);
    void socketWrite(int);
    void objectDestroyed(QObject *o);
    void relaySignal(QObject *obj, const QMetaObject *, int signalId, const QVariantList &args);

private slots:
    void serviceOwnerChangedNoLock(const QString &name, const QString &oldOwner, const QString &newOwner);
    void registerServiceNoLock(const QString &serviceName);
    void unregisterServiceNoLock(const QString &serviceName);

signals:
    void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);
    void callWithCallbackFailed(const QDBusError &error, const QDBusMessage &message);
    void newServerConnection(const QDBusConnection &connection);

public:
    QAtomicInt ref;
    QDBusConnection::ConnectionCapabilities capabilities;
    QString name;               // this connection's name
    QString baseService;        // this connection's base service
    QStringList serverConnectionNames;

    ConnectionMode mode;
    QDBusConnectionInterface *busService;

    // the dispatch lock protects everything related to the DBusConnection or DBusServer
    // including the timeouts and watches
    std::recursive_mutex dispatchLock;
    DBusConnection *connection;
    DBusServer *server;

    WatcherHash watchers;
    TimeoutHash timeouts;
    PendingTimeoutList timeoutsPendingAdd;

    // the master lock protects our own internal state
    QMutex lock;
    QDBusError lastError;

    QStringList serviceNames;
    WatchedServicesHash watchedServices;
    SignalHookHash signalHooks;
    MatchRefCountHash matchRefCounts;
    ObjectTreeNode rootNode;
    MetaObjectHash cachedMetaObjects;

    QMutex callDeliveryMutex;
    QDBusCallDeliveryEvent *callDeliveryState; // protected by the callDeliveryMutex mutex

    // static methods
    static int findSlot(QObject *obj, const QByteArray &normalizedName, QList<int>& params);
    static bool prepareHook(QDBusConnectionPrivate::SignalHook &hook, QString &key,
                            const QString &service,
                            const QString &path, const QString &interface, const QString &name,
                            const QStringList &argMatch,
                            QObject *receiver, const char *signal, int minMIdx,
                            bool buildSignature);
    static DBusHandlerResult messageFilter(DBusConnection *, DBusMessage *, void *);
    static bool checkReplyForDelivery(QDBusConnectionPrivate *target, QObject *object,
                                      int idx, const QList<int> &metaTypes,
                                      const QDBusMessage &msg);
    static QDBusCallDeliveryEvent *prepareReply(QDBusConnectionPrivate *target, QObject *object,
                                                int idx, const QList<int> &metaTypes,
                                                const QDBusMessage &msg);
    static void processFinishedCall(QDBusPendingCallPrivate *call);

    static QDBusConnectionPrivate *d(const QDBusConnection& q) { return q.d; }
    static QDBusConnection q(QDBusConnectionPrivate *connection) { return QDBusConnection(connection); }

    static void setSender(const QDBusConnectionPrivate *s);

    friend class QDBusActivateObjectEvent;
    friend class QDBusCallDeliveryEvent;
};

// in qdbusmisc.cpp
extern int qDBusParametersForMethod(const QMetaMethod &mm, QList<int>& metaTypes);
extern bool qDBusInterfaceInObject(QObject *obj, const QString &interface_name);
extern QString qDBusInterfaceFromMetaObject(const QMetaObject *mo);

// in qdbusinternalfilters.cpp
extern QString qDBusIntrospectObject(const QDBusConnectionPrivate::ObjectTreeNode &node, const QString &path);
extern QDBusMessage qDBusPropertyGet(const QDBusConnectionPrivate::ObjectTreeNode &node,
                                     const QDBusMessage &msg);
extern QDBusMessage qDBusPropertySet(const QDBusConnectionPrivate::ObjectTreeNode &node,
                                     const QDBusMessage &msg);
extern QDBusMessage qDBusPropertyGetAll(const QDBusConnectionPrivate::ObjectTreeNode &node,
                                        const QDBusMessage &msg);

QT_END_NAMESPACE

#endif
