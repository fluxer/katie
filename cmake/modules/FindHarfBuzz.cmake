# - Try to find HarfBuzz
# Once done this will define
#
#  HARFBUZZ_FOUND - system has HarfBuzz
#  HARFBUZZ_INCLUDES - the HarfBuzz include directory
#  HARFBUZZ_LIBRARIES - the libraries needed to use HarfBuzz
#
# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
pkg_check_modules(PC_HARFBUZZ QUIET harfbuzz)

set(HARFBUZZ_INCLUDES ${PC_HARFBUZZ_INCLUDE_DIRS})
set(HARFBUZZ_LIBRARIES ${PC_HARFBUZZ_LIBRARIES})

if(NOT HARFBUZZ_INCLUDES OR NOT HARFBUZZ_LIBRARIES)
    find_path(HARFBUZZ_INCLUDES
        NAMES hb.h
        PATH_SUFFIXES harfbuzz
        HINTS $ENV{HARFBUZZDIR}/include
    )

    find_library(HARFBUZZ_LIBRARIES
        NAMES harfbuzz
        HINTS $ENV{HARFBUZZDIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HarfBuzz
    VERSION_VAR PC_HARFBUZZ_VERSION
    REQUIRED_VARS HARFBUZZ_LIBRARIES HARFBUZZ_INCLUDES
)

mark_as_advanced(HARFBUZZ_INCLUDES HARFBUZZ_LIBRARIES)
