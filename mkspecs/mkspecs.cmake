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
    # using MATCHES in all checks due to CMP0054
    if (CMAKE_CXX_COMPILER_ID MATCHES "(Clang|AppleClang)")
        set(KATIE_COMPILER "clang")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(KATIE_COMPILER "gcc")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
        set(KATIE_COMPILER "icc")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
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

if(NOT KATIE_KEY)
    set(KATIE_KEY "${KATIE_ARCHITECTURE} ${KATIE_PLATFORM} ${KATIE_COMPILER} full-config")
endif()

# forward setup of configuration paths, overriden by platform mkspecs
set(QT_PREFIX_PATH "" CACHE PATH "General installation prefix")
set(QT_HEADERS_PATH "" CACHE PATH "Headers installation path")
set(QT_LIBRARIES_PATH "" CACHE PATH "Libraries installation path")
set(QT_BINARIES_PATH "" CACHE PATH "Binaries installation path")
set(QT_PLUGINS_PATH "" CACHE PATH "Plugins installation path")
set(QT_IMPORTS_PATH "" CACHE PATH "Declerative imports installation path")
set(QT_DATA_PATH "" CACHE PATH "Generic data installation path")
set(QT_TRANSLATIONS_PATH "" CACHE PATH "Translations instllation path")
set(QT_DOCUMENTATION_PATH "" CACHE PATH "Documenation installation path")
set(QT_EXAMPLES_PATH "" CACHE PATH "Examples installation path")
set(QT_DEMOS_PATH "" CACHE PATH "Demos installation path")
set(QT_SETTINGS_PATH "" CACHE PATH "Settings runtime path")

if(EXISTS ${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/${KATIE_PLATFORM}.cmake)
    include(${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/${KATIE_PLATFORM}.cmake)
endif()

# misc paths, relevant mostly for UNIX-like platforms
set(CMAKE_INSTALL_PATH "${QT_DATA_PATH}/cmake/Katie" CACHE PATH "CMake aware modules path")
set(LDCONF_INSTALL_PATH "${QT_DATA_PATH}/ld.so.conf.d" CACHE PATH "Run-time linker/loader configs path (UNIX)")
set(PROFILE_INSTALL_PATH "${QT_DATA_PATH}/profile.d" CACHE PATH "Shell profile scripts path (UNIX)")
set(MAN_INSTALL_PATH "${QT_DATA_PATH}/man" CACHE PATH "Manual pages path (UNIX)")
set(APPLICATIONS_INSTALL_PATH "${QT_DATA_PATH}/applications" CACHE PATH "Desktop applications register path (UNIX)")
set(PIXMAPS_INSTALL_PATH "${QT_DATA_PATH}/pixmaps" CACHE PATH "Desktop applications icon path (UNIX)")

# for distributions to override build specifications, do note that paths specified on the command
# line during configuration override even that file
if(EXISTS ${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/vendor.cmake)
    include(${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/vendor.cmake)
endif()
