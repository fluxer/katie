/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtDBus module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
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

#ifndef QDBUSABSTRACTINTERFACE_H
#define QDBUSABSTRACTINTERFACE_H

#include <QtCore/qlist.h>
#include <QtCore/qobject.h>
#include <QtDBus/qdbusmessage.h>
#include <QtDBus/qdbusextratypes.h>
#include <QtDBus/qdbusconnection.h>

#ifndef QT_NO_DBUS

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QDBusError;
class QDBusPendingCall;

class QDBusAbstractInterfacePrivate;

class Q_DBUS_EXPORT QDBusAbstractInterfaceBase: public QObject
{
public:
    int qt_metacall(QMetaObject::Call, int, void**);
protected:
    QDBusAbstractInterfaceBase(QDBusAbstractInterfacePrivate &dd, QObject *parent);
private:
    Q_DECLARE_PRIVATE(QDBusAbstractInterface)
};

class Q_DBUS_EXPORT QDBusAbstractInterface:
        public QDBusAbstractInterfaceBase
{
    Q_OBJECT

public:
    virtual ~QDBusAbstractInterface();
    bool isValid() const;

    QDBusConnection connection() const;

    QString service() const;
    QString path() const;
    QString interface() const;

    QDBusError lastError() const;

    void setTimeout(int timeout);
    int timeout() const;

    QDBusMessage call(const QString &method,
                      const QVariant &arg1 = QVariant(),
                      const QVariant &arg2 = QVariant(),
                      const QVariant &arg3 = QVariant(),
                      const QVariant &arg4 = QVariant(),
                      const QVariant &arg5 = QVariant(),
                      const QVariant &arg6 = QVariant(),
                      const QVariant &arg7 = QVariant(),
                      const QVariant &arg8 = QVariant()) const;

    QDBusMessage call(QDBus::CallMode mode,
                      const QString &method,
                      const QVariant &arg1 = QVariant(),
                      const QVariant &arg2 = QVariant(),
                      const QVariant &arg3 = QVariant(),
                      const QVariant &arg4 = QVariant(),
                      const QVariant &arg5 = QVariant(),
                      const QVariant &arg6 = QVariant(),
                      const QVariant &arg7 = QVariant(),
                      const QVariant &arg8 = QVariant()) const;

    QDBusMessage callWithArgumentList(QDBus::CallMode mode,
                                      const QString &method,
                                      const QList<QVariant> &args) const;

    bool callWithCallback(const QString &method,
                          const QList<QVariant> &args,
                          QObject *receiver, const char *member, const char *errorSlot);
    bool callWithCallback(const QString &method,
                          const QList<QVariant> &args,
                          QObject *receiver, const char *member);

    QDBusPendingCall asyncCall(const QString &method,
                               const QVariant &arg1 = QVariant(),
                               const QVariant &arg2 = QVariant(),
                               const QVariant &arg3 = QVariant(),
                               const QVariant &arg4 = QVariant(),
                               const QVariant &arg5 = QVariant(),
                               const QVariant &arg6 = QVariant(),
                               const QVariant &arg7 = QVariant(),
                               const QVariant &arg8 = QVariant()) const;
    QDBusPendingCall asyncCallWithArgumentList(const QString &method,
                                               const QList<QVariant> &args) const;

protected:
    QDBusAbstractInterface(const QString &service, const QString &path, const char *interface,
                           const QDBusConnection &connection, QObject *parent);
    QDBusAbstractInterface(QDBusAbstractInterfacePrivate &, QObject *parent);

    void connectNotify(const char *signal);
    void disconnectNotify(const char *signal);
    QVariant internalPropGet(const char *propname) const;
    void internalPropSet(const char *propname, const QVariant &value);

private:
    Q_DECLARE_PRIVATE(QDBusAbstractInterface)
    Q_PRIVATE_SLOT(d_func(), void _q_serviceOwnerChanged(QString,QString,QString))
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QT_NO_DBUS
#endif
