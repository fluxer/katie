/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2019 Ivailo Monev
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGLOBAL_H
#define QGLOBAL_H

#include <qconfig.h>
#include <stddef.h>

#define QT_VERSION_STR   "${KATIE_VERSION}"
/*
   QT_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define QT_VERSION ${KATIE_HEX}
/*
   can be used like #if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
*/
#define QT_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

#define QT_PACKAGEDATE_STR "YYYY-MM-DD"

#define QT_PACKAGE_TAG ""

/*
   The architechture, must be one of: (QT_ARCH_x)

     ALPHA     - ...
     ARM       - ...
     !ARMV6     - ...
     AVR32     - ...
     BFIN      - ...
     !GENERIC   - ...
     I386      - ...
     IA64      - ...
     M68K      - ...
     MIPS      - ...
     !PARISC    - ...
     !POWERPC   - ...
     S390      - ...
     SPARC     - ...
     X86_64    - ...
     SH        - ...
     !SH4A      - ...
     NACL      - ...

   Useful hyper-links:

     https://gcc.gnu.org/backends.html
     https://en.wikipedia.org/wiki/GNU_Compiler_Collection#Architectures
*/

#if defined(__alpha__) || defined(_M_ALPHA)
#  define QT_ARCH_ALPHA
#elif defined(__arm__) || defined(_M_ARM)
#  define QT_ARCH_ARM
#elif defined(__avr__)
#  define QT_ARCH_AVR32
#elif defined(__bfin__)
#  define QT_ARCH_BFIN
#elif defined(__ia64__) || defined(_M_IA64)
#  define QT_ARCH_IA64
#elif defined(__m68k__)
#  define QT_ARCH_M68K
#elif defined(__mips__)
#  define QT_ARCH_MIPS
#elif defined(__s390__)
#  define QT_ARCH_S390
#elif defined(__sparc__)
#  define QT_ARCH_SPARC
#elif defined(__sh__)
#  define QT_ARCH_SH
#elif defined(__native_client__)
#  define QT_ARCH_NACL
#elif defined(__x86_64__) || defined(_M_X64)
#  define QT_ARCH_X86_64
#elif defined(__i386__) || defined(_M_IX86)
#  define QT_ARCH_I386
#else
# error Unable to detect architecture, please update above list
#endif

// detect target endianness
#if defined (__BYTE_ORDER__) && \
    (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#  define Q_BYTE_ORDER       __BYTE_ORDER__
#  define Q_BIG_ENDIAN       __ORDER_BIG_ENDIAN__
#  define Q_LITTLE_ENDIAN    __ORDER_LITTLE_ENDIAN__
#elif defined (__LITTLE_ENDIAN__)
#  define Q_BIG_ENDIAN 1234
#  define Q_LITTLE_ENDIAN 4321
#  define Q_BYTE_ORDER Q_LITTLE_ENDIAN
#elif defined (__BIG_ENDIAN__)
#  define Q_BIG_ENDIAN 1234
#  define Q_LITTLE_ENDIAN 4321
#  define Q_BYTE_ORDER Q_BIG_ENDIAN
#else
#  error Unable to detect target endianness
#endif

// needed for std::swap
#ifdef __cplusplus
#include <algorithm>
#endif

#if defined(__cplusplus) && !defined(QT_NO_USING_NAMESPACE)

# define QT_NAMESPACE Katie
# define QT_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::name
# define QT_MANGLE_NAMESPACE(x) x
# define QT_USE_NAMESPACE using namespace ::QT_NAMESPACE;

# define QT_BEGIN_NAMESPACE namespace QT_NAMESPACE {
# define QT_END_NAMESPACE }
# define QT_BEGIN_INCLUDE_NAMESPACE }
# define QT_END_INCLUDE_NAMESPACE namespace QT_NAMESPACE {

namespace QT_NAMESPACE {}

# ifdef QT_NAMESPACE_COMPAT
QT_USE_NAMESPACE
# endif

#else /* QT_NO_USING_NAMESPACE && __cplusplus */

# define QT_NAMESPACE
# define QT_PREPEND_NAMESPACE(name) ::name
# define QT_MANGLE_NAMESPACE(x) x
# define QT_USE_NAMESPACE

# define QT_BEGIN_NAMESPACE
# define QT_END_NAMESPACE
# define QT_BEGIN_INCLUDE_NAMESPACE
# define QT_END_INCLUDE_NAMESPACE

#endif /* QT_NO_USING_NAMESPACE && __cplusplus */

#define QT_BEGIN_HEADER
#define QT_END_HEADER
#define QT_BEGIN_INCLUDE_HEADER
#define QT_END_INCLUDE_HEADER extern "C++"

/*
   The operating system, must be one of: (Q_OS_x)

     SOLARIS  - Sun Solaris
     HPUX     - HP-UX
     ULTRIX   - DEC Ultrix
     LINUX    - Linux
     FREEBSD  - FreeBSD
     NETBSD   - NetBSD
     OPENBSD  - OpenBSD
     BSDI     - BSD/OS
     IRIX     - SGI Irix
     OSF      - HP Tru64 UNIX
     SCO      - SCO OpenServer 5
     UNIXWARE - UnixWare 7, Open UNIX 8
     AIX      - AIX
     HURD     - GNU Hurd
     DGUX     - DG/UX
     RELIANT  - Reliant UNIX
     DYNIX    - DYNIX/ptx
     QNX      - QNX
     LYNX     - LynxOS
     BSD4     - Any BSD 4.4 system
     UNIX     - Any UNIX BSD/SYSV system
*/

#if defined(__sun) || defined(sun)
#  define Q_OS_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define Q_OS_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define Q_OS_ULTRIX
#elif defined(sinix)
#  define Q_OS_RELIANT
#elif defined(__native_client__)
#  define Q_OS_NACL
#elif defined(__linux__) || defined(__linux)
#  define Q_OS_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#  define Q_OS_FREEBSD
#  define Q_OS_BSD4
#elif defined(__NetBSD__)
#  define Q_OS_NETBSD
#  define Q_OS_BSD4
#elif defined(__OpenBSD__)
#  define Q_OS_OPENBSD
#  define Q_OS_BSD4
#elif defined(__bsdi__)
#  define Q_OS_BSDI
#  define Q_OS_BSD4
#elif defined(__osf__)
#  define Q_OS_OSF
#elif defined(_AIX)
#  define Q_OS_AIX
#elif defined(__Lynx__)
#  define Q_OS_LYNX
#elif defined(__GNU__)
#  define Q_OS_HURD
#elif defined(__DGUX__)
#  define Q_OS_DGUX
#elif defined(_SEQUENT_)
#  define Q_OS_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define Q_OS_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define Q_OS_UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define Q_OS_UNIXWARE
#else
#  error "Katie has not been ported to this OS"
#endif

#if !defined(Q_OS_UNIX)
#  define Q_OS_UNIX
#endif

#ifdef __LSB_VERSION__
#  if __LSB_VERSION__ < 40
#    error "This version of the Linux Standard Base is unsupported"
#  endif
#ifndef QT_LINUXBASE
#  define QT_LINUXBASE
#endif
#endif

/*
   The compiler, must be one of: (Q_CC_x)

     GNU      - GNU C++
     CLANG    - C++ front-end for the LLVM compiler

   Should be sorted most to least authoritative.
*/

#if defined(__GNUC__)
#  define Q_CC_GNU
#  define Q_C_CALLBACKS
#  define Q_ALIGNOF(type)   __alignof__(type)
#  define Q_TYPEOF(expr)    __typeof__(expr)
#  define Q_DECL_ALIGN(n)   __attribute__((__aligned__(n)))
#  define Q_LIKELY(expr)    __builtin_expect(!!(expr), true)
#  define Q_UNLIKELY(expr)  __builtin_expect(!!(expr), false)
#  if !defined(QT_MOC_CPP)
#    define Q_PACKED __attribute__ ((__packed__))
#    define Q_NO_PACKED_REFERENCE
#    ifndef __ARM_EABI__
#      define QT_NO_ARM_EABI
#    endif
#  endif

#elif defined(__clang__)
#  define Q_CC_CLANG
#  if !defined(__has_extension)
#    /* Compatibility with older Clang versions */
#    define __has_extension __has_feature
#  endif
#  define Q_C_CALLBACKS
#  define Q_ALIGNOF(type)   __alignof__(type)
#  define Q_TYPEOF(expr)    __typeof__(expr)
#  define Q_DECL_ALIGN(n)   __attribute__((__aligned__(n)))
#  define Q_LIKELY(expr)    __builtin_expect(!!(expr), true)
#  define Q_UNLIKELY(expr)  __builtin_expect(!!(expr), false)
#  if !defined(QT_MOC_CPP)
#    define Q_PACKED __attribute__ ((__packed__))
#    define Q_NO_PACKED_REFERENCE
#  endif

#else
#  error "Katie has not been tested with this compiler"
#endif

/*
 * C++11 support
 *
 *  Paper           Macro                               SD-6 macro
 *  N2541           Q_COMPILER_AUTO_FUNCTION
 *  N1984 N2546     Q_COMPILER_AUTO_TYPE
 *  N2437           Q_COMPILER_CLASS_ENUM
 *  N2235           Q_COMPILER_CONSTEXPR                __cpp_constexpr = 200704
 *  N2343 N3276     Q_COMPILER_DECLTYPE                 __cpp_decltype = 200707
 *  N2346           Q_COMPILER_DEFAULT_DELETE_MEMBERS
 *  N1987           Q_COMPILER_EXTERN_TEMPLATES
 *  N2672           Q_COMPILER_INITIALIZER_LISTS
 *  N2658 N2927     Q_COMPILER_LAMBDA                   __cpp_lambdas = 200907
 *  N2118 N2844 N3053 Q_COMPILER_RVALUE_REFS            __cpp_rvalue_references = 200610
 *  N2442           Q_COMPILER_UNICODE_STRINGS          __cpp_unicode_literals = 200710
 *  N2242 N2555     Q_COMPILER_VARIADIC_TEMPLATES       __cpp_variadic_templates = 200704
 *  N2431           Q_COMPILER_NULLPTR
 *
 * For any future version of the C++ standard, we use only the SD-6 macro.
 * For full listing, see
 *  http://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
 */

#if defined(Q_CC_CLANG)
#  if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    /* Detect C++ features using __has_feature(), see http://clang.llvm.org/docs/LanguageExtensions.html#cxx11 */
#    if __has_feature(cxx_auto_type)
#      define Q_COMPILER_AUTO_FUNCTION
#      define Q_COMPILER_AUTO_TYPE
#    endif
#    if __has_feature(cxx_constexpr)
#      define Q_COMPILER_CONSTEXPR
#    endif
#    if __has_feature(cxx_decltype) /* && __has_feature(cxx_decltype_incomplete_return_types) */
#      define Q_COMPILER_DECLTYPE
#    endif
#    if __has_feature(cxx_defaulted_functions) && __has_feature(cxx_deleted_functions)
#      define Q_COMPILER_DEFAULT_DELETE_MEMBERS
#    endif
#    if __has_feature(cxx_generalized_initializers)
#      define Q_COMPILER_INITIALIZER_LISTS
#    endif
#    if __has_feature(cxx_lambdas)
#      define Q_COMPILER_LAMBDA
#    endif
#    if __has_feature(cxx_rvalue_references)
#      define Q_COMPILER_RVALUE_REFS
#    endif
#    if __has_feature(cxx_strong_enums)
#      define Q_COMPILER_CLASS_ENUM
#    endif
#    if __has_feature(cxx_unicode_literals)
#      define Q_COMPILER_UNICODE_STRINGS
#    endif
#    if __has_feature(cxx_variadic_templates)
#      define Q_COMPILER_VARIADIC_TEMPLATES
#    endif
#    if __has_feature(cxx_nullptr)
#      define Q_COMPILER_NULLPTR
#    endif
    /* Features that have no __has_feature() check */
#    if ((__clang_major__ * 100) + __clang_minor__) >= 209 /* since clang 2.9 */
#      define Q_COMPILER_EXTERN_TEMPLATES
#    endif
#  endif
#endif

#if defined(Q_CC_GNU) && !defined(Q_CC_CLANG)
#  if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
     /* C++0x features supported in GCC 4.3: */
#    define Q_COMPILER_RVALUE_REFS
#    define Q_COMPILER_DECLTYPE
     /* C++0x features supported in GCC 4.4: */
#    define Q_COMPILER_VARIADIC_TEMPLATES
#    define Q_COMPILER_AUTO_FUNCTION
#    define Q_COMPILER_AUTO_TYPE
#    define Q_COMPILER_EXTERN_TEMPLATES
#    define Q_COMPILER_DEFAULT_DELETE_MEMBERS
#    define Q_COMPILER_CLASS_ENUM
#    define Q_COMPILER_INITIALIZER_LISTS
#    /* C++0x features supported in GCC 4.5: */
#    define Q_COMPILER_LAMBDA
#    define Q_COMPILER_UNICODE_STRINGS
#    /* C++0x features supported in GCC 4.6: */
#    define Q_COMPILER_CONSTEXPR
#    define Q_COMPILER_NULLPTR
#  endif
#endif

#ifdef Q_COMPILER_CONSTEXPR
# define Q_DECL_CONSTEXPR constexpr
#else
# define Q_DECL_CONSTEXPR
#endif

#ifdef Q_COMPILER_NULLPTR
# define Q_NULLPTR nullptr
#else
# define Q_NULLPTR NULL
#endif

#ifndef Q_PACKED
#  define Q_PACKED
#  undef Q_NO_PACKED_REFERENCE
#endif

#ifndef Q_CONSTRUCTOR_FUNCTION
# define Q_CONSTRUCTOR_FUNCTION0(AFUNC) \
   static const int AFUNC ## __init_variable__ = AFUNC();
# define Q_CONSTRUCTOR_FUNCTION(AFUNC) Q_CONSTRUCTOR_FUNCTION0(AFUNC)
#endif

#ifndef Q_DESTRUCTOR_FUNCTION
# define Q_DESTRUCTOR_FUNCTION0(AFUNC) \
    class AFUNC ## __dest_class__ { \
    public: \
       inline AFUNC ## __dest_class__() { } \
       inline ~ AFUNC ## __dest_class__() { AFUNC(); } \
    } AFUNC ## __dest_instance__;
# define Q_DESTRUCTOR_FUNCTION(AFUNC) Q_DESTRUCTOR_FUNCTION0(AFUNC)
#endif

#ifndef Q_REQUIRED_RESULT
#  if defined(Q_CC_GNU) || defined(Q_CC_CLANG)
#    define Q_REQUIRED_RESULT __attribute__ ((warn_unused_result))
#  else
#    define Q_REQUIRED_RESULT
#  endif
#endif

/*
   The window system, must be one of: (Q_WS_x)

     X11      - X Window System
*/

#if defined(Q_OS_UNIX)
#  define Q_WS_X11
#endif

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

/*
   Size-dependent types (architechture-dependent byte order)

   Make sure to update QMetaType when changing these typedefs
*/

typedef signed char qint8;         /* 8 bit signed */
typedef unsigned char quint8;      /* 8 bit unsigned */
typedef short qint16;              /* 16 bit signed */
typedef unsigned short quint16;    /* 16 bit unsigned */
typedef int qint32;                /* 32 bit signed */
typedef unsigned int quint32;      /* 32 bit unsigned */
#define Q_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#define Q_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long qint64;           /* 64 bit signed */
typedef unsigned long long quint64; /* 64 bit unsigned */

typedef qint64 qlonglong;
typedef quint64 qulonglong;

#define Q_INIT_RESOURCE_EXTERN(name) \
    extern int QT_MANGLE_NAMESPACE(qInitResources_ ## name) ();

#define Q_INIT_RESOURCE(name) \
    do { extern int QT_MANGLE_NAMESPACE(qInitResources_ ## name) ();       \
        QT_MANGLE_NAMESPACE(qInitResources_ ## name) (); } while (0)
#define Q_CLEANUP_RESOURCE(name) \
    do { extern int QT_MANGLE_NAMESPACE(qCleanupResources_ ## name) ();    \
        QT_MANGLE_NAMESPACE(qCleanupResources_ ## name) (); } while (0)

#if defined(__cplusplus)

/*
  quintptr and qptrdiff is guaranteed to be the same size as a pointer, i.e.

      sizeof(void *) == sizeof(quintptr)
      && sizeof(void *) == sizeof(qptrdiff)
*/
template <int> struct QIntegerForSize;
template <>    struct QIntegerForSize<1> { typedef quint8  Unsigned; typedef qint8  Signed; };
template <>    struct QIntegerForSize<2> { typedef quint16 Unsigned; typedef qint16 Signed; };
template <>    struct QIntegerForSize<4> { typedef quint32 Unsigned; typedef qint32 Signed; };
template <>    struct QIntegerForSize<8> { typedef quint64 Unsigned; typedef qint64 Signed; };
template <class T> struct QIntegerForSizeof: QIntegerForSize<sizeof(T)> { };
typedef QIntegerForSizeof<void*>::Unsigned quintptr;
typedef QIntegerForSizeof<void*>::Signed qptrdiff;

/*
   Useful type definitions for Qt
*/

QT_BEGIN_INCLUDE_NAMESPACE
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
QT_END_INCLUDE_NAMESPACE

/*
   Warnings and errors when using deprecated methods
*/
#if defined(Q_MOC_RUN)
#  define Q_DECL_DEPRECATED Q_DECL_DEPRECATED
#elif defined(Q_CC_GNU) || defined(Q_CC_CLANG)
#  define Q_DECL_DEPRECATED __attribute__ ((__deprecated__))
#else
#  define Q_DECL_DEPRECATED
#endif
#ifndef Q_DECL_VARIABLE_DEPRECATED
#  define Q_DECL_VARIABLE_DEPRECATED Q_DECL_DEPRECATED
#endif
#ifndef Q_DECL_CONSTRUCTOR_DEPRECATED
#  if defined(Q_MOC_RUN)
#    define Q_DECL_CONSTRUCTOR_DEPRECATED Q_DECL_CONSTRUCTOR_DEPRECATED
#  elif defined(Q_NO_DEPRECATED_CONSTRUCTORS)
#    define Q_DECL_CONSTRUCTOR_DEPRECATED
#  else
#    define Q_DECL_CONSTRUCTOR_DEPRECATED Q_DECL_DEPRECATED
#  endif
#endif

#if defined(QT_NO_DEPRECATED)
/* disable Qt3 support as well */
#  undef QT3_SUPPORT_WARNINGS
#  undef QT3_SUPPORT
#  undef QT_DEPRECATED
#  undef QT_DEPRECATED_VARIABLE
#  undef QT_DEPRECATED_CONSTRUCTOR
#elif defined(QT_DEPRECATED_WARNINGS)
#  undef QT_DEPRECATED
#  define QT_DEPRECATED Q_DECL_DEPRECATED
#  undef QT_DEPRECATED_VARIABLE
#  define QT_DEPRECATED_VARIABLE Q_DECL_VARIABLE_DEPRECATED
#  undef QT_DEPRECATED_CONSTRUCTOR
#  define QT_DEPRECATED_CONSTRUCTOR explicit Q_DECL_CONSTRUCTOR_DEPRECATED
#else
#  undef QT_DEPRECATED
#  define QT_DEPRECATED
#  undef QT_DEPRECATED_VARIABLE
#  define QT_DEPRECATED_VARIABLE
#  undef QT_DEPRECATED_CONSTRUCTOR
#  define QT_DEPRECATED_CONSTRUCTOR
#endif

#ifdef QT_ASCII_CAST_WARNINGS
#  define QT_ASCII_CAST_WARN Q_DECL_DEPRECATED
#  define QT_ASCII_CAST_WARN_CONSTRUCTOR Q_DECL_CONSTRUCTOR_DEPRECATED
#else
#  define QT_ASCII_CAST_WARN
#  define QT_ASCII_CAST_WARN_CONSTRUCTOR
#endif

#if defined(QT_ARCH_I386) && (defined(Q_CC_GNU) || defined(Q_CC_CLANG))
#  define QT_FASTCALL __attribute__((regparm(3)))
#else
#  define QT_FASTCALL
#endif

#if defined(QT_ARCH_ARM) || defined(QT_ARCH_ARMV6) || defined(QT_ARCH_AVR32) || defined(QT_ARCH_SH) || defined(QT_ARCH_SH4A)
#define QT_NO_FPU
#endif

// This logic must match the one in qmetatype.h
#if defined(QT_NO_FPU)
typedef float qreal;
#else
typedef double qreal;
#endif

/*
   Utility macros and inline functions
*/

template <typename T>
Q_DECL_CONSTEXPR inline T qAbs(const T &t) { return t >= 0 ? t : -t; }

template <typename T>
Q_DECL_CONSTEXPR inline const T &qMin(const T &a, const T &b) { return (a < b) ? a : b; }
template <typename T>
Q_DECL_CONSTEXPR inline const T &qMax(const T &a, const T &b) { return (a < b) ? b : a; }
template <typename T>
Q_DECL_CONSTEXPR inline const T &qBound(const T &min, const T &val, const T &max)
{ return qMax(min, qMin(max, val)); }

#ifndef Q_DECL_EXPORT
#  if defined(QT_VISIBILITY_AVAILABLE)
#    define Q_DECL_EXPORT __attribute__((visibility("default")))
#    define Q_DECL_HIDDEN __attribute__((visibility("hidden")))
#  endif
#  ifndef Q_DECL_EXPORT
#    define Q_DECL_EXPORT
#  endif
#endif
#ifndef Q_DECL_IMPORT
#  define Q_DECL_IMPORT
#endif
#ifndef Q_DECL_HIDDEN
#  define Q_DECL_HIDDEN
#endif

#if !defined(Q_CORE_EXPORT)
#  if defined(QT_SHARED)
#    define Q_CORE_EXPORT Q_DECL_EXPORT
#    define Q_GUI_EXPORT Q_DECL_EXPORT
#    define Q_SQL_EXPORT Q_DECL_EXPORT
#    define Q_NETWORK_EXPORT Q_DECL_EXPORT
#    define Q_SVG_EXPORT Q_DECL_EXPORT
#    define Q_TEST_EXPORT Q_DECL_EXPORT
#    define Q_DECLARATIVE_EXPORT Q_DECL_EXPORT
#    define Q_XML_EXPORT Q_DECL_EXPORT
#    define Q_SCRIPT_EXPORT Q_DECL_EXPORT
#    define Q_SCRIPTTOOLS_EXPORT Q_DECL_EXPORT
#    define Q_DBUS_EXPORT Q_DECL_EXPORT
#    define Q_UITOOLS_EXPORT Q_DECL_EXPORT
#  else
#    define Q_CORE_EXPORT
#    define Q_GUI_EXPORT
#    define Q_SQL_EXPORT
#    define Q_NETWORK_EXPORT
#    define Q_SVG_EXPORT
#    define Q_TEST_EXPORT
#    define Q_DECLARATIVE_EXPORT
#    define Q_XML_EXPORT
#    define Q_SCRIPT_EXPORT
#    define Q_SCRIPTTOOLS_EXPORT
#    define Q_DBUS_EXPORT
#    define Q_UITOOLS_EXPORT
#  endif
#endif

#define Q_CORE_EXPORT_INLINE Q_CORE_EXPORT inline
#define Q_GUI_EXPORT_INLINE Q_GUI_EXPORT inline

/*
   No, this is not an evil backdoor. QT_BUILD_INTERNAL just exports more symbols
   for Qt's internal unit tests. If you want slower loading times and more
   symbols that can vanish from version to version, feel free to define QT_BUILD_INTERNAL.
*/
#if defined(QT_BUILD_INTERNAL) && defined(QT_SHARED)
#    define Q_AUTOTEST_EXPORT Q_DECL_EXPORT
#else
#    define Q_AUTOTEST_EXPORT
#endif

inline void qt_noop(void) {}

/* These wrap try/catch so we can switch off exceptions later.

   Beware - do not use more than one QT_CATCH per QT_TRY, and do not use
   the exception instance in the catch block.
   If you can't live with those constraints, don't use these macros.
   Use the QT_NO_EXCEPTIONS macro to protect your code instead.
*/

#if !defined(QT_NO_EXCEPTIONS) && defined(Q_CC_GNU) && !defined (__EXCEPTIONS) && !defined(Q_MOC_RUN)
#  define QT_NO_EXCEPTIONS
#endif

#ifdef QT_NO_EXCEPTIONS
#  define QT_TRY if (true)
#  define QT_CATCH(A) else
#  define QT_THROW(A) qt_noop()
#  define QT_RETHROW qt_noop()
#else
#  define QT_TRY try
#  define QT_CATCH(A) catch (A)
#  define QT_THROW(A) throw A
#  define QT_RETHROW throw
#endif

/*
   System information
*/

class Q_CORE_EXPORT QSysInfo {
public:
    enum Sizes {
        WordSize = (sizeof(void *)<<3)
    };

    enum Endian {
        BigEndian,
        LittleEndian,

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        ByteOrder = BigEndian
#elif Q_BYTE_ORDER == Q_LITTLE_ENDIAN
        ByteOrder = LittleEndian
#else
#  error "Undefined byte order"
#endif
    };
};

Q_CORE_EXPORT const char *qVersion();
Q_CORE_EXPORT bool qSharedBuild();

#ifndef Q_OUTOFLINE_TEMPLATE
#  define Q_OUTOFLINE_TEMPLATE
#endif
#ifndef Q_INLINE_TEMPLATE
#  define Q_INLINE_TEMPLATE inline
#endif

#ifndef Q_TYPENAME
#  define Q_TYPENAME typename
#endif

/*
   Avoid "unused parameter" warnings
*/

#define Q_UNUSED(x) (void)x;

/*
   Debugging and error handling
*/

#if !defined(QT_NO_DEBUG) && !defined(QT_DEBUG)
#  define QT_DEBUG
#endif

#if (defined(QT_NO_DEBUG_OUTPUT) || defined(QT_NO_TEXTSTREAM)) && !defined(QT_NO_DEBUG_STREAM)
#define QT_NO_DEBUG_STREAM
#endif

class QString;
#ifndef qPrintable
#  define qPrintable(string) QString(string).toLocal8Bit().constData()
#endif

Q_CORE_EXPORT void qDebug(const char *, ...) /* print debug message */
#if (defined(Q_CC_GNU) || defined(Q_CC_CLANG)) && !defined(__INSURE__)
    __attribute__ ((format (printf, 1, 2)))
#endif
;

Q_CORE_EXPORT void qWarning(const char *, ...) /* print warning message */
#if (defined(Q_CC_GNU) || defined(Q_CC_CLANG)) && !defined(__INSURE__)
    __attribute__ ((format (printf, 1, 2)))
#endif
;

Q_CORE_EXPORT QString qt_error_string(int errorCode = -1);
Q_CORE_EXPORT void qCritical(const char *, ...) /* print critical message */
#if (defined(Q_CC_GNU) || defined(Q_CC_CLANG)) && !defined(__INSURE__)
    __attribute__ ((format (printf, 1, 2)))
#endif
;
Q_CORE_EXPORT void qFatal(const char *, ...) /* print fatal message and exit */
#if (defined(Q_CC_GNU) || defined(Q_CC_CLANG)) && !defined(__INSURE__)
    __attribute__ ((format (printf, 1, 2)))
#endif
;

Q_CORE_EXPORT void qErrnoWarning(int code, const char *msg, ...);
Q_CORE_EXPORT void qErrnoWarning(const char *msg, ...);

Q_CORE_EXPORT void qt_assert(const char *assertion, const char *file, int line);

#if !defined(Q_ASSERT)
#  ifndef QT_NO_DEBUG
#    define Q_ASSERT(cond) ((!(cond)) ? qt_assert(#cond,__FILE__,__LINE__) : qt_noop())
#  else
#    define Q_ASSERT(cond) qt_noop()
#  endif
#endif

Q_CORE_EXPORT void qt_assert_x(const char *where, const char *what, const char *file, int line);

#if !defined(Q_ASSERT_X)
#  ifndef QT_NO_DEBUG
#    define Q_ASSERT_X(cond, where, what) ((!(cond)) ? qt_assert_x(where, what,__FILE__,__LINE__) : qt_noop())
#  else
#    define Q_ASSERT_X(cond, where, what) qt_noop()
#  endif
#endif

Q_CORE_EXPORT void qt_check_pointer(const char *, int);
Q_CORE_EXPORT void qBadAlloc();

#ifdef QT_NO_EXCEPTIONS
#  if defined(QT_NO_DEBUG)
#    define Q_CHECK_PTR(p) qt_noop()
#  else
#    define Q_CHECK_PTR(p) do {if(!(p))qt_check_pointer(__FILE__,__LINE__);} while (0)
#  endif
#else
#  define Q_CHECK_PTR(p) do { if (!(p)) qBadAlloc(); } while (0)
#endif

template <typename T>
inline T *q_check_ptr(T *p) { Q_CHECK_PTR(p); return p; }

#if defined(Q_CC_GNU) && !defined(Q_OS_SOLARIS)
#  define Q_FUNC_INFO __PRETTY_FUNCTION__
#else
#   if defined(Q_OS_SOLARIS)
#      define Q_FUNC_INFO __FILE__ "(line number unavailable)"
#   else
        /* These two macros makes it possible to turn the builtin line expander into a
         * string literal. */
#       define QT_STRINGIFY2(x) #x
#       define QT_STRINGIFY(x) QT_STRINGIFY2(x)
#       define Q_FUNC_INFO __FILE__ ":" QT_STRINGIFY(__LINE__)
#   endif
#   undef QT_STRINGIFY2
#   undef QT_STRINGIFY
#endif

enum QtMsgType { QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg, QtSystemMsg = QtCriticalMsg };

Q_CORE_EXPORT void qt_message_output(QtMsgType, const char *buf);

typedef void (*QtMsgHandler)(QtMsgType, const char *);
Q_CORE_EXPORT QtMsgHandler qInstallMsgHandler(QtMsgHandler);


#if defined(QT_NO_THREAD)

template <typename T>
class QGlobalStatic
{
public:
    T *pointer;
    inline QGlobalStatic(T *p) : pointer(p) { }
    inline ~QGlobalStatic() { pointer = Q_NULLPTR; }
};

#define Q_GLOBAL_STATIC(TYPE, NAME)                                  \
    static TYPE *NAME()                                              \
    {                                                                \
        static TYPE thisVariable;                                    \
        static QGlobalStatic<TYPE > thisGlobalStatic(&thisVariable); \
        return thisGlobalStatic.pointer;                             \
    }

#define Q_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ARGS)                  \
    static TYPE *NAME()                                              \
    {                                                                \
        static TYPE thisVariable ARGS;                               \
        static QGlobalStatic<TYPE > thisGlobalStatic(&thisVariable); \
        return thisGlobalStatic.pointer;                             \
    }

#define Q_GLOBAL_STATIC_WITH_INITIALIZER(TYPE, NAME, INITIALIZER)    \
    static TYPE *NAME()                                              \
    {                                                                \
        static TYPE thisVariable;                                    \
        static QGlobalStatic<TYPE > thisGlobalStatic(0);             \
        if (!thisGlobalStatic.pointer) {                             \
            TYPE *x = thisGlobalStatic.pointer = &thisVariable;      \
            INITIALIZER;                                             \
        }                                                            \
        return thisGlobalStatic.pointer;                             \
    }

#else

// forward declaration, since qatomic.h needs qglobal.h
template <typename T> class QAtomicPointer;

// POD for Q_GLOBAL_STATIC
template <typename T>
class QGlobalStatic
{
public:
    QAtomicPointer<T> pointer;
    bool destroyed;
};

// Created as a function-local static to delete a QGlobalStatic<T>
template <typename T>
class QGlobalStaticDeleter
{
public:
    QGlobalStatic<T> &globalStatic;
    QGlobalStaticDeleter(QGlobalStatic<T> &_globalStatic)
        : globalStatic(_globalStatic)
    { }

    inline ~QGlobalStaticDeleter()
    {
        delete globalStatic.pointer;
        globalStatic.pointer = Q_NULLPTR;
        globalStatic.destroyed = true;
    }
};

#define Q_GLOBAL_STATIC_INIT(TYPE, NAME)                                      \
        static QGlobalStatic<TYPE > this_ ## NAME                             \
                            = { QAtomicPointer<TYPE>(Q_NULLPTR), false }

#define Q_GLOBAL_STATIC(TYPE, NAME)                                           \
    static TYPE *NAME()                                                       \
    {                                                                         \
        Q_GLOBAL_STATIC_INIT(TYPE, _StaticVar_);                              \
        if (!this__StaticVar_.pointer && !this__StaticVar_.destroyed) {       \
            TYPE *x = new TYPE;                                               \
            if (!this__StaticVar_.pointer.testAndSetOrdered(Q_NULLPTR, x))    \
                delete x;                                                     \
            else                                                              \
                static QGlobalStaticDeleter<TYPE > cleanup(this__StaticVar_); \
        }                                                                     \
        return this__StaticVar_.pointer;                                      \
    }

#define Q_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ARGS)                           \
    static TYPE *NAME()                                                       \
    {                                                                         \
        Q_GLOBAL_STATIC_INIT(TYPE, _StaticVar_);                              \
        if (!this__StaticVar_.pointer && !this__StaticVar_.destroyed) {       \
            TYPE *x = new TYPE ARGS;                                          \
            if (!this__StaticVar_.pointer.testAndSetOrdered(0, x))            \
                delete x;                                                     \
            else                                                              \
                static QGlobalStaticDeleter<TYPE > cleanup(this__StaticVar_); \
        }                                                                     \
        return this__StaticVar_.pointer;                                      \
    }

#define Q_GLOBAL_STATIC_WITH_INITIALIZER(TYPE, NAME, INITIALIZER)             \
    static TYPE *NAME()                                                       \
    {                                                                         \
        Q_GLOBAL_STATIC_INIT(TYPE, _StaticVar_);                              \
        if (!this__StaticVar_.pointer && !this__StaticVar_.destroyed) {       \
            QScopedPointer<TYPE > x(new TYPE);                                \
            INITIALIZER;                                                      \
            if (this__StaticVar_.pointer.testAndSetOrdered(0, x.data())) {    \
                static QGlobalStaticDeleter<TYPE > cleanup(this__StaticVar_); \
                x.take();                                                     \
            }                                                                 \
        }                                                                     \
        return this__StaticVar_.pointer;                                      \
    }

#endif

Q_DECL_CONSTEXPR static inline bool qFuzzyCompare(double p1, double p2)
{
    return (qAbs(p1 - p2) <= 0.000000000001 * qMin(qAbs(p1), qAbs(p2)));
}

Q_DECL_CONSTEXPR static inline bool qFuzzyCompare(float p1, float p2)
{
    return (qAbs(p1 - p2) <= 0.00001f * qMin(qAbs(p1), qAbs(p2)));
}

/*!
  \internal
*/
Q_DECL_CONSTEXPR static inline bool qFuzzyIsNull(double d)
{
    return qAbs(d) <= 0.000000000001;
}

/*!
  \internal
*/
Q_DECL_CONSTEXPR static inline bool qFuzzyIsNull(float f)
{
    return qAbs(f) <= 0.00001f;
}

/*
   This function tests a double for a null value. It doesn't
   check whether the actual value is 0 or close to 0, but whether
   it is binary 0.
*/
static inline bool qIsNull(double d)
{
    union U {
        double d;
        quint64 u;
    };
    U val;
    val.d = d;
    return val.u == quint64(0);
}

/*
   This function tests a float for a null value. It doesn't
   check whether the actual value is 0 or close to 0, but whether
   it is binary 0.
*/
static inline bool qIsNull(float f)
{
    union U {
        float f;
        quint32 u;
    };
    U val;
    val.f = f;
    return val.u == 0u;
}

/*
   Compilers which follow outdated template instantiation rules
   require a class to have a comparison operator to exist when
   a QList of this type is instantiated. It's not actually
   used in the list, though. Hence the dummy implementation.
   Just in case other code relies on it we better trigger a warning
   mandating a real implementation.
*/

#ifdef Q_FULL_TEMPLATE_INSTANTIATION
#  define Q_DUMMY_COMPARISON_OPERATOR(C) \
    bool operator==(const C&) const { \
        qWarning(#C"::operator==(const "#C"&) was called"); \
        return false; \
    }
#else
#  define Q_DUMMY_COMPARISON_OPERATOR(C)
#endif


/*
   QTypeInfo     - type trait functionality
   qIsDetached   - data sharing functionality
*/

/*
  The catch-all template.
*/

template <typename T> inline bool qIsDetached(T &) { return true; }

template <typename T>
class QTypeInfo
{
public:
    enum {
        isPointer = false,
        isComplex = true,
        isStatic = true,
        isLarge = (sizeof(T)>sizeof(void*))
    };
};

template <typename T>
class QTypeInfo<T*>
{
public:
    enum {
        isPointer = true,
        isComplex = false,
        isStatic = false,
        isLarge = false
    };
};

/*
   Specialize a specific type with:

     Q_DECLARE_TYPEINFO(type, flags);

   where 'type' is the name of the type to specialize and 'flags' is
   logically-OR'ed combination of the flags below.
*/
enum { /* TYPEINFO flags */
    Q_COMPLEX_TYPE = 0,
    Q_PRIMITIVE_TYPE = 0x1,
    Q_STATIC_TYPE = 0,
    Q_MOVABLE_TYPE = 0x2
};

#define Q_DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
class QTypeInfo<TYPE > \
{ \
public: \
    enum { \
        isComplex = (((FLAGS) & Q_PRIMITIVE_TYPE) == 0), \
        isStatic = (((FLAGS) & (Q_MOVABLE_TYPE | Q_PRIMITIVE_TYPE)) == 0), \
        isLarge = (sizeof(TYPE)>sizeof(void*)), \
        isPointer = false \
    }; \
    static inline const char *name() { return #TYPE; } \
}

#define Q_DECLARE_TYPEINFO(TYPE, FLAGS) \
template<> \
Q_DECLARE_TYPEINFO_BODY(TYPE, FLAGS)



template <typename T>
inline void qSwap(T &value1, T &value2)
{
    std::swap(value1, value2);
}

/*
   Specialize a shared type with:

     Q_DECLARE_SHARED(type);

   where 'type' is the name of the type to specialize.  NOTE: shared
   types must declare a 'bool isDetached(void) const;' member for this
   to work.
*/
#define Q_DECLARE_SHARED(TYPE)                                          \
template <> inline bool qIsDetached<TYPE>(TYPE &t) { return t.isDetached(); } \
template <> inline void qSwap<TYPE>(TYPE &value1, TYPE &value2) \
{ qSwap(value1.data_ptr(), value2.data_ptr()); } \
QT_END_NAMESPACE \
namespace std { \
    template<> inline void swap<QT_PREPEND_NAMESPACE(TYPE)>(QT_PREPEND_NAMESPACE(TYPE) &value1, QT_PREPEND_NAMESPACE(TYPE) &value2) \
    { swap(value1.data_ptr(), value2.data_ptr()); } \
} \
QT_BEGIN_NAMESPACE

/*
   QTypeInfo primitive specializations
*/
Q_DECLARE_TYPEINFO(bool, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(char, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(signed char, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(uchar, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(short, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(ushort, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(int, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(uint, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(long, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(ulong, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(qint64, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(quint64, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(float, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(double, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(long double, Q_PRIMITIVE_TYPE);

/*
   These functions make it possible to use standard C++ functions with
   a similar name from Qt header files (especially template classes).
*/

class Q_CORE_EXPORT QFlag
{
    int i;
public:
    inline QFlag(int i);
    inline operator int() const { return i; }
};

inline QFlag::QFlag(int ai) : i(ai) {}

class Q_CORE_EXPORT QIncompatibleFlag
{
    int i;
public:
    inline explicit QIncompatibleFlag(int i);
    inline operator int() const { return i; }
};

inline QIncompatibleFlag::QIncompatibleFlag(int ai) : i(ai) {}


#ifndef Q_NO_TYPESAFE_FLAGS

template<typename Enum>
class QFlags
{
    typedef void **Zero;
    int i;
public:
    typedef Enum enum_type;
    Q_DECL_CONSTEXPR inline QFlags(const QFlags &f) : i(f.i) {}
    Q_DECL_CONSTEXPR inline QFlags(Enum f) : i(f) {}
    Q_DECL_CONSTEXPR inline QFlags(Zero = 0) : i(0) {}
    inline QFlags(QFlag f) : i(f) {}

    inline QFlags &operator=(const QFlags &f) { i = f.i; return *this; }
    inline QFlags &operator&=(int mask) { i &= mask; return *this; }
    inline QFlags &operator&=(uint mask) { i &= mask; return *this; }
    inline QFlags &operator|=(QFlags f) { i |= f.i; return *this; }
    inline QFlags &operator|=(Enum f) { i |= f; return *this; }
    inline QFlags &operator^=(QFlags f) { i ^= f.i; return *this; }
    inline QFlags &operator^=(Enum f) { i ^= f; return *this; }

    Q_DECL_CONSTEXPR  inline operator int() const { return i; }

    Q_DECL_CONSTEXPR inline QFlags operator|(QFlags f) const { return QFlags(Enum(i | f.i)); }
    Q_DECL_CONSTEXPR inline QFlags operator|(Enum f) const { return QFlags(Enum(i | f)); }
    Q_DECL_CONSTEXPR inline QFlags operator^(QFlags f) const { return QFlags(Enum(i ^ f.i)); }
    Q_DECL_CONSTEXPR inline QFlags operator^(Enum f) const { return QFlags(Enum(i ^ f)); }
    Q_DECL_CONSTEXPR inline QFlags operator&(int mask) const { return QFlags(Enum(i & mask)); }
    Q_DECL_CONSTEXPR inline QFlags operator&(uint mask) const { return QFlags(Enum(i & mask)); }
    Q_DECL_CONSTEXPR inline QFlags operator&(Enum f) const { return QFlags(Enum(i & f)); }
    Q_DECL_CONSTEXPR inline QFlags operator~() const { return QFlags(Enum(~i)); }

    Q_DECL_CONSTEXPR inline bool operator!() const { return !i; }

    inline bool testFlag(Enum f) const { return (i & f) == f && (f != 0 || i == int(f) ); }
};

#define Q_DECLARE_FLAGS(Flags, Enum)\
typedef QFlags<Enum> Flags;

#define Q_DECLARE_INCOMPATIBLE_FLAGS(Flags) \
inline QIncompatibleFlag operator|(Flags::enum_type f1, int f2) \
{ return QIncompatibleFlag(int(f1) | f2); }

#define Q_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) \
{ return QFlags<Flags::enum_type>(f1) | f2; } \
Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator|(Flags::enum_type f1, QFlags<Flags::enum_type> f2) \
{ return f2 | f1; } Q_DECLARE_INCOMPATIBLE_FLAGS(Flags)


#else /* Q_NO_TYPESAFE_FLAGS */

#define Q_DECLARE_FLAGS(Flags, Enum)\
typedef uint Flags;
#define Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)

#endif /* Q_NO_TYPESAFE_FLAGS */

#if (defined(Q_CC_GNU) || defined(Q_CC_CLANG)) && defined(QT_FOREACH_COMPAT)
/* make use of typeof-extension */
template <typename T>
class QForeachContainer {
public:
    inline QForeachContainer(const T& t) : c(t), brk(0), i(c.begin()), e(c.end()) { }
    const T c;
    int brk;
    typename T::const_iterator i, e;
};

#define Q_FOREACH(variable, container)                                \
for (QForeachContainer<__typeof__(container)> _container_(container); \
     !_container_.brk && _container_.i != _container_.e;              \
     __extension__  ({ ++_container_.brk; ++_container_.i; }))        \
    for (variable = *_container_.i;; __extension__ ({--_container_.brk; break;}))

#else

#define Q_FOREACH(variable, container) for (variable: container)

#endif

#define Q_FOREVER for(;;)
#ifndef QT_NO_KEYWORDS
#  ifndef foreach
#    define foreach Q_FOREACH
#  endif
#  ifndef forever
#    define forever Q_FOREVER
#  endif
#endif

#if defined(Q_CC_GNU) || defined(Q_CC_CLANG)
#define Q_UNREACHABLE_IMPL() __builtin_unreachable()
#else
#define Q_UNREACHABLE_IMPL()
#endif

#define Q_UNREACHABLE() \
    Q_ASSERT_X(false, "Q_UNREACHABLE()", "Q_UNREACHABLE was reached"); \
    Q_UNREACHABLE_IMPL()

template <typename T> static inline T *qGetPtrHelper(T *ptr) { return ptr; }
template <typename Wrapper> static inline typename Wrapper::pointer qGetPtrHelper(const Wrapper &p) { return p.data(); }

#define Q_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(qGetPtrHelper(d_ptr)); } \
    friend class Class##Private;

#define Q_DECLARE_PRIVATE_D(Dptr, Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(Dptr); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(Dptr); } \
    friend class Class##Private;

#define Q_DECLARE_PUBLIC(Class)                                    \
    inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
    friend class Class;

#define Q_D(Class) Class##Private * const d = d_func()
#define Q_Q(Class) Class * const q = q_func()

#define QT_TR_NOOP(x) (x)
#define QT_TR_NOOP_UTF8(x) (x)
#define QT_TRANSLATE_NOOP(scope, x) (x)
#define QT_TRANSLATE_NOOP_UTF8(scope, x) (x)
#define QT_TRANSLATE_NOOP3(scope, x, comment) {x, comment}
#define QT_TRANSLATE_NOOP3_UTF8(scope, x, comment) {x, comment}

#define QDOC_PROPERTY(text)

/*
   When RTTI is not available, define this macro to force any uses of
   dynamic_cast to cause a compile failure.
*/

#ifdef QT_NO_DYNAMIC_CAST
#  define dynamic_cast QT_PREPEND_NAMESPACE(qt_dynamic_cast_check)

  template<typename T, typename X>
  T qt_dynamic_cast_check(X, T* = 0)
  { return T::dynamic_cast_will_always_fail_because_rtti_is_disabled; }
#endif

/*
   Some classes do not permit copies to be made of an object. These
   classes contains a private copy constructor and assignment
   operator to disable copying (the compiler gives an error message).
*/
#define Q_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

class QByteArray;
Q_CORE_EXPORT QByteArray qgetenv(const char *varName);
Q_CORE_EXPORT bool qputenv(const char *varName, const QByteArray& value);

inline int qIntCast(double f) { return int(f); }
inline int qIntCast(float f) { return int(f); }

/*
  Reentrant versions of basic rand() functions for random number generation
*/
Q_CORE_EXPORT void qsrand(uint seed);
Q_CORE_EXPORT int qrand();

#ifdef QT_NO_CONCURRENT
#  define QT_NO_QFUTURE
#endif

#if defined (__ELF__)
#  if defined (Q_OS_LINUX) || defined (Q_OS_SOLARIS) || defined (Q_OS_FREEBSD) || defined (Q_OS_OPENBSD)
#    define Q_OF_ELF
#  endif
#endif

QT_END_NAMESPACE
QT_END_HEADER

#endif /* __cplusplus */

#endif /* QGLOBAL_H */
