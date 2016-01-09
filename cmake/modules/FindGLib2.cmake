# - Try to find GLib2 core application building
# Once done this will define
#
#  GLIB2_FOUND - system has GLib2
#  GLIB2_INCLUDES - the GLib2 include directory
#  GLIB2_CONFIG_INCLUDES - the D-Bus message bus architecture include directory
#  GLIB2_LIBRARIES - The libraries needed to use GLib2
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(GLIB2_INCLUDES AND GLIB2_LIBRARIES)
    set(GLIB2_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_GLIB2 QUIET glib-2.0)
endif()

find_path(GLIB2_INCLUDES
    NAMES
    glib.h
    PATH_SUFFIXES glib-2.0
    HINTS
    $ENV{GLIB2DIR}/include
    ${PC_GLIB2_INCLUDEDIR}
    /usr/include
    /usr/local/include
    ${INCLUDE_INSTALL_DIR}
)

find_path(GLIB2_CONFIG_INCLUDES
    NAMES
    glibconfig.h
    PATH_SUFFIXES glib-2.0/include
    HINTS
    $ENV{GLIB2DIR}/include
    $ENV{GLIB2DIR}/lib
    ${PC_GLIB2_INCLUDEDIR}
    ${PC_GLIB2_LIBDIR}
    /usr/include
    /usr/local/include
    /usr/lib
    /usr/local/lib
    ${INCLUDE_INSTALL_DIR}
)

if(GLIB2_INCLUDES AND GLIB2_CONFIG_INCLUDES)
    set(GLIB2_INCLUDES ${GLIB2_INCLUDES} ${GLIB2_CONFIG_INCLUDES})
endif()

find_library(GLIB2_LIBRARIES
    glib-2.0
    HINTS
    $ENV{GLIB2DIR}/lib
    ${PC_GLIB2_LIBDIR}
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLib2 DEFAULT_MSG GLIB2_INCLUDES GLIB2_CONFIG_INCLUDES GLIB2_LIBRARIES)

mark_as_advanced(GLIB2_INCLUDES GLIB2_CONFIG_INCLUDES GLIB2_LIBRARIES)
