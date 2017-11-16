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

# FIXME: Plan9/NaCL does not supporting dynamic libraries
if(NOT KATIE_TYPE)
    set(KATIE_TYPE SHARED)
    katie_definition(-DQT_SHARED)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    katie_definition(-DQT_NO_DEBUG -DNDEBUG)
endif()

# TODO: check if data relocations should be disabled before any target additions and define
# Q_NO_DATA_RELOCATION

if(NOT KATIE_KEY)
    set(KATIE_KEY "${KATIE_ARCHITECTURE} ${KATIE_PLATFORM} ${KATIE_COMPILER} full-config")
endif()

# Set compiler standard to C++ 11, appending to CMAKE_CXX_FLAGS is done so that try_compile() is
# also affected, which KDE uses for visibility check for an example
if(KATIE_COMPILER STREQUAL "gcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    # TODO: set CMAKE_CXX_EXTENSIONS to OFF so it does not append the flag again?
elseif(KATIE_COMPILER STREQUAL "clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
if(NOT CMAKE_VERSION VERSION_LESS "3.1.0")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_STANDARD 11)
endif()

if(EXISTS ${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/${KATIE_PLATFORM}.cmake)
    include(${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/${KATIE_PLATFORM}.cmake)
endif()

# for distributions to override build specifications, do note that paths specified on the command
# line during configuration override even that file
if(EXISTS ${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/vendor.cmake)
    include(${KATIE_MKSPECS_DIR}/${KATIE_PLATFORM}/vendor.cmake)
endif()
