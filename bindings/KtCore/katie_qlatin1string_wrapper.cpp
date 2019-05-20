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
#include "katie_qlatin1string_wrapper.h"

// inner classes

// Extra includes
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


// Target ---------------------------------------------------------

extern "C" {
static int
Sbk_Katie_QLatin1String_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QLatin1String >()))
        return -1;

    ::Katie::QLatin1String* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QLatin1String", 1, 1, &(pyArgs[0])))
        return -1;


    // Overloaded function decisor
    // 0: QLatin1String::QLatin1String(Katie::QLatin1String)
    // 1: QLatin1String::QLatin1String(const char*)
    if (numArgs == 1
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        overloadId = 1; // QLatin1String(const char*)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        overloadId = 0; // QLatin1String(Katie::QLatin1String)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QLatin1String_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QLatin1String(const Katie::QLatin1String & QLatin1String)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // QLatin1String(Katie::QLatin1String)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QLatin1String(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QLatin1String(const char * s)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QLatin1String(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QLatin1String(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QLatin1String >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QLatin1String_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QLatin1String_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QLatin1String");
        return -1;
}

static PyObject* Sbk_Katie_QLatin1StringFunc_latin1(PyObject* self)
{
    ::Katie::QLatin1String* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QLatin1String *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // latin1()const
            const char * cppResult = const_cast<const ::Katie::QLatin1String*>(cppSelf)->latin1();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QLatin1String___copy__(PyObject* self)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QLatin1String& cppSelf = *reinterpret_cast< ::Katie::QLatin1String *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), &cppSelf);
    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QLatin1String_methods[] = {
    {"latin1", reinterpret_cast<PyCFunction>(Sbk_Katie_QLatin1StringFunc_latin1), METH_NOARGS},

    {"__copy__", reinterpret_cast<PyCFunction>(Sbk_Katie_QLatin1String___copy__), METH_NOARGS},
    {nullptr, nullptr} // Sentinel
};

// Rich comparison
static PyObject* Sbk_Katie_QLatin1String_richcompare(PyObject* self, PyObject* pyArg, int op)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QLatin1String& cppSelf = *reinterpret_cast< ::Katie::QLatin1String *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], reinterpret_cast<SbkObject *>(self)));
    SBK_UNUSED(cppSelf)
    PyObject* pyResult{};
    PythonToCppFunc pythonToCpp;
    SBK_UNUSED(pythonToCpp)

    switch (op) {
        case Py_NE:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator!=(const char * s) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf !=(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator!=(const Katie::QString & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QString cppArg0_local;
                ::Katie::QString* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf !=(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                pyResult = Py_True;
                Py_INCREF(pyResult);
            }

            break;
        case Py_LT:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator<(const char * s) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf <(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator<(const Katie::QString & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QString cppArg0_local;
                ::Katie::QString* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf <(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                goto Sbk_Katie_QLatin1String_RichComparison_TypeError;
            }

            break;
        case Py_LE:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator<=(const char * s) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf <=(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator<=(const Katie::QString & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QString cppArg0_local;
                ::Katie::QString* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf <=(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                goto Sbk_Katie_QLatin1String_RichComparison_TypeError;
            }

            break;
        case Py_EQ:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator==(const char * s) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf ==(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator==(const Katie::QString & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QString cppArg0_local;
                ::Katie::QString* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf ==(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                pyResult = Py_False;
                Py_INCREF(pyResult);
            }

            break;
        case Py_GT:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator>(const char * s) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf >(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator>(const Katie::QString & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QString cppArg0_local;
                ::Katie::QString* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf >(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                goto Sbk_Katie_QLatin1String_RichComparison_TypeError;
            }

            break;
        case Py_GE:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator>=(const char * s) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf >=(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator>=(const Katie::QString & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QString cppArg0_local;
                ::Katie::QString* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf >=(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                goto Sbk_Katie_QLatin1String_RichComparison_TypeError;
            }

            break;
        default:
            goto Sbk_Katie_QLatin1String_RichComparison_TypeError;
    }

    if (pyResult && !PyErr_Occurred())
        return pyResult;
    Sbk_Katie_QLatin1String_RichComparison_TypeError:
    PyErr_SetString(PyExc_NotImplementedError, "operator not implemented.");
    return {};

}

} // extern "C"

static int Sbk_Katie_QLatin1String_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QLatin1String_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QLatin1String_Type = nullptr;
static SbkObjectType *Sbk_Katie_QLatin1String_TypeF(void)
{
    return _Sbk_Katie_QLatin1String_Type;
}

static PyType_Slot Sbk_Katie_QLatin1String_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QLatin1String_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QLatin1String_clear)},
    {Py_tp_richcompare, reinterpret_cast<void*>(Sbk_Katie_QLatin1String_richcompare)},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QLatin1String_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QLatin1String_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QLatin1String_spec = {
    "KtCore.Katie.QLatin1String",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QLatin1String_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QLatin1String_PythonToCpp_QLatin1String_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QLatin1String_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QLatin1String_PythonToCpp_QLatin1String_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QLatin1String_TypeF())))
        return QLatin1String_PythonToCpp_QLatin1String_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QLatin1String_PTR_CppToPython_QLatin1String(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QLatin1String *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QLatin1String_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// C++ to Python copy conversion.
static PyObject* QLatin1String_COPY_CppToPython_QLatin1String(const void* cppIn) {
    return Shiboken::Object::newObject(Sbk_Katie_QLatin1String_TypeF(), new ::Katie::QLatin1String(*reinterpret_cast<const ::Katie::QLatin1String*>(cppIn)), true, true);
}

// Python to C++ copy conversion.
static void QLatin1String_PythonToCpp_QLatin1String_COPY(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QLatin1String*>(cppOut) = *reinterpret_cast< ::Katie::QLatin1String *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], reinterpret_cast<SbkObject *>(pyIn)));
}
static PythonToCppFunc is_QLatin1String_PythonToCpp_QLatin1String_COPY_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QLatin1String_TypeF())))
        return QLatin1String_PythonToCpp_QLatin1String_COPY;
    return {};
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QLatin1String_SignatureStrings[] = {
    "1:KtCore.Katie.QLatin1String(QLatin1String:KtCore.Katie.QLatin1String)",
    "0:KtCore.Katie.QLatin1String(s:str)",
    "KtCore.Katie.QLatin1String.latin1()->str",
    "KtCore.Katie.QLatin1String.__copy__()",
    nullptr}; // Sentinel

void init_Katie_QLatin1String(PyObject* module)
{
    _Sbk_Katie_QLatin1String_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QLatin1String",
        "Katie::QLatin1String",
        &Sbk_Katie_QLatin1String_spec,
        Katie_QLatin1String_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QLatin1String >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QLatin1String_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QLatin1String_TypeF(),
        QLatin1String_PythonToCpp_QLatin1String_PTR,
        is_QLatin1String_PythonToCpp_QLatin1String_PTR_Convertible,
        QLatin1String_PTR_CppToPython_QLatin1String,
        QLatin1String_COPY_CppToPython_QLatin1String);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QLatin1String");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QLatin1String*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QLatin1String&");
    Shiboken::Conversions::registerConverterName(converter, "QLatin1String");
    Shiboken::Conversions::registerConverterName(converter, "QLatin1String*");
    Shiboken::Conversions::registerConverterName(converter, "QLatin1String&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QLatin1String).name());

    // Add Python to C++ copy (value, not pointer neither reference) conversion to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        QLatin1String_PythonToCpp_QLatin1String_COPY,
        is_QLatin1String_PythonToCpp_QLatin1String_COPY_Convertible);


}
