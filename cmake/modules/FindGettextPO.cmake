# Try to find gettextpo, once done this will define:
#
#  GETTEXTPO_FOUND - system has gettextpo
#  GETTEXTPO_INCLUDES - the gettextpo include directory
#  GETTEXTPO_LIBRARIES - the libraries needed to use gettextpo
#
# Copyright (C) 2022 Ivailo Monev <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

find_package(PkgConfig REQUIRED)
include(FindPackageHandleStandardArgs)

# gettextpo does not provide pkg-config files

find_path(GETTEXTPO_INCLUDES
    NAMES gettext-po.h
    HINTS $ENV{GETTEXTPODIR}/include
)

find_library(GETTEXTPO_LIBRARIES
    NAMES gettextpo
    HINTS $ENV{GETTEXTPODIR}/lib
)

find_package_handle_standard_args(GettextPO
    VERSION_VAR PC_GETTEXTPO_VERSION
    REQUIRED_VARS GETTEXTPO_LIBRARIES GETTEXTPO_INCLUDES
)

mark_as_advanced(GETTEXTPO_INCLUDES GETTEXTPO_LIBRARIES)
