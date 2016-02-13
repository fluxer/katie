# Editors notes:
#
# * To find all FIXME/TODO related to the CMake build system (and those
#   inherited from the autotools build system) execute the following in the
#   top-level directory of the source tree:
#
#     find -name '*.cmake' -exec grep -E 'TODO|FIXME' {} +
#     find -name CMakeLists.txt -exec grep -E 'TODO|FIXME' {} +

project(Katie C CXX ASM)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_AUTOMOC FALSE)
set(CMAKE_AUTORCC FALSE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/staticlib")

include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckTypeSize)
include(FeatureSummary)
include(KatieMacros)
include(KatieBuildMacros)

# optional package features
option(WITH_OPENSSL "Build openssl support" ON)
add_feature_info(openssl WITH_OPENSSL "an open source something")

option(WITH_ZLIB "Build zlib support" ON)
add_feature_info(zlib WITH_ZLIB "an open source something")

option(WITH_NIS "Build nis support" ON)
add_feature_info(nis WITH_NIS "an open source something")

option(WITH_CUPS "Build cups support" ON)
add_feature_info(cups WITH_CUPS "an open source something")

option(WITH_ALSA "Build alsa support" ON)
add_feature_info(alsa WITH_ALSA "an open source something")

option(WITH_PNG "Build png support" ON)
add_feature_info(png WITH_PNG "an open source something")

option(WITH_JPEG "Build jpeg support" ON)
add_feature_info(jpeg WITH_JPEG "an open source something")

option(WITH_TIFF "Build tiff support" ON)
add_feature_info(tiff WITH_TIFF "an open source something")

option(WITH_MNG "Build mng support" ON)
add_feature_info(tiff WITH_MNG "an open source something")

option(WITH_NAS "Build nas support" ON)
add_feature_info(nas WITH_NAS "an open source something")

option(WITH_ICONV "Build iconv support" ON)
add_feature_info(iconv WITH_ICONV "an open source something")

option(WITH_GLIB2 "Build glib2 support" ON)
add_feature_info(glib2 WITH_GLIB2 "an open source something")

option(WITH_DBUS "Build dbus support" ON)
add_feature_info(dbus WITH_DBUS "an open source something")

option(WITH_GTK2 "Build gtk2 support" ON)
add_feature_info(gtk2 WITH_GTK2 "an open source something")

option(WITH_EGL "Build egl support" OFF)
add_feature_info(egl WITH_EGL "an open source something")

option(WITH_OPENGL "Build opengl support" ON)
add_feature_info(opengl WITH_OPENGL "an open source something")

option(WITH_FREETYPE "Build freetype2 support" ON)
add_feature_info(freetype2 WITH_FREETYPE "an open source something")

option(WITH_FONTCONFIG "Build fontconfig support" ON)
add_feature_info(fontconfig WITH_FONTCONFIG "an open source something")

option(WITH_ICU "Build icu support" ON)
add_feature_info(icu WITH_ICU "an open source something")

# TODO: enable when building against external harfbuzz works
option(WITH_HARFBUZZ "Build harfbuzz support" OFF)
add_feature_info(harfbuzz WITH_HARFBUZZ "an open source something")

option(WITH_ODBC "Build ODBC database plugin" ON)
add_feature_info(postgresql WITH_ODBC "an open source something")

option(WITH_PSQL "Build PostgreSQL database plugin" ON)
add_feature_info(postgresql WITH_PSQL "the world's most advanced open source database")

option(WITH_MYSQL "Build MySQL database plugin" ON)
add_feature_info(mysql WITH_MYSQL "the world's most popular open source database")

option(WITH_SQLITE "Build SQLite database plugin" ON)
add_feature_info(sqlite WITH_SQLITE "the world's most popular open source database")

option(WITH_VLC "Build VLC Phonon backend" ON)
add_feature_info(sqlite WITH_VLC "VLC C API Library")

option(WITH_X11 "Build X11/X.Org support" ON)
add_feature_info(x11 WITH_X11 "an open source something")

# katie features
option(KATIE_BOOTSTRAP "Build only core tools" OFF)
add_feature_info(bootstrap KATIE_BOOTSTRAP "internal build option")

option(WITH_OPENGLES1 "Build opengles1 support" OFF)
add_feature_info(opengles1 WITH_OPENGLES1 "an open source something")

option(WITH_OPENGLES2 "Build opengles2 support" OFF)
add_feature_info(opengles2 WITH_OPENGLES2 "an open source something")

option(WITH_ACCESSIBILITY "Build accessibility support" ON)
add_feature_info(accessibility WITH_ACCESSIBILITY "an open source something")

option(WITH_QTOPIA "Build qtopia support" OFF)
add_feature_info(qtopia WITH_QTOPIA "an open source something")

# windows
option(WITH_ICD "Build icd support (Windows)" ON)
add_feature_info(icd WITH_ICD "an closed source something")

option(WITH_DIRECTWRITE "Build DirectWrite support (Windows)" OFF)
add_feature_info(directwrite WITH_DIRECTWRITE "an closed source something")

# mac
option(WITH_COREWLAN "Build corewlan support (Mac)" ON)
add_feature_info(corewlan WITH_COREWLAN "an closed source something")

include(mkspecs/mkspecs.cmake)

katie_generate_string("qt_lcnsuser=${QT_LICENSE}" "QT_LICENSE_STR" "")
katie_generate_string("qt_lcnsprod=${QT_PRODUCT}" "QT_PRODUCT_STR" "")
katie_generate_string("qt_prfxpath=${QT_PREFIX_PATH}" "QT_PREFIX_PATH_STR" "")
katie_generate_string("qt_hdrspath=${QT_HEADERS_PATH}" "QT_HEADERS_PATH_STR" "QT_HEADERS_PATH_INST")
katie_generate_string("qt_libspath=${QT_LIBRARIES_PATH}" "QT_LIBRARIES_PATH_STR" "QT_LIBRARIES_PATH_INST")
katie_generate_string("qt_binspath=${QT_BINARIES_PATH}" "QT_BINARIES_PATH_STR" "QT_BINARIES_PATH_INST")
katie_generate_string("qt_plugpath=${QT_PLUGINS_PATH}" "QT_PLUGINS_PATH_STR" "QT_PLUGINS_PATH_INST")
katie_generate_string("qt_impspath=${QT_IMPORTS_PATH}" "QT_IMPORTS_PATH_STR" "QT_IMPORTS_PATH_INST")
katie_generate_string("qt_datapath=${QT_DATA_PATH}" "QT_DATA_PATH_STR" "QT_DATA_PATH_INST")
katie_generate_string("qt_trnspath=${QT_TRANSLATIONS_PATH}" "QT_TRANSLATIONS_PATH_STR" "QT_TRANSLATIONS_PATH_INST")
katie_generate_string("qt_stngpath=${QT_SETTINGS_PATH}" "QT_SETTINGS_PATH_STR" "QT_SETTINGS_PATH_INST")
katie_generate_string("qt_docspath=${QT_DOCUMENTATION_PATH}" "QT_DOCUMENTATION_PATH_STR" "QT_DOCUMENTATION_PATH_INST")
katie_generate_string("${QT_EXAMPLES_PATH}" "QT_EXAMPLES_PATH_STR" "QT_EXAMPLES_PATH_INST")
katie_generate_string("${QT_DEMOS_PATH}" "QT_DEMOS_PATH_STR" "QT_DEMOS_PATH_INST")


set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ toolkit derived from the Qt 4.8 framework")
set(CPACK_PACKAGE_VENDOR "Katie")
set(CPACK_PACKAGE_CONTACT "${KATIE_BUGREPORT}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${KATIE_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${KATIE_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${KATIE_MICRO})
set(CPACK_SOURCE_IGNORE_FILES "/build/;/mingw/;/.git;${CPACK_SOURCE_IGNORE_FILES}")
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_SOURCE_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TXZ")
    set(CPACK_SOURCE_GENERATOR "TXZ")
endif()
include(CPack)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
add_definitions(
    -DKATIE_DATE="${KATIE_DATE}"
    # maybe one day, use with caution!
    # -DQT_ASCII_CAST_WARNINGS
    # -DQT_USE_QSTRINGBUILDER
    # -DQT_NO_CAST_FROM_ASCII
    # -DQT_NO_CAST_TO_ASCII
)

# this is how Qt4 does it
if(NOT "${KATIE_PLATFORM}" STREQUAL "integrity" AND NOT MINGW)
    set(QT_LARGEFILE_SUPPORT 64)
    add_definitions(
        -D_LARGEFILE64_SOURCE
        -D_LARGEFILE_SOURCE
    )
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

find_package(PythonInterp)
set_package_properties(PythonInterp PROPERTIES
    PURPOSE "UI class maps generator script"
    DESCRIPTION "Programming language that lets you work quickly"
    URL "https://www.python.org/"
    TYPE REQUIRED
)

find_package(OpenSSL)
set_package_properties(OpenSSL PROPERTIES
    PURPOSE "Required for HTTPS support"
    DESCRIPTION "Support for secure network communications (SSL and TLS)"
    URL "http://openssl.org"
    TYPE RECOMMENDED
)

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES
    PURPOSE "Required for compression support"
    DESCRIPTION "A Massively Spiffy Yet Delicately Unobtrusive Compression Library"
    URL "http://zlib.net"
    TYPE RECOMMENDED
)

find_package(NIS)
set_package_properties(NIS PROPERTIES
    PURPOSE "Required for Network Information Service support"
    DESCRIPTION "Standard C library specific NIS implementation"
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
    URL "http://libjpeg-turbo.virtualgl.org/"
    TYPE RECOMMENDED
)

find_package(TIFF)
set_package_properties(TIFF PROPERTIES
    PURPOSE "TIFF format handler"
    DESCRIPTION "Library for manipulation of TIFF images"
    URL "http://www.remotesensing.org/libtiff/"
    TYPE RECOMMENDED
)

find_package(MNG)
set_package_properties(MNG PROPERTIES
    PURPOSE "MNG format handler"
    DESCRIPTION "THE reference library for reading, displaying, writing and examining Multiple-Image Network Graphics"
    URL "http://sourceforge.net/projects/libmng/"
    TYPE RECOMMENDED
)

find_package(NAS)
set_package_properties(NAS PROPERTIES
    PURPOSE "Basic audio support over wire"
    DESCRIPTION "Network transparent, client/server audio transport system"
    URL "http://www.radscan.com/nas.html"
    TYPE RECOMMENDED
)

find_package(Iconv)
set_package_properties(Iconv PROPERTIES
    PURPOSE "Iconv support"
    DESCRIPTION "iconv() implementation, for use on systems which don't have one or lack Unicode support"
    URL "http://www.gnu.org/software/libiconv/"
    TYPE RECOMMENDED
)

find_package(Cups)
set_package_properties(Cups PROPERTIES
    PURPOSE "Required for printing support"
    DESCRIPTION "CUPS is the standards-based, open source printing system"
    URL "http://www.cups.org"
    TYPE RECOMMENDED
)

find_package(ALSA)
set_package_properties(ALSA PROPERTIES
    PURPOSE "Required for ALSA support"
    DESCRIPTION "Advanced Linux Sound Architecture"
    URL "http://www.alsa-project.org"
    TYPE RECOMMENDED
)

# old versions of CMake ship bogus module, always use the custom one
cmake_policy(PUSH)
cmake_policy(SET CMP0017 OLD)
find_package(Freetype)
set_package_properties(Freetype PROPERTIES
    PURPOSE "Required for fonts configuration support"
    DESCRIPTION "Freely available software library to render fonts"
    URL "http://www.freetype.org"
    TYPE RECOMMENDED
)
cmake_policy(POP)

find_package(Fontconfig)
set_package_properties(Fontconfig PROPERTIES
    PURPOSE "Required for fonts configuration support"
    DESCRIPTION "Library for configuring and customizing font access"
    URL "http://www.freedesktop.org/wiki/Software/fontconfig"
    TYPE RECOMMENDED
)

find_package(ICU)
set_package_properties(ICU PROPERTIES
    PURPOSE "ICU locales support"
    DESCRIPTION "Mature, widely used libraries providing Unicode and Globalization support"
    URL "http://site.icu-project.org/"
    TYPE RECOMMENDED
)

# currently unused
find_package(HarfBuzz)
set_package_properties(HarfBuzz PROPERTIES
    PURPOSE "Currently none"
    DESCRIPTION "OpenType text shaping engine"
    URL "http://www.freedesktop.org/wiki/Software/HarfBuzz/"
    TYPE RECOMMENDED
)

find_package(GLib2)
set_package_properties(GLib2 PROPERTIES
    PURPOSE "Required for glib mainloop support"
    DESCRIPTION "Core application building blocks for libraries and applications written in C"
    URL "https://developer.gnome.org/glib"
    TYPE RECOMMENDED
)

find_package(DBus)
set_package_properties(DBus PROPERTIES
    PURPOSE "Required for D-Bus support"
    DESCRIPTION "Message bus system, a simple way for applications to talk to one another"
    URL "http://www.freedesktop.org/wiki/Software/dbus"
    TYPE RECOMMENDED
)

find_package(GTK2)
set_package_properties(GTK2 PROPERTIES
    PURPOSE "Required for GTK style and application integration support"
    DESCRIPTION "Multi-platform toolkit for creating graphical user interfaces"
    URL "http://www.gtk.org"
    TYPE RECOMMENDED
)

find_package(EGL)
set_package_properties(EGL PROPERTIES
    PURPOSE "Required for EGL support"
    DESCRIPTION "Platform dependendant EGL implementation"
    URL ""
    TYPE RECOMMENDED
)

find_package(OpenGL)
set_package_properties(OpenGL PROPERTIES
    PURPOSE "Required for GL/EGL support"
    DESCRIPTION "Platform dependendant OpenGL implementation"
    URL ""
    TYPE RECOMMENDED
)

find_package(Wayland)
set_package_properties(Wayland PROPERTIES
    PURPOSE "Required for Wayland support"
    DESCRIPTION "Wayland is intended as a simpler replacement for X"
    URL "http://wayland.freedesktop.org/"
    TYPE OPTIONAL
)

find_package(ODBC)
set_package_properties(ODBC PROPERTIES
    PURPOSE "Required for ODBC database support"
    DESCRIPTION "The definitive standard for ODBC on non MS Windows platforms"
    URL "http://www.unixodbc.org"
    TYPE RECOMMENDED
)

find_package(PostgreSQL)
set_package_properties(PostgreSQL PROPERTIES
    PURPOSE "Required for PostgreSQL database support"
    DESCRIPTION "The world's most advanced open source database"
    URL "http://www.postgresql.org"
    TYPE OPTIONAL
)

find_package(MySQL)
set_package_properties(MySQL PROPERTIES
    PURPOSE "Required for MySQL database support"
    DESCRIPTION "The world's most popular open source database"
    URL "http://www.mysql.com"
    TYPE OPTIONAL
)

find_package(Sqlite)
set_package_properties(Sqlite PROPERTIES
    PURPOSE "Required for SQLite database support"
    DESCRIPTION "Software library that implements SQL database engine"
    URL "https://www.sqlite.org/"
    TYPE RECOMMENDED
)

set(VLC_MIN_VERSION "2.1.0")
find_package(VLC)
set_package_properties(VLC PROPERTIES
    PURPOSE "Required for VLC Phonon backend"
    DESCRIPTION "VLC C API Library"
    URL "http://www.videolan.org/vlc/"
    TYPE RECOMMENDED
)

# module for Qt Platform Abstraction
# find_package(QPA)

if(UNIX AND NOT ${KATIE_PLATFORM} STREQUAL "mac" AND NOT KATIE_BOOTSTRAP)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)

    find_package(X11 COMPONENTS ICE SM Xcursor Xext Xfixes Xi Xinerama Xrandr Xrender X11 Xtst Xt)
    set_package_properties(X11 PROPERTIES
        PURPOSE "Required for X11/X.Org integration support"
        DESCRIPTION "Open source implementation of the X Window System"
        URL "http://www.x.org"
        TYPE REQUIRED
    )

    set_package_properties(Fontconfig PROPERTIES
        TYPE REQUIRED
    )

    set_package_properties(Freetype PROPERTIES
        TYPE REQUIRED
    )

    set_package_properties(X11 PROPERTIES
        TYPE REQUIRED
    )
endif()

# various sources use #include <shared/blah.h>
include_directories(${CMAKE_SOURCE_DIR}/src)

if(ENABLE_TESTING)
    message(WARNING "Unless it is intentionall you should not enable testing")
    enable_testing()
    add_definitions(-DQT_BUILD_INTERNAL)
endif()

if(NOT WTIH_EGL OR NOT EGL_FOUND)
    katie_definition(-DQT_NO_EGL)
endif()

if(NOT WITH_OPENSSL OR NOT OPENSSL_FOUND)
    katie_definition(-DQT_NO_OPENSSL)
endif()

if(NOT WITH_FREETYPE OR NOT FREETYPE_FOUND)
    set(WITH_FONTCONFIG OFF)
    katie_definition(-DQT_NO_FREETYPE)
endif()

if(NOT WITH_FONTCONFIG OR NOT FONTCONFIG_FOUND)
    katie_definition(-DQT_NO_FONTCONFIG)
endif()

if(NOT WITH_DBUS OR NOT DBUS_FOUND)
    katie_definition(-DQT_NO_DBUS)
endif()

if(NOT WITH_GLIB2 OR NOT GLIB2_FOUND)
    katie_definition(-DQT_NO_GLIB)
endif()

if(NOT WITH_CUPS OR NOT CUPS_FOUND)
    katie_definition(-DQT_NO_CUPS -DQT_NO_LPR)
endif()

if(NOT WITH_NIS OR NOT NIS_FOUND)
    katie_definition(-DQT_NO_NIS)
endif()

if(NOT WITH_DIRECTWRITE)
    katie_definition(-DQT_NO_DIRECTWRITE)
endif()

if(NOT WITH_OPENGL OR NOT OPENGL_FOUND)
    katie_definition(-DQT_NO_OPENGL)
endif()

if(NOT WITH_NAS OR NOT NAS_FOUND)
    katie_definition(-DQT_NO_NAS)
endif()

# semi-shared
include(src/shared/linguist/linguist.cmake)
include(src/shared/bootstrap.cmake)
if(NOT KATIE_BOOTSTRAP)
    include(src/shared/deviceskin/deviceskin.cmake)
    include(src/shared/help/help.cmake)
    include(src/shared/qtgradienteditor/qtgradienteditor.cmake)
    include(src/shared/qtpropertybrowser/qtpropertybrowser.cmake)
    include(src/shared/findwidget/findwidget.cmake)
endif()

# major components
add_subdirectory(src/core)
add_subdirectory(src/xml)
if(NOT KATIE_BOOTSTRAP)
    if(WITH_DBUS AND DBUS_FOUND)
        add_subdirectory(src/dbus)
    endif()
    add_subdirectory(src/declarative)
    add_subdirectory(src/designer)
    add_subdirectory(src/gui)
    add_subdirectory(src/help)
    add_subdirectory(src/imports)
    add_subdirectory(src/multimedia)
    add_subdirectory(src/network)
    if(WITH_OPENGL AND OPENGL_FOUND)
        add_subdirectory(src/opengl)
    endif()
    add_subdirectory(src/phonon)
    add_subdirectory(src/plugins)
    add_subdirectory(src/script)
    add_subdirectory(src/scripttools)
    add_subdirectory(src/sql)
    add_subdirectory(src/svg)
    add_subdirectory(src/test)
    add_subdirectory(src/uitools)
    if(KATIE_PLATFORM MATCHES "(win32|wince)")
        add_subdirectory(src/winmain)
    endif()
    add_subdirectory(src/xmlpatterns)
    if(ENABLE_TESTING)
        add_subdirectory(tests)
    endif()
endif()
add_subdirectory(src/tools)

# utilities needed for Katie maintainance
add_subdirectory(util/fixnonlatin1)
add_subdirectory(util/gencmap)
add_subdirectory(util/lexgen)
add_subdirectory(util/makeqpf)
add_subdirectory(util/normalize)
add_subdirectory(util/plugintest)
add_subdirectory(util/qlalr)
add_subdirectory(util/qurl-generateTLDs)
add_subdirectory(util/unicode)
add_subdirectory(util/xkbdatagen)
add_subdirectory(util/generaterun)

# obsolete but still widely required headers
katie_generate_obsolete(qtglobal.h QtCore qglobal.h)
katie_generate_obsolete(qmultihash.h QtCore qhash.h)
katie_generate_obsolete(qscopedarraypointer.h QtCore qscopedpointer.h)
katie_generate_obsolete(qdate.h QtCore qdatetime.h)
katie_generate_obsolete(qwizardpage.h QtGui qwizard.h)
katie_generate_obsolete(qtimeedit.h QtGui qdatetimeedit.h)
katie_generate_obsolete(qdateedit.h QtGui qdatetimeedit.h)
katie_generate_obsolete(qhboxlayout.h QtGui qboxlayout.h)
katie_generate_obsolete(qvboxlayout.h QtGui qboxlayout.h)
katie_generate_obsolete(qspaceritem.h QtGui qlayoutitem.h)
katie_generate_obsolete(qdoublespinbox.h QtGui qspinbox.h)
katie_generate_obsolete(global.h Phonon ../phonon/phononnamespace.h)
katie_generate_obsolete(qxmlstream.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamwriter.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamreader.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qnetworkconfigurationmanager.h QtNetwork qnetworkconfigmanager.h)
katie_generate_obsolete(qnetworkcachemetadata.h QtNetwork qabstractnetworkcache.h)
# those are aliases
if(UNIX AND WITH_X11 AND X11_FOUND)
    katie_generate_obsolete(qx11info.h QtGui qx11info_x11.h)
    katie_generate_obsolete(qx11embedcontainer QtGui qx11embed_x11.h)
    katie_generate_obsolete(qx11embedwidget.h QtGui qx11embed_x11.h)
endif()
# those have shuffled, e.g. from QtDesginer to QtUiTools
katie_generate_obsolete(customwidget.h QtDesginer ../QtUiTools/customwidget.h)
# TODO: deal with those and many more
# QMacStyle
# QAbstractFontEngine

# translations
file(GLOB TRANSLATIONS "translations/*.ts")
katie_translations(${TRANSLATIONS})

katie_fixup_string("${KATIE_DEFINITIONS}" KATIE_DEFINITIONS)

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

install(
    DIRECTORY mkspecs
    DESTINATION ${QT_DATA_PATH_INST}/cmake/Katie
    COMPONENT Devel
)

install(
    FILES
    ${CMAKE_BINARY_DIR}/KatieConfig.cmake
    ${CMAKE_BINARY_DIR}/KatieConfigVersion.cmake
    ${CMAKE_SOURCE_DIR}/cmake/modules/KatieMacros.cmake
    DESTINATION
    ${QT_DATA_PATH_INST}/cmake/Katie
    COMPONENT Devel
)

install(
    EXPORT KatieLibraryTargets
    NAMESPACE Katie::
    DESTINATION ${QT_DATA_PATH_INST}/cmake/Katie
    FILE KatieLibraryTargets.cmake
)

install(
    EXPORT KatieBinaryTargets
    NAMESPACE Katie::
    DESTINATION ${QT_DATA_PATH_INST}/cmake/Katie
    FILE KatieBinaryTargets.cmake
)

if(NOT KATIE_PLATFORM MATCHES "(win32|wince|mac)")
    if(CMAKE_SKIP_INSTALL_RPATH OR NOT CMAKE_INSTALL_RPATH)
        configure_file(
            ${CMAKE_SOURCE_DIR}/cmake/ld.so.conf.cmake
            ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.conf
            @ONLY
        )
        install(
            FILES ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.conf
            DESTINATION ${LDCONF_INSTALL_DIR}
        )
    endif()

    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/profile.sh.cmake
        ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.sh
        @ONLY
    )
    install(
        FILES ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.sh
        DESTINATION ${PROFILE_INSTALL_DIR}
    )
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES) 
