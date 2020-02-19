# - Try to find NSL
# Once done this will define
#
#  NSL_FOUND - system has NSL
#  NSL_INCLUDES - the NSL include directory
#  NSL_LIBRARIES - the libraries needed to use NSL
#
# Copyright (c) 2016-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

# NSL does not provide pkg-config files

find_path(NSL_INCLUDES
    NAMES rpcsvc/ypclnt.h
    HINTS $ENV{NSLDIR}/include
)

find_library(NSL_LIBRARIES
    NAMES nsl c
    HINTS $ENV{NSLDIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NSL
    REQUIRED_VARS NSL_LIBRARIES NSL_INCLUDES
)

mark_as_advanced(NSL_INCLUDES NSL_LIBRARIES)
