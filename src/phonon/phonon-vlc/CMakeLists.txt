if(PHONON_EXPERIMENTAL)
  add_definitions(-DPHONON_EXPERIMENTAL)
else()
  add_definitions(-DPHONON_VLC_NO_EXPERIMENTAL)
endif()

include_directories(
    ${VLC_INCLUDES}
    ${VLC_INCLUDES}/vlc/plugins
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/audio
)

set(PHONON_VLC_MAJOR_VERSION "0")
set(PHONON_VLC_MINOR_VERSION "8")
set(PHONON_VLC_PATCH_VERSION "50")
set(PHONON_VLC_VERSION "${PHONON_VLC_MAJOR_VERSION}.${PHONON_VLC_MINOR_VERSION}.${PHONON_VLC_PATCH_VERSION}")
add_definitions(-DPHONON_VLC_VERSION="${PHONON_VLC_VERSION}")

# PVLC only uses static mimetype lists, they are created by cmake, hooray.
include(MimeTypes.cmake)

add_definitions(-DPHONON_LIB_INSTALL_DIR="${LIB_INSTALL_DIR}")

if (MSVC OR (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"))
    include(CheckIncludeFileCXX)
    check_include_file_cxx(inttypes.h HAVE_INTTYPES)
    check_include_file_cxx(stdint.h   HAVE_STDINT)
    if ( NOT HAVE_STDINT AND NOT HAVE_INTTYPES )
        message (FATAL_ERROR
            "You don't have stdint.h and inttypes.h get them from:"
            "http://code.google.com/p/baseutils/source/browse/#svn/trunk/msvc"
        )
    endif ( NOT HAVE_STDINT AND NOT HAVE_INTTYPES )
endif (MSVC OR (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel"))

# bundled phonon is version 4.8.51
add_definitions(-DPHONON_BACKEND_VERSION_4_9)

set(phonon_vlc_SRCS
    audio/audiooutput.cpp
    audio/audiodataoutput.cpp
    audio/volumefadereffect.cpp
    backend.cpp
    devicemanager.cpp
    effect.cpp
    effectmanager.cpp
    media.cpp
    mediacontroller.cpp
    mediaobject.cpp
    mediaplayer.cpp
    sinknode.cpp
    streamreader.cpp
    # video/videodataoutput.cpp
    video/videowidget.cpp
    video/videomemorystream.cpp
    utils/debug.cpp
    utils/libvlc.cpp
)

if(${VLC_VERSION} VERSION_GREATER "2.2.0" OR ${VLC_VERSION} VERSION_EQUAL "2.2.0")
    list(APPEND phonon_vlc_SRCS equalizereffect.cpp)
endif()

if(PHONON_GRAPHICS)
    list(APPEND phonon_vlc_SRCS video/videographicsobject.cpp)
endif()

if(PHONON_EXPERIMENTAL)
    add_definitions(-DPHONON_EXPERIMENTAL)
    list(APPEND phonon_vlc_SRCS video/videodataoutput.cpp)
endif()

if(APPLE)
    list(APPEND phonon_vlc_SRCS
        video/mac/nsvideoview.mm
        video/mac/vlcmacwidget.mm)
endif(APPLE)

set(phonon_vlc_HDRS
    effectmanager.h
    backend.h
    mediaobject.h
    audio/audiooutput.h
    audio/volumefadereffect.h
    audio/audiodataoutput.h
    equalizereffect.h
    devicemanager.h
    video/videodataoutput.h
    video/videowidget.h
    video/videographicsobject.h
    streamreader.h
    effect.h
    media.h
    mediaplayer.h
)

katie_resources(${phonon_vlc_SRCS} ${phonon_vlc_HDRS})

add_library(phonon_vlc MODULE ${phonon_vlc_SRCS} ${phonon_vlc_HDRS})

set_target_properties(phonon_vlc PROPERTIES
    PREFIX ""
)

target_link_libraries(phonon_vlc
    KtPhonon
    ${VLC_LIBRARIES}
)

install(
    TARGETS phonon_vlc
    DESTINATION ${BACKEND_INSTALL_DIR}
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/mime.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/utils/mime.h
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/vlc.desktop.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/vlc.desktop
    @ONLY
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/vlc.desktop
    DESTINATION ${KDE4_SERVICES_INSTALL_DIR}
)
