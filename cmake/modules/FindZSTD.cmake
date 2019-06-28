# - Try to find ZSTD
# Once done this will define
#
#  ZSTD_FOUND - system has ZSTD
#  ZSTD_INCLUDES - the ZSTD include directory
#  ZSTD_LIBRARIES - The libraries needed to use ZSTD
#
# Copyright (c) 2015-2019, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(ZSTD_INCLUDES AND ZSTD_LIBRARIES)
    set(ZSTD_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_ZSTD QUIET libzstd)
endif()

find_path(ZSTD_INCLUDES
    NAMES
    zstd.h
    HINTS
    $ENV{ZSTDDIR}/include
    ${PC_ZSTD_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(ZSTD_LIBRARIES
    zstd
    HINTS
    $ENV{ZSTDDIR}/lib
    ${PC_ZSTD_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZSTD
    VERSION_VAR PC_ZSTD_VERSION
    REQUIRED_VARS ZSTD_LIBRARIES ZSTD_INCLUDES
)

mark_as_advanced(ZSTD_INCLUDES ZSTD_LIBRARIES)
