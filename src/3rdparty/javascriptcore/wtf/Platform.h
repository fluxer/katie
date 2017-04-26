/*
 * Copyright (C) 2006, 2007, 2008, 2009 Apple Inc.  All rights reserved.
 * Copyright (C) 2007-2009 Torch Mobile, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WTF_Platform_h
#define WTF_Platform_h

#include <qglobal.h>

QT_USE_NAMESPACE

/* ==== PLATFORM handles OS, operating environment, graphics API, and
   CPU. This macro will be phased out in favor of platform adaptation
   macros, policy decision macros, and top-level port definitions. ==== */
#define PLATFORM(WTF_FEATURE) (defined WTF_PLATFORM_##WTF_FEATURE  && WTF_PLATFORM_##WTF_FEATURE)


/* ==== Platform adaptation macros: these describe properties of the target environment. ==== */

/* COMPILER() - the compiler being used to build the project */
#define COMPILER(WTF_FEATURE) (defined WTF_COMPILER_##WTF_FEATURE  && WTF_COMPILER_##WTF_FEATURE)
/* COMPILER_SUPPORTS() - whether the compiler being used to build the project supports the given feature. */
#define COMPILER_SUPPORTS(WTF_COMPILER_FEATURE) (defined WTF_COMPILER_SUPPORTS_##WTF_COMPILER_FEATURE  && WTF_COMPILER_SUPPORTS_##WTF_COMPILER_FEATURE)

/* CPU() - the target CPU architecture */
#define CPU(WTF_FEATURE) (defined WTF_CPU_##WTF_FEATURE  && WTF_CPU_##WTF_FEATURE)
/* HAVE() - specific system features (headers, functions or similar) that are present or not */
#define HAVE(WTF_FEATURE) (defined HAVE_##WTF_FEATURE  && HAVE_##WTF_FEATURE)
/* OS() - underlying operating system; only to be used for mandated low-level services like 
   virtual memory, not to choose a GUI toolkit */
#define OS(WTF_FEATURE) (defined WTF_OS_##WTF_FEATURE  && WTF_OS_##WTF_FEATURE)


/* ==== Policy decision macros: these define policy choices for a particular port. ==== */

/* USE() - use a particular third-party library or optional OS service */
#define USE(WTF_FEATURE) (defined WTF_USE_##WTF_FEATURE  && WTF_USE_##WTF_FEATURE)
/* ENABLE() - turn on a specific feature of WebKit */
#define ENABLE(WTF_FEATURE) (defined ENABLE_##WTF_FEATURE  && ENABLE_##WTF_FEATURE)



/* ==== COMPILER() - the compiler being used to build the project ==== */

/* COMPILER(CLANG) - Clang  */
#if defined(__clang__)
#define WTF_COMPILER_CLANG 1

#define WTF_COMPILER_SUPPORTS_C_STATIC_ASSERT __has_feature(c_static_assert)
#define WTF_COMPILER_SUPPORTS_CXX_STATIC_ASSERT __has_feature(cxx_static_assert)
#endif

/* COMPILER(GCC) - GNU Compiler Collection */
#if defined(__GNUC__)
#define WTF_COMPILER_GCC 1
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define GCC_VERSION_AT_LEAST(major, minor, patch) (GCC_VERSION >= (major * 10000 + minor * 100 + patch))
#else
/* Define this for !GCC compilers, just so we can write things like GCC_VERSION_AT_LEAST(4, 1, 0). */
#define GCC_VERSION_AT_LEAST(major, minor, patch) 0
#endif

/* Specific compiler features */
#if COMPILER(GCC) && !COMPILER(CLANG)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
/* C11 support */
#define WTF_COMPILER_SUPPORTS_C_STATIC_ASSERT 1
#endif
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (defined(__cplusplus) && __cplusplus >= 201103L)
/* C++11 support */
#if GCC_VERSION_AT_LEAST(4, 3, 0)
#define WTF_COMPILER_SUPPORTS_CXX_STATIC_ASSERT 1
#endif
#endif /* defined(__GXX_EXPERIMENTAL_CXX0X__) || (defined(__cplusplus) && __cplusplus >= 201103L) */
#endif /* COMPILER(GCC) */

/* COMPILER(MINGW) - MinGW GCC */
/* COMPILER(MINGW64) - mingw-w64 GCC - only used as additional check to exclude mingw.org specific functions */
#if defined(__MINGW32__)
#define WTF_COMPILER_MINGW 1
#include <_mingw.h> /* private MinGW header */
    #if defined(__MINGW64_VERSION_MAJOR) /* best way to check for mingw-w64 vs mingw.org */
        #define WTF_COMPILER_MINGW64 1
    #endif /* __MINGW64_VERSION_MAJOR */
#endif /* __MINGW32__ */

/* COMPILER(SUNCC) - Sun CC compiler, also known as Sun Studio or Sun Pro */
#if defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#define WTF_COMPILER_SUNCC 1
#endif

/* COMPILER(INTEL) - Intel C++ Compiler */
#if defined(__INTEL_COMPILER)
#define WTF_COMPILER_INTEL 1
#endif

/* COMPILER(ACC) - HP aCC */
#if defined(__HP_aCC)
#define WTF_COMPILER_ACC 1
#endif

/* COMPILER(XLC) - IBM XL */
#if defined(__xlC__)
#define WTF_COMPILER_XLC 1
#endif


/* ==== CPU() - the target CPU architecture ==== */

/* This also defines CPU(BIG_ENDIAN) or CPU(MIDDLE_ENDIAN) or neither, as appropriate. */

/* CPU(ALPHA) - DEC Alpha */
#if defined(__alpha__)
#define WTF_CPU_ALPHA 1
#endif

/* CPU(IA64) - Itanium / IA-64 */
#if defined(__ia64__) || defined(__ia64) || defined(_M_IA64)
#define WTF_CPU_IA64 1
/* 32-bit mode on Itanium */
#if !defined(__LP64__)
#define WTF_CPU_IA64_32 1
#endif
/* Itanium can be both big- and little-endian;
   we need to determine at compile time which one it is.
    - HP's aCC compiler only compiles big-endian (so HP-UXi is always big-endian)
    - GCC defines __BIG_ENDIAN__ for us (default on HP-UX)
    - Linux is usually little-endian
    - I've never seen AIX or Windows on IA-64, but they should be little-endian too
*/
#if defined(__BIG_ENDIAN__) || defined(__HP_aCC)
# define WTF_CPU_BIG_ENDIAN 1
#endif
#endif

/* CPU(HPPA) - a.k.a. PA-RISC */
#if defined(__hppa) || defined(__hppa__)
#define WTF_CPU_HPPA 1
#define WTF_CPU_BIG_ENDIAN 1
#endif

/* CPU(PPC) - PowerPC 32-bit */
#if   defined(__ppc__)     \
    || defined(__PPC__)     \
    || defined(__powerpc__) \
    || defined(__powerpc)   \
    || defined(__POWERPC__) \
    || defined(_M_PPC)      \
    || defined(__PPC)
#define WTF_CPU_PPC 1
#define WTF_CPU_BIG_ENDIAN 1
#endif

/* CPU(PPC64) - PowerPC 64-bit */
#if   defined(__ppc64__) \
    || defined(__PPC64__)
#define WTF_CPU_PPC64 1
#define WTF_CPU_BIG_ENDIAN 1
#endif

/* CPU(SH4) - SuperH SH-4 */
#if defined(__SH4__)
#define WTF_CPU_SH4 1
#endif

/* CPU(SPARC32) - SPARC 32-bit */
#if defined(__sparc) && !defined(__arch64__) || defined(__sparcv8)
#define WTF_CPU_SPARC32 1
#define WTF_CPU_BIG_ENDIAN 1
#endif

/* CPU(SPARC64) - SPARC 64-bit */
#if defined(__sparc__) && defined(__arch64__) || defined (__sparcv9)
#define WTF_CPU_SPARC64 1
#define WTF_CPU_BIG_ENDIAN 1
#endif

/* CPU(SPARC) - any SPARC, true for CPU(SPARC32) and CPU(SPARC64) */
#if CPU(SPARC32) || CPU(SPARC64)
#define WTF_CPU_SPARC 1
#endif

/* CPU(X86) - i386 / x86 32-bit */
#if   defined(__i386__) \
    || defined(i386)     \
    || defined(_M_IX86)  \
    || defined(_X86_)    \
    || defined(__THW_INTEL)
#define WTF_CPU_X86 1
#endif

/* CPU(X86_64) - AMD64 / Intel64 / x86_64 64-bit */
#if   defined(__x86_64__) \
    || defined(_M_X64)
#define WTF_CPU_X86_64 1
#endif

/* 64-bit mode on AIX */
#ifdef __64BIT__
#define WTF_CPU_AIX64 1
#endif

/* CPU(AARCH64) - AArch64 */
#if defined(__aarch64__)
#define WTF_CPU_AARCH64 1
#if defined(__AARCH64EB__)
#define WTF_CPU_BIG_ENDIAN 1
#endif
#endif

/* CPU(ARM) - ARM, any version*/
#if   defined(arm) \
    || defined(__arm__) \
    || defined(__MARM__)
#define WTF_CPU_ARM 1

#if defined(__ARMEB__)
#define WTF_CPU_BIG_ENDIAN 1

#elif !defined(__ARM_EABI__) \
    && !defined(__EABI__) \
    && !defined(__VFP_FP__) \
    && !defined(ANDROID)
#define WTF_CPU_MIDDLE_ENDIAN 1

#endif

#define WTF_ARM_ARCH_AT_LEAST(N) (CPU(ARM) && WTF_ARM_ARCH_VERSION >= N)

/* Set WTF_ARM_ARCH_VERSION */
#if   defined(__ARM_ARCH_4__) \
    || defined(__ARM_ARCH_4T__) \
    || defined(__MARM_ARMV4__) \
    || defined(_ARMV4I_)
#define WTF_ARM_ARCH_VERSION 4

#elif defined(__ARM_ARCH_5__) \
    || defined(__ARM_ARCH_5T__) \
    || defined(__ARM_ARCH_5E__) \
    || defined(__ARM_ARCH_5TE__) \
    || defined(__ARM_ARCH_5TEJ__) \
    || defined(__MARM_ARMV5__)
#define WTF_ARM_ARCH_VERSION 5

#elif defined(__ARM_ARCH_6__) \
    || defined(__ARM_ARCH_6J__) \
    || defined(__ARM_ARCH_6K__) \
    || defined(__ARM_ARCH_6Z__) \
    || defined(__ARM_ARCH_6ZK__) \
    || defined(__ARM_ARCH_6T2__) \
    || defined(__ARMV6__)
#define WTF_ARM_ARCH_VERSION 6

#elif defined(__ARM_ARCH_7A__) \
    || defined(__ARM_ARCH_7R__)
#define WTF_ARM_ARCH_VERSION 7

#else
#define WTF_ARM_ARCH_VERSION 0

#endif

/* Set WTF_THUMB_ARCH_VERSION */
#if   defined(__ARM_ARCH_4T__)
#define WTF_THUMB_ARCH_VERSION 1

#elif defined(__ARM_ARCH_5T__) \
    || defined(__ARM_ARCH_5TE__) \
    || defined(__ARM_ARCH_5TEJ__)
#define WTF_THUMB_ARCH_VERSION 2

#elif defined(__ARM_ARCH_6J__) \
    || defined(__ARM_ARCH_6K__) \
    || defined(__ARM_ARCH_6Z__) \
    || defined(__ARM_ARCH_6ZK__) \
    || defined(__ARM_ARCH_6M__)
#define WTF_THUMB_ARCH_VERSION 3

#elif defined(__ARM_ARCH_6T2__) \
    || defined(__ARM_ARCH_7__) \
    || defined(__ARM_ARCH_7A__) \
    || defined(__ARM_ARCH_7R__) \
    || defined(__ARM_ARCH_7M__)
#define WTF_THUMB_ARCH_VERSION 4

#else
#define WTF_THUMB_ARCH_VERSION 0
#endif


/* CPU(ARMV5_OR_LOWER) - ARM instruction set v5 or earlier */
/* On ARMv5 and below the natural alignment is required. 
   And there are some other differences for v5 or earlier. */
#if !defined(ARMV5_OR_LOWER) && !WTF_ARM_ARCH_AT_LEAST(6)
#define WTF_CPU_ARMV5_OR_LOWER 1
#endif


/* CPU(ARM_TRADITIONAL) - Thumb2 is not available, only traditional ARM (v4 or greater) */
/* CPU(ARM_THUMB2) - Thumb2 instruction set is available */
/* Only one of these will be defined. */
#if !defined(WTF_CPU_ARM_TRADITIONAL) && !defined(WTF_CPU_ARM_THUMB2)
#  if defined(thumb2) || defined(__thumb2__) \
    || ((defined(__thumb) || defined(__thumb__)) && WTF_THUMB_ARCH_VERSION == 4)
#    define WTF_CPU_ARM_TRADITIONAL 0
#    define WTF_CPU_ARM_THUMB2 1
#  elif WTF_ARM_ARCH_AT_LEAST(4)
#    define WTF_CPU_ARM_TRADITIONAL 1
#    define WTF_CPU_ARM_THUMB2 0
#  else
#    error "Not supported ARM architecture"
#  endif
#elif CPU(ARM_TRADITIONAL) && CPU(ARM_THUMB2) /* Sanity Check */
#  error "Cannot use both of WTF_CPU_ARM_TRADITIONAL and WTF_CPU_ARM_THUMB2 platforms"
#endif /* !defined(WTF_CPU_ARM_TRADITIONAL) && !defined(WTF_CPU_ARM_THUMB2) */

#endif /* ARM */

/* CPU(MIPS) - MIPS, any version */
#if (defined(mips) || defined(__mips__) || defined(MIPS) || defined(_MIPS_))
#define WTF_CPU_MIPS 1
#include <sgidefs.h>
#if defined(__MIPSEB__)
#define WTF_CPU_BIG_ENDIAN 1
#endif
/* CPU(MIPS64) - MIPS 64-bit both BIG and LITTLE endian */
#if defined(_MIPS_SIM_ABI64) && (_MIPS_SIM == _MIPS_SIM_ABI64)
#define WTF_CPU_MIPS64 1
#endif

/* CPU(MIPSN32) - MIPS N32 ABI both BIG and LITTLE endian */
#if defined(_MIPS_SIM_ABIN32) && (_MIPS_SIM == _MIPS_SIM_ABIN32)
#define WTF_CPU_MIPSN32 1
#endif

/* CPU(MIPS32) - MIPS O32 ABI both BIG and LITTLE endian */
#if defined(_MIPS_SIM_ABI32) && (_MIPS_SIM == _MIPS_SIM_ABI32)
#define WTF_CPU_MIPS32 1
#endif
#endif /* __mips__ */


/* ==== OS() - underlying operating system; only to be used for mandated low-level services like 
   virtual memory, not to choose a GUI toolkit ==== */

/* OS(ANDROID) - Android */
#ifdef ANDROID
#define WTF_OS_ANDROID 1
#endif

/* OS(AIX) - AIX */
#ifdef _AIX
#define WTF_OS_AIX 1
#endif

/* OS(FREEBSD) - FreeBSD */
#ifdef __FreeBSD__
#define WTF_OS_FREEBSD 1
#endif

/* OS(HAIKU) - Haiku */
#ifdef __HAIKU__
#define WTF_OS_HAIKU 1
#endif

/* OS(HPUX) - HP-UX */
#if defined(hpux) || defined(__hpux)
#define WTF_OS_HPUX 1
#ifndef MAP_ANON
#define MAP_ANON MAP_ANONYMOUS
#endif
#endif

/* OS(LINUX) - Linux */
#ifdef __linux__
#define WTF_OS_LINUX 1
#endif

/* OS(NETBSD) - NetBSD */
#if defined(__NetBSD__)
#define WTF_PLATFORM_NETBSD 1
#endif

/* OS(OPENBSD) - OpenBSD */
#ifdef __OpenBSD__
#define WTF_OS_OPENBSD 1
#endif

/* OS(SOLARIS) - Solaris */
#if defined(sun) || defined(__sun)
#define WTF_OS_SOLARIS 1
#endif

/* OS(UNIX) - Any Unix-like system */
#if   OS(AIX)              \
    || OS(ANDROID)          \
    || OS(FREEBSD)          \
    || OS(HAIKU)            \
    || OS(HPUX)             \
    || OS(LINUX)            \
    || OS(NETBSD)           \
    || OS(OPENBSD)          \
    || OS(SOLARIS)          \
    || defined(unix)        \
    || defined(__unix)      \
    || defined(__unix__)
#define WTF_OS_UNIX 1
#endif

/* Operating environments */

/* FIXME: these are all mixes of OS, operating environment and policy choices. */
/* PLATFORM(CHROMIUM) */
/* PLATFORM(QT) */
/* PLATFORM(WX) */
/* PLATFORM(GTK) */
/* PLATFORM(HAIKU) */
/* PLATFORM(MAC) */
/* PLATFORM(WIN) */
#define WTF_PLATFORM_QT 1

/* PLATFORM(IPHONE) */
/* FIXME: this is sometimes used as an OS switch and sometimes for higher-level things */
#if (defined(TARGET_OS_EMBEDDED) && TARGET_OS_EMBEDDED) || (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE)
#define WTF_PLATFORM_IPHONE 1
#endif

#if !defined(WTF_PLATFORM_IPHONE)
#define WTF_PLATFORM_IPHONE 0
#endif

/* PLATFORM(ANDROID) */
/* FIXME: this is sometimes used as an OS() switch, and other times to drive
   policy choices */
#if defined(ANDROID)
#define WTF_PLATFORM_ANDROID 1
#endif

#if OS(FREEBSD) || OS(OPENBSD)
#define HAVE_PTHREAD_NP_H 1
#endif

#if OS(UNIX)
#define HAVE_SIGNAL_H 1
#endif

#if !OS(SOLARIS) \
     && !OS(HAIKU) && !OS(ANDROID) \
     && !OS(AIX) && !OS(HPUX)
#define HAVE_TM_GMTOFF 1
#define HAVE_TM_ZONE 1
#define HAVE_TIMEGM 1
#endif

#if OS(ANDROID)

#define HAVE_ERRNO_H 1
#define HAVE_LANGINFO_H 0
#define HAVE_MMAP 1
#define HAVE_SBRK 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_TIME_H 1

#else

#define HAVE_ERRNO_H 1
/* As long as Haiku doesn't have a complete support of locale this will be disabled. */
#if !OS(HAIKU)
#define HAVE_LANGINFO_H 1
#endif
#define HAVE_MMAP 1
#define HAVE_SBRK 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_TIME_H 1

#endif

/* ENABLE macro defaults */

#if !defined(ENABLE_OPCODE_STATS)
#define ENABLE_OPCODE_STATS 0
#endif

#define ENABLE_SAMPLING_COUNTERS 0
#define ENABLE_SAMPLING_FLAGS 0
#define ENABLE_OPCODE_SAMPLING 0
#define ENABLE_CODEBLOCK_SAMPLING 0
#if ENABLE(CODEBLOCK_SAMPLING) && !ENABLE(OPCODE_SAMPLING)
#error "CODEBLOCK_SAMPLING requires OPCODE_SAMPLING"
#endif
#if ENABLE(OPCODE_SAMPLING) || ENABLE(SAMPLING_FLAGS)
#define ENABLE_SAMPLING_THREAD 1
#endif

#if !defined(WTF_USE_JSVALUE64) && !defined(WTF_USE_JSVALUE32) && !defined(WTF_USE_JSVALUE32_64)
#if (CPU(X86_64) && (OS(UNIX) || OS(SOLARIS) || OS(HPUX))) || (CPU(IA64) && !CPU(IA64_32)) || CPU(ALPHA) || CPU(AIX64) || CPU(SPARC64) || CPU(MIPS64) || CPU(AARCH64)
#define WTF_USE_JSVALUE64 1
#elif CPU(ARM) || CPU(PPC64)
#define WTF_USE_JSVALUE32 1
#else
#define WTF_USE_JSVALUE32_64 1
#endif
#endif /* !defined(WTF_USE_JSVALUE64) && !defined(WTF_USE_JSVALUE32) && !defined(WTF_USE_JSVALUE32_64) */

#if COMPILER(GCC)
#define WARN_UNUSED_RETURN __attribute__ ((warn_unused_result))
#else
#define WARN_UNUSED_RETURN
#endif

#define ENABLE_JSC_ZOMBIES 0

#endif /* WTF_Platform_h */
