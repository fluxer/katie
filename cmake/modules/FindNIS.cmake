# - Try to find Network Information Service
# Once done this will define
#
#  NIS_FOUND - system has NIS
#  NIS_INCLUDES - the NIS include directory
#  NIS_LIBRARIES - The libraries needed to use NIS
#
# Copyright (c) 2015-2019, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(NIS_INCLUDES AND NIS_LIBRARIES)
    set(NIS_FIND_QUIETLY TRUE)
endif()

# NIS does not provide pkg-config files

find_path(NIS_INCLUDES
    NAMES
    rpc/rpc.h
    HINTS
    $ENV{NISDIR}/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(NIS_LIBRARIES
    nsl
    HINTS
    $ENV{NISDIR}/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NIS DEFAULT_MSG NIS_INCLUDES NIS_LIBRARIES)

mark_as_advanced(NIS_INCLUDES NIS_LIBRARIES)
