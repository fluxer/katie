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

#ifndef QDBUSPENDINGCALL_H
#define QDBUSPENDINGCALL_H

#include <QtCore/qobject.h>
#include <QtCore/qshareddata.h>
#include <QtDBus/qdbusmessage.h>


QT_BEGIN_NAMESPACE

class QDBusConnection;
class QDBusError;
class QDBusPendingCallWatcher;

class QDBusPendingCallPrivate;
class Q_DBUS_EXPORT QDBusPendingCall
{
public:
    QDBusPendingCall(const QDBusPendingCall &other);
    ~QDBusPendingCall();
    QDBusPendingCall &operator=(const QDBusPendingCall &other);

    // pretend that they aren't here
    bool isFinished() const;
    void waitForFinished();

    bool isError() const;
    bool isValid() const;
    QDBusError error() const;
    QDBusMessage reply() const;

    static QDBusPendingCall fromError(const QDBusError &error);
    static QDBusPendingCall fromCompletedCall(const QDBusMessage &message);

protected:
    QExplicitlySharedDataPointer<QDBusPendingCallPrivate> d;
    friend class QDBusPendingCallPrivate;
    friend class QDBusPendingCallWatcher;
    friend class QDBusConnection;

    QDBusPendingCall(QDBusPendingCallPrivate *dd);

private:
    QDBusPendingCall();         // not defined
};

class QDBusPendingCallWatcherPrivate;
class Q_DBUS_EXPORT QDBusPendingCallWatcher: public QObject, public QDBusPendingCall
{
    Q_OBJECT
public:
    QDBusPendingCallWatcher(const QDBusPendingCall &call, QObject *parent = Q_NULLPTR);
    ~QDBusPendingCallWatcher();

    void waitForFinished();     // non-virtual override

Q_SIGNALS:
    void finished(QDBusPendingCallWatcher *self);

private:
    Q_DECLARE_PRIVATE(QDBusPendingCallWatcher)
    Q_PRIVATE_SLOT(d_func(), void _q_finished())
};

QT_END_NAMESPACE


#endif
