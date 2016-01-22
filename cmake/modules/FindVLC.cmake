# - Try to find the VLC
# Once done this will define
#
#  VLC_FOUND - system has VLC
#  VLC_INCLUDES - the VLC include directory
#  VLC_LIBRARIES - The libraries needed to use VLC
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(VLC_INCLUDES AND VLC_LIBRARIES)
    set(VLC_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_VLC QUIET libvlc)
    
    set(VLC_VERSION ${PC_VLC_VERSION})
endif()

find_path(VLC_INCLUDES
    NAMES
    vlc/vlc.h
    HINTS
    $ENV{VLCDIR}/include
    ${PC_VLC_INCLUDEDIR}
    /usr/include
    /usr/local/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(VLC_LIBRARIES
    vlc
    HINTS
    $ENV{VLCDIR}/lib
    ${PC_VLC_LIBDIR}
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

find_library(VLC_CORE_LIBRARIES
    vlccore
    HINTS
    $ENV{VLCDIR}/lib
    ${PC_VLC_LIBDIR}
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

if(VLC_LIBRARIES AND VLC_CORE_LIBRARIES)
    set(VLC_LIBRARIES ${VLC_LIBRARIES} ${VLC_CORE_LIBRARIES})
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VLC DEFAULT_MSG VLC_INCLUDES VLC_LIBRARIES VLC_CORE_LIBRARIES)

mark_as_advanced(VLC_INCLUDES VLC_LIBRARIES)
