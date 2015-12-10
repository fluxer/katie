# - Try to find EGL
# Once done this will define
#
#  EGL_FOUND - system has EGL
#  EGL_INCLUDES - the EGL include directory
#  EGL_LIBRARIES - The libraries needed to use EGL
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(EGL_INCLUDES AND EGL_LIBRARIES)
    set(EGL_FIND_QUIETLY TRUE)
endif(EGL_INCLUDES AND EGL_LIBRARIES)

find_path(EGL_INCLUDES
    NAMES
    egl.h
    PATH_SUFFIXES EGL
    HINTS
    $ENV{EGLDIR}/include
    /usr/include
    /usr/local/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(EGL_LIBRARIES
    EGL
    HINTS
    $ENV{EGLDIR}/lib
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EGL DEFAULT_MSG EGL_INCLUDES EGL_LIBRARIES)

mark_as_advanced(EGL_INCLUDES EGL_LIBRARIES)
