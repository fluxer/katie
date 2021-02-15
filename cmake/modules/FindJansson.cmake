# - Try to find Jansson
# Once done this will define
#
#  JANSSON_FOUND - system has Jansson
#  JANSSON_INCLUDES - the Jansson include directory
#  JANSSON_LIBRARIES - the libraries needed to use Jansson
#
# Copyright (C) 2021, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_JANSSON QUIET jansson)

find_path(JANSSON_INCLUDES
    NAMES jansson.h
    HINTS $ENV{JANSSONDIR}/include ${PC_JANSSON_INCLUDEDIR}
)

find_library(JANSSON_LIBRARIES
    NAMES jansson
    HINTS $ENV{JANSSONDIR}/lib ${PC_JANSSON_LIBDIR}
)

find_package_handle_standard_args(Jansson
    VERSION_VAR PC_JANSSON_VERSION
    REQUIRED_VARS JANSSON_LIBRARIES JANSSON_INCLUDES
)

mark_as_advanced(JANSSON_INCLUDES JANSSON_LIBRARIES)
