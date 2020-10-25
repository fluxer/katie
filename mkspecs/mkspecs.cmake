# KatieConfig overrides KATIE_TYPE

if(NOT KATIE_TYPE)
    set(KATIE_TYPE SHARED)
    katie_definition(-DQT_SHARED)
endif()

# https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
# https://github.com/Kitware/CMake/blob/master/Modules/CMakeDetermineSystem.cmake
if(NOT KATIE_PLATFORM)
    if(CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(KATIE_PLATFORM "linux")
        katie_definition(-D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -D_GNU_SOURCE)
    elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
        set(KATIE_PLATFORM "freebsd")
        katie_definition(-D_THREAD_SAFE)
    elseif(CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
        set(KATIE_PLATFORM "openbsd")
        katie_definition(-D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE)
    elseif(CMAKE_SYSTEM_NAME MATCHES "GNU")
        set(KATIE_PLATFORM "hurd")
        # TODO: katie_definition(-D_THREAD_SAFE)
    elseif(CMAKE_SYSTEM_NAME MATCHES "(Solaris|SunOS)")
        set(KATIE_PLATFORM "solaris")
        # TODO: katie_definition(-D_THREAD_SAFE)
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
    else()
        message(FATAL_ERROR "Unknown compiler '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    katie_definition(-DQT_NO_DEBUG)
endif()

try_compile(bsymbolic_functions_test
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/bsymbolic_functions.cpp
    CMAKE_FLAGS -shared -Wl,-Bsymbolic-functions -fPIC
    OUTPUT_VARIABLE bsymbolic_functions_test_output
)
if(NOT bsymbolic_functions_test OR NOT CMAKE_POSITION_INDEPENDENT_CODE)
    katie_definition(-DQ_NO_DATA_RELOCATION)
endif()

# Set compiler standard to C++ 11, appending to CMAKE_CXX_FLAGS is done so that try_compile() is
# also affected, which KDE uses for visibility check for an example
if(NOT CMAKE_VERSION VERSION_LESS "3.8.0")
    cmake_policy(GET CMP0067 trycompilestandardpolicy)
else()
    set(trycompilestandardpolicy OLD)
endif()
if(NOT trycompilestandardpolicy EQUAL NEW)
    if(KATIE_COMPILER STREQUAL "gcc")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    elseif(KATIE_COMPILER STREQUAL "clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
endif()
if(NOT CMAKE_VERSION VERSION_LESS "3.1.0")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_STANDARD 11)
endif()
