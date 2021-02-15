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

#ifndef QDECLARATIVEVMEMETAOBJECT_P_H
#define QDECLARATIVEVMEMETAOBJECT_P_H

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

#include <QtCore/QMetaObject>
#include <QtCore/QBitArray>
#include <QtCore/QPair>
#include <QtGui/QColor>
#include <QtCore/qdatetime.h>
#include <QtCore/qlist.h>
#include <QtCore/qdebug.h>

#include "qobject_p.h"

#include "qdeclarativeguard_p.h"
#include "qdeclarativecompiler_p.h"
#include "qdeclarativecontext_p.h"

QT_BEGIN_NAMESPACE

#define QML_ALIAS_FLAG_PTR 0x00000001

struct QDeclarativeVMEMetaData
{
    short propertyCount;
    short aliasCount;
    short signalCount;
    short methodCount;

    struct AliasData {
        int contextIdx;
        int propertyIdx;
        int flags;

        bool isObjectAlias() const {
            return propertyIdx == -1;
        }
        bool isPropertyAlias() const {
            return !isObjectAlias() && !(propertyIdx & 0xFF000000);
        }
        bool isValueTypeAlias() const {
            return !isObjectAlias() && (propertyIdx & 0xFF000000);
        }
        int propertyIndex() const {
            return propertyIdx & 0x0000FFFF;
        }
        int valueTypeIndex() const {
            return (propertyIdx & 0x00FF0000) >> 16;
        }
        int valueType() const {
            return ((unsigned int)propertyIdx) >> 24;
        }
    };
    
    struct PropertyData {
        int propertyType;
    };

    struct MethodData {
        int parameterCount;
        int bodyOffset;
        int bodyLength;
        int lineNumber;
    };

    PropertyData *propertyData() const {
        return (PropertyData *)(((const char *)this) + sizeof(QDeclarativeVMEMetaData));
    }

    AliasData *aliasData() const {
        return (AliasData *)(propertyData() + propertyCount);
    }

    MethodData *methodData() const {
        return (MethodData *)(aliasData() + aliasCount);
    }
};

class QDeclarativeVMEVariant;
class QDeclarativeRefCount;
class QDeclarativeVMEMetaObject : public QAbstractDynamicMetaObject
{
public:
    QDeclarativeVMEMetaObject(QObject *obj, const QMetaObject *other, const QDeclarativeVMEMetaData *data,
                     QDeclarativeCompiledData *compiledData);
    ~QDeclarativeVMEMetaObject();

    bool aliasTarget(int index, QObject **target, int *coreIndex, int *valueTypeIndex) const;
    void registerInterceptor(int index, int valueIndex, QDeclarativePropertyValueInterceptor *interceptor);
    QScriptValue vmeMethod(int index);
    int vmeMethodLineNumber(int index);
    void setVmeMethod(int index, const QScriptValue &);
    QScriptValue vmeProperty(int index);
    void setVMEProperty(int index, const QScriptValue &);

    void connectAliasSignal(int index);

protected:
    virtual int metaCall(QMetaObject::Call _c, int _id, void **_a);

private:
    QObject *object;
    QDeclarativeCompiledData *compiledData;
    QDeclarativeGuardedContextData ctxt;

    const QDeclarativeVMEMetaData *metaData;
    int propOffset;
    int methodOffset;

    QDeclarativeVMEVariant *data;

    void connectAlias(int aliasId);
    QBitArray aConnected;
    QBitArray aInterceptors;
    QHash<int, QPair<int, QDeclarativePropertyValueInterceptor*> > interceptors;

    QScriptValue *methods;
    QScriptValue method(int);

    QScriptValue readVarProperty(int);
    QVariant readVarPropertyAsVariant(int);
    void writeVarProperty(int, const QScriptValue &);
    void writeVarProperty(int, const QVariant &);

    QAbstractDynamicMetaObject *parent;

    void listChanged(int);
    class List : public QList<QObject*>
    {
    public:
        List(int lpi) : QList<QObject*>(), notifyIndex(lpi) {}
        int notifyIndex;
    };
    QList<List> listProperties;

    static void list_append(QDeclarativeListProperty<QObject> *, QObject *);
    static int list_count(QDeclarativeListProperty<QObject> *);
    static QObject *list_at(QDeclarativeListProperty<QObject> *, int);
    static void list_clear(QDeclarativeListProperty<QObject> *);
};

QT_END_NAMESPACE

#endif // QDECLARATIVEVMEMETAOBJECT_P_H
