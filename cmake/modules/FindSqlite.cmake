# - Try to find SQL database engine
# Once done this will define
#
#  SQLITE_FOUND - system has SQL database engine
#  SQLITE_INCLUDES - the SQL database engine include directory
#  SQLITE_LIBRARIES - the libraries needed to use SQL database engine
#
# Copyright (c) 2015-2021, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_SQLITE QUIET sqlite3)

find_path(SQLITE_INCLUDES
    NAMES sqlite3.h
    PATH_SUFFIXES sqlite3
    HINTS $ENV{SQLITEDIR}/include ${PC_SQLITE_INCLUDEDIR}
)

find_library(SQLITE_LIBRARIES
    NAMES sqlite3
    HINTS $ENV{SQLITEDIR}/lib ${PC_SQLITE_LIBDIR}
)

find_package_handle_standard_args(SQLite
    VERSION_VAR PC_SQLITE_VERSION
    REQUIRED_VARS SQLITE_LIBRARIES SQLITE_INCLUDES
)

mark_as_advanced(SQLITE_INCLUDES SQLITE_LIBRARIES)
