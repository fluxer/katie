# - Try to find xdg-utils
# Once done this will define
#
#  XDGUTILS_FOUND - system has xdg-utils
#  XDGUTILS_OPEN - the xdg-open binary
#
# Copyright (C) 2021, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

include(FindPackageHandleStandardArgs)

find_program(XDGUTILS_OPEN
    NAMES xdg-open
    HINTS $ENV{XDGUTILSDIR}/bin
)

find_package_handle_standard_args(XdgUtils
    REQUIRED_VARS XDGUTILS_OPEN
)

mark_as_advanced(XDGUTILS_OPEN)
