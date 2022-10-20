# Try to find xxHash, once done this will define:
#
#  XXHASH_FOUND - system has xxHash
#  XXHASH_INCLUDES - the xxHash include directory
#  XXHASH_LIBRARIES - the libraries needed to use xxHash
#
# Copyright (C) 2022 Ivailo Monev <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

find_package(PkgConfig REQUIRED)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_XXHASH QUIET libxxhash)

find_path(XXHASH_INCLUDES
    NAMES xxhash.h
    HINTS $ENV{XXHASHDIR}/include ${PC_XXHASH_INCLUDEDIR}
)

find_library(XXHASH_LIBRARIES
    NAMES xxhash
    HINTS $ENV{XXHASHDIR}/lib ${PC_XXHASH_LIBDIR}
)

find_package_handle_standard_args(xxHash
    VERSION_VAR PC_XXHASH_VERSION
    REQUIRED_VARS XXHASH_LIBRARIES XXHASH_INCLUDES
)

mark_as_advanced(XXHASH_INCLUDES XXHASH_LIBRARIES)
