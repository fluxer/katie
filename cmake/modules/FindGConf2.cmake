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
    pkg_check_modules(PC_SQLITE QUIET gconf-2.0)
endif()

find_path(GCONF2_INCLUDES
    NAMES
    gconf/gconf.h
    PATH_SUFFIXES gconf/2
    HINTS
    $ENV{SQLITEDIR}/include
    ${PC_GCONF2_INCLUDEDIR}
    /usr/include
    /usr/local/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(GCONF2_LIBRARIES
    gconf-2
    HINTS
    $ENV{SQLITEDIR}/lib
    ${PC_GCONF2_LIBDIR}
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GConf2 DEFAULT_MSG GCONF2_INCLUDES GCONF2_LIBRARIES)

mark_as_advanced(GCONF2_INCLUDES GCONF2_LIBRARIES)
