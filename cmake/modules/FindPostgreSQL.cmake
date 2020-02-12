# - Try to find PostgreSQL database engine
# Once done this will define
#
#  POSTGRESQL_FOUND - system has PostgreSQL database engine
#  POSTGRESQL_INCLUDES - the PostgreSQL database engine include directory
#  POSTGRESQL_LIBRARIES - the libraries needed to use PostgreSQL database engine
#
# Copyright (c) 2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(POSTGRESQL_INCLUDES AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_POSTGRESQL QUIET libpq)

    set(POSTGRESQL_INCLUDES ${PC_POSTGRESQL_INCLUDE_DIRS})
    set(POSTGRESQL_LIBRARIES ${PC_POSTGRESQL_LIBRARIES})
endif()

if(NOT POSTGRESQL_INCLUDES OR NOT POSTGRESQL_LIBRARIES)
    find_path(POSTGRESQL_INCLUDES
        NAMES libpq-fe.h
        PATH_SUFFIXES postgresql
        HINTS $ENV{POSTGRESQLDIR}/include
    )

    find_library(POSTGRESQL_LIBRARIES
        NAMES pq
        HINTS $ENV{POSTGRESQLDIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PostgreSQL
    VERSION_VAR PC_POSTGRESQL_VERSION
    REQUIRED_VARS POSTGRESQL_LIBRARIES POSTGRESQL_INCLUDES
)

mark_as_advanced(POSTGRESQL_INCLUDES POSTGRESQL_LIBRARIES)
