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
#include "katie_qstringlist_wrapper.h"

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
Sbk_Katie_QStringList_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QStringList >()))
        return -1;

    ::Katie::QStringList* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QStringList", 0, 1, &(pyArgs[0])))
        return -1;


    // Overloaded function decisor
    // 0: QStringList::QStringList()
    // 1: QStringList::QStringList(Katie::QString)
    // 2: QStringList::QStringList(Katie::QStringList)
    if (numArgs == 0) {
        overloadId = 0; // QStringList()
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), (pyArgs[0])))) {
        overloadId = 2; // QStringList(Katie::QStringList)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        overloadId = 1; // QStringList(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringList_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QStringList()
        {

            if (!PyErr_Occurred()) {
                // QStringList()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QStringList();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QStringList(const Katie::QString & i)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QString cppArg0_local;
            ::Katie::QString* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // QStringList(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QStringList(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // QStringList(const Katie::QStringList & l)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QStringList cppArg0_local;
            ::Katie::QStringList* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // QStringList(Katie::QStringList)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QStringList(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QStringList >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QStringList_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QStringList_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QStringList");
        return -1;
}

static PyObject* Sbk_Katie_QStringListFunc_contains(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.contains(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.contains(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:contains", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QStringList::contains(Katie::QString,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // contains(Katie::QString,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 0; // contains(Katie::QString,Katie::Qt::CaseSensitivity)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc_contains_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "cs");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.contains(): got multiple values for keyword argument 'cs'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                    goto Sbk_Katie_QStringListFunc_contains_TypeError;
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

        ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // contains(Katie::QString,Katie::Qt::CaseSensitivity)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->contains(*cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringListFunc_contains_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QStringList.contains");
        return {};
}

static PyObject* Sbk_Katie_QStringListFunc_filter(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.filter(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.filter(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:filter", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QStringList::filter(Katie::QRegExp)const
    // 1: QStringList::filter(Katie::QString,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // filter(Katie::QString,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 1; // filter(Katie::QString,Katie::Qt::CaseSensitivity)const
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        overloadId = 0; // filter(Katie::QRegExp)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc_filter_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // filter(const Katie::QRegExp & rx) const
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // filter(Katie::QRegExp)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->filter(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
        case 1: // filter(const Katie::QString & str, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.filter(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringListFunc_filter_TypeError;
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

            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // filter(Katie::QString,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->filter(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringListFunc_filter_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QStringList.filter");
        return {};
}

static PyObject* Sbk_Katie_QStringListFunc_indexOf(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.indexOf(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.indexOf(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:indexOf", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QStringList::indexOf(Katie::QRegExp&,int)const
    // 1: QStringList::indexOf(Katie::QRegExp,int)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // indexOf(Katie::QRegExp&,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // indexOf(Katie::QRegExp&,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc_indexOf_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // indexOf(Katie::QRegExp & rx, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringListFunc_indexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // indexOf(Katie::QRegExp&,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->indexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // indexOf(const Katie::QRegExp & rx, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringListFunc_indexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // indexOf(Katie::QRegExp,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->indexOf(*cppArg0, cppArg1);
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

    Sbk_Katie_QStringListFunc_indexOf_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QStringList.indexOf");
        return {};
}

static PyObject* Sbk_Katie_QStringListFunc_join(PyObject* self, PyObject* pyArg)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QStringList::join(Katie::QString)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // join(Katie::QString)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc_join_TypeError;

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
            // join(Katie::QString)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->join(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringListFunc_join_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QStringList.join");
        return {};
}

static PyObject* Sbk_Katie_QStringListFunc_lastIndexOf(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.lastIndexOf(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.lastIndexOf(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:lastIndexOf", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QStringList::lastIndexOf(Katie::QRegExp&,int)const
    // 1: QStringList::lastIndexOf(Katie::QRegExp,int)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // lastIndexOf(Katie::QRegExp&,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // lastIndexOf(Katie::QRegExp&,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc_lastIndexOf_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // lastIndexOf(Katie::QRegExp & rx, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringListFunc_lastIndexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // lastIndexOf(Katie::QRegExp&,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // lastIndexOf(const Katie::QRegExp & rx, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringListFunc_lastIndexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // lastIndexOf(Katie::QRegExp,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QStringList*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1);
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

    Sbk_Katie_QStringListFunc_lastIndexOf_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QStringList.lastIndexOf");
        return {};
}

static PyObject* Sbk_Katie_QStringListFunc_removeDuplicates(PyObject* self)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // removeDuplicates()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = cppSelf->removeDuplicates();
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

static PyObject* Sbk_Katie_QStringListFunc_replaceInStrings(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.replaceInStrings(): too many arguments");
        return {};
    } else if (numArgs < 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.replaceInStrings(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:replaceInStrings", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QStringList::replaceInStrings(Katie::QRegExp,Katie::QString)
    // 1: QStringList::replaceInStrings(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
    if (numArgs >= 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
        if (numArgs == 2) {
            overloadId = 1; // replaceInStrings(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
        } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
            overloadId = 1; // replaceInStrings(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
        }
    } else if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
        overloadId = 0; // replaceInStrings(Katie::QRegExp,Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc_replaceInStrings_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // replaceInStrings(const Katie::QRegExp & rx, const Katie::QString & after)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
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
                // replaceInStrings(Katie::QRegExp,Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList & cppResult = cppSelf->replaceInStrings(*cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
        case 1: // replaceInStrings(const Katie::QString & before, const Katie::QString & after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QStringList.replaceInStrings(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringListFunc_replaceInStrings_TypeError;
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

            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // replaceInStrings(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList & cppResult = cppSelf->replaceInStrings(*cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringListFunc_replaceInStrings_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QStringList.replaceInStrings");
        return {};
}

static PyObject* Sbk_Katie_QStringListFunc_sort(PyObject* self)
{
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // sort()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->sort();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QStringList___copy__(PyObject* self)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QStringList& cppSelf = *reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppSelf);
    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QStringList_methods[] = {
    {"contains", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_contains), METH_VARARGS|METH_KEYWORDS},
    {"filter", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_filter), METH_VARARGS|METH_KEYWORDS},
    {"indexOf", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_indexOf), METH_VARARGS|METH_KEYWORDS},
    {"join", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_join), METH_O},
    {"lastIndexOf", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_lastIndexOf), METH_VARARGS|METH_KEYWORDS},
    {"removeDuplicates", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_removeDuplicates), METH_NOARGS},
    {"replaceInStrings", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_replaceInStrings), METH_VARARGS|METH_KEYWORDS},
    {"sort", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringListFunc_sort), METH_NOARGS},

    {"__copy__", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringList___copy__), METH_NOARGS},
    {nullptr, nullptr} // Sentinel
};

static PyObject* Sbk_Katie_QStringListFunc___add__(PyObject* self, PyObject* pyArg)
{
    bool isReverse = SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], pyArg)
                    && !SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], self);
    if (isReverse)
        std::swap(self, pyArg);
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Do not enter here if other object has implemented a reverse operator.
    if (!pyResult) {

    // Overloaded function decisor
    // 0: QStringList::operator+(Katie::QStringList)const
    if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), (pyArg)))) {
        overloadId = 0; // operator+(Katie::QStringList)const
    }

    if (isReverse && overloadId == -1) {
        PyErr_SetString(PyExc_NotImplementedError, "reverse operator not implemented.");
        return {};
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc___add___TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QStringList cppArg0_local;
        ::Katie::QStringList* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), pythonToCpp))
            pythonToCpp(pyArg, &cppArg0_local);
        else
            pythonToCpp(pyArg, &cppArg0);


        if (!PyErr_Occurred()) {
            // operator+(Katie::QStringList)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QStringList cppResult = (*cppSelf) + (*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
        }
    }

    } // End of "if (!pyResult)"

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringListFunc___add___TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QStringList.__add__");
        return {};
}

static PyObject* Sbk_Katie_QStringListFunc___lshift__(PyObject* self, PyObject* pyArg)
{
    bool isReverse = SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], pyArg)
                    && !SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], self);
    if (isReverse)
        std::swap(self, pyArg);
    ::Katie::QStringList* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    if (!isReverse
        && Shiboken::Object::checkType(pyArg)
        && !PyObject_TypeCheck(pyArg, self->ob_type)
        && PyObject_HasAttrString(pyArg, const_cast<char*>("__rlshift__"))) {
        PyObject* revOpMethod = PyObject_GetAttrString(pyArg, const_cast<char*>("__rlshift__"));
        if (revOpMethod && PyCallable_Check(revOpMethod)) {
            pyResult = PyObject_CallFunction(revOpMethod, const_cast<char*>("O"), self);
            if (PyErr_Occurred() && (PyErr_ExceptionMatches(PyExc_NotImplementedError) || PyErr_ExceptionMatches(PyExc_AttributeError))) {
                PyErr_Clear();
                Py_XDECREF(pyResult);
                pyResult = nullptr;
            }
        }
        Py_XDECREF(revOpMethod);

    }
    // Do not enter here if other object has implemented a reverse operator.
    if (!pyResult) {

    // Overloaded function decisor
    // 0: QStringList::operator<<(Katie::QString)
    // 1: QStringList::operator<<(Katie::QStringList)
    if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), (pyArg)))) {
        overloadId = 1; // operator<<(Katie::QStringList)
    } else if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // operator<<(Katie::QString)
    }

    if (isReverse && overloadId == -1) {
        PyErr_SetString(PyExc_NotImplementedError, "reverse operator not implemented.");
        return {};
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringListFunc___lshift___TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // operator<<(const Katie::QString & str)
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
                // operator<<(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList & cppResult = (*cppSelf) << (*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
        case 1: // operator<<(const Katie::QStringList & l)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QStringList cppArg0_local;
            ::Katie::QStringList* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // operator<<(Katie::QStringList)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList & cppResult = (*cppSelf) << (*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
    }

    } // End of "if (!pyResult)"

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringListFunc___lshift___TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QStringList.__lshift__");
        return {};
}

} // extern "C"

static int Sbk_Katie_QStringList_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QStringList_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QStringList_Type = nullptr;
static SbkObjectType *Sbk_Katie_QStringList_TypeF(void)
{
    return _Sbk_Katie_QStringList_Type;
}

static PyType_Slot Sbk_Katie_QStringList_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QStringList_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QStringList_clear)},
    {Py_tp_richcompare, nullptr},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QStringList_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QStringList_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    // type supports number protocol
    {Py_nb_add, (void *)Sbk_Katie_QStringListFunc___add__},
    {Py_nb_lshift, (void *)Sbk_Katie_QStringListFunc___lshift__},
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QStringList_spec = {
    "KtCore.Katie.QStringList",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QStringList_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QStringList_PythonToCpp_QStringList_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QStringList_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QStringList_PythonToCpp_QStringList_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QStringList_TypeF())))
        return QStringList_PythonToCpp_QStringList_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QStringList_PTR_CppToPython_QStringList(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QStringList *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QStringList_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// C++ to Python copy conversion.
static PyObject* QStringList_COPY_CppToPython_QStringList(const void* cppIn) {
    return Shiboken::Object::newObject(Sbk_Katie_QStringList_TypeF(), new ::Katie::QStringList(*reinterpret_cast<const ::Katie::QStringList*>(cppIn)), true, true);
}

// Python to C++ copy conversion.
static void QStringList_PythonToCpp_QStringList_COPY(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QStringList*>(cppOut) = *reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(pyIn)));
}
static PythonToCppFunc is_QStringList_PythonToCpp_QStringList_COPY_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QStringList_TypeF())))
        return QStringList_PythonToCpp_QStringList_COPY;
    return {};
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QStringList_SignatureStrings[] = {
    "2:KtCore.Katie.QStringList()",
    "1:KtCore.Katie.QStringList(i:KtCore.Katie.QString)",
    "0:KtCore.Katie.QStringList(l:KtCore.Katie.QStringList)",
    "KtCore.Katie.QStringList.contains(str:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "1:KtCore.Katie.QStringList.filter(rx:KtCore.Katie.QRegExp)->KtCore.Katie.QStringList",
    "0:KtCore.Katie.QStringList.filter(str:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QStringList",
    "1:KtCore.Katie.QStringList.indexOf(rx:KtCore.Katie.QRegExp,from:int=0)->int",
    "0:KtCore.Katie.QStringList.indexOf(rx:KtCore.Katie.QRegExp,from:int=0)->int",
    "KtCore.Katie.QStringList.join(sep:KtCore.Katie.QString)->KtCore.Katie.QString",
    "1:KtCore.Katie.QStringList.lastIndexOf(rx:KtCore.Katie.QRegExp,from:int=-1)->int",
    "0:KtCore.Katie.QStringList.lastIndexOf(rx:KtCore.Katie.QRegExp,from:int=-1)->int",
    "KtCore.Katie.QStringList.removeDuplicates()->int",
    "1:KtCore.Katie.QStringList.replaceInStrings(rx:KtCore.Katie.QRegExp,after:KtCore.Katie.QString)->KtCore.Katie.QStringList",
    "0:KtCore.Katie.QStringList.replaceInStrings(before:KtCore.Katie.QString,after:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QStringList",
    "KtCore.Katie.QStringList.sort()",
    "KtCore.Katie.QStringList.__copy__()",
    "KtCore.Katie.QStringList.__add__(other:KtCore.Katie.QStringList)->KtCore.Katie.QStringList",
    "1:KtCore.Katie.QStringList.__lshift__(str:KtCore.Katie.QString)->KtCore.Katie.QStringList",
    "0:KtCore.Katie.QStringList.__lshift__(l:KtCore.Katie.QStringList)->KtCore.Katie.QStringList",
    nullptr}; // Sentinel

void init_Katie_QStringList(PyObject* module)
{
    _Sbk_Katie_QStringList_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QStringList",
        "Katie::QStringList",
        &Sbk_Katie_QStringList_spec,
        Katie_QStringList_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QStringList >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QStringList_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QStringList_TypeF(),
        QStringList_PythonToCpp_QStringList_PTR,
        is_QStringList_PythonToCpp_QStringList_PTR_Convertible,
        QStringList_PTR_CppToPython_QStringList,
        QStringList_COPY_CppToPython_QStringList);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QStringList");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QStringList*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QStringList&");
    Shiboken::Conversions::registerConverterName(converter, "QStringList");
    Shiboken::Conversions::registerConverterName(converter, "QStringList*");
    Shiboken::Conversions::registerConverterName(converter, "QStringList&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QStringList).name());

    // Add Python to C++ copy (value, not pointer neither reference) conversion to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        QStringList_PythonToCpp_QStringList_COPY,
        is_QStringList_PythonToCpp_QStringList_COPY_Convertible);


}
