# - Try to find International Components for Unicode
# Once done this will define
#
#  ICU_FOUND - system has ICU
#  ICU_INCLUDES - the ICU include directory
#  ICU_LIBRARIES - The libraries needed to use ICU
#
# Copyright (c) 2015-2019, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(ICU_INCLUDES AND ICU_LIBRARIES)
    set(ICU_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_ICU QUIET icu-i18n)
endif()

find_path(ICU_INCLUDES
    NAMES
    unicode/unistr.h
    HINTS
    $ENV{ICUDIR}/include
    ${PC_ICU_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(ICU_LIBRARIES
    icui18n
    HINTS
    $ENV{ICUDIR}/lib
    ${PC_ICU_LIBDIR}
    ${LIB_INSTALL_DIR}
)

find_library(ICUUC_LIBRARIES
    icuuc
    HINTS
    $ENV{ICUDIR}/lib
    ${PC_ICU_LIBDIR}
    ${LIB_INSTALL_DIR}
)

if(ICU_LIBRARIES AND ICUUC_LIBRARIES)
    set(ICU_LIBRARIES ${ICU_LIBRARIES} ${ICUUC_LIBRARIES})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ICU
    VERSION_VAR PC_ICU_VERSION
    REQUIRED_VARS ICU_LIBRARIES ICUUC_LIBRARIES ICU_INCLUDES
)

mark_as_advanced(ICU_INCLUDES ICU_LIBRARIES ICUUC_LIBRARIES)
