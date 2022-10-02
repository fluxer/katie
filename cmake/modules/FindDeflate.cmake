# - Try to find libdeflate
# Once done this will define
#
#  DEFLATE_FOUND - system has libdeflate
#  DEFLATE_INCLUDES - the libdeflate include directory
#  DEFLATE_LIBRARIES - the libraries needed to use libdeflate
#
# Copyright (C) 2022, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

# libdeflate does not provide pkg-config files

find_package(PkgConfig REQUIRED)
include(FindPackageHandleStandardArgs)

find_path(DEFLATE_INCLUDES
    NAMES libdeflate.h
    HINTS $ENV{DEFLATEDIR}/include
)

find_library(DEFLATE_LIBRARIES
    NAMES deflate
    HINTS $ENV{DEFLATEDIR}/lib
)

find_package_handle_standard_args(Deflate
    REQUIRED_VARS DEFLATE_LIBRARIES DEFLATE_INCLUDES
)

mark_as_advanced(DEFLATE_INCLUDES DEFLATE_LIBRARIES)
