# Editors notes:
#
# * To find all FIXME/TODO related to the CMake build system (and those
#   inherited from the QMake build system) execute the following in the
#   top-level directory of the source tree:
#
#     find -name '*.cmake' -exec grep -E 'TODO|FIXME' {} +
#     find -name CMakeLists.txt -exec grep -E 'TODO|FIXME' {} +

cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

if(CMAKE_CROSSCOMPILING AND NOT CMAKE_CROSSCOMPILING_EMULATOR)
    message(FATAL_ERROR "Cross-compiling requires CMAKE_CROSSCOMPILING_EMULATOR to be set to QEMU executable, e.g. qemu-arm-static")
endif()

project(Katie C CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_AUTOMOC FALSE)
set(CMAKE_AUTOUIC FALSE)
set(CMAKE_AUTORCC FALSE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/staticlib")
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 300)

# enable symbols visibility preset for all targets
cmake_policy(SET CMP0063 NEW)

# LLVM linker does not have a default library search path, add some directories known to contain
# system wide libraries. the headers search path is also incomplete
if(CMAKE_SYSTEM_NAME MATCHES "BSD")
    foreach(incdir /usr/X11R6/include /usr/X11R7/include /usr/pkg/include /usr/local/include /usr/include)
        if(EXISTS "${incdir}")
            include_directories(${incdir})
        endif()
    endforeach()
    foreach(linkdir /usr/X11R6/lib /usr/X11R7/lib /usr/pkg/lib /usr/local/lib /usr/lib /lib)
        if(EXISTS "${linkdir}")
            link_directories(${linkdir})
        endif()
    endforeach()
endif()

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)
include(CheckIncludeFileCXX)
include(CheckTypeSize)
include(CMakePushCheckState)
include(CheckStructHasMember)
include(CheckCXXSourceRuns)
include(TestBigEndian)
include(FeatureSummary)
include(KatieBuildMacros)

# disable some compiler warnings
set(KATIE_NO_WARNINGS
    # placement new used in private QVariant functions
    -Wno-placement-new
    # QVector does not always use new/delete
    -Wno-class-memaccess
    # hot attribute used in function prototypes
    -Wno-attributes
    # deprecated X11, CUPS and OpenSSL API used
    -Wno-deprecated -Wno-deprecated-declarations
)
foreach(nowarning ${KATIE_NO_WARNINGS})
    string(REPLACE "-W" "" normalizednowarning "${nowarning}")
    check_cxx_compiler_flag(${nowarning} CXX_${normalizednowarning})
    if(CXX_${normalizednowarning})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${nowarning}")
    endif()
endforeach()

# https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
# https://github.com/Kitware/CMake/blob/master/Modules/CMakeDetermineSystem.cmake
if(CMAKE_SYSTEM_NAME MATCHES "(Linux|GNU)")
    katie_definition(-D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE)
elseif(CMAKE_SYSTEM_NAME MATCHES "(FreeBSD|DragonFly)")
    katie_definition(-D_THREAD_SAFE)
elseif(NOT CMAKE_SYSTEM_NAME MATCHES "(OpenBSD|NetBSD|Solaris|SunOS)")
    message(FATAL_ERROR "Unknown platform '${CMAKE_SYSTEM_NAME}'")
endif()

# mostly relevant to the build process, also stored in the version file for CMake and in qconfig.h.cmake
set(KATIE_MAJOR "4")
set(KATIE_MINOR "11")
set(KATIE_MICRO "1")
set(KATIE_HEX "0x041101")
set(KATIE_VERSION "${KATIE_MAJOR}.${KATIE_MINOR}.${KATIE_MICRO}")

# only for the build process
if(CMAKE_SYSTEM_PROCESSOR)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" KATIE_PROCESSOR)
elseif(CMAKE_HOST_SYSTEM_PROCESSOR)
    string(TOLOWER "${CMAKE_HOST_SYSTEM_PROCESSOR}" KATIE_PROCESSOR)
endif()

# used in katie_optimize_headers()
find_program(KATIE_UNIFDEF NAMES unifdef)
if(NOT KATIE_UNIFDEF)
    message(WARNING "Could not find unifdef, headers will not be optimized")
endif()

# used as shebang for profile, exec, dbus, xvfb and dbg scripts
find_program(KATIE_SHELL NAMES sh dash mksh ksh ash)
if(NOT KATIE_SHELL)
    message(FATAL_ERROR "Could not detect shell, supported are: sh, dash, mksh, ksh and ash")
endif()

# should be either STATIC or SHARED
set(KATIE_TYPE SHARED CACHE STRING "Build type")

# components and tools that will be build, changed depending on the requirements availability
# NOTE: excluding DesignerComponents as it was never used as public component, currently only
# used by the the designer tool. if it is ever needed for something it can just be added here,
# Q_DESIGNER_COMPONENTS_EXPORT must be introduced and genmap.py adjusted - the rest of the
# build system has everything in place.
set(KATIE_COMPONENTS "Core Gui Declarative Designer Network Svg Xml Script ScriptTools Test UiTools")
set(KATIE_TOOLS "moc uic rcc designer qscript qtconfig")

set(KATIE_HEADERS_PATH "${CMAKE_INSTALL_FULL_INCLUDEDIR}/katie" CACHE PATH "Headers installation path")
set(KATIE_LIBRARIES_PATH "${CMAKE_INSTALL_FULL_LIBDIR}" CACHE PATH "Libraries installation path")
set(KATIE_BINARIES_PATH "${CMAKE_INSTALL_FULL_BINDIR}" CACHE PATH "Binaries installation path")
set(KATIE_PLUGINS_PATH "${KATIE_LIBRARIES_PATH}/katie/plugins" CACHE PATH "Plugins installation path")
set(KATIE_IMPORTS_PATH "${KATIE_LIBRARIES_PATH}/katie/imports" CACHE PATH "Declarative imports installation path")
set(KATIE_TRANSLATIONS_PATH "${CMAKE_INSTALL_FULL_LOCALEDIR}" CACHE PATH "Translations installation path")
set(KATIE_CMAKE_PATH "${KATIE_LIBRARIES_PATH}/cmake/Katie" CACHE PATH "CMake aware modules installation path")
set(KATIE_LDCONF_PATH "${CMAKE_INSTALL_FULL_SYSCONFDIR}/ld.so.conf.d" CACHE PATH "Run-time linker/loader configs installation path")
set(KATIE_PROFILE_PATH "${CMAKE_INSTALL_FULL_SYSCONFDIR}/profile.d" CACHE PATH "Shell profile scripts installation path")
set(KATIE_MAN_PATH "${CMAKE_INSTALL_FULL_MANDIR}" CACHE PATH "Manual pages installation path")
set(KATIE_APPLICATIONS_PATH "${CMAKE_INSTALL_FULL_DATADIR}/applications" CACHE PATH "Desktop applications register installation path")
set(KATIE_PIXMAPS_PATH "${CMAKE_INSTALL_FULL_DATADIR}/pixmaps" CACHE PATH "Desktop applications icon installation path")
set(KATIE_PKGCONFIG_PATH "${KATIE_LIBRARIES_PATH}/pkgconfig" CACHE PATH "pkg-config installation path")
set(KATIE_TOOLS_SUFFIX "" CACHE STRING "Tools (moc, uic, rcc, etc.) suffix")

# optional package
option(WITH_CUPS "Build CUPS support" ON)
add_feature_info(cups WITH_CUPS "build CUPS support")

option(WITH_JPEG "Build JPEG support" ON)
add_feature_info(jpeg WITH_JPEG "build JPEG support")

option(WITH_TIFF "Build TIFF support" ON)
add_feature_info(tiff WITH_TIFF "build TIFF support")

option(WITH_DBUS "Build D-Bus support" ON)
add_feature_info(dbus WITH_DBUS "build D-Bus support")

option(WITH_FONTCONFIG "Build Fontconfig support" ON)
add_feature_info(fontconfig WITH_FONTCONFIG "build Fontconfig support")

option(WITH_ODBC "Build ODBC database plugin" ON)
add_feature_info(odbc WITH_ODBC "build ODBC support")

option(WITH_INTL "Build Intl support" ON)
add_feature_info(intl WITH_INTL "build Intl support")

option(WITH_EXECINFO "Build ExecInfo support" OFF)
add_feature_info(execinfo WITH_EXECINFO "build ExecInfo support")

# optional features
option(WITH_ACCESSIBILITY "Build accessibility support" ON)
add_feature_info(accessibility WITH_ACCESSIBILITY "build accessibility support")

option(KATIE_TESTS "Build automatic tests" OFF)
add_feature_info(tests KATIE_TESTS "build automatic tests")

option(KATIE_BENCHMARKS "Build automatic benchmarks" OFF)
add_feature_info(benchmarks KATIE_BENCHMARKS "build automatic benchmarks")

option(KATIE_UTILS "Build maintainance utilities" OFF)
add_feature_info(utils KATIE_UTILS "build maintainance utilities")

# v0.4+ required for inflateInit2() but that is pre-release
find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES
    PURPOSE "Required for compression support"
    DESCRIPTION "A Massively Spiffy Yet Delicately Unobtrusive Compression Library"
    URL "https://zlib.net"
    TYPE REQUIRED
)

# v4.6+ required for unorm2_getDecomposition()
find_package(ICU 4.6)
set_package_properties(ICU PROPERTIES
    PURPOSE "Required for locales support"
    DESCRIPTION "Mature, widely used libraries providing Unicode and Globalization support"
    URL "http://site.icu-project.org/"
    TYPE REQUIRED
)

# v1.1.3+ required for exported ZSTD_getErrorString() and ZSTD_getErrorCode()
find_package(ZSTD 1.1.3)
set_package_properties(ZSTD PROPERTIES
    PURPOSE "Required for compression support"
    DESCRIPTION "Fast real-time compression algorithm"
    URL "https://github.com/facebook/zstd"
    TYPE REQUIRED
)

# v2.8+ required for JSON_PARSER_MAX_DEPTH
find_package(Jansson 2.8)
set_package_properties(Jansson PROPERTIES
    PURPOSE "Required for JSON support"
    DESCRIPTION "C library for encoding, decoding and manipulating JSON data"
    URL "https://github.com/akheron/jansson"
    TYPE REQUIRED
)

# v1.1.0+ required for thread-safety without use of locking callbacks
find_package(OpenSSL 1.1.0)
set_package_properties(OpenSSL PROPERTIES
    PURPOSE "Required for HTTPS support"
    DESCRIPTION "Robust, commercial-grade, and full-featured toolkit for SSL and TLS"
    URL "https://openssl.org"
    TYPE REQUIRED
)

# v7.7+ required for JavaScript compatibility
find_package(PCRE 7.7)
set_package_properties(PCRE PROPERTIES
    PURPOSE "Required for regexp support in script component"
    DESCRIPTION "Perl Compatible Regular Expressions"
    URL "https://www.pcre.org/"
    TYPE REQUIRED
)

find_package(X11)
set_package_properties(X11 PROPERTIES
    PURPOSE "Required for X11/X.Org integration support"
    DESCRIPTION "Open source implementation of the X Window System"
    URL "https://www.x.org"
    TYPE REQUIRED
)

# v2.1.10+ required for FT_GlyphSlot_Embolden()
find_package(Freetype 2.1.10)
set_package_properties(Freetype PROPERTIES
    PURPOSE "Required for fonts configuration support"
    DESCRIPTION "Freely available software library to render fonts"
    URL "https://www.freetype.org"
    TYPE REQUIRED
)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)
set_package_properties(Threads PROPERTIES
    PURPOSE "Required for threads support"
    DESCRIPTION "Platform dependent threads implementation"
    URL ""
    TYPE REQUIRED
)

# v1.5.4+ required for png_longjmp_ptr() without noreturn attribute
find_package(PNG 1.5.4)
set_package_properties(PNG PROPERTIES
    PURPOSE "PNG format handler"
    DESCRIPTION "A collection of routines used to create PNG format graphics files"
    URL "http://www.libpng.org/pub/png/libpng.html"
    TYPE REQUIRED
)

find_package(JPEG)
set_package_properties(JPEG PROPERTIES
    PURPOSE "JPEG format handler"
    DESCRIPTION "JPEG image codec with accelerated baseline compression and decompression"
    URL "https://libjpeg-turbo.virtualgl.org/"
    TYPE RECOMMENDED
)

# v3.6.0+ required for TIFFReadRGBAImageOriented()
find_package(TIFF 3.6.0)
set_package_properties(TIFF PROPERTIES
    PURPOSE "TIFF format handler"
    DESCRIPTION "Library for manipulation of TIFF images"
    URL "http://www.libtiff.org/"
    TYPE RECOMMENDED
)

find_package(Cups)
set_package_properties(Cups PROPERTIES
    PURPOSE "Required for printing support"
    DESCRIPTION "CUPS is the standards-based, open source printing system"
    URL "https://www.cups.org"
    TYPE RECOMMENDED
)

# v2.4.2+ required for Freetype integration
find_package(Fontconfig 2.4.2)
set_package_properties(Fontconfig PROPERTIES
    PURPOSE "Required for fonts configuration support"
    DESCRIPTION "Library for configuring and customizing font access"
    URL "https://www.freedesktop.org/wiki/Software/fontconfig/"
    TYPE RECOMMENDED
)

# v1.5.12+ required for DBusBasicValue type
find_package(DBus 1.5.12)
set_package_properties(DBus PROPERTIES
    PURPOSE "Required for D-Bus support"
    DESCRIPTION "Message bus system, a simple way for applications to talk to one another"
    URL "https://www.freedesktop.org/wiki/Software/dbus"
    TYPE RECOMMENDED
)

find_package(Intl)
set_package_properties(Intl PROPERTIES
    PURPOSE "Required for translations support"
    DESCRIPTION "GNU gettext runtime library"
    URL "https://www.gnu.org/software/gettext/gettext.html"
    TYPE RECOMMENDED
)

find_package(ExecInfo)
set_package_properties(ExecInfo PROPERTIES
    PURPOSE "Required for stack traces on assert and crash"
    DESCRIPTION "Standard C library specific execinfo implementation"
    URL ""
    TYPE RECOMMENDED
)

find_package(XdgUtils)
set_package_properties(XdgUtils PROPERTIES
    PURPOSE "Required for desktop services"
    DESCRIPTION "Utilities for integrating applications with the desktop environment"
    URL "https://www.freedesktop.org/wiki/Software/xdg-utils/"
    TYPE RUNTIME
)

# stored in qconfig.h.cmake
check_type_size(size_t QT_POINTER_SIZE)

test_big_endian(KATIE_BIG_ENDIAN)
if(KATIE_BIG_ENDIAN)
    set(KATIE_BYTE_ORDER "Q_BIG_ENDIAN")
else()
    set(KATIE_BYTE_ORDER "Q_LITTLE_ENDIAN")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(KATIE_OS "LINUX")
elseif(CMAKE_SYSTEM_NAME MATCHES "GNU")
    set(KATIE_OS "HURD")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    set(KATIE_OS "FREEBSD")
elseif(CMAKE_SYSTEM_NAME MATCHES "DragonFly")
    set(KATIE_OS "DRAGONFLY")
elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
    set(KATIE_OS "OPENBSD")
elseif(CMAKE_SYSTEM_NAME MATCHES "NetBSD")
    set(KATIE_OS "NETBSD")
elseif(CMAKE_SYSTEM_NAME MATCHES "(Solaris|SunOS)")
    set(KATIE_OS "SOLARIS")
else()
    message(FATAL_ERROR "Unknown platform '${CMAKE_SYSTEM_NAME}'")
endif()

# https://en.wikipedia.org/wiki/Uname
if(KATIE_PROCESSOR MATCHES "^alpha")
    set(KATIE_ARCH "ALPHA")
elseif(KATIE_PROCESSOR MATCHES "^arm64")
    set(KATIE_ARCH "ARM64")
elseif(KATIE_PROCESSOR MATCHES "^arm")
    set(KATIE_ARCH "ARM")
elseif(KATIE_PROCESSOR MATCHES "^avr")
    set(KATIE_ARCH "AVR32")
elseif(KATIE_PROCESSOR MATCHES "^blackfin")
    set(KATIE_ARCH "BFIN")
elseif(KATIE_PROCESSOR MATCHES "^ia64")
    set(KATIE_ARCH "IA64")
elseif(KATIE_PROCESSOR MATCHES "^m68k")
    set(KATIE_ARCH "M68K")
elseif(KATIE_PROCESSOR MATCHES "^mips")
    set(KATIE_ARCH "MIPS")
elseif(KATIE_PROCESSOR MATCHES "^hppa")
    set(KATIE_ARCH "PARISC")
elseif(KATIE_PROCESSOR MATCHES "^ppc64")
    set(KATIE_ARCH "POWERPC64")
elseif(KATIE_PROCESSOR MATCHES "^ppc")
    set(KATIE_ARCH "POWERPC")
elseif(KATIE_PROCESSOR MATCHES "^s390")
    set(KATIE_ARCH "S390")
elseif(KATIE_PROCESSOR MATCHES "^sparc")
    set(KATIE_ARCH "SPARC")
elseif(KATIE_PROCESSOR MATCHES "^sh")
    # TODO: SH4A
    set(KATIE_ARCH "SH")
elseif(KATIE_PROCESSOR MATCHES "(x86_64|amd64)")
    set(KATIE_ARCH "X86_64")
elseif(KATIE_PROCESSOR MATCHES "(x86|i[3-6]86|i86pc)")
    set(KATIE_ARCH "I386")
else()
    message(FATAL_ERROR "Unknown CPU '${KATIE_PROCESSOR}'")
endif()

# only Solaris is known to have these
find_library(SOCKET_LIBRARY NAMES socket)
find_library(NSL_LIBRARY NAMES nsl)
set(SOCKET_AND_NSL_LIBRARIES)
if(SOCKET_LIBRARY)
    set(SOCKET_AND_NSL_LIBRARIES
        ${SOCKET_AND_NSL_LIBRARIES}
        ${SOCKET_LIBRARY}
    )
endif()
if(NSL_LIBRARY)
    set(SOCKET_AND_NSL_LIBRARIES
        ${SOCKET_AND_NSL_LIBRARIES}
        ${NSL_LIBRARY}
    )
endif()

# used in components, tests and JavaScriptCore
katie_check_header("cxxabi.h")
katie_check_function(posix_memalign "stdlib.h")
katie_check_function(getprogname "stdlib.h")
katie_check_function(arc4random_uniform "stdlib.h")
katie_check_function(get_current_dir_name "unistd.h")
katie_check_function(prctl "sys/prctl.h")
katie_check_function(feenableexcept "fenv.h")
katie_check_function(madvise "sys/mman.h")
katie_check_function(timegm "time.h")
katie_check_function(pipe2 "unistd.h")
katie_check_function(getdomainname "unistd.h")
katie_check_function(renameat2 "stdio.h")
katie_check_struct(tm tm_gmtoff "time.h")
katie_check_struct(tm tm_zone "time.h")
katie_check_struct(dirent d_type "dirent.h")

cmake_reset_check_state()
set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
katie_check_function(pthread_setname_np "pthread.h")
cmake_reset_check_state()

cmake_reset_check_state()
set(CMAKE_REQUIRED_LIBRARIES ${SOCKET_AND_NSL_LIBRARIES})
katie_check_function(getifaddrs "ifaddrs.h")
katie_check_function(accept4 "sys/socket.h")
katie_check_function(paccept "sys/socket.h")
cmake_reset_check_state()

katie_check_proc(exe)
katie_check_proc(cmdline)

if(KATIE_TESTS OR KATIE_BENCHMARKS)
    message(WARNING "Deploying tests/benchmarks build is a bad idea")
    enable_testing()
    add_definitions(-DQT_BUILD_INTERNAL)
endif()

foreach(script exec dbus xvfb dbg)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/${script}.sh.cmake
        ${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${script}.sh
        @ONLY
    )
    file(
        COPY ${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/${script}.sh
        DESTINATION ${CMAKE_BINARY_DIR}
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ WORLD_READ
    )
endforeach()

# optional packages
if(NOT WITH_FONTCONFIG OR NOT FONTCONFIG_FOUND)
    katie_config(QT_NO_FONTCONFIG)
endif()

if(NOT WITH_DBUS OR NOT DBUS_FOUND)
    katie_config(QT_NO_DBUS)
else()
    set(KATIE_COMPONENTS "${KATIE_COMPONENTS} DBus")
    set(KATIE_TOOLS "${KATIE_TOOLS} qdbus qdbusxml2cpp qdbuscpp2xml qdbusviewer")
endif()

if(NOT WITH_CUPS OR NOT CUPS_FOUND)
    katie_config(QT_NO_CUPS)
endif()

if(NOT WITH_INTL OR NOT INTL_FOUND)
    katie_config(QT_NO_TRANSLATION)
endif()

if(NOT WITH_EXECINFO OR NOT EXECINFO_FOUND)
    katie_config(QT_NO_EXECINFO)
endif()

# optional and conditional features
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    katie_config(QT_NO_DEBUG)
endif()

if(NOT WITH_ACCESSIBILITY)
    katie_config(QT_NO_ACCESSIBILITY)
endif()

add_library(pic_test ${KATIE_TYPE}  EXCLUDE_FROM_ALL "${CMAKE_SOURCE_DIR}/cmake/pic.cpp")
get_target_property(HAVE_pic pic_test POSITION_INDEPENDENT_CODE)
if(NOT HAVE_pic)
    katie_definition(-DQ_NO_DATA_RELOCATION)
endif()

# keep in sync with the GUI component CMake file, only definitions setup is needed here
foreach(x11ext Xshape Xinerama Xrandr Xrender Xfixes Xcursor)
    if(NOT X11_${x11ext}_FOUND)
        message(WARNING "The X11 ${x11ext} extension was not found")
        string(TOUPPER "${x11ext}" upperext)
        katie_config(QT_NO_${upperext})
    endif()
endforeach()
if(NOT X11_Xext_FOUND)
    message(WARNING "The X11 Xext extension was not found")
    katie_config(QT_NO_XSYNC)
    katie_config(QT_NO_XSHAPE)
endif()
if(NOT X11_SM_FOUND OR NOT X11_ICE_FOUND)
    message(WARNING "X11 SM/ICE was not found thus disabling session manager support")
    katie_config(QT_NO_SESSIONMANAGER)
endif()

configure_file(
    ${CMAKE_SOURCE_DIR}/src/core/global/qconfig.h.cmake
    ${CMAKE_BINARY_DIR}/include/QtCore/qconfig.h
)
configure_file(
    ${CMAKE_SOURCE_DIR}/src/core/global/qconfig.cpp.cmake
    ${CMAKE_BINARY_DIR}/include/qconfig.cpp
)

add_subdirectory(src/shared)

add_subdirectory(src/core)
add_subdirectory(src/xml)
if(WITH_DBUS AND DBUS_FOUND)
    add_subdirectory(src/dbus)
endif()
add_subdirectory(src/declarative)
add_subdirectory(src/designer)
add_subdirectory(src/gui)
add_subdirectory(src/imports)
add_subdirectory(src/network)
add_subdirectory(src/plugins)
add_subdirectory(src/script)
add_subdirectory(src/scripttools)
add_subdirectory(src/svg)
add_subdirectory(src/test)
add_subdirectory(src/uitools)

add_subdirectory(src/tools)

if(WITH_INTL AND INTL_FOUND)
    add_subdirectory(translations)
endif()

if(KATIE_TESTS OR KATIE_BENCHMARKS)
    add_subdirectory(tests)
endif()

if(KATIE_UTILS)
    add_subdirectory(util)
endif()

# the macro calls bellow are generated via the incfsck script
katie_generate_obsolete(QAbstractExtensionFactory QtDesigner extension.h)
katie_generate_obsolete(QAbstractExtensionManager QtDesigner extension.h)
katie_generate_obsolete(QAbstractFileEngineIterator QtCore qabstractfileengine.h)
katie_generate_obsolete(QAbstractFormBuilder QtDesigner abstractformbuilder.h)
katie_generate_obsolete(QAbstractGraphicsShapeItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QAbstractListModel QtCore qabstractitemmodel.h)
katie_generate_obsolete(QAbstractTableModel QtCore qabstractitemmodel.h)
katie_generate_obsolete(QAbstractUndoItem QtGui qtextdocument.h)
katie_generate_obsolete(QAccessible2Interface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleActionInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleApplication QtGui qaccessibleobject.h)
katie_generate_obsolete(QAccessibleBridgeFactoryInterface QtGui qaccessiblebridge.h)
katie_generate_obsolete(QAccessibleBridgePlugin QtGui qaccessiblebridge.h)
katie_generate_obsolete(QAccessibleEditableTextInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleEvent QtGui qaccessible.h)
katie_generate_obsolete(QAccessibleFactoryInterface QtGui qaccessibleplugin.h)
katie_generate_obsolete(QAccessibleImageInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleInterfaceEx QtGui qaccessible.h)
katie_generate_obsolete(QAccessibleInterface QtGui qaccessible.h)
katie_generate_obsolete(QAccessibleObjectEx QtGui qaccessibleobject.h)
katie_generate_obsolete(QAccessibleSimpleEditableTextInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleTable2CellInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleTable2Interface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleTableInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleTextInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleValueInterface QtGui qaccessible2.h)
katie_generate_obsolete(QAccessibleWidgetEx QtGui qaccessiblewidget.h)
katie_generate_obsolete(QActionEvent QtGui qevent.h)
katie_generate_obsolete(QAnimationDriver QtGui qabstractanimation.h)
katie_generate_obsolete(QArgument QtCore qobjectdefs.h)
katie_generate_obsolete(QAtomicInt QtCore qatomic.h)
katie_generate_obsolete(QAtomicPointer QtCore qatomic.h)
katie_generate_obsolete(QBasicAtomicInt QtCore qbasicatomic.h)
katie_generate_obsolete(QBitRef QtCore qbitarray.h)
katie_generate_obsolete(QBrushData QtGui qbrush.h)
katie_generate_obsolete(QByteRef QtCore qbytearray.h)
katie_generate_obsolete(QCharRef QtCore qstring.h)
katie_generate_obsolete(QChildEvent QtCore qcoreevent.h)
katie_generate_obsolete(QClipboardEvent QtGui qevent.h)
katie_generate_obsolete(QCloseEvent QtGui qevent.h)
katie_generate_obsolete(QConicalGradient QtGui qbrush.h)
katie_generate_obsolete(QContextMenuEvent QtGui qevent.h)
katie_generate_obsolete(QContiguousCacheData QtCore qcontiguouscache.h)
katie_generate_obsolete(QContiguousCacheTypedData QtCore qcontiguouscache.h)
katie_generate_obsolete(QDateEdit QtGui qdatetimeedit.h)
katie_generate_obsolete(QDate QtCore qdatetime.h)
katie_generate_obsolete(QDBusAbstractInterfaceBase QtDBus qdbusabstractinterface.h)
katie_generate_obsolete(QDBusObjectPath QtDBus qdbusextratypes.h)
katie_generate_obsolete(QDBusPendingCallWatcher QtDBus qdbuspendingcall.h)
katie_generate_obsolete(QDBusPendingReplyData QtDBus qdbuspendingreply.h)
katie_generate_obsolete(QDBusSignature QtDBus qdbusextratypes.h)
katie_generate_obsolete(QDBusVariant QtDBus qdbusextratypes.h)
katie_generate_obsolete(QDeclarativeAttachedPropertiesFunc QtDeclarative qdeclarativeprivate.h)
katie_generate_obsolete(QDeclarativeListProperty QtDeclarative qdeclarativelist.h)
katie_generate_obsolete(QDeclarativeListReference QtDeclarative qdeclarativelist.h)
katie_generate_obsolete(QDeclarativeProperties QtDeclarative qdeclarativeproperty.h)
katie_generate_obsolete(QDeclarativeTypeInfo QtDeclarative qdeclarativeprivate.h)
katie_generate_obsolete(QDesignerActionEditorInterface QtDesigner abstractactioneditor.h)
katie_generate_obsolete(QDesignerBrushManagerInterface QtDesigner abstractbrushmanager.h)
katie_generate_obsolete(QDesignerComponents QtDesigner qdesigner_components.h)
katie_generate_obsolete(QDesignerContainerExtension QtDesigner container.h)
katie_generate_obsolete(QDesignerCustomWidgetCollectionInterface QtDesigner customwidget.h)
katie_generate_obsolete(QDesignerCustomWidgetInterface QtDesigner customwidget.h)
katie_generate_obsolete(QDesignerDnDItemInterface QtDesigner abstractdnditem.h)
katie_generate_obsolete(QDesignerDynamicPropertySheetExtension QtDesigner dynamicpropertysheet.h)
katie_generate_obsolete(QDesignerExtraInfoExtension QtDesigner extrainfo.h)
katie_generate_obsolete(QDesignerFormEditorInterface QtDesigner abstractformeditor.h)
katie_generate_obsolete(QDesignerFormEditorPluginInterface QtDesigner abstractformeditorplugin.h)
katie_generate_obsolete(QDesignerFormWindowCursorInterface QtDesigner abstractformwindowcursor.h)
katie_generate_obsolete(QDesignerFormWindowInterface QtDesigner abstractformwindow.h)
katie_generate_obsolete(QDesignerFormWindowManagerInterface QtDesigner abstractformwindowmanager.h)
katie_generate_obsolete(QDesignerFormWindowToolInterface QtDesigner abstractformwindowtool.h)
katie_generate_obsolete(QDesignerIconCacheInterface QtDesigner abstracticoncache.h)
katie_generate_obsolete(QDesignerIntegrationInterface QtDesigner abstractintegration.h)
katie_generate_obsolete(QDesignerLanguageExtension QtDesigner abstractlanguage.h)
katie_generate_obsolete(QDesignerLayoutDecorationExtension QtDesigner layoutdecoration.h)
katie_generate_obsolete(QDesignerMemberSheetExtension QtDesigner membersheet.h)
katie_generate_obsolete(QDesignerMetaDataBaseInterface QtDesigner abstractmetadatabase.h)
katie_generate_obsolete(QDesignerMetaDataBaseItemInterface QtDesigner abstractmetadatabase.h)
katie_generate_obsolete(QDesignerObjectInspectorInterface QtDesigner abstractobjectinspector.h)
katie_generate_obsolete(QDesignerPromotionInterface QtDesigner abstractpromotioninterface.h)
katie_generate_obsolete(QDesignerPropertyEditorInterface QtDesigner abstractpropertyeditor.h)
katie_generate_obsolete(QDesignerPropertySheetExtension QtDesigner propertysheet.h)
katie_generate_obsolete(QDesignerResourceBrowserInterface QtDesigner abstractresourcebrowser.h)
katie_generate_obsolete(QDesignerTaskMenuExtension QtDesigner taskmenu.h)
katie_generate_obsolete(QDesignerWidgetBoxInterface QtDesigner abstractwidgetbox.h)
katie_generate_obsolete(QDesignerWidgetDataBaseInterface QtDesigner abstractwidgetdatabase.h)
katie_generate_obsolete(QDesignerWidgetDataBaseItemInterface QtDesigner abstractwidgetdatabase.h)
katie_generate_obsolete(QDesignerWidgetFactoryInterface QtDesigner abstractwidgetfactory.h)
katie_generate_obsolete(QDomAttr QtXml qdom.h)
katie_generate_obsolete(QDomCDATASection QtXml qdom.h)
katie_generate_obsolete(QDomCharacterData QtXml qdom.h)
katie_generate_obsolete(QDomComment QtXml qdom.h)
katie_generate_obsolete(QDomDocumentFragment QtXml qdom.h)
katie_generate_obsolete(QDomDocument QtXml qdom.h)
katie_generate_obsolete(QDomDocumentType QtXml qdom.h)
katie_generate_obsolete(QDomElement QtXml qdom.h)
katie_generate_obsolete(QDomEntity QtXml qdom.h)
katie_generate_obsolete(QDomEntityReference QtXml qdom.h)
katie_generate_obsolete(QDomImplementation QtXml qdom.h)
katie_generate_obsolete(QDomNamedNodeMap QtXml qdom.h)
katie_generate_obsolete(QDomNodeList QtXml qdom.h)
katie_generate_obsolete(QDomNode QtXml qdom.h)
katie_generate_obsolete(QDomNotation QtXml qdom.h)
katie_generate_obsolete(QDomProcessingInstruction QtXml qdom.h)
katie_generate_obsolete(QDomText QtXml qdom.h)
katie_generate_obsolete(QDoubleSpinBox QtGui qspinbox.h)
katie_generate_obsolete(QDoubleValidator QtGui qvalidator.h)
katie_generate_obsolete(QDragEnterEvent QtGui qevent.h)
katie_generate_obsolete(QDragLeaveEvent QtGui qevent.h)
katie_generate_obsolete(QDragMoveEvent QtGui qevent.h)
katie_generate_obsolete(QDropEvent QtGui qevent.h)
katie_generate_obsolete(QDynamicPropertyChangeEvent QtCore qcoreevent.h)
katie_generate_obsolete(QEvent QtCore qcoreevent.h)
katie_generate_obsolete(QEventSizeOfChecker QtTest qtestspontaneevent.h)
katie_generate_obsolete(QExplicitlySharedDataPointer QtCore qsharedpointer.h)
katie_generate_obsolete(QExtensionFactory QtDesigner default_extensionfactory.h)
katie_generate_obsolete(QFileInfoList QtCore qfileinfo.h)
katie_generate_obsolete(QFlag QtCore qglobal.h)
katie_generate_obsolete(QFlags QtCore qglobal.h)
katie_generate_obsolete(QFocusEvent QtGui qevent.h)
katie_generate_obsolete(QFontMetricsF QtGui qfontmetrics.h)
katie_generate_obsolete(QFormBuilder QtDesigner formbuilder.h)
katie_generate_obsolete(QFutureInterfaceBase QtCore qfutureinterface.h)
katie_generate_obsolete(QFutureIterator QtCore qfuture.h)
katie_generate_obsolete(QFutureWatcherBase QtCore qfuturewatcher.h)
katie_generate_obsolete(QGenericArgument QtCore qobjectdefs.h)
katie_generate_obsolete(QGenericReturnArgument QtCore qobjectdefs.h)
katie_generate_obsolete(QGlobalStaticDeleter QtCore qglobal.h)
katie_generate_obsolete(QGlobalStatic QtCore qglobal.h)
katie_generate_obsolete(QGradient QtGui qbrush.h)
katie_generate_obsolete(QGradientStop QtGui qbrush.h)
katie_generate_obsolete(QGradientStops QtGui qbrush.h)
katie_generate_obsolete(QGraphicsAnchor QtGui qgraphicsanchorlayout.h)
katie_generate_obsolete(QGraphicsBlurEffect QtGui qgraphicseffect.h)
katie_generate_obsolete(QGraphicsColorizeEffect QtGui qgraphicseffect.h)
katie_generate_obsolete(QGraphicsDropShadowEffect QtGui qgraphicseffect.h)
katie_generate_obsolete(QGraphicsEllipseItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsItemGroup QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsLineItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsObject QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsOpacityEffect QtGui qgraphicseffect.h)
katie_generate_obsolete(QGraphicsPathItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsPixmapItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsPolygonItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsRectItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsRotation QtGui qgraphicstransform.h)
katie_generate_obsolete(QGraphicsScale QtGui qgraphicstransform.h)
katie_generate_obsolete(QGraphicsSceneContextMenuEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSceneDragDropEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSceneHelpEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSceneHoverEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSceneMouseEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSceneMoveEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSceneResizeEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSceneWheelEvent QtGui qgraphicssceneevent.h)
katie_generate_obsolete(QGraphicsSimpleTextItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QGraphicsTextItem QtGui qgraphicsitem.h)
katie_generate_obsolete(QHashData QtCore qhash.h)
katie_generate_obsolete(QHashIterator QtCore qhash.h)
katie_generate_obsolete(QHashNode QtCore qhash.h)
katie_generate_obsolete(QHBoxLayout QtGui qboxlayout.h)
katie_generate_obsolete(QHelpEvent QtGui qevent.h)
katie_generate_obsolete(QHideEvent QtGui qevent.h)
katie_generate_obsolete(QHoverEvent QtGui qevent.h)
katie_generate_obsolete(QHttpHeader QtNetwork qhttp.h)
katie_generate_obsolete(QHttpPart QtNetwork qhttpmultipart.h)
katie_generate_obsolete(QHttpRequestHeader QtNetwork qhttp.h)
katie_generate_obsolete(QHttpResponseHeader QtNetwork qhttp.h)
katie_generate_obsolete(QIconEngineFactoryInterface QtGui qiconengineplugin.h)
katie_generate_obsolete(QIconEngineFactoryInterfaceV2 QtGui qiconengineplugin.h)
katie_generate_obsolete(QIconEnginePluginV2 QtGui qiconengineplugin.h)
katie_generate_obsolete(QIconEngineV2 QtGui qiconengine.h)
katie_generate_obsolete(QImageIOHandlerFactoryInterface QtGui qimageiohandler.h)
katie_generate_obsolete(QImageIOPlugin QtGui qimageiohandler.h)
katie_generate_obsolete(QIncompatibleFlag QtCore qglobal.h)
katie_generate_obsolete(QInputEvent QtGui qevent.h)
katie_generate_obsolete(QInternal QtCore qnamespace.h)
katie_generate_obsolete(QIntValidator QtGui qvalidator.h)
katie_generate_obsolete(QIPv6Address QtNetwork qhostaddress.h)
katie_generate_obsolete(Q_IPV6ADDR QtNetwork qhostaddress.h)
katie_generate_obsolete(QItemEditorCreatorBase QtGui qitemeditorfactory.h)
katie_generate_obsolete(QItemEditorCreator QtGui qitemeditorfactory.h)
katie_generate_obsolete(QItemSelection QtGui qitemselectionmodel.h)
katie_generate_obsolete(QItemSelectionRange QtGui qitemselectionmodel.h)
katie_generate_obsolete(QKeyEvent QtGui qevent.h)
katie_generate_obsolete(QLatin1Char QtCore qchar.h)
katie_generate_obsolete(QLatin1String QtCore qstring.h)
katie_generate_obsolete(QLinearGradient QtGui qbrush.h)
katie_generate_obsolete(QLineF QtCore qline.h)
katie_generate_obsolete(QLinkedListData QtCore qlinkedlist.h)
katie_generate_obsolete(QLinkedListIterator QtCore qlinkedlist.h)
katie_generate_obsolete(QLinkedListNode QtCore qlinkedlist.h)
katie_generate_obsolete(QListData QtCore qlist.h)
katie_generate_obsolete(QListIterator QtCore qlist.h)
katie_generate_obsolete(QListWidgetItem QtGui qlistwidget.h)
katie_generate_obsolete(QMapData QtCore qmap.h)
katie_generate_obsolete(QMapIterator QtCore qmap.h)
katie_generate_obsolete(QMapNode QtCore qmap.h)
katie_generate_obsolete(QMapPayloadNode QtCore qmap.h)
katie_generate_obsolete(QMatrix2x2 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMatrix2x3 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMatrix2x4 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMatrix3x2 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMatrix3x3 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMatrix3x4 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMatrix4x2 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMatrix4x3 QtGui qgenericmatrix.h)
katie_generate_obsolete(QMetaClassInfo QtCore qmetaobject.h)
katie_generate_obsolete(QMetaEnum QtCore qmetaobject.h)
katie_generate_obsolete(QMetaMethod QtCore qmetaobject.h)
katie_generate_obsolete(QMetaObjectAccessor QtCore qobjectdefs.h)
katie_generate_obsolete(QMetaProperty QtCore qmetaobject.h)
katie_generate_obsolete(QMetaTypeId2 QtCore qmetatype.h)
katie_generate_obsolete(QMetaTypeId QtCore qmetatype.h)
katie_generate_obsolete(QModelIndexList QtCore qabstractitemmodel.h)
katie_generate_obsolete(QModelIndex QtCore qabstractitemmodel.h)
katie_generate_obsolete(QMouseEvent QtGui qevent.h)
katie_generate_obsolete(QMoveEvent QtGui qevent.h)
katie_generate_obsolete(QMultiHash QtCore qhash.h)
katie_generate_obsolete(QMultiMap QtCore qmap.h)
katie_generate_obsolete(QMutableFutureIterator QtCore qfuture.h)
katie_generate_obsolete(QMutableHashIterator QtCore qhash.h)
katie_generate_obsolete(QMutableLinkedListIterator QtCore qlist.h)
katie_generate_obsolete(QMutableListIterator QtCore qlist.h)
katie_generate_obsolete(QMutableMapIterator QtCore qmap.h)
katie_generate_obsolete(QMutableSetIterator QtCore qset.h)
katie_generate_obsolete(QMutableStringListIterator QtCore qstringlist.h)
katie_generate_obsolete(QMutableVectorIterator QtCore qvector.h)
katie_generate_obsolete(QMutexLocker QtCore qmutex.h)
katie_generate_obsolete(QNetworkAddressEntry QtNetwork qnetworkinterface.h)
katie_generate_obsolete(QNetworkCacheMetaData QtNetwork qabstractnetworkcache.h)
katie_generate_obsolete(QNetworkProxyFactory QtNetwork qnetworkproxy.h)
katie_generate_obsolete(QNetworkProxyQuery QtNetwork qnetworkproxy.h)
katie_generate_obsolete(QNoDebug QtCore qdebug.h)
katie_generate_obsolete(QObjectData QtCore qobject.h)
katie_generate_obsolete(QObjectList QtCore qobject.h)
katie_generate_obsolete(QPaintEngineState QtGui qpaintengine.h)
katie_generate_obsolete(QPainterPathPrivate QtGui qpainterpath.h)
katie_generate_obsolete(QPainterPathStroker QtGui qpainterpath.h)
katie_generate_obsolete(QPaintEvent QtGui qevent.h)
katie_generate_obsolete(QPersistentModelIndex QtCore qabstractitemmodel.h)
katie_generate_obsolete(Q_PID QtCore qprocess.h)
katie_generate_obsolete(QPlainTextDocumentLayout QtGui qplaintextedit.h)
katie_generate_obsolete(QPointF QtCore qpoint.h)
katie_generate_obsolete(QPolygonF QtGui qpolygon.h)
katie_generate_obsolete(QProcessEnvironment QtCore qprocess.h)
katie_generate_obsolete(QRadialGradient QtGui qbrush.h)
katie_generate_obsolete(QReadLocker QtCore qreadwritelock.h)
katie_generate_obsolete(QRectF QtCore qrect.h)
katie_generate_obsolete(QRegExpValidator QtGui qvalidator.h)
katie_generate_obsolete(QResizeEvent QtGui qevent.h)
katie_generate_obsolete(QReturnArgument QtCore qobjectdefs.h)
katie_generate_obsolete(QScopedArrayPointer QtCore qscopedpointer.h)
katie_generate_obsolete(QScopedPointerArrayDeleter QtCore qscopedpointer.h)
katie_generate_obsolete(QScopedPointerPodDeleter QtCore qscopedpointer.h)
katie_generate_obsolete(QSetIterator QtCore qset.h)
katie_generate_obsolete(QSharedDataPointer QtCore qshareddata.h)
katie_generate_obsolete(QShortcutEvent QtGui qevent.h)
katie_generate_obsolete(QShowEvent QtGui qevent.h)
katie_generate_obsolete(QSizeF QtCore qsize.h)
katie_generate_obsolete(QSpacerItem QtGui qlayoutitem.h)
katie_generate_obsolete(QSplitterHandle QtGui qsplitter.h)
katie_generate_obsolete(QSpontaneKeyEvent QtTest qtestspontaneevent.h)
katie_generate_obsolete(QStandardItemEditorCreator QtGui qitemeditorfactory.h)
katie_generate_obsolete(QStandardItem QtGui qstandarditemmodel.h)
katie_generate_obsolete(QStatusTipEvent QtGui qevent.h)
katie_generate_obsolete(QStringListIterator QtCore qstringlist.h)
katie_generate_obsolete(QStringRef QtCore qstring.h)
katie_generate_obsolete(QStyleFactoryInterface QtGui qstyleplugin.h)
katie_generate_obsolete(QStyleHintReturnMask QtGui qstyleoption.h)
katie_generate_obsolete(QStyleHintReturn QtGui qstyleoption.h)
katie_generate_obsolete(QStyleHintReturnVariant QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionButton QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionComboBox QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionComplex QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionDockWidget QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionDockWidgetV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionFocusRect QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionFrame QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionFrameV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionFrameV3 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionGraphicsItem QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionGroupBox QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionHeader QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionMenuItem QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionProgressBar QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionProgressBarV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionRubberBand QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionSizeGrip QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionSlider QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionSpinBox QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabBarBase QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabBarBaseV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTab QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabV3 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabWidgetFrame QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabWidgetFrameV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTitleBar QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionToolBar QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionToolBox QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionToolBoxV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionToolButton QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItem QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItemV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItemV3 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItemV4 QtGui qstyleoption.h)
katie_generate_obsolete(QTableWidgetItem QtGui qtablewidget.h)
katie_generate_obsolete(QTableWidgetSelectionRange QtGui qtablewidget.h)
katie_generate_obsolete(QtAlgorithms QtCore qalgorithms.h)
katie_generate_obsolete(QtCleanUpFunction QtCore qcoreapplication.h)
katie_generate_obsolete(QtConfig QtCore qconfig.h)
katie_generate_obsolete(QtContainerFwd QtCore qcontainerfwd.h)
katie_generate_obsolete(QtDebug QtCore qdebug.h)
katie_generate_obsolete(QtEndian QtCore qendian.h)
katie_generate_obsolete(QTestAccessibilityEvent QtTest qtestaccessible.h)
katie_generate_obsolete(QTestAccessibility QtTest qtestaccessible.h)
katie_generate_obsolete(QTestDelayEvent QtTest qtestevent.h)
katie_generate_obsolete(QTestEventList QtTest qtestevent.h)
katie_generate_obsolete(QTestKeyClicksEvent QtTest qtestevent.h)
katie_generate_obsolete(QTestKeyEvent QtTest qtestevent.h)
katie_generate_obsolete(QTestMouseEvent QtTest qtestevent.h)
katie_generate_obsolete(QtEvents QtGui qevent.h)
katie_generate_obsolete(QTextBlockFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextBlockGroup QtGui qtextobject.h)
katie_generate_obsolete(QTextBlock QtGui qtextobject.h)
katie_generate_obsolete(QTextBlockUserData QtGui qtextobject.h)
katie_generate_obsolete(QTextCharFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextDecoder QtCore qtextcodec.h)
katie_generate_obsolete(QTextEncoder QtCore qtextcodec.h)
katie_generate_obsolete(QTextFragment QtGui qtextobject.h)
katie_generate_obsolete(QTextFrameFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextFrameLayoutData QtGui qtextobject.h)
katie_generate_obsolete(QTextFrame QtGui qtextobject.h)
katie_generate_obsolete(QTextImageFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextInlineObject QtGui qtextlayout.h)
katie_generate_obsolete(QTextItem QtGui qpaintengine.h)
katie_generate_obsolete(QTextLength QtGui qtextformat.h)
katie_generate_obsolete(QTextLine QtGui qtextlayout.h)
katie_generate_obsolete(QTextListFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextObjectInterface QtGui qabstracttextdocumentlayout.h)
katie_generate_obsolete(QTextStreamFunction QtCore qtextstream.h)
katie_generate_obsolete(QTextStreamManipulator QtCore qtextstream.h)
katie_generate_obsolete(QTextTableCellFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextTableCell QtGui qtexttable.h)
katie_generate_obsolete(QTextTableFormat QtGui qtextformat.h)
katie_generate_obsolete(QtGlobal QtCore qglobal.h)
katie_generate_obsolete(QTileRules QtGui qdrawutil.h)
katie_generate_obsolete(QTimeEdit QtGui qdatetimeedit.h)
katie_generate_obsolete(QTime QtCore qdatetime.h)
katie_generate_obsolete(QTimerEvent QtCore qcoreevent.h)
katie_generate_obsolete(QtMsgHandler QtCore qglobal.h)
katie_generate_obsolete(QtPluginInstanceFunction QtCore qplugin.h)
katie_generate_obsolete(QtPlugin QtCore qplugin.h)
katie_generate_obsolete(Qt QtCore qnamespace.h)
katie_generate_obsolete(QTreeWidgetItem QtGui qtreewidget.h)
katie_generate_obsolete(QtTestGui QtTest qtest_gui.h)
katie_generate_obsolete(QTypeInfo QtCore qglobal.h)
katie_generate_obsolete(QUndoCommand QtGui qundostack.h)
katie_generate_obsolete(QUnixPrintWidget QtGui qprintdialog.h)
katie_generate_obsolete(QUpdateLaterEvent QtGui qevent.h)
katie_generate_obsolete(QVariantHash QtCore qvariant.h)
katie_generate_obsolete(QVariantList QtCore qvariant.h)
katie_generate_obsolete(QVariantMap QtCore qvariant.h)
katie_generate_obsolete(QVBoxLayout QtGui qboxlayout.h)
katie_generate_obsolete(QVectorData QtCore qvector.h)
katie_generate_obsolete(QVectorIterator QtCore qvector.h)
katie_generate_obsolete(QVectorTypedData QtCore qvector.h)
katie_generate_obsolete(QWeakPointer QtCore qsharedpointer.h)
katie_generate_obsolete(QWhatsThisClickedEvent QtGui qevent.h)
katie_generate_obsolete(QWheelEvent QtGui qevent.h)
katie_generate_obsolete(QWidgetData QtGui qwidget.h)
katie_generate_obsolete(QWidgetItem QtGui qlayoutitem.h)
katie_generate_obsolete(QWidgetItemV2 QtGui qlayoutitem.h)
katie_generate_obsolete(QWidgetList QtGui qwindowdefs.h)
katie_generate_obsolete(QWidgetMapper QtGui qwindowdefs.h)
katie_generate_obsolete(QWidgetSet QtGui qwindowdefs.h)
katie_generate_obsolete(QWindowStateChangeEvent QtGui qevent.h)
katie_generate_obsolete(QWizardPage QtGui qwizard.h)
katie_generate_obsolete(QWriteLocker QtCore qreadwritelock.h)
katie_generate_obsolete(QX11EmbedContainer QtGui qx11embed_x11.h)
katie_generate_obsolete(QX11EmbedWidget QtGui qx11embed_x11.h)
katie_generate_obsolete(QX11Info QtGui qx11info_x11.h)
katie_generate_obsolete(QXmlAttributes QtXml qxml.h)
katie_generate_obsolete(QXmlContentHandler QtXml qxml.h)
katie_generate_obsolete(QXmlDeclHandler QtXml qxml.h)
katie_generate_obsolete(QXmlDefaultHandler QtXml qxml.h)
katie_generate_obsolete(QXmlDTDHandler QtXml qxml.h)
katie_generate_obsolete(QXmlEntityResolver QtXml qxml.h)
katie_generate_obsolete(QXmlErrorHandler QtXml qxml.h)
katie_generate_obsolete(QXmlInputSource QtXml qxml.h)
katie_generate_obsolete(QXmlLexicalHandler QtXml qxml.h)
katie_generate_obsolete(QXmlLocator QtXml qxml.h)
katie_generate_obsolete(QXmlNamespaceSupport QtXml qxml.h)
katie_generate_obsolete(QXmlParseException QtXml qxml.h)
katie_generate_obsolete(QXmlReader QtXml qxml.h)
katie_generate_obsolete(QXmlSimpleReader QtXml qxml.h)
katie_generate_obsolete(QXmlStreamAttribute QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamAttributes QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamEntityDeclaration QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamEntityDeclarations QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamEntityResolver QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamNamespaceDeclaration QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamNamespaceDeclarations QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamNotationDeclaration QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamNotationDeclarations QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamReader QtXml qxmlstream.h)
katie_generate_obsolete(QXmlStreamWriter QtXml qxmlstream.h)
# those have shuffled from QtDesigner to QtUiTools
katie_generate_obsolete(formbuilder.h QtDesigner ../QtUiTools/formbuilder.h)
katie_generate_obsolete(customwidget.h QtDesigner ../QtUiTools/customwidget.h)
katie_generate_obsolete(qdesignercustomwidgetinterface.h QtDesigner ../QtUiTools/customwidget.h)

katie_string_wrap("${KATIE_DEFINITIONS}" KATIE_DEFINITIONS)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/KatieConfig.cmake
    ${CMAKE_BINARY_DIR}/KatieConfig.cmake
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/KatieConfigVersion.cmake
    ${CMAKE_BINARY_DIR}/KatieConfigVersion.cmake
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/ld.so.conf.cmake
    ${CMAKE_BINARY_DIR}/ld.so.conf
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/profile.sh.cmake
    ${CMAKE_BINARY_DIR}/profile.sh
    @ONLY
)

install(
    FILES
    ${CMAKE_BINARY_DIR}/KatieConfig.cmake
    ${CMAKE_BINARY_DIR}/KatieConfigVersion.cmake
    ${CMAKE_SOURCE_DIR}/cmake/Qt4UseFile.cmake
    DESTINATION ${KATIE_CMAKE_PATH}
    COMPONENT Devel
)

install(
    EXPORT KatieTargets
    NAMESPACE Katie::
    DESTINATION ${KATIE_CMAKE_PATH}
    FILE KatieTargets.cmake
    COMPONENT Devel
)

install(
    FILES ${CMAKE_BINARY_DIR}/ld.so.conf
    DESTINATION ${KATIE_LDCONF_PATH}
    RENAME katie-${KATIE_PROCESSOR}.conf
    COMPONENT Runtime
)

install(
    FILES ${CMAKE_BINARY_DIR}/profile.sh
    DESTINATION ${KATIE_PROFILE_PATH}
    RENAME katie-${KATIE_PROCESSOR}.sh
    COMPONENT Runtime
)

katie_string_unwrap("${KATIE_COMPONENTS}" KATIE_COMPONENTS "Kt")
katie_string_unwrap("${KATIE_TOOLS}" KATIE_TOOLS)

# custom install targets to make packaging easier
if(KATIE_UNIFDEF)
    add_custom_target(install-devel
        COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Devel -P cmake_install.cmake
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        # installing from script does not execute other scripts
        COMMAND "${CMAKE_COMMAND}" -DUNIFDEF_EXECUTABLE="${KATIE_UNIFDEF}" -DHEADERS_DIRECTORY="${KATIE_HEADERS_PATH}" -P "${CMAKE_SOURCE_DIR}/cmake/modules/OptimizeHeaders.cmake"
    )
else()
    add_custom_target(install-devel
        COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Devel -P cmake_install.cmake
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endif()

add_custom_target(install-runtime
    DEPENDS ${KATIE_COMPONENTS}
    COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Runtime -P cmake_install.cmake
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

add_custom_target(install-tools
    DEPENDS ${KATIE_TOOLS}
    COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Tools -P cmake_install.cmake
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

add_custom_target(install-doc
    COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Doc -P cmake_install.cmake
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

feature_summary(
    WHAT ALL
    FATAL_ON_MISSING_REQUIRED_PACKAGES
)
