/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtScript module of the Katie Toolkit.
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

#ifndef QSCRIPTFUNCTION_P_H
#define QSCRIPTFUNCTION_P_H

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

#include "qscriptengine.h"

#include "PrototypeFunction.h"

QT_BEGIN_NAMESPACE

namespace QScript
{

class FunctionWrapper : public JSC::PrototypeFunction // ### subclass InternalFunction instead
{
public:
    // work around CELL_SIZE limitation
    struct Data
    {
        QScriptEngine::FunctionSignature function;
    };

    FunctionWrapper(JSC::ExecState*, int length, const JSC::Identifier&,
                    QScriptEngine::FunctionSignature);
    ~FunctionWrapper();

    virtual const JSC::ClassInfo* classInfo() const { return &info; }
    static const JSC::ClassInfo info;

    QScriptEngine::FunctionSignature function() const
    { return data->function; }

private:
    virtual JSC::ConstructType getConstructData(JSC::ConstructData&);

    static JSC::JSValue QT_FASTCALL proxyCall(JSC::ExecState *, JSC::JSObject *,
                                                JSC::JSValue, const JSC::ArgList &);
    static JSC::JSObject* proxyConstruct(JSC::ExecState *, JSC::JSObject *,
                                         const JSC::ArgList &);

private:
    Data *data;
};

class FunctionWithArgWrapper : public JSC::PrototypeFunction
{
public:
    // work around CELL_SIZE limitation
    struct Data
    {
        QScriptEngine::FunctionWithArgSignature function;
        void *arg;
    };

    FunctionWithArgWrapper(JSC::ExecState*, int length, const JSC::Identifier&,
                           QScriptEngine::FunctionWithArgSignature, void *);
    ~FunctionWithArgWrapper();

    virtual const JSC::ClassInfo* classInfo() const { return &info; }
    static const JSC::ClassInfo info;

    QScriptEngine::FunctionWithArgSignature function() const
    { return data->function; }

    void *arg() const
    { return data->arg; }

private:
    virtual JSC::ConstructType getConstructData(JSC::ConstructData&);

    static JSC::JSValue QT_FASTCALL proxyCall(JSC::ExecState *, JSC::JSObject *,
                                                JSC::JSValue , const JSC::ArgList &);
    static JSC::JSObject* proxyConstruct(JSC::ExecState *, JSC::JSObject *,
                                         const JSC::ArgList &);

private:
    Data *data;
};

} // namespace QScript

QT_END_NAMESPACE

#endif
