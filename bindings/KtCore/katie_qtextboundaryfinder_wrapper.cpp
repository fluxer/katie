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
#include "katie_qtextboundaryfinder_wrapper.h"

// inner classes

// Extra includes
#include <qchar.h>
#include <qstring.h>
#include <qtextboundaryfinder.h>


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
Sbk_Katie_QTextBoundaryFinder_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QTextBoundaryFinder >()))
        return -1;

    ::Katie::QTextBoundaryFinder* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QTextBoundaryFinder", 0, 3, &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return -1;


    // Overloaded function decisor
    // 0: QTextBoundaryFinder::QTextBoundaryFinder()
    // 1: QTextBoundaryFinder::QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType,const Katie::QChar*,int)
    // 2: QTextBoundaryFinder::QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType,Katie::QString)
    // 3: QTextBoundaryFinder::QTextBoundaryFinder(Katie::QTextBoundaryFinder)
    if (numArgs == 0) {
        overloadId = 0; // QTextBoundaryFinder()
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX]), (pyArgs[0])))) {
        overloadId = 3; // QTextBoundaryFinder(Katie::QTextBoundaryFinder)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX])->converter, (pyArgs[0])))) {
        if (numArgs == 3
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1])))
            && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
            overloadId = 1; // QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType,const Katie::QChar*,int)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            overloadId = 2; // QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType,Katie::QString)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QTextBoundaryFinder_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QTextBoundaryFinder()
        {

            if (!PyErr_Occurred()) {
                // QTextBoundaryFinder()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QTextBoundaryFinder();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType type, const Katie::QChar * chars, const int length)
        {
            ::Katie::QTextBoundaryFinder::BoundaryType cppArg0{Katie::QTextBoundaryFinder::Grapheme};
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return -1;
            ::Katie::QChar* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType,const Katie::QChar*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QTextBoundaryFinder(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType type, const Katie::QString & string)
        {
            ::Katie::QTextBoundaryFinder::BoundaryType cppArg0{Katie::QTextBoundaryFinder::Grapheme};
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return -1;
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // QTextBoundaryFinder(Katie::QTextBoundaryFinder::BoundaryType,Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QTextBoundaryFinder(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 3: // QTextBoundaryFinder(const Katie::QTextBoundaryFinder & other)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QTextBoundaryFinder* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QTextBoundaryFinder(Katie::QTextBoundaryFinder)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QTextBoundaryFinder(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QTextBoundaryFinder >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QTextBoundaryFinder_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QTextBoundaryFinder_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QTextBoundaryFinder");
        return -1;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_boundaryReasons(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // boundaryReasons()const
            Katie::QTextBoundaryFinder::BoundaryReasons cppResult = const_cast<const ::Katie::QTextBoundaryFinder*>(cppSelf)->boundaryReasons();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<Katie::QTextBoundaryFinder::BoundaryReasons>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_isAtBoundary(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isAtBoundary()const
            bool cppResult = const_cast<const ::Katie::QTextBoundaryFinder*>(cppSelf)->isAtBoundary();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_isValid(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isValid()const
            bool cppResult = const_cast<const ::Katie::QTextBoundaryFinder*>(cppSelf)->isValid();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_position(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // position()const
            int cppResult = const_cast<const ::Katie::QTextBoundaryFinder*>(cppSelf)->position();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_setPosition(PyObject* self, PyObject* pyArg)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QTextBoundaryFinder::setPosition(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // setPosition(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QTextBoundaryFinderFunc_setPosition_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setPosition(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setPosition(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QTextBoundaryFinderFunc_setPosition_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QTextBoundaryFinder.setPosition");
        return {};
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_string(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // string()const
            Katie::QString cppResult = const_cast<const ::Katie::QTextBoundaryFinder*>(cppSelf)->string();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_toEnd(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toEnd()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->toEnd();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_toNextBoundary(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toNextBoundary()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = cppSelf->toNextBoundary();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_toPreviousBoundary(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toPreviousBoundary()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = cppSelf->toPreviousBoundary();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_toStart(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toStart()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->toStart();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QTextBoundaryFinderFunc_type(PyObject* self)
{
    ::Katie::QTextBoundaryFinder* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QTextBoundaryFinder *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // type()const
            Katie::QTextBoundaryFinder::BoundaryType cppResult = Katie::QTextBoundaryFinder::BoundaryType(const_cast<const ::Katie::QTextBoundaryFinder*>(cppSelf)->type());
            pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX])->converter, &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QTextBoundaryFinder_methods[] = {
    {"boundaryReasons", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_boundaryReasons), METH_NOARGS},
    {"isAtBoundary", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_isAtBoundary), METH_NOARGS},
    {"isValid", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_isValid), METH_NOARGS},
    {"position", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_position), METH_NOARGS},
    {"setPosition", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_setPosition), METH_O},
    {"string", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_string), METH_NOARGS},
    {"toEnd", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_toEnd), METH_NOARGS},
    {"toNextBoundary", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_toNextBoundary), METH_NOARGS},
    {"toPreviousBoundary", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_toPreviousBoundary), METH_NOARGS},
    {"toStart", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_toStart), METH_NOARGS},
    {"type", reinterpret_cast<PyCFunction>(Sbk_Katie_QTextBoundaryFinderFunc_type), METH_NOARGS},

    {nullptr, nullptr} // Sentinel
};

} // extern "C"

static int Sbk_Katie_QTextBoundaryFinder_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QTextBoundaryFinder_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QTextBoundaryFinder_Type = nullptr;
static SbkObjectType *Sbk_Katie_QTextBoundaryFinder_TypeF(void)
{
    return _Sbk_Katie_QTextBoundaryFinder_Type;
}

static PyType_Slot Sbk_Katie_QTextBoundaryFinder_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QTextBoundaryFinder_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QTextBoundaryFinder_clear)},
    {Py_tp_richcompare, nullptr},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QTextBoundaryFinder_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QTextBoundaryFinder_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QTextBoundaryFinder_spec = {
    "KtCore.Katie.QTextBoundaryFinder",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QTextBoundaryFinder_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ enum conversion.
static void Katie_QTextBoundaryFinder_BoundaryType_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryType(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QTextBoundaryFinder::BoundaryType*>(cppOut) =
        static_cast<::Katie::QTextBoundaryFinder::BoundaryType>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QTextBoundaryFinder_BoundaryType_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryType_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX]))
        return Katie_QTextBoundaryFinder_BoundaryType_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryType;
    return {};
}
static PyObject* Katie_QTextBoundaryFinder_BoundaryType_CppToPython_Katie_QTextBoundaryFinder_BoundaryType(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QTextBoundaryFinder::BoundaryType *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX], castCppIn);

}

static void Katie_QTextBoundaryFinder_BoundaryReason_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryReason(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QTextBoundaryFinder::BoundaryReason*>(cppOut) =
        static_cast<::Katie::QTextBoundaryFinder::BoundaryReason>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QTextBoundaryFinder_BoundaryReason_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryReason_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX]))
        return Katie_QTextBoundaryFinder_BoundaryReason_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryReason;
    return {};
}
static PyObject* Katie_QTextBoundaryFinder_BoundaryReason_CppToPython_Katie_QTextBoundaryFinder_BoundaryReason(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QTextBoundaryFinder::BoundaryReason *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX], castCppIn);

}

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QTextBoundaryFinder_PythonToCpp_QTextBoundaryFinder_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QTextBoundaryFinder_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QTextBoundaryFinder_PythonToCpp_QTextBoundaryFinder_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QTextBoundaryFinder_TypeF())))
        return QTextBoundaryFinder_PythonToCpp_QTextBoundaryFinder_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QTextBoundaryFinder_PTR_CppToPython_QTextBoundaryFinder(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QTextBoundaryFinder *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QTextBoundaryFinder_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QTextBoundaryFinder_SignatureStrings[] = {
    "3:KtCore.Katie.QTextBoundaryFinder()",
    "2:KtCore.Katie.QTextBoundaryFinder(type:KtCore.Katie.QTextBoundaryFinder.BoundaryType,chars:KtCore.Katie.QChar,length:int)",
    "1:KtCore.Katie.QTextBoundaryFinder(type:KtCore.Katie.QTextBoundaryFinder.BoundaryType,string:KtCore.Katie.QString)",
    "0:KtCore.Katie.QTextBoundaryFinder(other:KtCore.Katie.QTextBoundaryFinder)",
    "KtCore.Katie.QTextBoundaryFinder.boundaryReasons()->Katie.QTextBoundaryFinder.BoundaryReasons",
    "KtCore.Katie.QTextBoundaryFinder.isAtBoundary()->bool",
    "KtCore.Katie.QTextBoundaryFinder.isValid()->bool",
    "KtCore.Katie.QTextBoundaryFinder.position()->int",
    "KtCore.Katie.QTextBoundaryFinder.setPosition(position:int)",
    "KtCore.Katie.QTextBoundaryFinder.string()->KtCore.Katie.QString",
    "KtCore.Katie.QTextBoundaryFinder.toEnd()",
    "KtCore.Katie.QTextBoundaryFinder.toNextBoundary()->int",
    "KtCore.Katie.QTextBoundaryFinder.toPreviousBoundary()->int",
    "KtCore.Katie.QTextBoundaryFinder.toStart()",
    "KtCore.Katie.QTextBoundaryFinder.type()->KtCore.Katie.QTextBoundaryFinder.BoundaryType",
    nullptr}; // Sentinel

void init_Katie_QTextBoundaryFinder(PyObject* module)
{
    _Sbk_Katie_QTextBoundaryFinder_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QTextBoundaryFinder",
        "Katie::QTextBoundaryFinder*",
        &Sbk_Katie_QTextBoundaryFinder_spec,
        Katie_QTextBoundaryFinder_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QTextBoundaryFinder >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QTextBoundaryFinder_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QTextBoundaryFinder_TypeF(),
        QTextBoundaryFinder_PythonToCpp_QTextBoundaryFinder_PTR,
        is_QTextBoundaryFinder_PythonToCpp_QTextBoundaryFinder_PTR_Convertible,
        QTextBoundaryFinder_PTR_CppToPython_QTextBoundaryFinder);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QTextBoundaryFinder");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QTextBoundaryFinder*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QTextBoundaryFinder&");
    Shiboken::Conversions::registerConverterName(converter, "QTextBoundaryFinder");
    Shiboken::Conversions::registerConverterName(converter, "QTextBoundaryFinder*");
    Shiboken::Conversions::registerConverterName(converter, "QTextBoundaryFinder&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QTextBoundaryFinder).name());


    // Initialization of enums.

    // Initialization of enum 'BoundaryType'.
    SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QTextBoundaryFinder_TypeF(),
        "BoundaryType",
        "KtCore.Katie.QTextBoundaryFinder.BoundaryType",
        "Katie::QTextBoundaryFinder::BoundaryType");
    if (!SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX],
        Sbk_Katie_QTextBoundaryFinder_TypeF(), "Grapheme", (long) Katie::QTextBoundaryFinder::BoundaryType::Grapheme))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX],
        Sbk_Katie_QTextBoundaryFinder_TypeF(), "Word", (long) Katie::QTextBoundaryFinder::BoundaryType::Word))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX],
        Sbk_Katie_QTextBoundaryFinder_TypeF(), "Line", (long) Katie::QTextBoundaryFinder::BoundaryType::Line))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX],
        Sbk_Katie_QTextBoundaryFinder_TypeF(), "Sentence", (long) Katie::QTextBoundaryFinder::BoundaryType::Sentence))
        return;
    // Register converter for enum 'Katie::QTextBoundaryFinder::BoundaryType'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX],
            Katie_QTextBoundaryFinder_BoundaryType_CppToPython_Katie_QTextBoundaryFinder_BoundaryType);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QTextBoundaryFinder_BoundaryType_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryType,
            is_Katie_QTextBoundaryFinder_BoundaryType_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryType_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYTYPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QTextBoundaryFinder::BoundaryType");
        Shiboken::Conversions::registerConverterName(converter, "QTextBoundaryFinder::BoundaryType");
        Shiboken::Conversions::registerConverterName(converter, "BoundaryType");
    }
    // End of 'BoundaryType' enum.

    // Initialization of enum 'BoundaryReason'.
    SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QTextBoundaryFinder_TypeF(),
        "BoundaryReason",
        "KtCore.Katie.QTextBoundaryFinder.BoundaryReason",
        "Katie::QTextBoundaryFinder::BoundaryReason");
    if (!SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX],
        Sbk_Katie_QTextBoundaryFinder_TypeF(), "NotAtBoundary", (long) Katie::QTextBoundaryFinder::BoundaryReason::NotAtBoundary))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX],
        Sbk_Katie_QTextBoundaryFinder_TypeF(), "StartWord", (long) Katie::QTextBoundaryFinder::BoundaryReason::StartWord))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX],
        Sbk_Katie_QTextBoundaryFinder_TypeF(), "EndWord", (long) Katie::QTextBoundaryFinder::BoundaryReason::EndWord))
        return;
    // Register converter for enum 'Katie::QTextBoundaryFinder::BoundaryReason'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX],
            Katie_QTextBoundaryFinder_BoundaryReason_CppToPython_Katie_QTextBoundaryFinder_BoundaryReason);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QTextBoundaryFinder_BoundaryReason_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryReason,
            is_Katie_QTextBoundaryFinder_BoundaryReason_PythonToCpp_Katie_QTextBoundaryFinder_BoundaryReason_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QTEXTBOUNDARYFINDER_BOUNDARYREASON_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QTextBoundaryFinder::BoundaryReason");
        Shiboken::Conversions::registerConverterName(converter, "QTextBoundaryFinder::BoundaryReason");
        Shiboken::Conversions::registerConverterName(converter, "BoundaryReason");
    }
    // End of 'BoundaryReason' enum.


}
