# - Try to find the libmng
# Once done this will define
#
#  MNG_FOUND - system has libmng
#  MNG_INCLUDES - the libmng include directory
#  MNG_LIBRARIES - the libraries needed to use libmng
#
# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_MNG QUIET libmng)

    set(MNG_INCLUDES ${PC_MNG_INCLUDE_DIRS})
    set(MNG_LIBRARIES ${PC_MNG_LIBRARIES})
endif()

if(NOT MNG_INCLUDES OR NOT MNG_LIBRARIES)
    find_path(MNG_INCLUDES
        NAMES libmng.h
        PATH_SUFFIXES mng
        HINTS $ENV{MNGDIR}/include
    )

    find_library(MNG_LIBRARIES
        NAMES mng
        HINTS $ENV{MNGDIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MNG
    VERSION_VAR PC_MNG_VERSION
    REQUIRED_VARS MNG_LIBRARIES MNG_INCLUDES
)

mark_as_advanced(MNG_INCLUDES MNG_LIBRARIES)
