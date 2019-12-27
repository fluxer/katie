# Editors notes:
#
# * To find all FIXME/TODO related to the CMake build system (and those
#   inherited from the QMake build system) execute the following in the
#   top-level directory of the source tree:
#
#     find -name '*.cmake' -exec grep -E 'TODO|FIXME' {} +
#     find -name CMakeLists.txt -exec grep -E 'TODO|FIXME' {} +

cmake_minimum_required(VERSION 2.8.12.2 FATAL_ERROR)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "\nThe project should not be build in the source directory")
elseif(CMAKE_CROSSCOMPILING AND NOT KATIE_CROSS_EXEC)
    message(FATAL_ERROR "\nCross-compiling requires KATIE_CROSS_EXEC to be set to QEMU executable, e.g. qemu-arm-static")
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

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ toolkit derived from the Qt 4.8 framework")
set(CPACK_PACKAGE_VENDOR "Katie")
set(CPACK_PACKAGE_CONTACT "xakepa10@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LGPL_EXCEPTION.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "4")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_SOURCE_IGNORE_FILES "/minsize/;/debug/;/oldmake/;/apidocs/;/.git;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_STRIP_FILES TRUE)
if(NOT CMAKE_VERSION VERSION_LESS "3.1.0")
    set(CPACK_GENERATOR "TXZ")
    set(CPACK_SOURCE_GENERATOR "TXZ")
else()
    set(CPACK_GENERATOR "TGZ")
    set(CPACK_SOURCE_GENERATOR "TGZ")
endif()
set(CPACK_PACKAGE_FILE_NAME "Katie-4.9.0-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}")
include(CPack)

# enable symbols visibility preset for all targets
if(NOT CMAKE_VERSION VERSION_LESS "3.3.0")
    cmake_policy(SET CMP0063 NEW)
endif()

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckTypeSize)
include(FeatureSummary)
include(KatieMacros)
include(KatieBuildMacros)

# mostly relevant to the build process, also stored in the version file for CMake
set(KATIE_MAJOR "4")
set(KATIE_MINOR "9")
set(KATIE_MICRO "0")
set(KATIE_HEX "0x040900")
set(KATIE_VERSION "${KATIE_MAJOR}.${KATIE_MINOR}.${KATIE_MICRO}")
# this one is set via macro later, if possible. also used in config.cpp.cmake
set(KATIE_GIT "unstable-${KATIE_VERSION}")

# components and tools that will be build, changed depending on the requirements availability
# NOTE: excluding DesignerComponents as it was never used as public component, currently only
# used by the the disigner tool. if it is ever needed for something it can just be added here
# and that's it - the rest of the build system has everything in place.
set(KATIE_COMPONENTS "Core Gui Declarative Designer Network Sql Svg Xml Script ScriptTools Test UiTools")
set(KATIE_TOOLS "moc uic rcc lupdate lrelease lconvert designer qscript qtconfig")

# for config.cpp.cmake
string(TIMESTAMP KATIE_DATE "%Y-%m-%d")
set(KATIE_LICENSE "Open Source")
set(KATIE_PRODUCT "AwesomeSauce")

include(GNUInstallDirs)
set(KATIE_PREFIX_FULL "${CMAKE_INSTALL_PREFIX}" CACHE PATH "General installation prefix")
set(KATIE_HEADERS_FULL "${CMAKE_INSTALL_FULL_INCLUDEDIR}/katie" CACHE PATH "Headers installation path")
set(KATIE_LIBRARIES_FULL "${CMAKE_INSTALL_FULL_LIBDIR}" CACHE PATH "Libraries installation path")
set(KATIE_BINARIES_FULL "${CMAKE_INSTALL_FULL_BINDIR}" CACHE PATH "Binaries installation path")
set(KATIE_PLUGINS_FULL "${KATIE_LIBRARIES_FULL}/katie/plugins" CACHE PATH "Plugins installation path")
set(KATIE_IMPORTS_FULL "${KATIE_LIBRARIES_FULL}/katie/imports" CACHE PATH "Declerative imports installation path")
set(KATIE_DATA_FULL "${CMAKE_INSTALL_FULL_DATADIR}" CACHE PATH "Generic data installation path")
set(KATIE_TRANSLATIONS_FULL "${KATIE_DATA_FULL}/translations" CACHE PATH "Translations instllation path")
set(KATIE_SETTINGS_FULL "${CMAKE_INSTALL_FULL_SYSCONFDIR}/xdg" CACHE PATH "Settings runtime path")
set(KATIE_CMAKE_FULL "${KATIE_LIBRARIES_FULL}/cmake/Katie" CACHE PATH "CMake aware modules installation path")
set(KATIE_LDCONF_FULL "${KATIE_DATA_FULL}/ld.so.conf.d" CACHE PATH "Run-time linker/loader configs installation path")
set(KATIE_PROFILE_FULL "${KATIE_DATA_FULL}/profile.d" CACHE PATH "Shell profile scripts installation path")
set(KATIE_MAN_FULL "${CMAKE_INSTALL_FULL_MANDIR}" CACHE PATH "Manual pages installation path")
set(KATIE_APPLICATIONS_FULL "${KATIE_DATA_FULL}/applications" CACHE PATH "Desktop applications register installation path")
set(KATIE_PIXMAPS_FULL "${KATIE_DATA_FULL}/pixmaps" CACHE PATH "Desktop applications icon installation path")
set(KATIE_PKGCONFIG_FULL "${KATIE_LIBRARIES_FULL}/pkgconfig" CACHE PATH "pkg-config installation path")
set(KATIE_TOOLS_SUFFIX "" CACHE STRING "Tools (moc, uic, rcc, etc.) suffix")
katie_setup_paths()

# optional package
option(WITH_NIS "Build NIS support" ON)
add_feature_info(nis WITH_NIS "build NIS support")

option(WITH_CUPS "Build CUPS support" ON)
add_feature_info(cups WITH_CUPS "build CUPS support")

option(WITH_PNG "Build PNG support" ON)
add_feature_info(png WITH_PNG "build PNG support")

option(WITH_JPEG "Build JPEG support" ON)
add_feature_info(jpeg WITH_JPEG "build JPEG support")

option(WITH_TIFF "Build TIFF support" ON)
add_feature_info(tiff WITH_TIFF "build TIFF support")

option(WITH_MNG "Build MNG support" ON)
add_feature_info(mng WITH_MNG "build MNG support")

option(WITH_DBUS "Build DBus support" ON)
add_feature_info(dbus WITH_DBUS "build DBus support")

option(WITH_FONTCONFIG "Build Fontconfig support" ON)
add_feature_info(fontconfig WITH_FONTCONFIG "build Fontconfig support")

# TODO: enable when building against external harfbuzz works
option(WITH_HARFBUZZ "Build Harfbuzz support" OFF)
add_feature_info(harfbuzz WITH_HARFBUZZ "build Harfbuzz support")

option(WITH_ODBC "Build ODBC database plugin" ON)
add_feature_info(odbc WITH_ODBC "build ODBC support")

option(WITH_PSQL "Build PostgreSQL database plugin" ON)
add_feature_info(postgresql WITH_PSQL "build PostgreSQL support")

option(WITH_MYSQL "Build MySQL database plugin" ON)
add_feature_info(mysql WITH_MYSQL "build MySQL support")

option(WITH_SQLITE "Build SQLite database plugin" ON)
add_feature_info(sqlite WITH_SQLITE "build SQLite support")

option(WITH_RESOLV "Build resolv support" ON)
add_feature_info(resolv WITH_RESOLV "build resolv support")

option(WITH_NSL "Build NSL support" ON)
add_feature_info(nsl WITH_NSL "build NSL support")

option(WITH_UNWIND "Build Unwind support" OFF)
add_feature_info(unwind WITH_UNWIND "build Unwind support")

# optional features
option(KATIE_TESTS "Build automatic tests" OFF)
add_feature_info(tests KATIE_TESTS "build automatic tests")

option(KATIE_BENCHMARKS "Build automatic benchmarks" OFF)
add_feature_info(benchmarks KATIE_BENCHMARKS "build automatic benchmarks")

option(KATIE_UTILS "Build maintainance utilities" OFF)
add_feature_info(utils KATIE_UTILS "build maintainance utilities")

option(KATIE_ALLINONE "Build targets from single source file" OFF)
add_feature_info(allinone KATIE_ALLINONE "build targets from single source file")

option(WITH_ACCESSIBILITY "Build accessibility support" ON)
add_feature_info(accessibility WITH_ACCESSIBILITY "build accessibility support")

include(mkspecs/mkspecs.cmake)

# these conditionals are probably going to become default in the future
add_definitions(
    -DQT_POSIX_IPC
    -DQT_USE_SYSTEM_PROXIES
    # -DQT_STRICT_ITERATORS
    # -DQT_ASCII_CAST_WARNINGS
    # maybe one day, this forces it onto other projects which (most likely)
    # means source code adjustments!
    # -DQT_NO_CAST_FROM_ASCII
    # -DQT_NO_CAST_TO_ASCII

    # both gui and svg components require harfbuzz
    -DHB_EXPORT=Q_CORE_EXPORT
    -DQT_NO_OPENTYPE
)

# used in qconfig.h
set(QT_LARGEFILE_SUPPORT 64)
add_definitions(
    -D_LARGEFILE64_SOURCE
    -D_LARGEFILE_SOURCE
)

# some things just cannot be done right, yet
if(KATIE_ALLINONE)
    add_definitions(-DQT_ALLINONE)
endif()

# for symbols mangling while unwinding in core component
check_include_file_cxx(cxxabi.h HAVE_CXXABI)

# for 3rd party source used in test component
check_include_file(sys/time.h HAVE_SYS_TIME_H)

# used in qconfig.h
check_type_size(size_t QT_POINTER_SIZE)

if(KATIE_COMPILER MATCHES "(gcc|clang)")
    set(QT_VISIBILITY_AVAILABLE TRUE)
endif()

configure_file(
    ${CMAKE_SOURCE_DIR}/src/core/global/qconfig.h.cmake
    ${CMAKE_BINARY_DIR}/include/QtCore/qconfig.h
)
configure_file(
    ${CMAKE_SOURCE_DIR}/src/core/global/qconfig.cpp.cmake
    ${CMAKE_BINARY_DIR}/include/qconfig.cpp
)
configure_file(
    ${CMAKE_SOURCE_DIR}/src/core/global/qglobal.h.cmake
    ${CMAKE_BINARY_DIR}/include/QtCore/qglobal.h
)

# v1.2.0+ for compressBound()
find_package(ZLIB 1.2.0)
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

# v0.7.3+ required for ZSTD_getDecompressedSize()
find_package(ZSTD 0.7.3)
set_package_properties(ZSTD PROPERTIES
    PURPOSE "Required for compression support"
    DESCRIPTION "Fast real-time compression algorithm"
    URL "https://github.com/facebook/zstd"
    TYPE REQUIRED
)

find_package(OpenSSL)
set_package_properties(OpenSSL PROPERTIES
    PURPOSE "Required for HTTPS support"
    DESCRIPTION "Support for secure network communications (SSL and TLS)"
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

find_package(Git)
set_package_properties(Git PROPERTIES
    PURPOSE "Git checkout stored for the sake of bug reports"
    DESCRIPTION "Free and open source distributed version control system"
    URL "https://git-scm.com/"
    TYPE RECOMMENDED
)

find_package(NIS)
set_package_properties(NIS PROPERTIES
    PURPOSE "Required for Network Information Service support"
    DESCRIPTION "Standard C library specific NIS implementation"
    URL ""
    TYPE RECOMMENDED
)

find_package(Resolv)
set_package_properties(Resolv PROPERTIES
    PURPOSE "Required for domain resolving support"
    DESCRIPTION "Standard C library specific resolv implementation"
    URL ""
    TYPE RECOMMENDED
)

find_package(NSL)
set_package_properties(NSL PROPERTIES
    PURPOSE "Required for printing information support"
    DESCRIPTION "Standard C library specific NSL implementation"
    URL ""
    TYPE RECOMMENDED
)

find_package(PNG)
set_package_properties(PNG PROPERTIES
    PURPOSE "PNG format handler"
    DESCRIPTION "A collection of routines used to create PNG format graphics files"
    URL "http://www.libpng.org/pub/png/libpng.html"
    TYPE RECOMMENDED
)

find_package(JPEG)
set_package_properties(JPEG PROPERTIES
    PURPOSE "JPEG format handler"
    DESCRIPTION "JPEG image codec with accelerated baseline compression and decompression"
    URL "https://libjpeg-turbo.virtualgl.org/"
    TYPE RECOMMENDED
)

find_package(TIFF)
set_package_properties(TIFF PROPERTIES
    PURPOSE "TIFF format handler"
    DESCRIPTION "Library for manipulation of TIFF images"
    URL "http://www.libtiff.org/"
    TYPE RECOMMENDED
)

find_package(MNG)
set_package_properties(MNG PROPERTIES
    PURPOSE "MNG format handler"
    DESCRIPTION "THE reference library for reading, displaying, writing and examining Multiple-Image Network Graphics"
    URL "https://sourceforge.net/projects/libmng/"
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

# currently unused
find_package(HarfBuzz)
set_package_properties(HarfBuzz PROPERTIES
    PURPOSE "Currently none"
    DESCRIPTION "OpenType text shaping engine"
    URL "https://www.freedesktop.org/wiki/Software/HarfBuzz/"
    TYPE RECOMMENDED
)

# v1.2+ required for dbus_watch_get_unix_fd()
find_package(DBus 1.2)
set_package_properties(DBus PROPERTIES
    PURPOSE "Required for D-Bus support"
    DESCRIPTION "Message bus system, a simple way for applications to talk to one another"
    URL "https://www.freedesktop.org/wiki/Software/dbus"
    TYPE RECOMMENDED
)

# v3.5.0+ required for sqlite3_open_v2()
find_package(Sqlite 3.5.0)
set_package_properties(Sqlite PROPERTIES
    PURPOSE "Required for SQLite database support"
    DESCRIPTION "Software library that implements SQL database engine"
    URL "https://www.sqlite.org/"
    TYPE RECOMMENDED
)

find_package(ODBC)
set_package_properties(ODBC PROPERTIES
    PURPOSE "Required for ODBC database support"
    DESCRIPTION "The definitive standard for ODBC on non MS Windows platforms"
    URL "http://www.unixodbc.org"
    TYPE OPTIONAL
)

find_package(PostgreSQL)
set_package_properties(PostgreSQL PROPERTIES
    PURPOSE "Required for PostgreSQL database support"
    DESCRIPTION "The world's most advanced open source database"
    URL "https://www.postgresql.org/"
    TYPE OPTIONAL
)

find_package(MySQL)
set_package_properties(MySQL PROPERTIES
    PURPOSE "Required for MySQL database support"
    DESCRIPTION "The world's most popular open source database"
    URL "https://www.mysql.com/"
    TYPE OPTIONAL
)

find_package(Unwind)
set_package_properties(Unwind PROPERTIES
    PURPOSE "Required for stack traces on assert"
    DESCRIPTION "A (mostly) platform-independent unwind API"
    URL "https://www.nongnu.org/libunwind/index.html"
    TYPE OPTIONAL
)

if(KATIE_TESTS OR KATIE_BENCHMARKS)
    message(WARNING
        "\nUnless it is intentionall you should not enable testing, deploying tests builds"
        " is a bad idea."
    )
    enable_testing()
    add_definitions(-DQT_BUILD_INTERNAL)
endif()


configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/exec.sh.cmake
    ${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/exec.sh
    @ONLY
)
file(
    COPY ${CMAKE_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/exec.sh
    DESTINATION ${CMAKE_BINARY_DIR}
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# optional packages
if(NOT WITH_FONTCONFIG OR NOT FONTCONFIG_FOUND)
    katie_definition(-DQT_NO_FONTCONFIG)
endif()

if(NOT WITH_DBUS OR NOT DBUS_FOUND)
    katie_definition(-DQT_NO_DBUS)
else()
    set(KATIE_COMPONENTS "${KATIE_COMPONENTS} DBus")
    set(KATIE_TOOLS "${KATIE_TOOLS} qdbusxml2cpp qdbuscpp2xml qdbusviewer")
endif()

if(NOT WITH_CUPS OR NOT CUPS_FOUND)
    katie_definition(-DQT_NO_CUPS -DQT_NO_LPR)
endif()

if(NOT WITH_RESOLV OR NOT RESOLV_FOUND)
    katie_definition(-DQT_NO_RESOLV)
endif()

if(NOT WITH_NSL OR NOT NSL_FOUND)
    katie_definition(-DQT_NO_NSL)
endif()

if(NOT WITH_NIS OR NOT NIS_FOUND)
    katie_definition(-DQT_NO_NIS)
endif()

if(NOT WITH_PNG OR NOT PNG_FOUND)
    katie_definition(-DQT_NO_IMAGEFORMAT_PNG)
endif()

if(NOT WITH_JPEG OR NOT JPEG_FOUND)
    katie_definition(-DQT_NO_IMAGEFORMAT_JPEG)
endif()

if(NOT WITH_MNG OR NOT MNG_FOUND)
    katie_definition(-DQT_NO_IMAGEFORMAT_MNG)
endif()

if(NOT WITH_TIFF OR NOT TIFF_FOUND)
    katie_definition(-DQT_NO_IMAGEFORMAT_TIFF)
endif()

if(NOT WITH_UNWIND OR NOT UNWIND_FOUND OR NOT HAVE_CXXABI)
    katie_definition(-DQT_NO_UNWIND)
endif()

# conditional features
if(NOT WITH_ACCESSIBILITY)
    katie_definition(-DQT_NO_ACCESSIBILITY)
endif()

# keep in sync with the gui component CMake file, only definitions setup is needed here
foreach(x11ext Xshape Xinerama Xrandr Xrender Xfixes Xcursor Xinput)
    if(NOT X11_${x11ext}_FOUND)
        message(WARNING "The X11 ${x11ext} extension was not found")
        string(TOUPPER "${x11ext}" upperext)
        katie_definition(-DQT_NO_${upperext})
    endif()
endforeach()
if(NOT X11_Xext_FOUND)
    message(WARNING "The X11 Xext extension was not found")
    katie_definition(-DQT_NO_XSYNC -DQT_NO_XSHAPE -DQT_NO_XSHM)
endif()
if(NOT X11_SM_FOUND)
    message(WARNING "X11 SM was not found thus disabling session manager support")
    katie_definition(-DQT_NO_SESSIONMANAGER)
endif()

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
add_subdirectory(src/sql)
add_subdirectory(src/svg)
add_subdirectory(src/test)
add_subdirectory(src/uitools)

add_subdirectory(src/tools)

add_subdirectory(translations)

if(KATIE_TESTS OR KATIE_BENCHMARKS)
    add_subdirectory(tests)
endif()

if(KATIE_UTILS)
    add_subdirectory(util)
endif()

# the macro calls bellow are generted via the incfsck script
katie_generate_obsolete(QAbstractFileEngineHandler QtCore qabstractfileengine.h)
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
katie_generate_obsolete(QActionEvent QtGui qevent.h)
katie_generate_obsolete(QAnimationDriver QtGui qabstractanimation.h)
katie_generate_obsolete(QArgument QtCore qobjectdefs.h)
katie_generate_obsolete(QAtomicInt QtCore qatomic.h)
katie_generate_obsolete(QAtomicPointer QtCore qatomic.h)
katie_generate_obsolete(QBasicAtomicInt QtCore qbasicatomic.h)
katie_generate_obsolete(QBitRef QtCore qbitarray.h)
katie_generate_obsolete(QBool QtCore qglobal.h)
katie_generate_obsolete(QByteRef QtCore qbytearray.h)
katie_generate_obsolete(QCharRef QtCore qstring.h)
katie_generate_obsolete(QChildEvent QtCore qcoreevent.h)
katie_generate_obsolete(QCloseEvent QtGui qevent.h)
katie_generate_obsolete(QConicalGradient QtGui qbrush.h)
katie_generate_obsolete(QContextMenuEvent QtGui qevent.h)
katie_generate_obsolete(QContiguousCacheData QtCore qcontiguouscache.h)
katie_generate_obsolete(QDateEdit QtGui qdatetimeedit.h)
katie_generate_obsolete(QDate QtCore qdatetime.h)
katie_generate_obsolete(QDBusAbstractInterfaceBase QtDBus qdbusabstractinterface.h)
katie_generate_obsolete(QDBusObjectPath QtDBus qdbusextratypes.h)
katie_generate_obsolete(QDBusPendingCallWatcher QtDBus qdbuspendingcall.h)
katie_generate_obsolete(QDBusPendingReplyData QtDBus qdbuspendingreply.h)
katie_generate_obsolete(QDBusSignature QtDBus qdbusextratypes.h)
katie_generate_obsolete(QDBusVariant QtDBus qdbusextratypes.h)
katie_generate_obsolete(QDeclarativeListProperty QtDeclarative qdeclarativelist.h)
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
katie_generate_obsolete(QEventSizeOfChecker QtTest qtestspontaneevent.h)
katie_generate_obsolete(QExplicitlySharedDataPointer QtCore qsharedpointer.h)
katie_generate_obsolete(QExtensionFactory QtDesigner default_extensionfactory.h)
katie_generate_obsolete(QFileInfoList QtCore qfileinfo.h)
katie_generate_obsolete(QFlag QtCore qglobal.h)
katie_generate_obsolete(QFlags QtCore qglobal.h)
katie_generate_obsolete(QFocusEvent QtGui qevent.h)
katie_generate_obsolete(QFontMetricsF QtGui qfontmetrics.h)
katie_generate_obsolete(QFutureInterfaceBase QtCore qfutureinterface.h)
katie_generate_obsolete(QFutureIterator QtCore qfuture.h)
katie_generate_obsolete(QGenericArgument QtCore qobjectdefs.h)
katie_generate_obsolete(QGestureEvent QtGui qevent.h)
katie_generate_obsolete(QGlobalStatic QtCore qglobal.h)
katie_generate_obsolete(QGradient QtGui qbrush.h)
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
katie_generate_obsolete(QHashIterator QtCore qhash.h)
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
katie_generate_obsolete(QInputContextFactoryInterface QtGui qinputcontextplugin.h)
katie_generate_obsolete(QInputEvent QtGui qevent.h)
katie_generate_obsolete(QInputMethodEvent QtGui qevent.h)
katie_generate_obsolete(QIntValidator QtGui qvalidator.h)
katie_generate_obsolete(QItemEditorCreatorBase QtGui qitemeditorfactory.h)
katie_generate_obsolete(QItemEditorCreator QtGui qitemeditorfactory.h)
katie_generate_obsolete(QItemSelection QtGui qitemselectionmodel.h)
katie_generate_obsolete(QItemSelectionRange QtGui qitemselectionmodel.h)
katie_generate_obsolete(QJsonParseError QtCore qjsondocument.h)
katie_generate_obsolete(QKeyEvent QtGui qevent.h)
katie_generate_obsolete(QLatin1Char QtCore qchar.h)
katie_generate_obsolete(QLatin1String QtCore qstring.h)
katie_generate_obsolete(QLinearGradient QtGui qbrush.h)
katie_generate_obsolete(QLineF QtCore qline.h)
katie_generate_obsolete(QLinkedListIterator QtCore qlinkedlist.h)
katie_generate_obsolete(QLinkedListNode QtCore qlinkedlist.h)
katie_generate_obsolete(QListIterator QtCore qlist.h)
katie_generate_obsolete(QListWidgetItem QtGui qlistwidget.h)
katie_generate_obsolete(QMapIterator QtCore qmap.h)
katie_generate_obsolete(QMetaClassInfo QtCore qmetaobject.h)
katie_generate_obsolete(QMetaEnum QtCore qmetaobject.h)
katie_generate_obsolete(QMetaMethod QtCore qmetaobject.h)
katie_generate_obsolete(QMetaProperty QtCore qmetaobject.h)
katie_generate_obsolete(QMetaTypeId2 QtCore qmetatype.h)
katie_generate_obsolete(QModelIndexList QtCore qabstractitemmodel.h)
katie_generate_obsolete(QModelIndex QtCore qabstractitemmodel.h)
katie_generate_obsolete(QMouseEvent QtGui qevent.h)
katie_generate_obsolete(QMoveEvent QtGui qevent.h)
katie_generate_obsolete(QMultiHash QtCore qhash.h)
katie_generate_obsolete(QMultiMap QtCore qmap.h)
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
katie_generate_obsolete(QNetworkConfigurationManager QtNetwork qnetworkconfigmanager.h)
katie_generate_obsolete(QNetworkProxyFactory QtNetwork qnetworkproxy.h)
katie_generate_obsolete(QNetworkProxyQuery QtNetwork qnetworkproxy.h)
katie_generate_obsolete(QObjectList QtCore qobject.h)
katie_generate_obsolete(QPainterPathStroker QtGui qpainterpath.h)
katie_generate_obsolete(QPaintEvent QtGui qevent.h)
katie_generate_obsolete(QPersistentModelIndex QtCore qabstractitemmodel.h)
katie_generate_obsolete(Q_PID QtCore qprocess.h)
katie_generate_obsolete(QPinchGesture QtGui qgesture.h)
katie_generate_obsolete(QPlainTextDocumentLayout QtGui qplaintextedit.h)
katie_generate_obsolete(QPointF QtCore qpoint.h)
katie_generate_obsolete(QPolygonF QtGui qpolygon.h)
katie_generate_obsolete(QProcessEnvironment QtCore qprocess.h)
katie_generate_obsolete(QRadialGradient QtGui qbrush.h)
katie_generate_obsolete(QReadLocker QtCore qreadwritelock.h)
katie_generate_obsolete(QRectF QtCore qrect.h)
katie_generate_obsolete(QRegExpValidator QtGui qvalidator.h)
katie_generate_obsolete(QResizeEvent QtGui qevent.h)
katie_generate_obsolete(QScopedArrayPointer QtCore qscopedpointer.h)
katie_generate_obsolete(QSharedDataPointer QtCore qshareddata.h)
katie_generate_obsolete(QShortcutEvent QtGui qevent.h)
katie_generate_obsolete(QShowEvent QtGui qevent.h)
katie_generate_obsolete(QSizeF QtCore qsize.h)
katie_generate_obsolete(QSpacerItem QtGui qlayoutitem.h)
katie_generate_obsolete(QSplitterHandle QtGui qsplitter.h)
katie_generate_obsolete(QSpontaneKeyEvent QtTest qtestspontaneevent.h)
katie_generate_obsolete(QSqlDriverCreatorBase QtSql qsqldatabase.h)
katie_generate_obsolete(QSqlDriverCreator QtSql qsqldatabase.h)
katie_generate_obsolete(QSqlDriverFactoryInterface QtSql qsqldriverplugin.h)
katie_generate_obsolete(QSqlRelation QtSql qsqlrelationaltablemodel.h)
katie_generate_obsolete(QStandardItem QtGui qstandarditemmodel.h)
katie_generate_obsolete(QStringListIterator QtCore qstringlist.h)
katie_generate_obsolete(QStyleHintReturnMask QtGui qstyleoption.h)
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
katie_generate_obsolete(QStyleOptionSlider QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionSpinBox QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabBarBase QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTab QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabV3 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabWidgetFrame QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTabWidgetFrameV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionTitleBar QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionToolBar QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionToolButton QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItem QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItemV2 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItemV3 QtGui qstyleoption.h)
katie_generate_obsolete(QStyleOptionViewItemV4 QtGui qstyleoption.h)
katie_generate_obsolete(QSysInfo QtCore qglobal.h)
katie_generate_obsolete(QTableWidgetItem QtGui qtablewidget.h)
katie_generate_obsolete(QtAlgorithms QtCore qalgorithms.h)
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
katie_generate_obsolete(QTextBlock QtGui qtextobject.h)
katie_generate_obsolete(QTextBlockUserData QtGui qtextobject.h)
katie_generate_obsolete(QTextCharFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextDecoder QtCore qtextcodec.h)
katie_generate_obsolete(QTextFrame QtGui qtextobject.h)
katie_generate_obsolete(QTextLength QtGui qtextformat.h)
katie_generate_obsolete(QTextLine QtGui qtextlayout.h)
katie_generate_obsolete(QTextListFormat QtGui qtextformat.h)
katie_generate_obsolete(QTextStreamManipulator QtCore qtextstream.h)
katie_generate_obsolete(QTextTableCell QtGui qtexttable.h)
katie_generate_obsolete(QTextTableFormat QtGui qtextformat.h)
katie_generate_obsolete(QtGlobal QtCore qglobal.h)
katie_generate_obsolete(QTimeEdit QtGui qdatetimeedit.h)
katie_generate_obsolete(QTime QtCore qdatetime.h)
katie_generate_obsolete(QTimerEvent QtCore qcoreevent.h)
katie_generate_obsolete(QTouchEvent QtGui qevent.h)
katie_generate_obsolete(QtPlugin QtCore qplugin.h)
katie_generate_obsolete(Qt QtCore qnamespace.h)
katie_generate_obsolete(QTreeWidgetItem QtGui qtreewidget.h)
katie_generate_obsolete(QtTestGui QtTest qtest_gui.h)
katie_generate_obsolete(QTypeInfo QtCore qglobal.h)
katie_generate_obsolete(QUndoCommand QtGui qundostack.h)
katie_generate_obsolete(QVariantHash QtCore qvariant.h)
katie_generate_obsolete(QVariantList QtCore qvariant.h)
katie_generate_obsolete(QVariantMap QtCore qvariant.h)
katie_generate_obsolete(QVBoxLayout QtGui qboxlayout.h)
katie_generate_obsolete(QVectorData QtCore qvector.h)
katie_generate_obsolete(QWeakPointer QtCore qsharedpointer.h)
katie_generate_obsolete(QWheelEvent QtGui qevent.h)
katie_generate_obsolete(QWidgetItem QtGui qlayoutitem.h)
katie_generate_obsolete(QWidgetList QtGui qwindowdefs.h)
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
katie_generate_obsolete(customwidget.h QtDesigner ../QtUiTools/customwidget.h)
katie_generate_obsolete(qdesignercustomwidgetinterface.h QtDesigner ../QtUiTools/customwidget.h)

katie_string_wrap("${KATIE_DEFINITIONS}" KATIE_DEFINITIONS)
katie_git_checkout("${GIT_EXECUTABLE}" KATIE_GIT)

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
    ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.conf
    @ONLY
)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/profile.sh.cmake
    ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.sh
    @ONLY
)

install(
    DIRECTORY mkspecs
    DESTINATION ${KATIE_CMAKE_RELATIVE}
    COMPONENT Devel
)

install(
    FILES
    ${CMAKE_BINARY_DIR}/KatieConfig.cmake
    ${CMAKE_BINARY_DIR}/KatieConfigVersion.cmake
    ${CMAKE_SOURCE_DIR}/cmake/Qt4UseFile.cmake
    ${CMAKE_SOURCE_DIR}/cmake/modules/KatieMacros.cmake
    DESTINATION ${KATIE_CMAKE_RELATIVE}
    COMPONENT Devel
)

install(
    EXPORT KatieLibraryTargets
    NAMESPACE Katie::
    DESTINATION ${KATIE_CMAKE_RELATIVE}
    FILE KatieLibraryTargets.cmake
    COMPONENT Devel
)

install(
    EXPORT KatieBinaryTargets
    NAMESPACE Katie::
    DESTINATION ${KATIE_CMAKE_RELATIVE}
    FILE KatieBinaryTargets.cmake
    COMPONENT Devel
)

install(
    FILES ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.conf
    DESTINATION ${KATIE_LDCONF_RELATIVE}
    COMPONENT Runtime
)

install(
    FILES ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.sh
    DESTINATION ${KATIE_PROFILE_RELATIVE}
    COMPONENT Runtime
)

katie_string_unwrap("${KATIE_COMPONENTS}" KATIE_COMPONENTS "Kt")
katie_string_unwrap("${KATIE_TOOLS}" KATIE_TOOLS)

add_custom_target(install-devel
    COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Devel -P cmake_install.cmake
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)

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

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES) 
