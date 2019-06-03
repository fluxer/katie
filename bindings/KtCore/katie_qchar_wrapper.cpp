/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
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
#include "katie_qchar_wrapper.h"

// inner classes

// Extra includes
#include <qchar.h>
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
Sbk_Katie_QChar_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QChar >()))
        return -1;

    ::Katie::QChar* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QChar", 0, 2, &(pyArgs[0]), &(pyArgs[1])))
        return -1;


    // Overloaded function decisor
    // 0: QChar::QChar()
    // 1: QChar::QChar(Katie::QChar)
    // 2: QChar::QChar(Katie::QChar::SpecialCharacter)
    // 3: QChar::QChar(Katie::QLatin1Char)
    // 4: QChar::QChar(char)
    // 5: QChar::QChar(int)
    // 6: QChar::QChar(short)
    // 7: QChar::QChar(uchar)
    // 8: QChar::QChar(uchar,uchar)
    // 9: QChar::QChar(uint)
    // 10: QChar::QChar(ushort)
    if (numArgs == 0) {
        overloadId = 0; // QChar()
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 10; // QChar(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1CHAR_IDX]), (pyArgs[0])))) {
        overloadId = 3; // QChar(Katie::QLatin1Char)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX])->converter, (pyArgs[0])))) {
        overloadId = 2; // QChar(Katie::QChar::SpecialCharacter)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 9; // QChar(uint)
    } else if (SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uchar>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 7; // QChar(uchar)
        } else if (numArgs == 2
            && SbkChar_Check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uchar>(), (pyArgs[1])))) {
            overloadId = 8; // QChar(uchar,uchar)
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<short>(), (pyArgs[0])))) {
        overloadId = 6; // QChar(short)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        overloadId = 5; // QChar(int)
    } else if (numArgs == 1
        && SbkChar_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArgs[0])))) {
        overloadId = 4; // QChar(char)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        overloadId = 1; // QChar(Katie::QChar)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QChar_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QChar()
        {

            if (!PyErr_Occurred()) {
                // QChar()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QChar(const Katie::QChar & QChar)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QChar cppArg0_local;
            ::Katie::QChar* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // QChar(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // QChar(const Katie::QChar::SpecialCharacter sc)
        {
            ::Katie::QChar::SpecialCharacter cppArg0{Katie::QChar::Null};
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(Katie::QChar::SpecialCharacter)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 3: // QChar(const Katie::QLatin1Char ch)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QLatin1Char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(Katie::QLatin1Char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 4: // QChar(const char c)
        {
            char cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(char)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 5: // QChar(const int rc)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 6: // QChar(const short rc)
        {
            short cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(short)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 7: // QChar(const uchar c)
        {
            uchar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(uchar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 8: // QChar(const uchar c, uchar r)
        {
            uchar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            uchar cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QChar(uchar,uchar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 9: // QChar(const uint rc)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 10: // QChar(const ushort rc)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QChar(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::Katie::QChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QChar >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QChar_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QChar_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar");
        return -1;
}

static PyObject* Sbk_Katie_QCharFunc_category(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "category", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::category()const
    // 1: static QChar::category(uint)
    // 2: static QChar::category(ushort)
    if (numArgs == 0) {
        overloadId = 0; // category()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // category(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // category(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_category_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // category() const
        {

            if (!PyErr_Occurred()) {
                // category()const
                Katie::QChar::Category cppResult = Katie::QChar::Category(const_cast<const ::Katie::QChar*>(cppSelf)->category());
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX])->converter, &cppResult);
            }
            break;
        }
        case 1: // category(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // category(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::Category cppResult = Katie::QChar::Category(::Katie::QChar::category(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX])->converter, &cppResult);
            }
            break;
        }
        case 2: // category(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // category(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::Category cppResult = Katie::QChar::Category(::Katie::QChar::category(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX])->converter, &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_category_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.category");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_cell(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // cell()const
            uchar cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->cell();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uchar>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_combiningClass(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "combiningClass", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::combiningClass()const
    // 1: static QChar::combiningClass(uint)
    // 2: static QChar::combiningClass(ushort)
    if (numArgs == 0) {
        overloadId = 0; // combiningClass()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // combiningClass(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // combiningClass(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_combiningClass_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // combiningClass() const
        {

            if (!PyErr_Occurred()) {
                // combiningClass()const
                unsigned char cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->combiningClass();
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<unsigned char>(), &cppResult);
            }
            break;
        }
        case 1: // combiningClass(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // combiningClass(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                unsigned char cppResult = ::Katie::QChar::combiningClass(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<unsigned char>(), &cppResult);
            }
            break;
        }
        case 2: // combiningClass(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // combiningClass(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                unsigned char cppResult = ::Katie::QChar::combiningClass(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<unsigned char>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_combiningClass_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.combiningClass");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_currentUnicodeVersion(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // currentUnicodeVersion()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QChar::UnicodeVersion cppResult = Katie::QChar::UnicodeVersion(::Katie::QChar::currentUnicodeVersion());
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX])->converter, &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_decomposition(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "decomposition", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::decomposition()const
    // 1: static QChar::decomposition(uint)
    if (numArgs == 0) {
        overloadId = 0; // decomposition()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // decomposition(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_decomposition_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // decomposition() const
        {

            if (!PyErr_Occurred()) {
                // decomposition()const
                Katie::QString cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->decomposition();
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
            }
            break;
        }
        case 1: // decomposition(uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // decomposition(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QString cppResult = ::Katie::QChar::decomposition(cppArg0);
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

    Sbk_Katie_QCharFunc_decomposition_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.decomposition");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_decompositionTag(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "decompositionTag", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::decompositionTag()const
    // 1: static QChar::decompositionTag(uint)
    if (numArgs == 0) {
        overloadId = 0; // decompositionTag()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // decompositionTag(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_decompositionTag_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // decompositionTag() const
        {

            if (!PyErr_Occurred()) {
                // decompositionTag()const
                Katie::QChar::Decomposition cppResult = Katie::QChar::Decomposition(const_cast<const ::Katie::QChar*>(cppSelf)->decompositionTag());
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX])->converter, &cppResult);
            }
            break;
        }
        case 1: // decompositionTag(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // decompositionTag(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::Decomposition cppResult = Katie::QChar::Decomposition(::Katie::QChar::decompositionTag(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX])->converter, &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_decompositionTag_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.decompositionTag");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_digitValue(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "digitValue", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::digitValue()const
    // 1: static QChar::digitValue(uint)
    // 2: static QChar::digitValue(ushort)
    if (numArgs == 0) {
        overloadId = 0; // digitValue()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // digitValue(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // digitValue(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_digitValue_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // digitValue() const
        {

            if (!PyErr_Occurred()) {
                // digitValue()const
                int cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->digitValue();
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 1: // digitValue(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // digitValue(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = ::Katie::QChar::digitValue(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
            }
            break;
        }
        case 2: // digitValue(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // digitValue(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                int cppResult = ::Katie::QChar::digitValue(cppArg0);
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

    Sbk_Katie_QCharFunc_digitValue_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.digitValue");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_direction(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "direction", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::direction()const
    // 1: static QChar::direction(uint)
    // 2: static QChar::direction(ushort)
    if (numArgs == 0) {
        overloadId = 0; // direction()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // direction(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // direction(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_direction_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // direction() const
        {

            if (!PyErr_Occurred()) {
                // direction()const
                Katie::QChar::Direction cppResult = Katie::QChar::Direction(const_cast<const ::Katie::QChar*>(cppSelf)->direction());
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX])->converter, &cppResult);
            }
            break;
        }
        case 1: // direction(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // direction(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::Direction cppResult = Katie::QChar::Direction(::Katie::QChar::direction(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX])->converter, &cppResult);
            }
            break;
        }
        case 2: // direction(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // direction(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::Direction cppResult = Katie::QChar::Direction(::Katie::QChar::direction(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX])->converter, &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_direction_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.direction");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_fromAscii(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QChar::fromAscii(char)
    if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // fromAscii(char)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_fromAscii_TypeError;

    // Call function/method
    {
        char cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // fromAscii(char)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QChar cppResult = ::Katie::QChar::fromAscii(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_fromAscii_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QChar.fromAscii");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_fromLatin1(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QChar::fromLatin1(char)
    if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<char>(), (pyArg)))) {
        overloadId = 0; // fromLatin1(char)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_fromLatin1_TypeError;

    // Call function/method
    {
        char cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // fromLatin1(char)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QChar cppResult = ::Katie::QChar::fromLatin1(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_fromLatin1_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QChar.fromLatin1");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_hasMirrored(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // hasMirrored()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->hasMirrored();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_highSurrogate(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QChar::highSurrogate(uint)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArg)))) {
        overloadId = 0; // highSurrogate(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_highSurrogate_TypeError;

    // Call function/method
    {
        uint cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // highSurrogate(uint)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ushort cppResult = ::Katie::QChar::highSurrogate(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_highSurrogate_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QChar.highSurrogate");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_isDigit(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isDigit()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isDigit();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isHighSurrogate(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "isHighSurrogate", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::isHighSurrogate()const
    // 1: static QChar::isHighSurrogate(uint)
    if (numArgs == 0) {
        overloadId = 0; // isHighSurrogate()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // isHighSurrogate(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_isHighSurrogate_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // isHighSurrogate() const
        {

            if (!PyErr_Occurred()) {
                // isHighSurrogate()const
                bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isHighSurrogate();
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // isHighSurrogate(uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // isHighSurrogate(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = ::Katie::QChar::isHighSurrogate(cppArg0);
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

    Sbk_Katie_QCharFunc_isHighSurrogate_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.isHighSurrogate");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_isLetter(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isLetter()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isLetter();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isLetterOrNumber(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isLetterOrNumber()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isLetterOrNumber();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isLowSurrogate(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "isLowSurrogate", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::isLowSurrogate()const
    // 1: static QChar::isLowSurrogate(uint)
    if (numArgs == 0) {
        overloadId = 0; // isLowSurrogate()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // isLowSurrogate(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_isLowSurrogate_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // isLowSurrogate() const
        {

            if (!PyErr_Occurred()) {
                // isLowSurrogate()const
                bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isLowSurrogate();
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
            }
            break;
        }
        case 1: // isLowSurrogate(uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // isLowSurrogate(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                bool cppResult = ::Katie::QChar::isLowSurrogate(cppArg0);
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

    Sbk_Katie_QCharFunc_isLowSurrogate_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.isLowSurrogate");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_isLower(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isLower()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isLower();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isMark(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isMark()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isMark();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isNull(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isNull()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isNull();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isNumber(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isNumber()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isNumber();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isPrint(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isPrint()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isPrint();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isPunct(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isPunct()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isPunct();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isSpace(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isSpace()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isSpace();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isSymbol(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isSymbol()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isSymbol();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isTitleCase(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isTitleCase()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isTitleCase();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_isUpper(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isUpper()const
            bool cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->isUpper();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_joining(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "joining", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::joining()const
    // 1: static QChar::joining(uint)
    // 2: static QChar::joining(ushort)
    if (numArgs == 0) {
        overloadId = 0; // joining()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // joining(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // joining(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_joining_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // joining() const
        {

            if (!PyErr_Occurred()) {
                // joining()const
                Katie::QChar::Joining cppResult = Katie::QChar::Joining(const_cast<const ::Katie::QChar*>(cppSelf)->joining());
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX])->converter, &cppResult);
            }
            break;
        }
        case 1: // joining(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // joining(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::Joining cppResult = Katie::QChar::Joining(::Katie::QChar::joining(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX])->converter, &cppResult);
            }
            break;
        }
        case 2: // joining(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // joining(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::Joining cppResult = Katie::QChar::Joining(::Katie::QChar::joining(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX])->converter, &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_joining_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.joining");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_lowSurrogate(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QChar::lowSurrogate(uint)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArg)))) {
        overloadId = 0; // lowSurrogate(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_lowSurrogate_TypeError;

    // Call function/method
    {
        uint cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // lowSurrogate(uint)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ushort cppResult = ::Katie::QChar::lowSurrogate(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_lowSurrogate_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QChar.lowSurrogate");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_mirroredChar(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "mirroredChar", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::mirroredChar()const
    // 1: static QChar::mirroredChar(uint)
    // 2: static QChar::mirroredChar(ushort)
    if (numArgs == 0) {
        overloadId = 0; // mirroredChar()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // mirroredChar(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // mirroredChar(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_mirroredChar_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // mirroredChar() const
        {

            if (!PyErr_Occurred()) {
                // mirroredChar()const
                Katie::QChar cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->mirroredChar();
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
            }
            break;
        }
        case 1: // mirroredChar(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // mirroredChar(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                uint cppResult = ::Katie::QChar::mirroredChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
            }
            break;
        }
        case 2: // mirroredChar(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // mirroredChar(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ushort cppResult = ::Katie::QChar::mirroredChar(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_mirroredChar_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.mirroredChar");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_requiresSurrogates(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QChar::requiresSurrogates(uint)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArg)))) {
        overloadId = 0; // requiresSurrogates(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_requiresSurrogates_TypeError;

    // Call function/method
    {
        uint cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // requiresSurrogates(uint)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = ::Katie::QChar::requiresSurrogates(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_requiresSurrogates_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QChar.requiresSurrogates");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_row(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // row()const
            uchar cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->row();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uchar>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_setCell(PyObject* self, PyObject* pyArg)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QChar::setCell(uchar)
    if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uchar>(), (pyArg)))) {
        overloadId = 0; // setCell(uchar)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_setCell_TypeError;

    // Call function/method
    {
        uchar cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setCell(uchar)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setCell(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCharFunc_setCell_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QChar.setCell");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_setRow(PyObject* self, PyObject* pyArg)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QChar::setRow(uchar)
    if (SbkChar_Check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uchar>(), (pyArg)))) {
        overloadId = 0; // setRow(uchar)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_setRow_TypeError;

    // Call function/method
    {
        uchar cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // setRow(uchar)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->setRow(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCharFunc_setRow_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QChar.setRow");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_surrogateToUcs4(PyObject* self, PyObject* args)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "surrogateToUcs4", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QChar::surrogateToUcs4(Katie::QChar,Katie::QChar)
    // 1: static QChar::surrogateToUcs4(ushort,ushort)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[1])))) {
        overloadId = 1; // surrogateToUcs4(ushort,ushort)
    } else if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppValueConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[1])))) {
        overloadId = 0; // surrogateToUcs4(Katie::QChar,Katie::QChar)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_surrogateToUcs4_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // surrogateToUcs4(Katie::QChar high, Katie::QChar low)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QChar cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            if (!Shiboken::Object::isValid(pyArgs[1]))
                return {};
            ::Katie::QChar cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // surrogateToUcs4(Katie::QChar,Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                uint cppResult = ::Katie::QChar::surrogateToUcs4(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
            }
            break;
        }
        case 1: // surrogateToUcs4(ushort high, ushort low)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            ::ushort cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // surrogateToUcs4(ushort,ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                uint cppResult = ::Katie::QChar::surrogateToUcs4(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_surrogateToUcs4_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.surrogateToUcs4");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_toAscii(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toAscii()const
            char cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->toAscii();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<char>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_toCaseFolded(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "toCaseFolded", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::toCaseFolded()const
    // 1: static QChar::toCaseFolded(uint)
    // 2: static QChar::toCaseFolded(ushort)
    if (numArgs == 0) {
        overloadId = 0; // toCaseFolded()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // toCaseFolded(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // toCaseFolded(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_toCaseFolded_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // toCaseFolded() const
        {

            if (!PyErr_Occurred()) {
                // toCaseFolded()const
                Katie::QChar cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->toCaseFolded();
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
            }
            break;
        }
        case 1: // toCaseFolded(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toCaseFolded(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                uint cppResult = ::Katie::QChar::toCaseFolded(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
            }
            break;
        }
        case 2: // toCaseFolded(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toCaseFolded(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ushort cppResult = ::Katie::QChar::toCaseFolded(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_toCaseFolded_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.toCaseFolded");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_toLatin1(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toLatin1()const
            char cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->toLatin1();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<char>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_toLower(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "toLower", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::toLower()const
    // 1: static QChar::toLower(uint)
    // 2: static QChar::toLower(ushort)
    if (numArgs == 0) {
        overloadId = 0; // toLower()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // toLower(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // toLower(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_toLower_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // toLower() const
        {

            if (!PyErr_Occurred()) {
                // toLower()const
                Katie::QChar cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->toLower();
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
            }
            break;
        }
        case 1: // toLower(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toLower(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                uint cppResult = ::Katie::QChar::toLower(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
            }
            break;
        }
        case 2: // toLower(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toLower(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ushort cppResult = ::Katie::QChar::toLower(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_toLower_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.toLower");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_toTitleCase(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "toTitleCase", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::toTitleCase()const
    // 1: static QChar::toTitleCase(uint)
    // 2: static QChar::toTitleCase(ushort)
    if (numArgs == 0) {
        overloadId = 0; // toTitleCase()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // toTitleCase(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // toTitleCase(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_toTitleCase_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // toTitleCase() const
        {

            if (!PyErr_Occurred()) {
                // toTitleCase()const
                Katie::QChar cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->toTitleCase();
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
            }
            break;
        }
        case 1: // toTitleCase(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toTitleCase(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                uint cppResult = ::Katie::QChar::toTitleCase(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
            }
            break;
        }
        case 2: // toTitleCase(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toTitleCase(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ushort cppResult = ::Katie::QChar::toTitleCase(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_toTitleCase_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.toTitleCase");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_toUpper(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "toUpper", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::toUpper()const
    // 1: static QChar::toUpper(uint)
    // 2: static QChar::toUpper(ushort)
    if (numArgs == 0) {
        overloadId = 0; // toUpper()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // toUpper(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // toUpper(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_toUpper_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // toUpper() const
        {

            if (!PyErr_Occurred()) {
                // toUpper()const
                Katie::QChar cppResult = const_cast<const ::Katie::QChar*>(cppSelf)->toUpper();
                pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
            }
            break;
        }
        case 1: // toUpper(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toUpper(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                uint cppResult = ::Katie::QChar::toUpper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
            }
            break;
        }
        case 2: // toUpper(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // toUpper(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ushort cppResult = ::Katie::QChar::toUpper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_toUpper_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.toUpper");
        return {};
}

static PyObject* Sbk_Katie_QCharFunc_unicode(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // unicode()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ushort & cppResult = cppSelf->unicode();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCharFunc_unicodeVersion(PyObject* self, PyObject* args)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (self) {
        if (!Shiboken::Object::isValid(self))
            return {};
        cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    }
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "unicodeVersion", 0, 1, &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QChar::unicodeVersion()const
    // 1: static QChar::unicodeVersion(uint)
    // 2: static QChar::unicodeVersion(ushort)
    if (numArgs == 0) {
        overloadId = 0; // unicodeVersion()const
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), (pyArgs[0])))) {
        overloadId = 2; // unicodeVersion(ushort)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 1; // unicodeVersion(uint)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCharFunc_unicodeVersion_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // unicodeVersion() const
        {

            if (!PyErr_Occurred()) {
                // unicodeVersion()const
                Katie::QChar::UnicodeVersion cppResult = Katie::QChar::UnicodeVersion(const_cast<const ::Katie::QChar*>(cppSelf)->unicodeVersion());
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX])->converter, &cppResult);
            }
            break;
        }
        case 1: // unicodeVersion(const uint ucs4)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // unicodeVersion(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::UnicodeVersion cppResult = Katie::QChar::UnicodeVersion(::Katie::QChar::unicodeVersion(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX])->converter, &cppResult);
            }
            break;
        }
        case 2: // unicodeVersion(const ushort ucs2)
        {
            ::ushort cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // unicodeVersion(ushort)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                Katie::QChar::UnicodeVersion cppResult = Katie::QChar::UnicodeVersion(::Katie::QChar::unicodeVersion(cppArg0));
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
                pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX])->converter, &cppResult);
            }
            break;
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCharFunc_unicodeVersion_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QChar.unicodeVersion");
        return {};
}

static PyObject* Sbk_Katie_QChar___copy__(PyObject* self)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    ::Katie::QChar& cppSelf = *reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    PyObject* pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppSelf);
    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QCharMethod_category = {
    "category", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_category), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_combiningClass = {
    "combiningClass", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_combiningClass), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_decomposition = {
    "decomposition", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_decomposition), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_decompositionTag = {
    "decompositionTag", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_decompositionTag), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_digitValue = {
    "digitValue", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_digitValue), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_direction = {
    "direction", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_direction), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_isHighSurrogate = {
    "isHighSurrogate", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isHighSurrogate), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_isLowSurrogate = {
    "isLowSurrogate", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isLowSurrogate), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_joining = {
    "joining", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_joining), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_mirroredChar = {
    "mirroredChar", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_mirroredChar), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_toCaseFolded = {
    "toCaseFolded", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_toCaseFolded), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_toLower = {
    "toLower", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_toLower), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_toTitleCase = {
    "toTitleCase", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_toTitleCase), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_toUpper = {
    "toUpper", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_toUpper), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QCharMethod_unicodeVersion = {
    "unicodeVersion", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_unicodeVersion), METH_VARARGS|METH_STATIC
};

static PyMethodDef Sbk_Katie_QChar_methods[] = {
    Sbk_Katie_QCharMethod_category,
    {"cell", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_cell), METH_NOARGS},
    Sbk_Katie_QCharMethod_combiningClass,
    {"currentUnicodeVersion", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_currentUnicodeVersion), METH_NOARGS|METH_STATIC},
    Sbk_Katie_QCharMethod_decomposition,
    Sbk_Katie_QCharMethod_decompositionTag,
    Sbk_Katie_QCharMethod_digitValue,
    Sbk_Katie_QCharMethod_direction,
    {"fromAscii", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_fromAscii), METH_O|METH_STATIC},
    {"fromLatin1", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_fromLatin1), METH_O|METH_STATIC},
    {"hasMirrored", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_hasMirrored), METH_NOARGS},
    {"highSurrogate", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_highSurrogate), METH_O|METH_STATIC},
    {"isDigit", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isDigit), METH_NOARGS},
    Sbk_Katie_QCharMethod_isHighSurrogate,
    {"isLetter", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isLetter), METH_NOARGS},
    {"isLetterOrNumber", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isLetterOrNumber), METH_NOARGS},
    Sbk_Katie_QCharMethod_isLowSurrogate,
    {"isLower", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isLower), METH_NOARGS},
    {"isMark", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isMark), METH_NOARGS},
    {"isNull", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isNull), METH_NOARGS},
    {"isNumber", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isNumber), METH_NOARGS},
    {"isPrint", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isPrint), METH_NOARGS},
    {"isPunct", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isPunct), METH_NOARGS},
    {"isSpace", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isSpace), METH_NOARGS},
    {"isSymbol", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isSymbol), METH_NOARGS},
    {"isTitleCase", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isTitleCase), METH_NOARGS},
    {"isUpper", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_isUpper), METH_NOARGS},
    Sbk_Katie_QCharMethod_joining,
    {"lowSurrogate", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_lowSurrogate), METH_O|METH_STATIC},
    Sbk_Katie_QCharMethod_mirroredChar,
    {"requiresSurrogates", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_requiresSurrogates), METH_O|METH_STATIC},
    {"row", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_row), METH_NOARGS},
    {"setCell", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_setCell), METH_O},
    {"setRow", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_setRow), METH_O},
    {"surrogateToUcs4", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_surrogateToUcs4), METH_VARARGS|METH_STATIC},
    {"toAscii", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_toAscii), METH_NOARGS},
    Sbk_Katie_QCharMethod_toCaseFolded,
    {"toLatin1", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_toLatin1), METH_NOARGS},
    Sbk_Katie_QCharMethod_toLower,
    Sbk_Katie_QCharMethod_toTitleCase,
    Sbk_Katie_QCharMethod_toUpper,
    {"unicode", reinterpret_cast<PyCFunction>(Sbk_Katie_QCharFunc_unicode), METH_NOARGS},
    Sbk_Katie_QCharMethod_unicodeVersion,

    {"__copy__", reinterpret_cast<PyCFunction>(Sbk_Katie_QChar___copy__), METH_NOARGS},
    {nullptr, nullptr} // Sentinel
};

static PyObject* Sbk_Katie_QChar_getattro(PyObject* self, PyObject* name)
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
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_category = {
            Sbk_Katie_QCharMethod_category.ml_name,
            Sbk_Katie_QCharMethod_category.ml_meth,
            Sbk_Katie_QCharMethod_category.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_category.ml_doc,
        };
        if (Shiboken::String::compare(name, "category") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_category, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_combiningClass = {
            Sbk_Katie_QCharMethod_combiningClass.ml_name,
            Sbk_Katie_QCharMethod_combiningClass.ml_meth,
            Sbk_Katie_QCharMethod_combiningClass.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_combiningClass.ml_doc,
        };
        if (Shiboken::String::compare(name, "combiningClass") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_combiningClass, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_decomposition = {
            Sbk_Katie_QCharMethod_decomposition.ml_name,
            Sbk_Katie_QCharMethod_decomposition.ml_meth,
            Sbk_Katie_QCharMethod_decomposition.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_decomposition.ml_doc,
        };
        if (Shiboken::String::compare(name, "decomposition") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_decomposition, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_decompositionTag = {
            Sbk_Katie_QCharMethod_decompositionTag.ml_name,
            Sbk_Katie_QCharMethod_decompositionTag.ml_meth,
            Sbk_Katie_QCharMethod_decompositionTag.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_decompositionTag.ml_doc,
        };
        if (Shiboken::String::compare(name, "decompositionTag") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_decompositionTag, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_digitValue = {
            Sbk_Katie_QCharMethod_digitValue.ml_name,
            Sbk_Katie_QCharMethod_digitValue.ml_meth,
            Sbk_Katie_QCharMethod_digitValue.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_digitValue.ml_doc,
        };
        if (Shiboken::String::compare(name, "digitValue") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_digitValue, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_direction = {
            Sbk_Katie_QCharMethod_direction.ml_name,
            Sbk_Katie_QCharMethod_direction.ml_meth,
            Sbk_Katie_QCharMethod_direction.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_direction.ml_doc,
        };
        if (Shiboken::String::compare(name, "direction") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_direction, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_isHighSurrogate = {
            Sbk_Katie_QCharMethod_isHighSurrogate.ml_name,
            Sbk_Katie_QCharMethod_isHighSurrogate.ml_meth,
            Sbk_Katie_QCharMethod_isHighSurrogate.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_isHighSurrogate.ml_doc,
        };
        if (Shiboken::String::compare(name, "isHighSurrogate") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_isHighSurrogate, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_isLowSurrogate = {
            Sbk_Katie_QCharMethod_isLowSurrogate.ml_name,
            Sbk_Katie_QCharMethod_isLowSurrogate.ml_meth,
            Sbk_Katie_QCharMethod_isLowSurrogate.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_isLowSurrogate.ml_doc,
        };
        if (Shiboken::String::compare(name, "isLowSurrogate") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_isLowSurrogate, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_joining = {
            Sbk_Katie_QCharMethod_joining.ml_name,
            Sbk_Katie_QCharMethod_joining.ml_meth,
            Sbk_Katie_QCharMethod_joining.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_joining.ml_doc,
        };
        if (Shiboken::String::compare(name, "joining") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_joining, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_mirroredChar = {
            Sbk_Katie_QCharMethod_mirroredChar.ml_name,
            Sbk_Katie_QCharMethod_mirroredChar.ml_meth,
            Sbk_Katie_QCharMethod_mirroredChar.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_mirroredChar.ml_doc,
        };
        if (Shiboken::String::compare(name, "mirroredChar") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_mirroredChar, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_toCaseFolded = {
            Sbk_Katie_QCharMethod_toCaseFolded.ml_name,
            Sbk_Katie_QCharMethod_toCaseFolded.ml_meth,
            Sbk_Katie_QCharMethod_toCaseFolded.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_toCaseFolded.ml_doc,
        };
        if (Shiboken::String::compare(name, "toCaseFolded") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_toCaseFolded, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_toLower = {
            Sbk_Katie_QCharMethod_toLower.ml_name,
            Sbk_Katie_QCharMethod_toLower.ml_meth,
            Sbk_Katie_QCharMethod_toLower.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_toLower.ml_doc,
        };
        if (Shiboken::String::compare(name, "toLower") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_toLower, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_toTitleCase = {
            Sbk_Katie_QCharMethod_toTitleCase.ml_name,
            Sbk_Katie_QCharMethod_toTitleCase.ml_meth,
            Sbk_Katie_QCharMethod_toTitleCase.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_toTitleCase.ml_doc,
        };
        if (Shiboken::String::compare(name, "toTitleCase") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_toTitleCase, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_toUpper = {
            Sbk_Katie_QCharMethod_toUpper.ml_name,
            Sbk_Katie_QCharMethod_toUpper.ml_meth,
            Sbk_Katie_QCharMethod_toUpper.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_toUpper.ml_doc,
        };
        if (Shiboken::String::compare(name, "toUpper") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_toUpper, self, 0);
        static PyMethodDef non_static_Sbk_Katie_QCharMethod_unicodeVersion = {
            Sbk_Katie_QCharMethod_unicodeVersion.ml_name,
            Sbk_Katie_QCharMethod_unicodeVersion.ml_meth,
            Sbk_Katie_QCharMethod_unicodeVersion.ml_flags & (~METH_STATIC),
            Sbk_Katie_QCharMethod_unicodeVersion.ml_doc,
        };
        if (Shiboken::String::compare(name, "unicodeVersion") == 0)
            return PyCFunction_NewEx(&non_static_Sbk_Katie_QCharMethod_unicodeVersion, self, 0);
    }
    return PyObject_GenericGetAttr(self, name);
}

static int Sbk_Katie_QChar___nb_bool(PyObject* self)
{
    ::Katie::QChar* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return -1;
    cppSelf = reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(self)));
    return !cppSelf->isNull();
}

} // extern "C"

static int Sbk_Katie_QChar_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QChar_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QChar_Type = nullptr;
static SbkObjectType *Sbk_Katie_QChar_TypeF(void)
{
    return _Sbk_Katie_QChar_Type;
}

static PyType_Slot Sbk_Katie_QChar_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    reinterpret_cast<void*>(Sbk_Katie_QChar_getattro)},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QChar_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QChar_clear)},
    {Py_tp_richcompare, nullptr},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QChar_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QChar_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    // type supports number protocol
#ifdef IS_PY3K
    {Py_nb_bool, (void *)Sbk_Katie_QChar___nb_bool},
#else
    {Py_nb_nonzero, (void *)Sbk_Katie_QChar___nb_bool},
#endif
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QChar_spec = {
    "KtCore.Katie.QChar",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QChar_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ enum conversion.
static void Katie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar_SpecialCharacter(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar::SpecialCharacter*>(cppOut) =
        static_cast<::Katie::QChar::SpecialCharacter>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar_SpecialCharacter_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX]))
        return Katie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar_SpecialCharacter;
    return {};
}
static PyObject* Katie_QChar_SpecialCharacter_CppToPython_Katie_QChar_SpecialCharacter(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QChar::SpecialCharacter *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX], castCppIn);

}

static void Katie_QChar_Category_PythonToCpp_Katie_QChar_Category(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar::Category*>(cppOut) =
        static_cast<::Katie::QChar::Category>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QChar_Category_PythonToCpp_Katie_QChar_Category_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX]))
        return Katie_QChar_Category_PythonToCpp_Katie_QChar_Category;
    return {};
}
static PyObject* Katie_QChar_Category_CppToPython_Katie_QChar_Category(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QChar::Category *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX], castCppIn);

}

static void Katie_QChar_Direction_PythonToCpp_Katie_QChar_Direction(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar::Direction*>(cppOut) =
        static_cast<::Katie::QChar::Direction>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QChar_Direction_PythonToCpp_Katie_QChar_Direction_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX]))
        return Katie_QChar_Direction_PythonToCpp_Katie_QChar_Direction;
    return {};
}
static PyObject* Katie_QChar_Direction_CppToPython_Katie_QChar_Direction(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QChar::Direction *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX], castCppIn);

}

static void Katie_QChar_Decomposition_PythonToCpp_Katie_QChar_Decomposition(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar::Decomposition*>(cppOut) =
        static_cast<::Katie::QChar::Decomposition>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QChar_Decomposition_PythonToCpp_Katie_QChar_Decomposition_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX]))
        return Katie_QChar_Decomposition_PythonToCpp_Katie_QChar_Decomposition;
    return {};
}
static PyObject* Katie_QChar_Decomposition_CppToPython_Katie_QChar_Decomposition(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QChar::Decomposition *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX], castCppIn);

}

static void Katie_QChar_Joining_PythonToCpp_Katie_QChar_Joining(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar::Joining*>(cppOut) =
        static_cast<::Katie::QChar::Joining>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QChar_Joining_PythonToCpp_Katie_QChar_Joining_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX]))
        return Katie_QChar_Joining_PythonToCpp_Katie_QChar_Joining;
    return {};
}
static PyObject* Katie_QChar_Joining_CppToPython_Katie_QChar_Joining(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QChar::Joining *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX], castCppIn);

}

static void Katie_QChar_CombiningClass_PythonToCpp_Katie_QChar_CombiningClass(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar::CombiningClass*>(cppOut) =
        static_cast<::Katie::QChar::CombiningClass>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QChar_CombiningClass_PythonToCpp_Katie_QChar_CombiningClass_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX]))
        return Katie_QChar_CombiningClass_PythonToCpp_Katie_QChar_CombiningClass;
    return {};
}
static PyObject* Katie_QChar_CombiningClass_CppToPython_Katie_QChar_CombiningClass(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QChar::CombiningClass *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX], castCppIn);

}

static void Katie_QChar_UnicodeVersion_PythonToCpp_Katie_QChar_UnicodeVersion(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar::UnicodeVersion*>(cppOut) =
        static_cast<::Katie::QChar::UnicodeVersion>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QChar_UnicodeVersion_PythonToCpp_Katie_QChar_UnicodeVersion_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX]))
        return Katie_QChar_UnicodeVersion_PythonToCpp_Katie_QChar_UnicodeVersion;
    return {};
}
static PyObject* Katie_QChar_UnicodeVersion_CppToPython_Katie_QChar_UnicodeVersion(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QChar::UnicodeVersion *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX], castCppIn);

}

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QChar_PythonToCpp_QChar_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QChar_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QChar_PythonToCpp_QChar_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QChar_TypeF())))
        return QChar_PythonToCpp_QChar_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QChar_PTR_CppToPython_QChar(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QChar *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QChar_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// C++ to Python copy conversion.
static PyObject* QChar_COPY_CppToPython_QChar(const void* cppIn) {
    return Shiboken::Object::newObject(Sbk_Katie_QChar_TypeF(), new ::Katie::QChar(*reinterpret_cast<const ::Katie::QChar*>(cppIn)), true, true);
}

// Python to C++ copy conversion.
static void QChar_PythonToCpp_QChar_COPY(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar*>(cppOut) = *reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(pyIn)));
}
static PythonToCppFunc is_QChar_PythonToCpp_QChar_COPY_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QChar_TypeF())))
        return QChar_PythonToCpp_QChar_COPY;
    return {};
}

// Implicit conversions.
static void constKatie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    ::Katie::QChar::SpecialCharacter cppIn{Katie::QChar::Null};
    Shiboken::Conversions::pythonToCppCopy(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX])->converter, pyIn, &cppIn);
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(cppIn);
}
static PythonToCppFunc is_constKatie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX], pyIn))
        return constKatie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar;
    return {};
}

static void constKatie_QLatin1Char_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(*reinterpret_cast< ::Katie::QLatin1Char *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1CHAR_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QLatin1Char_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QLATIN1CHAR_IDX], pyIn))
        return constKatie_QLatin1Char_PythonToCpp_Katie_QChar;
    return {};
}

static void constchar_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    char cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<char>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(cppIn);
}
static PythonToCppFunc is_constchar_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (SbkChar_Check(pyIn))
        return constchar_PythonToCpp_Katie_QChar;
    return {};
}

static void constint_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    int cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<int>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(cppIn);
}
static PythonToCppFunc is_constint_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (PyInt_Check(pyIn))
        return constint_PythonToCpp_Katie_QChar;
    return {};
}

static void constshort_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    short cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<short>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(cppIn);
}
static PythonToCppFunc is_constshort_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (PyInt_Check(pyIn))
        return constshort_PythonToCpp_Katie_QChar;
    return {};
}

static void constuchar_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    uchar cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<uchar>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(cppIn);
}
static PythonToCppFunc is_constuchar_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (SbkChar_Check(pyIn))
        return constuchar_PythonToCpp_Katie_QChar;
    return {};
}

static void constuint_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    uint cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(cppIn);
}
static PythonToCppFunc is_constuint_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (PyInt_Check(pyIn))
        return constuint_PythonToCpp_Katie_QChar;
    return {};
}

static void constushort_PythonToCpp_Katie_QChar(PyObject* pyIn, void* cppOut) {
    ::ushort cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QChar*>(cppOut) = ::Katie::QChar(cppIn);
}
static PythonToCppFunc is_constushort_PythonToCpp_Katie_QChar_Convertible(PyObject* pyIn) {
    if (Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<ushort>(), pyIn))
        return constushort_PythonToCpp_Katie_QChar;
    return {};
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QChar_SignatureStrings[] = {
    "10:KtCore.Katie.QChar()",
    "9:KtCore.Katie.QChar(QChar:KtCore.Katie.QChar)",
    "8:KtCore.Katie.QChar(sc:KtCore.Katie.QChar.SpecialCharacter)",
    "7:KtCore.Katie.QChar(ch:KtCore.Katie.QLatin1Char)",
    "6:KtCore.Katie.QChar(c:char)",
    "5:KtCore.Katie.QChar(rc:int)",
    "4:KtCore.Katie.QChar(rc:short)",
    "3:KtCore.Katie.QChar(c:unsigned char)",
    "2:KtCore.Katie.QChar(c:unsigned char,r:unsigned char)",
    "1:KtCore.Katie.QChar(rc:unsigned int)",
    "0:KtCore.Katie.QChar(rc:ushort)",
    "2:KtCore.Katie.QChar.category()->KtCore.Katie.QChar.Category",
    "1:KtCore.Katie.QChar.category(ucs4:unsigned int)->KtCore.Katie.QChar.Category",
    "0:KtCore.Katie.QChar.category(ucs2:ushort)->KtCore.Katie.QChar.Category",
    "KtCore.Katie.QChar.cell()->unsigned char",
    "2:KtCore.Katie.QChar.combiningClass()->unsigned char",
    "1:KtCore.Katie.QChar.combiningClass(ucs4:unsigned int)->unsigned char",
    "0:KtCore.Katie.QChar.combiningClass(ucs2:ushort)->unsigned char",
    "KtCore.Katie.QChar.currentUnicodeVersion()->KtCore.Katie.QChar.UnicodeVersion",
    "1:KtCore.Katie.QChar.decomposition()->KtCore.Katie.QString",
    "0:KtCore.Katie.QChar.decomposition(ucs4:unsigned int)->KtCore.Katie.QString",
    "1:KtCore.Katie.QChar.decompositionTag()->KtCore.Katie.QChar.Decomposition",
    "0:KtCore.Katie.QChar.decompositionTag(ucs4:unsigned int)->KtCore.Katie.QChar.Decomposition",
    "2:KtCore.Katie.QChar.digitValue()->int",
    "1:KtCore.Katie.QChar.digitValue(ucs4:unsigned int)->int",
    "0:KtCore.Katie.QChar.digitValue(ucs2:ushort)->int",
    "2:KtCore.Katie.QChar.direction()->KtCore.Katie.QChar.Direction",
    "1:KtCore.Katie.QChar.direction(ucs4:unsigned int)->KtCore.Katie.QChar.Direction",
    "0:KtCore.Katie.QChar.direction(ucs2:ushort)->KtCore.Katie.QChar.Direction",
    "KtCore.Katie.QChar.fromAscii(c:char)->KtCore.Katie.QChar",
    "KtCore.Katie.QChar.fromLatin1(c:char)->KtCore.Katie.QChar",
    "KtCore.Katie.QChar.hasMirrored()->bool",
    "KtCore.Katie.QChar.highSurrogate(ucs4:unsigned int)->ushort",
    "KtCore.Katie.QChar.isDigit()->bool",
    "1:KtCore.Katie.QChar.isHighSurrogate()->bool",
    "0:KtCore.Katie.QChar.isHighSurrogate(ucs4:unsigned int)->bool",
    "KtCore.Katie.QChar.isLetter()->bool",
    "KtCore.Katie.QChar.isLetterOrNumber()->bool",
    "1:KtCore.Katie.QChar.isLowSurrogate()->bool",
    "0:KtCore.Katie.QChar.isLowSurrogate(ucs4:unsigned int)->bool",
    "KtCore.Katie.QChar.isLower()->bool",
    "KtCore.Katie.QChar.isMark()->bool",
    "KtCore.Katie.QChar.isNull()->bool",
    "KtCore.Katie.QChar.isNumber()->bool",
    "KtCore.Katie.QChar.isPrint()->bool",
    "KtCore.Katie.QChar.isPunct()->bool",
    "KtCore.Katie.QChar.isSpace()->bool",
    "KtCore.Katie.QChar.isSymbol()->bool",
    "KtCore.Katie.QChar.isTitleCase()->bool",
    "KtCore.Katie.QChar.isUpper()->bool",
    "2:KtCore.Katie.QChar.joining()->KtCore.Katie.QChar.Joining",
    "1:KtCore.Katie.QChar.joining(ucs4:unsigned int)->KtCore.Katie.QChar.Joining",
    "0:KtCore.Katie.QChar.joining(ucs2:ushort)->KtCore.Katie.QChar.Joining",
    "KtCore.Katie.QChar.lowSurrogate(ucs4:unsigned int)->ushort",
    "2:KtCore.Katie.QChar.mirroredChar()->KtCore.Katie.QChar",
    "1:KtCore.Katie.QChar.mirroredChar(ucs4:unsigned int)->unsigned int",
    "0:KtCore.Katie.QChar.mirroredChar(ucs2:ushort)->ushort",
    "KtCore.Katie.QChar.requiresSurrogates(ucs4:unsigned int)->bool",
    "KtCore.Katie.QChar.row()->unsigned char",
    "KtCore.Katie.QChar.setCell(cell:unsigned char)",
    "KtCore.Katie.QChar.setRow(row:unsigned char)",
    "1:KtCore.Katie.QChar.surrogateToUcs4(high:KtCore.Katie.QChar,low:KtCore.Katie.QChar)->unsigned int",
    "0:KtCore.Katie.QChar.surrogateToUcs4(high:ushort,low:ushort)->unsigned int",
    "KtCore.Katie.QChar.toAscii()->char",
    "2:KtCore.Katie.QChar.toCaseFolded()->KtCore.Katie.QChar",
    "1:KtCore.Katie.QChar.toCaseFolded(ucs4:unsigned int)->unsigned int",
    "0:KtCore.Katie.QChar.toCaseFolded(ucs2:ushort)->ushort",
    "KtCore.Katie.QChar.toLatin1()->char",
    "2:KtCore.Katie.QChar.toLower()->KtCore.Katie.QChar",
    "1:KtCore.Katie.QChar.toLower(ucs4:unsigned int)->unsigned int",
    "0:KtCore.Katie.QChar.toLower(ucs2:ushort)->ushort",
    "2:KtCore.Katie.QChar.toTitleCase()->KtCore.Katie.QChar",
    "1:KtCore.Katie.QChar.toTitleCase(ucs4:unsigned int)->unsigned int",
    "0:KtCore.Katie.QChar.toTitleCase(ucs2:ushort)->ushort",
    "2:KtCore.Katie.QChar.toUpper()->KtCore.Katie.QChar",
    "1:KtCore.Katie.QChar.toUpper(ucs4:unsigned int)->unsigned int",
    "0:KtCore.Katie.QChar.toUpper(ucs2:ushort)->ushort",
    "1:KtCore.Katie.QChar.unicode()->KtCore.ushort",
    "0:KtCore.Katie.QChar.unicode()->ushort",
    "2:KtCore.Katie.QChar.unicodeVersion()->KtCore.Katie.QChar.UnicodeVersion",
    "1:KtCore.Katie.QChar.unicodeVersion(ucs4:unsigned int)->KtCore.Katie.QChar.UnicodeVersion",
    "0:KtCore.Katie.QChar.unicodeVersion(ucs2:ushort)->KtCore.Katie.QChar.UnicodeVersion",
    "KtCore.Katie.QChar.__copy__()",
    nullptr}; // Sentinel

void init_Katie_QChar(PyObject* module)
{
    _Sbk_Katie_QChar_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QChar",
        "Katie::QChar",
        &Sbk_Katie_QChar_spec,
        Katie_QChar_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QChar >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QChar_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QChar_TypeF(),
        QChar_PythonToCpp_QChar_PTR,
        is_QChar_PythonToCpp_QChar_PTR_Convertible,
        QChar_PTR_CppToPython_QChar,
        QChar_COPY_CppToPython_QChar);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QChar");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QChar*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QChar&");
    Shiboken::Conversions::registerConverterName(converter, "QChar");
    Shiboken::Conversions::registerConverterName(converter, "QChar*");
    Shiboken::Conversions::registerConverterName(converter, "QChar&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QChar).name());

    // Add Python to C++ copy (value, not pointer neither reference) conversion to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        QChar_PythonToCpp_QChar_COPY,
        is_QChar_PythonToCpp_QChar_COPY_Convertible);
    // Add implicit conversions to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar,
        is_constKatie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QLatin1Char_PythonToCpp_Katie_QChar,
        is_constKatie_QLatin1Char_PythonToCpp_Katie_QChar_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constchar_PythonToCpp_Katie_QChar,
        is_constchar_PythonToCpp_Katie_QChar_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constint_PythonToCpp_Katie_QChar,
        is_constint_PythonToCpp_Katie_QChar_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constshort_PythonToCpp_Katie_QChar,
        is_constshort_PythonToCpp_Katie_QChar_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constuchar_PythonToCpp_Katie_QChar,
        is_constuchar_PythonToCpp_Katie_QChar_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constuint_PythonToCpp_Katie_QChar,
        is_constuint_PythonToCpp_Katie_QChar_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constushort_PythonToCpp_Katie_QChar,
        is_constushort_PythonToCpp_Katie_QChar_Convertible);

    // Initialization of enums.

    // Initialization of enum 'SpecialCharacter'.
    SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QChar_TypeF(),
        "SpecialCharacter",
        "KtCore.Katie.QChar.SpecialCharacter",
        "Katie::QChar::SpecialCharacter");
    if (!SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "Null", (long) Katie::QChar::SpecialCharacter::Null))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "Nbsp", (long) Katie::QChar::SpecialCharacter::Nbsp))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "ReplacementCharacter", (long) Katie::QChar::SpecialCharacter::ReplacementCharacter))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "ObjectReplacementCharacter", (long) Katie::QChar::SpecialCharacter::ObjectReplacementCharacter))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "ByteOrderMark", (long) Katie::QChar::SpecialCharacter::ByteOrderMark))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "ByteOrderSwapped", (long) Katie::QChar::SpecialCharacter::ByteOrderSwapped))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "ParagraphSeparator", (long) Katie::QChar::SpecialCharacter::ParagraphSeparator))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
        Sbk_Katie_QChar_TypeF(), "LineSeparator", (long) Katie::QChar::SpecialCharacter::LineSeparator))
        return;
    // Register converter for enum 'Katie::QChar::SpecialCharacter'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX],
            Katie_QChar_SpecialCharacter_CppToPython_Katie_QChar_SpecialCharacter);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar_SpecialCharacter,
            is_Katie_QChar_SpecialCharacter_PythonToCpp_Katie_QChar_SpecialCharacter_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_SPECIALCHARACTER_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QChar::SpecialCharacter");
        Shiboken::Conversions::registerConverterName(converter, "QChar::SpecialCharacter");
        Shiboken::Conversions::registerConverterName(converter, "SpecialCharacter");
    }
    // End of 'SpecialCharacter' enum.

    // Initialization of enum 'Category'.
    SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QChar_TypeF(),
        "Category",
        "KtCore.Katie.QChar.Category",
        "Katie::QChar::Category");
    if (!SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "NoCategory", (long) Katie::QChar::Category::NoCategory))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Mark_NonSpacing", (long) Katie::QChar::Category::Mark_NonSpacing))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Mark_SpacingCombining", (long) Katie::QChar::Category::Mark_SpacingCombining))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Mark_Enclosing", (long) Katie::QChar::Category::Mark_Enclosing))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Number_DecimalDigit", (long) Katie::QChar::Category::Number_DecimalDigit))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Number_Letter", (long) Katie::QChar::Category::Number_Letter))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Number_Other", (long) Katie::QChar::Category::Number_Other))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Separator_Space", (long) Katie::QChar::Category::Separator_Space))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Separator_Line", (long) Katie::QChar::Category::Separator_Line))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Separator_Paragraph", (long) Katie::QChar::Category::Separator_Paragraph))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Other_Control", (long) Katie::QChar::Category::Other_Control))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Other_Format", (long) Katie::QChar::Category::Other_Format))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Other_Surrogate", (long) Katie::QChar::Category::Other_Surrogate))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Other_PrivateUse", (long) Katie::QChar::Category::Other_PrivateUse))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Other_NotAssigned", (long) Katie::QChar::Category::Other_NotAssigned))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Letter_Uppercase", (long) Katie::QChar::Category::Letter_Uppercase))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Letter_Lowercase", (long) Katie::QChar::Category::Letter_Lowercase))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Letter_Titlecase", (long) Katie::QChar::Category::Letter_Titlecase))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Letter_Modifier", (long) Katie::QChar::Category::Letter_Modifier))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Letter_Other", (long) Katie::QChar::Category::Letter_Other))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Punctuation_Connector", (long) Katie::QChar::Category::Punctuation_Connector))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Punctuation_Dash", (long) Katie::QChar::Category::Punctuation_Dash))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Punctuation_Open", (long) Katie::QChar::Category::Punctuation_Open))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Punctuation_Close", (long) Katie::QChar::Category::Punctuation_Close))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Punctuation_InitialQuote", (long) Katie::QChar::Category::Punctuation_InitialQuote))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Punctuation_FinalQuote", (long) Katie::QChar::Category::Punctuation_FinalQuote))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Punctuation_Other", (long) Katie::QChar::Category::Punctuation_Other))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Symbol_Math", (long) Katie::QChar::Category::Symbol_Math))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Symbol_Currency", (long) Katie::QChar::Category::Symbol_Currency))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Symbol_Modifier", (long) Katie::QChar::Category::Symbol_Modifier))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
        Sbk_Katie_QChar_TypeF(), "Symbol_Other", (long) Katie::QChar::Category::Symbol_Other))
        return;
    // Register converter for enum 'Katie::QChar::Category'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX],
            Katie_QChar_Category_CppToPython_Katie_QChar_Category);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QChar_Category_PythonToCpp_Katie_QChar_Category,
            is_Katie_QChar_Category_PythonToCpp_Katie_QChar_Category_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_CATEGORY_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QChar::Category");
        Shiboken::Conversions::registerConverterName(converter, "QChar::Category");
        Shiboken::Conversions::registerConverterName(converter, "Category");
    }
    // End of 'Category' enum.

    // Initialization of enum 'Direction'.
    SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QChar_TypeF(),
        "Direction",
        "KtCore.Katie.QChar.Direction",
        "Katie::QChar::Direction");
    if (!SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirL", (long) Katie::QChar::Direction::DirL))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirR", (long) Katie::QChar::Direction::DirR))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirEN", (long) Katie::QChar::Direction::DirEN))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirES", (long) Katie::QChar::Direction::DirES))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirET", (long) Katie::QChar::Direction::DirET))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirAN", (long) Katie::QChar::Direction::DirAN))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirCS", (long) Katie::QChar::Direction::DirCS))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirB", (long) Katie::QChar::Direction::DirB))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirS", (long) Katie::QChar::Direction::DirS))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirWS", (long) Katie::QChar::Direction::DirWS))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirON", (long) Katie::QChar::Direction::DirON))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirLRE", (long) Katie::QChar::Direction::DirLRE))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirLRO", (long) Katie::QChar::Direction::DirLRO))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirAL", (long) Katie::QChar::Direction::DirAL))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirRLE", (long) Katie::QChar::Direction::DirRLE))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirRLO", (long) Katie::QChar::Direction::DirRLO))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirPDF", (long) Katie::QChar::Direction::DirPDF))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirNSM", (long) Katie::QChar::Direction::DirNSM))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
        Sbk_Katie_QChar_TypeF(), "DirBN", (long) Katie::QChar::Direction::DirBN))
        return;
    // Register converter for enum 'Katie::QChar::Direction'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX],
            Katie_QChar_Direction_CppToPython_Katie_QChar_Direction);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QChar_Direction_PythonToCpp_Katie_QChar_Direction,
            is_Katie_QChar_Direction_PythonToCpp_Katie_QChar_Direction_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_DIRECTION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QChar::Direction");
        Shiboken::Conversions::registerConverterName(converter, "QChar::Direction");
        Shiboken::Conversions::registerConverterName(converter, "Direction");
    }
    // End of 'Direction' enum.

    // Initialization of enum 'Decomposition'.
    SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QChar_TypeF(),
        "Decomposition",
        "KtCore.Katie.QChar.Decomposition",
        "Katie::QChar::Decomposition");
    if (!SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "NoDecomposition", (long) Katie::QChar::Decomposition::NoDecomposition))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Canonical", (long) Katie::QChar::Decomposition::Canonical))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Font", (long) Katie::QChar::Decomposition::Font))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "NoBreak", (long) Katie::QChar::Decomposition::NoBreak))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Initial", (long) Katie::QChar::Decomposition::Initial))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Medial", (long) Katie::QChar::Decomposition::Medial))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Final", (long) Katie::QChar::Decomposition::Final))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Isolated", (long) Katie::QChar::Decomposition::Isolated))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Circle", (long) Katie::QChar::Decomposition::Circle))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Super", (long) Katie::QChar::Decomposition::Super))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Sub", (long) Katie::QChar::Decomposition::Sub))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Vertical", (long) Katie::QChar::Decomposition::Vertical))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Wide", (long) Katie::QChar::Decomposition::Wide))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Narrow", (long) Katie::QChar::Decomposition::Narrow))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Small", (long) Katie::QChar::Decomposition::Small))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Square", (long) Katie::QChar::Decomposition::Square))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Compat", (long) Katie::QChar::Decomposition::Compat))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
        Sbk_Katie_QChar_TypeF(), "Fraction", (long) Katie::QChar::Decomposition::Fraction))
        return;
    // Register converter for enum 'Katie::QChar::Decomposition'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX],
            Katie_QChar_Decomposition_CppToPython_Katie_QChar_Decomposition);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QChar_Decomposition_PythonToCpp_Katie_QChar_Decomposition,
            is_Katie_QChar_Decomposition_PythonToCpp_Katie_QChar_Decomposition_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_DECOMPOSITION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QChar::Decomposition");
        Shiboken::Conversions::registerConverterName(converter, "QChar::Decomposition");
        Shiboken::Conversions::registerConverterName(converter, "Decomposition");
    }
    // End of 'Decomposition' enum.

    // Initialization of enum 'Joining'.
    SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QChar_TypeF(),
        "Joining",
        "KtCore.Katie.QChar.Joining",
        "Katie::QChar::Joining");
    if (!SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX],
        Sbk_Katie_QChar_TypeF(), "OtherJoining", (long) Katie::QChar::Joining::OtherJoining))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX],
        Sbk_Katie_QChar_TypeF(), "Dual", (long) Katie::QChar::Joining::Dual))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX],
        Sbk_Katie_QChar_TypeF(), "Right", (long) Katie::QChar::Joining::Right))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX],
        Sbk_Katie_QChar_TypeF(), "Center", (long) Katie::QChar::Joining::Center))
        return;
    // Register converter for enum 'Katie::QChar::Joining'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX],
            Katie_QChar_Joining_CppToPython_Katie_QChar_Joining);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QChar_Joining_PythonToCpp_Katie_QChar_Joining,
            is_Katie_QChar_Joining_PythonToCpp_Katie_QChar_Joining_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_JOINING_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QChar::Joining");
        Shiboken::Conversions::registerConverterName(converter, "QChar::Joining");
        Shiboken::Conversions::registerConverterName(converter, "Joining");
    }
    // End of 'Joining' enum.

    // Initialization of enum 'CombiningClass'.
    SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QChar_TypeF(),
        "CombiningClass",
        "KtCore.Katie.QChar.CombiningClass",
        "Katie::QChar::CombiningClass");
    if (!SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_BelowLeftAttached", (long) Katie::QChar::CombiningClass::Combining_BelowLeftAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_BelowAttached", (long) Katie::QChar::CombiningClass::Combining_BelowAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_BelowRightAttached", (long) Katie::QChar::CombiningClass::Combining_BelowRightAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_LeftAttached", (long) Katie::QChar::CombiningClass::Combining_LeftAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_RightAttached", (long) Katie::QChar::CombiningClass::Combining_RightAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_AboveLeftAttached", (long) Katie::QChar::CombiningClass::Combining_AboveLeftAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_AboveAttached", (long) Katie::QChar::CombiningClass::Combining_AboveAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_AboveRightAttached", (long) Katie::QChar::CombiningClass::Combining_AboveRightAttached))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_BelowLeft", (long) Katie::QChar::CombiningClass::Combining_BelowLeft))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_Below", (long) Katie::QChar::CombiningClass::Combining_Below))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_BelowRight", (long) Katie::QChar::CombiningClass::Combining_BelowRight))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_Left", (long) Katie::QChar::CombiningClass::Combining_Left))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_Right", (long) Katie::QChar::CombiningClass::Combining_Right))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_AboveLeft", (long) Katie::QChar::CombiningClass::Combining_AboveLeft))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_Above", (long) Katie::QChar::CombiningClass::Combining_Above))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_AboveRight", (long) Katie::QChar::CombiningClass::Combining_AboveRight))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_DoubleBelow", (long) Katie::QChar::CombiningClass::Combining_DoubleBelow))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_DoubleAbove", (long) Katie::QChar::CombiningClass::Combining_DoubleAbove))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
        Sbk_Katie_QChar_TypeF(), "Combining_IotaSubscript", (long) Katie::QChar::CombiningClass::Combining_IotaSubscript))
        return;
    // Register converter for enum 'Katie::QChar::CombiningClass'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX],
            Katie_QChar_CombiningClass_CppToPython_Katie_QChar_CombiningClass);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QChar_CombiningClass_PythonToCpp_Katie_QChar_CombiningClass,
            is_Katie_QChar_CombiningClass_PythonToCpp_Katie_QChar_CombiningClass_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_COMBININGCLASS_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QChar::CombiningClass");
        Shiboken::Conversions::registerConverterName(converter, "QChar::CombiningClass");
        Shiboken::Conversions::registerConverterName(converter, "CombiningClass");
    }
    // End of 'CombiningClass' enum.

    // Initialization of enum 'UnicodeVersion'.
    SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QChar_TypeF(),
        "UnicodeVersion",
        "KtCore.Katie.QChar.UnicodeVersion",
        "Katie::QChar::UnicodeVersion");
    if (!SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_Unassigned", (long) Katie::QChar::UnicodeVersion::Unicode_Unassigned))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_1_1", (long) Katie::QChar::UnicodeVersion::Unicode_1_1))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_2_0", (long) Katie::QChar::UnicodeVersion::Unicode_2_0))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_2_1_2", (long) Katie::QChar::UnicodeVersion::Unicode_2_1_2))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_3_0", (long) Katie::QChar::UnicodeVersion::Unicode_3_0))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_3_1", (long) Katie::QChar::UnicodeVersion::Unicode_3_1))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_3_2", (long) Katie::QChar::UnicodeVersion::Unicode_3_2))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_4_0", (long) Katie::QChar::UnicodeVersion::Unicode_4_0))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_4_1", (long) Katie::QChar::UnicodeVersion::Unicode_4_1))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
        Sbk_Katie_QChar_TypeF(), "Unicode_5_0", (long) Katie::QChar::UnicodeVersion::Unicode_5_0))
        return;
    // Register converter for enum 'Katie::QChar::UnicodeVersion'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX],
            Katie_QChar_UnicodeVersion_CppToPython_Katie_QChar_UnicodeVersion);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QChar_UnicodeVersion_PythonToCpp_Katie_QChar_UnicodeVersion,
            is_Katie_QChar_UnicodeVersion_PythonToCpp_Katie_QChar_UnicodeVersion_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCHAR_UNICODEVERSION_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QChar::UnicodeVersion");
        Shiboken::Conversions::registerConverterName(converter, "QChar::UnicodeVersion");
        Shiboken::Conversions::registerConverterName(converter, "UnicodeVersion");
    }
    // End of 'UnicodeVersion' enum.


}
