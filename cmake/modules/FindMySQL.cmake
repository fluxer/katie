# - Try to find MySQL
# Once done this will define
#
#  MYSQL_FOUND - system has MySQL
#  MYSQL_INCLUDES - the MySQL include directory
#  MYSQL_LIBRARIES - the libraries needed to use MySQL
#
# Copyright (c) 2015-2019, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(MYSQL_INCLUDES AND MYSQL_LIBRARIES)
    set(MYSQL_FIND_QUIETLY TRUE)
endif()

# Only MariaDB provides pkg-config files and only in recent versions
if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_MYSQL QUIET libmariadb)
endif()

# However, both provide mysql_config
if(NOT PC_MYSQL_FOUND)
    find_program(MYSQL_CONFIG
        NAMES
        mysql_config
        HINTS
        $ENV{MYSQLDIR}/bin
    )

    if(MYSQL_CONFIG)
        execute_process(
            COMMAND ${MYSQL_CONFIG} --variable=pkgincludedir
            RESULT_VARIABLE procerror1
            OUTPUT_VARIABLE MYSQL_INCLUDES
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND ${MYSQL_CONFIG} --libmysqld-libs
            RESULT_VARIABLE proceerror2
            OUTPUT_VARIABLE MYSQL_LIBRARIES
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        execute_process(
            COMMAND ${MYSQL_CONFIG} --version
            RESULT_VARIABLE proceerror3
            OUTPUT_VARIABLE MYSQL_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        # just in case, MYSQL_VERSION is not critical
        if(NOT procerror1 STREQUAL "0" OR NOT proceerror2 STREQUAL "0")
            set(MYSQL_INCLUDES)
            set(MYSQL_LIBRARIES)
            set(MYSQL_VERSION)
        endif()
    endif()
else()
    set(MYSQL_VERSION "${PC_MYSQL_VERSION}")
endif()

if(NOT MYSQL_INCLUDES OR NOT MYSQL_LIBRARIES)
    find_path(MYSQL_INCLUDES
        NAMES
        mysql.h
        PATH_SUFFIXES mysql
        HINTS
        $ENV{MYSQLDIR}/include
        ${PC_MYSQL_INCLUDEDIR}
        ${INCLUDE_INSTALL_DIR}
    )

    find_library(MYSQL_LIBRARIES
        mysqld
        HINTS
        $ENV{MYSQLDIR}/lib
        ${PC_MYSQL_LIBDIR}
        ${LIB_INSTALL_DIR}
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MySQL
    VERSION_VAR MYSQL_VERSION
    REQUIRED_VARS MYSQL_LIBRARIES MYSQL_INCLUDES
)

mark_as_advanced(MYSQL_INCLUDES MYSQL_LIBRARIES)
