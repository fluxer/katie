# - Try to find execinfo
# Once done this will define
#
#  EXECINFO_FOUND - system has execinfo
#  EXECINFO_INCLUDES - the execinfo include directory
#  EXECINFO_LIBRARIES - the libraries needed to use execinfo
#
# Copyright (C) 2016, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

# execinfo does not provide pkg-config files

include(CMakePushCheckState)
include(FindPackageHandleStandardArgs)

set(EXECINFO_NAMES c execinfo)

find_path(EXECINFO_INCLUDES
    NAMES execinfo.h
    HINTS $ENV{EXECINFODIR}/include
)

set(EXECINFO_LIBRARIES)
foreach(name ${EXECINFO_NAMES})
    if(NOT EXECINFO_LIBRARIES)
        unset(HAVE_backtrace_symbols CACHE)
        cmake_reset_check_state()
        set(CMAKE_REQUIRED_LIBRARIES ${name})
        katie_check_defined(backtrace_symbols "execinfo.h")
        cmake_reset_check_state()
        if(HAVE_backtrace_symbols)
            find_library(EXECINFO_LIBRARIES
                NAMES ${name}
                HINTS $ENV{EXECINFODIR}/lib
            )
        endif()
    endif()
endforeach()

find_package_handle_standard_args(ExecInfo
    REQUIRED_VARS EXECINFO_LIBRARIES EXECINFO_INCLUDES
)

mark_as_advanced(EXECINFO_INCLUDES EXECINFO_LIBRARIES)
