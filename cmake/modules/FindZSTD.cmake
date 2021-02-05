# - Try to find ZSTD
# Once done this will define
#
#  ZSTD_FOUND - system has ZSTD
#  ZSTD_INCLUDES - the ZSTD include directory
#  ZSTD_LIBRARIES - the libraries needed to use ZSTD
#
# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_ZSTD QUIET libzstd)

find_path(ZSTD_INCLUDES
    NAMES zstd.h
    HINTS $ENV{ZSTDDIR}/include ${PC_ZSTD_INCLUDEDIR}
)

find_library(ZSTD_LIBRARIES
    NAMES zstd
    HINTS $ENV{ZSTDDIR}/lib ${PC_ZSTD_LIBDIR}
)

find_package_handle_standard_args(ZSTD
    VERSION_VAR PC_ZSTD_VERSION
    REQUIRED_VARS ZSTD_LIBRARIES ZSTD_INCLUDES
)

mark_as_advanced(ZSTD_INCLUDES ZSTD_LIBRARIES)
