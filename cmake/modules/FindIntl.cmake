# - Try to find intl
# Once done this will define
#
#  INTL_FOUND - system has intl
#  INTL_INCLUDES - the intl include directory
#  INTL_LIBRARIES - the libraries needed to use intl
#  INTL_MSGFMT - the msgfmt binary
#
# Copyright (C) 2016, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

# intl does not provide pkg-config files

include(CMakePushCheckState)
include(FindPackageHandleStandardArgs)

set(INTL_NAMES c intl)

find_path(INTL_INCLUDES
    NAMES libintl.h
    HINTS $ENV{INTLDIR}/include
)

set(INTL_LIBRARIES)
foreach(name ${INTL_NAMES})
    if(NOT INTL_LIBRARIES)
        unset(HAVE_bind_textdomain_codeset CACHE)
        cmake_reset_check_state()
        set(CMAKE_REQUIRED_LIBRARIES ${name})
        katie_check_defined(bind_textdomain_codeset "libintl.h")
        cmake_reset_check_state()
        if(HAVE_bind_textdomain_codeset)
            find_library(INTL_LIBRARIES
                NAMES ${name}
                HINTS $ENV{INTLDIR}/lib
            )
        endif()
    endif()
endforeach()

find_program(INTL_MSGFMT
    NAMES msgfmt
    HINTS $ENV{INTLDIR}/bin
)

find_package_handle_standard_args(Intl
    REQUIRED_VARS INTL_LIBRARIES INTL_INCLUDES INTL_MSGFMT
)

mark_as_advanced(INTL_INCLUDES INTL_LIBRARIES INTL_MSGFMT)
