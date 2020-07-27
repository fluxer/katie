# - Try to find resolv
# Once done this will define
#
#  RESOLV_FOUND - system has resolv
#  RESOLV_INCLUDES - the resolv include directory
#  RESOLV_LIBRARIES - the libraries needed to use resolv
#  RESOLV_THREAD_SAFE - resolv has thread-safe functions (res_ninit/res_nclose)
#
# Copyright (c) 2016-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

# resolv does not provide pkg-config files

set(RESOLV_NAMES c resolv)

find_path(RESOLV_INCLUDES
    NAMES resolv.h
    HINTS $ENV{RESOLVDIR}/include
)

set(RESOLV_LIBRARIES)
foreach(name ${RESOLV_NAMES})
    if(NOT HAVE_res_ninit AND NOT HAVE_res_init)
        cmake_reset_check_state()
        set(CMAKE_REQUIRED_LIBRARIES ${name})
        katie_check_function(res_ninit "resolv.h")
        cmake_pop_check_state()
        if(NOT HAVE_res_ninit)
            cmake_reset_check_state()
            set(CMAKE_REQUIRED_LIBRARIES ${name})
            katie_check_function(res_init "resolv.h")
            cmake_pop_check_state()
        endif()
        if(HAVE_res_ninit OR HAVE_res_init)
            find_library(RESOLV_LIBRARIES
                NAMES ${name}
                HINTS $ENV{RESOLVDIR}/lib
            )
        endif()
    endif()
endforeach()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Resolv
    REQUIRED_VARS RESOLV_LIBRARIES RESOLV_INCLUDES
)

set(RESOLV_THREAD_SAFE "${HAVE_res_ninit}" CACHE BOOL "resolv has thread-safe functions (res_ninit/res_nclose)")

mark_as_advanced(RESOLV_INCLUDES RESOLV_LIBRARIES RESOLV_THREAD_SAFE)
