string(TIMESTAMP KATIE_DATE "%Y-%m-%d")
set(KATIE_MAJOR "4")
set(KATIE_MINOR "9")
set(KATIE_MICRO "0")
set(KATIE_HEX "0x040900")
set(KATIE_NAME "Katie")
set(KATIE_VERSION "${KATIE_MAJOR}.${KATIE_MINOR}.${KATIE_MICRO}")
set(KATIE_STRING "katie ${KATIE_MAJOR}.${KATIE_MINOR}.${KATIE_MICRO}")
set(KATIE_BUGREPORT "xakepa10@gmail.com")
set(KATIE_URL "http://github.com/fluxer/katie")
set(KATIE_COMPONENTS "Core Gui DBus Declarative Designer DesignerComponents Help Multimedia Network OpenGL Sql Svg Xml XmlPatterns Script ScriptTools Test UiTools")
# TODO: make dbus tools optional
set(KATIE_TOOLS "moc uic rcc qdbusxml2cpp qdbuscpp2xml qhelpgenerator qcollectiongenerator lupdate lrelease lconvert designer")
set(QT_LICENSE "Open Source")
set(QT_PRODUCT "AwesomeSauce") # it's not a bug, it's a feature!

# KatieConfig overrides that
if(NOT KATIE_MKSPECS_DIR)
    set(KATIE_MKSPECS_DIR ${CMAKE_SOURCE_DIR}/mkspecs)
endif()

# TODO: more platforms/architectures support
include_directories(${KATIE_MKSPECS_DIR})
if(NOT KATIE_PLATFORM)
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        include_directories(${KATIE_MKSPECS_DIR}/linux)
        set(KATIE_PLATFORM "linux")
    elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
        include_directories(${KATIE_MKSPECS_DIR}/win32)
        set(KATIE_PLATFORM "win32")
    elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
        include_directories(${KATIE_MKSPECS_DIR}/mac)
        set(KATIE_PLATFORM "mac")
    elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
        include_directories(${KATIE_MKSPECS_DIR}/freebsd)
        set(KATIE_PLATFORM "freebsd")
    elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
        include_directories(${KATIE_MKSPECS_DIR}/openbsd)
        set(KATIE_PLATFORM "openbsd")
    else()
        message(FATAL_ERROR "Unknown platform '${CMAKE_SYSTEM_NAME}'")
    endif()
endif()

if(NOT KATIE_COMPILER)
    if (CMAKE_CXX_COMPILER_ID MATCHES "(Clang|AppleClang)")
        set(KATIE_COMPILER "clang")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(KATIE_COMPILER "gcc")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        set(KATIE_COMPILER "icc")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(KATIE_COMPILER "msvc")
    else()
        message(FATAL_ERROR "Unknown compiler '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

if(CMAKE_SYSTEM_PROCESSOR)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_SYSTEM_PROCESSOR)
elseif(CMAKE_HOST_SYSTEM_PROCESSOR)
    string(TOLOWER ${CMAKE_HOST_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_SYSTEM_PROCESSOR)
endif()
if(NOT KATIE_ARCHITECTURE)
    if(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
        set(KATIE_ARCHITECTURE "arm")
    elseif(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "^mips")
        set(KATIE_ARCHITECTURE "mips")
    elseif(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|amd64)")
        set(KATIE_ARCHITECTURE "x86_64")
    elseif(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "(x86|i[3-6]86)")
        set(KATIE_ARCHITECTURE "i386")
    else()
        message(FATAL_ERROR "Unknown CPU '${CMAKE_SYSTEM_PROCESSOR}'")
    endif()
endif()

# FIXME: Plan9 does not supporting dynamic libraries
if(NOT KATIE_TYPE)
    set(KATIE_TYPE SHARED)
    katie_definition(-DQT_SHARED)
endif()

if(KATIE_PLATFORM MATCHES "(win32|wince)" AND NOT KATIE_TYPE STREQUAL SHARED)
    katie_definition(-DQT_MAKEDLL)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    katie_definition(-DQT_NO_DEBUG -DNDEBUG)
endif()

include(${KATIE_MKSPECS_DIR}/tests/tests.cmake)

# vendors are free to change them, paths are relative to CMAKE_INSTALL_PREFIX
# otherwise CPack will attempt to install to the root directory on UNIX and
# fail to create the package. if vendors do not use CPack to generate their
# packages then paths can be full paths as seen in the shipped package files.
set(LDCONF_INSTALL_DIR "etc/ld.so.conf.d")
set(PROFILE_INSTALL_DIR "etc/profile.d")
set(MAN_INSTALL_DIR "share/man")
set(CMAKE_INSTALL_DIR "cmake")

if(NOT KATIE_KEY)
    set(KATIE_KEY "${KATIE_ARCHITECTURE} ${KATIE_PLATFORM} ${KATIE_COMPILER} full-config")
endif()

if(EXISTS ${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/${KATIE_PLATFORM}.cmake)
    include(${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/${KATIE_PLATFORM}.cmake)
endif()

# for distributions to override build specifications
if(EXISTS ${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/vendor.cmake)
    include(${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/vendor.cmake)
endif()
