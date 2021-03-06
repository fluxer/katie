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

#ifndef QDBUSABSTRACTINTERFACEPRIVATE_H
#define QDBUSABSTRACTINTERFACEPRIVATE_H

#include "qdbusabstractinterface.h"
#include "qdbusconnection.h"
#include "qdbuserror.h"
#include "qdbusconnection_p.h"
#include "qobject_p.h"

#define ANNOTATION_NO_WAIT      "org.freedesktop.DBus.Method.NoReply"


QT_BEGIN_NAMESPACE

class QDBusAbstractInterfacePrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QDBusAbstractInterface)

    QDBusConnection connection;
    QString service;
    QString currentOwner;
    QString path;
    QString interface;
    mutable QDBusError lastError;
    int timeout;

    // this is set during creation and never changed
    // it can't be const because QDBusInterfacePrivate has one more check
    bool isValid;

    QDBusAbstractInterfacePrivate(const QString &serv, const QString &p,
                                  const QString &iface, const QDBusConnection& con, bool dynamic);
    virtual ~QDBusAbstractInterfacePrivate() { }
    bool canMakeCalls() const;

    // these functions do not check if the property is valid
    void property(const QMetaProperty &mp, QVariant &where) const;
    bool setProperty(const QMetaProperty &mp, const QVariant &value);

    // return conn's d pointer
    inline QDBusConnectionPrivate *connectionPrivate() const
    { return QDBusConnectionPrivate::d(connection); }

    void _q_serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);
};

QT_END_NAMESPACE

#endif
