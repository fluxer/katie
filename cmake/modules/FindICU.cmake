# - Try to find International Components for Unicode
# Once done this will define
#
#  ICU_FOUND - system has ICU
#  ICU_INCLUDES - the ICU include directory
#  ICU_LIBRARIES - the libraries needed to use ICU
#
# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_ICU QUIET icu-i18n)
pkg_check_modules(PC_ICUUC QUIET icu-uc)

find_path(ICU_INCLUDES
    NAMES
    unicode/unistr.h
    HINTS $ENV{ICUDIR}/include ${PC_ICU_INCLUDEDIR}
)

find_library(ICU_LIBRARIES
    NAME icui18n
    HINTS $ENV{ICUDIR}/lib ${PC_ICU_LIBDIR}
)

find_library(ICUUC_LIBRARIES
    NAMES icuuc
    HINTS $ENV{ICUDIR}/lib ${PC_ICUUC_LIBDIR}
)

if(ICU_LIBRARIES AND ICUUC_LIBRARIES)
    set(ICU_LIBRARIES ${ICU_LIBRARIES} ${ICUUC_LIBRARIES})
endif()

find_package_handle_standard_args(ICU
    VERSION_VAR PC_ICU_VERSION
    REQUIRED_VARS ICU_LIBRARIES ICUUC_LIBRARIES ICU_INCLUDES
)

mark_as_advanced(ICU_INCLUDES ICU_LIBRARIES ICUUC_LIBRARIES)
