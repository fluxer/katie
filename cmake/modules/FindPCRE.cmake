# - Try to find the PCRE
# Once done this will define
#
#  PCRE_FOUND - system has PCRE
#  PCRE_INCLUDES - the PCRE include directory
#  PCRE_LIBRARIES - the libraries needed to use PCRE
#
# Copyright (C) 2016, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_PCRE QUIET libpcre)

find_path(PCRE_INCLUDES
    NAMES pcre.h
    HINTS $ENV{PCREDIR}/include ${PC_PCRE_INCLUDEDIR}
)

find_library(PCRE_LIBRARIES
    NAMES pcre
    HINTS $ENV{PCREDIR}/lib ${PC_PCRE_LIBDIR}
)

find_package_handle_standard_args(PCRE
    VERSION_VAR PC_PCRE_VERSION
    REQUIRED_VARS PCRE_LIBRARIES PCRE_INCLUDES
)

mark_as_advanced(PCRE_INCLUDES PCRE_LIBRARIES)
