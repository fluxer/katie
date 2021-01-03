/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
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

#include "qdbusargument_p.h"
#include "qdbusconnection.h"
#include <stdlib.h>

QT_BEGIN_NAMESPACE

QDBusDemarshaller::~QDBusDemarshaller()
{
}

inline QString QDBusDemarshaller::currentSignature()
{
    char *sig = dbus_message_iter_get_signature(&iterator);
    QString retval = QString::fromUtf8(sig);
    dbus_free(sig);

    return retval;
}

inline uchar QDBusDemarshaller::toByte()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.byt;
}

inline bool QDBusDemarshaller::toBool()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.bool_val;
}

inline ushort QDBusDemarshaller::toUShort()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.u16;
}

inline short QDBusDemarshaller::toShort()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.i16;
}

inline int QDBusDemarshaller::toInt()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.i32;
}

inline uint QDBusDemarshaller::toUInt()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.u32;
}

inline qlonglong QDBusDemarshaller::toLongLong()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.i64;
}

inline qulonglong QDBusDemarshaller::toULongLong()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.u64;
}

inline double QDBusDemarshaller::toDouble()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return value.dbl;
}

inline QString QDBusDemarshaller::toStringUnchecked()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return QString::fromUtf8(value.str);
}

inline QString QDBusDemarshaller::toString()
{
    if (isCurrentTypeStringLike())
        return toStringUnchecked();
    return QString();
}

inline QDBusObjectPath QDBusDemarshaller::toObjectPathUnchecked()
 {
     return QDBusObjectPath(toStringUnchecked());
 }

inline QDBusObjectPath QDBusDemarshaller::toObjectPath()
{
    if (isCurrentTypeStringLike())
        return toObjectPathUnchecked();
    return QDBusObjectPath();
}

inline QDBusSignature QDBusDemarshaller::toSignatureUnchecked()
 {
     return QDBusSignature(toStringUnchecked());
 }

inline QDBusSignature QDBusDemarshaller::toSignature()
{
    if (isCurrentTypeStringLike())
        return toSignatureUnchecked();
    return QDBusSignature();
}

inline QDBusUnixFileDescriptor QDBusDemarshaller::toUnixFileDescriptor()
{
    DBusBasicValue value;
    dbus_message_iter_get_basic(&iterator, &value);
    dbus_message_iter_next(&iterator);
    return QDBusUnixFileDescriptor(value.fd);
}

inline QDBusVariant QDBusDemarshaller::toVariant()
{
    QDBusDemarshaller sub(capabilities);
    sub.message = dbus_message_ref(message);
    dbus_message_iter_recurse(&iterator, &sub.iterator);
    dbus_message_iter_next(&iterator);

    return QDBusVariant( sub.toVariantInternal() );
}

QDBusArgument::ElementType QDBusDemarshaller::currentType()
{
    switch (dbus_message_iter_get_arg_type(&iterator)) {
    case DBUS_TYPE_BYTE:
    case DBUS_TYPE_INT16:
    case DBUS_TYPE_UINT16:
    case DBUS_TYPE_INT32:
    case DBUS_TYPE_UINT32:
    case DBUS_TYPE_INT64:
    case DBUS_TYPE_UINT64:
    case DBUS_TYPE_BOOLEAN:
    case DBUS_TYPE_DOUBLE:
    case DBUS_TYPE_STRING:
    case DBUS_TYPE_OBJECT_PATH:
    case DBUS_TYPE_SIGNATURE:
        return QDBusArgument::BasicType;

    case DBUS_TYPE_VARIANT:
        return QDBusArgument::VariantType;

    case DBUS_TYPE_ARRAY:
        switch (dbus_message_iter_get_element_type(&iterator)) {
        case DBUS_TYPE_BYTE:
        case DBUS_TYPE_STRING:
            // QByteArray and QStringList
            return QDBusArgument::BasicType;
        case DBUS_TYPE_DICT_ENTRY:
            return QDBusArgument::MapType;
        default:
            return QDBusArgument::ArrayType;
        }

    case DBUS_TYPE_STRUCT:
        return QDBusArgument::StructureType;
    case DBUS_TYPE_DICT_ENTRY:
        return QDBusArgument::MapEntryType;

    case DBUS_TYPE_UNIX_FD:
        return capabilities & QDBusConnection::UnixFileDescriptorPassing ?
                    QDBusArgument::BasicType : QDBusArgument::UnknownType;

    case DBUS_TYPE_INVALID:
        return QDBusArgument::UnknownType;

//    default:
//        qWarning("QDBusDemarshaller: Found unknown D-Bus type %d '%c'",
//                 dbus_message_iter_get_arg_type(&iterator),
//                 dbus_message_iter_get_arg_type(&iterator));
    }
    return QDBusArgument::UnknownType;
}

QVariant QDBusDemarshaller::toVariantInternal()
{
    switch (dbus_message_iter_get_arg_type(&iterator)) {
    case DBUS_TYPE_BYTE:
        return QVariant::fromValue(toByte());
    case DBUS_TYPE_INT16:
        return QVariant::fromValue(toShort());
    case DBUS_TYPE_UINT16:
        return QVariant::fromValue(toUShort());
    case DBUS_TYPE_INT32:
        return toInt();
    case DBUS_TYPE_UINT32:
        return toUInt();
    case DBUS_TYPE_DOUBLE:
        return toDouble();
    case DBUS_TYPE_BOOLEAN:
        return toBool();
    case DBUS_TYPE_INT64:
        return toLongLong();
    case DBUS_TYPE_UINT64:
        return toULongLong();
    case DBUS_TYPE_STRING:
        return toStringUnchecked();
    case DBUS_TYPE_OBJECT_PATH:
        return QVariant::fromValue(toObjectPathUnchecked());
    case DBUS_TYPE_SIGNATURE:
        return QVariant::fromValue(toSignatureUnchecked());
    case DBUS_TYPE_VARIANT:
        return QVariant::fromValue(toVariant());

    case DBUS_TYPE_ARRAY:
        switch (dbus_message_iter_get_element_type(&iterator)) {
        case DBUS_TYPE_BYTE:
            // QByteArray
            return toByteArrayUnchecked();
        case DBUS_TYPE_STRING:
            return toStringListUnchecked();
        case DBUS_TYPE_DICT_ENTRY:
            return QVariant::fromValue(duplicate());

        default:
            return QVariant::fromValue(duplicate());
        }

    case DBUS_TYPE_STRUCT:
        return QVariant::fromValue(duplicate());

    case DBUS_TYPE_UNIX_FD:
        if (capabilities & QDBusConnection::UnixFileDescriptorPassing)
            return qVariantFromValue(toUnixFileDescriptor());
        // fall through

    default:
//        qWarning("QDBusDemarshaller: Found unknown D-Bus type %d '%c'",
//                 dbus_message_iter_get_arg_type(&iterator),
//                 dbus_message_iter_get_arg_type(&iterator));
        char *ptr = 0;
        ptr += dbus_message_iter_get_arg_type(&iterator);
        dbus_message_iter_next(&iterator);

        // I hope you never dereference this pointer!
        return QVariant::fromValue<void *>(ptr);
    };
}

bool QDBusDemarshaller::isCurrentTypeStringLike()
{
    const int type = dbus_message_iter_get_arg_type(&iterator);
    switch (type) {
    case DBUS_TYPE_STRING:  //FALLTHROUGH
    case DBUS_TYPE_OBJECT_PATH:  //FALLTHROUGH
    case DBUS_TYPE_SIGNATURE:
        return true;
    default:
        return false;
    }
}

QStringList QDBusDemarshaller::toStringListUnchecked()
{
    QStringList list;

    QDBusDemarshaller sub(capabilities);
    dbus_message_iter_recurse(&iterator, &sub.iterator);
    dbus_message_iter_next(&iterator);
    while (!sub.atEnd())
        list.append(sub.toStringUnchecked());

    return list;
}

QStringList QDBusDemarshaller::toStringList()
{
    if (dbus_message_iter_get_arg_type(&iterator) == DBUS_TYPE_ARRAY
            && dbus_message_iter_get_element_type(&iterator) == DBUS_TYPE_STRING) {
        return toStringListUnchecked();
    }
    return QStringList();
}

QByteArray QDBusDemarshaller::toByteArrayUnchecked()
{
    DBusMessageIter sub;
    dbus_message_iter_recurse(&iterator, &sub);
    dbus_message_iter_next(&iterator);
    int len;
    char* data;
    dbus_message_iter_get_fixed_array(&sub,&data,&len);
    return QByteArray(data,len);
}

QByteArray QDBusDemarshaller::toByteArray()
{
    if (dbus_message_iter_get_arg_type(&iterator) == DBUS_TYPE_ARRAY
            && dbus_message_iter_get_element_type(&iterator) == DBUS_TYPE_BYTE) {
        return toByteArrayUnchecked();
    }
    return QByteArray();
}

bool QDBusDemarshaller::atEnd()
{
    // dbus_message_iter_has_next is broken if the list has one single element
    return dbus_message_iter_get_arg_type(&iterator) == DBUS_TYPE_INVALID;
}

inline QDBusDemarshaller *QDBusDemarshaller::beginStructure()
{
    return beginCommon();
}

inline QDBusDemarshaller *QDBusDemarshaller::beginArray()
{
    return beginCommon();
}

inline QDBusDemarshaller *QDBusDemarshaller::beginMap()
{
    return beginCommon();
}

inline QDBusDemarshaller *QDBusDemarshaller::beginMapEntry()
{
    return beginCommon();
}

QDBusDemarshaller *QDBusDemarshaller::beginCommon()
{
    QDBusDemarshaller *d = new QDBusDemarshaller(capabilities);
    d->parent = this;
    d->message = dbus_message_ref(message);

    // recurse
    dbus_message_iter_recurse(&iterator, &d->iterator);
    dbus_message_iter_next(&iterator);
    return d;
}

inline QDBusDemarshaller *QDBusDemarshaller::endStructure()
{
    return endCommon();
}

inline QDBusDemarshaller *QDBusDemarshaller::endArray()
{
    return endCommon();
}

inline QDBusDemarshaller *QDBusDemarshaller::endMap()
{
    return endCommon();
}

inline QDBusDemarshaller *QDBusDemarshaller::endMapEntry()
{
    return endCommon();
}

QDBusDemarshaller *QDBusDemarshaller::endCommon()
{
    QDBusDemarshaller *retval = parent;
    delete this;
    return retval;
}

QDBusArgument QDBusDemarshaller::duplicate()
{
    QDBusDemarshaller *d = new QDBusDemarshaller(capabilities);
    d->iterator = iterator;
    d->message = dbus_message_ref(message);

    dbus_message_iter_next(&iterator);
    return QDBusArgumentPrivate::create(d);
}

QT_END_NAMESPACE
