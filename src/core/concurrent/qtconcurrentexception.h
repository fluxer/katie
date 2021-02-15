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

#ifndef QTCONCURRENT_EXCEPTION_H
#define QTCONCURRENT_EXCEPTION_H

#include <QtCore/qglobal.h>

#ifndef QT_NO_QFUTURE

#include <QtCore/qatomic.h>

#ifndef QT_NO_EXCEPTIONS
#  include <exception>
#endif

QT_BEGIN_NAMESPACE


namespace QtConcurrent
{

#ifndef QT_NO_EXCEPTIONS

class Q_CORE_EXPORT Exception : public std::exception
{
public:
    virtual void raise() const;
    virtual Exception *clone() const;
};

class Q_CORE_EXPORT UnhandledException : public Exception
{
public:
    void raise() const;
    Exception *clone() const;
};

namespace internal {

class Base;
class ExceptionHolder
{
public:
    ExceptionHolder(Exception *exception = 0);
    ExceptionHolder(const ExceptionHolder &other);
    void operator=(const ExceptionHolder &other);
    ~ExceptionHolder();
    Exception *exception() const;
    Base *base;
};

class Q_CORE_EXPORT ExceptionStore
{
public:
    void setException(const Exception &e);
    bool hasException() const;
    ExceptionHolder exception();
    void throwPossibleException();
    bool hasThrown() const;
    ExceptionHolder exceptionHolder;
};

} // namespace internal

#else // QT_NO_EXCEPTIONS

namespace internal {

class Q_CORE_EXPORT ExceptionStore
{
public:
    ExceptionStore() { }
    inline void throwPossibleException() const {}
};

} // namespace internal

#endif

} // namespace QtConcurrent

QT_END_NAMESPACE

#endif // QT_NO_CONCURRENT

#endif
