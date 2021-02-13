# - Try to find MySQL
# Once done this will define
#
#  MYSQL_FOUND - system has MySQL
#  MYSQL_INCLUDES - the MySQL include directory
#  MYSQL_LIBRARIES - the libraries needed to use MySQL
#
# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

# Only MariaDB provides pkg-config files and only in recent versions
pkg_check_modules(PC_MYSQL QUIET libmariadb)

# However, both provide config program
find_program(MYSQL_CONFIG
    NAMES mariadb_config mysql_config
    HINTS $ENV{MYSQLDIR}/bin
)

if(MYSQL_CONFIG)
    execute_process(
        COMMAND ${MYSQL_CONFIG} --version
        OUTPUT_VARIABLE MYSQL_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

find_path(MYSQL_INCLUDES
    NAMES mysql.h
    PATH_SUFFIXES mariadb mysql
    HINTS $ENV{MYSQLDIR}/include ${PC_MYSQL_INCLUDEDIR}
)

find_library(MYSQL_LIBRARIES
    NAMES mariadbd mysqld
    HINTS $ENV{MYSQLDIR}/lib ${PC_MYSQL_LIBDIR}
)

find_package_handle_standard_args(MySQL
    VERSION_VAR MYSQL_VERSION
    REQUIRED_VARS MYSQL_LIBRARIES MYSQL_INCLUDES
)

mark_as_advanced(MYSQL_INCLUDES MYSQL_LIBRARIES)
