# Editors notes:
#
# * To find all FIXME/TODO related to the CMake build system (and those
#   inherited from the autotools build system) execute the following in the
#   top-level directory of the source tree:
#
#     find -name '*.cmake' -exec grep -E 'TODO|FIXME' {} +
#     find -name CMakeLists.txt -exec grep -E 'TODO|FIXME' {} +

cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(Katie C CXX ASM)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_AUTOMOC FALSE)
set(CMAKE_AUTOUIC FALSE)
set(CMAKE_AUTORCC FALSE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/staticlib")

include(CheckIncludeFile)
include(CheckTypeSize)
include(FeatureSummary)
include(KatieMacros)
include(KatieBuildMacros)

# components and tools that will be build, changed depending on the requirements availability
set(KATIE_COMPONENTS "Core Gui Declarative Designer DesignerComponents Help Multimedia Network Sql Svg Xml XmlPatterns Script ScriptTools Test UiTools")
set(KATIE_TOOLS "moc uic rcc qhelpgenerator qcollectiongenerator lupdate lrelease lconvert designer qscript qtconfig")

# optional package
option(WITH_THREADS "Build threads support" ON)
add_feature_info(threads WITH_THREADS "an open source something")

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
add_feature_info(mng WITH_MNG "an open source something")

option(WITH_NAS "Build nas support" ON)
add_feature_info(nas WITH_NAS "an open source something")

option(WITH_ICONV "Build iconv support" ON)
add_feature_info(iconv WITH_ICONV "an open source something")

option(WITH_GLIB2 "Build glib2 support" OFF)
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
add_feature_info(odbc WITH_ODBC "an open source something")

option(WITH_PSQL "Build PostgreSQL database plugin" ON)
add_feature_info(postgresql WITH_PSQL "the world's most advanced open source database")

option(WITH_MYSQL "Build MySQL database plugin" ON)
add_feature_info(mysql WITH_MYSQL "the world's most popular open source database")

option(WITH_SQLITE "Build SQLite database plugin" ON)
add_feature_info(sqlite WITH_SQLITE "the world's most popular open source database")

option(WITH_X11 "Build X11/X.Org support" ON)
add_feature_info(x11 WITH_X11 "an open source something")

# optional features
option(KATIE_BOOTSTRAP "Build only core tools" OFF)
add_feature_info(bootstrap KATIE_BOOTSTRAP "internal build option")

option(KATIE_ALLINONE "Build targets from single source file" OFF)
add_feature_info(allinone KATIE_ALLINONE "experimental build option")

option(WITH_OPENGLES1 "Build opengles1 support" OFF)
add_feature_info(opengles1 WITH_OPENGLES1 "an open source something")

option(WITH_OPENGLES2 "Build opengles2 support" OFF)
add_feature_info(opengles2 WITH_OPENGLES2 "an open source something")

option(WITH_ACCESSIBILITY "Build accessibility support" ON)
add_feature_info(accessibility WITH_ACCESSIBILITY "an open source something")

# windows
option(WITH_ICD "Build icd support (Windows)" ON)
add_feature_info(icd WITH_ICD "an closed source something")

option(WITH_DIRECTWRITE "Build DirectWrite support (Windows)" OFF)
add_feature_info(directwrite WITH_DIRECTWRITE "an closed source something")

# mac
option(WITH_COREWLAN "Build corewlan support (Mac)" ON)
add_feature_info(corewlan WITH_COREWLAN "an closed source something")

include(mkspecs/mkspecs.cmake)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ toolkit derived from the Qt 4.8 framework")
set(CPACK_PACKAGE_VENDOR "Katie")
set(CPACK_PACKAGE_CONTACT "${KATIE_BUGREPORT}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${KATIE_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${KATIE_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${KATIE_MICRO})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Katie")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/mingw/;/.git;${CPACK_SOURCE_IGNORE_FILES}")
set(CPACK_STRIP_FILES TRUE)
if(WIN32 AND CMAKE_CROSSCOMPILING)
    # Windows build on non-Windows host, NSIS is not manditory on such
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_SOURCE_GENERATOR "ZIP")
elseif(WIN32)
    # native Windows build, let's make a NSIS package
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_SOURCE_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TXZ")
    set(CPACK_SOURCE_GENERATOR "TXZ")
endif()
include(CPack)

# 9 minutes timeout for the tests, Travis timeouts on 10min and I do not want
# some tests to be skipped because of one test. Like the qpainter test - it
# does a lot of tests and can (and sometimes it does) take more then 10min.
set(CTEST_TEST_TIMEOUT 540)

if(NOT CMAKE_VERSION VERSION_LESS "3.3.0")
    cmake_policy(SET CMP0063 NEW)
endif()
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
add_definitions(
    -DKATIE_DATE="${KATIE_DATE}"
    -DQT_USE_QSTRINGBUILDER
    -DQT_ASCII_CAST_WARNINGS
    # maybe one day, use with caution!
    # -DQT_NO_CAST_FROM_ASCII
    # -DQT_NO_CAST_TO_ASCII
    # -DQT_STRICT_ITERATORS
)

# this is how Qt4 does it
if(NOT KATIE_PLATFORM STREQUAL "integrity" AND NOT MINGW)
    set(QT_LARGEFILE_SUPPORT 64)
    add_definitions(
        -D_LARGEFILE64_SOURCE
        -D_LARGEFILE_SOURCE
    )
endif()

# some things just cannot be done right
if(KATIE_ALLINONE)
    add_definitions(-DQT_ALLINONE)
endif()

# for 3rd party source used in help and test components
check_include_file(sys/time.h HAVE_SYS_TIME_H)
if(HAVE_SYS_TIME_H)
    add_definitions(-DTIME_WITH_SYS_TIME=1 -DHAVE_SYS_TIME_H)
else()
    add_definitions(-DTIME_WITH_SYS_TIME=0)
endif()

# used in qconfig.h
check_type_size(size_t QT_POINTER_SIZE)

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
    TYPE RECOMMENDED
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
    DESCRIPTION "Platform dependent EGL implementation"
    URL ""
    TYPE RECOMMENDED
)

find_package(OpenGL)
set_package_properties(OpenGL PROPERTIES
    PURPOSE "Required for GL/EGL support"
    DESCRIPTION "Platform dependent OpenGL implementation"
    URL ""
    TYPE RECOMMENDED
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

# TODO: make it optional, Qt4 was prepping for it I believe (QT_NO_THREAD)
find_package(Threads)
set_package_properties(Threads PROPERTIES
    PURPOSE "Required for threads support"
    DESCRIPTION "Platform dependent threads implementation"
    URL ""
    TYPE REQUIRED
)

if(NOT KATIE_PLATFORM MATCHES "(win32|wince|mac)" AND NOT KATIE_BOOTSTRAP)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

    find_package(X11 COMPONENTS ICE SM Xcursor Xext Xfixes XSync XShm XShape Xi Xkb Xinerama Xrandr Xrender X11 Xtst Xt)
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
endif()

if(PYTHONINTERP_FOUND)
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/genmap.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE genmap_result
        ERROR_VARIABLE genmap_error
    )
    if(NOT genmap_result EQUAL 0)
        message(SEND_ERROR "${genmap_error}")
    endif()
else()
    message(WARNING
        "\nUsing pre-generated classes map"
    )
endif()

# various sources use #include <shared/blah.h>
include_directories(${CMAKE_SOURCE_DIR}/src)

if(ENABLE_TESTING)
    message(WARNING
        "\nUnless it is intentionall you should not enable testing. You will have to unset "
        "QT_PLUGIN_PATH if you have Qt4 installed on the system prior to running the tests."
    )
    enable_testing()
    add_definitions(-DQT_BUILD_INTERNAL)
endif()

# optional packages
if(NOT WITH_THREADS OR NOT THREADS_FOUND)
    katie_definition(-DQT_NO_THREAD)
endif()

if(NOT WITH_EGL OR NOT EGL_FOUND)
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
else()
    set(KATIE_COMPONENTS "${KATIE_COMPONENTS} DBus")
    set(KATIE_TOOLS "${KATIE_TOOLS} qdbusxml2cpp qdbuscpp2xml qdbusviewer")
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

if(NOT WITH_OPENGL OR NOT OPENGL_FOUND)
    katie_definition(-DQT_NO_OPENGL)
else()
    set(KATIE_COMPONENTS "${KATIE_COMPONENTS} OpenGL")
endif()

if(NOT WITH_NAS OR NOT NAS_FOUND)
    katie_definition(-DQT_NO_NAS)
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


# conditional features
if(NOT WITH_ACCESSIBILITY)
    katie_definition(-DQT_NO_ACCESSIBILITY)
endif()

if(NOT WITH_DIRECTWRITE)
    katie_definition(-DQT_NO_DIRECTWRITE)
endif()

if(WITH_OPENGLES1)
    katie_definition(-DQT_OPENGL_ES_1)
endif()

if(WITH_OPENGLES2)
    katie_definition(-DQT_OPENGL_ES_2)
endif()

# keep in sucn with multimedia audio component CMake file, only definitions setup is needed here
if(KATIE_PLATFORM MATCHES "(mac|win32|wince)" OR (UNIX AND WITH_ALSA AND ALSA_FOUND))
    # NOTE: CMake's logical OR does not stop at the first met condition and
    # I'd rather not have a AND condition here with negatives
else()
    katie_definition(-DQT_NO_AUDIO_BACKEND)
endif()

# keep in sync with the gui component CMake file, only definitions setup is needed here
if(WITH_X11 AND X11_FOUND)
    # TODO: xim
    foreach(x11ext Xinerama Xrandr Xrender Xfixes Xcursor Xinput)
        # X11_${x11ext}_FOUND is set depending on the headers only
        if(NOT X11_${x11ext}_FOUND OR NOT X11_${x11ext}_LIB)
            message(WARNING "The X11 ${x11ext} extension was not found")
            string(TOUPPER "${x11ext}" upperext)
            katie_definition(-DQT_NO_${upperext})
        endif()
    endforeach()
    if(NOT X11_Xkbfile_FOUND)
        message(WARNING "The X11 Xkbfile extension was not found")
        katie_definition(-DQT_NO_XKB)
    endif()
    if(NOT X11_Xext_FOUND)
        message(WARNING "The X11 Xext extension was not found")
        katie_definition(-DQT_NO_XSYNC -DQT_NO_XSHAPE -DQTT_NO_XSHM)
    endif()
    if(NOT X11_Xinput_FOUND)
        message(WARNING "X11 Xinput extension was not found thus disabling tabled support too")
        katie_definition(-DQT_NO_TABLET)
    endif()
    if(NOT X11_SM_FOUND)
        message(WARNING "X11 SM was not found thus disabling session manager support")
        katie_definition(-DQT_NO_SESSIONMANAGER)
    endif()
endif()

# kinda optional package
if(NOT WITH_PNG)
    katie_definition(-DQT_NO_IMAGEFORMAT_PNG)
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
add_subdirectory(util/normalize)
add_subdirectory(util/plugintest)
add_subdirectory(util/qlalr)
add_subdirectory(util/qurl-generateTLDs)
add_subdirectory(util/unicode)
add_subdirectory(util/xkbdatagen)
add_subdirectory(util/generaterun)

# the macro calls bellow are generted via the incfsck script
katie_generate_obsolete(qxmlstreamnotationdeclaration.h QtCore qxmlstream.h)
katie_generate_obsolete(qxmlstreamnamespacedeclaration.h QtCore qxmlstream.h)
katie_generate_obsolete(qmutablestringlistiterator.h QtCore qstringlist.h)
katie_generate_obsolete(qvariantlist.h QtCore qvariant.h)
katie_generate_obsolete(qbyteref.h QtCore qbytearray.h)
katie_generate_obsolete(qtime.h QtCore qdatetime.h)
katie_generate_obsolete(qglobalstatic.h QtCore qglobal.h)
katie_generate_obsolete(qmutablelinkedlistiterator.h QtCore qlist.h)
katie_generate_obsolete(qprocessenvironment.h QtCore qprocess.h)
katie_generate_obsolete(qlatin1char.h QtCore qchar.h)
katie_generate_obsolete(qbasicatomicint.h QtCore qbasicatomic.h)
katie_generate_obsolete(qmodelindex.h QtCore qabstractitemmodel.h)
katie_generate_obsolete(qmodelindexlist.h QtCore qabstractitemmodel.h)
katie_generate_obsolete(qmetaproperty.h QtCore qmetaobject.h)
katie_generate_obsolete(qflags.h QtCore qglobal.h)
katie_generate_obsolete(qmutablehashiterator.h QtCore qhash.h)
katie_generate_obsolete(qmutablesetiterator.h QtCore qset.h)
katie_generate_obsolete(qmapiterator.h QtCore qmap.h)
katie_generate_obsolete(qxmlstreamwriter.h QtCore qxmlstream.h)
katie_generate_obsolete(qwritelocker.h QtCore qreadwritelock.h)
katie_generate_obsolete(qmutablelistiterator.h QtCore qlist.h)
katie_generate_obsolete(qsysinfo.h QtCore qglobal.h)
katie_generate_obsolete(qmetaclassinfo.h QtCore qmetaobject.h)
katie_generate_obsolete(qxmlstreamentityresolver.h QtCore qxmlstream.h)
katie_generate_obsolete(qgenericargument.h QtCore qobjectdefs.h)
katie_generate_obsolete(qmetaenum.h QtCore qmetaobject.h)
katie_generate_obsolete(qxmlstreamnotationdeclarations.h QtCore qxmlstream.h)
katie_generate_obsolete(qvarianthash.h QtCore qvariant.h)
katie_generate_obsolete(qbitref.h QtCore qbitarray.h)
katie_generate_obsolete(qrectf.h QtCore qrect.h)
katie_generate_obsolete(qmultimap.h QtCore qmap.h)
katie_generate_obsolete(qchildevent.h QtCore qcoreevent.h)
katie_generate_obsolete(qargument.h QtCore qobjectdefs.h)
katie_generate_obsolete(qt.h QtCore qnamespace.h)
katie_generate_obsolete(qobjectlist.h QtCore qobject.h)
katie_generate_obsolete(qmutablevectoriterator.h QtCore qvector.h)
katie_generate_obsolete(qmultihash.h QtCore qhash.h)
katie_generate_obsolete(qfileinfolist.h QtCore qfileinfo.h)
katie_generate_obsolete(qhashiterator.h QtCore qhash.h)
katie_generate_obsolete(qtglobal.h QtCore qglobal.h)
katie_generate_obsolete(qscopedarraypointer.h QtCore qscopedpointer.h)
katie_generate_obsolete(qxmlstreamentitydeclaration.h QtCore qxmlstream.h)
katie_generate_obsolete(qtplugin.h QtCore qplugin.h)
katie_generate_obsolete(qdynamicpropertychangeevent.h QtCore qcoreevent.h)
katie_generate_obsolete(qdate.h QtCore qdatetime.h)
katie_generate_obsolete(qabstractlistmodel.h QtCore qabstractitemmodel.h)
katie_generate_obsolete(qxmlstreamattribute.h QtCore qxmlstream.h)
katie_generate_obsolete(qatomicint.h QtCore qatomic.h)
katie_generate_obsolete(qlatin1string.h QtCore qstring.h)
katie_generate_obsolete(qpersistentmodelindex.h QtCore qabstractitemmodel.h)
katie_generate_obsolete(qbool.h QtCore qglobal.h)
katie_generate_obsolete(qmutexlocker.h QtCore qmutex.h)
katie_generate_obsolete(qlistiterator.h QtCore qlist.h)
katie_generate_obsolete(qreadlocker.h QtCore qreadwritelock.h)
katie_generate_obsolete(qxmlstreamreader.h QtCore qxmlstream.h)
katie_generate_obsolete(qxmlstreamentitydeclarations.h QtCore qxmlstream.h)
katie_generate_obsolete(qmetamethod.h QtCore qmetaobject.h)
katie_generate_obsolete(q_pid.h QtCore qprocess.h)
katie_generate_obsolete(qtdebug.h QtCore qdebug.h)
katie_generate_obsolete(qtimerevent.h QtCore qcoreevent.h)
katie_generate_obsolete(qtalgorithms.h QtCore qalgorithms.h)
katie_generate_obsolete(qxmlstreamstringref.h QtCore qxmlstream.h)
katie_generate_obsolete(qmutablemapiterator.h QtCore qmap.h)
katie_generate_obsolete(qlatin1literal.h QtCore qstringbuilder.h)
katie_generate_obsolete(qtendian.h QtCore qendian.h)
katie_generate_obsolete(qxmlstreamattributes.h QtCore qxmlstream.h)
katie_generate_obsolete(qabstracttablemodel.h QtCore qabstractitemmodel.h)
katie_generate_obsolete(qsizef.h QtCore qsize.h)
katie_generate_obsolete(qvariantmap.h QtCore qvariant.h)
katie_generate_obsolete(qexplicitlyshareddatapointer.h QtCore qsharedpointer.h)
katie_generate_obsolete(qweakpointer.h QtCore qsharedpointer.h)
katie_generate_obsolete(qcharref.h QtCore qstring.h)
katie_generate_obsolete(qpointf.h QtCore qpoint.h)
katie_generate_obsolete(qatomicpointer.h QtCore qatomic.h)
katie_generate_obsolete(qshareddatapointer.h QtCore qshareddata.h)
katie_generate_obsolete(qxmlstreamnamespacedeclarations.h QtCore qxmlstream.h)
katie_generate_obsolete(qdbuspendingreplydata.h QtDBus qdbuspendingreply.h)
katie_generate_obsolete(qdbusabstractinterfacebase.h QtDBus qdbusabstractinterface.h)
katie_generate_obsolete(qdbuspendingcallwatcher.h QtDBus qdbuspendingcall.h)
katie_generate_obsolete(qdbusobjectpath.h QtDBus qdbusextratypes.h)
katie_generate_obsolete(qdbusvariant.h QtDBus qdbusextratypes.h)
katie_generate_obsolete(qdbussignature.h QtDBus qdbusextratypes.h)
katie_generate_obsolete(qdeclarativelistproperty.h QtDeclarative qdeclarativelist.h)
katie_generate_obsolete(qdesignerextrainfoextension.h QtDesigner extrainfo.h)
katie_generate_obsolete(qdesignerlayoutdecorationextension.h QtDesigner layoutdecoration.h)
katie_generate_obsolete(qdesignerpromotioninterface.h QtDesigner abstractpromotioninterface.h)
katie_generate_obsolete(qdesignerbrushmanagerinterface.h QtDesigner abstractbrushmanager.h)
katie_generate_obsolete(qdesignerdnditeminterface.h QtDesigner abstractdnditem.h)
katie_generate_obsolete(qdesignermembersheetextension.h QtDesigner membersheet.h)
katie_generate_obsolete(qdesignerwidgetdatabaseiteminterface.h QtDesigner abstractwidgetdatabase.h)
katie_generate_obsolete(qdesignercomponents.h QtDesigner qdesigner_components.h)
katie_generate_obsolete(qdesignerformwindowmanagerinterface.h QtDesigner abstractformwindowmanager.h)
katie_generate_obsolete(qdesignerformwindowtoolinterface.h QtDesigner abstractformwindowtool.h)
katie_generate_obsolete(qdesignerdynamicpropertysheetextension.h QtDesigner dynamicpropertysheet.h)
katie_generate_obsolete(qdesignerwidgetboxinterface.h QtDesigner abstractwidgetbox.h)
katie_generate_obsolete(qdesignerpropertysheetextension.h QtDesigner propertysheet.h)
katie_generate_obsolete(qdesignericoncacheinterface.h QtDesigner abstracticoncache.h)
katie_generate_obsolete(qdesignerobjectinspectorinterface.h QtDesigner abstractobjectinspector.h)
katie_generate_obsolete(qdesignerintegrationinterface.h QtDesigner abstractintegration.h)
katie_generate_obsolete(qdesignerformeditorplugininterface.h QtDesigner abstractformeditorplugin.h)
katie_generate_obsolete(qdesigneractioneditorinterface.h QtDesigner abstractactioneditor.h)
katie_generate_obsolete(qdesignertaskmenuextension.h QtDesigner taskmenu.h)
katie_generate_obsolete(qdesignerformeditorinterface.h QtDesigner abstractformeditor.h)
katie_generate_obsolete(qdesignercustomwidgetinterface.h QtDesigner customwidget.h)
katie_generate_obsolete(qdesignermetadatabaseiteminterface.h QtDesigner abstractmetadatabase.h)
katie_generate_obsolete(qdesignerformwindowinterface.h QtDesigner abstractformwindow.h)
katie_generate_obsolete(qdesignercustomwidgetcollectioninterface.h QtDesigner customwidget.h)
katie_generate_obsolete(qdesignerformwindowcursorinterface.h QtDesigner abstractformwindowcursor.h)
katie_generate_obsolete(qabstractformbuilder.h QtDesigner abstractformbuilder.h)
katie_generate_obsolete(qdesignerpropertyeditorinterface.h QtDesigner abstractpropertyeditor.h)
katie_generate_obsolete(qdesignerresourcebrowserinterface.h QtDesigner abstractresourcebrowser.h)
katie_generate_obsolete(qdesignerwidgetfactoryinterface.h QtDesigner abstractwidgetfactory.h)
katie_generate_obsolete(qextensionfactory.h QtDesigner default_extensionfactory.h)
katie_generate_obsolete(qdesignercontainerextension.h QtDesigner container.h)
katie_generate_obsolete(qdesignerlanguageextension.h QtDesigner abstractlanguage.h)
katie_generate_obsolete(qdesignermetadatabaseinterface.h QtDesigner abstractmetadatabase.h)
katie_generate_obsolete(qdesignerwidgetdatabaseinterface.h QtDesigner abstractwidgetdatabase.h)
katie_generate_obsolete(qstyleoptionviewitem.h QtGui qstyleoption.h)
katie_generate_obsolete(qintvalidator.h QtGui qvalidator.h)
katie_generate_obsolete(qtexttableformat.h QtGui qtextformat.h)
katie_generate_obsolete(qmimesource.h QtGui qmime.h)
katie_generate_obsolete(qstyleoptiongraphicsitem.h QtGui qstyleoption.h)
katie_generate_obsolete(qaccessiblebridgefactoryinterface.h QtGui qaccessiblebridge.h)
katie_generate_obsolete(qiconenginev2.h QtGui qiconengine.h)
katie_generate_obsolete(qdragenterevent.h QtGui qevent.h)
katie_generate_obsolete(qradialgradient.h QtGui qbrush.h)
katie_generate_obsolete(qdateedit.h QtGui qdatetimeedit.h)
katie_generate_obsolete(qgraphicssceneresizeevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qaccessibleapplication.h QtGui qaccessibleobject.h)
katie_generate_obsolete(qaccessibletableinterface.h QtGui qaccessible2.h)
katie_generate_obsolete(qgraphicsscenehoverevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qstyleoptionfocusrect.h QtGui qstyleoption.h)
katie_generate_obsolete(qstyleoptiongroupbox.h QtGui qstyleoption.h)
katie_generate_obsolete(qaccessibleactioninterface.h QtGui qaccessible2.h)
katie_generate_obsolete(qwidgetlist.h QtGui qwindowdefs.h)
katie_generate_obsolete(qiconenginefactoryinterfacev2.h QtGui qiconengineplugin.h)
katie_generate_obsolete(qtreewidgetitem.h QtGui qtreewidget.h)
katie_generate_obsolete(qgraphicslineitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qstyleoptiontabbarbase.h QtGui qstyleoption.h)
katie_generate_obsolete(qgraphicstextitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qstyleoptiontabv3.h QtGui qstyleoption.h)
katie_generate_obsolete(qiconenginepluginv2.h QtGui qiconengineplugin.h)
katie_generate_obsolete(qstyleoptionframev2.h QtGui qstyleoption.h)
katie_generate_obsolete(qgraphicsrectitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qspaceritem.h QtGui qlayoutitem.h)
katie_generate_obsolete(qabstractundoitem.h QtGui qtextdocument.h)
katie_generate_obsolete(qimagetextkeylang.h QtGui qimage.h)
katie_generate_obsolete(qitemeditorcreatorbase.h QtGui qitemeditorfactory.h)
katie_generate_obsolete(qstyleoptiontoolbutton.h QtGui qstyleoption.h)
katie_generate_obsolete(qaccessibletable2interface.h QtGui qaccessible2.h)
katie_generate_obsolete(qgraphicsscenemouseevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qgraphicspixmapitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qdropevent.h QtGui qevent.h)
katie_generate_obsolete(qcontextmenuevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptionrubberband.h QtGui qstyleoption.h)
katie_generate_obsolete(qaccessibletable2cellinterface.h QtGui qaccessible2.h)
katie_generate_obsolete(qgraphicsscenewheelevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qstyleoptionviewitemv3.h QtGui qstyleoption.h)
katie_generate_obsolete(qitemselection.h QtGui qitemselectionmodel.h)
katie_generate_obsolete(qgraphicsscenecontextmenuevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qundocommand.h QtGui qundostack.h)
katie_generate_obsolete(qgraphicsdropshadoweffect.h QtGui qgraphicseffect.h)
katie_generate_obsolete(qactionevent.h QtGui qevent.h)
katie_generate_obsolete(qgraphicsscenehelpevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qgraphicsobject.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qgraphicspolygonitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qtextcharformat.h QtGui qtextformat.h)
katie_generate_obsolete(qinputevent.h QtGui qevent.h)
katie_generate_obsolete(qkeyevent.h QtGui qevent.h)
katie_generate_obsolete(qiconenginefactoryinterface.h QtGui qiconengineplugin.h)
katie_generate_obsolete(qstyleoptionmenuitem.h QtGui qstyleoption.h)
katie_generate_obsolete(qstyleoptionspinbox.h QtGui qstyleoption.h)
katie_generate_obsolete(qshortcutevent.h QtGui qevent.h)
katie_generate_obsolete(qgraphicsscenemoveevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qaccessibleinterface.h QtGui qaccessible.h)
katie_generate_obsolete(qinputcontextfactoryinterface.h QtGui qinputcontextplugin.h)
katie_generate_obsolete(qtextlength.h QtGui qtextformat.h)
katie_generate_obsolete(qtextblock.h QtGui qtextobject.h)
katie_generate_obsolete(qdragmoveevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptionprogressbarv2.h QtGui qstyleoption.h)
katie_generate_obsolete(qcloseevent.h QtGui qevent.h)
katie_generate_obsolete(qwindowstatechangeevent.h QtGui qevent.h)
katie_generate_obsolete(qshowevent.h QtGui qevent.h)
katie_generate_obsolete(qaccessibleinterfaceex.h QtGui qaccessible.h)
katie_generate_obsolete(qwheelevent.h QtGui qevent.h)
katie_generate_obsolete(qhboxlayout.h QtGui qboxlayout.h)
katie_generate_obsolete(qstyleoptioncombobox.h QtGui qstyleoption.h)
katie_generate_obsolete(qgraphicsitemgroup.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qgestureevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptionframev3.h QtGui qstyleoption.h)
katie_generate_obsolete(qvboxlayout.h QtGui qboxlayout.h)
katie_generate_obsolete(qaccessiblebridgeplugin.h QtGui qaccessiblebridge.h)
katie_generate_obsolete(qlistwidgetitem.h QtGui qlistwidget.h)
katie_generate_obsolete(qpolygonf.h QtGui qpolygon.h)
katie_generate_obsolete(qitemselectionrange.h QtGui qitemselectionmodel.h)
katie_generate_obsolete(qtablewidgetitem.h QtGui qtablewidget.h)
katie_generate_obsolete(qimageioplugin.h QtGui qimageiohandler.h)
katie_generate_obsolete(qfocusevent.h QtGui qevent.h)
katie_generate_obsolete(qaccessibleimageinterface.h QtGui qaccessible2.h)
katie_generate_obsolete(qmoveevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptionviewitemv4.h QtGui qstyleoption.h)
katie_generate_obsolete(qtextline.h QtGui qtextlayout.h)
katie_generate_obsolete(qconicalgradient.h QtGui qbrush.h)
katie_generate_obsolete(qstandarditem.h QtGui qstandarditemmodel.h)
katie_generate_obsolete(qstyleoptionviewitemv2.h QtGui qstyleoption.h)
katie_generate_obsolete(qstyleoptionbutton.h QtGui qstyleoption.h)
katie_generate_obsolete(qgraphicsscale.h QtGui qgraphicstransform.h)
katie_generate_obsolete(qaccessiblesimpleeditabletextinterface.h QtGui qaccessible2.h)
katie_generate_obsolete(qfileopenevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptionframe.h QtGui qstyleoption.h)
katie_generate_obsolete(qabstractgraphicsshapeitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qtextlistformat.h QtGui qtextformat.h)
katie_generate_obsolete(qwidgetitem.h QtGui qlayoutitem.h)
katie_generate_obsolete(qstyleoptionslider.h QtGui qstyleoption.h)
katie_generate_obsolete(qregexpvalidator.h QtGui qvalidator.h)
katie_generate_obsolete(qpinchgesture.h QtGui qgesture.h)
katie_generate_obsolete(qgraphicsscenedragdropevent.h QtGui qgraphicssceneevent.h)
katie_generate_obsolete(qgraphicsopacityeffect.h QtGui qgraphicseffect.h)
katie_generate_obsolete(qpaintevent.h QtGui qevent.h)
katie_generate_obsolete(qfontmetricsf.h QtGui qfontmetrics.h)
katie_generate_obsolete(qhideevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptiontoolbar.h QtGui qstyleoption.h)
katie_generate_obsolete(qtevents.h QtGui qevent.h)
katie_generate_obsolete(qinputmethodevent.h QtGui qevent.h)
katie_generate_obsolete(qdoublespinbox.h QtGui qspinbox.h)
katie_generate_obsolete(qaccessible2interface.h QtGui qaccessible2.h)
katie_generate_obsolete(qtimeedit.h QtGui qdatetimeedit.h)
katie_generate_obsolete(qicondragevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptiontitlebar.h QtGui qstyleoption.h)
katie_generate_obsolete(qhoverevent.h QtGui qevent.h)
katie_generate_obsolete(qwizardpage.h QtGui qwizard.h)
katie_generate_obsolete(qdragleaveevent.h QtGui qevent.h)
katie_generate_obsolete(qstyleoptioncomplex.h QtGui qstyleoption.h)
katie_generate_obsolete(qtouchevent.h QtGui qevent.h)
katie_generate_obsolete(qgraphicspathitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qgradient.h QtGui qbrush.h)
katie_generate_obsolete(qstyleoptionheader.h QtGui qstyleoption.h)
katie_generate_obsolete(qaccessiblefactoryinterface.h QtGui qaccessibleplugin.h)
katie_generate_obsolete(qresizeevent.h QtGui qevent.h)
katie_generate_obsolete(qtextframe.h QtGui qtextobject.h)
katie_generate_obsolete(qtexttablecell.h QtGui qtexttable.h)
katie_generate_obsolete(qitemeditorcreator.h QtGui qitemeditorfactory.h)
katie_generate_obsolete(qpainterpathstroker.h QtGui qpainterpath.h)
katie_generate_obsolete(qaccessibleevent.h QtGui qaccessible.h)
katie_generate_obsolete(qplaintextdocumentlayout.h QtGui qplaintextedit.h)
katie_generate_obsolete(qhelpevent.h QtGui qevent.h)
katie_generate_obsolete(qmouseevent.h QtGui qevent.h)
katie_generate_obsolete(qdoublevalidator.h QtGui qvalidator.h)
katie_generate_obsolete(qimageiohandlerfactoryinterface.h QtGui qimageiohandler.h)
katie_generate_obsolete(qlineargradient.h QtGui qbrush.h)
katie_generate_obsolete(qgraphicssimpletextitem.h QtGui qgraphicsitem.h)
katie_generate_obsolete(qgraphicsrotation.h QtGui qgraphicstransform.h)
katie_generate_obsolete(qaccessibleeditabletextinterface.h QtGui qaccessible2.h)
katie_generate_obsolete(qsplitterhandle.h QtGui qsplitter.h)
katie_generate_obsolete(qaccessibleobjectex.h QtGui qaccessibleobject.h)
katie_generate_obsolete(qhelpindexmodel.h QtHelp qhelpindexwidget.h)
katie_generate_obsolete(qhelpcontentmodel.h QtHelp qhelpcontentwidget.h)
katie_generate_obsolete(qhttprequestheader.h QtNetwork qhttp.h)
katie_generate_obsolete(qnetworkproxyquery.h QtNetwork qnetworkproxy.h)
katie_generate_obsolete(qhttpresponseheader.h QtNetwork qhttp.h)
katie_generate_obsolete(qnetworkcachemetadata.h QtNetwork qabstractnetworkcache.h)
katie_generate_obsolete(qnetworkaddressentry.h QtNetwork qnetworkinterface.h)
katie_generate_obsolete(qnetworkconfigurationmanager.h QtNetwork qnetworkconfigmanager.h)
katie_generate_obsolete(qnetworkproxyfactory.h QtNetwork qnetworkproxy.h)
katie_generate_obsolete(qhttppart.h QtNetwork qhttpmultipart.h)
katie_generate_obsolete(qhttpheader.h QtNetwork qhttp.h)
katie_generate_obsolete(qglshader.h QtOpenGL qglshaderprogram.h)
katie_generate_obsolete(qglcontext.h QtOpenGL qgl.h)
katie_generate_obsolete(qglwidget.h QtOpenGL qgl.h)
katie_generate_obsolete(qtestaccessibility.h QtTest qtestaccessible.h)
katie_generate_obsolete(qeventsizeofchecker.h QtTest qtestspontaneevent.h)
katie_generate_obsolete(qtestkeyclicksevent.h QtTest qtestevent.h)
katie_generate_obsolete(qtesteventlist.h QtTest qtestevent.h)
katie_generate_obsolete(qtestaccessibilityevent.h QtTest qtestaccessible.h)
katie_generate_obsolete(qttestgui.h QtTest qtest_gui.h)
katie_generate_obsolete(qtestdelayevent.h QtTest qtestevent.h)
katie_generate_obsolete(qtestkeyevent.h QtTest qtestevent.h)
katie_generate_obsolete(qspontanekeyevent.h QtTest qtestspontaneevent.h)
katie_generate_obsolete(qtestmouseevent.h QtTest qtestevent.h)
katie_generate_obsolete(qxmlnamespacesupport.h QtXml qxml.h)
katie_generate_obsolete(qdomcomment.h QtXml qdom.h)
katie_generate_obsolete(qdomtext.h QtXml qdom.h)
katie_generate_obsolete(qxmlentityresolver.h QtXml qxml.h)
katie_generate_obsolete(qxmllexicalhandler.h QtXml qxml.h)
katie_generate_obsolete(qxmlcontenthandler.h QtXml qxml.h)
katie_generate_obsolete(qxmlattributes.h QtXml qxml.h)
katie_generate_obsolete(qxmlinputsource.h QtXml qxml.h)
katie_generate_obsolete(qxmlsimplereader.h QtXml qxml.h)
katie_generate_obsolete(qdomentity.h QtXml qdom.h)
katie_generate_obsolete(qxmldefaulthandler.h QtXml qxml.h)
katie_generate_obsolete(qdomdocumenttype.h QtXml qdom.h)
katie_generate_obsolete(qdomattr.h QtXml qdom.h)
katie_generate_obsolete(qdomdocument.h QtXml qdom.h)
katie_generate_obsolete(qdomdocumentfragment.h QtXml qdom.h)
katie_generate_obsolete(qxmldeclhandler.h QtXml qxml.h)
katie_generate_obsolete(qdomnamednodemap.h QtXml qdom.h)
katie_generate_obsolete(qdomnotation.h QtXml qdom.h)
katie_generate_obsolete(qdomprocessinginstruction.h QtXml qdom.h)
katie_generate_obsolete(qdomcharacterdata.h QtXml qdom.h)
katie_generate_obsolete(qxmlreader.h QtXml qxml.h)
katie_generate_obsolete(qxmllocator.h QtXml qxml.h)
katie_generate_obsolete(qdomimplementation.h QtXml qdom.h)
katie_generate_obsolete(qxmlerrorhandler.h QtXml qxml.h)
katie_generate_obsolete(qdomelement.h QtXml qdom.h)
katie_generate_obsolete(qdomnode.h QtXml qdom.h)
katie_generate_obsolete(qdomentityreference.h QtXml qdom.h)
katie_generate_obsolete(qxmlparseexception.h QtXml qxml.h)
katie_generate_obsolete(qdomnodelist.h QtXml qdom.h)
katie_generate_obsolete(qdomcdatasection.h QtXml qdom.h)
katie_generate_obsolete(qxmldtdhandler.h QtXml qxml.h)
katie_generate_obsolete(qxmlitem.h QtXmlPatterns qabstractxmlnodemodel.h)
katie_generate_obsolete(qxmlnodemodelindex.h QtXmlPatterns qabstractxmlnodemodel.h)
# aliases are not handled by the incfsck script on purpose
if(UNIX AND WITH_X11 AND X11_FOUND)
    katie_generate_obsolete(qx11info.h QtGui qx11info_x11.h)
    katie_generate_obsolete(qx11embedcontainer.h QtGui qx11embed_x11.h)
    katie_generate_obsolete(qx11embedwidget.h QtGui qx11embed_x11.h)
endif()
if(KATIE_PLATFORM STREQUAL "mac")
    katie_generate_obsolete(qmacstyle.h QtGui qmacstyle_mac.h)
    katie_generate_obsolete(qmaccocoaviewcontainer.h QtGui qmaccocoaviewcontainer_mac.h)
endif()
katie_generate_obsolete(qttestgui.h QtTest qtest_gui.h)
# those have shuffled from QtXml to QtCore
katie_generate_obsolete(qxmlstream.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamattribute.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamattributes.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamstringref.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamnotationdeclaration.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamnotationdeclarations.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamentitydeclaration.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamentitydeclarations.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamnamespacedeclaration.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamnamespacedeclarations.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamentityresolver.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamreader.h QtXml ../QtCore/qxmlstream.h)
katie_generate_obsolete(qxmlstreamwriter.h QtXml ../QtCore/qxmlstream.h)
# and those from QtDesigner to QtUiTools
katie_generate_obsolete(customwidget.h QtDesigner ../QtUiTools/customwidget.h)
katie_generate_obsolete(qdesignercustomwidgetinterface.h QtDesigner ../QtUiTools/customwidget.h)

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

# translations
katie_setup_paths()
file(GLOB TRANSLATIONS "translations/*.ts")
katie_translations(${TRANSLATIONS})

install(
    DIRECTORY mkspecs
    DESTINATION ${CMAKE_INSTALL_PATH}
    COMPONENT Devel
)

install(
    FILES
    ${CMAKE_BINARY_DIR}/KatieConfig.cmake
    ${CMAKE_BINARY_DIR}/KatieConfigVersion.cmake
    ${CMAKE_SOURCE_DIR}/cmake/modules/KatieMacros.cmake
    DESTINATION ${CMAKE_INSTALL_PATH}
    COMPONENT Devel
)

install(
    EXPORT KatieLibraryTargets
    NAMESPACE Katie::
    DESTINATION ${CMAKE_INSTALL_PATH}
    FILE KatieLibraryTargets.cmake
)

install(
    EXPORT KatieBinaryTargets
    NAMESPACE Katie::
    DESTINATION ${CMAKE_INSTALL_PATH}
    FILE KatieBinaryTargets.cmake
)

if(NOT KATIE_PLATFORM MATCHES "(win32|wince|mac)")
    install(
        FILES ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.conf
        DESTINATION ${LDCONF_INSTALL_PATH}
    )
    install(
        FILES ${CMAKE_BINARY_DIR}/katie-${KATIE_ARCHITECTURE}.sh
        DESTINATION ${PROFILE_INSTALL_PATH}
    )
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES) 
