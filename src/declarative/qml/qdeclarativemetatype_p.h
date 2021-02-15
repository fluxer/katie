/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtDeclarative module of the Katie Toolkit.
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

#ifndef QDECLARATIVEMETATYPE_P_H
#define QDECLARATIVEMETATYPE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qdeclarative.h"

#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>
#include <QtCore/qbitarray.h>
#include "qdeclarativeglobal_p.h"
#include "qdeclarativedirparser_p.h"

QT_BEGIN_NAMESPACE

class QDeclarativeType;
class QDeclarativeCustomParser;
class QDeclarativeTypePrivate;

class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeMetaType
{
public:
    static bool canCopy(int type);
    static bool copy(int type, void *data, const void *copy = 0);

    static QList<QByteArray> qmlTypeNames();
    static QList<QDeclarativeType*> qmlTypes();

    static QDeclarativeType *qmlType(const QByteArray &, int, int);
    static QDeclarativeType *qmlType(const QMetaObject *);
    static QDeclarativeType *qmlType(const QMetaObject *metaObject, const QByteArray &module, int version_major, int version_minor);
    static QDeclarativeType *qmlType(int);

    static QDeclarativeDirComponents qmlComponents(const QByteArray& module, int version_major, int version_minor);

    static QMetaProperty defaultProperty(const QMetaObject *);
    static QMetaProperty defaultProperty(QObject *);
    static QMetaMethod defaultMethod(const QMetaObject *);
    static QMetaMethod defaultMethod(QObject *);

    static bool isQObject(int);
    static QObject *toQObject(const QVariant &, bool *ok = 0);

    static int listType(int);
    static int attachedPropertiesFuncId(const QMetaObject *);
    static QDeclarativeAttachedPropertiesFunc attachedPropertiesFuncById(int);

    enum TypeCategory { Unknown, Object, List };
    static TypeCategory typeCategory(int);
        
    static bool isInterface(int);
    static const char *interfaceIId(int);
    static bool isList(int);

    static bool isModule(const QByteArray &module, int versionMajor, int versionMinor);

    static QList<QDeclarativePrivate::AutoParentFunction> parentFunctions();
};

class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeType
{
public:
    QByteArray typeName() const;
    QByteArray qmlTypeName() const;

    QByteArray module() const;
    int majorVersion() const;
    int minorVersion() const;

    bool availableInVersion(int vmajor, int vminor) const;
    bool availableInVersion(const QByteArray &module, int vmajor, int vminor) const;

    QObject *create() const;
    void create(QObject **, void **, size_t) const;

    typedef void (*CreateFunc)(void *);
    CreateFunc createFunction() const;
    int createSize() const;

    QDeclarativeCustomParser *customParser() const;

    bool isCreatable() const;
    bool isExtendedType() const;
    QString noCreationReason() const;

    bool isInterface() const;
    int typeId() const;
    int qListTypeId() const;

    const QMetaObject *metaObject() const;
    const QMetaObject *baseMetaObject() const;
    int metaObjectRevision() const;
    bool containsRevisionedAttributes() const;

    QDeclarativeAttachedPropertiesFunc attachedPropertiesFunction() const;
    const QMetaObject *attachedPropertiesType() const;
    int attachedPropertiesId() const;

    int parserStatusCast() const;
    QVariant fromObject(QObject *) const;
    const char *interfaceIId() const;
    int propertyValueSourceCast() const;
    int propertyValueInterceptorCast() const;

    int index() const;

private:
    QDeclarativeType *superType() const;
    friend class QDeclarativeTypePrivate;
    friend struct QDeclarativeMetaTypeData;
    friend int registerType(const QDeclarativePrivate::RegisterType &);
    friend int registerInterface(const QDeclarativePrivate::RegisterInterface &);
    friend int registerComponent(const QDeclarativePrivate::RegisterComponent &);
    QDeclarativeType(int, const QDeclarativePrivate::RegisterInterface &);
    QDeclarativeType(int, const QDeclarativePrivate::RegisterType &);
    ~QDeclarativeType();

    QDeclarativeTypePrivate *d;
};

QT_END_NAMESPACE

#endif // QDECLARATIVEMETATYPE_P_H

