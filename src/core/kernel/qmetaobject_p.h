/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
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

#ifndef QMETAOBJECT_P_H
#define QMETAOBJECT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists for the convenience
// of moc.  This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

#include "qobjectdefs.h"
#include "qcorecommon_p.h"

#ifndef QT_NO_QOBJECT
#  include "qobject_p.h"
#endif

#ifndef QT_BOOTSTRAPPED
#  include <qmutex.h>
#endif

QT_BEGIN_NAMESPACE

enum PropertyFlags  {
    Invalid = 0x00000000,
    Readable = 0x00000001,
    Writable = 0x00000002,
    Resettable = 0x00000004,
    EnumOrFlag = 0x00000008,
    StdCppSet = 0x00000100,
    // Override = 0x00000200,
    Constant = 0x00000400,
    Final = 0x00000800,
    Designable = 0x00001000,
    ResolveDesignable = 0x00002000,
    Scriptable = 0x00004000,
    ResolveScriptable = 0x00008000,
    Stored = 0x00010000,
    ResolveStored = 0x00020000,
    Editable = 0x00040000,
    ResolveEditable = 0x00080000,
    User = 0x00100000,
    ResolveUser = 0x00200000,
    Notify = 0x00400000,
    Revisioned = 0x00800000
};

enum MethodFlags  {
    AccessPrivate = 0x00,
    AccessProtected = 0x01,
    AccessPublic = 0x02,
    AccessMask = 0x03, //mask

    MethodMethod = 0x00,
    MethodSignal = 0x04,
    MethodSlot = 0x08,
    MethodConstructor = 0x0c,
    MethodTypeMask = 0x0c,

    MethodCloned = 0x10,
    MethodScriptable = 0x20,
    MethodRevisioned = 0x40
};

enum MetaObjectFlags {
    DynamicMetaObject = 0x01,
    RequiresVariantMetaObject = 0x02
};

class QMutex;

struct QMetaObjectPrivate
{
    int revision;
    int className;
    int classInfoCount, classInfoData;
    int methodCount, methodData;
    int propertyCount, propertyData;
    int enumeratorCount, enumeratorData;
    int constructorCount, constructorData; //since revision 2
    int flags; //since revision 3
    int signalCount; //since revision 4
    // revision 5 introduces changes in normalized signatures, no new members
    // revision 6 added qt_static_metacall as a member of each Q_OBJECT and inside QMetaObject itself

    static inline const QMetaObjectPrivate *get(const QMetaObject *metaobject)
    { return reinterpret_cast<const QMetaObjectPrivate*>(metaobject->d.data); }

    static int indexOfSignalRelative(const QMetaObject **baseObject,
                                     const char* name);
    static int indexOfSlotRelative(const QMetaObject **m,
                           const char *slot);
    static int originalClone(const QMetaObject *obj, int local_method_index);

#ifndef QT_NO_QOBJECT
    //defined in qobject.cpp
    enum DisconnectType { DisconnectAll, DisconnectOne };
    static void memberIndexes(const QObject *obj, const QMetaMethod &member,
                              int *signalIndex, int *methodIndex);
    static bool connect(const QObject *sender, int signal_index,
                        const QObject *receiver, int method_index_relative,
                        const QMetaObject *rmeta = Q_NULLPTR,
                        int type = 0, int *types = Q_NULLPTR);
    static bool disconnect(const QObject *sender, int signal_index,
                           const QObject *receiver, int method_index,
                           DisconnectType = DisconnectAll);
    static inline bool disconnectHelper(QObjectPrivate::Connection *c,
                                        const QObject *receiver, int method_index,
                                        QMutex *senderMutex, DisconnectType);
#endif
};

#ifndef UTILS_H
// mirrored in moc's utils.h
static inline bool is_ident_char(char s)
{
    return ((s >= 'a' && s <= 'z')
            || (s >= 'A' && s <= 'Z')
            || (s >= '0' && s <= '9')
            || s == '_'
       );
}

static inline bool is_space(char s)
{
    return (s == ' ' || s == '\t');
}
#endif

#ifndef QT_BOOTSTRAPPED
typedef QHash<quint32, QByteArray> QNormalizedTypeHash;
Q_GLOBAL_STATIC(QNormalizedTypeHash, qGlobalNormalizedTypeHash);
Q_GLOBAL_STATIC(QMutex, qGlobalNormalizedTypeMutex)
#endif

static const struct TypeTblData {
    const char* original;
    const int originalsize;
    const char* substitute;
    const int substitutesize;
} TypeTbl[] = {
    // remove 'struct', 'class', and 'enum' in the middle
    { " struct ", 8, " ", 1 },
    { " class ", 7, " ", 1 },
    { " enum ", 6, " ", 1 },
    // substitute 'unsigned x' with those defined in global header
    { "unsigned int", 12, "uint", 4 },
    { "unsigned long long", 18, "ulonglong", 9 },
    { "unsigned long", 13, "ulong", 5 },
    { "unsigned short", 14, "ushort", 6 },
    { "unsigned char", 13, "uchar", 5 }
};
static const qint16 TypeTblSize = sizeof(TypeTbl) / sizeof(TypeTblData);

static inline bool arrayStartsWith(const QByteArray &array, const char *array2, const int len) {
    return (qstrncmp(array.constData(), array2, len) == 0);
}


// This code is shared with moc.cpp
static inline QByteArray normalizeTypeInternal(const char *t, const char *e)
{
    const int len = e - t;
#ifndef QT_BOOTSTRAPPED
    const quint32 cachekey = qChecksum32(t, len);
    QMutexLocker lock(qGlobalNormalizedTypeMutex());
    QByteArray cached = qGlobalNormalizedTypeHash()->value(cachekey);
    if (!cached.isEmpty()) {
        return cached;
    }
    lock.unlock();
#endif

    QByteArray result = QByteArray(t, len);

    /*
      Convert 'char const *' into 'const char *'. Start at index 1,
      not 0, because 'const char *' is already OK. We musn't convert
      'char * const *' into 'const char **' and we must beware of
      'Bar<const Bla>'.
    */
    int searchindex = result.indexOf(" const", 1);
    while (searchindex > 0) {
        if (result.at(searchindex - 1) != '*') {
            result.remove(searchindex, 6);
            if (!arrayStartsWith(result, "const ", 6)) {
                result.prepend("const ");
            }
        }
        searchindex = result.indexOf(" const", searchindex + 1);
    }

    // convert const reference to value and const value to value
    char lastchar = result.at(result.size()-1);
    if (arrayStartsWith(result, "const ", 6) && lastchar != '*') {
        result.remove(0, 6);
        if (lastchar == '&') {
            result.chop(1);
        }
    }

    // discard 'struct', 'class', and 'enum'; they are optional
    // and we don't want them in the normalized signature
    if (arrayStartsWith(result, "struct ", 7)) {
        result.remove(0, 7);
    } else if (arrayStartsWith(result, "class ", 6)) {
        result.remove(0, 6);
    } else if (arrayStartsWith(result, "enum ", 5)) {
        result.remove(0, 5);
    }

    // discard from table, does not use overload and avoids strlen() call
    for (qint16 i = 0; i < TypeTblSize; i++) {
        result.replace(TypeTbl[i].original, TypeTbl[i].originalsize,
            TypeTbl[i].substitute, TypeTbl[i].substitutesize);
     }

#ifndef QT_BOOTSTRAPPED
    lock.relock();
    qGlobalNormalizedTypeHash()->insert(cachekey, result);
#endif

    // qDebug() << "t" << t << "e" << e << "result" << result;

    return result;
}


QT_END_NAMESPACE

#endif

