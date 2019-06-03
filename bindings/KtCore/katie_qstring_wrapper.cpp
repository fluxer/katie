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
#include "katie_qstring_wrapper.h"

// inner classes

// Extra includes
#include <qbytearray.h>
#include <qchar.h>
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
Sbk_Katie_QString_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QString >()))
        return -1;

    ::Katie::QString* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString(): too many arguments");
        return -1;
    }

    if (!PyArg_ParseTuple(args, "|OO:QString", &(pyArgs[0]), &(pyArgs[1])))
        return -1;


    // Overloaded function decisor
    // 0: QString::QString()
    // 1: QString::QString(Katie::QByteArray)
    // 2: QString::QString(const Katie::QChar*,int)
    // 3: QString::QString(Katie::QChar)
    // 4: QString::QString(Katie::QLatin1String)
    // 5: QString::QString(Katie::QString)
    // 6: QString::QString(const char*)
    // 7: QString::QString(int,Katie::QChar)
    // 8: QString::QString(int,Katie::Qt::Initialization)
    if (numArgs == 0) {
        overloadId = 0; // QString()
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_INITIALIZATION_IDX])->converter, (pyArgs[1])))) {
            overloadId = 8; // QString(int,Katie::Qt::Initialization)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1])))) {
            overloadId = 7; // QString(int,Katie::QChar)
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        overloadId = 6; // QString(const char*)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        overloadId = 4; // QString(Katie::QLatin1String)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // QString(const Katie::QChar*,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 2; // QString(const Katie::QChar*,int)
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        overloadId = 1; // QString(Katie::QByteArray)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        overloadId = 5; // QString(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QString_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QString()
        {

            if (!PyErr_Occurred()) {
                // QString()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QString(const Katie::QByteArray & a)
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
                // QString(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // QString(const Katie::QChar * unicode, int size)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "size");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString(): got multiple values for keyword argument 'size'.");
                    return -1;
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QString_Init_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QChar* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QString(const Katie::QChar*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 3: // QString(const Katie::QChar c)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QString(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 4: // QString(const Katie::QLatin1String & latin1)
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
                // QString(Katie::QLatin1String)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 5: // QString(const Katie::QString & arg__1)
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
                // QString(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 6: // QString(const char * ch)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QString(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 7: // QString(const int size, const Katie::QChar c)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return -1;
            ::Katie::QChar cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QString(int,Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 8: // QString(int size, Katie::Qt::Initialization arg__2)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::Katie::Qt::Initialization cppArg1{Katie::Qt::Uninitialized};
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QString(int,Katie::Qt::Initialization)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QString(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QString >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QString_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QString_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString");
        return -1;
}

static PyObject* Sbk_Katie_QStringFunc_append(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::append(Katie::QChar)
    // 1: QString::append(Katie::QByteArray)
    // 2: QString::append(Katie::QLatin1String)
    // 3: QString::append(Katie::QString)
    // 4: QString::append(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 4; // append(const char*)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
        overloadId = 2; // append(Katie::QLatin1String)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // append(Katie::QByteArray)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArg)))) {
        overloadId = 0; // append(Katie::QChar)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 3; // append(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_append_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // append(Katie::QChar c)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // append(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->append(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // append(const Katie::QByteArray & s)
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
                // append(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->append(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // append(const Katie::QLatin1String & s)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // append(Katie::QLatin1String)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->append(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // append(const Katie::QString & s)
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
                // append(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->append(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 4: // append(const char * s)
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // append(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->append(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_append_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.append");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_arg(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 9) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOOOOOOOO:arg", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2]), &(pyArgs[3]), &(pyArgs[4]), &(pyArgs[5]), &(pyArgs[6]), &(pyArgs[7]), &(pyArgs[8])))
        return {};


    // Overloaded function decisor
    // 0: QString::arg(Katie::QChar,int,Katie::QChar)const
    // 1: QString::arg(char,int,Katie::QChar)const
    // 2: QString::arg(Katie::QString,int,Katie::QChar)const
    // 3: QString::arg(Katie::QString,Katie::QString)const
    // 4: QString::arg(Katie::QString,Katie::QString,Katie::QString)const
    // 5: QString::arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
    // 6: QString::arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
    // 7: QString::arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
    // 8: QString::arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
    // 9: QString::arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
    // 10: QString::arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
    // 11: QString::arg(double,int,char,int,Katie::QChar)const
    // 12: QString::arg(int,int,int,Katie::QChar)const
    // 13: QString::arg(long,int,int,Katie::QChar)const
    // 14: QString::arg(long long,int,int,Katie::QChar)const
    // 15: QString::arg(short,int,int,Katie::QChar)const
    // 16: QString::arg(uint,int,int,Katie::QChar)const
    // 17: QString::arg(ulong,int,int,Katie::QChar)const
    // 18: QString::arg(unsigned long long,int,int,Katie::QChar)const
    // 19: QString::arg(ushort,int,int,Katie::QChar)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 19; // arg(ushort,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 19; // arg(ushort,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 19; // arg(ushort,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 19; // arg(ushort,int,int,Katie::QChar)const
                }
            }
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 18; // arg(unsigned long long,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 18; // arg(unsigned long long,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 18; // arg(unsigned long long,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 18; // arg(unsigned long long,int,int,Katie::QChar)const
                }
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ulong>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 17; // arg(ulong,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 17; // arg(ulong,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 17; // arg(ulong,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 17; // arg(ulong,int,int,Katie::QChar)const
                }
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 16; // arg(uint,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 16; // arg(uint,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 16; // arg(uint,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 16; // arg(uint,int,int,Katie::QChar)const
                }
            }
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 14; // arg(long long,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 14; // arg(long long,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 14; // arg(long long,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 14; // arg(long long,int,int,Katie::QChar)const
                }
            }
        }
    } else if (PyFloat_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<double>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 11; // arg(double,int,char,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 11; // arg(double,int,char,int,Katie::QChar)const
            } else if (SbkChar_Check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 11; // arg(double,int,char,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[3])))) {
                    if (numArgs == 4) {
                        overloadId = 11; // arg(double,int,char,int,Katie::QChar)const
                    } else if ((pythonToCpp[4] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[4])))) {
                        overloadId = 11; // arg(double,int,char,int,Katie::QChar)const
                    }
                }
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<short>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 15; // arg(short,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 15; // arg(short,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 15; // arg(short,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 15; // arg(short,int,int,Katie::QChar)const
                }
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 13; // arg(long,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 13; // arg(long,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 13; // arg(long,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 13; // arg(long,int,int,Katie::QChar)const
                }
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 12; // arg(int,int,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 12; // arg(int,int,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 12; // arg(int,int,int,Katie::QChar)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3])))) {
                    overloadId = 12; // arg(int,int,int,Katie::QChar)const
                }
            }
        }
    } else if (SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // arg(char,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 1; // arg(char,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2])))) {
                overloadId = 1; // arg(char,int,Katie::QChar)const
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // arg(Katie::QChar,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // arg(Katie::QChar,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2])))) {
                overloadId = 0; // arg(Katie::QChar,int,Katie::QChar)const
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // arg(Katie::QString,int,Katie::QChar)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 3; // arg(Katie::QString,Katie::QString)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[2])))) {
                if (numArgs == 3) {
                    overloadId = 4; // arg(Katie::QString,Katie::QString,Katie::QString)const
                } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[3])))) {
                    if (numArgs == 4) {
                        overloadId = 5; // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                    } else if ((pythonToCpp[4] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[4])))) {
                        if (numArgs == 5) {
                            overloadId = 6; // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                        } else if ((pythonToCpp[5] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[5])))) {
                            if (numArgs == 6) {
                                overloadId = 7; // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                            } else if ((pythonToCpp[6] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[6])))) {
                                if (numArgs == 7) {
                                    overloadId = 8; // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                                } else if ((pythonToCpp[7] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[7])))) {
                                    if (numArgs == 8) {
                                        overloadId = 9; // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                                    } else if (numArgs == 9
                                        && (pythonToCpp[8] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[8])))) {
                                        overloadId = 10; // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 2; // arg(Katie::QString,int,Katie::QChar)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2])))) {
                overloadId = 2; // arg(Katie::QString,int,Katie::QChar)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_arg_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // arg(Katie::QChar a, int fieldWidth, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QChar cppArg2_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg2 = &cppArg2_local;
            if (pythonToCpp[2]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);
            }

            if (!PyErr_Occurred()) {
                // arg(Katie::QChar,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, *cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // arg(char a, int fieldWidth, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QChar cppArg2_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg2 = &cppArg2_local;
            if (pythonToCpp[2]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);
            }

            if (!PyErr_Occurred()) {
                // arg(char,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, *cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // arg(const Katie::QString & a, int fieldWidth, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
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
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QChar cppArg2_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg2 = &cppArg2_local;
            if (pythonToCpp[2]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);
            }

            if (!PyErr_Occurred()) {
                // arg(Katie::QString,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, cppArg1, *cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // arg(const Katie::QString & a1, const Katie::QString & a2) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 4: // arg(const Katie::QString & a1, const Katie::QString & a2, const Katie::QString & a3) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1, *cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 5: // arg(const Katie::QString & a1, const Katie::QString & a2, const Katie::QString & a3, const Katie::QString & a4) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QString cppArg3_local;
            ::Katie::QString* cppArg3 = &cppArg3_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1, *cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 6: // arg(const Katie::QString & a1, const Katie::QString & a2, const Katie::QString & a3, const Katie::QString & a4, const Katie::QString & a5) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QString cppArg3_local;
            ::Katie::QString* cppArg3 = &cppArg3_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!Shiboken::Object::isValid(pyArgs[4]))
                return {};
            ::Katie::QString cppArg4_local;
            ::Katie::QString* cppArg4 = &cppArg4_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[4]))
                pythonToCpp[4](pyArgs[4], &cppArg4_local);
            else
                pythonToCpp[4](pyArgs[4], &cppArg4);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1, *cppArg2, *cppArg3, *cppArg4);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 7: // arg(const Katie::QString & a1, const Katie::QString & a2, const Katie::QString & a3, const Katie::QString & a4, const Katie::QString & a5, const Katie::QString & a6) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QString cppArg3_local;
            ::Katie::QString* cppArg3 = &cppArg3_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!Shiboken::Object::isValid(pyArgs[4]))
                return {};
            ::Katie::QString cppArg4_local;
            ::Katie::QString* cppArg4 = &cppArg4_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[4]))
                pythonToCpp[4](pyArgs[4], &cppArg4_local);
            else
                pythonToCpp[4](pyArgs[4], &cppArg4);

            if (!Shiboken::Object::isValid(pyArgs[5]))
                return {};
            ::Katie::QString cppArg5_local;
            ::Katie::QString* cppArg5 = &cppArg5_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[5]))
                pythonToCpp[5](pyArgs[5], &cppArg5_local);
            else
                pythonToCpp[5](pyArgs[5], &cppArg5);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1, *cppArg2, *cppArg3, *cppArg4, *cppArg5);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 8: // arg(const Katie::QString & a1, const Katie::QString & a2, const Katie::QString & a3, const Katie::QString & a4, const Katie::QString & a5, const Katie::QString & a6, const Katie::QString & a7) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QString cppArg3_local;
            ::Katie::QString* cppArg3 = &cppArg3_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!Shiboken::Object::isValid(pyArgs[4]))
                return {};
            ::Katie::QString cppArg4_local;
            ::Katie::QString* cppArg4 = &cppArg4_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[4]))
                pythonToCpp[4](pyArgs[4], &cppArg4_local);
            else
                pythonToCpp[4](pyArgs[4], &cppArg4);

            if (!Shiboken::Object::isValid(pyArgs[5]))
                return {};
            ::Katie::QString cppArg5_local;
            ::Katie::QString* cppArg5 = &cppArg5_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[5]))
                pythonToCpp[5](pyArgs[5], &cppArg5_local);
            else
                pythonToCpp[5](pyArgs[5], &cppArg5);

            if (!Shiboken::Object::isValid(pyArgs[6]))
                return {};
            ::Katie::QString cppArg6_local;
            ::Katie::QString* cppArg6 = &cppArg6_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[6]))
                pythonToCpp[6](pyArgs[6], &cppArg6_local);
            else
                pythonToCpp[6](pyArgs[6], &cppArg6);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1, *cppArg2, *cppArg3, *cppArg4, *cppArg5, *cppArg6);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 9: // arg(const Katie::QString & a1, const Katie::QString & a2, const Katie::QString & a3, const Katie::QString & a4, const Katie::QString & a5, const Katie::QString & a6, const Katie::QString & a7, const Katie::QString & a8) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QString cppArg3_local;
            ::Katie::QString* cppArg3 = &cppArg3_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!Shiboken::Object::isValid(pyArgs[4]))
                return {};
            ::Katie::QString cppArg4_local;
            ::Katie::QString* cppArg4 = &cppArg4_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[4]))
                pythonToCpp[4](pyArgs[4], &cppArg4_local);
            else
                pythonToCpp[4](pyArgs[4], &cppArg4);

            if (!Shiboken::Object::isValid(pyArgs[5]))
                return {};
            ::Katie::QString cppArg5_local;
            ::Katie::QString* cppArg5 = &cppArg5_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[5]))
                pythonToCpp[5](pyArgs[5], &cppArg5_local);
            else
                pythonToCpp[5](pyArgs[5], &cppArg5);

            if (!Shiboken::Object::isValid(pyArgs[6]))
                return {};
            ::Katie::QString cppArg6_local;
            ::Katie::QString* cppArg6 = &cppArg6_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[6]))
                pythonToCpp[6](pyArgs[6], &cppArg6_local);
            else
                pythonToCpp[6](pyArgs[6], &cppArg6);

            if (!Shiboken::Object::isValid(pyArgs[7]))
                return {};
            ::Katie::QString cppArg7_local;
            ::Katie::QString* cppArg7 = &cppArg7_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[7]))
                pythonToCpp[7](pyArgs[7], &cppArg7_local);
            else
                pythonToCpp[7](pyArgs[7], &cppArg7);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1, *cppArg2, *cppArg3, *cppArg4, *cppArg5, *cppArg6, *cppArg7);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 10: // arg(const Katie::QString & a1, const Katie::QString & a2, const Katie::QString & a3, const Katie::QString & a4, const Katie::QString & a5, const Katie::QString & a6, const Katie::QString & a7, const Katie::QString & a8, const Katie::QString & a9) const
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QString cppArg3_local;
            ::Katie::QString* cppArg3 = &cppArg3_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!Shiboken::Object::isValid(pyArgs[4]))
                return {};
            ::Katie::QString cppArg4_local;
            ::Katie::QString* cppArg4 = &cppArg4_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[4]))
                pythonToCpp[4](pyArgs[4], &cppArg4_local);
            else
                pythonToCpp[4](pyArgs[4], &cppArg4);

            if (!Shiboken::Object::isValid(pyArgs[5]))
                return {};
            ::Katie::QString cppArg5_local;
            ::Katie::QString* cppArg5 = &cppArg5_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[5]))
                pythonToCpp[5](pyArgs[5], &cppArg5_local);
            else
                pythonToCpp[5](pyArgs[5], &cppArg5);

            if (!Shiboken::Object::isValid(pyArgs[6]))
                return {};
            ::Katie::QString cppArg6_local;
            ::Katie::QString* cppArg6 = &cppArg6_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[6]))
                pythonToCpp[6](pyArgs[6], &cppArg6_local);
            else
                pythonToCpp[6](pyArgs[6], &cppArg6);

            if (!Shiboken::Object::isValid(pyArgs[7]))
                return {};
            ::Katie::QString cppArg7_local;
            ::Katie::QString* cppArg7 = &cppArg7_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[7]))
                pythonToCpp[7](pyArgs[7], &cppArg7_local);
            else
                pythonToCpp[7](pyArgs[7], &cppArg7);

            if (!Shiboken::Object::isValid(pyArgs[8]))
                return {};
            ::Katie::QString cppArg8_local;
            ::Katie::QString* cppArg8 = &cppArg8_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[8]))
                pythonToCpp[8](pyArgs[8], &cppArg8_local);
            else
                pythonToCpp[8](pyArgs[8], &cppArg8);


            if (!PyErr_Occurred()) {
                // arg(Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString,Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(*cppArg0, *cppArg1, *cppArg2, *cppArg3, *cppArg4, *cppArg5, *cppArg6, *cppArg7, *cppArg8);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 11: // arg(double a, int fieldWidth, char fmt, int prec, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fmt");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fmt'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!SbkChar_Check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "prec");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'prec'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[4]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[4] = value;
                    if (!(pythonToCpp[4] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[4]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            double cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            char cppArg2 = 'g';
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            int cppArg3 = -1;
            if (pythonToCpp[3]) pythonToCpp[3](pyArgs[3], &cppArg3);
            if (!Shiboken::Object::isValid(pyArgs[4]))
                return {};
            ::Katie::QChar cppArg4_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg4 = &cppArg4_local;
            if (pythonToCpp[4]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[4]))
                pythonToCpp[4](pyArgs[4], &cppArg4_local);
            else
                pythonToCpp[4](pyArgs[4], &cppArg4);
            }

            if (!PyErr_Occurred()) {
                // arg(double,int,char,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, cppArg3, *cppArg4);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 12: // arg(int a, int fieldWidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(int,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 13: // arg(long a, int fieldwidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldwidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldwidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(long,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 14: // arg(long long a, int fieldwidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldwidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldwidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(long long,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 15: // arg(short a, int fieldWidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            short cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(short,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 16: // arg(uint a, int fieldWidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(uint,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 17: // arg(ulong a, int fieldwidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldwidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldwidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            ::ulong cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(ulong,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 18: // arg(unsigned long long a, int fieldwidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldwidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldwidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            unsigned long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(unsigned long long,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 19: // arg(ushort a, int fieldWidth, int base, const Katie::QChar & fillChar) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "fieldWidth");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fieldWidth'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
                value = PyDict_GetItemString(kwds, "fillChar");
                if (value && pyArgs[3]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.arg(): got multiple values for keyword argument 'fillChar'.");
                    return {};
                } else if (value) {
                    pyArgs[3] = value;
                    if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[3]))))
                        goto Sbk_Katie_QStringFunc_arg_TypeError;
                }
            }
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = 10;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
            if (!Shiboken::Object::isValid(pyArgs[3]))
                return {};
            ::Katie::QChar cppArg3_local = QLatin1Char(' ');
            ::Katie::QChar* cppArg3 = &cppArg3_local;
            if (pythonToCpp[3]) {
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[3]))
                pythonToCpp[3](pyArgs[3], &cppArg3_local);
            else
                pythonToCpp[3](pyArgs[3], &cppArg3);
            }

            if (!PyErr_Occurred()) {
                // arg(ushort,int,int,Katie::QChar)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->arg(cppArg0, cppArg1, cppArg2, *cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_arg_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.arg");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_at(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::at(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // at(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_at_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // at(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            const Katie::QChar cppResult = const_cast<const ::Katie::QString*>(cppSelf)->at(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_at_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.at");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_begin(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // begin()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QChar * cppResult = cppSelf->begin();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_capacity(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // capacity()const
            int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->capacity();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_chop(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::chop(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // chop(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_chop_TypeError;

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

    Sbk_Katie_QStringFunc_chop_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.chop");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_clear(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));

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

static PyObject* Sbk_Katie_QStringFunc_compare(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    }
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.compare(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.compare(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:compare", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::compare(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
    // 1: static QString::compare(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
    // 2: QString::compare(Katie::QString,Katie::Qt::CaseSensitivity)const
    // 3: static QString::compare(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
    // 4: static QString::compare(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // compare(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 1; // compare(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 1; // compare(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 0; // compare(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // compare(Katie::QString,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 3; // compare(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 3; // compare(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 4; // compare(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 4; // compare(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 2; // compare(Katie::QString,Katie::Qt::CaseSensitivity)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_compare_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // compare(const Katie::QLatin1String & other, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.compare(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_compare_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // compare(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->compare(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // compare(const Katie::QLatin1String & s1, const Katie::QString & s2, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.compare(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_compare_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
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
                // compare(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = ::Katie::QString::compare(*cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // compare(const Katie::QString & s, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.compare(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_compare_TypeError;
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
                // compare(Katie::QString,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->compare(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 3: // compare(const Katie::QString & s1, const Katie::QLatin1String & s2, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.compare(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_compare_TypeError;
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
            ::Katie::QLatin1String cppArg1_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // compare(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = ::Katie::QString::compare(*cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 4: // compare(const Katie::QString & s1, const Katie::QString & s2, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.compare(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_compare_TypeError;
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
                // compare(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = ::Katie::QString::compare(*cppArg0, *cppArg1, cppArg2);
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

    Sbk_Katie_QStringFunc_compare_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.compare");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_constBegin(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // constBegin()const
            const Katie::QChar * cppResult = const_cast<const ::Katie::QString*>(cppSelf)->constBegin();
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_constData(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // constData()const
            const Katie::QChar * cppResult = const_cast<const ::Katie::QString*>(cppSelf)->constData();
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_constEnd(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // constEnd()const
            const Katie::QChar * cppResult = const_cast<const ::Katie::QString*>(cppSelf)->constEnd();
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_contains(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.contains(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.contains(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:contains", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::contains(Katie::QChar,Katie::Qt::CaseSensitivity)const
    // 1: QString::contains(Katie::QRegExp&)const
    // 2: QString::contains(Katie::QRegExp)const
    // 3: QString::contains(Katie::QString,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        overloadId = 1; // contains(Katie::QRegExp&)const
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // contains(Katie::QChar,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 0; // contains(Katie::QChar,Katie::Qt::CaseSensitivity)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // contains(Katie::QString,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 3; // contains(Katie::QString,Katie::Qt::CaseSensitivity)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_contains_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // contains(Katie::QChar c, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.contains(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_contains_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // contains(Katie::QChar,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->contains(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // contains(Katie::QRegExp & rx) const
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // contains(Katie::QRegExp&)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->contains(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 2: // contains(const Katie::QRegExp & rx) const
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // contains(Katie::QRegExp)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->contains(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 3: // contains(const Katie::QString & s, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.contains(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_contains_TypeError;
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
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->contains(*cppArg0, cppArg1);
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

    Sbk_Katie_QStringFunc_contains_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.contains");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_count(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.count(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:count", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::count()const
    // 1: QString::count(Katie::QChar,Katie::Qt::CaseSensitivity)const
    // 2: QString::count(Katie::QRegExp)const
    // 3: QString::count(Katie::QString,Katie::Qt::CaseSensitivity)const
    if (numArgs == 0) {
        overloadId = 0; // count()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        overloadId = 2; // count(Katie::QRegExp)const
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // count(Katie::QChar,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 1; // count(Katie::QChar,Katie::Qt::CaseSensitivity)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // count(Katie::QString,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 3; // count(Katie::QString,Katie::Qt::CaseSensitivity)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_count_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // count() const
        {

            if (!PyErr_Occurred()) {
                // count()const
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->count();
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // count(Katie::QChar c, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.count(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_count_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // count(Katie::QChar,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->count(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // count(const Katie::QRegExp & arg__1) const
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // count(Katie::QRegExp)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->count(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 3: // count(const Katie::QString & s, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.count(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_count_TypeError;
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
                // count(Katie::QString,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->count(*cppArg0, cppArg1);
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

    Sbk_Katie_QStringFunc_count_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.count");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_data(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // data()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QChar * cppResult = cppSelf->data();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_detach(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));

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

static PyObject* Sbk_Katie_QStringFunc_end(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // end()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QChar * cppResult = cppSelf->end();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_endsWith(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.endsWith(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.endsWith(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:endsWith", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::endsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
    // 1: QString::endsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
    // 2: QString::endsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // endsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 1; // endsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // endsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 0; // endsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // endsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 2; // endsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_endsWith_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // endsWith(const Katie::QChar & c, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.endsWith(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_endsWith_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0_local;
            ::Katie::QChar* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // endsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->endsWith(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // endsWith(const Katie::QLatin1String & s, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.endsWith(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_endsWith_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // endsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->endsWith(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 2: // endsWith(const Katie::QString & s, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.endsWith(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_endsWith_TypeError;
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
                // endsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->endsWith(*cppArg0, cppArg1);
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

    Sbk_Katie_QStringFunc_endsWith_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.endsWith");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fill(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fill(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fill(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fill", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::fill(Katie::QChar,int)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fill(Katie::QChar,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fill(Katie::QChar,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fill_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fill(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_fill_TypeError;
            }
        }
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return {};
        ::Katie::QChar cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fill(Katie::QChar,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString & cppResult = cppSelf->fill(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fill_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fill");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromAscii(PyObject* self, PyObject* args, PyObject* kwds)
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromAscii(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromAscii(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fromAscii", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QString::fromAscii(const char*,int)
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fromAscii(const char*,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fromAscii(const char*,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromAscii_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromAscii(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_fromAscii_TypeError;
            }
        }
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromAscii(const char*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromAscii(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromAscii_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fromAscii");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromLatin1(PyObject* self, PyObject* args, PyObject* kwds)
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromLatin1(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromLatin1(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fromLatin1", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QString::fromLatin1(const char*,int)
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fromLatin1(const char*,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fromLatin1(const char*,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromLatin1_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromLatin1(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_fromLatin1_TypeError;
            }
        }
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromLatin1(const char*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromLatin1(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromLatin1_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fromLatin1");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromLocal8Bit(PyObject* self, PyObject* args, PyObject* kwds)
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromLocal8Bit(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromLocal8Bit(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fromLocal8Bit", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QString::fromLocal8Bit(const char*,int)
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fromLocal8Bit(const char*,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fromLocal8Bit(const char*,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromLocal8Bit_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromLocal8Bit(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_fromLocal8Bit_TypeError;
            }
        }
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromLocal8Bit(const char*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromLocal8Bit(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromLocal8Bit_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fromLocal8Bit");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromRawData(PyObject* self, PyObject* args)
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
    // 0: static QString::fromRawData(const Katie::QChar*,int)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 0; // fromRawData(const Katie::QChar*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromRawData_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return {};
        ::Katie::QChar* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromRawData(const Katie::QChar*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromRawData(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromRawData_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fromRawData");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromStdString(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QString::fromStdString(std::string)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<std::string>(), (pyArg)))) {
        overloadId = 0; // fromStdString(std::string)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromStdString_TypeError;

    // Call function/method
    {
        ::std::string cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // fromStdString(std::string)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromStdString(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromStdString_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.fromStdString");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromUcs4(PyObject* self, PyObject* args, PyObject* kwds)
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUcs4(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUcs4(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fromUcs4", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QString::fromUcs4(const uint*,int)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fromUcs4(const uint*,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fromUcs4(const uint*,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromUcs4_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUcs4(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_fromUcs4_TypeError;
            }
        }
        uint* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromUcs4(const uint*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromUcs4(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromUcs4_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fromUcs4");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromUtf16(PyObject* self, PyObject* args, PyObject* kwds)
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUtf16(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUtf16(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fromUtf16", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QString::fromUtf16(const ushort*,int)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fromUtf16(const ushort*,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fromUtf16(const ushort*,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromUtf16_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUtf16(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_fromUtf16_TypeError;
            }
        }
        ::ushort* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromUtf16(const ushort*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromUtf16(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromUtf16_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fromUtf16");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_fromUtf8(PyObject* self, PyObject* args, PyObject* kwds)
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUtf8(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUtf8(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:fromUtf8", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QString::fromUtf8(const char*,int)
    if (Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // fromUtf8(const char*,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // fromUtf8(const char*,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_fromUtf8_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "size");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.fromUtf8(): got multiple values for keyword argument 'size'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_fromUtf8_TypeError;
            }
        }
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // fromUtf8(const char*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QString::fromUtf8(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_fromUtf8_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.fromUtf8");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_indexOf(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:indexOf", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::indexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
    // 1: QString::indexOf(Katie::QRegExp&,int)const
    // 2: QString::indexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
    // 3: QString::indexOf(Katie::QRegExp,int)const
    // 4: QString::indexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // indexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 2; // indexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 2; // indexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // indexOf(Katie::QRegExp&,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 1; // indexOf(Katie::QRegExp&,int)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // indexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // indexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 0; // indexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 4; // indexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 4; // indexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 4; // indexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_indexOf_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // indexOf(Katie::QChar c, int from, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // indexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->indexOf(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // indexOf(Katie::QRegExp & arg__1, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
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
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->indexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // indexOf(const Katie::QLatin1String & s, int from, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            int cppArg1 = 0;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // indexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->indexOf(*cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 3: // indexOf(const Katie::QRegExp & arg__1, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
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
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->indexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 4: // indexOf(const Katie::QString & s, int from, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.indexOf(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_indexOf_TypeError;
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
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // indexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->indexOf(*cppArg0, cppArg1, cppArg2);
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

    Sbk_Katie_QStringFunc_indexOf_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.indexOf");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_insert(PyObject* self, PyObject* args)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
    // 0: QString::insert(int,Katie::QChar)
    // 1: QString::insert(int,const Katie::QChar*,int)
    // 2: QString::insert(int,Katie::QLatin1String)
    // 3: QString::insert(int,Katie::QString)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[1])))) {
            overloadId = 2; // insert(int,Katie::QLatin1String)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // insert(int,Katie::QChar)
            } else if (numArgs == 3
                && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
                overloadId = 1; // insert(int,const Katie::QChar*,int)
            }
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            overloadId = 3; // insert(int,Katie::QString)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_insert_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // insert(int i, Katie::QChar c)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QChar cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // insert(int,Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->insert(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // insert(int i, const Katie::QChar * uc, int len)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QChar* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // insert(int,const Katie::QChar*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->insert(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // insert(int i, const Katie::QLatin1String & s)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QLatin1String cppArg1_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // insert(int,Katie::QLatin1String)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->insert(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // insert(int i, const Katie::QString & s)
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
                Katie::QString & cppResult = cppSelf->insert(cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_insert_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.insert");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_isDetached(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isDetached()const
            bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->isDetached();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_isEmpty(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isEmpty()const
            bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->isEmpty();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_isNull(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isNull()const
            bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->isNull();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_isRightToLeft(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isRightToLeft()const
            bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->isRightToLeft();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_isSharedWith(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::isSharedWith(Katie::QString)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // isSharedWith(Katie::QString)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_isSharedWith_TypeError;

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
            // isSharedWith(Katie::QString)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->isSharedWith(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_isSharedWith_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.isSharedWith");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_isSimpleText(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isSimpleText()const
            bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->isSimpleText();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_lastIndexOf(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:lastIndexOf", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::lastIndexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
    // 1: QString::lastIndexOf(Katie::QRegExp&,int)const
    // 2: QString::lastIndexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
    // 3: QString::lastIndexOf(Katie::QRegExp,int)const
    // 4: QString::lastIndexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // lastIndexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 2; // lastIndexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 2; // lastIndexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // lastIndexOf(Katie::QRegExp&,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 1; // lastIndexOf(Katie::QRegExp&,int)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // lastIndexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // lastIndexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 0; // lastIndexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 4; // lastIndexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 4; // lastIndexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 4; // lastIndexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // lastIndexOf(Katie::QChar c, int from, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // lastIndexOf(Katie::QChar,int,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->lastIndexOf(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // lastIndexOf(Katie::QRegExp & arg__1, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
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
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // lastIndexOf(const Katie::QLatin1String & s, int from, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            int cppArg1 = -1;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // lastIndexOf(Katie::QLatin1String,int,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 3: // lastIndexOf(const Katie::QRegExp & arg__1, int from) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
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
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 4: // lastIndexOf(const Katie::QString & s, int from, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "from");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'from'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.lastIndexOf(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_lastIndexOf_TypeError;
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
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // lastIndexOf(Katie::QString,int,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->lastIndexOf(*cppArg0, cppArg1, cppArg2);
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

    Sbk_Katie_QStringFunc_lastIndexOf_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.lastIndexOf");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_left(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::left(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // left(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_left_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // left(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->left(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_left_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.left");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_leftJustified(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.leftJustified(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.leftJustified(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:leftJustified", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::leftJustified(int,Katie::QChar,bool)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // leftJustified(int,Katie::QChar,bool)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // leftJustified(int,Katie::QChar,bool)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2])))) {
                overloadId = 0; // leftJustified(int,Katie::QChar,bool)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_leftJustified_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "fill");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.leftJustified(): got multiple values for keyword argument 'fill'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_leftJustified_TypeError;
            }
            value = PyDict_GetItemString(kwds, "trunc");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.leftJustified(): got multiple values for keyword argument 'trunc'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2]))))
                    goto Sbk_Katie_QStringFunc_leftJustified_TypeError;
            }
        }
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        if (!Shiboken::Object::isValid(pyArgs[1]))
            return {};
        ::Katie::QChar cppArg1 = QLatin1Char(' ');
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
        bool cppArg2 = false;
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // leftJustified(int,Katie::QChar,bool)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->leftJustified(cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_leftJustified_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.leftJustified");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_length(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // length()const
            int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->length();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_localeAwareCompare(PyObject* self, PyObject* args)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "localeAwareCompare", 1, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::localeAwareCompare(Katie::QString)const
    // 1: static QString::localeAwareCompare(Katie::QString,Katie::QString)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // localeAwareCompare(Katie::QString)const
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            overloadId = 1; // localeAwareCompare(Katie::QString,Katie::QString)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_localeAwareCompare_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // localeAwareCompare(const Katie::QString & s) const
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
                // localeAwareCompare(Katie::QString)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->localeAwareCompare(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // localeAwareCompare(const Katie::QString & s1, const Katie::QString & s2)
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
            ::Katie::QString cppArg1_local;
            ::Katie::QString* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);


            if (!PyErr_Occurred()) {
                // localeAwareCompare(Katie::QString,Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = ::Katie::QString::localeAwareCompare(*cppArg0, *cppArg1);
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

    Sbk_Katie_QStringFunc_localeAwareCompare_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.localeAwareCompare");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_mid(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.mid(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.mid(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:mid", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::mid(int,int)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // mid(int,int)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 0; // mid(int,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_mid_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "n");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.mid(): got multiple values for keyword argument 'n'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_mid_TypeError;
            }
        }
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = -1;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // mid(int,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->mid(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_mid_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.mid");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_normalized(PyObject* self, PyObject* args)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "normalized", 1, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::normalized(Katie::QString::NormalizationForm)const
    // 1: QString::normalized(Katie::QString::NormalizationForm,Katie::QChar::UnicodeVersion)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX])->converter, (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // normalized(Katie::QString::NormalizationForm)const
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX])->converter, (pyArgs[1])))) {
            overloadId = 1; // normalized(Katie::QString::NormalizationForm,Katie::QChar::UnicodeVersion)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_normalized_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // normalized(Katie::QString::NormalizationForm mode) const
        {
            ::Katie::QString::NormalizationForm cppArg0{Katie::QString::NormalizationForm_D};
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // normalized(Katie::QString::NormalizationForm)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->normalized(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // normalized(Katie::QString::NormalizationForm mode, Katie::QChar::UnicodeVersion version) const
        {
            ::Katie::QString::NormalizationForm cppArg0{Katie::QString::NormalizationForm_D};
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::Katie::QChar::UnicodeVersion cppArg1{Katie::QChar::Unicode_Unassigned};
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // normalized(Katie::QString::NormalizationForm,Katie::QChar::UnicodeVersion)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->normalized(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_normalized_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.normalized");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_number(PyObject* self, PyObject* args, PyObject* kwds)
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:number", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: static QString::number(double,char,int)
    // 1: static QString::number(int,int)
    // 2: static QString::number(long,int)
    // 3: static QString::number(long long,int)
    // 4: static QString::number(uint,int)
    // 5: static QString::number(ulong,int)
    // 6: static QString::number(unsigned long long,int)
    if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 6; // number(unsigned long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 6; // number(unsigned long long,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ulong>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 5; // number(ulong,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 5; // number(ulong,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 4; // number(uint,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 4; // number(uint,int)
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // number(long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // number(long long,int)
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
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // number(long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 2; // number(long,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // number(int,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 1; // number(int,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_number_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // number(double arg__1, char f, int prec)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "f");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'f'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
                }
                value = PyDict_GetItemString(kwds, "prec");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'prec'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
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
                Katie::QString cppResult = ::Katie::QString::number(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // number(int arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
                }
            }
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(int,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = ::Katie::QString::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // number(long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
                }
            }
            long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = ::Katie::QString::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // number(long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
                }
            }
            long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = ::Katie::QString::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 4: // number(uint arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
                }
            }
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(uint,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = ::Katie::QString::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 5: // number(ulong arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
                }
            }
            ::ulong cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(ulong,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = ::Katie::QString::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 6: // number(unsigned long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.number(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_number_TypeError;
                }
            }
            unsigned long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // number(unsigned long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = ::Katie::QString::number(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_number_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.number");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_prepend(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::prepend(Katie::QChar)
    // 1: QString::prepend(Katie::QByteArray)
    // 2: QString::prepend(Katie::QLatin1String)
    // 3: QString::prepend(Katie::QString)
    // 4: QString::prepend(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 4; // prepend(const char*)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
        overloadId = 2; // prepend(Katie::QLatin1String)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 1; // prepend(Katie::QByteArray)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArg)))) {
        overloadId = 0; // prepend(Katie::QChar)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 3; // prepend(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_prepend_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // prepend(Katie::QChar c)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // prepend(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->prepend(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // prepend(const Katie::QByteArray & s)
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
                // prepend(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->prepend(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // prepend(const Katie::QLatin1String & s)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // prepend(Katie::QLatin1String)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->prepend(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // prepend(const Katie::QString & s)
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
                // prepend(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->prepend(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 4: // prepend(const char * s)
        {
            const char* cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // prepend(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->prepend(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_prepend_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.prepend");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_push_back(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::push_back(Katie::QChar)
    // 1: QString::push_back(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArg)))) {
        overloadId = 0; // push_back(Katie::QChar)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 1; // push_back(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_push_back_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // push_back(Katie::QChar c)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // push_back(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_back(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // push_back(const Katie::QString & s)
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
                // push_back(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_back(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QStringFunc_push_back_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.push_back");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_push_front(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::push_front(Katie::QChar)
    // 1: QString::push_front(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArg)))) {
        overloadId = 0; // push_front(Katie::QChar)
    } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 1; // push_front(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_push_front_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // push_front(Katie::QChar c)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // push_front(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_front(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // push_front(const Katie::QString & s)
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
                // push_front(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cppSelf->push_front(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QStringFunc_push_front_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.push_front");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_remove(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.remove(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.remove(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:remove", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::remove(Katie::QChar,Katie::Qt::CaseSensitivity)
    // 1: QString::remove(Katie::QRegExp)
    // 2: QString::remove(Katie::QString,Katie::Qt::CaseSensitivity)
    // 3: QString::remove(int,int)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 3; // remove(int,int)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        overloadId = 1; // remove(Katie::QRegExp)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // remove(Katie::QChar,Katie::Qt::CaseSensitivity)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 0; // remove(Katie::QChar,Katie::Qt::CaseSensitivity)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // remove(Katie::QString,Katie::Qt::CaseSensitivity)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 2; // remove(Katie::QString,Katie::Qt::CaseSensitivity)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_remove_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // remove(Katie::QChar c, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.remove(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_remove_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // remove(Katie::QChar,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->remove(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // remove(const Katie::QRegExp & rx)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // remove(Katie::QRegExp)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->remove(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // remove(const Katie::QString & s, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.remove(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_remove_TypeError;
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
                // remove(Katie::QString,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->remove(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // remove(int i, int len)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // remove(int,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->remove(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_remove_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.remove");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_repeated(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::repeated(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // repeated(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_repeated_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // repeated(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->repeated(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_repeated_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.repeated");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_replace(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): too many arguments");
        return {};
    } else if (numArgs < 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOOOO:replace", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2]), &(pyArgs[3]), &(pyArgs[4])))
        return {};


    // Overloaded function decisor
    // 0: QString::replace(Katie::QChar,Katie::QChar,Katie::Qt::CaseSensitivity)
    // 1: QString::replace(Katie::QChar,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
    // 2: QString::replace(Katie::QChar,Katie::QString,Katie::Qt::CaseSensitivity)
    // 3: QString::replace(const Katie::QChar*,int,const Katie::QChar*,int,Katie::Qt::CaseSensitivity)
    // 4: QString::replace(Katie::QLatin1String,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
    // 5: QString::replace(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
    // 6: QString::replace(Katie::QRegExp,Katie::QString)
    // 7: QString::replace(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
    // 8: QString::replace(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
    // 9: QString::replace(int,int,Katie::QChar)
    // 10: QString::replace(int,int,const Katie::QChar*,int)
    // 11: QString::replace(int,int,Katie::QString)
    if (numArgs >= 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2])))) {
            if (numArgs == 3) {
                overloadId = 9; // replace(int,int,Katie::QChar)
            } else if (numArgs == 4
                && (pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[3])))) {
                overloadId = 10; // replace(int,int,const Katie::QChar*,int)
            }
        } else if (numArgs == 3
            && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[2])))) {
            overloadId = 11; // replace(int,int,Katie::QString)
        }
    } else if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
        overloadId = 6; // replace(Katie::QRegExp,Katie::QString)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 4; // replace(Katie::QLatin1String,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 4; // replace(Katie::QLatin1String,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 5; // replace(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 5; // replace(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs >= 4
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))
            && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[2])))
            && (pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[3])))) {
            if (numArgs == 4) {
                overloadId = 3; // replace(const Katie::QChar*,int,const Katie::QChar*,int,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[4] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[4])))) {
                overloadId = 3; // replace(const Katie::QChar*,int,const Katie::QChar*,int,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 1; // replace(Katie::QChar,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 1; // replace(Katie::QChar,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // replace(Katie::QChar,Katie::QChar,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 0; // replace(Katie::QChar,Katie::QChar,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 2; // replace(Katie::QChar,Katie::QString,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 2; // replace(Katie::QChar,Katie::QString,Katie::Qt::CaseSensitivity)
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 7; // replace(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 7; // replace(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
            }
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 8; // replace(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 8; // replace(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_replace_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // replace(Katie::QChar before, Katie::QChar after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QChar cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // replace(Katie::QChar,Katie::QChar,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // replace(Katie::QChar c, const Katie::QLatin1String & after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QLatin1String cppArg1_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // replace(Katie::QChar,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // replace(Katie::QChar c, const Katie::QString & after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
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
                // replace(Katie::QChar,Katie::QString,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // replace(const Katie::QChar * before, int blen, const Katie::QChar * after, int alen, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[4]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[4] = value;
                    if (!(pythonToCpp[4] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[4]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QChar* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);
            int cppArg3;
            pythonToCpp[3](pyArgs[3], &cppArg3);
            ::Katie::Qt::CaseSensitivity cppArg4 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[4]) pythonToCpp[4](pyArgs[4], &cppArg4);

            if (!PyErr_Occurred()) {
                // replace(const Katie::QChar*,int,const Katie::QChar*,int,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(cppArg0, cppArg1, cppArg2, cppArg3, cppArg4);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 4: // replace(const Katie::QLatin1String & before, const Katie::QLatin1String & after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QLatin1String cppArg1_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // replace(Katie::QLatin1String,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(*cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 5: // replace(const Katie::QLatin1String & before, const Katie::QString & after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
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
                // replace(Katie::QLatin1String,Katie::QString,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(*cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 6: // replace(const Katie::QRegExp & rx, const Katie::QString & after)
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
                // replace(Katie::QRegExp,Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(*cppArg0, *cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 7: // replace(const Katie::QString & before, const Katie::QLatin1String & after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
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
            ::Katie::QLatin1String cppArg1_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg1 = &cppArg1_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[1]))
                pythonToCpp[1](pyArgs[1], &cppArg1_local);
            else
                pythonToCpp[1](pyArgs[1], &cppArg1);

            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // replace(Katie::QString,Katie::QLatin1String,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(*cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 8: // replace(const Katie::QString & before, const Katie::QString & after, Katie::Qt::CaseSensitivity cs)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.replace(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_replace_TypeError;
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
                // replace(Katie::QString,Katie::QString,Katie::Qt::CaseSensitivity)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(*cppArg0, *cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 9: // replace(int i, int len, Katie::QChar after)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QChar cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // replace(int,int,Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 10: // replace(int i, int len, const Katie::QChar * s, int slen)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QChar* cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);
            int cppArg3;
            pythonToCpp[3](pyArgs[3], &cppArg3);

            if (!PyErr_Occurred()) {
                // replace(int,int,const Katie::QChar*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(cppArg0, cppArg1, cppArg2, cppArg3);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 11: // replace(int i, int len, const Katie::QString & after)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            if (!Shiboken::Object::isValid(pyArgs[2]))
                return {};
            ::Katie::QString cppArg2_local;
            ::Katie::QString* cppArg2 = &cppArg2_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), pythonToCpp[2]))
                pythonToCpp[2](pyArgs[2], &cppArg2_local);
            else
                pythonToCpp[2](pyArgs[2], &cppArg2);


            if (!PyErr_Occurred()) {
                // replace(int,int,Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->replace(cppArg0, cppArg1, *cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_replace_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.replace");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_reserve(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::reserve(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // reserve(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_reserve_TypeError;

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

    Sbk_Katie_QStringFunc_reserve_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.reserve");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_resize(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::resize(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // resize(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_resize_TypeError;

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

    Sbk_Katie_QStringFunc_resize_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.resize");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_right(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::right(int)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // right(int)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_right_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // right(int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->right(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_right_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.right");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_rightJustified(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.rightJustified(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.rightJustified(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:rightJustified", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::rightJustified(int,Katie::QChar,bool)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // rightJustified(int,Katie::QChar,bool)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // rightJustified(int,Katie::QChar,bool)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2])))) {
                overloadId = 0; // rightJustified(int,Katie::QChar,bool)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_rightJustified_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "fill");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.rightJustified(): got multiple values for keyword argument 'fill'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_rightJustified_TypeError;
            }
            value = PyDict_GetItemString(kwds, "trunc");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.rightJustified(): got multiple values for keyword argument 'trunc'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[2]))))
                    goto Sbk_Katie_QStringFunc_rightJustified_TypeError;
            }
        }
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        if (!Shiboken::Object::isValid(pyArgs[1]))
            return {};
        ::Katie::QChar cppArg1 = QLatin1Char(' ');
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
        bool cppArg2 = false;
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

        if (!PyErr_Occurred()) {
            // rightJustified(int,Katie::QChar,bool)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->rightJustified(cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_rightJustified_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.rightJustified");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_section(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.section(): too many arguments");
        return {};
    } else if (numArgs < 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.section(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:section", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::section(Katie::QChar,int,int)const
    // 1: QString::section(Katie::QRegExp,int,int)const
    // 2: QString::section(Katie::QString,int,int)const
    if (numArgs >= 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        if (numArgs == 2) {
            overloadId = 1; // section(Katie::QRegExp,int,int)const
        } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
            overloadId = 1; // section(Katie::QRegExp,int,int)const
        }
    } else if (numArgs >= 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        if (numArgs == 2) {
            overloadId = 0; // section(Katie::QChar,int,int)const
        } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
            overloadId = 0; // section(Katie::QChar,int,int)const
        }
    } else if (numArgs >= 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        if (numArgs == 2) {
            overloadId = 2; // section(Katie::QString,int,int)const
        } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2])))) {
            overloadId = 2; // section(Katie::QString,int,int)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_section_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // section(Katie::QChar sep, int start, int end) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "end");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.section(): got multiple values for keyword argument 'end'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_section_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = -1;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // section(Katie::QChar,int,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->section(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // section(const Katie::QRegExp & reg, int start, int end) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "end");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.section(): got multiple values for keyword argument 'end'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_section_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = -1;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // section(Katie::QRegExp,int,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->section(*cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // section(const Katie::QString & in_sep, int start, int end) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "end");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.section(): got multiple values for keyword argument 'end'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_section_TypeError;
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

            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            int cppArg2 = -1;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // section(Katie::QString,int,int)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->section(*cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_section_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.section");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_setNum(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:setNum", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::setNum(double,char,int)
    // 1: QString::setNum(float,char,int)
    // 2: QString::setNum(int,int)
    // 3: QString::setNum(long,int)
    // 4: QString::setNum(long long,int)
    // 5: QString::setNum(short,int)
    // 6: QString::setNum(uint,int)
    // 7: QString::setNum(ulong,int)
    // 8: QString::setNum(unsigned long long,int)
    // 9: QString::setNum(ushort,int)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 9; // setNum(ushort,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 9; // setNum(ushort,int)
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 8; // setNum(unsigned long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 8; // setNum(unsigned long long,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ulong>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 7; // setNum(ulong,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 7; // setNum(ulong,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 6; // setNum(uint,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 6; // setNum(uint,int)
        }
    } else if (PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 4; // setNum(long long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 4; // setNum(long long,int)
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
            overloadId = 5; // setNum(short,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 5; // setNum(short,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 3; // setNum(long,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 3; // setNum(long,int)
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // setNum(int,int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 2; // setNum(int,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_setNum_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // setNum(double arg__1, char f, int prec)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "f");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'f'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
                value = PyDict_GetItemString(kwds, "prec");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'prec'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
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
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // setNum(float arg__1, char f, int prec)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "f");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'f'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
                value = PyDict_GetItemString(kwds, "prec");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'prec'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
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
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 2: // setNum(int arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(int,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 3: // setNum(long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 4: // setNum(long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 5: // setNum(short arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            short cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(short,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 6: // setNum(uint arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(uint,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 7: // setNum(ulong arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            ::ulong cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(ulong,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 8: // setNum(unsigned long long arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            unsigned long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(unsigned long long,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 9: // setNum(ushort arg__1, int base)
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "base");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.setNum(): got multiple values for keyword argument 'base'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_setNum_TypeError;
                }
            }
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1 = 10;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // setNum(ushort,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString & cppResult = cppSelf->setNum(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_setNum_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.setNum");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_setRawData(PyObject* self, PyObject* args)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
    // 0: QString::setRawData(const Katie::QChar*,int)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 0; // setRawData(const Katie::QChar*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_setRawData_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return {};
        ::Katie::QChar* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // setRawData(const Katie::QChar*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString & cppResult = cppSelf->setRawData(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_setRawData_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.setRawData");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_setUnicode(PyObject* self, PyObject* args)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "setUnicode", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::setUnicode(const Katie::QChar*,int)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 0; // setUnicode(const Katie::QChar*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_setUnicode_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArgs[0]))
            return {};
        ::Katie::QChar* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // setUnicode(const Katie::QChar*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString & cppResult = cppSelf->setUnicode(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_setUnicode_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.setUnicode");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_setUtf16(PyObject* self, PyObject* args)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "setUtf16", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::setUtf16(const ushort*,int)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 0; // setUtf16(const ushort*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_setUtf16_TypeError;

    // Call function/method
    {
        ::ushort* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // setUtf16(const ushort*,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString & cppResult = cppSelf->setUtf16(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_setUtf16_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.setUtf16");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_simplified(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // simplified()const
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->simplified();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_size(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // size()const
            int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->size();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_split(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.split(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.split(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOO:split", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return {};


    // Overloaded function decisor
    // 0: QString::split(Katie::QChar,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
    // 1: QString::split(Katie::QRegExp,Katie::QString::SplitBehavior)const
    // 2: QString::split(Katie::QString,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // split(Katie::QRegExp,Katie::QString::SplitBehavior)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX])->converter, (pyArgs[1])))) {
            overloadId = 1; // split(Katie::QRegExp,Katie::QString::SplitBehavior)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // split(Katie::QChar,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX])->converter, (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 0; // split(Katie::QChar,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 0; // split(Katie::QChar,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
            }
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // split(Katie::QString,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX])->converter, (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 2; // split(Katie::QString,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
            } else if ((pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2])))) {
                overloadId = 2; // split(Katie::QString,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_split_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // split(const Katie::QChar & sep, Katie::QString::SplitBehavior behavior, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "behavior");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.split(): got multiple values for keyword argument 'behavior'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_split_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.split(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_split_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0_local;
            ::Katie::QChar* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            ::Katie::QString::SplitBehavior cppArg1 = Katie::QString::SplitBehavior::KeepEmptyParts;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // split(Katie::QChar,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList cppResult = const_cast<const ::Katie::QString*>(cppSelf)->split(*cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
        case 1: // split(const Katie::QRegExp & sep, Katie::QString::SplitBehavior behavior) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "behavior");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.split(): got multiple values for keyword argument 'behavior'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_split_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::Katie::QString::SplitBehavior cppArg1 = Katie::QString::SplitBehavior::KeepEmptyParts;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // split(Katie::QRegExp,Katie::QString::SplitBehavior)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList cppResult = const_cast<const ::Katie::QString*>(cppSelf)->split(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
            }
            break;
        }
        case 2: // split(const Katie::QString & sep, Katie::QString::SplitBehavior behavior, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "behavior");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.split(): got multiple values for keyword argument 'behavior'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_split_TypeError;
                }
                value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[2]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.split(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[2] = value;
                    if (!(pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[2]))))
                        goto Sbk_Katie_QStringFunc_split_TypeError;
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

            ::Katie::QString::SplitBehavior cppArg1 = Katie::QString::SplitBehavior::KeepEmptyParts;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);
            ::Katie::Qt::CaseSensitivity cppArg2 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // split(Katie::QString,Katie::QString::SplitBehavior,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QStringList cppResult = const_cast<const ::Katie::QString*>(cppSelf)->split(*cppArg0, cppArg1, cppArg2);
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

    Sbk_Katie_QStringFunc_split_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.split");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_sprintf(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::sprintf(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 0; // sprintf(const char*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_sprintf_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // sprintf(const char*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString & cppResult = cppSelf->sprintf(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::referenceToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_sprintf_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.sprintf");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_squeeze(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));

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

static PyObject* Sbk_Katie_QStringFunc_startsWith(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.startsWith(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.startsWith(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:startsWith", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::startsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
    // 1: QString::startsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
    // 2: QString::startsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 1; // startsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 1; // startsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // startsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 0; // startsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
        }
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 2; // startsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1])))) {
            overloadId = 2; // startsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_startsWith_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // startsWith(const Katie::QChar & c, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.startsWith(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_startsWith_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0_local;
            ::Katie::QChar* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // startsWith(Katie::QChar,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->startsWith(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // startsWith(const Katie::QLatin1String & s, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.startsWith(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_startsWith_TypeError;
                }
            }
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);

            ::Katie::Qt::CaseSensitivity cppArg1 = Katie::Qt::CaseSensitive;
            if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // startsWith(Katie::QLatin1String,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->startsWith(*cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 2: // startsWith(const Katie::QString & s, Katie::Qt::CaseSensitivity cs) const
        {
            if (kwds) {
                PyObject* value = PyDict_GetItemString(kwds, "cs");
                if (value && pyArgs[1]) {
                    PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.startsWith(): got multiple values for keyword argument 'cs'.");
                    return {};
                } else if (value) {
                    pyArgs[1] = value;
                    if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_CASESENSITIVITY_IDX])->converter, (pyArgs[1]))))
                        goto Sbk_Katie_QStringFunc_startsWith_TypeError;
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
                // startsWith(Katie::QString,Katie::Qt::CaseSensitivity)const
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = const_cast<const ::Katie::QString*>(cppSelf)->startsWith(*cppArg0, cppArg1);
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

    Sbk_Katie_QStringFunc_startsWith_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.startsWith");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_swap(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::swap(Katie::QString&)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // swap(Katie::QString&)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_swap_TypeError;

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
            // swap(Katie::QString&)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->swap(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QStringFunc_swap_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.swap");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toAscii(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toAscii()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toAscii();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_toCaseFolded(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toCaseFolded()const
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toCaseFolded();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_toDouble(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toDouble(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toDouble", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QString::toDouble(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toDouble(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toDouble(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toDouble_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toDouble(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toDouble_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toDouble(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            double cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toDouble(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<double>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toDouble_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toDouble");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toFloat(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toFloat(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toFloat", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QString::toFloat(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toFloat(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toFloat(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toFloat_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toFloat(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toFloat_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toFloat(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            float cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toFloat(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<float>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toFloat_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toFloat");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toInt(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toInt(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toInt", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toInt(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toInt_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toInt(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toInt_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toInt(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toInt_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toInt(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toInt(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toInt_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toInt");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toLatin1(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toLatin1()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toLatin1();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_toLocal8Bit(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toLocal8Bit()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toLocal8Bit();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_toLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toLong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toLong(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toLong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toLong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toLong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            long cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toLong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toLong");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toLongLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toLongLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toLongLong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toLongLong(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toLongLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toLongLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toLongLong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toLongLong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toLongLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toLongLong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            long long cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toLongLong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toLongLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toLongLong");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toLower(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toLower()const
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toLower();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_toShort(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toShort(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toShort", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toShort(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toShort_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toShort(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toShort_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toShort(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toShort_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toShort(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            short cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toShort(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<short>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toShort_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toShort");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toStdString(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toStdString()const
            std::string cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toStdString();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<std::string>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_toUInt(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toUInt(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toUInt", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toUInt(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toUInt_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toUInt(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toUInt_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toUInt(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toUInt_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toUInt(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            uint cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toUInt(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toUInt_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toUInt");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toULong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toULong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toULong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toULong(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toULong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toULong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toULong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toULong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toULong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toULong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ulong cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toULong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ulong>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toULong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toULong");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toULongLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toULongLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toULongLong", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toULongLong(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toULongLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toULongLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toULongLong_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toULongLong(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toULongLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toULongLong(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            unsigned long long cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toULongLong(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toULongLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toULongLong");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toUShort(PyObject* self, PyObject* args, PyObject* kwds)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toUShort(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:toUShort", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QString::toUShort(bool*,int)const
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
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_toUShort_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toUShort(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QStringFunc_toUShort_TypeError;
            }
            value = PyDict_GetItemString(kwds, "base");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QString.toUShort(): got multiple values for keyword argument 'base'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1]))))
                    goto Sbk_Katie_QStringFunc_toUShort_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);
        int cppArg1 = 10;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // toUShort(bool*,int)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ushort cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toUShort(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QStringFunc_toUShort_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QString.toUShort");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_toUpper(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toUpper()const
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toUpper();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_toUtf8(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toUtf8()const
            Katie::QByteArray cppResult = const_cast<const ::Katie::QString*>(cppSelf)->toUtf8();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_trimmed(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // trimmed()const
            Katie::QString cppResult = const_cast<const ::Katie::QString*>(cppSelf)->trimmed();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_truncate(PyObject* self, PyObject* pyArg)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::truncate(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // truncate(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc_truncate_TypeError;

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

    Sbk_Katie_QStringFunc_truncate_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.truncate");
        return {};
}

static PyObject* Sbk_Katie_QStringFunc_unicode(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // unicode()const
            const Katie::QChar * cppResult = const_cast<const ::Katie::QString*>(cppSelf)->unicode();
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), cppResult);
            Shiboken::Object::setParent(self, pyResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QStringFunc_utf16(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // utf16()const
            const ushort * cppResult = const_cast<const ::Katie::QString*>(cppSelf)->utf16();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QString___copy__(PyObject* self)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QString& cppSelf = *reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppSelf);
    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QStringMethod_compare = {
    "compare", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_compare), METH_VARARGS|METH_KEYWORDS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QStringMethod_localeAwareCompare = {
    "localeAwareCompare", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_localeAwareCompare), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QString_methods[] = {
    {"append", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_append), METH_O},
    {"arg", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_arg), METH_VARARGS|METH_KEYWORDS},
    {"at", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_at), METH_O},
    {"begin", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_begin), METH_NOARGS},
    {"capacity", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_capacity), METH_NOARGS},
    {"chop", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_chop), METH_O},
    {"clear", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_clear), METH_NOARGS},
    Sbk_Katie_QStringMethod_compare,
    {"constBegin", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_constBegin), METH_NOARGS},
    {"constData", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_constData), METH_NOARGS},
    {"constEnd", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_constEnd), METH_NOARGS},
    {"contains", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_contains), METH_VARARGS|METH_KEYWORDS},
    {"count", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_count), METH_VARARGS|METH_KEYWORDS},
    {"data", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_data), METH_NOARGS},
    {"detach", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_detach), METH_NOARGS},
    {"end", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_end), METH_NOARGS},
    {"endsWith", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_endsWith), METH_VARARGS|METH_KEYWORDS},
    {"fill", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fill), METH_VARARGS|METH_KEYWORDS},
    {"fromAscii", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromAscii), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"fromLatin1", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromLatin1), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"fromLocal8Bit", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromLocal8Bit), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"fromRawData", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromRawData), METH_VARARGS|METH_STATIC},
    {"fromStdString", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromStdString), METH_O|METH_STATIC},
    {"fromUcs4", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromUcs4), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"fromUtf16", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromUtf16), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"fromUtf8", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_fromUtf8), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"indexOf", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_indexOf), METH_VARARGS|METH_KEYWORDS},
    {"insert", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_insert), METH_VARARGS},
    {"isDetached", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_isDetached), METH_NOARGS},
    {"isEmpty", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_isEmpty), METH_NOARGS},
    {"isNull", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_isNull), METH_NOARGS},
    {"isRightToLeft", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_isRightToLeft), METH_NOARGS},
    {"isSharedWith", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_isSharedWith), METH_O},
    {"isSimpleText", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_isSimpleText), METH_NOARGS},
    {"lastIndexOf", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_lastIndexOf), METH_VARARGS|METH_KEYWORDS},
    {"left", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_left), METH_O},
    {"leftJustified", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_leftJustified), METH_VARARGS|METH_KEYWORDS},
    {"length", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_length), METH_NOARGS},
    Sbk_Katie_QStringMethod_localeAwareCompare,
    {"mid", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_mid), METH_VARARGS|METH_KEYWORDS},
    {"normalized", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_normalized), METH_VARARGS},
    {"number", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_number), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"prepend", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_prepend), METH_O},
    {"push_back", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_push_back), METH_O},
    {"push_front", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_push_front), METH_O},
    {"remove", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_remove), METH_VARARGS|METH_KEYWORDS},
    {"repeated", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_repeated), METH_O},
    {"replace", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_replace), METH_VARARGS|METH_KEYWORDS},
    {"reserve", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_reserve), METH_O},
    {"resize", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_resize), METH_O},
    {"right", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_right), METH_O},
    {"rightJustified", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_rightJustified), METH_VARARGS|METH_KEYWORDS},
    {"section", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_section), METH_VARARGS|METH_KEYWORDS},
    {"setNum", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_setNum), METH_VARARGS|METH_KEYWORDS},
    {"setRawData", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_setRawData), METH_VARARGS},
    {"setUnicode", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_setUnicode), METH_VARARGS},
    {"setUtf16", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_setUtf16), METH_VARARGS},
    {"simplified", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_simplified), METH_NOARGS},
    {"size", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_size), METH_NOARGS},
    {"split", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_split), METH_VARARGS|METH_KEYWORDS},
    {"sprintf", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_sprintf), METH_O},
    {"squeeze", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_squeeze), METH_NOARGS},
    {"startsWith", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_startsWith), METH_VARARGS|METH_KEYWORDS},
    {"swap", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_swap), METH_O},
    {"toAscii", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toAscii), METH_NOARGS},
    {"toCaseFolded", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toCaseFolded), METH_NOARGS},
    {"toDouble", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toDouble), METH_VARARGS|METH_KEYWORDS},
    {"toFloat", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toFloat), METH_VARARGS|METH_KEYWORDS},
    {"toInt", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toInt), METH_VARARGS|METH_KEYWORDS},
    {"toLatin1", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toLatin1), METH_NOARGS},
    {"toLocal8Bit", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toLocal8Bit), METH_NOARGS},
    {"toLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toLong), METH_VARARGS|METH_KEYWORDS},
    {"toLongLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toLongLong), METH_VARARGS|METH_KEYWORDS},
    {"toLower", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toLower), METH_NOARGS},
    {"toShort", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toShort), METH_VARARGS|METH_KEYWORDS},
    {"toStdString", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toStdString), METH_NOARGS},
    {"toUInt", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toUInt), METH_VARARGS|METH_KEYWORDS},
    {"toULong", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toULong), METH_VARARGS|METH_KEYWORDS},
    {"toULongLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toULongLong), METH_VARARGS|METH_KEYWORDS},
    {"toUShort", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toUShort), METH_VARARGS|METH_KEYWORDS},
    {"toUpper", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toUpper), METH_NOARGS},
    {"toUtf8", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_toUtf8), METH_NOARGS},
    {"trimmed", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_trimmed), METH_NOARGS},
    {"truncate", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_truncate), METH_O},
    {"unicode", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_unicode), METH_NOARGS},
    {"utf16", reinterpret_cast<PyCFunction>(Sbk_Katie_QStringFunc_utf16), METH_NOARGS},

    {"__copy__", reinterpret_cast<PyCFunction>(Sbk_Katie_QString___copy__), METH_NOARGS},
    {nullptr, nullptr} // Sentinel
};

static PyObject* Sbk_Katie_QString_getattro(PyObject* self, PyObject* name)
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
        static PyMethodDef non_static_Sbk_Katie_QStringMethod_compare = {
            Sbk_Katie_QStringMethod_compare.ml_name,
            Sbk_Katie_QStringMethod_compare.ml_meth,
            Sbk_Katie_QStringMethod_compare.ml_flags & (~METH_STATIC),
            Sbk_Katie_QStringMethod_compare.ml_doc,
        };
        if (Shiboken::String::compare(name, "compare") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QStringMethod_compare, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QStringMethod_localeAwareCompare = {
            Sbk_Katie_QStringMethod_localeAwareCompare.ml_name,
            Sbk_Katie_QStringMethod_localeAwareCompare.ml_meth,
            Sbk_Katie_QStringMethod_localeAwareCompare.ml_flags & (~METH_STATIC),
            Sbk_Katie_QStringMethod_localeAwareCompare.ml_doc,
        };
        if (Shiboken::String::compare(name, "localeAwareCompare") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QStringMethod_localeAwareCompare, self, 0);
    }
    return PyObject_GenericGetAttr(self, name);
}

static int Sbk_Katie_QString___nb_bool(PyObject* self)
{
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return -1;
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    return !cppSelf->isNull();
}

static PyObject* Sbk_Katie_QStringFunc___iadd__(PyObject* self, PyObject* pyArg)
{
    bool isReverse = SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], pyArg)
                    && !SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], self);
    if (isReverse)
        std::swap(self, pyArg);
    ::Katie::QString* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QString::operator+=(Katie::QChar)
    // 1: QString::operator+=(Katie::QChar::SpecialCharacter)
    // 2: QString::operator+=(char)
    // 3: QString::operator+=(Katie::QByteArray)
    // 4: QString::operator+=(Katie::QLatin1String)
    // 5: QString::operator+=(Katie::QString)
    // 6: QString::operator+=(const char*)
    if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
        overloadId = 4; // operator+=(Katie::QLatin1String)
    } else if (!isReverse
        && SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 2; // operator+=(char)
    } else if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX])->converter, (pyArg)))) {
        overloadId = 1; // operator+=(Katie::QChar::SpecialCharacter)
    } else if (!isReverse
        && Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 6; // operator+=(const char*)
    } else if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
        overloadId = 3; // operator+=(Katie::QByteArray)
    } else if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArg)))) {
        overloadId = 0; // operator+=(Katie::QChar)
    } else if (!isReverse
        && (pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 5; // operator+=(Katie::QString)
    }

    if (isReverse && overloadId == -1) {
        PyErr_SetString(PyExc_NotImplementedError, "reverse operator not implemented.");
        return {};
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QStringFunc___iadd___TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // operator+=(Katie::QChar c)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // operator+=(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                (*cppSelf) += cppArg0;
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // operator+=(Katie::QChar::SpecialCharacter c)
        {
            ::Katie::QChar::SpecialCharacter cppArg0{Katie::QChar::Null};
            pythonToCpp(pyArg, &cppArg0);

            if (!PyErr_Occurred()) {
                // operator+=(Katie::QChar::SpecialCharacter)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                (*cppSelf) += cppArg0;
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // operator+=(char c)
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
        case 3: // operator+=(const Katie::QByteArray & s)
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
        case 4: // operator+=(const Katie::QLatin1String & s)
        {
            if (!Shiboken::Object::isValid(pyArg))
                return {};
            ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
            ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                pythonToCpp(pyArg, &cppArg0_local);
            else
                pythonToCpp(pyArg, &cppArg0);


            if (!PyErr_Occurred()) {
                // operator+=(Katie::QLatin1String)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                (*cppSelf) += (*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 5: // operator+=(const Katie::QString & s)
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
        case 6: // operator+=(const char * s)
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

    Sbk_Katie_QStringFunc___iadd___TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QString.__iadd__");
        return {};
}

// Rich comparison
static PyObject* Sbk_Katie_QString_richcompare(PyObject* self, PyObject* pyArg, int op)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QString& cppSelf = *reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(self)));
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
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
                // operator!=(const Katie::QLatin1String & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
                ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf !=(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
                // operator!=(const Katie::QByteArray & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QByteArray cppArg0_local;
                ::Katie::QByteArray* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf !=(*cppArg0);
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
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
                // operator<(const Katie::QLatin1String & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
                ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf <(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
                // operator<(const Katie::QByteArray & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QByteArray cppArg0_local;
                ::Katie::QByteArray* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf <(*cppArg0);
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
                goto Sbk_Katie_QString_RichComparison_TypeError;
            }

            break;
        case Py_LE:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator<=(const char * s2) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf <=(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
                // operator<=(const Katie::QLatin1String & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
                ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf <=(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
                // operator<=(const Katie::QByteArray & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QByteArray cppArg0_local;
                ::Katie::QByteArray* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf <=(*cppArg0);
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
                goto Sbk_Katie_QString_RichComparison_TypeError;
            }

            break;
        case Py_EQ:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator==(const char * s) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf ==(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
                // operator==(const Katie::QLatin1String & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
                ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf ==(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
                // operator==(const Katie::QByteArray & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QByteArray cppArg0_local;
                ::Katie::QByteArray* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf ==(*cppArg0);
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
                // operator>(const char * s2) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf >(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
                // operator>(const Katie::QLatin1String & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
                ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf >(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
                // operator>(const Katie::QByteArray & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QByteArray cppArg0_local;
                ::Katie::QByteArray* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf >(*cppArg0);
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
                goto Sbk_Katie_QString_RichComparison_TypeError;
            }

            break;
        case Py_GE:
            if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
                // operator>=(const char * s2) const
                const char* cppArg0;
                pythonToCpp(pyArg, &cppArg0);
                bool cppResult = cppSelf >=(cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArg)))) {
                // operator>=(const Katie::QLatin1String & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QLatin1String cppArg0_local = ::Katie::QLatin1String(static_cast<char*>(nullptr));
                ::Katie::QLatin1String* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf >=(*cppArg0);
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            } else if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArg)))) {
                // operator>=(const Katie::QByteArray & s) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QByteArray cppArg0_local;
                ::Katie::QByteArray* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), pythonToCpp))
                    pythonToCpp(pyArg, &cppArg0_local);
                else
                    pythonToCpp(pyArg, &cppArg0);

                bool cppResult = cppSelf >=(*cppArg0);
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
                goto Sbk_Katie_QString_RichComparison_TypeError;
            }

            break;
        default:
            goto Sbk_Katie_QString_RichComparison_TypeError;
    }

    if (pyResult && !PyErr_Occurred())
        return pyResult;
    Sbk_Katie_QString_RichComparison_TypeError:
    PyErr_SetString(PyExc_NotImplementedError, "operator not implemented.");
    return {};

}

} // extern "C"

static int Sbk_Katie_QString_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QString_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QString_Type = nullptr;
static SbkObjectType *Sbk_Katie_QString_TypeF(void)
{
    return _Sbk_Katie_QString_Type;
}

static PyType_Slot Sbk_Katie_QString_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    reinterpret_cast<void*>(Sbk_Katie_QString_getattro)},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QString_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QString_clear)},
    {Py_tp_richcompare, reinterpret_cast<void*>(Sbk_Katie_QString_richcompare)},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QString_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QString_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    // type supports number protocol
    {Py_nb_inplace_add, (void *)Sbk_Katie_QStringFunc___iadd__},
#ifdef IS_PY3K
    {Py_nb_bool, (void *)Sbk_Katie_QString___nb_bool},
#else
    {Py_nb_nonzero, (void *)Sbk_Katie_QString___nb_bool},
#endif
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QString_spec = {
    "KtCore.Katie.QString",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QString_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ enum conversion.
static void Katie_QString_SectionFlag_PythonToCpp_Katie_QString_SectionFlag(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString::SectionFlag*>(cppOut) =
        static_cast<::Katie::QString::SectionFlag>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QString_SectionFlag_PythonToCpp_Katie_QString_SectionFlag_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX]))
        return Katie_QString_SectionFlag_PythonToCpp_Katie_QString_SectionFlag;
    return {};
}
static PyObject* Katie_QString_SectionFlag_CppToPython_Katie_QString_SectionFlag(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QString::SectionFlag *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX], castCppIn);

}

static void Katie_QString_SplitBehavior_PythonToCpp_Katie_QString_SplitBehavior(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString::SplitBehavior*>(cppOut) =
        static_cast<::Katie::QString::SplitBehavior>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QString_SplitBehavior_PythonToCpp_Katie_QString_SplitBehavior_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX]))
        return Katie_QString_SplitBehavior_PythonToCpp_Katie_QString_SplitBehavior;
    return {};
}
static PyObject* Katie_QString_SplitBehavior_CppToPython_Katie_QString_SplitBehavior(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QString::SplitBehavior *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX], castCppIn);

}

static void Katie_QString_NormalizationForm_PythonToCpp_Katie_QString_NormalizationForm(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString::NormalizationForm*>(cppOut) =
        static_cast<::Katie::QString::NormalizationForm>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QString_NormalizationForm_PythonToCpp_Katie_QString_NormalizationForm_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX]))
        return Katie_QString_NormalizationForm_PythonToCpp_Katie_QString_NormalizationForm;
    return {};
}
static PyObject* Katie_QString_NormalizationForm_CppToPython_Katie_QString_NormalizationForm(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QString::NormalizationForm *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX], castCppIn);

}

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QString_PythonToCpp_QString_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QString_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QString_PythonToCpp_QString_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QString_TypeF())))
        return QString_PythonToCpp_QString_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QString_PTR_CppToPython_QString(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QString *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QString_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// C++ to Python copy conversion.
static PyObject* QString_COPY_CppToPython_QString(const void* cppIn) {
    return Shiboken::Object::newObject(Sbk_Katie_QString_TypeF(), new ::Katie::QString(*reinterpret_cast<const ::Katie::QString*>(cppIn)), true, true);
}

// Python to C++ copy conversion.
static void QString_PythonToCpp_QString_COPY(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString*>(cppOut) = *reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(pyIn)));
}
static PythonToCppFunc is_QString_PythonToCpp_QString_COPY_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QString_TypeF())))
        return QString_PythonToCpp_QString_COPY;
    return {};
}

// Implicit conversions.
static void constKatie_QByteArrayREF_PythonToCpp_Katie_QString(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString*>(cppOut) = ::Katie::QString(*reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QByteArrayREF_PythonToCpp_Katie_QString_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], pyIn))
        return constKatie_QByteArrayREF_PythonToCpp_Katie_QString;
    return {};
}

static void constKatie_QCharPTR_PythonToCpp_Katie_QString(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString*>(cppOut) = ::Katie::QString(reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QCharPTR_PythonToCpp_Katie_QString_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], pyIn))
        return constKatie_QCharPTR_PythonToCpp_Katie_QString;
    return {};
}

static void constKatie_QChar_PythonToCpp_Katie_QString(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString*>(cppOut) = ::Katie::QString(*reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QChar_PythonToCpp_Katie_QString_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], pyIn))
        return constKatie_QChar_PythonToCpp_Katie_QString;
    return {};
}

static void constKatie_QLatin1StringREF_PythonToCpp_Katie_QString(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QString*>(cppOut) = ::Katie::QString(*reinterpret_cast< ::Katie::QLatin1String *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QLatin1StringREF_PythonToCpp_Katie_QString_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], pyIn))
        return constKatie_QLatin1StringREF_PythonToCpp_Katie_QString;
    return {};
}

static void constcharPTR_PythonToCpp_Katie_QString(PyObject* pyIn, void* cppOut) {
    const char* cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QString*>(cppOut) = ::Katie::QString(cppIn);
}
static PythonToCppFunc is_constcharPTR_PythonToCpp_Katie_QString_Convertible(PyObject* pyIn) {
    if (Shiboken::String::check(pyIn))
        return constcharPTR_PythonToCpp_Katie_QString;
    return {};
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QString_SignatureStrings[] = {
    "8:KtCore.Katie.QString()",
    "7:KtCore.Katie.QString(a:KtCore.Katie.QByteArray)",
    "6:KtCore.Katie.QString(unicode:KtCore.Katie.QChar,size:int=-1)",
    "5:KtCore.Katie.QString(c:KtCore.Katie.QChar)",
    "4:KtCore.Katie.QString(latin1:KtCore.Katie.QLatin1String)",
    "3:KtCore.Katie.QString(arg__1:KtCore.Katie.QString)",
    "2:KtCore.Katie.QString(ch:str)",
    "1:KtCore.Katie.QString(size:int,c:KtCore.Katie.QChar)",
    "0:KtCore.Katie.QString(size:int,arg__2:KtCore.Katie.Qt.Initialization)",
    "4:KtCore.Katie.QString.append(c:KtCore.Katie.QChar)->KtCore.Katie.QString",
    "3:KtCore.Katie.QString.append(s:KtCore.Katie.QByteArray)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.append(s:KtCore.Katie.QLatin1String)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.append(s:KtCore.Katie.QString)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.append(s:str)->KtCore.Katie.QString",
    "19:KtCore.Katie.QString.arg(a:KtCore.Katie.QChar,fieldWidth:int=0,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "18:KtCore.Katie.QString.arg(a:char,fieldWidth:int=0,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "17:KtCore.Katie.QString.arg(a:KtCore.Katie.QString,fieldWidth:int=0,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "16:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString)->KtCore.Katie.QString",
    "15:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString,a3:KtCore.Katie.QString)->KtCore.Katie.QString",
    "14:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString,a3:KtCore.Katie.QString,a4:KtCore.Katie.QString)->KtCore.Katie.QString",
    "13:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString,a3:KtCore.Katie.QString,a4:KtCore.Katie.QString,a5:KtCore.Katie.QString)->KtCore.Katie.QString",
    "12:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString,a3:KtCore.Katie.QString,a4:KtCore.Katie.QString,a5:KtCore.Katie.QString,a6:KtCore.Katie.QString)->KtCore.Katie.QString",
    "11:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString,a3:KtCore.Katie.QString,a4:KtCore.Katie.QString,a5:KtCore.Katie.QString,a6:KtCore.Katie.QString,a7:KtCore.Katie.QString)->KtCore.Katie.QString",
    "10:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString,a3:KtCore.Katie.QString,a4:KtCore.Katie.QString,a5:KtCore.Katie.QString,a6:KtCore.Katie.QString,a7:KtCore.Katie.QString,a8:KtCore.Katie.QString)->KtCore.Katie.QString",
    "9:KtCore.Katie.QString.arg(a1:KtCore.Katie.QString,a2:KtCore.Katie.QString,a3:KtCore.Katie.QString,a4:KtCore.Katie.QString,a5:KtCore.Katie.QString,a6:KtCore.Katie.QString,a7:KtCore.Katie.QString,a8:KtCore.Katie.QString,a9:KtCore.Katie.QString)->KtCore.Katie.QString",
    "8:KtCore.Katie.QString.arg(a:double,fieldWidth:int=0,fmt:char='g',prec:int=-1,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "7:KtCore.Katie.QString.arg(a:int,fieldWidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "6:KtCore.Katie.QString.arg(a:long,fieldwidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "5:KtCore.Katie.QString.arg(a:long long,fieldwidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "4:KtCore.Katie.QString.arg(a:short,fieldWidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "3:KtCore.Katie.QString.arg(a:unsigned int,fieldWidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.arg(a:ulong,fieldwidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.arg(a:unsigned long long,fieldwidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.arg(a:ushort,fieldWidth:int=0,base:int=10,fillChar:KtCore.Katie.QChar=QLatin1Char(' '))->KtCore.Katie.QString",
    "KtCore.Katie.QString.at(i:int)->KtCore.Katie.QChar",
    "1:KtCore.Katie.QString.begin()->KtCore.Katie.QChar",
    "0:KtCore.Katie.QString.begin()->KtCore.Katie.QChar",
    "KtCore.Katie.QString.capacity()->int",
    "KtCore.Katie.QString.chop(n:int)",
    "KtCore.Katie.QString.clear()",
    "4:KtCore.Katie.QString.compare(other:KtCore.Katie.QLatin1String,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "3:KtCore.Katie.QString.compare(s1:KtCore.Katie.QLatin1String,s2:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "2:KtCore.Katie.QString.compare(s:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "1:KtCore.Katie.QString.compare(s1:KtCore.Katie.QString,s2:KtCore.Katie.QLatin1String,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "0:KtCore.Katie.QString.compare(s1:KtCore.Katie.QString,s2:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "KtCore.Katie.QString.constBegin()->KtCore.Katie.QChar",
    "KtCore.Katie.QString.constData()->KtCore.Katie.QChar",
    "KtCore.Katie.QString.constEnd()->KtCore.Katie.QChar",
    "3:KtCore.Katie.QString.contains(c:KtCore.Katie.QChar,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "2:KtCore.Katie.QString.contains(rx:KtCore.Katie.QRegExp)->bool",
    "1:KtCore.Katie.QString.contains(rx:KtCore.Katie.QRegExp)->bool",
    "0:KtCore.Katie.QString.contains(s:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "3:KtCore.Katie.QString.count()->int",
    "2:KtCore.Katie.QString.count(c:KtCore.Katie.QChar,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "1:KtCore.Katie.QString.count(arg__1:KtCore.Katie.QRegExp)->int",
    "0:KtCore.Katie.QString.count(s:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "1:KtCore.Katie.QString.data()->KtCore.Katie.QChar",
    "0:KtCore.Katie.QString.data()->KtCore.Katie.QChar",
    "KtCore.Katie.QString.detach()",
    "1:KtCore.Katie.QString.end()->KtCore.Katie.QChar",
    "0:KtCore.Katie.QString.end()->KtCore.Katie.QChar",
    "2:KtCore.Katie.QString.endsWith(c:KtCore.Katie.QChar,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "1:KtCore.Katie.QString.endsWith(s:KtCore.Katie.QLatin1String,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "0:KtCore.Katie.QString.endsWith(s:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "KtCore.Katie.QString.fill(c:KtCore.Katie.QChar,size:int=-1)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromAscii(arg__1:str,size:int=-1)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromLatin1(arg__1:str,size:int=-1)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromLocal8Bit(arg__1:str,size:int=-1)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromRawData(arg__1:KtCore.Katie.QChar,size:int)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromStdString(s:std.string)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromUcs4(arg__1:KtCore.uint,size:int=-1)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromUtf16(arg__1:KtCore.ushort,size:int=-1)->KtCore.Katie.QString",
    "KtCore.Katie.QString.fromUtf8(arg__1:str,size:int=-1)->KtCore.Katie.QString",
    "4:KtCore.Katie.QString.indexOf(c:KtCore.Katie.QChar,from:int=0,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "3:KtCore.Katie.QString.indexOf(arg__1:KtCore.Katie.QRegExp,from:int=0)->int",
    "2:KtCore.Katie.QString.indexOf(s:KtCore.Katie.QLatin1String,from:int=0,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "1:KtCore.Katie.QString.indexOf(arg__1:KtCore.Katie.QRegExp,from:int=0)->int",
    "0:KtCore.Katie.QString.indexOf(s:KtCore.Katie.QString,from:int=0,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "3:KtCore.Katie.QString.insert(i:int,c:KtCore.Katie.QChar)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.insert(i:int,uc:KtCore.Katie.QChar,len:int)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.insert(i:int,s:KtCore.Katie.QLatin1String)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.insert(i:int,s:KtCore.Katie.QString)->KtCore.Katie.QString",
    "KtCore.Katie.QString.isDetached()->bool",
    "KtCore.Katie.QString.isEmpty()->bool",
    "KtCore.Katie.QString.isNull()->bool",
    "KtCore.Katie.QString.isRightToLeft()->bool",
    "KtCore.Katie.QString.isSharedWith(other:KtCore.Katie.QString)->bool",
    "KtCore.Katie.QString.isSimpleText()->bool",
    "4:KtCore.Katie.QString.lastIndexOf(c:KtCore.Katie.QChar,from:int=-1,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "3:KtCore.Katie.QString.lastIndexOf(arg__1:KtCore.Katie.QRegExp,from:int=-1)->int",
    "2:KtCore.Katie.QString.lastIndexOf(s:KtCore.Katie.QLatin1String,from:int=-1,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "1:KtCore.Katie.QString.lastIndexOf(arg__1:KtCore.Katie.QRegExp,from:int=-1)->int",
    "0:KtCore.Katie.QString.lastIndexOf(s:KtCore.Katie.QString,from:int=-1,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->int",
    "KtCore.Katie.QString.left(n:int)->KtCore.Katie.QString",
    "KtCore.Katie.QString.leftJustified(width:int,fill:KtCore.Katie.QChar=QLatin1Char(' '),trunc:bool=false)->KtCore.Katie.QString",
    "KtCore.Katie.QString.length()->int",
    "1:KtCore.Katie.QString.localeAwareCompare(s:KtCore.Katie.QString)->int",
    "0:KtCore.Katie.QString.localeAwareCompare(s1:KtCore.Katie.QString,s2:KtCore.Katie.QString)->int",
    "KtCore.Katie.QString.mid(position:int,n:int=-1)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.normalized(mode:KtCore.Katie.QString.NormalizationForm)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.normalized(mode:KtCore.Katie.QString.NormalizationForm,version:KtCore.Katie.QChar.UnicodeVersion)->KtCore.Katie.QString",
    "6:KtCore.Katie.QString.number(arg__1:double,f:char='g',prec:int=6)->KtCore.Katie.QString",
    "5:KtCore.Katie.QString.number(arg__1:int,base:int=10)->KtCore.Katie.QString",
    "4:KtCore.Katie.QString.number(arg__1:long,base:int=10)->KtCore.Katie.QString",
    "3:KtCore.Katie.QString.number(arg__1:long long,base:int=10)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.number(arg__1:unsigned int,base:int=10)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.number(arg__1:ulong,base:int=10)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.number(arg__1:unsigned long long,base:int=10)->KtCore.Katie.QString",
    "4:KtCore.Katie.QString.prepend(c:KtCore.Katie.QChar)->KtCore.Katie.QString",
    "3:KtCore.Katie.QString.prepend(s:KtCore.Katie.QByteArray)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.prepend(s:KtCore.Katie.QLatin1String)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.prepend(s:KtCore.Katie.QString)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.prepend(s:str)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.push_back(c:KtCore.Katie.QChar)",
    "0:KtCore.Katie.QString.push_back(s:KtCore.Katie.QString)",
    "1:KtCore.Katie.QString.push_front(c:KtCore.Katie.QChar)",
    "0:KtCore.Katie.QString.push_front(s:KtCore.Katie.QString)",
    "3:KtCore.Katie.QString.remove(c:KtCore.Katie.QChar,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.remove(rx:KtCore.Katie.QRegExp)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.remove(s:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.remove(i:int,len:int)->KtCore.Katie.QString",
    "KtCore.Katie.QString.repeated(times:int)->KtCore.Katie.QString",
    "11:KtCore.Katie.QString.replace(before:KtCore.Katie.QChar,after:KtCore.Katie.QChar,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "10:KtCore.Katie.QString.replace(c:KtCore.Katie.QChar,after:KtCore.Katie.QLatin1String,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "9:KtCore.Katie.QString.replace(c:KtCore.Katie.QChar,after:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "8:KtCore.Katie.QString.replace(before:KtCore.Katie.QChar,blen:int,after:KtCore.Katie.QChar,alen:int,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "7:KtCore.Katie.QString.replace(before:KtCore.Katie.QLatin1String,after:KtCore.Katie.QLatin1String,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "6:KtCore.Katie.QString.replace(before:KtCore.Katie.QLatin1String,after:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "5:KtCore.Katie.QString.replace(rx:KtCore.Katie.QRegExp,after:KtCore.Katie.QString)->KtCore.Katie.QString",
    "4:KtCore.Katie.QString.replace(before:KtCore.Katie.QString,after:KtCore.Katie.QLatin1String,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "3:KtCore.Katie.QString.replace(before:KtCore.Katie.QString,after:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.replace(i:int,len:int,after:KtCore.Katie.QChar)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.replace(i:int,len:int,s:KtCore.Katie.QChar,slen:int)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.replace(i:int,len:int,after:KtCore.Katie.QString)->KtCore.Katie.QString",
    "KtCore.Katie.QString.reserve(size:int)",
    "KtCore.Katie.QString.resize(size:int)",
    "KtCore.Katie.QString.right(n:int)->KtCore.Katie.QString",
    "KtCore.Katie.QString.rightJustified(width:int,fill:KtCore.Katie.QChar=QLatin1Char(' '),trunc:bool=false)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.section(sep:KtCore.Katie.QChar,start:int,end:int=-1)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.section(reg:KtCore.Katie.QRegExp,start:int,end:int=-1)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.section(in_sep:KtCore.Katie.QString,start:int,end:int=-1)->KtCore.Katie.QString",
    "9:KtCore.Katie.QString.setNum(arg__1:double,f:char='g',prec:int=6)->KtCore.Katie.QString",
    "8:KtCore.Katie.QString.setNum(arg__1:float,f:char='g',prec:int=6)->KtCore.Katie.QString",
    "7:KtCore.Katie.QString.setNum(arg__1:int,base:int=10)->KtCore.Katie.QString",
    "6:KtCore.Katie.QString.setNum(arg__1:long,base:int=10)->KtCore.Katie.QString",
    "5:KtCore.Katie.QString.setNum(arg__1:long long,base:int=10)->KtCore.Katie.QString",
    "4:KtCore.Katie.QString.setNum(arg__1:short,base:int=10)->KtCore.Katie.QString",
    "3:KtCore.Katie.QString.setNum(arg__1:unsigned int,base:int=10)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.setNum(arg__1:ulong,base:int=10)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.setNum(arg__1:unsigned long long,base:int=10)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.setNum(arg__1:ushort,base:int=10)->KtCore.Katie.QString",
    "KtCore.Katie.QString.setRawData(unicode:KtCore.Katie.QChar,size:int)->KtCore.Katie.QString",
    "KtCore.Katie.QString.setUnicode(unicode:KtCore.Katie.QChar,size:int)->KtCore.Katie.QString",
    "KtCore.Katie.QString.setUtf16(utf16:KtCore.ushort,size:int)->KtCore.Katie.QString",
    "KtCore.Katie.QString.simplified()->KtCore.Katie.QString",
    "KtCore.Katie.QString.size()->int",
    "2:KtCore.Katie.QString.split(sep:KtCore.Katie.QChar,behavior:KtCore.Katie.QString.SplitBehavior=KeepEmptyParts,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QStringList",
    "1:KtCore.Katie.QString.split(sep:KtCore.Katie.QRegExp,behavior:KtCore.Katie.QString.SplitBehavior=KeepEmptyParts)->KtCore.Katie.QStringList",
    "0:KtCore.Katie.QString.split(sep:KtCore.Katie.QString,behavior:KtCore.Katie.QString.SplitBehavior=KeepEmptyParts,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->KtCore.Katie.QStringList",
    "KtCore.Katie.QString.sprintf(format:str)->KtCore.Katie.QString",
    "KtCore.Katie.QString.squeeze()",
    "2:KtCore.Katie.QString.startsWith(c:KtCore.Katie.QChar,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "1:KtCore.Katie.QString.startsWith(s:KtCore.Katie.QLatin1String,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "0:KtCore.Katie.QString.startsWith(s:KtCore.Katie.QString,cs:KtCore.Katie.Qt.CaseSensitivity=Qt.CaseSensitive)->bool",
    "KtCore.Katie.QString.swap(other:KtCore.Katie.QString)",
    "KtCore.Katie.QString.toAscii()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QString.toCaseFolded()->KtCore.Katie.QString",
    "KtCore.Katie.QString.toDouble(ok:KtCore.bool=Q_NULLPTR)->double",
    "KtCore.Katie.QString.toFloat(ok:KtCore.bool=Q_NULLPTR)->float",
    "KtCore.Katie.QString.toInt(ok:KtCore.bool=Q_NULLPTR,base:int=10)->int",
    "KtCore.Katie.QString.toLatin1()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QString.toLocal8Bit()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QString.toLong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->long",
    "KtCore.Katie.QString.toLongLong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->long long",
    "KtCore.Katie.QString.toLower()->KtCore.Katie.QString",
    "KtCore.Katie.QString.toShort(ok:KtCore.bool=Q_NULLPTR,base:int=10)->short",
    "KtCore.Katie.QString.toStdString()->std.string",
    "KtCore.Katie.QString.toUInt(ok:KtCore.bool=Q_NULLPTR,base:int=10)->unsigned int",
    "KtCore.Katie.QString.toULong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->ulong",
    "KtCore.Katie.QString.toULongLong(ok:KtCore.bool=Q_NULLPTR,base:int=10)->unsigned long long",
    "KtCore.Katie.QString.toUShort(ok:KtCore.bool=Q_NULLPTR,base:int=10)->ushort",
    "KtCore.Katie.QString.toUpper()->KtCore.Katie.QString",
    "KtCore.Katie.QString.toUtf8()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QString.trimmed()->KtCore.Katie.QString",
    "KtCore.Katie.QString.truncate(pos:int)",
    "KtCore.Katie.QString.unicode()->KtCore.Katie.QChar",
    "KtCore.Katie.QString.utf16()->KtCore.ushort",
    "KtCore.Katie.QString.__copy__()",
    "6:KtCore.Katie.QString.__iadd__(c:KtCore.Katie.QChar)->KtCore.Katie.QString",
    "5:KtCore.Katie.QString.__iadd__(c:KtCore.Katie.QChar.SpecialCharacter)->KtCore.Katie.QString",
    "4:KtCore.Katie.QString.__iadd__(c:char)->KtCore.Katie.QString",
    "3:KtCore.Katie.QString.__iadd__(s:KtCore.Katie.QByteArray)->KtCore.Katie.QString",
    "2:KtCore.Katie.QString.__iadd__(s:KtCore.Katie.QLatin1String)->KtCore.Katie.QString",
    "1:KtCore.Katie.QString.__iadd__(s:KtCore.Katie.QString)->KtCore.Katie.QString",
    "0:KtCore.Katie.QString.__iadd__(s:str)->KtCore.Katie.QString",
    nullptr}; // Sentinel

void init_Katie_QString(PyObject* module)
{
    _Sbk_Katie_QString_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QString",
        "Katie::QString",
        &Sbk_Katie_QString_spec,
        Katie_QString_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QString >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QString_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QString_TypeF(),
        QString_PythonToCpp_QString_PTR,
        is_QString_PythonToCpp_QString_PTR_Convertible,
        QString_PTR_CppToPython_QString,
        QString_COPY_CppToPython_QString);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QString");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QString*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QString&");
    Shiboken::Conversions::registerConverterName(converter, "QString");
    Shiboken::Conversions::registerConverterName(converter, "QString*");
    Shiboken::Conversions::registerConverterName(converter, "QString&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QString).name());

    // Add Python to C++ copy (value, not pointer neither reference) conversion to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        QString_PythonToCpp_QString_COPY,
        is_QString_PythonToCpp_QString_COPY_Convertible);
    // Add implicit conversions to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QByteArrayREF_PythonToCpp_Katie_QString,
        is_constKatie_QByteArrayREF_PythonToCpp_Katie_QString_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QCharPTR_PythonToCpp_Katie_QString,
        is_constKatie_QCharPTR_PythonToCpp_Katie_QString_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QChar_PythonToCpp_Katie_QString,
        is_constKatie_QChar_PythonToCpp_Katie_QString_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QLatin1StringREF_PythonToCpp_Katie_QString,
        is_constKatie_QLatin1StringREF_PythonToCpp_Katie_QString_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constcharPTR_PythonToCpp_Katie_QString,
        is_constcharPTR_PythonToCpp_Katie_QString_Convertible);

    // Initialization of enums.

    // Initialization of enum 'SectionFlag'.
    SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QString_TypeF(),
        "SectionFlag",
        "KtCore.Katie.QString.SectionFlag",
        "Katie::QString::SectionFlag");
    if (!SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX],
        Sbk_Katie_QString_TypeF(), "SectionDefault", (long) Katie::QString::SectionFlag::SectionDefault))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX],
        Sbk_Katie_QString_TypeF(), "SectionSkipEmpty", (long) Katie::QString::SectionFlag::SectionSkipEmpty))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX],
        Sbk_Katie_QString_TypeF(), "SectionIncludeLeadingSep", (long) Katie::QString::SectionFlag::SectionIncludeLeadingSep))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX],
        Sbk_Katie_QString_TypeF(), "SectionIncludeTrailingSep", (long) Katie::QString::SectionFlag::SectionIncludeTrailingSep))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX],
        Sbk_Katie_QString_TypeF(), "SectionCaseInsensitiveSeps", (long) Katie::QString::SectionFlag::SectionCaseInsensitiveSeps))
        return;
    // Register converter for enum 'Katie::QString::SectionFlag'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX],
            Katie_QString_SectionFlag_CppToPython_Katie_QString_SectionFlag);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QString_SectionFlag_PythonToCpp_Katie_QString_SectionFlag,
            is_Katie_QString_SectionFlag_PythonToCpp_Katie_QString_SectionFlag_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QSTRING_SECTIONFLAG_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QString::SectionFlag");
        Shiboken::Conversions::registerConverterName(converter, "QString::SectionFlag");
        Shiboken::Conversions::registerConverterName(converter, "SectionFlag");
    }
    // End of 'SectionFlag' enum.

    // Initialization of enum 'SplitBehavior'.
    SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QString_TypeF(),
        "SplitBehavior",
        "KtCore.Katie.QString.SplitBehavior",
        "Katie::QString::SplitBehavior");
    if (!SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX],
        Sbk_Katie_QString_TypeF(), "KeepEmptyParts", (long) Katie::QString::SplitBehavior::KeepEmptyParts))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX],
        Sbk_Katie_QString_TypeF(), "SkipEmptyParts", (long) Katie::QString::SplitBehavior::SkipEmptyParts))
        return;
    // Register converter for enum 'Katie::QString::SplitBehavior'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX],
            Katie_QString_SplitBehavior_CppToPython_Katie_QString_SplitBehavior);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QString_SplitBehavior_PythonToCpp_Katie_QString_SplitBehavior,
            is_Katie_QString_SplitBehavior_PythonToCpp_Katie_QString_SplitBehavior_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QSTRING_SPLITBEHAVIOR_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QString::SplitBehavior");
        Shiboken::Conversions::registerConverterName(converter, "QString::SplitBehavior");
        Shiboken::Conversions::registerConverterName(converter, "SplitBehavior");
    }
    // End of 'SplitBehavior' enum.

    // Initialization of enum 'NormalizationForm'.
    SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QString_TypeF(),
        "NormalizationForm",
        "KtCore.Katie.QString.NormalizationForm",
        "Katie::QString::NormalizationForm");
    if (!SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX],
        Sbk_Katie_QString_TypeF(), "NormalizationForm_D", (long) Katie::QString::NormalizationForm::NormalizationForm_D))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX],
        Sbk_Katie_QString_TypeF(), "NormalizationForm_C", (long) Katie::QString::NormalizationForm::NormalizationForm_C))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX],
        Sbk_Katie_QString_TypeF(), "NormalizationForm_KD", (long) Katie::QString::NormalizationForm::NormalizationForm_KD))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX],
        Sbk_Katie_QString_TypeF(), "NormalizationForm_KC", (long) Katie::QString::NormalizationForm::NormalizationForm_KC))
        return;
    // Register converter for enum 'Katie::QString::NormalizationForm'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX],
            Katie_QString_NormalizationForm_CppToPython_Katie_QString_NormalizationForm);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QString_NormalizationForm_PythonToCpp_Katie_QString_NormalizationForm,
            is_Katie_QString_NormalizationForm_PythonToCpp_Katie_QString_NormalizationForm_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QSTRING_NORMALIZATIONFORM_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QString::NormalizationForm");
        Shiboken::Conversions::registerConverterName(converter, "QString::NormalizationForm");
        Shiboken::Conversions::registerConverterName(converter, "NormalizationForm");
    }
    // End of 'NormalizationForm' enum.


}
