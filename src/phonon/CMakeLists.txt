add_definitions(
    -DQT_BUILD_PHONON_LIB
    -DMAKE_PHONON_LIB
    -DPHONON_LIBRARY_PATH="${QT_PLUGINS_PATH}"
    -DPHONON_BACKEND_DIR_SUFFIX="/phonon_backend/"
    # do not use any platform plugin
    # -DQT_NO_PHONON_PLATFORMPLUGIN
    # -DPHONON_NO_CAPTURE
)
set(EXTRA_PHONON_LIBS KtCore)

# TODO: aliases
set(PHONON_PUBLIC_HEADERS
    ${PHONON_PUBLIC_HEADERS}
    AbstractAudioOutput
    AbstractMediaStream
    AbstractVideoOutput
    AddonInterface
    AudioCaptureDevice
    AudioCaptureDeviceModel
    AudioChannelDescription
    AudioChannelDescriptionModel
    AudioDataOutput
    AudioOutput
    AudioOutputDevice
    AudioOutputDeviceModel
    AudioOutputInterface
    AudioOutputInterface40
    AudioOutputInterface42
    AvCapture
    BackendCapabilities
    BackendInterface
    Effect
    EffectDescription
    EffectDescriptionModel
    EffectInterface
    EffectParameter
    EffectWidget
    Global
    GlobalDescriptionContainer
    MediaController
    MediaNode
    MediaObject
    MediaObjectInterface
    MediaSource
    Mrl
    ObjectDescription
    ObjectDescriptionData
    ObjectDescriptionModel
    ObjectDescriptionModelData
    Path
    PlatformPlugin
    SeekSlider
    StreamInterface
    SubtitleDescription
    SubtitleDescriptionModel
    VideoCaptureDevice
    VideoCaptureDeviceModel
    VideoPlayer
    VideoWidget
    VideoWidgetInterface
    VideoWidgetInterface44
    VideoWidgetInterfaceLatest
    VolumeFaderEffect
    VolumeFaderInterface
    VolumeSlider
)

set(PHONON_LIB_MAJOR_VERSION ${KATIE_MAJOR})
set(PHONON_LIB_MINOR_VERSION ${KATIE_MINOR})
set(PHONON_LIB_PATCH_VERSION ${KATIE_MICRO})
set(PHONON_EXPERIMENTAL TRUE)
set(PHONON_GRAPHICS TRUE)
set(BACKEND_INSTALL_DIR "${QT_PLUGINS_PATH_INST}/phonon_backend/")

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/experimental/factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/experimental/globalconfig.h
    PROPERTIES SKIP_RESOURCE TRUE
)

set(PHONON_HEADERS
    ${PHONON_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractaudiooutput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractmediastream.h
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractvideooutput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/addoninterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audiodataoutput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audiodataoutputinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audiooutput.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audiooutputinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backendcapabilities.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backendinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effect.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effectinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effectparameter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effectwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globalconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globaldescriptioncontainer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mediacontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/medianode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mediaobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mediaobjectinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mediasource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mrl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectdescription.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectdescriptionmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/path.h
    ${CMAKE_CURRENT_SOURCE_DIR}/phonon_export.h
    ${CMAKE_CURRENT_SOURCE_DIR}/phonondefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platformplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/pulsesupport.h
    ${CMAKE_CURRENT_SOURCE_DIR}/seekslider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/streaminterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/videoplayer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/videowidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/videowidgetinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/volumefadereffect.h
    ${CMAKE_CURRENT_SOURCE_DIR}/volumefaderinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/volumeslider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractaudiooutput_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractmediastream_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractvideooutput_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audiodataoutput_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audiooutputadaptor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/audiooutput_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/backendcapabilities_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effectparameter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effect_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/effectwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/factory_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/frontendinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globalconfig_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globalstatic_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/iodevicestream_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/medianodedestructionhandler_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/medianode_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mediaobject_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mediasource_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectdescriptionmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectdescription_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/path_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/phonondefs_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/phononnamespace_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/phononpimpl_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platform_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsettingsgroup_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/seekslider_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/statesvalidator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/streaminterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/swiftslider_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/videowidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/volumefadereffect_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/volumeslider_p.h
)

set(PHONON_SOURCES
    ${PHONON_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractaudiooutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractaudiooutput_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractmediastream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractvideooutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractvideooutput_p.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audiodataoutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audiooutput.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/audiooutputinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backendcapabilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effectparameter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/effectwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/globalconfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/iodevicestream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mediacontroller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mediaobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/medianode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mediasource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mrl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/objectdescription.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/objectdescriptionmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/path.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/phononnamespace.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pulsesupport.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/seekslider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/statesvalidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/streaminterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/swiftslider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/volumefadereffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/volumeslider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/videoplayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/videowidget.cpp
)

if((WITH_OPENGL AND OPENGL_FOUND) AND PHONON_GRAPHICS)
    set(PHONON_HEADERS
        ${PHONON_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/abstractvideographicspainter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/glpainter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/videoframe.h
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/videographicsobjectinterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/videoframe.h
        # VGO does not need to be public, as there is only pull but no push behavior.
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/videographicsobject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/videographicsobjectinterface.h
    )

    set(PHONON_SOURCES
        ${PHONON_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/glpainter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/glslpainter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/qpainterpainter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/graphicsview/videographicsobject.cpp
    )
    set(EXTRA_PHONON_LIBS
        ${EXTRA_PHONON_LIBS}
        KtOpenGL
    )
else()
    set(PHONON_GRAPHICS TRUE)
    katie_definition(-DPHONON_NO_GRAPHICSVIEW)
endif()

if(WITH_GLIB2 AND GLIB2_FOUND AND WITH_PULSEAUDIO AND PULSEAUDIO_FOUND)
    set(PHONON_HEADERS
        ${PHONON_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/pulsestream_p.h
    )
    set(PHONON_SOURCES
        ${PHONON_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/pulsestream.cpp
    )
    set(EXTRA_PHONON_LIBS
        ${EXTRA_PHONON_LIBS}
        ${PULSEAUDIO_LIBRARIES}
        ${GLIB2_LIBRARIES}
    )
    add_definitions(-DHAVE_PULSEAUDIO)
    include_directories(
        ${GLIB2_INCLUDES}
        ${PULSEAUDIO_INCLUDES}
    )
endif()

if(WITH_DBUS AND DBUS_FOUND)
    set(PHONON_SOURCES
        ${PHONON_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/audiooutputadaptor.cpp
    )
    set(EXTRA_PHONON_LIBS
        ${EXTRA_PHONON_LIBS}
        KtDBus
    )

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/org.kde.Phonon.AudioOutput.xml
        ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Phonon.AudioOutput.xml
        @ONLY
    )
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Phonon.AudioOutput.xml
        DESTINATION  ${DBUS_INTERFACES_INSTALL_DIR}
    )
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/phononconfig_p.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/phononconfig_p.h
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/phononnamespace.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/phononnamespace.h
)

set(PHONON_HEADERS
    ${PHONON_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/phononconfig_p.h
    ${CMAKE_CURRENT_BINARY_DIR}/phononnamespace.h
)

if(PHONON_EXPERIMENTAL)
    set(PHONON_EXPERIMENTAL_HEADERS
        ${PHONON_EXPERIMENTAL_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/abstractaudiodataoutput.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/abstractvideodataoutput.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/audiodataoutput.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/audiodataoutputinterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/audioformat.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/avcapture.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/avcaptureinterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/backendcapabilities.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/backendinterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/export.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/mediasource.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/objectdescription.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/snapshotinterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/packet.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/packetpool.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/phononnamespace.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videodataoutput.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videodataoutput2.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videodataoutputinterface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videoframe.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videoframe2.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videowidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/visualization.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/globalconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/factory_p.h
    )
    set(PHONON_SOURCES
        ${PHONON_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/abstractaudiodataoutput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/abstractvideodataoutput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/audiodataoutput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/audioformat.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/avcapture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/backendcapabilities.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/factory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/globalconfig.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/mediasource.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/objectdescription.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/packet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/packetpool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videodataoutput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videodataoutput2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/videowidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/experimental/visualization.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/abstractaudiooutput_p.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/abstractvideooutput_p.cpp
    )
    add_definitions(-DMAKE_PHONONEXPERIMENTAL_LIB)
endif()

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtOpenGL
    ${CMAKE_BINARY_DIR}/privateinclude/QtOpenGL
    ${CMAKE_BINARY_DIR}/include/phonon
    ${CMAKE_BINARY_DIR}/privateinclude/phonon
    ${CMAKE_BINARY_DIR}/include/phonon/experimental
    ${CMAKE_BINARY_DIR}/privateinclude/phonon/experimental
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/experimental
    ${CMAKE_CURRENT_BINARY_DIR}/graphicsview
)

katie_generate_misc("${PHONON_HEADERS}" phonon)
katie_generate_misc("${PHONON_EXPERIMENTAL_HEADERS}" phonon/experimental)
# for Qt Phonon and KDE Phonon compat do it for both
# TODO: migrate the vlc backend from Phonon, Katana is happy with phonon
katie_generate_public("${PHONON_PUBLIC_HEADERS}" Phonon)
katie_generate_public("${PHONON_PUBLIC_HEADERS}" phonon)
if(PHONON_EXPERIMENTAL)
    katie_generate_map(Phonon "PHONON_EXPORT|PHONONEXPERIMENTAL_EXPORT")
    katie_generate_map(phonon "PHONON_EXPORT|PHONONEXPERIMENTAL_EXPORT")
else()
    katie_generate_map(Phonon PHONON_EXPORT)
    katie_generate_map(phonon PHONON_EXPORT)
endif()
katie_resources(${PHONON_SOURCES} ${PHONON_HEADERS})
katie_resources(${PHONON_EXPERIMENTAL_HEADERS})
katie_setup_flags()

add_library(KtPhonon ${KATIE_TYPE} ${PHONON_SOURCES} ${PHONON_HEADERS} ${PHONON_EXPERIMENTAL_HEADERS})
target_link_libraries(KtPhonon ${EXTRA_PHONON_LIBS})
set_target_properties(KtPhonon PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Phonon
)

set(PHONON_REQUIRES KtCore)
if(WITH_OPENGL AND OPENGL_FOUND)
    set(PHONON_REQUIRES "${PHONON_REQUIRES} KtOpenGL")
endif()
if(WITH_DBUS AND DBUS_FOUND)
    set(PHONON_REQUIRES "${PHONON_REQUIRES} KtDBus")
endif()
katie_generate_package(KtPhonon "" "${EXTRA_PHONON_LIBS}" "${PHONON_REQUIRES}")

if(WITH_VLC AND LIBVLC_FOUND)
    add_subdirectory(phonon-vlc)
endif()

install(
    TARGETS KtPhonon
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH_INST}
)

install(
    DIRECTORY
    ${CMAKE_BINARY_DIR}/include/phonon
    ${CMAKE_BINARY_DIR}/include/Phonon
    DESTINATION ${QT_HEADERS_PATH_INST}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/phonon)
