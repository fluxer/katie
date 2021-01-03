# - Try to find ODBC
# Once done this will define
#
#  ODBC_FOUND - system has ODBC
#  ODBC_INCLUDES - the ODBC include directory
#  ODBC_LIBRARIES - the libraries needed to use ODBC
#
# Copyright (c) 2015-2021, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_ODBC QUIET odbc)
if(NOT PC_ODBC_FOUND)
    pkg_check_modules(PC_ODBC QUIET libiodbc)
endif()

find_path(ODBC_INCLUDES
    NAMES sql.h
    PATH_SUFFIXES iodbc libiodbc
    HINTS $ENV{ODBCDIR}/include ${PC_ODBC_INCLUDEDIR}
)

find_library(ODBC_LIBRARIES
    NAMES odbc iodbc
    HINTS $ENV{ODBCDIR}/lib ${PC_ODBC_LIBDIR}
)

find_package_handle_standard_args(ODBC
    VERSION_VAR PC_ODBC_VERSION
    REQUIRED_VARS ODBC_LIBRARIES ODBC_INCLUDES
)

mark_as_advanced(ODBC_INCLUDES ODBC_LIBRARIES)
