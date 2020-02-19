# - Try to find resolv
# Once done this will define
#
#  RESOLV_FOUND - system has resolv
#  RESOLV_INCLUDES - the resolv include directory
#  RESOLV_LIBRARIES - the libraries needed to use resolv
#
# Copyright (c) 2016-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

# resolv does not provide pkg-config files

find_path(RESOLV_INCLUDES
    NAMES resolv.h
    HINTS $ENV{RESOLVDIR}/include
)

find_library(RESOLV_LIBRARIES
    NAMES resolv c
    HINTS $ENV{RESOLVDIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Resolv
    REQUIRED_VARS RESOLV_LIBRARIES RESOLV_INCLUDES
)

mark_as_advanced(RESOLV_INCLUDES RESOLV_LIBRARIES)
