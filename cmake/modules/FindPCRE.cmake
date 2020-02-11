# - Try to find the PCRE
# Once done this will define
#
#  PCRE_FOUND - system has PCRE
#  PCRE_INCLUDES - the PCRE include directory
#  PCRE_LIBRARIES - the libraries needed to use PCRE
#
# Copyright (c) 2016-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(PCRE_INCLUDES AND PCRE_LIBRARIES)
    set(PCRE_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_PCRE QUIET libpcre)

    set(PCRE_INCLUDES ${PC_PCRE_INCLUDE_DIRS})
    set(PCRE_LIBRARIES ${PC_PCRE_LIBRARIES})
endif()

if(NOT PCRE_INCLUDES OR NOT PCRE_LIBRARIES)
    find_path(PCRE_INCLUDES
        NAMES pcre.h
        HINTS $ENV{PCREDIR}/include
    )

    find_library(PCRE_LIBRARIES
        NAMES pcre
        HINTS $ENV{PCREDIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCRE
    VERSION_VAR PC_PCRE_VERSION
    REQUIRED_VARS PCRE_LIBRARIES PCRE_INCLUDES
)

mark_as_advanced(PCRE_INCLUDES PCRE_LIBRARIES)
