# - Try to find HarfBuzz
# Once done this will define
#
#  HARFBUZZ_FOUND - system has HarfBuzz
#  HARFBUZZ_INCLUDES - the HarfBuzz include directory
#  HARFBUZZ_LIBRARIES - The libraries needed to use HarfBuzz
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(HARFBUZZ_INCLUDES AND HARFBUZZ_LIBRARIES)
    set(HARFBUZZ_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_HARFBUZZ QUIET harfbuzz)
endif()

find_path(HARFBUZZ_INCLUDES
    NAMES
    hb.h
    PATH_SUFFIXES harfbuzz
    HINTS
    $ENV{HARFBUZZDIR}/include
    ${PC_HARFBUZZ_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(HARFBUZZ_LIBRARIES
    harfbuzz
    HINTS
    $ENV{HARFBUZZDIR}/lib
    ${PC_HARFBUZZ_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HarfBuzz
    VERSION_VAR PC_HARFBUZZ_VERSION
    REQUIRED_VARS HARFBUZZ_LIBRARIES HARFBUZZ_INCLUDES
)

mark_as_advanced(HARFBUZZ_INCLUDES HARFBUZZ_LIBRARIES)
