# - Try to find GConf2
# Once done this will define
#
#  GCONF2_FOUND - system has GConf2
#  GCONF2_INCLUDES - the GConf2 include directory
#  GCONF2_LIBRARIES - The libraries needed to use GConf2
#
# Copyright (c) 2016, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(GCONF2_INCLUDES AND GCONF2_LIBRARIES)
    set(GCONF2_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_GCONF2 QUIET gconf-2.0)
endif()

find_path(GCONF2_INCLUDES
    NAMES
    gconf/gconf.h
    PATH_SUFFIXES gconf/2
    HINTS
    $ENV{GCONF2DIR}/include
    ${PC_GCONF2_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(GCONF2_LIBRARIES
    gconf-2
    HINTS
    $ENV{GCONF2DIR}/lib
    ${PC_GCONF2_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GConf2
    VERSION_VAR PC_GCONF2_VERSION
    REQUIRED_VARS GCONF2_LIBRARIES GCONF2_INCLUDES
)

mark_as_advanced(GCONF2_INCLUDES GCONF2_LIBRARIES)
