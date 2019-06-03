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
#include "katie_qbytearray_wrapper.h"

// inner classes

// Extra includes
#include <qbytearray.h>
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
Sbk_Katie_QByteArray_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QByteArray >()))
        return -1;

    ::Katie::QByteArray* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QByteArray", 0, 2, &(pyArgs[0]), &(pyArgs[1])))
        return -1;


    // Overloaded function decisor
    // 0: QByteArray::QByteArray()
    // 1: QByteArray::QByteArray(Katie::QByteArray)
    // 2: QByteArray::QByteArray(const char*)
    // 3: QByteArray::QByteArray(const char*,int)
    // 4: QByteArray::QByteArray(int,Katie::Qt::Initialization)
    // 5: QByteArray::QByteArray(int,char)
    if (numArgs == 0) {
        overloadId = 0; // QByteArray()
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX])->converter, (pyArgs[1])))) {
            overloadId = 4; // QByteArray(int,Katie::Qt::Initialization)
        } else if (numArgs == 2
            && SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            overloadId = 5; // QByteArray(int,char)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // QByteArray(const char*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // QByteArray(const char*,int)
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        overloadId = 1; // QByteArray(Katie::QByteArray)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArray_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QByteArray()
        {

            if (!PyErr_Occurred()) {
                // QByteArray()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QByteArray();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QByteArray(const Katie::QByteArray & arg__1)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // QByteArray(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QByteArray(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // QByteArray(const char * arg__1)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QByteArray(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QByteArray(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 3: // QByteArray(const char * arg__1, int size)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QByteArray(const char*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QByteArray(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 4: // QByteArray(int size, Katie::Qt::Initialization arg__2)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::Katie::Qt::Initialization cppArg1{Katie::Qt::Uninitialized};
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QByteArray(int,Katie::Qt::Initialization)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QByteArray(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 5: // QByteArray(int size, char c)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            char cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QByteArray(int,char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QByteArray(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QByteArray >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QByteArray_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QByteArray_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray");
        return -1;
}

static PyObject* Sbk_Katie_QByteArrayFunc_append(PyObject* self, PyObject* args)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "append", 1, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::append(char)
    // 1: QByteArray::append(Katie::QByteArray)
    // 2: QByteArray::append(Katie::QString)
    // 3: QByteArray::append(const char*)
    // 4: QByteArray::append(const char*,int)
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // append(const char*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 4; // append(const char*,int)
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        overloadId = 1; // append(Katie::QByteArray)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        overloadId = 2; // append(Katie::QString)
    } else if (numArgs == 1
        && SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        overloadId = 0; // append(char)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_append_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // append(char c)
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // append(char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->append(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 1: // append(const Katie::QByteArray & a)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // append(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->append(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 2: // append(const Katie::QString & s)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // append(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->append(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 3: // append(const char * s)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // append(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->append(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 4: // append(const char * s, int len)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // append(const char*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->append(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_append_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.append");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_at(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::at(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // at(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_at_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // at(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            char cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->at(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<char>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_at_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.at");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_begin(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // begin()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            char * cppResult = cppSelf->begin();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_capacity(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // capacity()const
            int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->capacity();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_chop(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::chop(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // chop(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_chop_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // chop(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->chop(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QByteArrayFunc_chop_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.chop");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_clear(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // clear()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->clear();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QByteArrayFunc_constBegin(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // constBegin()const
            const char * cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->constBegin();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_constData(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // constData()const
            const char * cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->constData();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_constEnd(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // constEnd()const
            const char * cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->constEnd();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_contains(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::contains(char)const
    // 1: QByteArray::contains(Katie::QByteArray)const
    // 2: QByteArray::contains(const char*)const
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 2; // contains(const char*)const
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // contains(Katie::QByteArray)const
    } else if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // contains(char)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_contains_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // contains(char c) const
        {
            char cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // contains(char)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->contains(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // contains(const Katie::QByteArray & a) const
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // contains(Katie::QByteArray)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->contains(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 2: // contains(const char * a) const
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // contains(const char*)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->contains(cppArg0);
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

    Sbk_Katie_QByteArrayFunc_contains_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.contains");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_count(PyObject* self, PyObject* args)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "count", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::count()const
    // 1: QByteArray::count(char)const
    // 2: QByteArray::count(Katie::QByteArray)const
    // 3: QByteArray::count(const char*)const
    if (numArgs == 0) {
        overloadId = 0; // count()const
    } else if (numArgs == 1
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        overloadId = 3; // count(const char*)const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        overloadId = 2; // count(Katie::QByteArray)const
    } else if (numArgs == 1
        && SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        overloadId = 1; // count(char)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_count_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // count() const
        {

            if (!PyErr_Occurred()) {
                // count()const
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->count();
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // count(char c) const
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // count(char)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->count(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // count(const Katie::QByteArray & a) const
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // count(Katie::QByteArray)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->count(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 3: // count(const char * a) const
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // count(const char*)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->count(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_count_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.count");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_data(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // data()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            char * cppResult = cppSelf->data();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_detach(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // detach()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->detach();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QByteArrayFunc_end(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // end()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            char * cppResult = cppSelf->end();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_endsWith(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::endsWith(char)const
    // 1: QByteArray::endsWith(Katie::QByteArray)const
    // 2: QByteArray::endsWith(const char*)const
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 2; // endsWith(const char*)const
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // endsWith(Katie::QByteArray)const
    } else if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // endsWith(char)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_endsWith_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // endsWith(char c) const
        {
            char cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // endsWith(char)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->endsWith(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // endsWith(const Katie::QByteArray & a) const
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // endsWith(Katie::QByteArray)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->endsWith(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 2: // endsWith(const char * c) const
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // endsWith(const char*)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->endsWith(cppArg0);
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

    Sbk_Katie_QByteArrayFunc_endsWith_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.endsWith");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_fill(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.fill(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.fill(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fill", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::fill(char,int)
    if (SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fill(char,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fill(char,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_fill_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.fill(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_fill_TypeError;
            }
        }
        char cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fill(char,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray & cppResult = cppSelf->fill(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_fill_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.fill");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_fromBase64(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QByteArray::fromBase64(Katie::QByteArray)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 0; // fromBase64(Katie::QByteArray)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_fromBase64_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QByteArray cppArg0_local;
        ::Katie::QByteArray* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
            pythonToCpp(pyArg, &cppArg0_local);
        else
            pythonToCpp(pyArg, &cppArg0);


        if (!PyErr_Occurred()) {
            // fromBase64(Katie::QByteArray)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = ::Katie::QByteArray::fromBase64(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_fromBase64_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.fromBase64");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_fromHex(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QByteArray::fromHex(Katie::QByteArray)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 0; // fromHex(Katie::QByteArray)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_fromHex_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QByteArray cppArg0_local;
        ::Katie::QByteArray* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
            pythonToCpp(pyArg, &cppArg0_local);
        else
            pythonToCpp(pyArg, &cppArg0);


        if (!PyErr_Occurred()) {
            // fromHex(Katie::QByteArray)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = ::Katie::QByteArray::fromHex(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_fromHex_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.fromHex");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_fromPercentEncoding(PyObject* self, PyObject* args, PyObject* kwds)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.fromPercentEncoding(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.fromPercentEncoding(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fromPercentEncoding", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QByteArray::fromPercentEncoding(Katie::QByteArray,char)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fromPercentEncoding(Katie::QByteArray,char)
        } else if (SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            overloadId = 0; // fromPercentEncoding(Katie::QByteArray,char)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_fromPercentEncoding_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "percent");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.fromPercentEncoding(): got multiple values for keyword argument 'percent'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_fromPercentEncoding_TypeError;
            }
        }
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return {};
        ::Katie::QByteArray cppArg0_local;
        ::Katie::QByteArray* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
            pythonToCpp[0](pyArgs[0], &cppArg0_local);
        else
            pythonToCpp[0](pyArgs[0], &cppArg0);

        char cppArg1 = '%';
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromPercentEncoding(Katie::QByteArray,char)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = ::Katie::QByteArray::fromPercentEncoding(*cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_fromPercentEncoding_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.fromPercentEncoding");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_fromRawData(PyObject* self, PyObject* args)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "fromRawData", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QByteArray::fromRawData(const char*,int)
    if (numArgs == 2
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 0; // fromRawData(const char*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_fromRawData_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromRawData(const char*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = ::Katie::QByteArray::fromRawData(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_fromRawData_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.fromRawData");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_fromStdString(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QByteArray::fromStdString(std::string)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<std::string>(), (pyArg)))) {
        overloadId = 0; // fromStdString(std::string)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_fromStdString_TypeError;

    // Call function/method
    {
        ::std::string cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // fromStdString(std::string)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = ::Katie::QByteArray::fromStdString(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_fromStdString_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.fromStdString");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_indexOf(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.indexOf(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.indexOf(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:indexOf", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::indexOf(char,int)const
    // 1: QByteArray::indexOf(Katie::QByteArray,int)const
    // 2: QByteArray::indexOf(Katie::QString,int)const
    // 3: QByteArray::indexOf(const char*,int)const
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // indexOf(const char*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // indexOf(const char*,int)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // indexOf(Katie::QByteArray,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 1; // indexOf(Katie::QByteArray,int)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // indexOf(Katie::QString,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 2; // indexOf(Katie::QString,int)const
        }
    } else if (SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // indexOf(char,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // indexOf(char,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_indexOf_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // indexOf(char c, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_indexOf_TypeError;
                }
            }
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // indexOf(char,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->indexOf(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // indexOf(const Katie::QByteArray & a, const int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_indexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // indexOf(Katie::QByteArray,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->indexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // indexOf(const Katie::QString & s, const int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_indexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // indexOf(Katie::QString,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->indexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 3: // indexOf(const char * c, const int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_indexOf_TypeError;
                }
            }
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // indexOf(const char*,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->indexOf(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_indexOf_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.indexOf");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_insert(PyObject* self, PyObject* args)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "insert", 2, 3, &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::insert(int,Katie::QString)
    // 1: QByteArray::insert(int,char)
    // 2: QByteArray::insert(int,Katie::QByteArray)
    // 3: QByteArray::insert(int,const char*)
    // 4: QByteArray::insert(int,const char*,int)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 3; // insert(int,const char*)
            } else if (numArgs == 3
                && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                overloadId = 4; // insert(int,const char*,int)
            }
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[1])))) {
            overloadId = 2; // insert(int,Katie::QByteArray)
        } else if (numArgs == 2
            && SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            overloadId = 1; // insert(int,char)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            overloadId = 0; // insert(int,Katie::QString)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_insert_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // insert(const int i, const Katie::QString & s)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // insert(int,Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->insert(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 1: // insert(int i, char c)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            char cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // insert(int,char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->insert(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 2: // insert(int i, const Katie::QByteArray & a)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QByteArray cppArg1_local;
            ::Katie::QByteArray* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // insert(int,Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->insert(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 3: // insert(int i, const char * s)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // insert(int,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->insert(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 4: // insert(int i, const char * s, int len)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // insert(int,const char*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->insert(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_insert_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.insert");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_isDetached(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isDetached()const
            bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->isDetached();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_isEmpty(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isEmpty()const
            bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->isEmpty();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_isNull(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isNull()const
            bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->isNull();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_isSharedWith(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::isSharedWith(Katie::QByteArray)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 0; // isSharedWith(Katie::QByteArray)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_isSharedWith_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QByteArray cppArg0_local;
        ::Katie::QByteArray* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
            pythonToCpp(pyArg, &cppArg0_local);
        else
            pythonToCpp(pyArg, &cppArg0);


        if (!PyErr_Occurred()) {
            // isSharedWith(Katie::QByteArray)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->isSharedWith(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_isSharedWith_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.isSharedWith");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_lastIndexOf(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.lastIndexOf(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.lastIndexOf(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:lastIndexOf", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::lastIndexOf(char,int)const
    // 1: QByteArray::lastIndexOf(Katie::QByteArray,int)const
    // 2: QByteArray::lastIndexOf(Katie::QString,int)const
    // 3: QByteArray::lastIndexOf(const char*,int)const
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // lastIndexOf(const char*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // lastIndexOf(const char*,int)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // lastIndexOf(Katie::QByteArray,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 1; // lastIndexOf(Katie::QByteArray,int)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // lastIndexOf(Katie::QString,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 2; // lastIndexOf(Katie::QString,int)const
        }
    } else if (SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // lastIndexOf(char,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // lastIndexOf(char,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_lastIndexOf_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // lastIndexOf(char c, const int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_lastIndexOf_TypeError;
                }
            }
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // lastIndexOf(char,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->lastIndexOf(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // lastIndexOf(const Katie::QByteArray & a, const int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_lastIndexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // lastIndexOf(Katie::QByteArray,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // lastIndexOf(const Katie::QString & s, const int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_lastIndexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // lastIndexOf(Katie::QString,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 3: // lastIndexOf(const char * c, const int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_lastIndexOf_TypeError;
                }
            }
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // lastIndexOf(const char*,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->lastIndexOf(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_lastIndexOf_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.lastIndexOf");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_left(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::left(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // left(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_left_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // left(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->left(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_left_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.left");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_leftJustified(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 3) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.leftJustified(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.leftJustified(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:leftJustified", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::leftJustified(int,char,bool)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // leftJustified(int,char,bool)const
        } else if (SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // leftJustified(int,char,bool)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2])))) {
                overloadId = 0; // leftJustified(int,char,bool)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_leftJustified_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "fill");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.leftJustified(): got multiple values for keyword argument 'fill'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_leftJustified_TypeError;
            }
            value = PyDict_GetItemString(kwds, "truncate");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.leftJustified(): got multiple values for keyword argument 'truncate'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2]))))
                    goto Sbk_Katie_QByteArrayFunc_leftJustified_TypeError;
            }
        }
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        char cppArg1 = ' ';
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
        bool cppArg2 = false;
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // leftJustified(int,char,bool)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->leftJustified(cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_leftJustified_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.leftJustified");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_length(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // length()const
            int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->length();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_mid(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.mid(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.mid(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:mid", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::mid(int,int)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // mid(int,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // mid(int,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_mid_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "len");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.mid(): got multiple values for keyword argument 'len'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_mid_TypeError;
            }
        }
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // mid(int,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->mid(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_mid_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.mid");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_number(PyObject* self, PyObject* args, PyObject* kwds)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 3) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:number", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: static QByteArray::number(double,char,int)
    // 1: static QByteArray::number(int,int)
    // 2: static QByteArray::number(long long,int)
    // 3: static QByteArray::number(uint,int)
    // 4: static QByteArray::number(unsigned long long,int)
    if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 4; // number(unsigned long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 4; // number(unsigned long long,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // number(uint,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // number(uint,int)
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // number(long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 2; // number(long long,int)
        }
    } else if (PyFloat_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<double>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // number(double,char,int)
        } else if (SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // number(double,char,int)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                overloadId = 0; // number(double,char,int)
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // number(int,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 1; // number(int,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_number_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // number(double arg__1, char f, int prec)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "f");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): got multiple values for keyword argument 'f'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_number_TypeError;
                }
                value = PyDict_GetItemString(kwds, "prec");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): got multiple values for keyword argument 'prec'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QByteArrayFunc_number_TypeError;
                }
            }
            double cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            char cppArg1 = 'g';
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 6;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // number(double,char,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray cppResult = ::Katie::QByteArray::number(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 1: // number(int arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_number_TypeError;
                }
            }
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(int,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray cppResult = ::Katie::QByteArray::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 2: // number(long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_number_TypeError;
                }
            }
            long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray cppResult = ::Katie::QByteArray::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 3: // number(uint arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_number_TypeError;
                }
            }
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(uint,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray cppResult = ::Katie::QByteArray::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 4: // number(unsigned long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_number_TypeError;
                }
            }
            unsigned long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(unsigned long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray cppResult = ::Katie::QByteArray::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_number_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.number");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_prepend(PyObject* self, PyObject* args)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "prepend", 1, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::prepend(char)
    // 1: QByteArray::prepend(Katie::QByteArray)
    // 2: QByteArray::prepend(const char*)
    // 3: QByteArray::prepend(const char*,int)
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // prepend(const char*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // prepend(const char*,int)
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        overloadId = 1; // prepend(Katie::QByteArray)
    } else if (numArgs == 1
        && SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        overloadId = 0; // prepend(char)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_prepend_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // prepend(char c)
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // prepend(char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->prepend(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 1: // prepend(const Katie::QByteArray & a)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // prepend(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->prepend(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 2: // prepend(const char * s)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // prepend(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->prepend(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 3: // prepend(const char * s, int len)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // prepend(const char*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->prepend(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_prepend_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.prepend");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_push_back(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::push_back(char)
    // 1: QByteArray::push_back(Katie::QByteArray)
    // 2: QByteArray::push_back(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 2; // push_back(const char*)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // push_back(Katie::QByteArray)
    } else if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // push_back(char)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_push_back_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // push_back(char c)
        {
            char cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // push_back(char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_back(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // push_back(const Katie::QByteArray & a)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // push_back(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_back(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // push_back(const char * c)
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // push_back(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_back(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QByteArrayFunc_push_back_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.push_back");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_push_front(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::push_front(char)
    // 1: QByteArray::push_front(Katie::QByteArray)
    // 2: QByteArray::push_front(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 2; // push_front(const char*)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // push_front(Katie::QByteArray)
    } else if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // push_front(char)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_push_front_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // push_front(char c)
        {
            char cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // push_front(char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_front(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // push_front(const Katie::QByteArray & a)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // push_front(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_front(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // push_front(const char * c)
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // push_front(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_front(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QByteArrayFunc_push_front_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.push_front");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_remove(PyObject* self, PyObject* args)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "remove", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::remove(int,int)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 0; // remove(int,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_remove_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // remove(int,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray & cppResult = cppSelf->remove(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_remove_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.remove");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_repeated(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::repeated(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // repeated(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_repeated_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // repeated(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->repeated(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_repeated_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.repeated");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_replace(PyObject* self, PyObject* args)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "replace", 2, 4, &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2]), &(pyArgs[3])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::replace(char,char)
    // 1: QByteArray::replace(char,Katie::QByteArray)
    // 2: QByteArray::replace(char,const char*)
    // 3: QByteArray::replace(char,Katie::QString)
    // 4: QByteArray::replace(Katie::QByteArray,Katie::QByteArray)
    // 5: QByteArray::replace(Katie::QByteArray,const char*)
    // 6: QByteArray::replace(Katie::QString,Katie::QByteArray)
    // 7: QByteArray::replace(Katie::QString,const char*)
    // 8: QByteArray::replace(const char*,Katie::QByteArray)
    // 9: QByteArray::replace(const char*,const char*)
    // 10: QByteArray::replace(const char*,int,const char*,int)
    // 11: QByteArray::replace(int,int,Katie::QByteArray)
    // 12: QByteArray::replace(int,int,const char*)
    // 13: QByteArray::replace(int,int,const char*,int)
    if (numArgs >= 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        if (Shiboken::String::check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[2])))) {
            if (numArgs == 3) {
                overloadId = 12; // replace(int,int,const char*)
            } else if (numArgs == 4
                && (pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[3])))) {
                overloadId = 13; // replace(int,int,const char*,int)
            }
        } else if (numArgs == 3
            && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[2])))) {
            overloadId = 11; // replace(int,int,Katie::QByteArray)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 4
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))
            && Shiboken::String::check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[2])))
            && (pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[3])))) {
            overloadId = 10; // replace(const char*,int,const char*,int)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
            overloadId = 9; // replace(const char*,const char*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[1])))) {
            overloadId = 8; // replace(const char*,Katie::QByteArray)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        if (numArgs == 2
            && Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
            overloadId = 5; // replace(Katie::QByteArray,const char*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[1])))) {
            overloadId = 4; // replace(Katie::QByteArray,Katie::QByteArray)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 2
            && Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
            overloadId = 7; // replace(Katie::QString,const char*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[1])))) {
            overloadId = 6; // replace(Katie::QString,Katie::QByteArray)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        if (numArgs == 2
            && Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
            overloadId = 2; // replace(char,const char*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[1])))) {
            overloadId = 1; // replace(char,Katie::QByteArray)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            overloadId = 3; // replace(char,Katie::QString)
        } else if (numArgs == 2
            && SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            overloadId = 0; // replace(char,char)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_replace_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // replace(char before, char after)
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            char cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // replace(char,char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 1: // replace(char before, const Katie::QByteArray & after)
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QByteArray cppArg1_local;
            ::Katie::QByteArray* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // replace(char,Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 2: // replace(char before, const char * after)
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // replace(char,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 3: // replace(char c, const Katie::QString & after)
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // replace(char,Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 4: // replace(const Katie::QByteArray & before, const Katie::QByteArray & after)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QByteArray cppArg1_local;
            ::Katie::QByteArray* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // replace(Katie::QByteArray,Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(*cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 5: // replace(const Katie::QByteArray & before, const char * after)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // replace(Katie::QByteArray,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 6: // replace(const Katie::QString & before, const Katie::QByteArray & after)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QByteArray cppArg1_local;
            ::Katie::QByteArray* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // replace(Katie::QString,Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(*cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 7: // replace(const Katie::QString & before, const char * after)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // replace(Katie::QString,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 8: // replace(const char * before, const Katie::QByteArray & after)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QByteArray cppArg1_local;
            ::Katie::QByteArray* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // replace(const char*,Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 9: // replace(const char * before, const char * after)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            const char* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // replace(const char*,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 10: // replace(const char * before, int bsize, const char * after, int asize)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            const char* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);
            int cppArg3;
            pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!PyErr_Occurred()) {
                // replace(const char*,int,const char*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, cppArg1, cppArg2, cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 11: // replace(int index, int len, const Katie::QByteArray & s)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QByteArray cppArg2_local;
            ::Katie::QByteArray* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);


            if (!PyErr_Occurred()) {
                // replace(int,int,Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, cppArg1, *cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 12: // replace(int index, int len, const char * s)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            const char* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // replace(int,int,const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 13: // replace(int index, int len, const char * s, int alen)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            const char* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);
            int cppArg3;
            pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!PyErr_Occurred()) {
                // replace(int,int,const char*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->replace(cppArg0, cppArg1, cppArg2, cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_replace_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.replace");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_reserve(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::reserve(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // reserve(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_reserve_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // reserve(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->reserve(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QByteArrayFunc_reserve_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.reserve");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_resize(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::resize(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // resize(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_resize_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // resize(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->resize(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QByteArrayFunc_resize_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.resize");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_right(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::right(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // right(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_right_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // right(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->right(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_right_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.right");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_rightJustified(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 3) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.rightJustified(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.rightJustified(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:rightJustified", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::rightJustified(int,char,bool)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // rightJustified(int,char,bool)const
        } else if (SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // rightJustified(int,char,bool)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2])))) {
                overloadId = 0; // rightJustified(int,char,bool)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_rightJustified_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "fill");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.rightJustified(): got multiple values for keyword argument 'fill'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_rightJustified_TypeError;
            }
            value = PyDict_GetItemString(kwds, "truncate");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.rightJustified(): got multiple values for keyword argument 'truncate'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2]))))
                    goto Sbk_Katie_QByteArrayFunc_rightJustified_TypeError;
            }
        }
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        char cppArg1 = ' ';
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
        bool cppArg2 = false;
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // rightJustified(int,char,bool)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->rightJustified(cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_rightJustified_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.rightJustified");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_setNum(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 3) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:setNum", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::setNum(double,char,int)
    // 1: QByteArray::setNum(float,char,int)
    // 2: QByteArray::setNum(int,int)
    // 3: QByteArray::setNum(long long,int)
    // 4: QByteArray::setNum(short,int)
    // 5: QByteArray::setNum(uint,int)
    // 6: QByteArray::setNum(unsigned long long,int)
    // 7: QByteArray::setNum(ushort,int)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 7; // setNum(ushort,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 7; // setNum(ushort,int)
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 6; // setNum(unsigned long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 6; // setNum(unsigned long long,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 5; // setNum(uint,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 5; // setNum(uint,int)
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // setNum(long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // setNum(long long,int)
        }
    } else if (PyFloat_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<float>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // setNum(float,char,int)
        } else if (SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 1; // setNum(float,char,int)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                overloadId = 1; // setNum(float,char,int)
            }
        }
    } else if (PyFloat_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<double>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // setNum(double,char,int)
        } else if (SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // setNum(double,char,int)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                overloadId = 0; // setNum(double,char,int)
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<short>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 4; // setNum(short,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 4; // setNum(short,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // setNum(int,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 2; // setNum(int,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // setNum(double arg__1, char f, int prec)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "f");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'f'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
                value = PyDict_GetItemString(kwds, "prec");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'prec'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            double cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            char cppArg1 = 'g';
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 6;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // setNum(double,char,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 1: // setNum(float arg__1, char f, int prec)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "f");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'f'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
                value = PyDict_GetItemString(kwds, "prec");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'prec'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            float cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            char cppArg1 = 'g';
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 6;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // setNum(float,char,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 2: // setNum(int arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(int,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 3: // setNum(long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 4: // setNum(short arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            short cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(short,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 5: // setNum(uint arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(uint,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 6: // setNum(unsigned long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            unsigned long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(unsigned long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
        case 7: // setNum(ushort arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QByteArrayFunc_setNum_TypeError;
                }
            }
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(ushort,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QByteArray & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_setNum_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.setNum");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_setRawData(PyObject* self, PyObject* args)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "setRawData", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::setRawData(const char*,uint)
    if (numArgs == 2
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[1])))) {
        overloadId = 0; // setRawData(const char*,uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_setRawData_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        uint cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // setRawData(const char*,uint)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray & cppResult = cppSelf->setRawData(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_setRawData_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.setRawData");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_simplified(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // simplified()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->simplified();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_size(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // size()const
            int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->size();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_squeeze(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // squeeze()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->squeeze();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QByteArrayFunc_startsWith(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::startsWith(char)const
    // 1: QByteArray::startsWith(Katie::QByteArray)const
    // 2: QByteArray::startsWith(const char*)const
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 2; // startsWith(const char*)const
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // startsWith(Katie::QByteArray)const
    } else if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // startsWith(char)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_startsWith_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // startsWith(char c) const
        {
            char cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // startsWith(char)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->startsWith(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // startsWith(const Katie::QByteArray & a) const
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // startsWith(Katie::QByteArray)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->startsWith(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 2: // startsWith(const char * c) const
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // startsWith(const char*)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->startsWith(cppArg0);
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

    Sbk_Katie_QByteArrayFunc_startsWith_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.startsWith");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_swap(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::swap(Katie::QByteArray&)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 0; // swap(Katie::QByteArray&)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_swap_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QByteArray cppArg0_local;
        ::Katie::QByteArray* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
            pythonToCpp(pyArg, &cppArg0_local);
        else
            pythonToCpp(pyArg, &cppArg0);


        if (!PyErr_Occurred()) {
            // swap(Katie::QByteArray&)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->swap(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QByteArrayFunc_swap_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.swap");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toBase64(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toBase64()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toBase64();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_toDouble(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toDouble(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toDouble", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toDouble(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toDouble(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toDouble(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toDouble_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toDouble(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toDouble_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toDouble(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            double cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toDouble(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<double>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toDouble_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toDouble");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toFloat(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toFloat(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toFloat", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toFloat(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toFloat(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toFloat(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toFloat_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toFloat(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toFloat_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toFloat(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            float cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toFloat(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<float>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toFloat_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toFloat");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toHex(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toHex()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toHex();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_toInt(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toInt(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toInt", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toInt(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toInt(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toInt(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toInt(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toInt_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toInt(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toInt_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toInt(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toInt_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toInt(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toInt(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toInt_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toInt");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toLong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toLong(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toLong(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toLong(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toLong(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toLong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toLong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toLong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            long cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toLong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toLong");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toLongLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toLongLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toLongLong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toLongLong(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toLongLong(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toLongLong(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toLongLong(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toLongLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toLongLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toLongLong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toLongLong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toLongLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toLongLong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            long long cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toLongLong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toLongLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toLongLong");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toLower(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toLower()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toLower();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_toPercentEncoding(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 3) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toPercentEncoding(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:toPercentEncoding", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toPercentEncoding(Katie::QByteArray,Katie::QByteArray,char)const
    if (numArgs == 0) {
        overloadId = 0; // toPercentEncoding(Katie::QByteArray,Katie::QByteArray,char)const
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toPercentEncoding(Katie::QByteArray,Katie::QByteArray,char)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // toPercentEncoding(Katie::QByteArray,Katie::QByteArray,char)const
            } else if (SbkChar_Check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[2])))) {
                overloadId = 0; // toPercentEncoding(Katie::QByteArray,Katie::QByteArray,char)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toPercentEncoding_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "exclude");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toPercentEncoding(): got multiple values for keyword argument 'exclude'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toPercentEncoding_TypeError;
            }
            value = PyDict_GetItemString(kwds, "include");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toPercentEncoding(): got multiple values for keyword argument 'include'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toPercentEncoding_TypeError;
            }
            value = PyDict_GetItemString(kwds, "percent");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toPercentEncoding(): got multiple values for keyword argument 'percent'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!SbkChar_Check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[2]))))
                    goto Sbk_Katie_QByteArrayFunc_toPercentEncoding_TypeError;
            }
        }
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return {};
        ::Katie::QByteArray cppArg0_local = Katie::QByteArray();
        ::Katie::QByteArray* cppArg0 = &cppArg0_local;
        if (pythonToCpp[0]) {
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[0]))
            pythonToCpp[0](pyArgs[0], &cppArg0_local);
        else
            pythonToCpp[0](pyArgs[0], &cppArg0);
        }
        if (!Shiboken::Object::isValid(pyArgs[1]))
            return {};
        ::Katie::QByteArray cppArg1_local = Katie::QByteArray();
        ::Katie::QByteArray* cppArg1 = &cppArg1_local;
        if (pythonToCpp[1]) {
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp[1]))
            pythonToCpp[1](pyArgs[1], &cppArg1_local);
        else
            pythonToCpp[1](pyArgs[1], &cppArg1);
        }
        char cppArg2 = '%';
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // toPercentEncoding(Katie::QByteArray,Katie::QByteArray,char)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toPercentEncoding(*cppArg0, *cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toPercentEncoding_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toPercentEncoding");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toShort(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toShort(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toShort", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toShort(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toShort(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toShort(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toShort(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toShort_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toShort(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toShort_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toShort(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toShort_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toShort(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            short cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toShort(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<short>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toShort_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toShort");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toStdString(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toStdString()const
            std::string cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toStdString();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<std::string>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_toUInt(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toUInt(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toUInt", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toUInt(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toUInt(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toUInt(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toUInt(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toUInt_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toUInt(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toUInt_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toUInt(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toUInt_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toUInt(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            uint cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toUInt(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toUInt_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toUInt");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toULong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toULong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toULong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toULong(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toULong(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toULong(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toULong(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toULong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toULong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toULong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toULong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toULong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toULong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ulong cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toULong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ulong>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toULong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toULong");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toULongLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toULongLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toULongLong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toULongLong(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toULongLong(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toULongLong(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toULongLong(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toULongLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toULongLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toULongLong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toULongLong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toULongLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toULongLong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            unsigned long long cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toULongLong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toULongLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toULongLong");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toUShort(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toUShort(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toUShort", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QByteArray::toUShort(bool*,int)const
    if (numArgs == 0) {
        overloadId = 0; // toUShort(bool*,int)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // toUShort(bool*,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // toUShort(bool*,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_toUShort_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toUShort(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QByteArrayFunc_toUShort_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QByteArray.toUShort(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QByteArrayFunc_toUShort_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toUShort(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ushort cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toUShort(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QByteArrayFunc_toUShort_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QByteArray.toUShort");
        return {};
}

static PyObject* Sbk_Katie_QByteArrayFunc_toUpper(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toUpper()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->toUpper();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_trimmed(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // trimmed()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QByteArray*>(cppSelf)->trimmed();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QByteArrayFunc_truncate(PyObject* self, PyObject* pyArg)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::truncate(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // truncate(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc_truncate_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // truncate(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->truncate(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QByteArrayFunc_truncate_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.truncate");
        return {};
}

static PyObject* Sbk_Katie_QByteArray___copy__(PyObject* self)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QByteArray& cppSelf = *reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppSelf);
    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QByteArray_methods[] = {
    {"append", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_append), METH_VARARGS},
    {"at", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_at), METH_O},
    {"begin", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_begin), METH_NOARGS},
    {"capacity", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_capacity), METH_NOARGS},
    {"chop", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_chop), METH_O},
    {"clear", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_clear), METH_NOARGS},
    {"constBegin", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_constBegin), METH_NOARGS},
    {"constData", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_constData), METH_NOARGS},
    {"constEnd", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_constEnd), METH_NOARGS},
    {"contains", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_contains), METH_O},
    {"count", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_count), METH_VARARGS},
    {"data", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_data), METH_NOARGS},
    {"detach", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_detach), METH_NOARGS},
    {"end", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_end), METH_NOARGS},
    {"endsWith", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_endsWith), METH_O},
    {"fill", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_fill), METH_VARARGS|METH_KEYWORDS},
    {"fromBase64", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_fromBase64), METH_O|METH_STATIC},
    {"fromHex", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_fromHex), METH_O|METH_STATIC},
    {"fromPercentEncoding", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_fromPercentEncoding), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"fromRawData", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_fromRawData), METH_VARARGS|METH_STATIC},
    {"fromStdString", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_fromStdString), METH_O|METH_STATIC},
    {"indexOf", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_indexOf), METH_VARARGS|METH_KEYWORDS},
    {"insert", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_insert), METH_VARARGS},
    {"isDetached", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_isDetached), METH_NOARGS},
    {"isEmpty", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_isEmpty), METH_NOARGS},
    {"isNull", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_isNull), METH_NOARGS},
    {"isSharedWith", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_isSharedWith), METH_O},
    {"lastIndexOf", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_lastIndexOf), METH_VARARGS|METH_KEYWORDS},
    {"left", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_left), METH_O},
    {"leftJustified", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_leftJustified), METH_VARARGS|METH_KEYWORDS},
    {"length", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_length), METH_NOARGS},
    {"mid", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_mid), METH_VARARGS|METH_KEYWORDS},
    {"number", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_number), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"prepend", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_prepend), METH_VARARGS},
    {"push_back", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_push_back), METH_O},
    {"push_front", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_push_front), METH_O},
    {"remove", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_remove), METH_VARARGS},
    {"repeated", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_repeated), METH_O},
    {"replace", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_replace), METH_VARARGS},
    {"reserve", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_reserve), METH_O},
    {"resize", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_resize), METH_O},
    {"right", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_right), METH_O},
    {"rightJustified", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_rightJustified), METH_VARARGS|METH_KEYWORDS},
    {"setNum", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_setNum), METH_VARARGS|METH_KEYWORDS},
    {"setRawData", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_setRawData), METH_VARARGS},
    {"simplified", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_simplified), METH_NOARGS},
    {"size", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_size), METH_NOARGS},
    {"squeeze", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_squeeze), METH_NOARGS},
    {"startsWith", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_startsWith), METH_O},
    {"swap", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_swap), METH_O},
    {"toBase64", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toBase64), METH_NOARGS},
    {"toDouble", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toDouble), METH_VARARGS|METH_KEYWORDS},
    {"toFloat", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toFloat), METH_VARARGS|METH_KEYWORDS},
    {"toHex", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toHex), METH_NOARGS},
    {"toInt", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toInt), METH_VARARGS|METH_KEYWORDS},
    {"toLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toLong), METH_VARARGS|METH_KEYWORDS},
    {"toLongLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toLongLong), METH_VARARGS|METH_KEYWORDS},
    {"toLower", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toLower), METH_NOARGS},
    {"toPercentEncoding", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toPercentEncoding), METH_VARARGS|METH_KEYWORDS},
    {"toShort", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toShort), METH_VARARGS|METH_KEYWORDS},
    {"toStdString", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toStdString), METH_NOARGS},
    {"toUInt", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toUInt), METH_VARARGS|METH_KEYWORDS},
    {"toULong", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toULong), METH_VARARGS|METH_KEYWORDS},
    {"toULongLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toULongLong), METH_VARARGS|METH_KEYWORDS},
    {"toUShort", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toUShort), METH_VARARGS|METH_KEYWORDS},
    {"toUpper", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_toUpper), METH_NOARGS},
    {"trimmed", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_trimmed), METH_NOARGS},
    {"truncate", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArrayFunc_truncate), METH_O},

    {"__copy__", reinterpret_cast<PyCFunction>(Sbk_Katie_QByteArray___copy__), METH_NOARGS},
    {nullptr, nullptr} // Sentinel
};

static int Sbk_Katie_QByteArray___nb_bool(PyObject* self)
{
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return -1;
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    return !cppSelf->isNull();
}

static PyObject* Sbk_Katie_QByteArrayFunc___iadd__(PyObject* self, PyObject* pyArg)
{
    bool isReverse = SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], pyArg)
                    && !SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], self);
    if (isReverse)
        std::swap(self, pyArg);
    ::Katie::QByteArray* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QByteArray::operator+=(char)
    // 1: QByteArray::operator+=(Katie::QByteArray)
    // 2: QByteArray::operator+=(Katie::QString)
    // 3: QByteArray::operator+=(const char*)
    if (!isReverse
        && Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 3; // operator+=(const char*)
    } else if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // operator+=(Katie::QByteArray)
    } else if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 2; // operator+=(Katie::QString)
    } else if (!isReverse
        && SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // operator+=(char)
    }

    if (isReverse && overloadId == -1) {
        PyErr_SetString(PyExc_NotImplementedError, "reverse operator not implemented.");
        return {};
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QByteArrayFunc___iadd___TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // operator+=(char c)
        {
            char cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // operator+=(char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                (*cppSelf) += cppArg0;
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // operator+=(const Katie::QByteArray & a)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QByteArray cppArg0_local;
            ::Katie::QByteArray* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // operator+=(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                (*cppSelf) += (*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // operator+=(const Katie::QString & s)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // operator+=(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                (*cppSelf) += (*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 3: // operator+=(const char * s)
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // operator+=(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                (*cppSelf) += cppArg0;
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_INCREF(self);
    return self;

    Sbk_Katie_QByteArrayFunc___iadd___TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QByteArray.__iadd__");
        return {};
}

// Rich comparison
static PyObject* Sbk_Katie_QByteArray_richcompare(PyObject* self, PyObject* pyArg, int op)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QByteArray& cppSelf = *reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(self)));
    SBK_UNUSED(cppSelf)
    PyObject* pyResult{};
    PythonToCppFunc pythonToCpp;
    SBK_UNUSED(pythonToCpp)

    switch (op) {
        case Py_NE:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator!=(const Katie::QString & s2) const
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
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator<(const Katie::QString & s2) const
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
                goto Sbk_Katie_QByteArray_RichComparison_TypeError;
            }

            break;
        case Py_LE:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator<=(const Katie::QString & s2) const
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
                goto Sbk_Katie_QByteArray_RichComparison_TypeError;
            }

            break;
        case Py_EQ:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator==(const Katie::QString & s2) const
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
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator>(const Katie::QString & s2) const
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
                goto Sbk_Katie_QByteArray_RichComparison_TypeError;
            }

            break;
        case Py_GE:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
                // operator>=(const Katie::QString & s2) const
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
                goto Sbk_Katie_QByteArray_RichComparison_TypeError;
            }

            break;
        default:
            goto Sbk_Katie_QByteArray_RichComparison_TypeError;
    }

    if (pyResult && !PyErr_Occurred())
        return pyResult;
    Sbk_Katie_QByteArray_RichComparison_TypeError:
    PyErr_SetString(PyExc_NotImplementedError, "operator not implemented.");
    return {};

}

} // extern "C"

static int Sbk_Katie_QByteArray_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QByteArray_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QByteArray_Type = nullptr;
static SbkObjectType *Sbk_Katie_QByteArray_TypeF(void)
{
    return _Sbk_Katie_QByteArray_Type;
}

static PyType_Slot Sbk_Katie_QByteArray_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QByteArray_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QByteArray_clear)},
    {Py_tp_richcompare, reinterpret_cast<void*>(Sbk_Katie_QByteArray_richcompare)},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QByteArray_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QByteArray_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    // type supports number protocol
    {Py_nb_inplace_add, (void *)Sbk_Katie_QByteArrayFunc___iadd__},
#ifdef IS_PY3K
    {Py_nb_bool, (void *)Sbk_Katie_QByteArray___nb_bool},
#else
    {Py_nb_nonzero, (void *)Sbk_Katie_QByteArray___nb_bool},
#endif
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QByteArray_spec = {
    "KtCore.Katie.QByteArray",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QByteArray_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QByteArray_PythonToCpp_QByteArray_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QByteArray_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QByteArray_PythonToCpp_QByteArray_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QByteArray_TypeF())))
        return QByteArray_PythonToCpp_QByteArray_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QByteArray_PTR_CppToPython_QByteArray(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QByteArray *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QByteArray_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// C++ to Python copy conversion.
static PyObject* QByteArray_COPY_CppToPython_QByteArray(const void* cppIn) {
    return Shiboken::Object::newObject(Sbk_Katie_QByteArray_TypeF(), new ::Katie::QByteArray(*reinterpret_cast<const ::Katie::QByteArray*>(cppIn)), true, true);
}

// Python to C++ copy conversion.
static void QByteArray_PythonToCpp_QByteArray_COPY(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QByteArray*>(cppOut) = *reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(pyIn)));
}
static PythonToCppFunc is_QByteArray_PythonToCpp_QByteArray_COPY_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QByteArray_TypeF())))
        return QByteArray_PythonToCpp_QByteArray_COPY;
    return {};
}

// Implicit conversions.
static void constcharPTR_PythonToCpp_Katie_QByteArray(PyObject* pyIn, void* cppOut) {
    const char* cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QByteArray*>(cppOut) = ::Katie::QByteArray(cppIn);
}
static PythonToCppFunc is_constcharPTR_PythonToCpp_Katie_QByteArray_Convertible(PyObject* pyIn) {
    if (Shiboken::String::check(pyIn))
        return constcharPTR_PythonToCpp_Katie_QByteArray;
    return {};
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QByteArray_SignatureStrings[] = {
    "5:KtCore.Katie.QByteArray()",
    "4:KtCore.Katie.QByteArray(arg__1:KtCore.Katie.QByteArray)",
    "3:KtCore.Katie.QByteArray(arg__1:str)",
    "2:KtCore.Katie.QByteArray(arg__1:str,size:int)",
    "1:KtCore.Katie.QByteArray(size:int,arg__2:KtCore.Katie.Qt.Initialization)",
    "0:KtCore.Katie.QByteArray(size:int,c:char)",
    "4:KtCore.Katie.QByteArray.append(c:char)->KtCore.Katie.QByteArray",
    "3:KtCore.Katie.QByteArray.append(a:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.append(s:KtCore.Katie.QString)->KtCore.Katie.QByteArray",
    "1:KtCore.Katie.QByteArray.append(s:str)->KtCore.Katie.QByteArray",
    "0:KtCore.Katie.QByteArray.append(s:str,len:int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.at(i:int)->char",
    "1:KtCore.Katie.QByteArray.begin()->str",
    "0:KtCore.Katie.QByteArray.begin()->str",
    "KtCore.Katie.QByteArray.capacity()->int",
    "KtCore.Katie.QByteArray.chop(n:int)",
    "KtCore.Katie.QByteArray.clear()",
    "KtCore.Katie.QByteArray.constBegin()->str",
    "KtCore.Katie.QByteArray.constData()->str",
    "KtCore.Katie.QByteArray.constEnd()->str",
    "2:KtCore.Katie.QByteArray.contains(c:char)->bool",
    "1:KtCore.Katie.QByteArray.contains(a:KtCore.Katie.QByteArray)->bool",
    "0:KtCore.Katie.QByteArray.contains(a:str)->bool",
    "3:KtCore.Katie.QByteArray.count()->int",
    "2:KtCore.Katie.QByteArray.count(c:char)->int",
    "1:KtCore.Katie.QByteArray.count(a:KtCore.Katie.QByteArray)->int",
    "0:KtCore.Katie.QByteArray.count(a:str)->int",
    "1:KtCore.Katie.QByteArray.data()->str",
    "0:KtCore.Katie.QByteArray.data()->str",
    "KtCore.Katie.QByteArray.detach()",
    "1:KtCore.Katie.QByteArray.end()->str",
    "0:KtCore.Katie.QByteArray.end()->str",
    "2:KtCore.Katie.QByteArray.endsWith(c:char)->bool",
    "1:KtCore.Katie.QByteArray.endsWith(a:KtCore.Katie.QByteArray)->bool",
    "0:KtCore.Katie.QByteArray.endsWith(c:str)->bool",
    "KtCore.Katie.QByteArray.fill(c:char,size:int=-1)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.fromBase64(base64:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.fromHex(hexEncoded:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.fromPercentEncoding(pctEncoded:KtCore.Katie.QByteArray,percent:char='%')->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.fromRawData(arg__1:str,size:int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.fromStdString(s:std.string)->KtCore.Katie.QByteArray",
    "3:KtCore.Katie.QByteArray.indexOf(c:char,from:int=0)->int",
    "2:KtCore.Katie.QByteArray.indexOf(a:KtCore.Katie.QByteArray,from:int=0)->int",
    "1:KtCore.Katie.QByteArray.indexOf(s:KtCore.Katie.QString,from:int=0)->int",
    "0:KtCore.Katie.QByteArray.indexOf(c:str,from:int=0)->int",
    "4:KtCore.Katie.QByteArray.insert(i:int,s:KtCore.Katie.QString)->KtCore.Katie.QByteArray",
    "3:KtCore.Katie.QByteArray.insert(i:int,c:char)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.insert(i:int,a:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "1:KtCore.Katie.QByteArray.insert(i:int,s:str)->KtCore.Katie.QByteArray",
    "0:KtCore.Katie.QByteArray.insert(i:int,s:str,len:int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.isDetached()->bool",
    "KtCore.Katie.QByteArray.isEmpty()->bool",
    "KtCore.Katie.QByteArray.isNull()->bool",
    "KtCore.Katie.QByteArray.isSharedWith(other:KtCore.Katie.QByteArray)->bool",
    "3:KtCore.Katie.QByteArray.lastIndexOf(c:char,from:int=-1)->int",
    "2:KtCore.Katie.QByteArray.lastIndexOf(a:KtCore.Katie.QByteArray,from:int=-1)->int",
    "1:KtCore.Katie.QByteArray.lastIndexOf(s:KtCore.Katie.QString,from:int=-1)->int",
    "0:KtCore.Katie.QByteArray.lastIndexOf(c:str,from:int=-1)->int",
    "KtCore.Katie.QByteArray.left(len:int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.leftJustified(width:int,fill:char=' ',truncate:bool=false)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.length()->int",
    "KtCore.Katie.QByteArray.mid(index:int,len:int=-1)->KtCore.Katie.QByteArray",
    "4:KtCore.Katie.QByteArray.number(arg__1:double,f:char='g',prec:int=6)->KtCore.Katie.QByteArray",
    "3:KtCore.Katie.QByteArray.number(arg__1:int,base:int=10)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.number(arg__1:long long,base:int=10)->KtCore.Katie.QByteArray",
    "1:KtCore.Katie.QByteArray.number(arg__1:unsigned int,base:int=10)->KtCore.Katie.QByteArray",
    "0:KtCore.Katie.QByteArray.number(arg__1:unsigned long long,base:int=10)->KtCore.Katie.QByteArray",
    "3:KtCore.Katie.QByteArray.prepend(c:char)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.prepend(a:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "1:KtCore.Katie.QByteArray.prepend(s:str)->KtCore.Katie.QByteArray",
    "0:KtCore.Katie.QByteArray.prepend(s:str,len:int)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.push_back(c:char)",
    "1:KtCore.Katie.QByteArray.push_back(a:KtCore.Katie.QByteArray)",
    "0:KtCore.Katie.QByteArray.push_back(c:str)",
    "2:KtCore.Katie.QByteArray.push_front(c:char)",
    "1:KtCore.Katie.QByteArray.push_front(a:KtCore.Katie.QByteArray)",
    "0:KtCore.Katie.QByteArray.push_front(c:str)",
    "KtCore.Katie.QByteArray.remove(index:int,len:int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.repeated(times:int)->KtCore.Katie.QByteArray",
    "13:KtCore.Katie.QByteArray.replace(before:char,after:char)->KtCore.Katie.QByteArray",
    "12:KtCore.Katie.QByteArray.replace(before:char,after:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "11:KtCore.Katie.QByteArray.replace(before:char,after:str)->KtCore.Katie.QByteArray",
    "10:KtCore.Katie.QByteArray.replace(c:char,after:KtCore.Katie.QString)->KtCore.Katie.QByteArray",
    "9:KtCore.Katie.QByteArray.replace(before:KtCore.Katie.QByteArray,after:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "8:KtCore.Katie.QByteArray.replace(before:KtCore.Katie.QByteArray,after:str)->KtCore.Katie.QByteArray",
    "7:KtCore.Katie.QByteArray.replace(before:KtCore.Katie.QString,after:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "6:KtCore.Katie.QByteArray.replace(before:KtCore.Katie.QString,after:str)->KtCore.Katie.QByteArray",
    "5:KtCore.Katie.QByteArray.replace(before:str,after:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "4:KtCore.Katie.QByteArray.replace(before:str,after:str)->KtCore.Katie.QByteArray",
    "3:KtCore.Katie.QByteArray.replace(before:str,bsize:int,after:str,asize:int)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.replace(index:int,len:int,s:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "1:KtCore.Katie.QByteArray.replace(index:int,len:int,s:str)->KtCore.Katie.QByteArray",
    "0:KtCore.Katie.QByteArray.replace(index:int,len:int,s:str,alen:int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.reserve(size:int)",
    "KtCore.Katie.QByteArray.resize(size:int)",
    "KtCore.Katie.QByteArray.right(len:int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.rightJustified(width:int,fill:char=' ',truncate:bool=false)->KtCore.Katie.QByteArray",
    "7:KtCore.Katie.QByteArray.setNum(arg__1:double,f:char='g',prec:int=6)->KtCore.Katie.QByteArray",
    "6:KtCore.Katie.QByteArray.setNum(arg__1:float,f:char='g',prec:int=6)->KtCore.Katie.QByteArray",
    "5:KtCore.Katie.QByteArray.setNum(arg__1:int,base:int=10)->KtCore.Katie.QByteArray",
    "4:KtCore.Katie.QByteArray.setNum(arg__1:long long,base:int=10)->KtCore.Katie.QByteArray",
    "3:KtCore.Katie.QByteArray.setNum(arg__1:short,base:int=10)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.setNum(arg__1:unsigned int,base:int=10)->KtCore.Katie.QByteArray",
    "1:KtCore.Katie.QByteArray.setNum(arg__1:unsigned long long,base:int=10)->KtCore.Katie.QByteArray",
    "0:KtCore.Katie.QByteArray.setNum(arg__1:ushort,base:int=10)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.setRawData(a:str,n:unsigned int)->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.simplified()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.size()->int",
    "KtCore.Katie.QByteArray.squeeze()",
    "2:KtCore.Katie.QByteArray.startsWith(c:char)->bool",
    "1:KtCore.Katie.QByteArray.startsWith(a:KtCore.Katie.QByteArray)->bool",
    "0:KtCore.Katie.QByteArray.startsWith(c:str)->bool",
    "KtCore.Katie.QByteArray.swap(other:KtCore.Katie.QByteArray)",
    "KtCore.Katie.QByteArray.toBase64()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.toDouble(ok:KtCore.bool=Q_NULLPTR)->double",
    "KtCore.Katie.QByteArray.toFloat(ok:KtCore.bool=Q_NULLPTR)->float",
    "KtCore.Katie.QByteArray.toHex()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.toInt(ok:KtCore.bool=Q_NULLPTR,base:int=10)->int",
    "KtCore.Katie.QByteArray.toLong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->long",
    "KtCore.Katie.QByteArray.toLongLong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->long long",
    "KtCore.Katie.QByteArray.toLower()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.toPercentEncoding(exclude:KtCore.Katie.QByteArray=Katie.QByteArray(),include:KtCore.Katie.QByteArray=Katie.QByteArray(),percent:char='%')->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.toShort(ok:KtCore.bool=Q_NULLPTR,base:int=10)->short",
    "KtCore.Katie.QByteArray.toStdString()->std.string",
    "KtCore.Katie.QByteArray.toUInt(ok:KtCore.bool=Q_NULLPTR,base:int=10)->unsigned int",
    "KtCore.Katie.QByteArray.toULong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->ulong",
    "KtCore.Katie.QByteArray.toULongLong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->unsigned long long",
    "KtCore.Katie.QByteArray.toUShort(ok:KtCore.bool=Q_NULLPTR,base:int=10)->ushort",
    "KtCore.Katie.QByteArray.toUpper()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.trimmed()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QByteArray.truncate(pos:int)",
    "KtCore.Katie.QByteArray.__copy__()",
    "3:KtCore.Katie.QByteArray.__iadd__(c:char)->KtCore.Katie.QByteArray",
    "2:KtCore.Katie.QByteArray.__iadd__(a:KtCore.Katie.QByteArray)->KtCore.Katie.QByteArray",
    "1:KtCore.Katie.QByteArray.__iadd__(s:KtCore.Katie.QString)->KtCore.Katie.QByteArray",
    "0:KtCore.Katie.QByteArray.__iadd__(s:str)->KtCore.Katie.QByteArray",
    nullptr}; // Sentinel

void init_Katie_QByteArray(PyObject* module)
{
    _Sbk_Katie_QByteArray_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QByteArray",
        "Katie::QByteArray",
        &Sbk_Katie_QByteArray_spec,
        Katie_QByteArray_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QByteArray >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QByteArray_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QByteArray_TypeF(),
        QByteArray_PythonToCpp_QByteArray_PTR,
        is_QByteArray_PythonToCpp_QByteArray_PTR_Convertible,
        QByteArray_PTR_CppToPython_QByteArray,
        QByteArray_COPY_CppToPython_QByteArray);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QByteArray");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QByteArray*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QByteArray&");
    Shiboken::Conversions::registerConverterName(converter, "QByteArray");
    Shiboken::Conversions::registerConverterName(converter, "QByteArray*");
    Shiboken::Conversions::registerConverterName(converter, "QByteArray&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QByteArray).name());

    // Add Python to C++ copy (value, not pointer neither reference) conversion to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        QByteArray_PythonToCpp_QByteArray_COPY,
        is_QByteArray_PythonToCpp_QByteArray_COPY_Convertible);
    // Add implicit conversions to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constcharPTR_PythonToCpp_Katie_QByteArray,
        is_constcharPTR_PythonToCpp_Katie_QByteArray_Convertible);


}
