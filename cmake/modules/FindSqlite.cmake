# - Try to find SQL database engine
# Once done this will define
#
#  SQLITE_FOUND - system has SQL database engine
#  SQLITE_INCLUDES - the SQL database engine include directory
#  SQLITE_LIBRARIES - the libraries needed to use SQL database engine
#
# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
pkg_check_modules(PC_SQLITE QUIET sqlite3)

set(SQLITE_INCLUDES ${PC_SQLITE_INCLUDE_DIRS})
set(SQLITE_LIBRARIES ${PC_SQLITE_LIBRARIES})

if(NOT SQLITE_INCLUDES OR NOT SQLITE_LIBRARIES)
    find_path(SQLITE_INCLUDES
        NAMES sqlite3.h
        PATH_SUFFIXES sqlite3
        HINTS $ENV{SQLITEDIR}/include
    )

    find_library(SQLITE_LIBRARIES
        NAMES sqlite3
        HINTS $ENV{SQLITEDIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite
    VERSION_VAR PC_SQLITE_VERSION
    REQUIRED_VARS SQLITE_LIBRARIES SQLITE_INCLUDES
)

mark_as_advanced(SQLITE_INCLUDES SQLITE_LIBRARIES)
