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

#ifndef QDBUSEXTRATYPES_H
#define QDBUSEXTRATYPES_H

// define some useful types for D-BUS

#include <QtCore/qvariant.h>



QT_BEGIN_NAMESPACE

class Q_DBUS_EXPORT QDBusObjectPath : private QString
{
public:
    inline QDBusObjectPath() { }

    inline explicit QDBusObjectPath(const char *path);
    inline explicit QDBusObjectPath(const QLatin1String &path);
    inline explicit QDBusObjectPath(const QString &path);
    inline QDBusObjectPath &operator=(const QDBusObjectPath &path);

    inline void setPath(const QString &path);

    inline QString path() const
    { return *this; }

private:
    void check();
};

inline QDBusObjectPath::QDBusObjectPath(const char *objectPath)
    : QString(QString::fromLatin1(objectPath))
{ check(); }

inline QDBusObjectPath::QDBusObjectPath(const QLatin1String &objectPath)
    : QString(objectPath)
{ check(); }

inline QDBusObjectPath::QDBusObjectPath(const QString &objectPath)
    : QString(objectPath)
{ check(); }

inline QDBusObjectPath &QDBusObjectPath::operator=(const QDBusObjectPath &_path)
{ QString::operator=(_path); check(); return *this; }

inline void QDBusObjectPath::setPath(const QString &objectPath)
{ QString::operator=(objectPath); check(); }

inline bool operator==(const QDBusObjectPath &lhs, const QDBusObjectPath &rhs)
{ return lhs.path() == rhs.path(); }

inline bool operator!=(const QDBusObjectPath &lhs, const QDBusObjectPath &rhs)
{ return lhs.path() != rhs.path(); }

inline bool operator<(const QDBusObjectPath &lhs, const QDBusObjectPath &rhs)
{ return lhs.path() < rhs.path(); }

inline uint qHash(const QDBusObjectPath &objectPath)
{ return qHash(objectPath.path()); }


class Q_DBUS_EXPORT QDBusSignature : private QString
{
public:
    inline QDBusSignature() { }

    inline explicit QDBusSignature(const char *signature);
    inline explicit QDBusSignature(const QLatin1String &signature);
    inline explicit QDBusSignature(const QString &signature);
    inline QDBusSignature &operator=(const QDBusSignature &signature);

    inline void setSignature(const QString &signature);

    inline QString signature() const
    { return *this; }

private:
    void check();
};

inline QDBusSignature::QDBusSignature(const char *dBusSignature)
    : QString(QString::fromAscii(dBusSignature))
{ check(); }

inline QDBusSignature::QDBusSignature(const QLatin1String &dBusSignature)
    : QString(dBusSignature)
{ check(); }

inline QDBusSignature::QDBusSignature(const QString &dBusSignature)
    : QString(dBusSignature)
{ check(); }

inline QDBusSignature &QDBusSignature::operator=(const QDBusSignature &dbusSignature)
{ QString::operator=(dbusSignature); check(); return *this; }

inline void QDBusSignature::setSignature(const QString &dBusSignature)
{ QString::operator=(dBusSignature); check(); }

inline bool operator==(const QDBusSignature &lhs, const QDBusSignature &rhs)
{ return lhs.signature() == rhs.signature(); }

inline bool operator!=(const QDBusSignature &lhs, const QDBusSignature &rhs)
{ return lhs.signature() != rhs.signature(); }

inline bool operator<(const QDBusSignature &lhs, const QDBusSignature &rhs)
{ return lhs.signature() < rhs.signature(); }

inline uint qHash(const QDBusSignature &signature)
{ return qHash(signature.signature()); }

class Q_DBUS_EXPORT QDBusVariant : private QVariant
{
public:
    inline QDBusVariant() { }
    inline explicit QDBusVariant(const QVariant &variant);

    inline void setVariant(const QVariant &variant);

    inline QVariant variant() const
    { return *this; }
};

inline  QDBusVariant::QDBusVariant(const QVariant &dBusVariant)
    : QVariant(dBusVariant) { }

inline void QDBusVariant::setVariant(const QVariant &dBusVariant)
{ QVariant::operator=(dBusVariant); }

inline bool operator==(const QDBusVariant &v1, const QDBusVariant &v2)
{ return v1.variant() == v2.variant(); }

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QDBusVariant)
Q_DECLARE_METATYPE(QDBusObjectPath)
Q_DECLARE_METATYPE(QList<QDBusObjectPath>)
Q_DECLARE_METATYPE(QDBusSignature)
Q_DECLARE_METATYPE(QList<QDBusSignature>)


#endif
