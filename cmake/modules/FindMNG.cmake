# - Try to find the libmng
# Once done this will define
#
#  MNG_FOUND - system has libmng
#  MNG_INCLUDES - the libmng include directory
#  MNG_LIBRARIES - The libraries needed to use libmng
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(MNG_INCLUDES AND MNG_LIBRARIES)
    set(MNG_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_MNG QUIET libmng)
endif()

find_path(MNG_INCLUDES
    NAMES
    libmng.h
    PATH_SUFFIXES mng
    HINTS
    $ENV{MNGDIR}/include
    ${PC_MNG_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(MNG_LIBRARIES
    mng
    HINTS
    $ENV{MNGDIR}/lib
    ${PC_MNG_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MNG
    VERSION_VAR PC_MNG_VERSION
    REQUIRED_VARS MNG_LIBRARIES MNG_INCLUDES
)

mark_as_advanced(MNG_INCLUDES MNG_LIBRARIES)
