# - Try to find Fontconfig
# Once done this will define
#
#  FONTCONFIG_FOUND - system has Fontconfig
#  FONTCONFIG_INCLUDES - the Fontconfig include directory
#  FONTCONFIG_LIBRARIES - the libraries needed to use Fontconfig
#
# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_FONTCONFIG QUIET fontconfig)

    set(FONTCONFIG_INCLUDES ${PC_FONTCONFIG_INCLUDE_DIRS})
    set(FONTCONFIG_LIBRARIES ${PC_FONTCONFIG_LIBRARIES})
endif()

if(NOT FONTCONFIG_INCLUDES OR NOT FONTCONFIG_LIBRARIES)
    find_path(FONTCONFIG_INCLUDES
        NAMES fontconfig/fontconfig.h
        HINTS $ENV{FONTCONFIGDIR}/include
    )

    find_library(FONTCONFIG_LIBRARIES
        NAMES fontconfig
        HINTS $ENV{FONTCONFIGDIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Fontconfig
    VERSION_VAR PC_FONTCONFIG_VERSION
    REQUIRED_VARS FONTCONFIG_LIBRARIES FONTCONFIG_INCLUDES
)

mark_as_advanced(FONTCONFIG_INCLUDES FONTCONFIG_LIBRARIES)
