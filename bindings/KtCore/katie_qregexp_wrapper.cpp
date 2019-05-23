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
#include "katie_qregexp_wrapper.h"

// inner classes

// Extra includes
#include <qregexp.h>
#include <qstring.h>
#include <qstringlist.h>


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
Sbk_Katie_QRegExp_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QRegExp >()))
        return -1;

    ::Katie::QRegExp* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 3) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp(): too many arguments");
        return -1;
    }

    if (!PyArg_ParseTuple(args, "|OOO:QRegExp", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return -1;


    // Overloaded function decisor
    // 0: QRegExp::QRegExp()
    // 1: QRegExp::QRegExp(Katie::QRegExp)
    // 2: QRegExp::QRegExp(Katie::QString,Katie::Qt::CaseSensitivity,Katie::QRegExp::PatternSyntax)
    if (numArgs == 0) {
        overloadId = 0; // QRegExp()
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // QRegExp(Katie::QString,Katie::Qt::CaseSensitivity,Katie::QRegExp::PatternSyntax)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 2; // QRegExp(Katie::QString,Katie::Qt::CaseSensitivity,Katie::QRegExp::PatternSyntax)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX])->converter, (pyArgs[2])))) {
                overloadId = 2; // QRegExp(Katie::QString,Katie::Qt::CaseSensitivity,Katie::QRegExp::PatternSyntax)
            }
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        overloadId = 1; // QRegExp(Katie::QRegExp)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExp_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QRegExp()
        {

            if (!PyErr_Occurred()) {
                // QRegExp()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QRegExp();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QRegExp(const Katie::QRegExp & rx)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QRegExp(Katie::QRegExp)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QRegExp(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // QRegExp(const Katie::QString & pattern, Katie::Qt::CaseSensitivity cs, Katie::QRegExp::PatternSyntax syntax)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp(): got multiple values for keyword argument 'cs'.");
                    return -1;
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QRegExp_Init_TypeError;
                }
                value = PyDict_GetItemString(kwds, "syntax");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp(): got multiple values for keyword argument 'syntax'.");
                    return -1;
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QRegExp_Init_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::QRegExp::PatternSyntax cppArg2 = Katie::QRegExp::PatternSyntax::RegExp;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // QRegExp(Katie::QString,Katie::Qt::CaseSensitivity,Katie::QRegExp::PatternSyntax)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QRegExp(*cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QRegExp >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QRegExp_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QRegExp_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QRegExp");
        return -1;
}

static PyObject* Sbk_Katie_QRegExpFunc_cap(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.cap(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:cap", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QRegExp::cap(int)const
    if (numArgs == 0) {
        overloadId = 0; // cap(int)const
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        overloadId = 0; // cap(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_cap_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "nth");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.cap(): got multiple values for keyword argument 'nth'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0]))))
                    goto Sbk_Katie_QRegExpFunc_cap_TypeError;
            }
        }
        int cppArg0 = 0;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // cap(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->cap(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QRegExpFunc_cap_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QRegExp.cap");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_captureCount(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // captureCount()const
            int cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->captureCount();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_capturedTexts(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // capturedTexts()const
            Katie::QStringList cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->capturedTexts();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_caseSensitivity(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // caseSensitivity()const
            Katie::Qt::CaseSensitivity cppResult = Katie::Qt::CaseSensitivity(const_cast<const ::Katie::QRegExp*>(cppSelf)->caseSensitivity());
            pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_errorString(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // errorString()const
            Katie::QString cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->errorString();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_escape(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QRegExp::escape(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // escape(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_escape_TypeError;

    // Call function/method
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
            // escape(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QRegExp::escape(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QRegExpFunc_escape_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QRegExp.escape");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_exactMatch(PyObject* self, PyObject* pyArg)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QRegExp::exactMatch(Katie::QString)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // exactMatch(Katie::QString)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_exactMatch_TypeError;

    // Call function/method
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
            // exactMatch(Katie::QString)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->exactMatch(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QRegExpFunc_exactMatch_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QRegExp.exactMatch");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_indexIn(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.indexIn(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.indexIn(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:indexIn", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QRegExp::indexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // indexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // indexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX])->converter, (pyArgs[2])))) {
                overloadId = 0; // indexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_indexIn_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "offset");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.indexIn(): got multiple values for keyword argument 'offset'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QRegExpFunc_indexIn_TypeError;
            }
            value = PyDict_GetItemString(kwds, "caretMode");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.indexIn(): got multiple values for keyword argument 'caretMode'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX])->converter, (pyArgs[2]))))
                    goto Sbk_Katie_QRegExpFunc_indexIn_TypeError;
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
        ::Katie::QRegExp::CaretMode cppArg2 = Katie::QRegExp::CaretMode::CaretAtZero;
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // indexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->indexIn(*cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QRegExpFunc_indexIn_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QRegExp.indexIn");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_isEmpty(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isEmpty()const
            bool cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->isEmpty();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_isMinimal(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isMinimal()const
            bool cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->isMinimal();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_isValid(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isValid()const
            bool cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->isValid();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_lastIndexIn(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.lastIndexIn(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.lastIndexIn(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:lastIndexIn", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QRegExp::lastIndexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // lastIndexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // lastIndexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX])->converter, (pyArgs[2])))) {
                overloadId = 0; // lastIndexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_lastIndexIn_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "offset");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.lastIndexIn(): got multiple values for keyword argument 'offset'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QRegExpFunc_lastIndexIn_TypeError;
            }
            value = PyDict_GetItemString(kwds, "caretMode");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.lastIndexIn(): got multiple values for keyword argument 'caretMode'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX])->converter, (pyArgs[2]))))
                    goto Sbk_Katie_QRegExpFunc_lastIndexIn_TypeError;
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
        ::Katie::QRegExp::CaretMode cppArg2 = Katie::QRegExp::CaretMode::CaretAtZero;
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // lastIndexIn(Katie::QString,int,Katie::QRegExp::CaretMode)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->lastIndexIn(*cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QRegExpFunc_lastIndexIn_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QRegExp.lastIndexIn");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_matchedLength(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // matchedLength()const
            int cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->matchedLength();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_pattern(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // pattern()const
            Katie::QString cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->pattern();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_patternSyntax(PyObject* self)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // patternSyntax()const
            Katie::QRegExp::PatternSyntax cppResult = Katie::QRegExp::PatternSyntax(const_cast<const ::Katie::QRegExp*>(cppSelf)->patternSyntax());
            pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX])->converter, &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QRegExpFunc_pos(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.pos(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:pos", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QRegExp::pos(int)const
    if (numArgs == 0) {
        overloadId = 0; // pos(int)const
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        overloadId = 0; // pos(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_pos_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "nth");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QRegExp.pos(): got multiple values for keyword argument 'nth'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0]))))
                    goto Sbk_Katie_QRegExpFunc_pos_TypeError;
            }
        }
        int cppArg0 = 0;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // pos(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = const_cast<const ::Katie::QRegExp*>(cppSelf)->pos(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QRegExpFunc_pos_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QRegExp.pos");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_setCaseSensitivity(PyObject* self, PyObject* pyArg)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QRegExp::setCaseSensitivity(Katie::Qt::CaseSensitivity)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArg)))) {
        overloadId = 0; // setCaseSensitivity(Katie::Qt::CaseSensitivity)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_setCaseSensitivity_TypeError;

    // Call function/method
    {
        ::Katie::Qt::CaseSensitivity cppArg0{Katie::Qt::CaseInsensitive};
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setCaseSensitivity(Katie::Qt::CaseSensitivity)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setCaseSensitivity(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QRegExpFunc_setCaseSensitivity_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QRegExp.setCaseSensitivity");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_setMinimal(PyObject* self, PyObject* pyArg)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QRegExp::setMinimal(bool)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArg)))) {
        overloadId = 0; // setMinimal(bool)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_setMinimal_TypeError;

    // Call function/method
    {
        bool cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setMinimal(bool)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setMinimal(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QRegExpFunc_setMinimal_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QRegExp.setMinimal");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_setPattern(PyObject* self, PyObject* pyArg)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QRegExp::setPattern(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // setPattern(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_setPattern_TypeError;

    // Call function/method
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
            // setPattern(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setPattern(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QRegExpFunc_setPattern_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QRegExp.setPattern");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_setPatternSyntax(PyObject* self, PyObject* pyArg)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QRegExp::setPatternSyntax(Katie::QRegExp::PatternSyntax)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX])->converter, (pyArg)))) {
        overloadId = 0; // setPatternSyntax(Katie::QRegExp::PatternSyntax)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_setPatternSyntax_TypeError;

    // Call function/method
    {
        ::Katie::QRegExp::PatternSyntax cppArg0{Katie::QRegExp::RegExp};
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setPatternSyntax(Katie::QRegExp::PatternSyntax)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setPatternSyntax(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QRegExpFunc_setPatternSyntax_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QRegExp.setPatternSyntax");
        return {};
}

static PyObject* Sbk_Katie_QRegExpFunc_swap(PyObject* self, PyObject* pyArg)
{
    ::Katie::QRegExp* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QRegExp::swap(Katie::QRegExp&)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArg)))) {
        overloadId = 0; // swap(Katie::QRegExp&)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QRegExpFunc_swap_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QRegExp* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // swap(Katie::QRegExp&)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->swap(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QRegExpFunc_swap_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QRegExp.swap");
        return {};
}

static PyMethodDef Sbk_Katie_QRegExp_methods[] = {
    {"cap", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_cap), METH_VARARGS|METH_KEYWORDS},
    {"captureCount", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_captureCount), METH_NOARGS},
    {"capturedTexts", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_capturedTexts), METH_NOARGS},
    {"caseSensitivity", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_caseSensitivity), METH_NOARGS},
    {"errorString", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_errorString), METH_NOARGS},
    {"escape", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_escape), METH_O|METH_STATIC},
    {"exactMatch", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_exactMatch), METH_O},
    {"indexIn", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_indexIn), METH_VARARGS|METH_KEYWORDS},
    {"isEmpty", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_isEmpty), METH_NOARGS},
    {"isMinimal", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_isMinimal), METH_NOARGS},
    {"isValid", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_isValid), METH_NOARGS},
    {"lastIndexIn", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_lastIndexIn), METH_VARARGS|METH_KEYWORDS},
    {"matchedLength", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_matchedLength), METH_NOARGS},
    {"pattern", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_pattern), METH_NOARGS},
    {"patternSyntax", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_patternSyntax), METH_NOARGS},
    {"pos", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_pos), METH_VARARGS|METH_KEYWORDS},
    {"setCaseSensitivity", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_setCaseSensitivity), METH_O},
    {"setMinimal", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_setMinimal), METH_O},
    {"setPattern", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_setPattern), METH_O},
    {"setPatternSyntax", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_setPatternSyntax), METH_O},
    {"swap", reinterpret_cast<PyCFunction>(Sbk_Katie_QRegExpFunc_swap), METH_O},

    {nullptr, nullptr} // Sentinel
};

// Rich comparison
static PyObject* Sbk_Katie_QRegExp_richcompare(PyObject* self, PyObject* pyArg, int op)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QRegExp& cppSelf = *reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(self)));
    SBK_UNUSED(cppSelf)
    PyObject* pyResult{};
    PythonToCppFunc pythonToCpp;
    SBK_UNUSED(pythonToCpp)

    switch (op) {
        case Py_NE:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArg)))) {
                // operator!=(const Katie::QRegExp & rx) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QRegExp* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf !=(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                pyResult = Py_True;
                Py_INCREF(pyResult);
            }

            break;
        case Py_EQ:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArg)))) {
                // operator==(const Katie::QRegExp & rx) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QRegExp* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf ==(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else {
                pyResult = Py_False;
                Py_INCREF(pyResult);
            }

            break;
        default:
            goto Sbk_Katie_QRegExp_RichComparison_TypeError;
    }

    if (pyResult && !PyErr_Occurred())
        return pyResult;
    Sbk_Katie_QRegExp_RichComparison_TypeError:
    PyErr_SetString(PyExc_NotImplementedError, "operator not implemented.");
    return {};

}

} // extern "C"

static int Sbk_Katie_QRegExp_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QRegExp_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QRegExp_Type = nullptr;
static SbkObjectType *Sbk_Katie_QRegExp_TypeF(void)
{
    return _Sbk_Katie_QRegExp_Type;
}

static PyType_Slot Sbk_Katie_QRegExp_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QRegExp_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QRegExp_clear)},
    {Py_tp_richcompare, reinterpret_cast<void*>(Sbk_Katie_QRegExp_richcompare)},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QRegExp_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QRegExp_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QRegExp_spec = {
    "KtCore.Katie.QRegExp",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QRegExp_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ enum conversion.
static void Katie_QRegExp_PatternSyntax_PythonToCpp_Katie_QRegExp_PatternSyntax(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QRegExp::PatternSyntax*>(cppOut) =
        static_cast<::Katie::QRegExp::PatternSyntax>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QRegExp_PatternSyntax_PythonToCpp_Katie_QRegExp_PatternSyntax_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX]))
        return Katie_QRegExp_PatternSyntax_PythonToCpp_Katie_QRegExp_PatternSyntax;
    return {};
}
static PyObject* Katie_QRegExp_PatternSyntax_CppToPython_Katie_QRegExp_PatternSyntax(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QRegExp::PatternSyntax *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX], castCppIn);

}

static void Katie_QRegExp_CaretMode_PythonToCpp_Katie_QRegExp_CaretMode(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QRegExp::CaretMode*>(cppOut) =
        static_cast<::Katie::QRegExp::CaretMode>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QRegExp_CaretMode_PythonToCpp_Katie_QRegExp_CaretMode_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX]))
        return Katie_QRegExp_CaretMode_PythonToCpp_Katie_QRegExp_CaretMode;
    return {};
}
static PyObject* Katie_QRegExp_CaretMode_CppToPython_Katie_QRegExp_CaretMode(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QRegExp::CaretMode *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX], castCppIn);

}

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QRegExp_PythonToCpp_QRegExp_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QRegExp_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QRegExp_PythonToCpp_QRegExp_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QRegExp_TypeF())))
        return QRegExp_PythonToCpp_QRegExp_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QRegExp_PTR_CppToPython_QRegExp(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QRegExp *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QRegExp_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QRegExp_SignatureStrings[] = {
    "2:KtCore.Katie.QRegExp()",
    "1:KtCore.Katie.QRegExp(rx:KtCore.Katie.QRegExp)",
    "0:KtCore.Katie.QRegExp(pattern:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive,syntax:KtCore.Katie.QRegExp.PatternSyntax=RegExp)",
    "KtCore.Katie.QRegExp.cap(nth:int=0)->KtCore.Katie.QString",
    "KtCore.Katie.QRegExp.captureCount()->int",
    "KtCore.Katie.QRegExp.capturedTexts()->KtCore.Katie.QStringList",
    "KtCore.Katie.QRegExp.caseSensitivity()->KtCore.Katie.Qt.CaseSensitivity",
    "KtCore.Katie.QRegExp.errorString()->KtCore.Katie.QString",
    "KtCore.Katie.QRegExp.escape(str:KtCore.Katie.QString)->KtCore.Katie.QString",
    "KtCore.Katie.QRegExp.exactMatch(str:KtCore.Katie.QString)->bool",
    "KtCore.Katie.QRegExp.indexIn(str:KtCore.Katie.QString,offset:int=0,caretMode:KtCore.Katie.QRegExp.CaretMode=CaretAtZero)->int",
    "KtCore.Katie.QRegExp.isEmpty()->bool",
    "KtCore.Katie.QRegExp.isMinimal()->bool",
    "KtCore.Katie.QRegExp.isValid()->bool",
    "KtCore.Katie.QRegExp.lastIndexIn(str:KtCore.Katie.QString,offset:int=-1,caretMode:KtCore.Katie.QRegExp.CaretMode=CaretAtZero)->int",
    "KtCore.Katie.QRegExp.matchedLength()->int",
    "KtCore.Katie.QRegExp.pattern()->KtCore.Katie.QString",
    "KtCore.Katie.QRegExp.patternSyntax()->KtCore.Katie.QRegExp.PatternSyntax",
    "KtCore.Katie.QRegExp.pos(nth:int=0)->int",
    "KtCore.Katie.QRegExp.setCaseSensitivity(cs:KtCore.Katie.Qt.CaseSensitivity)",
    "KtCore.Katie.QRegExp.setMinimal(minimal:bool)",
    "KtCore.Katie.QRegExp.setPattern(pattern:KtCore.Katie.QString)",
    "KtCore.Katie.QRegExp.setPatternSyntax(syntax:KtCore.Katie.QRegExp.PatternSyntax)",
    "KtCore.Katie.QRegExp.swap(other:KtCore.Katie.QRegExp)",
    nullptr}; // Sentinel

void init_Katie_QRegExp(PyObject* module)
{
    _Sbk_Katie_QRegExp_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QRegExp",
        "Katie::QRegExp*",
        &Sbk_Katie_QRegExp_spec,
        Katie_QRegExp_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QRegExp >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QRegExp_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QRegExp_TypeF(),
        QRegExp_PythonToCpp_QRegExp_PTR,
        is_QRegExp_PythonToCpp_QRegExp_PTR_Convertible,
        QRegExp_PTR_CppToPython_QRegExp);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QRegExp");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QRegExp*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QRegExp&");
    Shiboken::Conversions::registerConverterName(converter, "QRegExp");
    Shiboken::Conversions::registerConverterName(converter, "QRegExp*");
    Shiboken::Conversions::registerConverterName(converter, "QRegExp&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QRegExp).name());


    // Initialization of enums.

    // Initialization of enum 'PatternSyntax'.
    SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QRegExp_TypeF(),
        "PatternSyntax",
        "KtCore.Katie.QRegExp.PatternSyntax",
        "Katie::QRegExp::PatternSyntax");
    if (!SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX],
        Sbk_Katie_QRegExp_TypeF(), "RegExp", (long) Katie::QRegExp::PatternSyntax::RegExp))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX],
        Sbk_Katie_QRegExp_TypeF(), "Wildcard", (long) Katie::QRegExp::PatternSyntax::Wildcard))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX],
        Sbk_Katie_QRegExp_TypeF(), "FixedString", (long) Katie::QRegExp::PatternSyntax::FixedString))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX],
        Sbk_Katie_QRegExp_TypeF(), "RegExp2", (long) Katie::QRegExp::PatternSyntax::RegExp2))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX],
        Sbk_Katie_QRegExp_TypeF(), "WildcardUnix", (long) Katie::QRegExp::PatternSyntax::WildcardUnix))
        return;
    // Register converter for enum 'Katie::QRegExp::PatternSyntax'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX],
            Katie_QRegExp_PatternSyntax_CppToPython_Katie_QRegExp_PatternSyntax);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QRegExp_PatternSyntax_PythonToCpp_Katie_QRegExp_PatternSyntax,
            is_Katie_QRegExp_PatternSyntax_PythonToCpp_Katie_QRegExp_PatternSyntax_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QREGEXP_PATTERNSYNTAX_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QRegExp::PatternSyntax");
        Shiboken::Conversions::registerConverterName(converter, "QRegExp::PatternSyntax");
        Shiboken::Conversions::registerConverterName(converter, "PatternSyntax");
    }
    // End of 'PatternSyntax' enum.

    // Initialization of enum 'CaretMode'.
    SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QRegExp_TypeF(),
        "CaretMode",
        "KtCore.Katie.QRegExp.CaretMode",
        "Katie::QRegExp::CaretMode");
    if (!SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX],
        Sbk_Katie_QRegExp_TypeF(), "CaretAtZero", (long) Katie::QRegExp::CaretMode::CaretAtZero))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX],
        Sbk_Katie_QRegExp_TypeF(), "CaretAtOffset", (long) Katie::QRegExp::CaretMode::CaretAtOffset))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX],
        Sbk_Katie_QRegExp_TypeF(), "CaretWontMatch", (long) Katie::QRegExp::CaretMode::CaretWontMatch))
        return;
    // Register converter for enum 'Katie::QRegExp::CaretMode'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX],
            Katie_QRegExp_CaretMode_CppToPython_Katie_QRegExp_CaretMode);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QRegExp_CaretMode_PythonToCpp_Katie_QRegExp_CaretMode,
            is_Katie_QRegExp_CaretMode_PythonToCpp_Katie_QRegExp_CaretMode_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QREGEXP_CARETMODE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QRegExp::CaretMode");
        Shiboken::Conversions::registerConverterName(converter, "QRegExp::CaretMode");
        Shiboken::Conversions::registerConverterName(converter, "CaretMode");
    }
    // End of 'CaretMode' enum.


}
