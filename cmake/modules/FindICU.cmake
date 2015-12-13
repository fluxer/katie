# - Try to find International Components for Unicode
# Once done this will define
#
#  ICU_FOUND - system has ICU
#  ICU_INCLUDES - the ICU include directory
#  ICU_LIBRARIES - The libraries needed to use ICU
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(ICU_INCLUDES AND ICU_LIBRARIES)
    set(ICU_FIND_QUIETLY TRUE)
endif()

find_path(ICU_INCLUDES
    NAMES
    unistr.h
    PATH_SUFFIXES unicode
    HINTS
    $ENV{ICUDIR}/include
    /usr/include
    /usr/local/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(ICU_LIBRARIES
    icui18n
    HINTS
    $ENV{ICUDIR}/lib
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ICU DEFAULT_MSG ICU_INCLUDES ICU_LIBRARIES)

mark_as_advanced(ICU_INCLUDES ICU_LIBRARIES)
