# - Try to find Fontconfig
# Once done this will define
#
#  FONTCONFIG_FOUND - system has Fontconfig
#  FONTCONFIG_INCLUDES - the Fontconfig include directory
#  FONTCONFIG_LIBRARIES - the libraries needed to use Fontconfig
#
# Copyright (C) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPkgConfig)
include(FindPackageHandleStandardArgs)

pkg_check_modules(PC_FONTCONFIG QUIET fontconfig)

find_path(FONTCONFIG_INCLUDES
    NAMES fontconfig/fontconfig.h
    HINTS $ENV{FONTCONFIGDIR}/include ${PC_FONTCONFIG_INCLUDEDIR}
)

find_library(FONTCONFIG_LIBRARIES
    NAMES fontconfig
    HINTS $ENV{FONTCONFIGDIR}/lib ${PC_FONTCONFIG_LIBDIR}
)

find_package_handle_standard_args(Fontconfig
    VERSION_VAR PC_FONTCONFIG_VERSION
    REQUIRED_VARS FONTCONFIG_LIBRARIES FONTCONFIG_INCLUDES
)

mark_as_advanced(FONTCONFIG_INCLUDES FONTCONFIG_LIBRARIES)
