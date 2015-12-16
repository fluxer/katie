# - Try to find D-Bus message bus system
# Once done this will define
#
#  DBUS_FOUND - system has D-Bus message bus system
#  DBUS_INCLUDES - the D-Bus message bus system include directory
#  DBUS_LIBRARIES - The libraries needed to use D-Bus message bus system
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(DBUS_INCLUDES AND DBUS_LIBRARIES)
    set(DBUS_FIND_QUIETLY TRUE)
endif()

find_path(DBUS_INCLUDES
    NAMES
    dbus/dbus.h
    PATH_SUFFIXES dbus-1.0
    HINTS
    $ENV{DBUSDIR}/include
    /usr/include
    /usr/local/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(DBUS_LIBRARIES
    dbus-1
    HINTS
    $ENV{DBUSDIR}/lib
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DBUS DEFAULT_MSG DBUS_INCLUDES DBUS_LIBRARIES)

mark_as_advanced(DBUS_INCLUDES DBUS_LIBRARIES)
