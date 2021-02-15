# - Try to find D-Bus message bus system
# Once done this will define
#
#  DBUS_FOUND - system has D-Bus message bus system
#  DBUS_INCLUDES - the D-Bus message bus system include directory
#  DBUS_ARCH_INCLUDES - the D-Bus message bus architecture include directory
#  DBUS_LIBRARIES - the libraries needed to use D-Bus message bus system
#
# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_DBUS QUIET dbus-1)

find_path(DBUS_INCLUDES
    NAMES dbus/dbus.h
    PATH_SUFFIXES dbus-1.0
    HINTS $ENV{DBUSDIR}/include ${PC_DBUS_INCLUDEDIR}
)

find_path(DBUS_ARCH_INCLUDES
    NAMES dbus/dbus-arch-deps.h
    PATH_SUFFIXES dbus-1.0/include
    HINTS $ENV{DBUSDIR}/include $ENV{DBUSDIR}/lib ${PC_DBUS_INCLUDEDIR} ${PC_DBUS_LIBDIR}
)

find_library(DBUS_LIBRARIES
    NAMES dbus-1
    HINTS $ENV{DBUSDIR}/lib ${PC_DBUS_LIBDIR}
)

if(DBUS_INCLUDES AND DBUS_ARCH_INCLUDES)
    set(DBUS_INCLUDES ${DBUS_INCLUDES} ${DBUS_ARCH_INCLUDES})
endif()

find_package_handle_standard_args(DBus
    VERSION_VAR PC_DBUS_VERSION
    REQUIRED_VARS DBUS_LIBRARIES DBUS_INCLUDES DBUS_ARCH_INCLUDES
)

mark_as_advanced(DBUS_INCLUDES DBUS_ARCH_INCLUDES DBUS_LIBRARIES)
