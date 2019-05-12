/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


// default includes
#include <shiboken.h>
#include <typeinfo>

// module include
#include "ktcore_python.h"

// main header
#include "katie_qobject_wrapper.h"

// inner classes

// Extra includes
#include <qobject.h>
#include <qstring.h>


#include <cctype>
#include <cstring>
using namespace Katie;



template <class T>
static const char *typeNameOf(const T &t)
{
    const char *typeName =  typeid(t).name();
    auto size = std::strlen(typeName);
#if defined(Q_CC_MSVC) // MSVC: "class QPaintDevice * __ptr64"
    if (auto lastStar = strchr(typeName, '*')) {
        // MSVC: "class QPaintDevice * __ptr64"
        while (*--lastStar == ' ') {
        }
        size = lastStar - typeName + 1;
    }
#else // g++, Clang: "QPaintDevice *" -> "P12QPaintDevice"
    if (size > 2 && typeName[0] == 'P' && std::isdigit(typeName[1])) {
        ++typeName;
        --size;
    }
#endif
    char *result = new char[size + 1];
    result[size] = '\0';
    memcpy(result, typeName, size);
    return result;
}

// Native ---------------------------------------------------------

QObjectWrapper::QObjectWrapper(Katie::QObject * parent) : Katie::QObject(parent)
{
    // ... middle
}

void QObjectWrapper::connectNotify(const char * signal)
{
    Shiboken::GilState gil;
    if (PyErr_Occurred())
        return;
    Shiboken::AutoDecRef pyOverride(Shiboken::BindingManager::instance().getOverride(this, "connectNotify"));
    if (pyOverride.isNull()) {
        gil.release();
        this->::Katie::QObject::connectNotify(signal);
        return;
    }

    Shiboken::AutoDecRef pyArgs(Py_BuildValue("(N)",
        Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), signal)
    ));

    Shiboken::AutoDecRef pyResult(PyObject_Call(pyOverride, pyArgs, nullptr));
    // An error happened in python code!
    if (pyResult.isNull()) {
        PyErr_Print();
        return;
    }
}

void QObjectWrapper::disconnectNotify(const char * signal)
{
    Shiboken::GilState gil;
    if (PyErr_Occurred())
        return;
    Shiboken::AutoDecRef pyOverride(Shiboken::BindingManager::instance().getOverride(this, "disconnectNotify"));
    if (pyOverride.isNull()) {
        gil.release();
        this->::Katie::QObject::disconnectNotify(signal);
        return;
    }

    Shiboken::AutoDecRef pyArgs(Py_BuildValue("(N)",
        Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), signal)
    ));

    Shiboken::AutoDecRef pyResult(PyObject_Call(pyOverride, pyArgs, nullptr));
    // An error happened in python code!
    if (pyResult.isNull()) {
        PyErr_Print();
        return;
    }
}

QObjectWrapper::~QObjectWrapper()
{
    SbkObject* wrapper = Shiboken::BindingManager::instance().retrieveWrapper(this);
    Shiboken::Object::destroy(wrapper, this);
}

// Target ---------------------------------------------------------

extern "C" {
static int
Sbk_Katie_QObject_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QObject >()))
        return -1;

    ::QObjectWrapper* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject(): too many arguments");
        return -1;
    }

    if (!PyArg_ParseTuple(args, "|O:QObject", &(pyArgs[0])))
        return -1;


    // Overloaded function decisor
    // 0: QObject::QObject(Katie::QObject*)
    if (numArgs == 0) {
        overloadId = 0; // QObject(Katie::QObject*)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0])))) {
        overloadId = 0; // QObject(Katie::QObject*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObject_Init_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "parent");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject(): got multiple values for keyword argument 'parent'.");
                return -1;
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0]))))
                    goto Sbk_Katie_QObject_Init_TypeError;
            }
        }
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return -1;
        ::Katie::QObject* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // QObject(Katie::QObject*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cptr = new ::QObjectWrapper(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            Shiboken::Object::setParent(pyArgs[0], self);
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QObject >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QObject_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    Shiboken::Object::setHasCppWrapper(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QObject_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QObject");
        return -1;
}

static PyObject* Sbk_Katie_QObjectFunc_blockSignals(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::blockSignals(bool)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArg)))) {
        overloadId = 0; // blockSignals(bool)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_blockSignals_TypeError;

    // Call function/method
    {
        bool cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // blockSignals(bool)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = cppSelf->blockSignals(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_blockSignals_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.blockSignals");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_connect(PyObject* self, PyObject* args, PyObject* kwds)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 5) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.connect(): too many arguments");
        return {};
    } else if (numArgs < 3) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.connect(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOOOO:connect", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2]), &(pyArgs[3]), &(pyArgs[4])))
        return {};


    // Overloaded function decisor
    // 0: static QObject::connect(const Katie::QObject*,const char*,const Katie::QObject*,const char*,Katie::Qt::ConnectionType)
    // 1: QObject::connect(const Katie::QObject*,const char*,const char*,Katie::Qt::ConnectionType)const
    if (numArgs >= 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0])))
        && Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
        if (Shiboken::String::check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[2])))) {
            if (numArgs == 3) {
                overloadId = 1; // connect(const Katie::QObject*,const char*,const char*,Katie::Qt::ConnectionType)const
            } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX])->converter, (pyArgs[3])))) {
                overloadId = 1; // connect(const Katie::QObject*,const char*,const char*,Katie::Qt::ConnectionType)const
            }
        } else if (numArgs >= 4
            && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[2])))
            && Shiboken::String::check(pyArgs[3]) && (pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[3])))) {
            if (numArgs == 4) {
                overloadId = 0; // connect(const Katie::QObject*,const char*,const Katie::QObject*,const char*,Katie::Qt::ConnectionType)
            } else if ((pythonToCpp[4] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX])->converter, (pyArgs[4])))) {
                overloadId = 0; // connect(const Katie::QObject*,const char*,const Katie::QObject*,const char*,Katie::Qt::ConnectionType)
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_connect_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // connect(const Katie::QObject * sender, const char * signal, const Katie::QObject * receiver, const char * member, Katie::Qt::ConnectionType type)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "type");
                if (value && pyArgs[4]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.connect(): got multiple values for keyword argument 'type'.");
                    return {};
                } else if (value) {
                    pyArgs[4] = value;
                    if (!(pythonToCpp[4] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX])->converter, (pyArgs[4]))))
                        goto Sbk_Katie_QObjectFunc_connect_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QObject* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QObject* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);
            const char* cppArg3;
            pythonToCpp[3](pyArgs[3], &cppArg3);
            ::Katie::Qt::ConnectionType cppArg4 = Katie::Qt::AutoConnection;
            if (pythonToCpp[4]) pythonToCpp[4](pyArgs[4], &cppArg4);

            if (!PyErr_Occurred()) {
                // connect(const Katie::QObject*,const char*,const Katie::QObject*,const char*,Katie::Qt::ConnectionType)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = ::Katie::QObject::connect(cppArg0, cppArg1, cppArg2, cppArg3, cppArg4);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // connect(const Katie::QObject * sender, const char * signal, const char * member, Katie::Qt::ConnectionType type) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "type");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.connect(): got multiple values for keyword argument 'type'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CONNECTIONTYPE_IDX])->converter, (pyArgs[3]))))
                        goto Sbk_Katie_QObjectFunc_connect_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QObject* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            const char* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);
            ::Katie::Qt::ConnectionType cppArg3 = Katie::Qt::AutoConnection;
            if (pythonToCpp[3]) pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!PyErr_Occurred()) {
                // connect(const Katie::QObject*,const char*,const char*,Katie::Qt::ConnectionType)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::QObjectWrapper*>(cppSelf)->connect(cppArg0, cppArg1, cppArg2, cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_connect_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QObject.connect");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_connectNotify(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::connectNotify(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 0; // connectNotify(const char*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_connectNotify_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // connectNotify(const char*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            static_cast<::QObjectWrapper*>(cppSelf)->QObjectWrapper::connectNotify_protected(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_connectNotify_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.connectNotify");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_deleteLater(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // deleteLater()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->deleteLater();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QObjectFunc_destroyed(PyObject* self, PyObject* args, PyObject* kwds)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.destroyed(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:destroyed", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QObject::destroyed(Katie::QObject*)
    if (numArgs == 0) {
        overloadId = 0; // destroyed(Katie::QObject*)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0])))) {
        overloadId = 0; // destroyed(Katie::QObject*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_destroyed_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "arg__1");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.destroyed(): got multiple values for keyword argument 'arg__1'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0]))))
                    goto Sbk_Katie_QObjectFunc_destroyed_TypeError;
            }
        }
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return {};
        ::Katie::QObject* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // destroyed(Katie::QObject*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            static_cast<::QObjectWrapper*>(cppSelf)->QObjectWrapper::destroyed_protected(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_destroyed_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QObject.destroyed");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_disconnect(PyObject* self, PyObject* args, PyObject* kwds)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 4) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.disconnect(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOOO:disconnect", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2]), &(pyArgs[3])))
        return {};


    // Overloaded function decisor
    // 0: QObject::disconnect(const Katie::QObject*,const char*)
    // 1: static QObject::disconnect(const Katie::QObject*,const char*,const Katie::QObject*,const char*)
    // 2: QObject::disconnect(const char*,const Katie::QObject*,const char*)
    if (numArgs == 0) {
        overloadId = 2; // disconnect(const char*,const Katie::QObject*,const char*)
    } else if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // disconnect(const char*,const Katie::QObject*,const char*)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 2; // disconnect(const char*,const Katie::QObject*,const char*)
            } else if (Shiboken::String::check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[2])))) {
                overloadId = 2; // disconnect(const char*,const Katie::QObject*,const char*)
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // disconnect(const Katie::QObject*,const char*)
        } else if (Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // disconnect(const Katie::QObject*,const char*)
            } else if (numArgs == 4
                && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[2])))
                && Shiboken::String::check(pyArgs[3]) && (pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[3])))) {
                overloadId = 1; // disconnect(const Katie::QObject*,const char*,const Katie::QObject*,const char*)
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_disconnect_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // disconnect(const Katie::QObject * receiver, const char * member)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "member");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.disconnect(): got multiple values for keyword argument 'member'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1]))))
                        goto Sbk_Katie_QObjectFunc_disconnect_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QObject* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1 = Q_NULLPTR;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // disconnect(const Katie::QObject*,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = cppSelf->disconnect(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // disconnect(const Katie::QObject * sender, const char * signal, const Katie::QObject * receiver, const char * member)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QObject* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QObject* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);
            const char* cppArg3;
            pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!PyErr_Occurred()) {
                // disconnect(const Katie::QObject*,const char*,const Katie::QObject*,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = ::Katie::QObject::disconnect(cppArg0, cppArg1, cppArg2, cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 2: // disconnect(const char * signal, const Katie::QObject * receiver, const char * member)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "signal");
                if (value && pyArgs[0]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.disconnect(): got multiple values for keyword argument 'signal'.");
                    return {};
                } else if (value) {
                    pyArgs[0] = value;
                    if (!Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0]))))
                        goto Sbk_Katie_QObjectFunc_disconnect_TypeError;
                }
                value = PyDict_GetItemString(kwds, "receiver");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.disconnect(): got multiple values for keyword argument 'receiver'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[1]))))
                        goto Sbk_Katie_QObjectFunc_disconnect_TypeError;
                }
                value = PyDict_GetItemString(kwds, "member");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QObject.disconnect(): got multiple values for keyword argument 'member'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!Shiboken::String::check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[2]))))
                        goto Sbk_Katie_QObjectFunc_disconnect_TypeError;
                }
            }
            const char* cppArg0 = Q_NULLPTR;
            if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QObject* cppArg1 = Q_NULLPTR;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            const char* cppArg2 = Q_NULLPTR;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // disconnect(const char*,const Katie::QObject*,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = cppSelf->disconnect(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_disconnect_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QObject.disconnect");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_disconnectNotify(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::disconnectNotify(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 0; // disconnectNotify(const char*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_disconnectNotify_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // disconnectNotify(const char*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            static_cast<::QObjectWrapper*>(cppSelf)->QObjectWrapper::disconnectNotify_protected(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_disconnectNotify_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.disconnectNotify");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_dumpObjectInfo(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // dumpObjectInfo()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->dumpObjectInfo();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QObjectFunc_dumpObjectTree(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // dumpObjectTree()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->dumpObjectTree();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QObjectFunc_inherits(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::inherits(const char*)const
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 0; // inherits(const char*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_inherits_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // inherits(const char*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = const_cast<const ::QObjectWrapper*>(cppSelf)->inherits(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_inherits_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.inherits");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_installEventFilter(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::installEventFilter(Katie::QObject*)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArg)))) {
        overloadId = 0; // installEventFilter(Katie::QObject*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_installEventFilter_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QObject* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // installEventFilter(Katie::QObject*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->installEventFilter(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_installEventFilter_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.installEventFilter");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_isWidgetType(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isWidgetType()const
            bool cppResult = const_cast<const ::QObjectWrapper*>(cppSelf)->isWidgetType();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QObjectFunc_killTimer(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::killTimer(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // killTimer(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_killTimer_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // killTimer(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->killTimer(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_killTimer_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.killTimer");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_objectName(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // objectName()const
            Katie::QString* cppResult = new Katie::QString(const_cast<const ::QObjectWrapper*>(cppSelf)->objectName());
            pyResult = Shiboken::Object::newObject(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), cppResult, true, true);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QObjectFunc_parent(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // parent()const
            Katie::QObject * cppResult = const_cast<const ::QObjectWrapper*>(cppSelf)->parent();
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QObjectFunc_receivers(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::receivers(const char*)const
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 0; // receivers(const char*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_receivers_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // receivers(const char*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = static_cast<::QObjectWrapper*>(cppSelf)->QObjectWrapper::receivers_protected(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_receivers_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.receivers");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_removeEventFilter(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::removeEventFilter(Katie::QObject*)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArg)))) {
        overloadId = 0; // removeEventFilter(Katie::QObject*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_removeEventFilter_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QObject* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // removeEventFilter(Katie::QObject*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->removeEventFilter(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_removeEventFilter_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.removeEventFilter");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_sender(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // sender()const
            Katie::QObject * cppResult = static_cast<::QObjectWrapper*>(cppSelf)->QObjectWrapper::sender_protected();
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QObjectFunc_senderSignalIndex(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // senderSignalIndex()const
            int cppResult = static_cast<::QObjectWrapper*>(cppSelf)->QObjectWrapper::senderSignalIndex_protected();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QObjectFunc_setObjectName(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::setObjectName(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // setObjectName(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_setObjectName_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QString* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setObjectName(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setObjectName(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_setObjectName_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.setObjectName");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_setParent(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::setParent(Katie::QObject*)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArg)))) {
        overloadId = 0; // setParent(Katie::QObject*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_setParent_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QObject* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setParent(Katie::QObject*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setParent(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QObjectFunc_setParent_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.setParent");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_signalsBlocked(PyObject* self)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // signalsBlocked()const
            bool cppResult = const_cast<const ::QObjectWrapper*>(cppSelf)->signalsBlocked();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QObjectFunc_startTimer(PyObject* self, PyObject* pyArg)
{
    QObjectWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QObjectWrapper *>(reinterpret_cast< ::Katie::QObject *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QObject::startTimer(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // startTimer(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_startTimer_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // startTimer(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = cppSelf->startTimer(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_startTimer_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QObject.startTimer");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_tr(PyObject* self, PyObject* args)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "tr", 3, 3, &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: static QObject::tr(const char*,const char*,int)
    if (numArgs == 3
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))
        && Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))
        && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
        overloadId = 0; // tr(const char*,const char*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_tr_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        const char* cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);
        int cppArg2;
        pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // tr(const char*,const char*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString* cppResult = new Katie::QString(::Katie::QObject::tr(cppArg0, cppArg1, cppArg2));
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Object::newObject(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), cppResult, true, true);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_tr_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QObject.tr");
        return {};
}

static PyObject* Sbk_Katie_QObjectFunc_trUtf8(PyObject* self, PyObject* args)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "trUtf8", 3, 3, &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: static QObject::trUtf8(const char*,const char*,int)
    if (numArgs == 3
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))
        && Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))
        && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
        overloadId = 0; // trUtf8(const char*,const char*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QObjectFunc_trUtf8_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        const char* cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);
        int cppArg2;
        pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // trUtf8(const char*,const char*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString* cppResult = new Katie::QString(::Katie::QObject::trUtf8(cppArg0, cppArg1, cppArg2));
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Object::newObject(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), cppResult, true, true);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QObjectFunc_trUtf8_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QObject.trUtf8");
        return {};
}

static PyMethodDef Sbk_Katie_QObjectMethod_connect = {
    "connect", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_connect), METH_VARARGS|METH_KEYWORDS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QObjectMethod_disconnect = {
    "disconnect", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_disconnect), METH_VARARGS|METH_KEYWORDS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QObject_methods[] = {
    {"blockSignals", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_blockSignals), METH_O},
    Sbk_Katie_QObjectMethod_connect,
    {"connectNotify", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_connectNotify), METH_O},
    {"deleteLater", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_deleteLater), METH_NOARGS},
    {"destroyed", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_destroyed), METH_VARARGS|METH_KEYWORDS},
    Sbk_Katie_QObjectMethod_disconnect,
    {"disconnectNotify", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_disconnectNotify), METH_O},
    {"dumpObjectInfo", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_dumpObjectInfo), METH_NOARGS},
    {"dumpObjectTree", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_dumpObjectTree), METH_NOARGS},
    {"inherits", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_inherits), METH_O},
    {"installEventFilter", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_installEventFilter), METH_O},
    {"isWidgetType", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_isWidgetType), METH_NOARGS},
    {"killTimer", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_killTimer), METH_O},
    {"objectName", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_objectName), METH_NOARGS},
    {"parent", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_parent), METH_NOARGS},
    {"receivers", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_receivers), METH_O},
    {"removeEventFilter", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_removeEventFilter), METH_O},
    {"sender", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_sender), METH_NOARGS},
    {"senderSignalIndex", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_senderSignalIndex), METH_NOARGS},
    {"setObjectName", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_setObjectName), METH_O},
    {"setParent", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_setParent), METH_O},
    {"signalsBlocked", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_signalsBlocked), METH_NOARGS},
    {"startTimer", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_startTimer), METH_O},
    {"tr", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_tr), METH_VARARGS|METH_STATIC},
    {"trUtf8", reinterpret_cast<PyCFunction>(Sbk_Katie_QObjectFunc_trUtf8), METH_VARARGS|METH_STATIC},

    {nullptr, nullptr} // Sentinel
};

static PyObject* Sbk_Katie_QObject_getattro(PyObject* self, PyObject* name)
{
    if (self) {
        // Search the method in the instance dict
        if (reinterpret_cast<SbkObject*>(self)->ob_dict) {
            PyObject* meth = PyDict_GetItem(reinterpret_cast<SbkObject*>(self)->ob_dict, name);
            if (meth) {
                Py_INCREF(meth);
                return meth;
            }
        }
        // Search the method in the type dict
        if (Shiboken::Object::isUserType(self)) {
            Shiboken::AutoDecRef tmp(_Pep_PrivateMangle(self, name));
            PyObject *meth = PyDict_GetItem(Py_TYPE(self)->tp_dict, tmp);
            if (meth)
                return PyFunction_Check(meth) ? SBK_PyMethod_New(meth, self) : PyObject_GenericGetAttr(self, name);
        }
        static PyMethodDef non_static_Sbk_Katie_QObjectMethod_connect = {
            Sbk_Katie_QObjectMethod_connect.ml_name,
            Sbk_Katie_QObjectMethod_connect.ml_meth,
            Sbk_Katie_QObjectMethod_connect.ml_flags & (~METH_STATIC),
            Sbk_Katie_QObjectMethod_connect.ml_doc,
        };
        if (Shiboken::String::compare(name, "connect") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QObjectMethod_connect, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QObjectMethod_disconnect = {
            Sbk_Katie_QObjectMethod_disconnect.ml_name,
            Sbk_Katie_QObjectMethod_disconnect.ml_meth,
            Sbk_Katie_QObjectMethod_disconnect.ml_flags & (~METH_STATIC),
            Sbk_Katie_QObjectMethod_disconnect.ml_doc,
        };
        if (Shiboken::String::compare(name, "disconnect") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QObjectMethod_disconnect, self, 0);
    }
    return PyObject_GenericGetAttr(self, name);
}

} // extern "C"

static int Sbk_Katie_QObject_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QObject_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QObject_Type = nullptr;
static SbkObjectType *Sbk_Katie_QObject_TypeF(void)
{
    return _Sbk_Katie_QObject_Type;
}

static PyType_Slot Sbk_Katie_QObject_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    reinterpret_cast<void*>(Sbk_Katie_QObject_getattro)},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QObject_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QObject_clear)},
    {Py_tp_richcompare, nullptr},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QObject_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QObject_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QObject_spec = {
    "KtCore.Katie.QObject",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QObject_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QObject_PythonToCpp_QObject_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QObject_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QObject_PythonToCpp_QObject_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QObject_TypeF())))
        return QObject_PythonToCpp_QObject_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QObject_PTR_CppToPython_QObject(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QObject *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QObject_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QObject_SignatureStrings[] = {
    "KtCore.Katie.QObject(parent:KtCore.Katie.QObject=Q_NULLPTR)",
    "KtCore.Katie.QObject.blockSignals(b:bool)->bool",
    "1:KtCore.Katie.QObject.connect(sender:KtCore.Katie.QObject,signal:str,receiver:KtCore.Katie.QObject,member:str,type:KtCore.Katie.Qt.ConnectionType=Qt.AutoConnection)->bool",
    "0:KtCore.Katie.QObject.connect(sender:KtCore.Katie.QObject,signal:str,member:str,type:KtCore.Katie.Qt.ConnectionType=Qt.AutoConnection)->bool",
    "KtCore.Katie.QObject.connectNotify(signal:str)",
    "KtCore.Katie.QObject.deleteLater()",
    "KtCore.Katie.QObject.destroyed(arg__1:KtCore.Katie.QObject=Q_NULLPTR)",
    "2:KtCore.Katie.QObject.disconnect(receiver:KtCore.Katie.QObject,member:str=Q_NULLPTR)->bool",
    "1:KtCore.Katie.QObject.disconnect(sender:KtCore.Katie.QObject,signal:str,receiver:KtCore.Katie.QObject,member:str)->bool",
    "0:KtCore.Katie.QObject.disconnect(signal:str=Q_NULLPTR,receiver:KtCore.Katie.QObject=Q_NULLPTR,member:str=Q_NULLPTR)->bool",
    "KtCore.Katie.QObject.disconnectNotify(signal:str)",
    "KtCore.Katie.QObject.dumpObjectInfo()",
    "KtCore.Katie.QObject.dumpObjectTree()",
    "KtCore.Katie.QObject.inherits(classname:str)->bool",
    "KtCore.Katie.QObject.installEventFilter(arg__1:KtCore.Katie.QObject)",
    "KtCore.Katie.QObject.isWidgetType()->bool",
    "KtCore.Katie.QObject.killTimer(id:int)",
    "KtCore.Katie.QObject.objectName()->KtCore.Katie.QString",
    "KtCore.Katie.QObject.parent()->KtCore.Katie.QObject",
    "KtCore.Katie.QObject.receivers(signal:str)->int",
    "KtCore.Katie.QObject.removeEventFilter(arg__1:KtCore.Katie.QObject)",
    "KtCore.Katie.QObject.sender()->KtCore.Katie.QObject",
    "KtCore.Katie.QObject.senderSignalIndex()->int",
    "KtCore.Katie.QObject.setObjectName(name:KtCore.Katie.QString)",
    "KtCore.Katie.QObject.setParent(arg__1:KtCore.Katie.QObject)",
    "KtCore.Katie.QObject.signalsBlocked()->bool",
    "KtCore.Katie.QObject.startTimer(interval:int)->int",
    "KtCore.Katie.QObject.tr(s:str,c:str,n:int)->KtCore.Katie.QString",
    "KtCore.Katie.QObject.trUtf8(s:str,c:str,n:int)->KtCore.Katie.QString",
    nullptr}; // Sentinel

void init_Katie_QObject(PyObject* module)
{
    _Sbk_Katie_QObject_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QObject",
        "Katie::QObject*",
        &Sbk_Katie_QObject_spec,
        Katie_QObject_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QObject >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QObject_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QObject_TypeF(),
        QObject_PythonToCpp_QObject_PTR,
        is_QObject_PythonToCpp_QObject_PTR_Convertible,
        QObject_PTR_CppToPython_QObject);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QObject");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QObject*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QObject&");
    Shiboken::Conversions::registerConverterName(converter, "QObject");
    Shiboken::Conversions::registerConverterName(converter, "QObject*");
    Shiboken::Conversions::registerConverterName(converter, "QObject&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QObject).name());
    Shiboken::Conversions::registerConverterName(converter, typeid(::QObjectWrapper).name());



}
