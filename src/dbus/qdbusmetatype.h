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

#ifndef QDBUSMETATYPE_H
#define QDBUSMETATYPE_H

#include "QtCore/qmetatype.h"
#include <QtDBus/qdbusargument.h>



QT_BEGIN_NAMESPACE


class Q_DBUS_EXPORT QDBusMetaType
{
public:
    typedef void (*MarshallFunction)(QDBusArgument &, const void *);
    typedef void (*DemarshallFunction)(const QDBusArgument &, void *);

    static void registerMarshallOperators(int typeId, MarshallFunction, DemarshallFunction);
    static bool marshall(QDBusArgument &, int id, const void *data);
    static bool demarshall(const QDBusArgument &, int id, void *data);

    static int signatureToType(const char *signature);
    static const char *typeToSignature(int type);
};

template<typename T>
void qDBusMarshallHelper(QDBusArgument &arg, const T *t)
{ arg << *t; }

template<typename T>
void qDBusDemarshallHelper(const QDBusArgument &arg, T *t)
{ arg >> *t; }

template<typename T>
int qDBusRegisterMetaType()
{
    void (*mf)(QDBusArgument &, const T *) = qDBusMarshallHelper<T>;
    void (*df)(const QDBusArgument &, T *) = qDBusDemarshallHelper<T>;

    int id = qRegisterMetaType<T>(); // make sure it's registered
    QDBusMetaType::registerMarshallOperators(id,
        reinterpret_cast<QDBusMetaType::MarshallFunction>(mf),
        reinterpret_cast<QDBusMetaType::DemarshallFunction>(df));
    return id;
}

QT_END_NAMESPACE


#endif
