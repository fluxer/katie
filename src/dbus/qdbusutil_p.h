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
// This file is not part of the Katie API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef QDBUSUTIL_H
#define QDBUSUTIL_H

#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>

#include <QtDBus/qdbuserror.h>



QT_BEGIN_NAMESPACE

namespace QDBusUtil
{
    Q_DBUS_EXPORT bool isValidInterfaceName(const QString &ifaceName);

    Q_DBUS_EXPORT bool isValidUniqueConnectionName(const QString &busName);

    Q_DBUS_EXPORT bool isValidBusName(const QString &busName);

    Q_DBUS_EXPORT bool isValidMemberName(const QString &memberName);

    Q_DBUS_EXPORT bool isValidErrorName(const QString &errorName);

    Q_DBUS_EXPORT bool isValidPartOfObjectPath(const QString &path);

    Q_DBUS_EXPORT bool isValidObjectPath(const QString &path);

    Q_DBUS_EXPORT bool isValidFixedType(int c);

    Q_DBUS_EXPORT bool isValidBasicType(int c);

    Q_DBUS_EXPORT bool isValidSignature(const QString &signature);

    Q_DBUS_EXPORT bool isValidSingleSignature(const QString &signature);

    Q_DBUS_EXPORT QString argumentToString(const QVariant &variant);

    enum AllowEmptyFlag {
        EmptyAllowed,
        EmptyNotAllowed
    };

    inline bool checkInterfaceName(const QString &name, QDBusError *error)
    {
        if (name.isEmpty()) {
            return true;
        }
        if (isValidInterfaceName(name)) return true;
        *error = QDBusError(QDBusError::InvalidInterface, QString::fromLatin1("Invalid interface class: %1").arg(name));
        return false;
    }

    inline bool checkBusName(const QString &name, AllowEmptyFlag empty, QDBusError *error)
    {
        if (name.isEmpty()) {
            if (empty == EmptyAllowed) return true;
            *error = QDBusError(QDBusError::InvalidService, QLatin1String("Service name cannot be empty"));
            return false;
        }
        if (isValidBusName(name)) return true;
        *error = QDBusError(QDBusError::InvalidService, QString::fromLatin1("Invalid service name: %1").arg(name));
        return false;
    }

    inline bool checkObjectPath(const QString &path, AllowEmptyFlag empty, QDBusError *error)
    {
        if (path.isEmpty()) {
            if (empty == EmptyAllowed) return true;
            *error = QDBusError(QDBusError::InvalidObjectPath, QLatin1String("Object path cannot be empty"));
            return false;
        }
        if (isValidObjectPath(path)) return true;
        *error = QDBusError(QDBusError::InvalidObjectPath, QString::fromLatin1("Invalid object path: %1").arg(path));
        return false;
    }

    inline bool checkMemberName(const QString &name, QDBusError *error)
    {
        if (name.isEmpty()) {
            *error = QDBusError(QDBusError::InvalidMember, QLatin1String("method name cannot be empty"));
            return false;
        }
        if (isValidMemberName(name)) return true;
        *error = QDBusError(QDBusError::InvalidMember, QString::fromLatin1("Invalid method name: %2")
                            .arg(name));
        return false;
    }

    inline bool checkErrorName(const QString &name, QDBusError *error)
    {
        if (name.isEmpty()) {
            *error = QDBusError(QDBusError::InvalidInterface, QLatin1String("Error name cannot be empty"));
            return false;
        }
        if (isValidErrorName(name)) return true;
        *error = QDBusError(QDBusError::InvalidInterface, QString::fromLatin1("Invalid error name: %1").arg(name));
        return false;
    }
}

QT_END_NAMESPACE


#endif
