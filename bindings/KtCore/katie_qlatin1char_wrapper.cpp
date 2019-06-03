/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
**
** This file is part of the FOO module of the Katie Toolkit.
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
#include "katie_qlatin1char_wrapper.h"

// inner classes

// Extra includes


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
Sbk_Katie_QLatin1Char_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QLatin1Char >()))
        return -1;

    ::Katie::QLatin1Char* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QLatin1Char", 1, 1, &(pyArgs[0])))
        return -1;


    // Overloaded function decisor
    // 0: QLatin1Char::QLatin1Char(char)
    if (numArgs == 1
        && SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        overloadId = 0; // QLatin1Char(char)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QLatin1Char_Init_TypeError;

    // Call function/method
    {
        char cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // QLatin1Char(char)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cptr = new ::Katie::QLatin1Char(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QLatin1Char >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QLatin1Char_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QLatin1Char_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QLatin1Char");
        return -1;
}

static PyObject* Sbk_Katie_QLatin1CharFunc_toLatin1(PyObject* self)
{
    ::Katie::QLatin1Char* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QLatin1Char *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1CHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toLatin1()const
            char cppResult = const_cast<const ::Katie::QLatin1Char*>(cppSelf)->toLatin1();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<char>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QLatin1CharFunc_unicode(PyObject* self)
{
    ::Katie::QLatin1Char* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QLatin1Char *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1CHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // unicode()const
            ushort cppResult = const_cast<const ::Katie::QLatin1Char*>(cppSelf)->unicode();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QLatin1Char_methods[] = {
    {"toLatin1", reinterpret_cast<PyCFunction>(Sbk_Katie_QLatin1CharFunc_toLatin1), METH_NOARGS},
    {"unicode", reinterpret_cast<PyCFunction>(Sbk_Katie_QLatin1CharFunc_unicode), METH_NOARGS},

    {nullptr, nullptr} // Sentinel
};

} // extern "C"

static int Sbk_Katie_QLatin1Char_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QLatin1Char_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QLatin1Char_Type = nullptr;
static SbkObjectType *Sbk_Katie_QLatin1Char_TypeF(void)
{
    return _Sbk_Katie_QLatin1Char_Type;
}

static PyType_Slot Sbk_Katie_QLatin1Char_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QLatin1Char_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QLatin1Char_clear)},
    {Py_tp_richcompare, nullptr},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QLatin1Char_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QLatin1Char_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QLatin1Char_spec = {
    "KtCore.Katie.QLatin1Char",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QLatin1Char_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QLatin1Char_PythonToCpp_QLatin1Char_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QLatin1Char_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QLatin1Char_PythonToCpp_QLatin1Char_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QLatin1Char_TypeF())))
        return QLatin1Char_PythonToCpp_QLatin1Char_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QLatin1Char_PTR_CppToPython_QLatin1Char(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QLatin1Char *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QLatin1Char_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QLatin1Char_SignatureStrings[] = {
    "KtCore.Katie.QLatin1Char(c:char)",
    "KtCore.Katie.QLatin1Char.toLatin1()->char",
    "KtCore.Katie.QLatin1Char.unicode()->ushort",
    nullptr}; // Sentinel

void init_Katie_QLatin1Char(PyObject* module)
{
    _Sbk_Katie_QLatin1Char_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QLatin1Char",
        "Katie::QLatin1Char*",
        &Sbk_Katie_QLatin1Char_spec,
        Katie_QLatin1Char_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QLatin1Char >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QLATIN1CHAR_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QLatin1Char_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QLatin1Char_TypeF(),
        QLatin1Char_PythonToCpp_QLatin1Char_PTR,
        is_QLatin1Char_PythonToCpp_QLatin1Char_PTR_Convertible,
        QLatin1Char_PTR_CppToPython_QLatin1Char);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QLatin1Char");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QLatin1Char*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QLatin1Char&");
    Shiboken::Conversions::registerConverterName(converter, "QLatin1Char");
    Shiboken::Conversions::registerConverterName(converter, "QLatin1Char*");
    Shiboken::Conversions::registerConverterName(converter, "QLatin1Char&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QLatin1Char).name());



}
