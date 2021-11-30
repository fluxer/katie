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

#ifndef QGLOBAL_H
#define QGLOBAL_H

#include <qconfig.h>
#include <stddef.h>

/*
   can be used like #if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
*/
#define QT_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

// Compatibility, used to be conditional
#define Q_WS_X11
#define Q_OS_UNIX

#include <utility> // std::swap
#include <cstdint> // std::uintptr_t

#if !defined(QT_NO_USING_NAMESPACE)

# define QT_NAMESPACE Katie
# define QT_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::name
# define QT_USE_NAMESPACE using namespace ::QT_NAMESPACE;

# define QT_BEGIN_NAMESPACE namespace QT_NAMESPACE {
# define QT_END_NAMESPACE }
# define QT_BEGIN_INCLUDE_NAMESPACE }
# define QT_END_INCLUDE_NAMESPACE namespace QT_NAMESPACE {

namespace QT_NAMESPACE {}

# ifdef QT_NAMESPACE_COMPAT
QT_USE_NAMESPACE
# endif

#else /* QT_NO_USING_NAMESPACE */

# define QT_NAMESPACE
# define QT_PREPEND_NAMESPACE(name) ::name
# define QT_USE_NAMESPACE

# define QT_BEGIN_NAMESPACE
# define QT_END_NAMESPACE
# define QT_BEGIN_INCLUDE_NAMESPACE
# define QT_END_INCLUDE_NAMESPACE

#endif /* QT_NO_USING_NAMESPACE */

#define QT_BEGIN_INCLUDE_HEADER
#define QT_END_INCLUDE_HEADER extern "C++"

/*
   The compiler, must be one of: (Q_CC_x)

     GNU      - GNU C++
     CLANG    - C++ front-end for the LLVM compiler

   Should be sorted most to least authoritative.

   Paper             Macro                             SD-6 macro
   N2672             Q_COMPILER_INITIALIZER_LISTS
   N2118 N2844 N3053 Q_COMPILER_RVALUE_REFS            __cpp_rvalue_references = 200610

  For any future version of the C++ standard, we use only the SD-6 macro.
  For full listing, see
   http://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
*/
#if defined(__GNUC__)
#  define Q_CC_GNU
#  if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
     /* C++0x features supported in GCC 4.3: */
#    define Q_COMPILER_RVALUE_REFS
     /* C++0x features supported in GCC 4.4: */
#    define Q_COMPILER_INITIALIZER_LISTS
     /* C++0x features supported in GCC 4.6: */
#    ifdef __EXCEPTIONS
#      define Q_COMPILER_EXCEPTIONS
#    endif
#  endif

#elif defined(__clang__)
#  define Q_CC_CLANG
#  if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    /* Detect C++ features using __has_feature(), see http://clang.llvm.org/docs/LanguageExtensions.html#cxx11 */
#    if __has_feature(cxx_generalized_initializers)
#      define Q_COMPILER_INITIALIZER_LISTS
#    endif
#    if __has_feature(cxx_rvalue_references)
#      define Q_COMPILER_RVALUE_REFS
#    endif
#    if __has_feature(cxx_exceptions)
#      define Q_COMPILER_EXCEPTIONS
#    endif
#  endif

#else
#  error "Katie has not been tested with this compiler"
#endif

#define Q_C_CALLBACKS
#define Q_ALIGNOF(type)   __alignof__(type)
#define Q_TYPEOF(expr)    __typeof__(expr)
#define Q_DECL_ALIGN(n)   __attribute__((__aligned__(n)))
#define Q_REQUIRED_RESULT __attribute__((warn_unused_result))
#define Q_LIKELY(expr)    __builtin_expect(!!(expr), true)
#define Q_UNLIKELY(expr)  __builtin_expect(!!(expr), false)
#define Q_FUNC_INFO       __PRETTY_FUNCTION__
#define Q_PACKED          __attribute__((__packed__))
#define Q_OUTOFLINE_TEMPLATE
#define Q_INLINE_TEMPLATE inline

#define Q_CONSTRUCTOR_FUNCTION(AFUNC) \
    static const int __init_variable__ ## AFUNC = AFUNC();

#define Q_DESTRUCTOR_FUNCTION0(AFUNC) \
    class AFUNC ## __dest_class__ { \
    public: \
       inline AFUNC ## __dest_class__() { } \
       inline ~ AFUNC ## __dest_class__() { AFUNC(); } \
    } AFUNC ## __dest_instance__;
#define Q_DESTRUCTOR_FUNCTION(AFUNC) Q_DESTRUCTOR_FUNCTION0(AFUNC)

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

#define Q_INIT_RESOURCE(name) \
    extern int qInitResources_ ## name (); \
    qInitResources_ ## name ();
#define Q_CLEANUP_RESOURCE(name) \
    extern int qCleanupResources_ ## name (); \
    qCleanupResources_ ## name ();

/*
   Useful type definitions for Qt
*/
typedef std::uintptr_t quintptr;
typedef std::ptrdiff_t qptrdiff;

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
#else
#  define Q_DECL_DEPRECATED __attribute__((__deprecated__))
#endif

#if defined(QT_NO_DEPRECATED)
#  undef QT_DEPRECATED
#  undef QT_DEPRECATED_CONSTRUCTOR
#elif defined(QT_DEPRECATED_WARNINGS)
#  undef QT_DEPRECATED
#  define QT_DEPRECATED Q_DECL_DEPRECATED
#  undef QT_DEPRECATED_CONSTRUCTOR
#  define QT_DEPRECATED_CONSTRUCTOR explicit Q_DECL_DEPRECATED
#else
#  undef QT_DEPRECATED
#  define QT_DEPRECATED
#  undef QT_DEPRECATED_CONSTRUCTOR
#  define QT_DEPRECATED_CONSTRUCTOR
#endif

#ifdef QT_ASCII_CAST_WARNINGS
#  define QT_ASCII_CAST_WARN Q_DECL_DEPRECATED
#  define QT_ASCII_CAST_WARN_CONSTRUCTOR QT_DEPRECATED_CONSTRUCTOR
#else
#  define QT_ASCII_CAST_WARN
#  define QT_ASCII_CAST_WARN_CONSTRUCTOR
#endif

#if defined(QT_ARCH_I386)
#  define QT_FASTCALL __attribute__((regparm(3)))
#else
#  define QT_FASTCALL __attribute__((hot))
#endif

// This logic must match the one in qmetatype.h
#if !defined(QT_NO_FPU) && defined(QT_ARCH_ARM) || defined(QT_ARCH_AVR32) || defined(QT_ARCH_SH)
#  define QT_NO_FPU
#endif

#if defined(QT_NO_FPU)
typedef float qreal;
#else
typedef double qreal;
#endif

#define Q_DECL_EXPORT __attribute__((visibility("default")))
#define Q_DECL_HIDDEN __attribute__((visibility("hidden")))
#define Q_DECL_IMPORT

#ifdef KtCore_EXPORTS
#  define Q_CORE_EXPORT Q_DECL_EXPORT
#else
#  define Q_CORE_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtGui_EXPORTS
#  define Q_GUI_EXPORT Q_DECL_EXPORT
#else
#  define Q_GUI_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtNetwork_EXPORTS
#  define Q_NETWORK_EXPORT Q_DECL_EXPORT
#else
#  define Q_NETWORK_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtSvg_EXPORTS
#  define Q_SVG_EXPORT Q_DECL_EXPORT
#else
#  define Q_SVG_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtTest_EXPORTS
#  define Q_TEST_EXPORT Q_DECL_EXPORT
#else
#  define Q_TEST_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtDeclarative_EXPORTS
#  define Q_DECLARATIVE_EXPORT Q_DECL_EXPORT
#else
#  define Q_DECLARATIVE_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtXml_EXPORTS
#  define Q_XML_EXPORT Q_DECL_EXPORT
#else
#  define Q_XML_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtScript_EXPORTS
#  define Q_SCRIPT_EXPORT Q_DECL_EXPORT
#else
#  define Q_SCRIPT_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtScriptTools_EXPORTS
#  define Q_SCRIPTTOOLS_EXPORT Q_DECL_EXPORT
#else
#  define Q_SCRIPTTOOLS_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtDBus_EXPORTS
#  define Q_DBUS_EXPORT Q_DECL_EXPORT
#else
#  define Q_DBUS_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtUiTools_EXPORTS
#  define Q_UITOOLS_EXPORT Q_DECL_EXPORT
#else
#  define Q_UITOOLS_EXPORT Q_DECL_IMPORT
#endif

#ifdef KtDesigner_EXPORTS
#  define Q_DESIGNER_EXPORT Q_DECL_EXPORT
#else
#  define Q_DESIGNER_EXPORT Q_DECL_IMPORT
#endif

#define Q_CORE_EXPORT_INLINE Q_CORE_EXPORT inline
#define Q_GUI_EXPORT_INLINE Q_GUI_EXPORT inline

/*
   No, this is not an evil backdoor. QT_BUILD_INTERNAL just exports more symbols
   for Qt's internal unit tests. If you want slower loading times and more
   symbols that can vanish from version to version, feel free to define QT_BUILD_INTERNAL.
*/
#if defined(QT_BUILD_INTERNAL)
#    define Q_AUTOTEST_EXPORT Q_DECL_EXPORT
#else
#    define Q_AUTOTEST_EXPORT
#endif

/* These wrap try/catch so we can switch off exceptions later.

   Beware - do not use the exception instance in the catch block.
   If you can't live with that constraint, don't use these macros.
   Use the QT_NO_EXCEPTIONS macro to protect your code instead.
*/
#if !defined(QT_NO_EXCEPTIONS) && !defined(Q_COMPILER_EXCEPTIONS) && !defined(Q_MOC_RUN)
#  define QT_NO_EXCEPTIONS
#endif

#ifdef QT_NO_EXCEPTIONS
#  define QT_TRY if (true)
#  define QT_CATCH(A) else if (false)
#  define QT_THROW(A) {}
#  define QT_RETHROW {}
#else
#  define QT_TRY try
#  define QT_CATCH(A) catch (A)
#  define QT_THROW(A) throw A
#  define QT_RETHROW throw
#endif

Q_CORE_EXPORT const char *qVersion();

/*
   Avoid "unused parameter" warnings
*/
#define Q_UNUSED(x) (void)x;

class QString;
#define qPrintable(string) QString(string).toLocal8Bit().constData()

Q_CORE_EXPORT QString qt_error_string(int errorCode);

Q_CORE_EXPORT void qDebug(const char *, ...) /* print debug message */
#if !defined(__INSURE__)
    __attribute__((format (printf, 1, 2)))
#endif
;

Q_CORE_EXPORT void qWarning(const char *, ...) /* print warning message */
#if !defined(__INSURE__)
    __attribute__((format (printf, 1, 2)))
#endif
;

Q_CORE_EXPORT void qCritical(const char *, ...) /* print critical message */
#if !defined(__INSURE__)
    __attribute__((format (printf, 1, 2)))
#endif
;
Q_CORE_EXPORT void qFatal(const char *, ...) /* print fatal message and exit */
#if !defined(__INSURE__)
    __attribute__((format (printf, 1, 2)))
#endif
;

Q_CORE_EXPORT void qt_assert(const char *assertion, const char *file, int line);
Q_CORE_EXPORT void qt_assert_x(const char *where, const char *what, const char *file, int line);
Q_CORE_EXPORT void qt_check_pointer(const char *file, int line);

#ifndef QT_NO_DEBUG
#  define Q_ASSERT(cond) do { if(!(cond)) qt_assert(#cond,__FILE__,__LINE__); } while (0)
#  define Q_ASSERT_X(cond, where, what) do { if(!(cond)) qt_assert_x(where, what,__FILE__,__LINE__); } while (0)
#  define Q_CHECK_PTR(p) do { if(!p) qt_check_pointer(__FILE__,__LINE__); } while (0)
#else
#  define Q_ASSERT(cond)
#  define Q_ASSERT_X(cond, where, what)
#  define Q_CHECK_PTR(p)
#endif

enum QtMsgType { QtDebugMsg, QtWarningMsg, QtCriticalMsg, QtFatalMsg };

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
    inline ~QGlobalStatic() { pointer = nullptr; }
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
        static QGlobalStatic<TYPE > thisGlobalStatic(nullptr);     \
        if (!thisGlobalStatic.pointer) {                             \
            TYPE *x = thisGlobalStatic.pointer = &thisVariable;      \
            INITIALIZER;                                             \
        }                                                            \
        return thisGlobalStatic.pointer;                             \
    }

#else // QT_NO_THREAD

// Forward declaration, since qatomic.h needs qglobal.h
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
        globalStatic.pointer = nullptr;
        globalStatic.destroyed = true;
    }
};

#define Q_GLOBAL_STATIC(TYPE, NAME)                                           \
    static TYPE *NAME()                                                       \
    {                                                                         \
        static QGlobalStatic<TYPE > this__StaticVar_                          \
            = { QAtomicPointer<TYPE>(new TYPE), false };                      \
        static QGlobalStaticDeleter<TYPE > cleanup(this__StaticVar_);         \
        return this__StaticVar_.pointer;                                      \
    }

#define Q_GLOBAL_STATIC_WITH_ARGS(TYPE, NAME, ARGS)                           \
    static TYPE *NAME()                                                       \
    {                                                                         \
        static QGlobalStatic<TYPE > this__StaticVar_                          \
            = { QAtomicPointer<TYPE>(new TYPE ARGS), false };                 \
        static QGlobalStaticDeleter<TYPE > cleanup(this__StaticVar_);         \
        return this__StaticVar_.pointer;                                      \
    }

#define Q_GLOBAL_STATIC_WITH_INITIALIZER(TYPE, NAME, INITIALIZER)                  \
    static TYPE *NAME()                                                            \
    {                                                                              \
        static QGlobalStatic<TYPE > this__StaticVar_                               \
            = { QAtomicPointer<TYPE>(nullptr), false };                            \
        if (!this__StaticVar_.pointer && !this__StaticVar_.destroyed) {            \
            QScopedPointer<TYPE > x(new TYPE);                                     \
            INITIALIZER;                                                           \
            if (this__StaticVar_.pointer.testAndSetOrdered(nullptr, x.data())) {   \
                static QGlobalStaticDeleter<TYPE > cleanup(this__StaticVar_);      \
                x.take();                                                          \
            }                                                                      \
        }                                                                          \
        return this__StaticVar_.pointer;                                           \
    }

#endif // QT_NO_THREAD


/*
   Utility macros and inline functions
*/
template <typename T>
constexpr inline T qAbs(const T &t)
{
    return t >= 0 ? t : -t;
}

template <typename T>
constexpr inline const T &qMin(const T &a, const T &b)
{
    return (a < b) ? a : b;
}

template <typename T>
constexpr inline const T &qMax(const T &a, const T &b)
{
    return (a < b) ? b : a;
}

template <typename T>
constexpr inline const T &qBound(const T &min, const T &val, const T &max)
{
    return qMax(min, qMin(max, val));
}

constexpr static inline bool qFuzzyCompare(double p1, double p2)
{
    return (qAbs(p1 - p2) <= 0.000000000001 * qMin(qAbs(p1), qAbs(p2)));
}

constexpr static inline bool qFuzzyCompare(float p1, float p2)
{
    return (qAbs(p1 - p2) <= 0.00001f * qMin(qAbs(p1), qAbs(p2)));
}

/*!
  \internal
*/
constexpr static inline bool qFuzzyIsNull(double d)
{
    return qAbs(d) <= 0.000000000001;
}

/*!
  \internal
*/
constexpr static inline bool qFuzzyIsNull(float f)
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
   QTypeInfo     - type trait functionality
*/
template <typename T>
class QTypeInfo
{
public:
    enum {
        isComplex = true,
        isStatic = true,
        isLarge = (sizeof(T) > QT_POINTER_SIZE)
    };
};

template <typename T>
class QTypeInfo<T*>
{
public:
    enum {
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
        isLarge = (sizeof(TYPE) > QT_POINTER_SIZE), \
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
    inline QFlag(int ai) : i(ai) {}
    inline operator int() const { return i; }
};

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
    constexpr inline QFlags(const QFlags &f) : i(f.i) {}
    constexpr inline QFlags(Enum f) : i(f) {}
    constexpr inline QFlags(Zero = 0) : i(0) {}
    inline QFlags(QFlag f) : i(f) {}

    inline QFlags &operator=(const QFlags &f) { i = f.i; return *this; }
    inline QFlags &operator&=(int mask) { i &= mask; return *this; }
    inline QFlags &operator&=(uint mask) { i &= mask; return *this; }
    inline QFlags &operator|=(QFlags f) { i |= f.i; return *this; }
    inline QFlags &operator|=(Enum f) { i |= f; return *this; }
    inline QFlags &operator^=(QFlags f) { i ^= f.i; return *this; }
    inline QFlags &operator^=(Enum f) { i ^= f; return *this; }

    constexpr  inline operator int() const { return i; }

    constexpr inline QFlags operator|(QFlags f) const { return QFlags(Enum(i | f.i)); }
    constexpr inline QFlags operator|(Enum f) const { return QFlags(Enum(i | f)); }
    constexpr inline QFlags operator^(QFlags f) const { return QFlags(Enum(i ^ f.i)); }
    constexpr inline QFlags operator^(Enum f) const { return QFlags(Enum(i ^ f)); }
    constexpr inline QFlags operator&(int mask) const { return QFlags(Enum(i & mask)); }
    constexpr inline QFlags operator&(uint mask) const { return QFlags(Enum(i & mask)); }
    constexpr inline QFlags operator&(Enum f) const { return QFlags(Enum(i & f)); }
    constexpr inline QFlags operator~() const { return QFlags(Enum(~i)); }

    constexpr inline bool operator!() const { return !i; }

    inline bool testFlag(Enum f) const { return (i & f) == f && (f != 0 || i == int(f) ); }
};

#define Q_DECLARE_FLAGS(Flags, Enum)\
typedef QFlags<Enum> Flags;

#define Q_DECLARE_INCOMPATIBLE_FLAGS(Flags) \
inline QIncompatibleFlag operator|(Flags::enum_type f1, int f2) \
{ return QIncompatibleFlag(int(f1) | f2); }

#define Q_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
constexpr inline QFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) \
{ return QFlags<Flags::enum_type>(f1) | f2; } \
constexpr inline QFlags<Flags::enum_type> operator|(Flags::enum_type f1, QFlags<Flags::enum_type> f2) \
{ return f2 | f1; } Q_DECLARE_INCOMPATIBLE_FLAGS(Flags)


#else /* Q_NO_TYPESAFE_FLAGS */

#define Q_DECLARE_FLAGS(Flags, Enum)\
typedef uint Flags;
#define Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)

#endif /* Q_NO_TYPESAFE_FLAGS */

#ifdef QT_FOREACH_COMPAT
template <typename T>
class QForeachContainer {
public:
    inline QForeachContainer(const T& t) : c(t) { }
    inline typename T::const_iterator begin() { return c.begin(); }
    inline typename T::const_iterator end() { return c.end(); }
private:
    const T c;
};

#define Q_FOREACH(variable, container) \
    for (variable: QForeachContainer<Q_TYPEOF(container)>(container))

#else // QT_FOREACH_COMPAT

#define Q_FOREACH(variable, container) for (variable: container)

#endif // QT_FOREACH_COMPAT

#define Q_FOREVER for(;;)
#ifndef QT_NO_KEYWORDS
#  ifndef foreach
#    define foreach Q_FOREACH
#  endif
#  ifndef forever
#    define forever Q_FOREVER
#  endif
#endif

#define Q_UNREACHABLE() \
    Q_ASSERT_X(false, "Q_UNREACHABLE()", "Q_UNREACHABLE was reached"); \
    __builtin_unreachable()

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
    Class(const Class &) = delete; \
    Class &operator=(const Class &) = delete;

class QByteArray;
Q_CORE_EXPORT QByteArray qgetenv(const char *varName);
Q_CORE_EXPORT bool qputenv(const char *varName, const QByteArray& value);

/*
  Reentrant versions of basic rand() functions for random number generation
*/
Q_CORE_EXPORT void qsrand(uint seed);
Q_CORE_EXPORT int qrand();

QT_END_NAMESPACE

#endif /* QGLOBAL_H */
