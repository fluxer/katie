# - Try to find the PCRE
# Once done this will define
#
#  PCRE_FOUND - system has PCRE
#  PCRE_INCLUDES - the PCRE include directory
#  PCRE_LIBRARIES - The libraries needed to use PCRE
#
# Copyright (c) 2016, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(PCRE_INCLUDES AND PCRE_LIBRARIES)
    set(PCRE_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_PCRE QUIET libpcre)
endif()

find_path(PCRE_INCLUDES
    NAMES
    pcre.h
    HINTS
    $ENV{PCREDIR}/include
    ${PC_PCRE_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(PCRE_LIBRARIES
    pcre
    HINTS
    $ENV{PCREDIR}/lib
    ${PC_PCRE_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCRE
    VERSION_VAR PC_PCRE_VERSION
    REQUIRED_VARS PCRE_LIBRARIES PCRE_INCLUDES
)

mark_as_advanced(PCRE_INCLUDES PCRE_LIBRARIES)
