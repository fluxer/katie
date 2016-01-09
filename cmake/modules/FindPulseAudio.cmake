# - Try to find the PulseAudio
# Once done this will define
#
#  PULSEAUDIO_FOUND - system has PulseAudio
#  PULSEAUDIO_INCLUDES - the PulseAudio include directory
#  PULSEAUDIO_LIBRARIES - The libraries needed to use PulseAudio
#
# Copyright (c) 2015, Ivailo Monev, <xakepa10@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.

if(PULSEAUDIO_INCLUDES AND PULSEAUDIO_LIBRARIES)
    set(PULSEAUDIO_FIND_QUIETLY TRUE)
endif()

if(NOT WIN32)
    include(FindPkgConfig)
    pkg_check_modules(PC_PULSEAUDIO QUIET libpulse)
    pkg_check_modules(PC_PULSEAUDIO_MAINLOOP QUIET libpulse-mainloop-glib)
endif()

find_path(PULSEAUDIO_INCLUDES
    NAMES
    pulse/pulseaudio.h
    HINTS
    $ENV{PULSEAUDIODIR}/include
    ${PC_PULSEAUDIO_INCLUDEDIR}
    /usr/include
    /usr/local/include
    ${INCLUDE_INSTALL_DIR}
)

find_library(PULSEAUDIO_LIBRARIES
    mng
    HINTS
    $ENV{PULSEAUDIODIR}/lib
    ${PC_PULSEAUDIO_LIBDIR}
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

find_library(PULSEAUDIO_MAINLOOP_LIBRARIES
    mng
    HINTS
    $ENV{PULSEAUDIODIR}/lib
    ${PC_PULSEAUDIO_MAINLOOP_LIBDIR}
    /usr/lib
    /usr/local/lib
    ${LIB_INSTALL_DIR}
)

if(PULSEAUDIO_LIBRARIES AND PULSEAUDIO_MAINLOOP_LIBRARIES)
    set(PULSEAUDIO_LIBRARIES ${PULSEAUDIO_LIBRARIES} ${PULSEAUDIO_MAINLOOP_LIBRARIES})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PulseAudio DEFAULT_MSG PULSEAUDIO_INCLUDES PULSEAUDIO_LIBRARIES PULSEAUDIO_MAINLOOP_LIBRARIES)

mark_as_advanced(PULSEAUDIO_INCLUDES PULSEAUDIO_LIBRARIES PULSEAUDIO_MAINLOOP_LIBRARIES)
