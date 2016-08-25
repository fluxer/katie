add_definitions(-DQT_BUILD_MULTIMEDIA_LIB)
set(EXTRA_MULTIMEDIA_LIBS KtCore KtGui)

set(MULTIMEDIA_PUBLIC_HEADERS
    ${MULTIMEDIA_PUBLIC_HEADERS}
    QAbstractAudioDeviceInfo
    QAbstractAudioInput
    QAbstractAudioOutput
    QAbstractVideoBuffer
    QAbstractVideoSurface
    QAudio
    QAudioDeviceInfo
    QAudioEngineFactoryInterface
    QAudioEnginePlugin
    QAudioFormat
    QAudioInput
    QAudioOutput
    QVideoFrame
    QVideoSurfaceFormat
)

include(audio/audio.cmake)
include(video/video.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtMultimedia
    ${CMAKE_BINARY_DIR}/privateinclude/QtMultimedia
    ${CMAKE_CURRENT_SOURCE_DIR}/audio
    ${CMAKE_CURRENT_SOURCE_DIR}/video
    ${CMAKE_CURRENT_BINARY_DIR}/audio
    ${CMAKE_CURRENT_BINARY_DIR}/video
)

katie_generate_misc("${MULTIMEDIA_HEADERS}" QtMultimedia)
katie_generate_public("${MULTIMEDIA_PUBLIC_HEADERS}" QtMultimedia)
katie_setup_paths()
katie_generate_package(KtMultimedia "KtCore KtGui")
katie_setup_target(KtMultimedia ${MULTIMEDIA_SOURCES} ${MULTIMEDIA_HEADERS})

add_library(KtMultimedia ${KATIE_TYPE} ${KtMultimedia_SOURCES})
target_link_libraries(KtMultimedia ${EXTRA_MULTIMEDIA_LIBS})
set_target_properties(KtMultimedia PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Multimedia
)

install(
    TARGETS KtMultimedia
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtMultimedia
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtMultimedia)
