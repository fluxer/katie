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

#include "qplatformdefs.h"
#include "qstring.h"
#include "qlist.h"
#include "qcorecommon_p.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#ifndef QT_NO_EXCEPTIONS
#  include <exception>
#endif

#ifndef QT_NO_EXECINFO
#  include "qcoreapplication.h"
#  include <execinfo.h>
#  ifdef QT_HAVE_CXXABI_H
#    include <cxxabi.h>
#  endif
#endif

QT_BEGIN_NAMESPACE


/*!
    \class QFlag
    \brief The QFlag class is a helper data type for QFlags.

    It is equivalent to a plain \c int, except with respect to
    function overloading and type conversions. You should never need
    to use this class in your applications.

    \sa QFlags
*/

/*!
    \fn QFlag::QFlag(int value)

    Constructs a QFlag object that stores the given \a value.
*/

/*!
    \fn QFlag::operator int() const

    Returns the value stored by the QFlag object.
*/

/*!
    \class QFlags
    \brief The QFlags class provides a type-safe way of storing
    OR-combinations of enum values.


    \ingroup tools

    The QFlags<Enum> class is a template class, where Enum is an enum
    type. QFlags is used throughout Qt for storing combinations of
    enum values.

    The traditional C++ approach for storing OR-combinations of enum
    values is to use an \c int or \c uint variable. The inconvenience
    with this approach is that there's no type checking at all; any
    enum value can be OR'd with any other enum value and passed on to
    a function that takes an \c int or \c uint.

    Qt uses QFlags to provide type safety. For example, the
    Qt::Alignment type is simply a typedef for
    QFlags<Qt::AlignmentFlag>. QLabel::setAlignment() takes a
    Qt::Alignment parameter, which means that any combination of
    Qt::AlignmentFlag values,or 0, is legal:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 0

    If you try to pass a value from another enum or just a plain
    integer other than 0, the compiler will report an error. If you
    need to cast integer values to flags in a untyped fashion, you can
    use the explicit QFlags constructor as cast operator.

    If you want to use QFlags for your own enum types, use
    the Q_DECLARE_FLAGS() and Q_DECLARE_OPERATORS_FOR_FLAGS().

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 1

    You can then use the \c MyClass::Options type to store
    combinations of \c MyClass::Option values.

    \section1 Flags and the Meta-Object System

    The Q_DECLARE_FLAGS() macro does not expose the flags to the meta-object
    system, so they cannot be used by Qt Script or edited in Katie Designer.
    To make the flags available for these purposes, the Q_FLAGS() macro must
    be used:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp meta-object flags

    \section1 Naming Convention

    A sensible naming convention for enum types and associated QFlags
    types is to give a singular name to the enum type (e.g., \c
    Option) and a plural name to the QFlags type (e.g., \c Options).
    When a singular name is desired for the QFlags type (e.g., \c
    Alignment), you can use \c Flag as the suffix for the enum type
    (e.g., \c AlignmentFlag).

    \sa QFlag
*/

/*!
    \typedef QFlags::enum_type

    Typedef for the Enum template type.
*/

/*!
    \fn QFlags::QFlags(const QFlags &other)

    Constructs a copy of \a other.
*/

/*!
    \fn QFlags::QFlags(Enum flag)

    Constructs a QFlags object storing the given \a flag.
*/

/*!
    \fn QFlags::QFlags(Zero zero)

    Constructs a QFlags object with no flags set. \a zero must be a
    literal 0 value.
*/

/*!
    \fn QFlags::QFlags(QFlag value)

    Constructs a QFlags object initialized with the given integer \a
    value.

    The QFlag type is a helper type. By using it here instead of \c
    int, we effectively ensure that arbitrary enum values cannot be
    cast to a QFlags, whereas untyped enum values (i.e., \c int
    values) can.
*/

/*!
    \fn QFlags &QFlags::operator=(const QFlags &other)

    Assigns \a other to this object and returns a reference to this
    object.
*/

/*!
    \fn QFlags &QFlags::operator&=(int mask)

    Performs a bitwise AND operation with \a mask and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator&(), operator|=(), operator^=()
*/

/*!
    \fn QFlags &QFlags::operator&=(uint mask)

    \overload
*/

/*!
    \fn QFlags &QFlags::operator|=(QFlags other)

    Performs a bitwise OR operation with \a other and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator|(), operator&=(), operator^=()
*/

/*!
    \fn QFlags &QFlags::operator|=(Enum other)

    \overload
*/

/*!
    \fn QFlags &QFlags::operator^=(QFlags other)

    Performs a bitwise XOR operation with \a other and stores the
    result in this QFlags object. Returns a reference to this object.

    \sa operator^(), operator&=(), operator|=()
*/

/*!
    \fn QFlags &QFlags::operator^=(Enum other)

    \overload
*/

/*!
    \fn QFlags::operator int() const

    Returns the value stored in the QFlags object as an integer.
*/

/*!
    \fn QFlags QFlags::operator|(QFlags other) const

    Returns a QFlags object containing the result of the bitwise OR
    operation on this object and \a other.

    \sa operator|=(), operator^(), operator&(), operator~()
*/

/*!
    \fn QFlags QFlags::operator|(Enum other) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator^(QFlags other) const

    Returns a QFlags object containing the result of the bitwise XOR
    operation on this object and \a other.

    \sa operator^=(), operator&(), operator|(), operator~()
*/

/*!
    \fn QFlags QFlags::operator^(Enum other) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator&(int mask) const

    Returns a QFlags object containing the result of the bitwise AND
    operation on this object and \a mask.

    \sa operator&=(), operator|(), operator^(), operator~()
*/

/*!
    \fn QFlags QFlags::operator&(uint mask) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator&(Enum mask) const

    \overload
*/

/*!
    \fn QFlags QFlags::operator~() const

    Returns a QFlags object that contains the bitwise negation of
    this object.

    \sa operator&(), operator|(), operator^()
*/

/*!
    \fn bool QFlags::operator!() const

    Returns true if no flag is set (i.e., if the value stored by the
    QFlags object is 0); otherwise returns false.
*/

/*!
    \fn bool QFlags::testFlag(Enum flag) const
    \since 4.2

    Returns true if the \a flag is set, otherwise false.
*/

/*!
  \macro Q_DISABLE_COPY(Class)
  \relates QObject

  Disables the use of copy constructors and assignment operators
  for the given \a Class.

  Instances of subclasses of QObject should not be thought of as
  values that can be copied or assigned, but as unique identities.
  This means that when you create your own subclass of QObject
  (director or indirect), you should \e not give it a copy constructor
  or an assignment operator.  However, it may not enough to simply
  omit them from your class, because, if you mistakenly write some code
  that requires a copy constructor or an assignment operator (it's easy
  to do), your compiler will thoughtfully create it for you. You must
  do more.

  The curious user will have seen that the Qt classes derived
  from QObject typically include this macro in a private section:

  \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 43

  It declares a copy constructor and an assignment operator in the
  private section, so that if you use them by mistake, the compiler
  will report an error.

  \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 44

  But even this might not catch absolutely every case. You might be
  tempted to do something like this:

  \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 45

  First of all, don't do that. Most compilers will generate code that
  uses the copy constructor, so the privacy violation error will be
  reported, but your C++ compiler is not required to generate code for
  this statement in a specific way. It could generate code using
  \e{neither} the copy constructor \e{nor} the assignment operator we
  made private. In that case, no error would be reported, but your
  application would probably crash when you called a member function
  of \c{w}.
*/

/*!
    \macro Q_DECLARE_FLAGS(Flags, Enum)
    \relates QFlags

    The Q_DECLARE_FLAGS() macro expands to

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 2

    \a Enum is the name of an existing enum type, whereas \a Flags is
    the name of the QFlags<\e{Enum}> typedef.

    See the QFlags documentation for details.

    \sa Q_DECLARE_OPERATORS_FOR_FLAGS()
*/

/*!
    \macro Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)
    \relates QFlags

    The Q_DECLARE_OPERATORS_FOR_FLAGS() macro declares global \c
    operator|() functions for \a Flags, which is of type QFlags<T>.

    See the QFlags documentation for details.

    \sa Q_DECLARE_FLAGS()
*/

/*!
    \headerfile <QtGlobal>
    \title Global Qt Declarations
    \ingroup funclists

    \brief The <QtGlobal> header file includes the fundamental global
    declarations. It is included by most other Qt header files.

    The global declarations include \l{types}, \l{functions} and
    \l{macros}.

    The type definitions are partly convenience definitions for basic
    types (some of which guarantee certain bit-sizes on all platforms
    supported by Qt), partly types related to Qt message handling. The
    functions are related to generating messages, Qt version handling
    and comparing and adjusting object values. And finally, some of
    the declared macros enable programmers to add compiler or platform
    specific code to their applications, while others are convenience
    macros for larger operations.

    \section1 Types

    The header file declares several type definitions that guarantee a
    specified bit-size on all platforms supported by Qt for various
    basic types, for example \l qint8 which is a signed char
    guaranteed to be 8-bit on all platforms supported by Qt. The
    header file also declares the \l qlonglong type definition for \c
    {long long int } (\c __int64 on Windows).

    Several convenience type definitions are declared: \l qreal for \c
    double, \l uchar for \c unsigned char, \l uint for \c unsigned
    int, \l ulong for \c unsigned long and \l ushort for \c unsigned
    short.

    Finally, the QtMsgType definition identifies the various messages
    that can be generated and sent to a Qt message handler;
    QtMsgHandler is a type definition for a pointer to a function with
    the signature \c {void myMsgHandler(QtMsgType, const char *)}.

    \section1 Functions

    The <QtGlobal> header file contains several functions comparing
    and adjusting an object's value. These functions take a template
    type as argument: You can retrieve the absolute value of an object
    using the qAbs() function, and you can bound a given object's
    value by given minimum and maximum values using the qBound()
    function. You can retrieve the minimum and maximum of two given
    objects using qMin() and qMax() respectively. All these functions
    return a corresponding template type; the template types can be
    replaced by any other type.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 3

    <QtGlobal> also contains functions that generate messages from the
    given string argument: qCritical(), qDebug(), qFatal() and
    qWarning(). These functions call the message handler with the
    given message.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 4

    qInstallMsgHandler() function which installs the given
    QtMsgHandler, and the qVersion() function which returns the
    version number of Qt at run-time as a string.

    \section1 Macros

    The <QtGlobal> header file provides a range of macros (Q_CC_*)
    that are defined if the application is compiled using the
    specified platforms. For example, the Q_CC_GNU macro is defined
    if the application is compiled using GNU Compiler Collection.
    The header file also declares a range of macros (Q_OS_*) that
    are defined for the specified platforms. For example, Q_OS_LINUx
    which is defined for Linux.

    The purpose of these macros is to enable programmers to add
    compiler or platform specific code to their application.

    The remaining macros are convenience macros for larger operations:
    The QT_TRANSLATE_NOOP() and QT_TR_NOOP() macros provide the
    possibility of marking text for dynamic translation,
    i.e. translation without changing the stored source text. The
    Q_ASSERT() and Q_ASSERT_X() enables warning messages of various
    level of refinement. The Q_FOREACH() and foreach() macros
    implement Qt's foreach loop.

    The Q_INT64_C() and Q_UINT64_C() macros wrap signed and unsigned
    64-bit integer literals in a platform-independent way. The
    Q_CHECK_PTR() macro prints a warning containing the source code's
    file name and line number, saying that the program ran out of
    memory, if the pointer is 0. The qPrintable() macro represent an
    easy way of printing text.

    Finally, the QT_POINTER_SIZE macro expands to the size of a
    pointer in bytes, and the QT_VERSION and QT_VERSION_STR macros
    expand to a numeric value or a string, respectively, specifying
    Qt's version number, i.e the version the application is compiled
    against.

    \sa <QtAlgorithms>
*/

/*!
    \typedef qreal
    \relates <QtGlobal>

    Typedef for \c double on all platforms except for those using CPUs with
    ARM architectures.
    On ARM-based platforms, \c qreal is a typedef for \c float for performance
    reasons.
*/

/*! \typedef uchar
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned char}.
*/

/*! \typedef ushort
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned short}.
*/

/*! \typedef uint
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned int}.
*/

/*! \typedef ulong
    \relates <QtGlobal>

    Convenience typedef for \c{unsigned long}.
*/

/*! \typedef qint8
    \relates <QtGlobal>

    Typedef for \c{signed char}. This type is guaranteed to be 8-bit
    on all platforms supported by Qt.
*/

/*!
    \typedef quint8
    \relates <QtGlobal>

    Typedef for \c{unsigned char}. This type is guaranteed to
    be 8-bit on all platforms supported by Qt.
*/

/*! \typedef qint16
    \relates <QtGlobal>

    Typedef for \c{signed short}. This type is guaranteed to be
    16-bit on all platforms supported by Qt.
*/

/*!
    \typedef quint16
    \relates <QtGlobal>

    Typedef for \c{unsigned short}. This type is guaranteed to
    be 16-bit on all platforms supported by Qt.
*/

/*! \typedef qint32
    \relates <QtGlobal>

    Typedef for \c{signed int}. This type is guaranteed to be 32-bit
    on all platforms supported by Qt.
*/

/*!
    \typedef quint32
    \relates <QtGlobal>

    Typedef for \c{unsigned int}. This type is guaranteed to
    be 32-bit on all platforms supported by Qt.
*/

/*! \typedef qint64
    \relates <QtGlobal>

    Typedef for \c{long long int} (\c __int64 on Windows). This type
    is guaranteed to be 64-bit on all platforms supported by Qt.

    Literals of this type can be created using the Q_INT64_C() macro:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 5

    \sa Q_INT64_C(), quint64, qlonglong
*/

/*!
    \typedef quint64
    \relates <QtGlobal>

    Typedef for \c{unsigned long long int} (\c{unsigned __int64} on
    Windows). This type is guaranteed to be 64-bit on all platforms
    supported by Qt.

    Literals of this type can be created using the Q_UINT64_C()
    macro:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 6

    \sa Q_UINT64_C(), qint64, qulonglong
*/

/*!
    \typedef quintptr
    \relates <QtGlobal>

    Integral type for representing a pointers (useful for hashing,
    etc.).

    Typedef for either quint32 or quint64. This type is guaranteed to
    be the same size as a pointer on all platforms supported by Qt. On
    a system with 32-bit pointers, quintptr is a typedef for quint32;
    on a system with 64-bit pointers, quintptr is a typedef for
    quint64.

    Note that quintptr is unsigned. Use qptrdiff for signed values.

    \sa qptrdiff, quint32, quint64
*/

/*!
    \typedef qptrdiff
    \relates <QtGlobal>

    Integral type for representing pointer differences.

    Typedef for either qint32 or qint64. This type is guaranteed to be
    the same size as a pointer on all platforms supported by Qt. On a
    system with 32-bit pointers, quintptr is a typedef for quint32; on
    a system with 64-bit pointers, quintptr is a typedef for quint64.

    Note that qptrdiff is signed. Use quintptr for unsigned values.

    \sa quintptr, qint32, qint64
*/

/*!
    \typedef QtMsgHandler
    \relates <QtGlobal>

    This is a typedef for a pointer to a function with the following
    signature:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 7

    \sa QtMsgType, qInstallMsgHandler()
*/

/*!
    \enum QtMsgType
    \relates <QtGlobal>

    This enum describes the messages that can be sent to a message
    handler (QtMsgHandler). You can use the enum to identify and
    associate the various message types with the appropriate
    actions.

    \value QtDebugMsg
           A message generated by the qDebug() function.
    \value QtWarningMsg
           A message generated by the qWarning() function.
    \value QtCriticalMsg
           A message generated by the qCritical() function.
    \value QtFatalMsg
           A message generated by the qFatal() function.

    \sa QtMsgHandler, qInstallMsgHandler()
*/

/*! \macro qint64 Q_INT64_C(literal)
    \relates <QtGlobal>

    Wraps the signed 64-bit integer \a literal in a
    platform-independent way.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 8

    \sa qint64, Q_UINT64_C()
*/

/*! \macro quint64 Q_UINT64_C(literal)
    \relates <QtGlobal>

    Wraps the unsigned 64-bit integer \a literal in a
    platform-independent way.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 9

    \sa quint64, Q_INT64_C()
*/

/*! \typedef qlonglong
    \relates <QtGlobal>

    Typedef for \c{long long int} (\c __int64 on Windows). This is
    the same as \l qint64.

    \sa qulonglong, qint64
*/

/*!
    \typedef qulonglong
    \relates <QtGlobal>

    Typedef for \c{unsigned long long int} (\c{unsigned __int64} on
    Windows). This is the same as \l quint64.

    \sa quint64, qlonglong
*/

/*! \fn const T &qAbs(const T &value)
    \relates <QtGlobal>

    Compares \a value to the 0 of type T and returns the absolute
    value. Thus if T is \e {double}, then \a value is compared to
    \e{(double) 0}.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 10
*/

/*! \fn const T &qMin(const T &value1, const T &value2)
    \relates <QtGlobal>

    Returns the minimum of \a value1 and \a value2.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 13

    \sa qMax(), qBound()
*/

/*! \fn const T &qMax(const T &value1, const T &value2)
    \relates <QtGlobal>

    Returns the maximum of \a value1 and \a value2.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 14

    \sa qMin(), qBound()
*/

/*! \fn const T &qBound(const T &min, const T &value, const T &max)
    \relates <QtGlobal>

    Returns \a value bounded by \a min and \a max. This is equivalent
    to qMax(\a min, qMin(\a value, \a max)).

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 15

    \sa qMin(), qMax()
*/

/*!
    \macro QT_VERSION_CHECK
    \relates <QtGlobal>

    Turns the major, minor and patch numbers of a version into an
    integer, 0xMMNNPP (MM = major, NN = minor, PP = patch). This can
    be compared with another similarly processed version id.

    \sa QT_VERSION
*/

/*!
    \macro QT_VERSION
    \relates <QtGlobal>

    This macro expands a numeric value of the form 0xMMNNPP (MM =
    major, NN = minor, PP = patch) that specifies Qt's version
    number. For example, if you compile your application against Qt
    4.1.2, the QT_VERSION macro will expand to 0x040102.

    You can use QT_VERSION to use the latest Qt features where
    available.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 16

    \sa QT_VERSION_STR, qVersion()
*/

/*!
    \macro QT_VERSION_STR
    \relates <QtGlobal>

    This macro expands to a string that specifies Qt's version number
    (for example, "4.1.2"). This is the version against which the
    application is compiled.

    \sa qVersion(), QT_VERSION
*/

/*!
    \relates <QtGlobal>

    Returns the version number of Qt at run-time as a string (for
    example, "4.1.2"). This may be a different version than the
    version the application was compiled against.

    \sa QT_VERSION_STR
*/

const char *qVersion()
{
    return QT_VERSION_STR;
}

/*****************************************************************************
  System detection routines
 *****************************************************************************/

/*!
    \macro Q_WS_X11
    \relates <QtGlobal>

    Defined on X11.
*/

/*!
    \macro Q_OS_LINUX
    \relates <QtGlobal>

    Defined on Linux.
*/

/*!
    \macro Q_OS_FREEBSD
    \relates <QtGlobal>

    Defined on FreeBSD.
*/

/*!
    \macro Q_OS_NETBSD
    \relates <QtGlobal>

    Defined on NetBSD.
*/

/*!
    \macro Q_OS_OPENBSD
    \relates <QtGlobal>

    Defined on OpenBSD.
*/

/*!
    \macro Q_OS_DRAGONFLY
    \relates <QtGlobal>

    Defined on DragonFly BSD.
*/

/*!
    \macro Q_OS_HURD
    \relates <QtGlobal>

    Defined on GNU Hurd.
*/

/*!
    \macro Q_OS_SOLARIS
    \relates <QtGlobal>

    Defined on Sun Solaris.
*/

/*!
    \macro Q_OS_UNIX
    \relates <QtGlobal>

    Defined on Any UNIX BSD/SYSV system.
*/

/*!
    \macro Q_CC_GNU
    \relates <QtGlobal>

    Defined if the application is compiled using GNU C++.
*/

/*!
    \macro Q_CC_CLANG
    \relates <QtGlobal>

    Defined if the application is compiled using C++ front-end for the LLVM
    compiler.
*/


/*!
    \macro void Q_ASSERT(bool test)
    \relates <QtGlobal>

    Prints a warning message containing the source code file name and
    line number if \a test is false.

    Q_ASSERT() is useful for testing pre- and post-conditions
    during development. It does nothing if \c QT_NO_DEBUG was defined
    during compilation.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 17

    If \c b is zero, the Q_ASSERT statement will output the following
    message using the qFatal() function:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 18

    \sa Q_ASSERT_X(), qFatal(), {Debugging Techniques}
*/

/*!
    \macro void Q_ASSERT_X(bool test, const char *where, const char *what)
    \relates <QtGlobal>

    Prints the message \a what together with the location \a where,
    the source file name and line number if \a test is false.

    Q_ASSERT_X is useful for testing pre- and post-conditions during
    development. It does nothing if \c QT_NO_DEBUG was defined during
    compilation.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 19

    If \c b is zero, the Q_ASSERT_X statement will output the following
    message using the qFatal() function:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 20

    \sa Q_ASSERT(), qFatal(), {Debugging Techniques}
*/

/*!
    \macro void Q_CHECK_PTR(void *pointer)
    \relates <QtGlobal>

    If \a pointer is 0, prints a warning message containing the source
    code's file name and line number, saying that the program ran out
    of memory.

    Q_CHECK_PTR does nothing if \c QT_NO_DEBUG was defined during
    compilation.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 21

    \sa qWarning(), {Debugging Techniques}
*/

/*!
    \macro const char* Q_FUNC_INFO()
    \relates <QtGlobal>

    Expands to a string that describe the function the macro resides in. How this string looks
    more specifically is compiler dependent. With GNU GCC it is typically the function signature,
    while with other compilers it might be the line and column number.

    Q_FUNC_INFO can be conveniently used with qDebug(). For example, this function:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 22

    when instantiated with the integer type, will with the GCC compiler produce:

    \tt{const TInputType& myMin(const TInputType&, const TInputType&) [with TInputType = int] was called with value1: 3 value2: 4}

    If this macro is used outside a function, the behavior is undefined.
 */

#ifndef QT_NO_EXECINFO
static void qt_print_backtrace()
{
    ::setvbuf(stderr, nullptr, _IONBF, 0);
    QSTACKARRAY(void*, buffer, 256);
    int  nptrs = backtrace(buffer, sizeof(buffer));
    char **strings = backtrace_symbols(buffer, nptrs);
    if (!strings) {
        ::fprintf(stderr, "qt_print_backtrace: unable to get backtrace\n");
        return;
    }

    for (int i = 0; i < nptrs; i++) {
#ifdef QT_HAVE_CXXABI_H
        int status;
        char* demangled = abi::__cxa_demangle(strings[i], nullptr, nullptr, &status);
        if (status == 0) {
            ::fprintf(stderr, " %s\n", demangled);
            ::free(demangled);
            continue;
        }
#endif
        ::fprintf(stderr, " %s\n", strings[i]);
    }

    ::free(strings);
}

typedef void (*QCrashHandler)(int);

static bool qt_set_crash_handler(QCrashHandler handler) {
    sigset_t mask;
    if (::sigemptyset(&mask) != 0) {
        ::fprintf(stderr, "sigaddset error: %s\n", ::strerror(errno));
        return false;
    }

#define HANDLE_SIGNAL(x) \
    ::signal(x, handler); \
    if (::sigaddset(&mask, x) != 0) { \
        ::fprintf(stderr, "sigaddset error: %s\n", ::strerror(errno)); \
        return false; \
    }

#ifdef SIGSEGV
    HANDLE_SIGNAL(SIGSEGV)
#endif
#ifdef SIGBUS
    HANDLE_SIGNAL(SIGBUS)
#endif
#ifdef SIGFPE
    HANDLE_SIGNAL(SIGFPE)
#endif
#ifdef SIGILL
    HANDLE_SIGNAL(SIGILL)
#endif
#ifdef SIGABRT
    HANDLE_SIGNAL(SIGABRT)
#endif

#undef HANDLE_SIGNAL

    if (::sigprocmask(SIG_UNBLOCK, &mask, nullptr) != 0) {
        ::fprintf(stderr, "sigprocmask error: %s\n", ::strerror(errno));
        return false;
    }

    return true;
}

static void qt_crash_handler(int sig) {
    const QByteArray name = QCoreApplication::applicationName().toLatin1();
    const std::string pid = std::to_string(::getpid());

    if (name.isEmpty()) {
        ::fprintf(stderr, "PID %s crashed\n", pid.c_str());
    } else {
        ::fprintf(stderr, "%s with PID %s crashed\n", name.constData(), pid.c_str());
    }

    qt_set_crash_handler(SIG_DFL);

    qt_print_backtrace();

    ::raise(sig);
}

int qt_install_crash_handler()
{
    qt_set_crash_handler(qt_crash_handler);
    return 0;
}

Q_CONSTRUCTOR_FUNCTION(qt_install_crash_handler);
#endif

static const bool fatalwarnings = !qgetenv("QT_FATAL_WARNINGS").isNull();
#ifndef QT_NO_EXECINFO
static const bool tracewarnings = !qgetenv("QT_TRACE_WARNINGS").isNull();
#endif

/*
  The Q_ASSERT macro calls this function when the test fails.
*/
void qt_assert(const char *assertion, const char *file, int line)
{
#ifndef QT_NO_EXECINFO
    // don't print backtrace twice if trace will be printed in qt_message_output()
    if (!fatalwarnings)
        qt_print_backtrace();
#endif
    qFatal("ASSERT: \"%s\" in file %s, line %d", assertion, file, line);
}

/*
  The Q_ASSERT_X macro calls this function when the test fails.
*/
void qt_assert_x(const char *where, const char *what, const char *file, int line)
{
#ifndef QT_NO_EXECINFO
    // don't print backtrace twice if trace will be printed in qt_message_output()
    if (!fatalwarnings)
        qt_print_backtrace();
#endif
    qFatal("ASSERT failure in %s: \"%s\", file %s, line %d", where, what, file, line);
}

/*
  The Q_CHECK_PTR macro calls this function if an allocation check
  fails.
*/
void qt_check_pointer(const char *file, int line)
{
    qFatal("In file %s, line %d: Out of memory", file, line);
}

static QtMsgHandler handler = 0;                // pointer to debug handler

/*!
    \fn QtMsgHandler qInstallMsgHandler(QtMsgHandler handler)
    \relates <QtGlobal>

    Installs a Qt message \a handler which has been defined
    previously. Returns a pointer to the previous message handler
    (which may be 0).

    The message handler is a function that prints out debug messages,
    warnings, critical and fatal error messages. The Qt library (debug
    mode) contains hundreds of warning messages that are printed
    when internal errors (usually invalid function arguments)
    occur. Qt built in release mode also contains such warnings unless
    QT_NO_WARNING_OUTPUT and/or QT_NO_DEBUG_OUTPUT have been set during
    compilation. If you implement your own message handler, you get total
    control of these messages.

    The default message handler prints the message to the standard
    output under X11 or to the debugger under Windows. If it is a
    fatal message, the application aborts immediately.

    Only one message handler can be defined, since this is usually
    done on an application-wide basis to control debug output.

    To restore the message handler, call \c qInstallMsgHandler(0).

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 23

    \sa qDebug(), qWarning(), qCritical(), qFatal(), QtMsgType,
    {Debugging Techniques}
*/

QtMsgHandler qInstallMsgHandler(QtMsgHandler h)
{
    QtMsgHandler old = handler;
    handler = h;
    return old;
}

/*!
    \internal
*/
void qt_message_output(QtMsgType msgType, const char *buf)
{
    if (handler) {
        (*handler)(msgType, buf);
    } else {
        ::fprintf(stderr, "%s\n", buf);
        ::fflush(stderr);
    }

#ifndef QT_NO_EXECINFO
    if (msgType == QtWarningMsg && tracewarnings)
        qt_print_backtrace();
#endif
    if (msgType == QtFatalMsg || (msgType == QtWarningMsg && fatalwarnings))
        abort(); // trap; generates core dump
}

/*!
    \internal
*/
static void qt_message(QtMsgType msgType, const char *format, va_list ap)
{
    if (Q_LIKELY(format)) {
        QSTACKARRAY(char, messagebuf, 1024);
        ::vsnprintf(messagebuf, sizeof(messagebuf), format, ap);
        qt_message_output(msgType, messagebuf);
    }
}

#undef qDebug
/*!
    \relates <QtGlobal>

    Calls the message handler with the debug message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the console, if it is a
    console application; otherwise, it is sent to the debugger. This
    function does nothing if \c QT_NO_DEBUG_OUTPUT was defined
    during compilation.

    If you pass the function a format string and a list of arguments,
    it works in similar way to the C printf() function. The format
    should be a Latin-1 string.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 24

    If you include \c <QtDebug>, a more convenient syntax is also
    available:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 25

    With this syntax, the function returns a QDebug object that is
    configured to use the QtDebugMsg message type. It automatically
    puts a single space between each item, and outputs a newline at
    the end. It supports many C++ and Qt types.

    To suppress the output at run-time, install your own message handler
    with qInstallMsgHandler().

    \sa qWarning(), qCritical(), qFatal(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qDebug(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg); // use variable arg list
    qt_message(QtDebugMsg, msg, ap);
    va_end(ap);
}

#undef qWarning
/*!
    \relates <QtGlobal>

    Calls the message handler with the warning message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the debugger. This
    function does nothing if \c QT_NO_WARNING_OUTPUT was defined
    during compilation; it exits if the environment variable \c
    QT_FATAL_WARNINGS is defined.

    This function takes a format string and a list of arguments,
    similar to the C printf() function. The format should be a Latin-1
    string.

    Example:
    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 26

    If you include <QtDebug>, a more convenient syntax is
    also available:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 27

    This syntax inserts a space between each item, and
    appends a newline at the end.

    To suppress the output at runtime, install your own message handler
    with qInstallMsgHandler().

    \sa qDebug(), qCritical(), qFatal(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qWarning(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg); // use variable arg list
    qt_message(QtWarningMsg, msg, ap);
    va_end(ap);
}

/*!
    \relates <QtGlobal>

    Calls the message handler with the critical message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the debugger.

    This function takes a format string and a list of arguments,
    similar to the C printf() function. The format should be a Latin-1
    string.

    Example:
    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 28

    If you include <QtDebug>, a more convenient syntax is
    also available:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 29

    A space is inserted between the items, and a newline is
    appended at the end.

    To suppress the output at runtime, install your own message handler
    with qInstallMsgHandler().

    \sa qDebug(), qWarning(), qFatal(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qCritical(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg); // use variable arg list
    qt_message(QtCriticalMsg, msg, ap);
    va_end(ap);
}

/*!
    \relates <QtGlobal>

    Calls the message handler with the fatal message \a msg. If no
    message handler has been installed, the message is printed to
    stderr. Under Windows, the message is sent to the debugger.

    If you are using the \bold{default message handler} this function will
    abort on Unix systems to create a core dump. On Windows, for debug builds,
    this function will report a _CRT_ERROR enabling you to connect a debugger
    to the application.

    This function takes a format string and a list of arguments,
    similar to the C printf() function.

    Example:
    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 30

    To suppress the output at runtime, install your own message handler
    with qInstallMsgHandler().

    \sa qDebug(), qCritical(), qWarning(), qInstallMsgHandler(),
        {Debugging Techniques}
*/
void qFatal(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg); // use variable arg list
    qt_message(QtFatalMsg, msg, ap);
    va_end(ap);
}

// getenv is declared as deprecated in VS2005. This function
// makes use of the new secure getenv function.
/*!
    \relates <QtGlobal>

    Returns the value of the environment variable with name \a
    varName. To get the variable string, use QByteArray::constData().

    \note qgetenv() was introduced because getenv() from the standard
    C library was deprecated in VC2005 (and later versions). qgetenv()
    uses the new replacement function in VC, and calls the standard C
    library's implementation on all other platforms.

    \sa qputenv()
*/
QByteArray qgetenv(const char *varName)
{
    return QByteArray(::getenv(varName));
}

/*!
    \relates <QtGlobal>

    This function sets the \a value of the environment variable named
    \a varName. It will create the variable if it does not exist. It
    returns 0 if the variable could not be set.

    \note qputenv() was introduced because putenv() from the standard
    C library was deprecated in VC2005 (and later versions). qputenv()
    uses the replacement function in VC, and calls the standard C
    library's implementation on all other platforms.

    \sa qgetenv()
*/
bool qputenv(const char *varName, const QByteArray& value)
{
    QByteArray buffer(varName);
    buffer += '=';
    buffer += value;
    char* envVar = qstrdup(buffer.constData());
    int result = putenv(envVar);
    if (result != 0) // error. we have to delete the string.
        delete[] envVar;
    return result == 0;
}

/*!
    \relates <QtGlobal>
    \since 4.2

    Thread-safe version of the standard C++ \c srand() function.

    Sets the argument \a seed to be used to generate a new random number sequence of
    pseudo random integers to be returned by qrand().

    The sequence of random numbers generated is deterministic per thread. For example,
    if two threads call qsrand(1) and subsequently calls qrand(), the threads will get
    the same random number sequence.

    \sa qrand()
*/
void qsrand(uint seed)
{
    std::srand(seed);
}

/*!
    \relates <QtGlobal>
    \since 4.2

    Thread-safe version of the standard C++ \c rand() function.

    Returns a value between 0 and \c RAND_MAX (defined in \c <cstdlib> and
    \c <stdlib.h>), the next number in the current sequence of pseudo-random
    integers.

    Use \c qsrand() to initialize the pseudo-random number generator with
    a seed value.

    \sa qsrand()
*/
int qrand()
{
#ifdef QT_HAVE_ARC4RANDOM_UNIFORM
    return ::arc4random_uniform(RAND_MAX);
#else
    // Seed the PRNG once per thread with a combination of current time and its address
    thread_local time_t almostrandom = 0;
    if (!almostrandom) {
        ::time(&almostrandom);
        std::srand(almostrandom + std::intptr_t(&almostrandom));
    }
    return std::rand();
#endif
}

/*!
    \macro forever
    \relates <QtGlobal>

    This macro is provided for convenience for writing infinite
    loops.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 31

    It is equivalent to \c{for (;;)}.

    If you're worried about namespace pollution, you can disable this
    macro by adding the following line to your \c .pro file:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 32

    \sa Q_FOREVER
*/

/*!
    \macro Q_FOREVER
    \relates <QtGlobal>

    Same as \l{forever}.

    This macro is available even when \c no_keywords is specified
    using the \c .pro file's \c CONFIG variable.

    \sa foreach()
*/

/*!
    \macro foreach(variable, container)
    \relates <QtGlobal>

    This macro is used to implement Qt's \c foreach loop. The \a
    variable parameter is a variable name or variable definition; the
    \a container parameter is a Qt container whose value type
    corresponds to the type of the variable. See \l{The foreach
    Keyword} for details.

    If you're worried about namespace pollution, you can disable this
    macro by adding the following line to your \c .pro file:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 33

    \sa Q_FOREACH()
*/

/*!
    \macro Q_FOREACH(variable, container)
    \relates <QtGlobal>

    Same as foreach(\a variable, \a container).

    This macro is available even when \c no_keywords is specified
    using the \c .pro file's \c CONFIG variable.

    \sa foreach()
*/

/*!
    \macro QT_TR_NOOP(sourceText)
    \relates <QtGlobal>

    Marks the string literal \a sourceText for dynamic translation in
    the current context (class), i.e the stored \a sourceText will not
    be altered.

    The macro expands to \a sourceText.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 34

    The macro QT_TR_NOOP_UTF8() is identical except that it notes
    that the source string is encoded in UTF-8. Corresponding variants
    exist in the QT_TRANSLATE_NOOP() family of macros, too. Note that
    using these macros is not required if UTF-8 codec is used for
    translations.

    \sa QT_TRANSLATE_NOOP(), {Internationalization with Qt}
*/

/*!
    \macro QT_TRANSLATE_NOOP(context, sourceText)
    \relates <QtGlobal>

    Marks the string literal \a sourceText for dynamic translation in
    the given \a context; i.e, the stored \a sourceText will not be
    altered. The \a context is typically a class and also needs to
    be specified as string literal.

    The macro expands to \a sourceText.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 35

    \sa QT_TR_NOOP(), {Internationalization with Qt}
*/

/*!
    \macro Q_LIKELY(expr)
    \relates <QtGlobal>
    \since 4.8

    \brief Hints to the compiler that the enclosed condition, \a expr, is
    likely to evaluate to \c true.

    Use of this macro can help the compiler to optimize the code.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp qlikely

    \sa Q_UNLIKELY()
*/

/*!
    \macro Q_UNLIKELY(expr)
    \relates <QtGlobal>
    \since 4.8

    \brief Hints to the compiler that the enclosed condition, \a expr, is
    likely to evaluate to \c false.

    Use of this macro can help the compiler to optimize the code.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp qunlikely

    \sa Q_LIKELY()
*/

/*!
    \macro QT_POINTER_SIZE
    \relates <QtGlobal>

    Expands to the size of a pointer in bytes (4 or 8). This is
    equivalent to \c sizeof(void *) but can be used in a preprocessor
    directive.
*/

/*!
    \macro const char *qPrintable(const QString &str)
    \relates <QtGlobal>

    Returns \a str as a \c{const char *}. This is equivalent to
    \a{str}.toLocal8Bit().constData().

    The char pointer will be invalid after the statement in which
    qPrintable() is used. This is because the array returned by
    toLocal8Bit() will fall out of scope.

    Example:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 37


    \sa qDebug(), qWarning(), qCritical(), qFatal()
*/

/*!
    \macro Q_DECLARE_TYPEINFO(Type, Flags)
    \relates <QtGlobal>

    You can use this macro to specify information about a custom type
    \a Type. With accurate type information, Qt's \l{Container Classes}
    {generic containers} can choose appropriate storage methods and
    algorithms.

    \a Flags can be one of the following:

    \list
    \o \c Q_PRIMITIVE_TYPE specifies that \a Type is a POD (plain old
       data) type with no constructor or destructor.
    \o \c Q_MOVABLE_TYPE specifies that \a Type has a constructor
       and/or a destructor but can be moved in memory using \c
       memcpy().
    \o \c Q_COMPLEX_TYPE (the default) specifies that \a Type has
       constructors and/or a destructor and that it may not be moved
       in memory.
    \endlist

    Example of a "primitive" type:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 38

    Example of a movable type:

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 39
*/

/*!
    \macro Q_UNUSED(name)
    \relates <QtGlobal>

    Indicates to the compiler that the parameter with the specified
    \a name is not used in the body of a function. This can be used to
    suppress compiler warnings while allowing functions to be defined
    with meaningful parameter names in their signatures.
*/

Q_GLOBAL_STATIC(QList<qInternalCallback>, qGlobalCallbacks)

void QInternal::registerCallback(qInternalCallback callback)
{
    qGlobalCallbacks()->append(callback);
}

void QInternal::unregisterCallback(qInternalCallback callback)
{
    qGlobalCallbacks()->removeAll(callback);
}

bool QInternal::activateCallbacks(void **parameters)
{
    QList<qInternalCallback> *callbacks = qGlobalCallbacks();
    bool ret = false;
    for (int i = 0; i < callbacks->size(); i++) {
        ret |= (callbacks->at(i))(parameters);
    }
    return ret;
}

/*!
    \macro Q_BYTE_ORDER
    \relates <QtGlobal>

    This macro can be used to determine the byte order your system
    uses for storing data in memory. i.e., whether your system is
    little-endian or big-endian. It is set by Qt to one of the macros
    Q_LITTLE_ENDIAN or Q_BIG_ENDIAN. You normally won't need to worry
    about endian-ness, but you might, for example if you need to know
    which byte of an integer or UTF-16 character is stored in the
    lowest address. Endian-ness is important in networking, where
    computers with different values for Q_BYTE_ORDER must pass data
    back and forth.

    Use this macro as in the following examples.

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 40

    \sa Q_BIG_ENDIAN, Q_LITTLE_ENDIAN
*/

/*!
    \macro Q_LITTLE_ENDIAN
    \relates <QtGlobal>

    This macro represents a value you can compare to the macro
    Q_BYTE_ORDER to determine the endian-ness of your system.  In a
    little-endian system, the least significant byte is stored at the
    lowest address. The other bytes follow in increasing order of
    significance.

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 41

    \sa Q_BYTE_ORDER, Q_BIG_ENDIAN
*/

/*!
    \macro Q_BIG_ENDIAN
    \relates <QtGlobal>

    This macro represents a value you can compare to the macro
    Q_BYTE_ORDER to determine the endian-ness of your system.  In a
    big-endian system, the most significant byte is stored at the
    lowest address. The other bytes follow in decreasing order of
    significance.

    \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 42

    \sa Q_BYTE_ORDER, Q_LITTLE_ENDIAN
*/

/*!
    \macro Q_GLOBAL_STATIC(type, name)
    \internal

    Declares a global static variable with the given \a type and \a name.

    Use this macro to instantiate an object in a thread-safe way, creating
    a global pointer that can be used to refer to it.
*/

/*!
    \macro Q_GLOBAL_STATIC_WITH_ARGS(type, name, arguments)
    \internal

    Declares a global static variable with the specified \a type and \a name.

    Use this macro to instantiate an object using the \a arguments specified
    in a thread-safe way, creating a global pointer that can be used to refer
    to it.
*/

/*!
    \macro Q_GLOBAL_STATIC_WITH_INITIALIZER(type, name, initializer)
    \internal

    Declares a global static variable with the specified \a type and \a name.

    Use this macro to instantiate an object using the \a initializer specified
    in a thread-safe way, creating a global pointer that can be used to refer
    to it.

    \warning This macro is subject to a race condition that can cause the object
    to be constructed twice. However, if this occurs, the second instance will
    be immediately deleted.

    See also
    \l{http://www.aristeia.com/publications.html}{"C++ and the perils of Double-Checked Locking"}
    by Scott Meyers and Andrei Alexandrescu.
*/

/*!
    \macro QT_NAMESPACE
    \internal

    If this macro is defined to \c ns all Qt classes are put in a namespace
    called \c ns. Also, moc will output code putting metaobjects etc.
    into namespace \c ns.

    \sa QT_BEGIN_NAMESPACE, QT_END_NAMESPACE,
    QT_PREPEND_NAMESPACE, QT_BEGIN_INCLUDE_NAMESPACE, QT_END_INCLUDE_NAMESPACE
*/

/*!
    \macro QT_PREPEND_NAMESPACE(identifier)
    \internal

    This macro qualifies \a identifier with the full namespace.
    It expands to \c{::QT_NAMESPACE::identifier} if \c QT_NAMESPACE is defined
    and only \a identifier otherwise.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_USE_NAMESPACE
    \internal

    This macro expands to using QT_NAMESPACE if QT_NAMESPACE is defined
    and nothing otherwise.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_BEGIN_NAMESPACE
    \internal

    This macro expands to

    \snippet snippets/code/src_corelib_global_qglobal.cpp begin namespace macro

    if \c QT_NAMESPACE is defined and nothing otherwise. If should always
    appear in the file-level scope and be followed by \c QT_END_NAMESPACE
    at the same logical level with respect to preprocessor conditionals
    in the same file.

    As a rule of thumb, \c QT_BEGIN_NAMESPACE should appear in all Katie header
    and source files after the last \c{#include} line and before the first
    declaration.

    If that rule can't be followed because, e.g., \c{#include} lines and
    declarations are wildly mixed, place \c QT_BEGIN_NAMESPACE before
    the first declaration and wrap the \c{#include} lines in
    \c QT_BEGIN_INCLUDE_NAMESPACE and \c QT_END_INCLUDE_NAMESPACE.

    When using the \c QT_NAMESPACE feature in user code
    (e.g., when building plugins statically linked to Qt) where
    the user code is not intended to go into the \c QT_NAMESPACE
    namespace, all forward declarations of Qt classes need to
    be wrapped in \c QT_BEGIN_NAMESPACE and \c QT_END_NAMESPACE.
    After that, a \c QT_USE_NAMESPACE should follow.
    No further changes should be needed.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_END_NAMESPACE
    \internal

    This macro expands to

    \snippet snippets/code/src_corelib_global_qglobal.cpp end namespace macro

    if \c QT_NAMESPACE is defined and nothing otherwise. It is used to cancel
    the effect of \c QT_BEGIN_NAMESPACE.

    If a source file ends with a \c{#include} directive that includes a moc file,
    \c QT_END_NAMESPACE should be placed before that \c{#include}.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_BEGIN_INCLUDE_NAMESPACE
    \internal

    This macro is equivalent to \c QT_END_NAMESPACE.
    It only serves as syntactic sugar and is intended
    to be used before #include lines within a
    \c QT_BEGIN_NAMESPACE ... \c QT_END_NAMESPACE block.

    \sa QT_NAMESPACE
*/

/*!
    \macro QT_END_INCLUDE_NAMESPACE
    \internal

    This macro is equivalent to \c QT_BEGIN_NAMESPACE.
    It only serves as syntactic sugar and is intended
    to be used after #include lines within a
    \c QT_BEGIN_NAMESPACE ... \c QT_END_NAMESPACE block.

    \sa QT_NAMESPACE
*/

/*!
 \fn bool qFuzzyCompare(double p1, double p2)
 \relates <QtGlobal>
 \since 4.4
 \threadsafe

 Compares the floating point value \a p1 and \a p2 and
 returns \c true if they are considered equal, otherwise \c false.

 Note that comparing values where either \a p1 or \a p2 is 0.0 will not work.
 The solution to this is to compare against values greater than or equal to 1.0.

 \snippet doc/src/snippets/code/src_corelib_global_qglobal.cpp 46

 The two numbers are compared in a relative way, where the
 exactness is stronger the smaller the numbers are.
 */

/*!
 \fn bool qFuzzyCompare(float p1, float p2)
 \relates <QtGlobal>
 \since 4.4
 \threadsafe

 Compares the floating point value \a p1 and \a p2 and
 returns \c true if they are considered equal, otherwise \c false.

 The two numbers are compared in a relative way, where the
 exactness is stronger the smaller the numbers are.
 */

/*!
    \macro QT_REQUIRE_VERSION(int argc, char **argv, const char *version)
    \relates <QtGlobal>

    This macro can be used to ensure that the application is run
    against a recent enough version of Qt. This is especially useful
    if your application depends on a specific bug fix introduced in a
    bug-fix release (e.g., 4.0.2).

    The \a argc and \a argv parameters are the \c main() function's
    \c argc and \c argv parameters. The \a version parameter is a
    string literal that specifies which version of Qt the application
    requires (e.g., "4.0.2").

    Example:

    \snippet doc/src/snippets/code/src_gui_dialogs_qmessagebox.cpp 4
*/

/*!
    \macro Q_DECL_EXPORT
    \relates <QtGlobal>

    This macro marks a symbol for shared library export (see
     \l{sharedlibrary.html}{Creating Shared Libraries}).

    \sa Q_DECL_IMPORT
*/

/*!
    \macro Q_DECL_IMPORT
    \relates <QtGlobal>

    This macro declares a symbol to be an import from a shared library (see
    \l{sharedlibrary.html}{Creating Shared Libraries}).

    \sa Q_DECL_EXPORT
*/

// the displayMode value is according to the what are blocks in the piecetable, not
// what the w3c defines. same as those in qtexthtmlparser.cpp
static const char* elementsTbl[]= {
    "a\0",
    "address\0",
    "b\0",
    "big\0",
    "blockquote\0",
    "body\0",
    "br\0",
    "caption\0",
    "center\0",
    "cite\0",
    "code\0",
    "dd\0",
    "dfn\0",
    "div\0",
    "dl\0",
    "dt\0",
    "em\0",
    "font\0",
    "h1\0",
    "h2\0",
    "h3\0",
    "h4\0",
    "h5\0",
    "h6\0",
    "head\0",
    "hr\0",
    "html\0",
    "i\0",
    "img\0",
    "kbd\0",
    "li\0",
    "link\0",
    "meta\0",
    "nobr\0",
    "ol\0",
    "p\0",
    "pre\0",
    "qt\0",
    "s\0",
    "samp\0",
    "script\0",
    "small\0",
    "span\0",
    "strong\0",
    "style\0",
    "sub\0",
    "sup\0",
    "table\0",
    "tbody\0",
    "td\0",
    "tfoot\0",
    "th\0",
    "thead\0",
    "title\0",
    "tr\0",
    "tt\0",
    "u\0",
    "ul\0",
    "var\0",
};
static const qint16 elementsTblSize = 59;

/*!
    Returns true if the string \a text is likely to be rich text;
    otherwise returns false.

    This function uses a fast and therefore simple heuristic. It
    mainly checks whether there is something that looks like a tag
    before the first line break. Although the result may be correct
    for common cases, there is no guarantee.
*/
bool Qt::mightBeRichText(const QString& text)
{
    if (text.isEmpty())
        return false;
    int start = 0;

    while (start < text.length() && text.at(start).isSpace())
        ++start;

    // skip a leading <?xml ... ?> as for example with xhtml
    if (text.mid(start, 5) == QLatin1String("<?xml")) {
        while (start < text.length()) {
            if (text.at(start) == QLatin1Char('?')
                && start + 2 < text.length()
                && text.at(start + 1) == QLatin1Char('>')) {
                start += 2;
                break;
            }
            ++start;
        }

        while (start < text.length() && text.at(start).isSpace())
            ++start;
    }

    if (text.mid(start, 5).toLower() == QLatin1String("<!doc"))
        return true;
    int open = start;
    while (open < text.length() && text.at(open) != QLatin1Char('<')
            && text.at(open) != QLatin1Char('\n')) {
        if (text.at(open) == QLatin1Char('&') &&  text.mid(open+1,3) == QLatin1String("lt;"))
            return true; // support desperate attempt of user to see <...>
        ++open;
    }
    if (open < text.length() && text.at(open) == QLatin1Char('<')) {
        const int close = text.indexOf(QLatin1Char('>'), open);
        if (close > -1) {
            QByteArray tag;
            for (int i = open+1; i < close; ++i) {
                if (text[i].isDigit() || text[i].isLetter())
                    tag += text[i].toLatin1();
                else if (!tag.isEmpty() && text[i].isSpace())
                    break;
                else if (!tag.isEmpty() && text[i] == QLatin1Char('/') && i + 1 == close)
                    break;
                else if (!text[i].isSpace() && (!tag.isEmpty() || text[i] != QLatin1Char('!')))
                    return false; // that's not a tag
            }
            for (qint16 i = 0; i < elementsTblSize; i++) {
                if (qstricmp(tag.constData(), elementsTbl[i]) == 0) {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

/*!
    Converts the plain text string \a plain to a HTML string with
    HTML metacharacters \c{<}, \c{>}, \c{&}, \c{'}, and \c{"} replaced by HTML
    entities.

    Example:

    \snippet doc/src/snippets/code/src_gui_text_qtextdocument.cpp 0

    \sa convertFromPlainText(), mightBeRichText()
*/
QString Qt::escape(const QString& plain)
{
    QString rich;
    rich.reserve(int(plain.length() * qreal(1.1)));
    for (int i = 0; i < plain.length(); ++i) {
        if (plain.at(i) == QLatin1Char('<'))
            rich += QLatin1String("&lt;");
        else if (plain.at(i) == QLatin1Char('>'))
            rich += QLatin1String("&gt;");
        else if (plain.at(i) == QLatin1Char('&'))
            rich += QLatin1String("&amp;");
        else if (plain.at(i) == QLatin1Char('\''))
            rich += QLatin1String("&apos;");
        else if (plain.at(i) == QLatin1Char('"'))
            rich += QLatin1String("&quot;");
        else
            rich += plain.at(i);
    }
    return rich;
}

/*!
    \fn QString Qt::convertFromPlainText(const QString &plain, WhiteSpaceMode mode)

    Converts the plain text string \a plain to an HTML-formatted
    paragraph while preserving most of its look.

    \a mode defines how whitespace is handled.

    \sa escape(), mightBeRichText()
*/
QString Qt::convertFromPlainText(const QString &plain, Qt::WhiteSpaceMode mode)
{
    int col = 0;
    QString rich(QLatin1String("<p>"));
    for (int i = 0; i < plain.length(); ++i) {
        if (plain[i] == QLatin1Char('\n')){
            int c = 1;
            while (i+1 < plain.length() && plain[i+1] == QLatin1Char('\n')) {
                i++;
                c++;
            }
            if (c == 1)
                rich += QLatin1String("<br>\n");
            else {
                rich += QLatin1String("</p>\n");
                while (--c > 1)
                    rich += QLatin1String("<br>\n");
                rich += QLatin1String("<p>");
            }
            col = 0;
        } else {
            if (mode == Qt::WhiteSpacePre && plain[i] == QLatin1Char('\t')){
                rich += QChar(0x00a0U);
                ++col;
                while (col % 8) {
                    rich += QChar(0x00a0U);
                    ++col;
                }
            }
            else if (mode == Qt::WhiteSpacePre && plain[i].isSpace())
                rich += QChar(0x00a0U);
            else if (plain[i] == QLatin1Char('<'))
                rich += QLatin1String("&lt;");
            else if (plain[i] == QLatin1Char('>'))
                rich += QLatin1String("&gt;");
            else if (plain[i] == QLatin1Char('&'))
                rich += QLatin1String("&amp;");
            else
                rich += plain[i];
            ++col;
        }
    }
    if (col != 0)
        rich += QLatin1String("</p>");
    return rich;
}

QT_END_NAMESPACE
