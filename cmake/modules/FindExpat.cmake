# - Try to find Expat
# Once done this will define
#
#  EXPAT_FOUND - system has Expat
#  EXPAT_INCLUDES - the Expat include directory
#  EXPAT_LIBRARIES - the libraries needed to use Expat
#
# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_EXPAT QUIET expat)

find_path(EXPAT_INCLUDES
    NAMES expat.h
    HINTS $ENV{EXPATDIR}/include ${PC_EXPAT_INCLUDEDIR}
)

find_library(EXPAT_LIBRARIES
    NAMES expat
    HINTS $ENV{EXPATDIR}/lib ${PC_EXPAT_LIBDIR}
)

find_package_handle_standard_args(Expat
    VERSION_VAR PC_EXPAT_VERSION
    REQUIRED_VARS EXPAT_LIBRARIES EXPAT_INCLUDES
)

mark_as_advanced(EXPAT_INCLUDES EXPAT_LIBRARIES)
