/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016 Ivailo Monev
**
** This file is part of the QtCore module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

// Generated code, do not edit! Use generator at tools/qtconcurrent/generaterun/
#ifndef QTCONCURRENT_RUN_H
#define QTCONCURRENT_RUN_H

#include <QtCore/qglobal.h>

#ifndef QT_NO_CONCURRENT

#include <QtCore/qtconcurrentrunbase.h>
#include <QtCore/qtconcurrentstoredfunctioncall.h>

#include <type_traits>

QT_BEGIN_NAMESPACE

namespace QtConcurrent {

template <typename T>
QFuture<T> run(T (*functionPointer)())
{
    return (new StoredFunctorCall0<T, T (*)()>(functionPointer))->start();
}
template <typename T, typename Param1, typename Arg1>
QFuture<T> run(T (*functionPointer)(Param1), const Arg1 &arg1)
{
    return (new StoredFunctorCall1<T, T (*)(Param1), Arg1>(functionPointer, arg1))->start();
}
template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2>
QFuture<T> run(T (*functionPointer)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
{
    return (new StoredFunctorCall2<T, T (*)(Param1, Param2), Arg1, Arg2>(functionPointer, arg1, arg2))->start();
}
template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
QFuture<T> run(T (*functionPointer)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return (new StoredFunctorCall3<T, T (*)(Param1, Param2, Param3), Arg1, Arg2, Arg3>(functionPointer, arg1, arg2, arg3))->start();
}
template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
QFuture<T> run(T (*functionPointer)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    return (new StoredFunctorCall4<T, T (*)(Param1, Param2, Param3, Param4), Arg1, Arg2, Arg3, Arg4>(functionPointer, arg1, arg2, arg3, arg4))->start();
}
template <typename T, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
QFuture<T> run(T (*functionPointer)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    return (new StoredFunctorCall5<T, T (*)(Param1, Param2, Param3, Param4, Param5), Arg1, Arg2, Arg3, Arg4, Arg5>(functionPointer, arg1, arg2, arg3, arg4, arg5))->start();
}

template <typename Functor>
auto run(Functor functor) -> typename std::enable_if<!QtPrivate::HasResultType<Functor>::Value, QFuture<decltype(functor())> >::Type
{
    typedef decltype(functor()) result_type;
    return (new StoredFunctorCall0<result_type, Functor>(functor))->start();
}

template <typename Functor, typename Arg1>
auto run(Functor functor, const Arg1 &arg1)
    -> typename std::enable_if<!QtPrivate::HasResultType<Functor>::Value, QFuture<decltype(functor(arg1))> >::Type
{
    typedef decltype(functor(arg1)) result_type;
    return (new StoredFunctorCall1<result_type, Functor, Arg1>(functor, arg1))->start();
}

template <typename Functor, typename Arg1, typename Arg2>
auto run(Functor functor, const Arg1 &arg1, const Arg2 &arg2)
    -> typename std::enable_if<!QtPrivate::HasResultType<Functor>::Value, QFuture<decltype(functor(arg1, arg2))> >::Type
{
    typedef decltype(functor(arg1, arg2)) result_type;
    return (new StoredFunctorCall2<result_type, Functor, Arg1, Arg2>(functor, arg1, arg2))->start();
}

template <typename Functor, typename Arg1, typename Arg2, typename Arg3>
auto run(Functor functor, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
    -> typename std::enable_if<!QtPrivate::HasResultType<Functor>::Value, QFuture<decltype(functor(arg1, arg2, arg3))> >::Type
{
    typedef decltype(functor(arg1, arg2, arg3)) result_type;
    return (new StoredFunctorCall3<result_type, Functor, Arg1, Arg2, Arg3>(functor, arg1, arg2, arg3))->start();
}

template <typename Functor, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
auto run(Functor functor, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
    -> typename std::enable_if<!QtPrivate::HasResultType<Functor>::Value, QFuture<decltype(functor(arg1, arg2, arg3, arg4))> >::Type
{
    typedef decltype(functor(arg1, arg2, arg3, arg4)) result_type;
    return (new StoredFunctorCall4<result_type, Functor, Arg1, Arg2, Arg3, Arg4>(functor, arg1, arg2, arg3, arg4))->start();
}

template <typename Functor, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
auto run(Functor functor, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
    -> typename std::enable_if<!QtPrivate::HasResultType<Functor>::Value, QFuture<decltype(functor(arg1, arg2, arg3, arg4, arg5))> >::Type
{
    typedef decltype(functor(arg1, arg2, arg3, arg4, arg5)) result_type;
    return (new StoredFunctorCall5<result_type, Functor, Arg1, Arg2, Arg3, Arg4, Arg5>(functor, arg1, arg2, arg3, arg4, arg5))->start();
}

template <typename FunctionObject>
QFuture<typename FunctionObject::result_type> run(FunctionObject functionObject)
{
    return (new StoredFunctorCall0<Q_TYPENAME FunctionObject::result_type, FunctionObject>(functionObject))->start();
}
template <typename FunctionObject, typename Arg1>
QFuture<typename FunctionObject::result_type> run(FunctionObject functionObject, const Arg1 &arg1)
{
    return (new StoredFunctorCall1<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1>(functionObject, arg1))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2>
QFuture<typename FunctionObject::result_type> run(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2)
{
    return (new StoredFunctorCall2<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2>(functionObject, arg1, arg2))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3>
QFuture<typename FunctionObject::result_type> run(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return (new StoredFunctorCall3<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3>(functionObject, arg1, arg2, arg3))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
QFuture<typename FunctionObject::result_type> run(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    return (new StoredFunctorCall4<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4>(functionObject, arg1, arg2, arg3, arg4))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
QFuture<typename FunctionObject::result_type> run(FunctionObject functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    return (new StoredFunctorCall5<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4, Arg5>(functionObject, arg1, arg2, arg3, arg4, arg5))->start();
}

template <typename FunctionObject>
QFuture<typename FunctionObject::result_type> run(FunctionObject *functionObject)
{
    return (new Q_TYPENAME SelectStoredFunctorPointerCall0<Q_TYPENAME FunctionObject::result_type, FunctionObject>::type(functionObject))->start();
}
template <typename FunctionObject, typename Arg1>
QFuture<typename FunctionObject::result_type> run(FunctionObject *functionObject, const Arg1 &arg1)
{
    return (new Q_TYPENAME SelectStoredFunctorPointerCall1<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1>::type(functionObject, arg1))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2>
QFuture<typename FunctionObject::result_type> run(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2)
{
    return (new Q_TYPENAME SelectStoredFunctorPointerCall2<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2>::type(functionObject, arg1, arg2))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3>
QFuture<typename FunctionObject::result_type> run(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return (new Q_TYPENAME SelectStoredFunctorPointerCall3<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3>::type(functionObject, arg1, arg2, arg3))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
QFuture<typename FunctionObject::result_type> run(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    return (new Q_TYPENAME SelectStoredFunctorPointerCall4<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4>::type(functionObject, arg1, arg2, arg3, arg4))->start();
}
template <typename FunctionObject, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
QFuture<typename FunctionObject::result_type> run(FunctionObject *functionObject, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    return (new Q_TYPENAME SelectStoredFunctorPointerCall5<Q_TYPENAME FunctionObject::result_type, FunctionObject, Arg1, Arg2, Arg3, Arg4, Arg5>::type(functionObject, arg1, arg2, arg3, arg4, arg5))->start();
}

template <typename T, typename Class>
QFuture<T> run(const Class &object, T (Class::*fn)())
{
    return (new Q_TYPENAME SelectStoredMemberFunctionCall0<T, Class>::type(fn, object))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1), const Arg1 &arg1)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionCall1<T, Class, Param1, Arg1>::type(fn, object, arg1))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5))->start();
}

template <typename T, typename Class>
QFuture<T> run(const Class &object, T (Class::*fn)() const)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionCall0<T, Class>::type(fn, object))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1) const, const Arg1 &arg1)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionCall1<T, Class, Param1, Arg1>::type(fn, object, arg1))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2) const, const Arg1 &arg1, const Arg2 &arg2)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2, Param3) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
QFuture<T> run(const Class &object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5))->start();
}

template <typename T, typename Class>
QFuture<T> run(Class *object, T (Class::*fn)())
{
    return (new Q_TYPENAME SelectStoredMemberFunctionPointerCall0<T, Class>::type(fn, object))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1>
QFuture<T> run(Class *object, T (Class::*fn)(Param1), const Arg1 &arg1)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionPointerCall1<T, Class, Param1, Arg1>::type(fn, object, arg1))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
QFuture<T> run(Class *object, T (Class::*fn)(Param1, Param2), const Arg1 &arg1, const Arg2 &arg2)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionPointerCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
QFuture<T> run(Class *object, T (Class::*fn)(Param1, Param2, Param3), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionPointerCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
QFuture<T> run(Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionPointerCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
QFuture<T> run(Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5), const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    return (new Q_TYPENAME SelectStoredMemberFunctionPointerCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5))->start();
}

template <typename T, typename Class>
QFuture<T> run(const Class *object, T (Class::*fn)() const)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionPointerCall0<T, Class>::type(fn, object))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1>
QFuture<T> run(const Class *object, T (Class::*fn)(Param1) const, const Arg1 &arg1)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionPointerCall1<T, Class, Param1, Arg1>::type(fn, object, arg1))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
QFuture<T> run(const Class *object, T (Class::*fn)(Param1, Param2) const, const Arg1 &arg1, const Arg2 &arg2)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionPointerCall2<T, Class, Param1, Arg1, Param2, Arg2>::type(fn, object, arg1, arg2))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
QFuture<T> run(const Class *object, T (Class::*fn)(Param1, Param2, Param3) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionPointerCall3<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3>::type(fn, object, arg1, arg2, arg3))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
QFuture<T> run(const Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionPointerCall4<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4>::type(fn, object, arg1, arg2, arg3, arg4))->start();
}
template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
QFuture<T> run(const Class *object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5) const, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
{
    return (new Q_TYPENAME SelectStoredConstMemberFunctionPointerCall5<T, Class, Param1, Arg1, Param2, Arg2, Param3, Arg3, Param4, Arg4, Param5, Arg5>::type(fn, object, arg1, arg2, arg3, arg4, arg5))->start();
}

} //namespace QtConcurrent

QT_END_NAMESPACE

#endif // QT_NO_CONCURRENT

#endif
