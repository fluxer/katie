macro(KATIE_DEFINITION DEF)
    set(KATIE_DEFINITIONS ${KATIE_DEFINITIONS} ${DEF} ${ARGN})
    add_definitions(${DEF} ${ARGN})
endmacro()

# https://www.gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html
# https://github.com/Kitware/CMake/blob/master/Modules/CMakeDetermineSystem.cmake
if(CMAKE_SYSTEM_NAME MATCHES "(Linux|GNU)")
    katie_definition(-D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -D_GNU_SOURCE)
elseif(CMAKE_SYSTEM_NAME MATCHES "(FreeBSD|DragonFly)")
    katie_definition(-D_THREAD_SAFE)
elseif(NOT CMAKE_SYSTEM_NAME MATCHES "(OpenBSD|NetBSD|Solaris|SunOS)")
    message(FATAL_ERROR "Unknown platform '${CMAKE_SYSTEM_NAME}'")
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    katie_definition(-DQT_NO_DEBUG)
endif()

# Set compiler standard to C++ 11, appending to CMAKE_CXX_FLAGS is done so that try_compile() is
# also affected, which KDE uses for visibility check for an example
if(NOT CMAKE_VERSION VERSION_LESS "3.8.0")
    cmake_policy(GET CMP0067 trycompilestandardpolicy)
else()
    set(trycompilestandardpolicy OLD)
endif()
if(NOT trycompilestandardpolicy EQUAL NEW)
    # using MATCHES in all checks due to CMP0054
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR "Unknown compiler '${CMAKE_CXX_COMPILER_ID}'")
    endif()
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
