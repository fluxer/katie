# - Try to find D-Bus message bus system
# Once done this will define
#
#  DBUS_FOUND - system has D-Bus message bus system
#  DBUS_INCLUDES - the D-Bus message bus system include directory
#  DBUS_ARCH_INCLUDES - the D-Bus message bus architecture include directory
#  DBUS_LIBRARIES - The libraries needed to use D-Bus message bus system
#
# Copyright (c) 2015-2019, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(DBUS_INCLUDES AND DBUS_LIBRARIES)
    set(DBUS_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_DBUS QUIET dbus-1)
endif()

find_path(DBUS_INCLUDES
    NAMES
    dbus/dbus.h
    PATH_SUFFIXES dbus-1.0
    HINTS
    $ENV{DBUSDIR}/include
    ${PC_DBUS_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_path(DBUS_ARCH_INCLUDES
    NAMES
    dbus/dbus-arch-deps.h
    PATH_SUFFIXES dbus-1.0/include
    HINTS
    $ENV{DBUSDIR}/include
    $ENV{DBUSDIR}/lib
    ${PC_DBUS_INCLUDEDIR}
    ${PC_DBUS_LIBDIR}
    ${INCLUDE_INSTALL_DIR}
    ${LIB_INSTALL_DIR}
)

if(DBUS_INCLUDES AND DBUS_ARCH_INCLUDES)
    set(DBUS_INCLUDES ${DBUS_INCLUDES} ${DBUS_ARCH_INCLUDES})
endif()

find_library(DBUS_LIBRARIES
    dbus-1
    HINTS
    $ENV{DBUSDIR}/lib
    ${PC_DBUS_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DBus
    VERSION_VAR PC_DBUS_VERSION
    REQUIRED_VARS DBUS_LIBRARIES DBUS_INCLUDES DBUS_ARCH_INCLUDES
)

mark_as_advanced(DBUS_INCLUDES DBUS_ARCH_INCLUDES DBUS_LIBRARIES)
