# - Try to find Network Information Service
# Once done this will define
#
#  NIS_FOUND - system has NIS
#  NIS_INCLUDES - the NIS include directory
#  NIS_LIBRARIES - the libraries needed to use NIS
#
# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

# NIS does not provide pkg-config files

find_path(NIS_INCLUDES
    NAMES rpc/rpc.h
    HINTS $ENV{NISDIR}/include
)

find_library(NIS_LIBRARIES
    NAMES nsl
    HINTS $ENV{NISDIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NIS
    REQUIRED_VARS NIS_LIBRARIES NIS_INCLUDES
)

mark_as_advanced(NIS_INCLUDES NIS_LIBRARIES)
