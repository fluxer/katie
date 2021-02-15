# - Try to find PostgreSQL database engine
# Once done this will define
#
#  POSTGRESQL_FOUND - system has PostgreSQL database engine
#  POSTGRESQL_INCLUDES - the PostgreSQL database engine include directory
#  POSTGRESQL_LIBRARIES - the libraries needed to use PostgreSQL database engine
#
# Copyright (C) 2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_POSTGRESQL QUIET libpq)

find_path(POSTGRESQL_INCLUDES
    NAMES libpq-fe.h
    PATH_SUFFIXES postgresql
    HINTS $ENV{POSTGRESQLDIR}/include ${PC_POSTGRESQL_INCLUDEDIR}
)

find_library(POSTGRESQL_LIBRARIES
    NAMES pq
    HINTS $ENV{POSTGRESQLDIR}/lib ${PC_POSTGRESQL_LIBDIR}
)

find_package_handle_standard_args(PostgreSQL
    VERSION_VAR PC_POSTGRESQL_VERSION
    REQUIRED_VARS POSTGRESQL_LIBRARIES POSTGRESQL_INCLUDES
)

mark_as_advanced(POSTGRESQL_INCLUDES POSTGRESQL_LIBRARIES)
