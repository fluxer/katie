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

#ifndef QSCRIPTDECLARATIVEOBJECT_P_H
#define QSCRIPTDECLARATIVEOBJECT_P_H

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

#include <QtCore/qobjectdefs.h>

#include "Platform.h"
#include "qscriptobject_p.h"
#include "qscriptdeclarativeclass_p.h"

QT_BEGIN_NAMESPACE

class QScriptClass;

class QScriptDeclarativeClassPrivate
{
public:
    QScriptDeclarativeClassPrivate() : engine(0), context(0), supportsCall(false) {}

    QScriptEngine *engine;
    QScriptContext *context;
    bool supportsCall;

    static QScriptDeclarativeClassPrivate *get(QScriptDeclarativeClass *c) {
        return c->d_ptr;
    }
};

namespace QScript
{

class DeclarativeObjectDelegate : public QScriptObjectDelegate
{
public:
    DeclarativeObjectDelegate(QScriptDeclarativeClass *c, QScriptDeclarativeClass::Object *o);
    ~DeclarativeObjectDelegate();

    virtual Type type() const;

    QScriptDeclarativeClass *scriptClass() const { return m_class; }
    QScriptDeclarativeClass::Object *object() const { return m_object; }

    virtual bool getOwnPropertySlot(QScriptObject*, JSC::ExecState*,
                                    const JSC::Identifier& propertyName,
                                    JSC::PropertySlot&);
    virtual void put(QScriptObject*, JSC::ExecState* exec,
                     const JSC::Identifier& propertyName,
                     JSC::JSValue, JSC::PutPropertySlot&);
    virtual bool deleteProperty(QScriptObject*, JSC::ExecState*,
                                const JSC::Identifier& propertyName);
    virtual void getOwnPropertyNames(QScriptObject*, JSC::ExecState*,
                                     JSC::PropertyNameArray&, 
                                     JSC::EnumerationMode mode = JSC::ExcludeDontEnumProperties);

    virtual JSC::CallType getCallData(QScriptObject*, JSC::CallData&);
    static JSC::JSValue QT_FASTCALL call(JSC::ExecState*, JSC::JSObject*,
                                           JSC::JSValue, const JSC::ArgList&);

    virtual JSC::ConstructType getConstructData(QScriptObject*, JSC::ConstructData&);

    virtual bool hasInstance(QScriptObject*, JSC::ExecState*,
                             JSC::JSValue value, JSC::JSValue proto);

    bool compareToObject(QScriptObject *, JSC::ExecState *, JSC::JSObject *);

private:
    QScriptDeclarativeClass *m_class;
    QScriptDeclarativeClass::Object *m_object;
};

} // namespace QScript

QT_END_NAMESPACE

#endif
