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

// this class is for helping qdbus get stuff

#include "qnmdbushelper.h"

#include "qnetworkmanagerservice.h"

#include <QDBusError>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusReply>

#include <QDebug>

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

QNmDBusHelper::QNmDBusHelper(QObject * parent)
        : QObject(parent)
{
}

QNmDBusHelper::~QNmDBusHelper()
{
}

void QNmDBusHelper::deviceStateChanged(quint32 state)
 {
    QDBusMessage msg = this->message();
    if(state == NM_DEVICE_STATE_ACTIVATED
       || state == NM_DEVICE_STATE_DISCONNECTED
       || state == NM_DEVICE_STATE_UNAVAILABLE
       || state == NM_DEVICE_STATE_FAILED) {
        emit pathForStateChanged(msg.path(), state);
    }
 }

void QNmDBusHelper::slotAccessPointAdded(QDBusObjectPath path)
{
    if(path.path().length() > 2) {
        QDBusMessage msg = this->message();
        emit pathForAccessPointAdded(msg.path(), path);
    }
}

void QNmDBusHelper::slotAccessPointRemoved(QDBusObjectPath path)
{
    if(path.path().length() > 2) {
        QDBusMessage msg = this->message();
        emit pathForAccessPointRemoved(msg.path(), path);
    }
}

void QNmDBusHelper::slotPropertiesChanged(QMap<QString,QVariant> map)
{
    QDBusMessage msg = this->message();
    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();
        if( i.key() == QLatin1String("State")) { //state only applies to device interfaces
            quint32 state = i.value().toUInt();
            if( state == NM_DEVICE_STATE_ACTIVATED
                || state == NM_DEVICE_STATE_DISCONNECTED
                || state == NM_DEVICE_STATE_UNAVAILABLE
                || state == NM_DEVICE_STATE_FAILED) {
                emit  pathForPropertiesChanged( msg.path(), map);
            }
        } else if( i.key() == QLatin1String("ActiveAccessPoint")) {
            emit pathForPropertiesChanged(msg.path(), map);
            //            qWarning()  << __PRETTY_FUNCTION__ << i.key() << ": " << i.value().value<QDBusObjectPath>().path();
            //      } else if( i.key() == "Strength")
            //            qWarning()  << __PRETTY_FUNCTION__ << i.key() << ": " << i.value().toUInt();
            //   else
            //            qWarning()  << __PRETTY_FUNCTION__ << i.key() << ": " << i.value();
        } else if (i.key() == QLatin1String("ActiveConnections")) {
            emit pathForPropertiesChanged(msg.path(), map);
        }
    }
}

void QNmDBusHelper::slotSettingsRemoved()
{
    QDBusMessage msg = this->message();
    emit pathForSettingsRemoved(msg.path());
}

QT_END_NAMESPACE

#endif // QT_NO_DBUS
#include "moc_qnmdbushelper.h"
