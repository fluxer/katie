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
#include "katie_qvariant_wrapper.h"

// inner classes

// Extra includes
#include <qbytearray.h>
#include <qchar.h>
#include <qregexp.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qvariant.h>


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

QVariantWrapper::QVariantWrapper() : Katie::QVariant()
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(Katie::QVariant::Type type) : Katie::QVariant(type)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(bool b) : Katie::QVariant(b)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(const Katie::QByteArray & bytearray) : Katie::QVariant(bytearray)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(const Katie::QChar & qchar) : Katie::QVariant(qchar)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(const Katie::QLatin1String & string) : Katie::QVariant(string)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(const Katie::QRegExp & regExp) : Katie::QVariant(regExp)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(const Katie::QString & string) : Katie::QVariant(string)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(const Katie::QStringList & stringlist) : Katie::QVariant(stringlist)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(const char * str) : Katie::QVariant(str)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(double d) : Katie::QVariant(d)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(float f) : Katie::QVariant(f)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(int i) : Katie::QVariant(i)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(int typeOrUserType, const void * copy) : Katie::QVariant(typeOrUserType, copy)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(int typeOrUserType, const void * copy, uint flags) : Katie::QVariant(typeOrUserType, copy, flags)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(long long ll) : Katie::QVariant(ll)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(uint ui) : Katie::QVariant(ui)
{
    // ... middle
}

QVariantWrapper::QVariantWrapper(unsigned long long ull) : Katie::QVariant(ull)
{
    // ... middle
}

QVariantWrapper::~QVariantWrapper()
{
    SbkObject* wrapper = Shiboken::BindingManager::instance().retrieveWrapper(this);
    Shiboken::Object::destroy(wrapper, this);
}

// Target ---------------------------------------------------------

extern "C" {
static int
Sbk_Katie_QVariant_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QVariant >()))
        return -1;

    ::QVariantWrapper* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QVariant", 0, 3, &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2])))
        return -1;


    // Overloaded function decisor
    // 0: QVariant::QVariant()
    // 1: QVariant::QVariant(Katie::QVariant::Type)
    // 2: QVariant::QVariant(bool)
    // 3: QVariant::QVariant(Katie::QByteArray)
    // 4: QVariant::QVariant(Katie::QChar)
    // 5: QVariant::QVariant(Katie::QLatin1String)
    // 6: QVariant::QVariant(Katie::QRegExp)
    // 7: QVariant::QVariant(Katie::QString)
    // 8: QVariant::QVariant(Katie::QStringList)
    // 9: QVariant::QVariant(Katie::QVariant)
    // 10: QVariant::QVariant(const char*)
    // 11: QVariant::QVariant(double)
    // 12: QVariant::QVariant(float)
    // 13: QVariant::QVariant(int)
    // 14: QVariant::QVariant(int,const void*)
    // 15: QVariant::QVariant(int,const void*,uint)
    // 16: QVariant::QVariant(long long)
    // 17: QVariant::QVariant(uint)
    // 18: QVariant::QVariant(unsigned long long)
    if (numArgs == 0) {
        overloadId = 0; // QVariant()
    } else if (numArgs == 1
        && PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), (pyArgs[0])))) {
        overloadId = 18; // QVariant(unsigned long long)
    } else if (numArgs == 1
        && PyLong_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), (pyArgs[0])))) {
        overloadId = 16; // QVariant(long long)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), (pyArgs[0])))) {
        overloadId = 8; // QVariant(Katie::QStringList)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), (pyArgs[0])))) {
        overloadId = 6; // QVariant(Katie::QRegExp)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX]), (pyArgs[0])))) {
        overloadId = 5; // QVariant(Katie::QLatin1String)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])->converter, (pyArgs[0])))) {
        overloadId = 1; // QVariant(Katie::QVariant::Type)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[0])))) {
        overloadId = 17; // QVariant(uint)
    } else if (numArgs == 1
        && PyFloat_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<float>(), (pyArgs[0])))) {
        overloadId = 12; // QVariant(float)
    } else if (numArgs == 1
        && PyFloat_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<double>(), (pyArgs[0])))) {
        overloadId = 11; // QVariant(double)
    } else if (numArgs == 1
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))) {
        overloadId = 10; // QVariant(const char*)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), (pyArgs[0])))) {
        overloadId = 3; // QVariant(Katie::QByteArray)
    } else if (numArgs == 1
        && PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 2; // QVariant(bool)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 13; // QVariant(int)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<void*>(), (pyArgs[1])))) {
            if (numArgs == 2) {
                overloadId = 14; // QVariant(int,const void*)
            } else if (numArgs == 3
                && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), (pyArgs[2])))) {
                overloadId = 15; // QVariant(int,const void*,uint)
            }
        }
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), (pyArgs[0])))) {
        overloadId = 4; // QVariant(Katie::QChar)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArgs[0])))) {
        overloadId = 7; // QVariant(Katie::QString)
    } else if (numArgs == 1
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), (pyArgs[0])))) {
        overloadId = 9; // QVariant(Katie::QVariant)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariant_Init_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // QVariant()
        {

            if (!PyErr_Occurred()) {
                // QVariant()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // QVariant(Katie::QVariant::Type type)
        {
            ::Katie::QVariant::Type cppArg0{Katie::QVariant::Invalid};
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(Katie::QVariant::Type)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 2: // QVariant(bool b)
        {
            bool cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(bool)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 3: // QVariant(const Katie::QByteArray & bytearray)
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
                // QVariant(Katie::QByteArray)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 4: // QVariant(const Katie::QChar & qchar)
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
                // QVariant(Katie::QChar)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 5: // QVariant(const Katie::QLatin1String & string)
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
                // QVariant(Katie::QLatin1String)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 6: // QVariant(const Katie::QRegExp & regExp)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QRegExp* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(Katie::QRegExp)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 7: // QVariant(const Katie::QString & string)
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
                // QVariant(Katie::QString)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 8: // QVariant(const Katie::QStringList & stringlist)
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
                // QVariant(Katie::QStringList)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 9: // QVariant(const Katie::QVariant & other)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return -1;
            ::Katie::QVariant cppArg0_local;
            ::Katie::QVariant* cppArg0 = &cppArg0_local;
            if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), pythonToCpp[0]))
                pythonToCpp[0](pyArgs[0], &cppArg0_local);
            else
                pythonToCpp[0](pyArgs[0], &cppArg0);


            if (!PyErr_Occurred()) {
                // QVariant(Katie::QVariant)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(*cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 10: // QVariant(const char * str)
        {
            const char* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(const char*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 11: // QVariant(double d)
        {
            double cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(double)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 12: // QVariant(float f)
        {
            float cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(float)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 13: // QVariant(int i)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 14: // QVariant(int typeOrUserType, const void * copy)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            void* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // QVariant(int,const void*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 15: // QVariant(int typeOrUserType, const void * copy, uint flags)
        {
            int cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            void* cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);
            uint cppArg2;
            pythonToCpp[2](pyArgs[2], &cppArg2);

            if (!PyErr_Occurred()) {
                // QVariant(int,const void*,uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0, cppArg1, cppArg2);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 16: // QVariant(long long ll)
        {
            long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(long long)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 17: // QVariant(uint ui)
        {
            uint cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(uint)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 18: // QVariant(unsigned long long ull)
        {
            unsigned long long cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // QVariant(unsigned long long)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                cptr = new ::QVariantWrapper(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QVariant >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QVariant_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    Shiboken::Object::setHasCppWrapper(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QVariant_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant");
        return -1;
}

static PyObject* Sbk_Katie_QVariantFunc_canConvert(PyObject* self, PyObject* pyArg)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QVariant::canConvert(Katie::QVariant::Type)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])->converter, (pyArg)))) {
        overloadId = 0; // canConvert(Katie::QVariant::Type)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_canConvert_TypeError;

    // Call function/method
    {
        ::Katie::QVariant::Type cppArg0{Katie::QVariant::Invalid};
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // canConvert(Katie::QVariant::Type)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->canConvert(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_canConvert_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QVariant.canConvert");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_clear(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));

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

static PyObject* Sbk_Katie_QVariantFunc_cmp(PyObject* self, PyObject* pyArg)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QVariant::cmp(Katie::QVariant)const
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), (pyArg)))) {
        overloadId = 0; // cmp(Katie::QVariant)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_cmp_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QVariant cppArg0_local;
        ::Katie::QVariant* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), pythonToCpp))
            pythonToCpp(pyArg, &cppArg0_local);
        else
            pythonToCpp(pyArg, &cppArg0);


        if (!PyErr_Occurred()) {
            // cmp(Katie::QVariant)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = static_cast<::QVariantWrapper*>(cppSelf)->QVariantWrapper::cmp_protected(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_cmp_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QVariant.cmp");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_constData(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // constData()const
            const void * cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->constData();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<void*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_convert(PyObject* self, PyObject* pyArg)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QVariant::convert(Katie::QVariant::Type)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])->converter, (pyArg)))) {
        overloadId = 0; // convert(Katie::QVariant::Type)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_convert_TypeError;

    // Call function/method
    {
        ::Katie::QVariant::Type cppArg0{Katie::QVariant::Invalid};
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // convert(Katie::QVariant::Type)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = cppSelf->convert(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_convert_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QVariant.convert");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_create(PyObject* self, PyObject* args)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "create", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::create(int,const void*)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<void*>(), (pyArgs[1])))) {
        overloadId = 0; // create(int,const void*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_create_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        void* cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // create(int,const void*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            static_cast<::QVariantWrapper*>(cppSelf)->QVariantWrapper::create_protected(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QVariantFunc_create_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.create");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_data(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // data()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            void * cppResult = cppSelf->data();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<void*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_detach(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));

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

static PyObject* Sbk_Katie_QVariantFunc_isDetached(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isDetached()const
            bool cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->isDetached();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_isNull(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isNull()const
            bool cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->isNull();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_isValid(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // isValid()const
            bool cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->isValid();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_nameToType(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QVariant::nameToType(const char*)
    if (Shiboken::String::check(pyArg) && (pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArg)))) {
        overloadId = 0; // nameToType(const char*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_nameToType_TypeError;

    // Call function/method
    {
        const char* cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // nameToType(const char*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QVariant::Type cppResult = Katie::QVariant::Type(::Katie::QVariant::nameToType(cppArg0));
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])->converter, &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_nameToType_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QVariant.nameToType");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_swap(PyObject* self, PyObject* pyArg)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QVariant::swap(Katie::QVariant&)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), (pyArg)))) {
        overloadId = 0; // swap(Katie::QVariant&)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_swap_TypeError;

    // Call function/method
    {
        if (!Shiboken::Object::isValid(pyArg))
            return {};
        ::Katie::QVariant cppArg0_local;
        ::Katie::QVariant* cppArg0 = &cppArg0_local;
        if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), pythonToCpp))
            pythonToCpp(pyArg, &cppArg0_local);
        else
            pythonToCpp(pyArg, &cppArg0);


        if (!PyErr_Occurred()) {
            // swap(Katie::QVariant&)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cppSelf->swap(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QVariantFunc_swap_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QVariant.swap");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_toBool(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toBool()const
            bool cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toBool();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_toByteArray(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toByteArray()const
            Katie::QByteArray cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toByteArray();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_toChar(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toChar()const
            Katie::QChar cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toChar();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_toDouble(PyObject* self, PyObject* args, PyObject* kwds)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toDouble(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toDouble", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::toDouble(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toDouble(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toDouble(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_toDouble_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toDouble(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QVariantFunc_toDouble_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toDouble(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            double cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toDouble(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<double>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_toDouble_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.toDouble");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_toFloat(PyObject* self, PyObject* args, PyObject* kwds)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toFloat(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toFloat", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::toFloat(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toFloat(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toFloat(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_toFloat_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toFloat(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QVariantFunc_toFloat_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toFloat(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            float cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toFloat(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<float>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_toFloat_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.toFloat");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_toInt(PyObject* self, PyObject* args, PyObject* kwds)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toInt(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toInt", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::toInt(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toInt(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toInt(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_toInt_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toInt(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QVariantFunc_toInt_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toInt(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toInt(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_toInt_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.toInt");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_toLongLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toLongLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toLongLong", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::toLongLong(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toLongLong(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toLongLong(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_toLongLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toLongLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QVariantFunc_toLongLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toLongLong(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            long long cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toLongLong(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_toLongLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.toLongLong");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_toReal(PyObject* self, PyObject* args, PyObject* kwds)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toReal(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toReal", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::toReal(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toReal(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toReal(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_toReal_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toReal(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QVariantFunc_toReal_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toReal(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            double cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toReal(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<double>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_toReal_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.toReal");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_toRegExp(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toRegExp()const
            Katie::QRegExp* cppResult = new Katie::QRegExp(const_cast<const ::QVariantWrapper*>(cppSelf)->toRegExp());
            pyResult = Shiboken::Object::newObject(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX]), cppResult, true, true);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_toString(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toString()const
            Katie::QString cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toString();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_toStringList(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // toStringList()const
            Katie::QStringList cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toStringList();
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_toUInt(PyObject* self, PyObject* args, PyObject* kwds)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toUInt(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toUInt", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::toUInt(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toUInt(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toUInt(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_toUInt_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toUInt(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QVariantFunc_toUInt_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toUInt(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            uint cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toUInt(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_toUInt_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.toUInt");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_toULongLong(PyObject* self, PyObject* args, PyObject* kwds)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toULongLong(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:toULongLong", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: QVariant::toULongLong(bool*)const
    if (numArgs == 0) {
        overloadId = 0; // toULongLong(bool*)const
    } else if (PyBool_Check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0])))) {
        overloadId = 0; // toULongLong(bool*)const
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_toULongLong_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "ok");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QVariant.toULongLong(): got multiple values for keyword argument 'ok'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[0]))))
                    goto Sbk_Katie_QVariantFunc_toULongLong_TypeError;
            }
        }
        bool* cppArg0 = Q_NULLPTR;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // toULongLong(bool*)const
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            unsigned long long cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->toULongLong(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_toULongLong_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QVariant.toULongLong");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_type(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // type()const
            Katie::QVariant::Type cppResult = Katie::QVariant::Type(const_cast<const ::QVariantWrapper*>(cppSelf)->type());
            pyResult = Shiboken::Conversions::copyToPython(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])->converter, &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_typeName(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // typeName()const
            const char * cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->typeName();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariantFunc_typeToName(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QVariant::typeToName(Katie::QVariant::Type)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])->converter, (pyArg)))) {
        overloadId = 0; // typeToName(Katie::QVariant::Type)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QVariantFunc_typeToName_TypeError;

    // Call function/method
    {
        ::Katie::QVariant::Type cppArg0{Katie::QVariant::Invalid};
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // typeToName(Katie::QVariant::Type)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            const char * cppResult = ::Katie::QVariant::typeToName(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QVariantFunc_typeToName_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QVariant.typeToName");
        return {};
}

static PyObject* Sbk_Katie_QVariantFunc_userType(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // userType()const
            int cppResult = const_cast<const ::QVariantWrapper*>(cppSelf)->userType();
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<int>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QVariant___copy__(PyObject* self)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    QVariantWrapper& cppSelf = *static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), &cppSelf);
    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyMethodDef Sbk_Katie_QVariant_methods[] = {
    {"canConvert", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_canConvert), METH_O},
    {"clear", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_clear), METH_NOARGS},
    {"cmp", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_cmp), METH_O},
    {"constData", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_constData), METH_NOARGS},
    {"convert", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_convert), METH_O},
    {"create", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_create), METH_VARARGS},
    {"data", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_data), METH_NOARGS},
    {"detach", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_detach), METH_NOARGS},
    {"isDetached", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_isDetached), METH_NOARGS},
    {"isNull", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_isNull), METH_NOARGS},
    {"isValid", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_isValid), METH_NOARGS},
    {"nameToType", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_nameToType), METH_O|METH_STATIC},
    {"swap", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_swap), METH_O},
    {"toBool", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toBool), METH_NOARGS},
    {"toByteArray", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toByteArray), METH_NOARGS},
    {"toChar", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toChar), METH_NOARGS},
    {"toDouble", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toDouble), METH_VARARGS|METH_KEYWORDS},
    {"toFloat", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toFloat), METH_VARARGS|METH_KEYWORDS},
    {"toInt", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toInt), METH_VARARGS|METH_KEYWORDS},
    {"toLongLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toLongLong), METH_VARARGS|METH_KEYWORDS},
    {"toReal", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toReal), METH_VARARGS|METH_KEYWORDS},
    {"toRegExp", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toRegExp), METH_NOARGS},
    {"toString", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toString), METH_NOARGS},
    {"toStringList", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toStringList), METH_NOARGS},
    {"toUInt", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toUInt), METH_VARARGS|METH_KEYWORDS},
    {"toULongLong", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_toULongLong), METH_VARARGS|METH_KEYWORDS},
    {"type", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_type), METH_NOARGS},
    {"typeName", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_typeName), METH_NOARGS},
    {"typeToName", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_typeToName), METH_O|METH_STATIC},
    {"userType", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariantFunc_userType), METH_NOARGS},

    {"__copy__", reinterpret_cast<PyCFunction>(Sbk_Katie_QVariant___copy__), METH_NOARGS},
    {nullptr, nullptr} // Sentinel
};

static int Sbk_Katie_QVariant___nb_bool(PyObject* self)
{
    QVariantWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return -1;
    cppSelf = static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    return !cppSelf->isNull();
}

// Rich comparison
static PyObject* Sbk_Katie_QVariant_richcompare(PyObject* self, PyObject* pyArg, int op)
{
    if (!Shiboken::Object::isValid(self))
        return {};
    QVariantWrapper& cppSelf = *static_cast<QVariantWrapper *>(reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(self))));
    SBK_UNUSED(cppSelf)
    PyObject* pyResult{};
    PythonToCppFunc pythonToCpp;
    SBK_UNUSED(pythonToCpp)

    switch (op) {
        case Py_NE:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), (pyArg)))) {
                // operator!=(const Katie::QVariant & v) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QVariant cppArg0_local;
                ::Katie::QVariant* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), pythonToCpp))
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
        case Py_EQ:
            if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), (pyArg)))) {
                // operator==(const Katie::QVariant & v) const
                if (!Shiboken::Object::isValid(pyArg))
                    return {};
                ::Katie::QVariant cppArg0_local;
                ::Katie::QVariant* cppArg0 = &cppArg0_local;
                if (Shiboken::Conversions::isImplicitConversion(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]), pythonToCpp))
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
        default:
            goto Sbk_Katie_QVariant_RichComparison_TypeError;
    }

    if (pyResult && !PyErr_Occurred())
        return pyResult;
    Sbk_Katie_QVariant_RichComparison_TypeError:
    PyErr_SetString(PyExc_NotImplementedError, "operator not implemented.");
    return {};

}

} // extern "C"

static int Sbk_Katie_QVariant_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QVariant_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QVariant_Type = nullptr;
static SbkObjectType *Sbk_Katie_QVariant_TypeF(void)
{
    return _Sbk_Katie_QVariant_Type;
}

static PyType_Slot Sbk_Katie_QVariant_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QVariant_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QVariant_clear)},
    {Py_tp_richcompare, reinterpret_cast<void*>(Sbk_Katie_QVariant_richcompare)},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QVariant_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QVariant_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkObjectTpNew)},
    // type supports number protocol
#ifdef IS_PY3K
    {Py_nb_bool, (void *)Sbk_Katie_QVariant___nb_bool},
#else
    {Py_nb_nonzero, (void *)Sbk_Katie_QVariant___nb_bool},
#endif
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QVariant_spec = {
    "KtCore.Katie.QVariant",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES|Py_TPFLAGS_HAVE_GC,
    Sbk_Katie_QVariant_slots
};

} //extern "C"


// Type conversion functions.

// Python to C++ enum conversion.
static void Katie_QVariant_Type_PythonToCpp_Katie_QVariant_Type(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant::Type*>(cppOut) =
        static_cast<::Katie::QVariant::Type>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QVariant_Type_PythonToCpp_Katie_QVariant_Type_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX]))
        return Katie_QVariant_Type_PythonToCpp_Katie_QVariant_Type;
    return {};
}
static PyObject* Katie_QVariant_Type_CppToPython_Katie_QVariant_Type(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QVariant::Type *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX], castCppIn);

}

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QVariant_PythonToCpp_QVariant_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QVariant_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QVariant_PythonToCpp_QVariant_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QVariant_TypeF())))
        return QVariant_PythonToCpp_QVariant_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QVariant_PTR_CppToPython_QVariant(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QVariant *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QVariant_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// C++ to Python copy conversion.
static PyObject* QVariant_COPY_CppToPython_QVariant(const void* cppIn) {
    return Shiboken::Object::newObject(Sbk_Katie_QVariant_TypeF(), new ::QVariantWrapper(*reinterpret_cast<const ::Katie::QVariant*>(cppIn)), true, true);
}

// Python to C++ copy conversion.
static void QVariant_PythonToCpp_QVariant_COPY(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = *reinterpret_cast< ::Katie::QVariant *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX], reinterpret_cast<SbkObject *>(pyIn)));
}
static PythonToCppFunc is_QVariant_PythonToCpp_QVariant_COPY_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QVariant_TypeF())))
        return QVariant_PythonToCpp_QVariant_COPY;
    return {};
}

// Implicit conversions.
static void Katie_QVariant_Type_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    ::Katie::QVariant::Type cppIn{Katie::QVariant::Invalid};
    Shiboken::Conversions::pythonToCppCopy(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])->converter, pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_Katie_QVariant_Type_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX], pyIn))
        return Katie_QVariant_Type_PythonToCpp_Katie_QVariant;
    return {};
}

static void bool_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    bool cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_bool_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (PyBool_Check(pyIn))
        return bool_PythonToCpp_Katie_QVariant;
    return {};
}

static void constKatie_QByteArrayREF_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(*reinterpret_cast< ::Katie::QByteArray *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QByteArrayREF_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QBYTEARRAY_IDX], pyIn))
        return constKatie_QByteArrayREF_PythonToCpp_Katie_QVariant;
    return {};
}

static void constKatie_QCharREF_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(*reinterpret_cast< ::Katie::QChar *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QCharREF_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QCHAR_IDX], pyIn))
        return constKatie_QCharREF_PythonToCpp_Katie_QVariant;
    return {};
}

static void constKatie_QLatin1StringREF_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(*reinterpret_cast< ::Katie::QLatin1String *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QLatin1StringREF_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QLATIN1STRING_IDX], pyIn))
        return constKatie_QLatin1StringREF_PythonToCpp_Katie_QVariant;
    return {};
}

static void constKatie_QRegExpREF_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(*reinterpret_cast< ::Katie::QRegExp *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QRegExpREF_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QREGEXP_IDX], pyIn))
        return constKatie_QRegExpREF_PythonToCpp_Katie_QVariant;
    return {};
}

static void constKatie_QStringREF_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(*reinterpret_cast< ::Katie::QString *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QStringREF_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX], pyIn))
        return constKatie_QStringREF_PythonToCpp_Katie_QVariant;
    return {};
}

static void constKatie_QStringListREF_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(*reinterpret_cast< ::Katie::QStringList *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], reinterpret_cast<SbkObject *>(pyIn))));
}
static PythonToCppFunc is_constKatie_QStringListREF_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (SbkObject_TypeCheck(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX], pyIn))
        return constKatie_QStringListREF_PythonToCpp_Katie_QVariant;
    return {};
}

static void constcharPTR_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    const char* cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_constcharPTR_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (Shiboken::String::check(pyIn))
        return constcharPTR_PythonToCpp_Katie_QVariant;
    return {};
}

static void double_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    double cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<double>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_double_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (PyFloat_Check(pyIn))
        return double_PythonToCpp_Katie_QVariant;
    return {};
}

static void float_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    float cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<float>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_float_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (PyFloat_Check(pyIn))
        return float_PythonToCpp_Katie_QVariant;
    return {};
}

static void int_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    int cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<int>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_int_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (PyInt_Check(pyIn))
        return int_PythonToCpp_Katie_QVariant;
    return {};
}

static void longlong_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    long long cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_longlong_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (PyLong_Check(pyIn))
        return longlong_PythonToCpp_Katie_QVariant;
    return {};
}

static void uint_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    uint cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<uint>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_uint_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (PyInt_Check(pyIn))
        return uint_PythonToCpp_Katie_QVariant;
    return {};
}

static void unsignedlonglong_PythonToCpp_Katie_QVariant(PyObject* pyIn, void* cppOut) {
    unsigned long long cppIn;
    Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<unsigned long long>(), pyIn, &cppIn);
    *reinterpret_cast<::Katie::QVariant*>(cppOut) = ::Katie::QVariant(cppIn);
}
static PythonToCppFunc is_unsignedlonglong_PythonToCpp_Katie_QVariant_Convertible(PyObject* pyIn) {
    if (PyLong_Check(pyIn))
        return unsignedlonglong_PythonToCpp_Katie_QVariant;
    return {};
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QVariant_SignatureStrings[] = {
    "18:KtCore.Katie.QVariant()",
    "17:KtCore.Katie.QVariant(type:KtCore.Katie.QVariant.Type)",
    "16:KtCore.Katie.QVariant(b:bool)",
    "15:KtCore.Katie.QVariant(bytearray:KtCore.Katie.QByteArray)",
    "14:KtCore.Katie.QVariant(qchar:KtCore.Katie.QChar)",
    "13:KtCore.Katie.QVariant(string:KtCore.Katie.QLatin1String)",
    "12:KtCore.Katie.QVariant(regExp:KtCore.Katie.QRegExp)",
    "11:KtCore.Katie.QVariant(string:KtCore.Katie.QString)",
    "10:KtCore.Katie.QVariant(stringlist:KtCore.Katie.QStringList)",
    "9:KtCore.Katie.QVariant(other:KtCore.Katie.QVariant)",
    "8:KtCore.Katie.QVariant(str:str)",
    "7:KtCore.Katie.QVariant(d:double)",
    "6:KtCore.Katie.QVariant(f:float)",
    "5:KtCore.Katie.QVariant(i:int)",
    "4:KtCore.Katie.QVariant(typeOrUserType:int,copy:void)",
    "3:KtCore.Katie.QVariant(typeOrUserType:int,copy:void,flags:unsigned int)",
    "2:KtCore.Katie.QVariant(ll:long long)",
    "1:KtCore.Katie.QVariant(ui:unsigned int)",
    "0:KtCore.Katie.QVariant(ull:unsigned long long)",
    "KtCore.Katie.QVariant.canConvert(t:KtCore.Katie.QVariant.Type)->bool",
    "KtCore.Katie.QVariant.clear()",
    "KtCore.Katie.QVariant.cmp(other:KtCore.Katie.QVariant)->bool",
    "KtCore.Katie.QVariant.constData()->void",
    "KtCore.Katie.QVariant.convert(t:KtCore.Katie.QVariant.Type)->bool",
    "KtCore.Katie.QVariant.create(type:int,copy:void)",
    "1:KtCore.Katie.QVariant.data()->void",
    "0:KtCore.Katie.QVariant.data()->void",
    "KtCore.Katie.QVariant.detach()",
    "KtCore.Katie.QVariant.isDetached()->bool",
    "KtCore.Katie.QVariant.isNull()->bool",
    "KtCore.Katie.QVariant.isValid()->bool",
    "KtCore.Katie.QVariant.nameToType(name:str)->KtCore.Katie.QVariant.Type",
    "KtCore.Katie.QVariant.swap(other:KtCore.Katie.QVariant)",
    "KtCore.Katie.QVariant.toBool()->bool",
    "KtCore.Katie.QVariant.toByteArray()->KtCore.Katie.QByteArray",
    "KtCore.Katie.QVariant.toChar()->KtCore.Katie.QChar",
    "KtCore.Katie.QVariant.toDouble(ok:KtCore.bool=Q_NULLPTR)->double",
    "KtCore.Katie.QVariant.toFloat(ok:KtCore.bool=Q_NULLPTR)->float",
    "KtCore.Katie.QVariant.toInt(ok:KtCore.bool=Q_NULLPTR)->int",
    "KtCore.Katie.QVariant.toLongLong(ok:KtCore.bool=Q_NULLPTR)->long long",
    "KtCore.Katie.QVariant.toReal(ok:KtCore.bool=Q_NULLPTR)->double",
    "KtCore.Katie.QVariant.toRegExp()->KtCore.Katie.QRegExp",
    "KtCore.Katie.QVariant.toString()->KtCore.Katie.QString",
    "KtCore.Katie.QVariant.toStringList()->KtCore.Katie.QStringList",
    "KtCore.Katie.QVariant.toUInt(ok:KtCore.bool=Q_NULLPTR)->unsigned int",
    "KtCore.Katie.QVariant.toULongLong(ok:KtCore.bool=Q_NULLPTR)->unsigned long long",
    "KtCore.Katie.QVariant.type()->KtCore.Katie.QVariant.Type",
    "KtCore.Katie.QVariant.typeName()->str",
    "KtCore.Katie.QVariant.typeToName(type:KtCore.Katie.QVariant.Type)->str",
    "KtCore.Katie.QVariant.userType()->int",
    "KtCore.Katie.QVariant.__copy__()",
    nullptr}; // Sentinel

void init_Katie_QVariant(PyObject* module)
{
    _Sbk_Katie_QVariant_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QVariant",
        "Katie::QVariant",
        &Sbk_Katie_QVariant_spec,
        Katie_QVariant_SignatureStrings,
        &Shiboken::callCppDestructor< ::QVariantWrapper >,
        0,
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QVARIANT_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QVariant_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QVariant_TypeF(),
        QVariant_PythonToCpp_QVariant_PTR,
        is_QVariant_PythonToCpp_QVariant_PTR_Convertible,
        QVariant_PTR_CppToPython_QVariant,
        QVariant_COPY_CppToPython_QVariant);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QVariant");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QVariant*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QVariant&");
    Shiboken::Conversions::registerConverterName(converter, "QVariant");
    Shiboken::Conversions::registerConverterName(converter, "QVariant*");
    Shiboken::Conversions::registerConverterName(converter, "QVariant&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QVariant).name());
    Shiboken::Conversions::registerConverterName(converter, typeid(::QVariantWrapper).name());

    // Add Python to C++ copy (value, not pointer neither reference) conversion to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        QVariant_PythonToCpp_QVariant_COPY,
        is_QVariant_PythonToCpp_QVariant_COPY_Convertible);
    // Add implicit conversions to type converter.
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        Katie_QVariant_Type_PythonToCpp_Katie_QVariant,
        is_Katie_QVariant_Type_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        bool_PythonToCpp_Katie_QVariant,
        is_bool_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QByteArrayREF_PythonToCpp_Katie_QVariant,
        is_constKatie_QByteArrayREF_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QCharREF_PythonToCpp_Katie_QVariant,
        is_constKatie_QCharREF_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QLatin1StringREF_PythonToCpp_Katie_QVariant,
        is_constKatie_QLatin1StringREF_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QRegExpREF_PythonToCpp_Katie_QVariant,
        is_constKatie_QRegExpREF_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QStringREF_PythonToCpp_Katie_QVariant,
        is_constKatie_QStringREF_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constKatie_QStringListREF_PythonToCpp_Katie_QVariant,
        is_constKatie_QStringListREF_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        constcharPTR_PythonToCpp_Katie_QVariant,
        is_constcharPTR_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        double_PythonToCpp_Katie_QVariant,
        is_double_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        float_PythonToCpp_Katie_QVariant,
        is_float_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        int_PythonToCpp_Katie_QVariant,
        is_int_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        longlong_PythonToCpp_Katie_QVariant,
        is_longlong_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        uint_PythonToCpp_Katie_QVariant,
        is_uint_PythonToCpp_Katie_QVariant_Convertible);
    Shiboken::Conversions::addPythonToCppValueConversion(converter,
        unsignedlonglong_PythonToCpp_Katie_QVariant,
        is_unsignedlonglong_PythonToCpp_Katie_QVariant_Convertible);

    // Initialization of enums.

    // Initialization of enum 'Type'.
    SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QVariant_TypeF(),
        "Type",
        "KtCore.Katie.QVariant.Type",
        "Katie::QVariant::Type");
    if (!SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Invalid", (long) Katie::QVariant::Type::Invalid))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Bool", (long) Katie::QVariant::Type::Bool))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Int", (long) Katie::QVariant::Type::Int))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "UInt", (long) Katie::QVariant::Type::UInt))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "LongLong", (long) Katie::QVariant::Type::LongLong))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "ULongLong", (long) Katie::QVariant::Type::ULongLong))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Double", (long) Katie::QVariant::Type::Double))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Float", (long) Katie::QVariant::Type::Float))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Char", (long) Katie::QVariant::Type::Char))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Map", (long) Katie::QVariant::Type::Map))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "List", (long) Katie::QVariant::Type::List))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "String", (long) Katie::QVariant::Type::String))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "StringList", (long) Katie::QVariant::Type::StringList))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "ByteArray", (long) Katie::QVariant::Type::ByteArray))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "BitArray", (long) Katie::QVariant::Type::BitArray))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Date", (long) Katie::QVariant::Type::Date))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Time", (long) Katie::QVariant::Type::Time))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "DateTime", (long) Katie::QVariant::Type::DateTime))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Url", (long) Katie::QVariant::Type::Url))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Locale", (long) Katie::QVariant::Type::Locale))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Rect", (long) Katie::QVariant::Type::Rect))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "RectF", (long) Katie::QVariant::Type::RectF))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Size", (long) Katie::QVariant::Type::Size))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "SizeF", (long) Katie::QVariant::Type::SizeF))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Line", (long) Katie::QVariant::Type::Line))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "LineF", (long) Katie::QVariant::Type::LineF))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Point", (long) Katie::QVariant::Type::Point))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "PointF", (long) Katie::QVariant::Type::PointF))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "RegExp", (long) Katie::QVariant::Type::RegExp))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Hash", (long) Katie::QVariant::Type::Hash))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "EasingCurve", (long) Katie::QVariant::Type::EasingCurve))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "JsonValue", (long) Katie::QVariant::Type::JsonValue))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "JsonObject", (long) Katie::QVariant::Type::JsonObject))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "JsonArray", (long) Katie::QVariant::Type::JsonArray))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "JsonDocument", (long) Katie::QVariant::Type::JsonDocument))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "LastCoreType", (long) Katie::QVariant::Type::LastCoreType))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Font", (long) Katie::QVariant::Type::Font))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Pixmap", (long) Katie::QVariant::Type::Pixmap))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Brush", (long) Katie::QVariant::Type::Brush))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Color", (long) Katie::QVariant::Type::Color))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Palette", (long) Katie::QVariant::Type::Palette))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Icon", (long) Katie::QVariant::Type::Icon))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Image", (long) Katie::QVariant::Type::Image))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Polygon", (long) Katie::QVariant::Type::Polygon))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Region", (long) Katie::QVariant::Type::Region))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Bitmap", (long) Katie::QVariant::Type::Bitmap))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Cursor", (long) Katie::QVariant::Type::Cursor))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "SizePolicy", (long) Katie::QVariant::Type::SizePolicy))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "KeySequence", (long) Katie::QVariant::Type::KeySequence))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Pen", (long) Katie::QVariant::Type::Pen))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "TextLength", (long) Katie::QVariant::Type::TextLength))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "TextFormat", (long) Katie::QVariant::Type::TextFormat))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Matrix", (long) Katie::QVariant::Type::Matrix))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Transform", (long) Katie::QVariant::Type::Transform))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Matrix4x4", (long) Katie::QVariant::Type::Matrix4x4))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Vector2D", (long) Katie::QVariant::Type::Vector2D))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Vector3D", (long) Katie::QVariant::Type::Vector3D))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Vector4D", (long) Katie::QVariant::Type::Vector4D))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "Quaternion", (long) Katie::QVariant::Type::Quaternion))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "LastGuiType", (long) Katie::QVariant::Type::LastGuiType))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "UserType", (long) Katie::QVariant::Type::UserType))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
        Sbk_Katie_QVariant_TypeF(), "LastType", (long) Katie::QVariant::Type::LastType))
        return;
    // Register converter for enum 'Katie::QVariant::Type'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX],
            Katie_QVariant_Type_CppToPython_Katie_QVariant_Type);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QVariant_Type_PythonToCpp_Katie_QVariant_Type,
            is_Katie_QVariant_Type_PythonToCpp_Katie_QVariant_Type_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QVARIANT_TYPE_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QVariant::Type");
        Shiboken::Conversions::registerConverterName(converter, "QVariant::Type");
        Shiboken::Conversions::registerConverterName(converter, "Type");
    }
    // End of 'Type' enum.


}
