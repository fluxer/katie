# - Try to find resolv
# Once done this will define
#
#  RESOLV_FOUND - system has resolv
#  RESOLV_INCLUDES - the resolv include directory
#  RESOLV_LIBRARIES - the libraries needed to use resolv
#
# Copyright (c) 2016-2019, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(RESOLV_INCLUDES AND RESOLV_LIBRARIES)
    set(RESOLV_FIND_QUIETLY TRUE)
endif()

# resolv does not provide pkg-config files

find_path(RESOLV_INCLUDES
    NAMES
    resolv.h
    HINTS
    $ENV{RESOLVDIR}/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(RESOLV_LIBRARIES
    NAMES
    resolv c
    HINTS
    $ENV{RESOLVDIR}/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Resolv
    REQUIRED_VARS RESOLV_LIBRARIES RESOLV_INCLUDES
)

mark_as_advanced(RESOLV_INCLUDES RESOLV_LIBRARIES)
