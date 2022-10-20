# Try to find libdeflate, once done this will define:
#
#  DEFLATE_FOUND - system has libdeflate
#  DEFLATE_INCLUDES - the libdeflate include directory
#  DEFLATE_LIBRARIES - the libraries needed to use libdeflate
#
# Copyright (C) 2022 Ivailo Monev <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

find_package(PkgConfig REQUIRED)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_DEFLATE QUIET libdeflate)

find_path(DEFLATE_INCLUDES
    NAMES libdeflate.h
    HINTS $ENV{DEFLATEDIR}/include ${PC_DEFLATE_INCLUDEDIR}
)

find_library(DEFLATE_LIBRARIES
    NAMES deflate
    HINTS $ENV{DEFLATEDIR}/lib ${PC_DEFLATE_LIBDIR}
)

find_package_handle_standard_args(Deflate
    VERSION_VAR PC_DEFLATE_VERSION
    REQUIRED_VARS DEFLATE_LIBRARIES DEFLATE_INCLUDES
)

mark_as_advanced(DEFLATE_INCLUDES DEFLATE_LIBRARIES)
