# - Try to find SQL database engine
# Once done this will define
#
#  SQLITE_FOUND - system has SQL database engine
#  SQLITE_INCLUDES - the SQL database engine include directory
#  SQLITE_LIBRARIES - The libraries needed to use SQL database engine
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(SQLITE_INCLUDES AND SQLITE_LIBRARIES)
    set(SQLITE_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_SQLITE QUIET sqlite3)
endif()

find_path(SQLITE_INCLUDES
    NAMES
    sqlite3.h
    PATH_SUFFIXES sqlite3
    HINTS
    $ENV{SQLITEDIR}/include
    ${PC_SQLITE_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(SQLITE_LIBRARIES
    sqlite3
    HINTS
    $ENV{SQLITEDIR}/lib
    ${PC_SQLITE_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite
    VERSION_VAR PC_SQLITE_VERSION
    REQUIRED_VARS SQLITE_LIBRARIES SQLITE_INCLUDES
)

mark_as_advanced(SQLITE_INCLUDES SQLITE_LIBRARIES)
