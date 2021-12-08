# - Try to find Cairo 2D graphics library
# Once done this will define
#
#  CAIRO_FOUND - system has Cairo 2D graphics library
#  CAIRO_INCLUDES - the Cairo 2D graphics library include directory
#  CAIRO_LIBRARIES - the libraries needed to use Cairo 2D graphics library
#
# Copyright (C) 2021, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_CAIRO QUIET cairo)

find_path(CAIRO_INCLUDES
    NAMES cairo/cairo.h
    HINTS $ENV{ZSTDDIR}/include ${PC_CAIRO_INCLUDEDIR}
)

find_library(CAIRO_LIBRARIES
    NAMES cairo
    HINTS $ENV{ZSTDDIR}/lib ${PC_CAIRO_LIBDIR}
)

find_package_handle_standard_args(Cairo
    VERSION_VAR PC_CAIRO_VERSION
    REQUIRED_VARS CAIRO_LIBRARIES CAIRO_INCLUDES
)

mark_as_advanced(CAIRO_INCLUDES CAIRO_LIBRARIES) 
