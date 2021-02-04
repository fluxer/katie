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
