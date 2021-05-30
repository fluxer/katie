include_directories(${JPEG_INCLUDE_DIR})

set(QJPEGPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qjpeghandler_p.h
)

set(QJPEGPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qjpegplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qjpeghandler.cpp
)

katie_setup_target(qjpegplugin ${QJPEGPLUGIN_SOURCES} ${QJPEGPLUGIN_HEADERS})

add_library(qjpegplugin MODULE ${qjpegplugin_SOURCES})
target_link_libraries(qjpegplugin KtCore KtGui ${JPEG_LIBRARIES})
set_target_properties(qjpegplugin PROPERTIES OUTPUT_NAME qjpeg)

katie_setup_plugin(qjpegplugin)

install(
    TARGETS qjpegplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/imageformats
    COMPONENT Runtime
)
