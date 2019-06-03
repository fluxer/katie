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
#include "katie_qcoreapplication_wrapper.h"

// inner classes

// Extra includes
#include <qcoreapplication.h>
#include <qobject.h>
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

QCoreApplicationWrapper::QCoreApplicationWrapper(int & argc, char ** argv) : Katie::QCoreApplication(argc, argv)
{
    // ... middle
}

void QCoreApplicationWrapper::connectNotify(const char * signal)
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

void QCoreApplicationWrapper::disconnectNotify(const char * signal)
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

QCoreApplicationWrapper::~QCoreApplicationWrapper()
{
    SbkObject* wrapper = Shiboken::BindingManager::instance().retrieveWrapper(this);
    Shiboken::Object::destroy(wrapper, this);
}

// Target ---------------------------------------------------------

extern "C" {
static int
Sbk_Katie_QCoreApplication_Init(PyObject* self, PyObject* args, PyObject* kwds)
{
    SbkObject* sbkSelf = reinterpret_cast<SbkObject*>(self);
    if (Shiboken::Object::isUserType(self) && !Shiboken::ObjectType::canCallConstructor(self->ob_type, Shiboken::SbkType< ::Katie::QCoreApplication >()))
        return -1;

    ::QCoreApplicationWrapper* cptr{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "QCoreApplication", 1, 1, &(pyArgs[0])))
        return -1;


    // Overloaded function decisor
    // 0: QCoreApplication::QCoreApplication(int&,char**)
    if (numArgs == 1
        && PySequence_Check(pyArgs[0])) {
        overloadId = 0; // QCoreApplication(int&,char**)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplication_Init_TypeError;

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // QCoreApplication(int&,char**)
            // Begin code injection

            int argc_out = PySequence_Size(pyArgs[1-1]);


            Shiboken::AutoArrayPointer<char*> argv_out(argc_out);
            for (int i = 0; i < argc_out; ++i)
                Shiboken::Conversions::pythonToCppCopy(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), PySequence_Fast_GET_ITEM(pyArgs[1-1], i), &(argv_out[i]));

            // End of code injection
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            cptr = new ::QCoreApplicationWrapper(argc_out, argv_out);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred() || !Shiboken::Object::setCppPointer(sbkSelf, Shiboken::SbkType< ::Katie::QCoreApplication >(), cptr)) {
        delete cptr;
        return -1;
    }
    if (!cptr) goto Sbk_Katie_QCoreApplication_Init_TypeError;

    Shiboken::Object::setValidCpp(sbkSelf, true);
    Shiboken::Object::setHasCppWrapper(sbkSelf, true);
    if (Shiboken::BindingManager::instance().hasWrapper(cptr)) {
        Shiboken::BindingManager::instance().releaseWrapper(Shiboken::BindingManager::instance().retrieveWrapper(cptr));
    }
    Shiboken::BindingManager::instance().registerWrapper(sbkSelf, cptr);


    return 1;

    Sbk_Katie_QCoreApplication_Init_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication");
        return -1;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_aboutToQuit(PyObject* self)
{
    QCoreApplicationWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QCoreApplicationWrapper *>(reinterpret_cast< ::Katie::QCoreApplication *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_IDX], reinterpret_cast<SbkObject *>(self))));

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // aboutToQuit()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            static_cast<::QCoreApplicationWrapper*>(cppSelf)->QCoreApplicationWrapper::aboutToQuit_protected();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_addLibraryPath(PyObject* self, PyObject* pyArg)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::addLibraryPath(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // addLibraryPath(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_addLibraryPath_TypeError;

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
            // addLibraryPath(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::addLibraryPath(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_addLibraryPath_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.addLibraryPath");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_applicationDirPath(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // applicationDirPath()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QCoreApplication::applicationDirPath();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_applicationFilePath(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // applicationFilePath()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QCoreApplication::applicationFilePath();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_applicationName(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // applicationName()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QCoreApplication::applicationName();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_applicationPid(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // applicationPid()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            long long cppResult = ::Katie::QCoreApplication::applicationPid();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<long long>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_applicationVersion(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // applicationVersion()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QCoreApplication::applicationVersion();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_arguments(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // arguments()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QStringList cppResult = ::Katie::QCoreApplication::arguments();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_closingDown(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // closingDown()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = ::Katie::QCoreApplication::closingDown();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_exec(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // exec()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            int cppResult = ::Katie::QCoreApplication::exec();
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

static PyObject* Sbk_Katie_QCoreApplicationFunc_exit(PyObject* self, PyObject* args, PyObject* kwds)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.exit(): too many arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|O:exit", &(pyArgs[0])))
        return {};


    // Overloaded function decisor
    // 0: static QCoreApplication::exit(int)
    if (numArgs == 0) {
        overloadId = 0; // exit(int)
    } else if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))) {
        overloadId = 0; // exit(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_exit_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "retcode");
            if (value && pyArgs[0]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.exit(): got multiple values for keyword argument 'retcode'.");
                return {};
            } else if (value) {
                pyArgs[0] = value;
                if (!(pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0]))))
                    goto Sbk_Katie_QCoreApplicationFunc_exit_TypeError;
            }
        }
        int cppArg0 = 0;
        if (pythonToCpp[0]) pythonToCpp[0](pyArgs[0], &cppArg0);

        if (!PyErr_Occurred()) {
            // exit(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::exit(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_exit_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.exit");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_filterEvent(PyObject* self, PyObject* args)
{
    QCoreApplicationWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QCoreApplicationWrapper *>(reinterpret_cast< ::Katie::QCoreApplication *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_IDX], reinterpret_cast<SbkObject *>(self))));
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "filterEvent", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: QCoreApplication::filterEvent(void*,long*)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<void*>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<long>(), (pyArgs[1])))) {
        overloadId = 0; // filterEvent(void*,long*)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_filterEvent_TypeError;

    // Call function/method
    {
        void* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        long* cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // filterEvent(void*,long*)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = cppSelf->filterEvent(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCoreApplicationFunc_filterEvent_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.filterEvent");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_flush(PyObject* self)
{

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // flush()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::flush();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_hasPendingEvents(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // hasPendingEvents()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = ::Katie::QCoreApplication::hasPendingEvents();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_instance(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // instance()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QCoreApplication * cppResult = ::Katie::QCoreApplication::instance();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::pointerToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_IDX]), cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_libraryPaths(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // libraryPaths()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QStringList cppResult = ::Katie::QCoreApplication::libraryPaths();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_organizationDomain(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // organizationDomain()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QCoreApplication::organizationDomain();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_organizationName(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // organizationName()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QCoreApplication::organizationName();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_processEvents(PyObject* self)
{

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // processEvents()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::processEvents();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_quit(PyObject* self)
{

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // quit()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::quit();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_removeLibraryPath(PyObject* self, PyObject* pyArg)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::removeLibraryPath(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // removeLibraryPath(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_removeLibraryPath_TypeError;

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
            // removeLibraryPath(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::removeLibraryPath(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_removeLibraryPath_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.removeLibraryPath");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_removePostedEvents(PyObject* self, PyObject* args)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "removePostedEvents", 1, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QCoreApplication::removePostedEvents(Katie::QObject*)
    // 1: static QCoreApplication::removePostedEvents(Katie::QObject*,int)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // removePostedEvents(Katie::QObject*)
        } else if (numArgs == 2
            && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
            overloadId = 1; // removePostedEvents(Katie::QObject*,int)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_removePostedEvents_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // removePostedEvents(Katie::QObject * receiver)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QObject* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);

            if (!PyErr_Occurred()) {
                // removePostedEvents(Katie::QObject*)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ::Katie::QCoreApplication::removePostedEvents(cppArg0);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // removePostedEvents(Katie::QObject * receiver, int eventType)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QObject* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // removePostedEvents(Katie::QObject*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ::Katie::QCoreApplication::removePostedEvents(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_removePostedEvents_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.removePostedEvents");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_sendPostedEvents(PyObject* self, PyObject* args)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs == 1)
        goto Sbk_Katie_QCoreApplicationFunc_sendPostedEvents_TypeError;

    if (!PyArg_UnpackTuple(args, "sendPostedEvents", 0, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QCoreApplication::sendPostedEvents()
    // 1: static QCoreApplication::sendPostedEvents(Katie::QObject*,int)
    if (numArgs == 0) {
        overloadId = 0; // sendPostedEvents()
    } else if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppPointerConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[1])))) {
        overloadId = 1; // sendPostedEvents(Katie::QObject*,int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_sendPostedEvents_TypeError;

    // Call function/method
    switch (overloadId) {
        case 0: // sendPostedEvents()
        {

            if (!PyErr_Occurred()) {
                // sendPostedEvents()
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ::Katie::QCoreApplication::sendPostedEvents();
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
        case 1: // sendPostedEvents(Katie::QObject * receiver, int event_type)
        {
            if (!Shiboken::Object::isValid(pyArgs[0]))
                return {};
            ::Katie::QObject* cppArg0;
            pythonToCpp[0](pyArgs[0], &cppArg0);
            int cppArg1;
            pythonToCpp[1](pyArgs[1], &cppArg1);

            if (!PyErr_Occurred()) {
                // sendPostedEvents(Katie::QObject*,int)
                PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
                ::Katie::QCoreApplication::sendPostedEvents(cppArg0, cppArg1);
                PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            }
            break;
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_sendPostedEvents_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.sendPostedEvents");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_setApplicationName(PyObject* self, PyObject* pyArg)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::setApplicationName(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // setApplicationName(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_setApplicationName_TypeError;

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
            // setApplicationName(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::setApplicationName(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_setApplicationName_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.setApplicationName");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_setApplicationVersion(PyObject* self, PyObject* pyArg)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::setApplicationVersion(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // setApplicationVersion(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_setApplicationVersion_TypeError;

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
            // setApplicationVersion(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::setApplicationVersion(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_setApplicationVersion_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.setApplicationVersion");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_setAttribute(PyObject* self, PyObject* args, PyObject* kwds)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numNamedArgs = (kwds ? PyDict_Size(kwds) : 0);
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths
    if (numArgs + numNamedArgs > 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.setAttribute(): too many arguments");
        return {};
    } else if (numArgs < 1) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.setAttribute(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OO:setAttribute", &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QCoreApplication::setAttribute(Katie::Qt::ApplicationAttribute,bool)
    if ((pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX])->converter, (pyArgs[0])))) {
        if (numArgs == 1) {
            overloadId = 0; // setAttribute(Katie::Qt::ApplicationAttribute,bool)
        } else if ((pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[1])))) {
            overloadId = 0; // setAttribute(Katie::Qt::ApplicationAttribute,bool)
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_setAttribute_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "on");
            if (value && pyArgs[1]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.setAttribute(): got multiple values for keyword argument 'on'.");
                return {};
            } else if (value) {
                pyArgs[1] = value;
                if (!(pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[1]))))
                    goto Sbk_Katie_QCoreApplicationFunc_setAttribute_TypeError;
            }
        }
        ::Katie::Qt::ApplicationAttribute cppArg0{Katie::Qt::AA_ImmediateWidgetCreation};
        pythonToCpp[0](pyArgs[0], &cppArg0);
        bool cppArg1 = true;
        if (pythonToCpp[1]) pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // setAttribute(Katie::Qt::ApplicationAttribute,bool)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::setAttribute(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_setAttribute_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.setAttribute");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_setLibraryPaths(PyObject* self, PyObject* pyArg)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::setLibraryPaths(Katie::QStringList)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRINGLIST_IDX]), (pyArg)))) {
        overloadId = 0; // setLibraryPaths(Katie::QStringList)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_setLibraryPaths_TypeError;

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
            // setLibraryPaths(Katie::QStringList)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::setLibraryPaths(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_setLibraryPaths_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.setLibraryPaths");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_setOrganizationDomain(PyObject* self, PyObject* pyArg)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::setOrganizationDomain(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // setOrganizationDomain(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_setOrganizationDomain_TypeError;

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
            // setOrganizationDomain(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::setOrganizationDomain(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_setOrganizationDomain_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.setOrganizationDomain");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_setOrganizationName(PyObject* self, PyObject* pyArg)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::setOrganizationName(Katie::QString)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppReferenceConvertible(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), (pyArg)))) {
        overloadId = 0; // setOrganizationName(Katie::QString)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_setOrganizationName_TypeError;

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
            // setOrganizationName(Katie::QString)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::setOrganizationName(*cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_setOrganizationName_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.setOrganizationName");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_startingUp(PyObject* self)
{
    PyObject* pyResult{};

    // Call function/method
    {

        if (!PyErr_Occurred()) {
            // startingUp()
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = ::Katie::QCoreApplication::startingUp();
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_testAttribute(PyObject* self, PyObject* pyArg)
{
    PyObject* pyResult{};
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: static QCoreApplication::testAttribute(Katie::Qt::ApplicationAttribute)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QT_APPLICATIONATTRIBUTE_IDX])->converter, (pyArg)))) {
        overloadId = 0; // testAttribute(Katie::Qt::ApplicationAttribute)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_testAttribute_TypeError;

    // Call function/method
    {
        ::Katie::Qt::ApplicationAttribute cppArg0{Katie::Qt::AA_ImmediateWidgetCreation};
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // testAttribute(Katie::Qt::ApplicationAttribute)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            bool cppResult = ::Katie::QCoreApplication::testAttribute(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCoreApplicationFunc_testAttribute_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.testAttribute");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_tr(PyObject* self, PyObject* args)
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
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_tr_TypeError;

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
            Katie::QString cppResult = ::Katie::QCoreApplication::tr(cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCoreApplicationFunc_tr_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.tr");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_trUtf8(PyObject* self, PyObject* args)
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
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_trUtf8_TypeError;

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
            Katie::QString cppResult = ::Katie::QCoreApplication::trUtf8(cppArg0, cppArg1, cppArg2);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCoreApplicationFunc_trUtf8_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.trUtf8");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_translate(PyObject* self, PyObject* args, PyObject* kwds)
{
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
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.translate(): too many arguments");
        return {};
    } else if (numArgs < 2) {
        PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.translate(): not enough arguments");
        return {};
    }

    if (!PyArg_ParseTuple(args, "|OOOOO:translate", &(pyArgs[0]), &(pyArgs[1]), &(pyArgs[2]), &(pyArgs[3]), &(pyArgs[4])))
        return {};


    // Overloaded function decisor
    // 0: static QCoreApplication::translate(const char*,const char*,const char*,Katie::QCoreApplication::Encoding,int)
    if (numArgs >= 2
        && Shiboken::String::check(pyArgs[0]) && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[0])))
        && Shiboken::String::check(pyArgs[1]) && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[1])))) {
        if (numArgs == 2) {
            overloadId = 0; // translate(const char*,const char*,const char*,Katie::QCoreApplication::Encoding,int)
        } else if (Shiboken::String::check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[2])))) {
            if (numArgs == 3) {
                overloadId = 0; // translate(const char*,const char*,const char*,Katie::QCoreApplication::Encoding,int)
            } else if ((pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX])->converter, (pyArgs[3])))) {
                if (numArgs == 4) {
                    overloadId = 0; // translate(const char*,const char*,const char*,Katie::QCoreApplication::Encoding,int)
                } else if ((pythonToCpp[4] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[4])))) {
                    overloadId = 0; // translate(const char*,const char*,const char*,Katie::QCoreApplication::Encoding,int)
                }
            }
        }
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_translate_TypeError;

    // Call function/method
    {
        if (kwds) {
            PyObject* value = PyDict_GetItemString(kwds, "disambiguation");
            if (value && pyArgs[2]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.translate(): got multiple values for keyword argument 'disambiguation'.");
                return {};
            } else if (value) {
                pyArgs[2] = value;
                if (!Shiboken::String::check(pyArgs[2]) && (pythonToCpp[2] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<const char*>(), (pyArgs[2]))))
                    goto Sbk_Katie_QCoreApplicationFunc_translate_TypeError;
            }
            value = PyDict_GetItemString(kwds, "encoding");
            if (value && pyArgs[3]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.translate(): got multiple values for keyword argument 'encoding'.");
                return {};
            } else if (value) {
                pyArgs[3] = value;
                if (!(pythonToCpp[3] = Shiboken::Conversions::isPythonToCppConvertible(*PepType_SGTP(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX])->converter, (pyArgs[3]))))
                    goto Sbk_Katie_QCoreApplicationFunc_translate_TypeError;
            }
            value = PyDict_GetItemString(kwds, "n");
            if (value && pyArgs[4]) {
                PyErr_SetString(PyExc_TypeError, "KtCore.Katie.QCoreApplication.translate(): got multiple values for keyword argument 'n'.");
                return {};
            } else if (value) {
                pyArgs[4] = value;
                if (!(pythonToCpp[4] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[4]))))
                    goto Sbk_Katie_QCoreApplicationFunc_translate_TypeError;
            }
        }
        const char* cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        const char* cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);
        const char* cppArg2 = Q_NULLPTR;
        if (pythonToCpp[2]) pythonToCpp[2](pyArgs[2], &cppArg2);
        ::Katie::QCoreApplication::Encoding cppArg3 = Katie::QCoreApplication::Encoding::CodecForTr;
        if (pythonToCpp[3]) pythonToCpp[3](pyArgs[3], &cppArg3);
        int cppArg4 = -1;
        if (pythonToCpp[4]) pythonToCpp[4](pyArgs[4], &cppArg4);

        if (!PyErr_Occurred()) {
            // translate(const char*,const char*,const char*,Katie::QCoreApplication::Encoding,int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            Katie::QString cppResult = ::Katie::QCoreApplication::translate(cppArg0, cppArg1, cppArg2, cppArg3, cppArg4);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
            pyResult = Shiboken::Conversions::copyToPython(reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QSTRING_IDX]), &cppResult);
        }
    }

    if (PyErr_Occurred() || !pyResult) {
        Py_XDECREF(pyResult);
        return {};
    }
    return pyResult;

    Sbk_Katie_QCoreApplicationFunc_translate_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.translate");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_unixSignal(PyObject* self, PyObject* pyArg)
{
    QCoreApplicationWrapper* cppSelf = nullptr;
    SBK_UNUSED(cppSelf)
    if (!Shiboken::Object::isValid(self))
        return {};
    cppSelf = static_cast<QCoreApplicationWrapper *>(reinterpret_cast< ::Katie::QCoreApplication *>(Shiboken::Conversions::cppPointer(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_IDX], reinterpret_cast<SbkObject *>(self))));
    int overloadId = -1;
    PythonToCppFunc pythonToCpp{};
    SBK_UNUSED(pythonToCpp)

    // Overloaded function decisor
    // 0: QCoreApplication::unixSignal(int)
    if ((pythonToCpp = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArg)))) {
        overloadId = 0; // unixSignal(int)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_unixSignal_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp(pyArg, &cppArg0);

        if (!PyErr_Occurred()) {
            // unixSignal(int)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            static_cast<::QCoreApplicationWrapper*>(cppSelf)->QCoreApplicationWrapper::unixSignal_protected(cppArg0);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_unixSignal_TypeError:
        Shiboken::setErrorAboutWrongArguments(pyArg, "KtCore.Katie.QCoreApplication.unixSignal");
        return {};
}

static PyObject* Sbk_Katie_QCoreApplicationFunc_watchUnixSignal(PyObject* self, PyObject* args)
{
    int overloadId = -1;
    PythonToCppFunc pythonToCpp[] = { nullptr, nullptr };
    SBK_UNUSED(pythonToCpp)
    int numArgs = PyTuple_GET_SIZE(args);
    SBK_UNUSED(numArgs)
    PyObject* pyArgs[] = {0, 0};

    // invalid argument lengths


    if (!PyArg_UnpackTuple(args, "watchUnixSignal", 2, 2, &(pyArgs[0]), &(pyArgs[1])))
        return {};


    // Overloaded function decisor
    // 0: static QCoreApplication::watchUnixSignal(int,bool)
    if (numArgs == 2
        && (pythonToCpp[0] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<int>(), (pyArgs[0])))
        && (pythonToCpp[1] = Shiboken::Conversions::isPythonToCppConvertible(Shiboken::Conversions::PrimitiveTypeConverter<bool>(), (pyArgs[1])))) {
        overloadId = 0; // watchUnixSignal(int,bool)
    }

    // Function signature not found.
    if (overloadId == -1) goto Sbk_Katie_QCoreApplicationFunc_watchUnixSignal_TypeError;

    // Call function/method
    {
        int cppArg0;
        pythonToCpp[0](pyArgs[0], &cppArg0);
        bool cppArg1;
        pythonToCpp[1](pyArgs[1], &cppArg1);

        if (!PyErr_Occurred()) {
            // watchUnixSignal(int,bool)
            PyThreadState* _save = PyEval_SaveThread(); // Py_BEGIN_ALLOW_THREADS
            ::Katie::QCoreApplication::watchUnixSignal(cppArg0, cppArg1);
            PyEval_RestoreThread(_save); // Py_END_ALLOW_THREADS
        }
    }

    if (PyErr_Occurred()) {
        return {};
    }
    Py_RETURN_NONE;

    Sbk_Katie_QCoreApplicationFunc_watchUnixSignal_TypeError:
        Shiboken::setErrorAboutWrongArguments(args, "KtCore.Katie.QCoreApplication.watchUnixSignal");
        return {};
}

static PyMethodDef Sbk_Katie_QCoreApplication_methods[] = {
    {"aboutToQuit", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_aboutToQuit), METH_NOARGS},
    {"addLibraryPath", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_addLibraryPath), METH_O|METH_STATIC},
    {"applicationDirPath", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_applicationDirPath), METH_NOARGS|METH_STATIC},
    {"applicationFilePath", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_applicationFilePath), METH_NOARGS|METH_STATIC},
    {"applicationName", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_applicationName), METH_NOARGS|METH_STATIC},
    {"applicationPid", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_applicationPid), METH_NOARGS|METH_STATIC},
    {"applicationVersion", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_applicationVersion), METH_NOARGS|METH_STATIC},
    {"arguments", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_arguments), METH_NOARGS|METH_STATIC},
    {"closingDown", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_closingDown), METH_NOARGS|METH_STATIC},
    {"exec", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_exec), METH_NOARGS|METH_STATIC},
    {"exit", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_exit), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"filterEvent", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_filterEvent), METH_VARARGS},
    {"flush", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_flush), METH_NOARGS|METH_STATIC},
    {"hasPendingEvents", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_hasPendingEvents), METH_NOARGS|METH_STATIC},
    {"instance", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_instance), METH_NOARGS|METH_STATIC},
    {"libraryPaths", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_libraryPaths), METH_NOARGS|METH_STATIC},
    {"organizationDomain", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_organizationDomain), METH_NOARGS|METH_STATIC},
    {"organizationName", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_organizationName), METH_NOARGS|METH_STATIC},
    {"processEvents", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_processEvents), METH_NOARGS|METH_STATIC},
    {"quit", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_quit), METH_NOARGS|METH_STATIC},
    {"removeLibraryPath", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_removeLibraryPath), METH_O|METH_STATIC},
    {"removePostedEvents", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_removePostedEvents), METH_VARARGS|METH_STATIC},
    {"sendPostedEvents", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_sendPostedEvents), METH_VARARGS|METH_STATIC},
    {"setApplicationName", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_setApplicationName), METH_O|METH_STATIC},
    {"setApplicationVersion", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_setApplicationVersion), METH_O|METH_STATIC},
    {"setAttribute", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_setAttribute), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"setLibraryPaths", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_setLibraryPaths), METH_O|METH_STATIC},
    {"setOrganizationDomain", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_setOrganizationDomain), METH_O|METH_STATIC},
    {"setOrganizationName", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_setOrganizationName), METH_O|METH_STATIC},
    {"startingUp", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_startingUp), METH_NOARGS|METH_STATIC},
    {"testAttribute", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_testAttribute), METH_O|METH_STATIC},
    {"tr", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_tr), METH_VARARGS|METH_STATIC},
    {"trUtf8", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_trUtf8), METH_VARARGS|METH_STATIC},
    {"translate", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_translate), METH_VARARGS|METH_KEYWORDS|METH_STATIC},
    {"unixSignal", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_unixSignal), METH_O},
    {"watchUnixSignal", reinterpret_cast<PyCFunction>(Sbk_Katie_QCoreApplicationFunc_watchUnixSignal), METH_VARARGS|METH_STATIC},

    {nullptr, nullptr} // Sentinel
};

} // extern "C"

static int Sbk_Katie_QCoreApplication_traverse(PyObject* self, visitproc visit, void* arg)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_traverse(self, visit, arg);
}
static int Sbk_Katie_QCoreApplication_clear(PyObject* self)
{
    return reinterpret_cast<PyTypeObject *>(SbkObject_TypeF())->tp_clear(self);
}
// Class Definition -----------------------------------------------
extern "C" {
static SbkObjectType *_Sbk_Katie_QCoreApplication_Type = nullptr;
static SbkObjectType *Sbk_Katie_QCoreApplication_TypeF(void)
{
    return _Sbk_Katie_QCoreApplication_Type;
}

static PyType_Slot Sbk_Katie_QCoreApplication_slots[] = {
    {Py_tp_base,        nullptr}, // inserted by introduceWrapperType
    {Py_tp_dealloc,     reinterpret_cast<void*>(&SbkDeallocQAppWrapper)},
    {Py_tp_repr,        nullptr},
    {Py_tp_hash,        nullptr},
    {Py_tp_call,        nullptr},
    {Py_tp_str,         nullptr},
    {Py_tp_getattro,    nullptr},
    {Py_tp_setattro,    nullptr},
    {Py_tp_traverse,    reinterpret_cast<void*>(Sbk_Katie_QCoreApplication_traverse)},
    {Py_tp_clear,       reinterpret_cast<void*>(Sbk_Katie_QCoreApplication_clear)},
    {Py_tp_richcompare, nullptr},
    {Py_tp_iter,        nullptr},
    {Py_tp_iternext,    nullptr},
    {Py_tp_methods,     reinterpret_cast<void*>(Sbk_Katie_QCoreApplication_methods)},
    {Py_tp_getset,      nullptr},
    {Py_tp_init,        reinterpret_cast<void*>(Sbk_Katie_QCoreApplication_Init)},
    {Py_tp_new,         reinterpret_cast<void*>(SbkQAppTpNew)},
    {0, nullptr}
};
static PyType_Spec Sbk_Katie_QCoreApplication_spec = {
    "KtCore.Katie.QCoreApplication",
    sizeof(SbkObject),
    0,
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE|Py_TPFLAGS_CHECKTYPES,
    Sbk_Katie_QCoreApplication_slots
};

} //extern "C"

static void* Sbk_Katie_QCoreApplication_typeDiscovery(void* cptr, SbkObjectType* instanceType)
{
    if (instanceType == reinterpret_cast<SbkObjectType*>(Shiboken::SbkType< ::Katie::QObject >()))
        return dynamic_cast< ::Katie::QCoreApplication*>(reinterpret_cast< ::Katie::QObject*>(cptr));
    return {};
}


// Type conversion functions.

// Python to C++ enum conversion.
static void Katie_QCoreApplication_Encoding_PythonToCpp_Katie_QCoreApplication_Encoding(PyObject* pyIn, void* cppOut) {
    *reinterpret_cast<::Katie::QCoreApplication::Encoding*>(cppOut) =
        static_cast<::Katie::QCoreApplication::Encoding>(Shiboken::Enum::getValue(pyIn));

}
static PythonToCppFunc is_Katie_QCoreApplication_Encoding_PythonToCpp_Katie_QCoreApplication_Encoding_Convertible(PyObject* pyIn) {
    if (PyObject_TypeCheck(pyIn, SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX]))
        return Katie_QCoreApplication_Encoding_PythonToCpp_Katie_QCoreApplication_Encoding;
    return {};
}
static PyObject* Katie_QCoreApplication_Encoding_CppToPython_Katie_QCoreApplication_Encoding(const void* cppIn) {
    const int castCppIn = int(*reinterpret_cast<const ::Katie::QCoreApplication::Encoding *>(cppIn));
    return Shiboken::Enum::newItem(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX], castCppIn);

}

// Python to C++ pointer conversion - returns the C++ object of the Python wrapper (keeps object identity).
static void QCoreApplication_PythonToCpp_QCoreApplication_PTR(PyObject* pyIn, void* cppOut) {
    Shiboken::Conversions::pythonToCppPointer(Sbk_Katie_QCoreApplication_TypeF(), pyIn, cppOut);
}
static PythonToCppFunc is_QCoreApplication_PythonToCpp_QCoreApplication_PTR_Convertible(PyObject* pyIn) {
    if (pyIn == Py_None)
        return Shiboken::Conversions::nonePythonToCppNullPtr;
    if (PyObject_TypeCheck(pyIn, reinterpret_cast<PyTypeObject*>(Sbk_Katie_QCoreApplication_TypeF())))
        return QCoreApplication_PythonToCpp_QCoreApplication_PTR;
    return {};
}

// C++ to Python pointer conversion - tries to find the Python wrapper for the C++ object (keeps object identity).
static PyObject* QCoreApplication_PTR_CppToPython_QCoreApplication(const void* cppIn) {
    auto pyOut = reinterpret_cast<PyObject*>(Shiboken::BindingManager::instance().retrieveWrapper(cppIn));
    if (pyOut) {
        Py_INCREF(pyOut);
        return pyOut;
    }
    bool changedTypeName = false;
    auto tCppIn = reinterpret_cast<const ::Katie::QCoreApplication *>(cppIn);
    const char *typeName = typeid(*tCppIn).name();
    auto sbkType = Shiboken::ObjectType::typeForTypeName(typeName);
    if (sbkType && Shiboken::ObjectType::hasSpecialCastFunction(sbkType)) {
        typeName = typeNameOf(tCppIn);
        changedTypeName = true;
     }
    PyObject *result = Shiboken::Object::newObject(Sbk_Katie_QCoreApplication_TypeF(), const_cast<void*>(cppIn), false, /* exactType */ changedTypeName, typeName);
    if (changedTypeName)
        delete [] typeName;
    return result;
}

// The signatures string for the functions.
// Multiple signatures have their index "n:" in front.
static const char *Katie_QCoreApplication_SignatureStrings[] = {
    "KtCore.Katie.QCoreApplication(argc:KtCore.int,argv:KtCore.char)",
    "KtCore.Katie.QCoreApplication.aboutToQuit()",
    "KtCore.Katie.QCoreApplication.addLibraryPath(arg__1:KtCore.Katie.QString)",
    "KtCore.Katie.QCoreApplication.applicationDirPath()->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.applicationFilePath()->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.applicationName()->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.applicationPid()->long long",
    "KtCore.Katie.QCoreApplication.applicationVersion()->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.arguments()->KtCore.Katie.QStringList",
    "KtCore.Katie.QCoreApplication.closingDown()->bool",
    "KtCore.Katie.QCoreApplication.exec()->int",
    "KtCore.Katie.QCoreApplication.exit(retcode:int=0)",
    "KtCore.Katie.QCoreApplication.filterEvent(message:void,result:KtCore.long)->bool",
    "KtCore.Katie.QCoreApplication.flush()",
    "KtCore.Katie.QCoreApplication.hasPendingEvents()->bool",
    "KtCore.Katie.QCoreApplication.instance()->KtCore.Katie.QCoreApplication",
    "KtCore.Katie.QCoreApplication.libraryPaths()->KtCore.Katie.QStringList",
    "KtCore.Katie.QCoreApplication.organizationDomain()->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.organizationName()->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.processEvents()",
    "KtCore.Katie.QCoreApplication.quit()",
    "KtCore.Katie.QCoreApplication.removeLibraryPath(arg__1:KtCore.Katie.QString)",
    "1:KtCore.Katie.QCoreApplication.removePostedEvents(receiver:KtCore.Katie.QObject)",
    "0:KtCore.Katie.QCoreApplication.removePostedEvents(receiver:KtCore.Katie.QObject,eventType:int)",
    "1:KtCore.Katie.QCoreApplication.sendPostedEvents()",
    "0:KtCore.Katie.QCoreApplication.sendPostedEvents(receiver:KtCore.Katie.QObject,event_type:int)",
    "KtCore.Katie.QCoreApplication.setApplicationName(application:KtCore.Katie.QString)",
    "KtCore.Katie.QCoreApplication.setApplicationVersion(version:KtCore.Katie.QString)",
    "KtCore.Katie.QCoreApplication.setAttribute(attribute:KtCore.Katie.Qt.ApplicationAttribute,on:bool=true)",
    "KtCore.Katie.QCoreApplication.setLibraryPaths(arg__1:KtCore.Katie.QStringList)",
    "KtCore.Katie.QCoreApplication.setOrganizationDomain(orgDomain:KtCore.Katie.QString)",
    "KtCore.Katie.QCoreApplication.setOrganizationName(orgName:KtCore.Katie.QString)",
    "KtCore.Katie.QCoreApplication.startingUp()->bool",
    "KtCore.Katie.QCoreApplication.testAttribute(attribute:KtCore.Katie.Qt.ApplicationAttribute)->bool",
    "KtCore.Katie.QCoreApplication.tr(s:str,c:str,n:int)->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.trUtf8(s:str,c:str,n:int)->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.translate(context:str,sourceText:str,disambiguation:str=Q_NULLPTR,encoding:KtCore.Katie.QCoreApplication.Encoding=CodecForTr,n:int=-1)->KtCore.Katie.QString",
    "KtCore.Katie.QCoreApplication.unixSignal(arg__1:int)",
    "KtCore.Katie.QCoreApplication.watchUnixSignal(signal:int,watch:bool)",
    nullptr}; // Sentinel

void init_Katie_QCoreApplication(PyObject* module)
{
    _Sbk_Katie_QCoreApplication_Type = Shiboken::ObjectType::introduceWrapperType(
        module,
        "QCoreApplication",
        "Katie::QCoreApplication*",
        &Sbk_Katie_QCoreApplication_spec,
        Katie_QCoreApplication_SignatureStrings,
        &Shiboken::callCppDestructor< ::Katie::QCoreApplication >,
        reinterpret_cast<SbkObjectType *>(SbkKtCoreTypes[SBK_KATIE_QOBJECT_IDX]),
        0,
        0    );
    
    SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_IDX]
        = reinterpret_cast<PyTypeObject*>(Sbk_Katie_QCoreApplication_TypeF());

    // Register Converter
    SbkConverter* converter = Shiboken::Conversions::createConverter(Sbk_Katie_QCoreApplication_TypeF(),
        QCoreApplication_PythonToCpp_QCoreApplication_PTR,
        is_QCoreApplication_PythonToCpp_QCoreApplication_PTR_Convertible,
        QCoreApplication_PTR_CppToPython_QCoreApplication);

    Shiboken::Conversions::registerConverterName(converter, "Katie::QCoreApplication");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QCoreApplication*");
    Shiboken::Conversions::registerConverterName(converter, "Katie::QCoreApplication&");
    Shiboken::Conversions::registerConverterName(converter, "QCoreApplication");
    Shiboken::Conversions::registerConverterName(converter, "QCoreApplication*");
    Shiboken::Conversions::registerConverterName(converter, "QCoreApplication&");
    Shiboken::Conversions::registerConverterName(converter, typeid(::Katie::QCoreApplication).name());
    Shiboken::Conversions::registerConverterName(converter, typeid(::QCoreApplicationWrapper).name());


    Shiboken::ObjectType::setTypeDiscoveryFunctionV2(Sbk_Katie_QCoreApplication_TypeF(), &Sbk_Katie_QCoreApplication_typeDiscovery);

    // Initialization of enums.

    // Initialization of enum 'Encoding'.
    SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX] = Shiboken::Enum::createScopedEnum(Sbk_Katie_QCoreApplication_TypeF(),
        "Encoding",
        "KtCore.Katie.QCoreApplication.Encoding",
        "Katie::QCoreApplication::Encoding");
    if (!SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX])
        return;

    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX],
        Sbk_Katie_QCoreApplication_TypeF(), "CodecForTr", (long) Katie::QCoreApplication::Encoding::CodecForTr))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX],
        Sbk_Katie_QCoreApplication_TypeF(), "UnicodeUTF8", (long) Katie::QCoreApplication::Encoding::UnicodeUTF8))
        return;
    if (!Shiboken::Enum::createScopedEnumItem(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX],
        Sbk_Katie_QCoreApplication_TypeF(), "DefaultCodec", (long) Katie::QCoreApplication::Encoding::DefaultCodec))
        return;
    // Register converter for enum 'Katie::QCoreApplication::Encoding'.
    {
        SbkConverter* converter = Shiboken::Conversions::createConverter(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX],
            Katie_QCoreApplication_Encoding_CppToPython_Katie_QCoreApplication_Encoding);
        Shiboken::Conversions::addPythonToCppValueConversion(converter,
            Katie_QCoreApplication_Encoding_PythonToCpp_Katie_QCoreApplication_Encoding,
            is_Katie_QCoreApplication_Encoding_PythonToCpp_Katie_QCoreApplication_Encoding_Convertible);
        Shiboken::Enum::setTypeConverter(SbkKtCoreTypes[SBK_KATIE_QCOREAPPLICATION_ENCODING_IDX], converter);
        Shiboken::Conversions::registerConverterName(converter, "Katie::QCoreApplication::Encoding");
        Shiboken::Conversions::registerConverterName(converter, "QCoreApplication::Encoding");
        Shiboken::Conversions::registerConverterName(converter, "Encoding");
    }
    // End of 'Encoding' enum.


}
