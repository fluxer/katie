# - Try to find Unwind
# Once done this will define
#
#  UNWIND_FOUND - system has Unwind
#  UNWIND_INCLUDES - the Unwind include directory
#  UNWIND_LIBRARIES - the libraries needed to use Unwind
#
# Copyright (c) 2015-2020, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(UNWIND_INCLUDES AND UNWIND_LIBRARIES)
    set(UNWIND_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_UNWIND QUIET libunwind)
endif()

find_path(UNWIND_INCLUDES
    NAMES
    libunwind.h
    HINTS
    $ENV{UNWINDDIR}/include
    ${PC_UNWIND_INCLUDEDIR}
    ${INCLUDE_INSTALL_DIR}
)

find_library(UNWIND_LIBRARIES
    unwind
    HINTS
    $ENV{UNWINDDIR}/lib
    ${PC_UNWIND_LIBDIR}
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Unwind
    VERSION_VAR PC_UNWIND_VERSION
    REQUIRED_VARS UNWIND_LIBRARIES UNWIND_INCLUDES
)

mark_as_advanced(UNWIND_INCLUDES UNWIND_LIBRARIES)
