# - Try to find ODBC
# Once done this will define
#
#  ODBC_FOUND - system has ODBC
#  ODBC_INCLUDES - the ODBC include directory
#  ODBC_LIBRARIES - The libraries needed to use ODBC
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(ODBC_INCLUDES AND ODBC_LIBRARIES)
    set(ODBC_FIND_QUIETLY TRUE)
endif()

# ODBC does not provide pkg-config files

find_path(ODBC_INCLUDES
    NAMES
    sql.h
    PATH_SUFFIXES iodbc libiodbc
    HINTS
    $ENV{ODBCDIR}/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(ODBC_LIBRARIES
    odbc iodbc
    HINTS
    $ENV{ODBCDIR}/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ODBC DEFAULT_MSG ODBC_INCLUDES ODBC_LIBRARIES)

mark_as_advanced(ODBC_INCLUDES ODBC_LIBRARIES)
