set(QJPEGPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qjpegplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qjpeghandler.cpp
)

katie_setup_target(qjpegplugin ${QJPEGPLUGIN_SOURCES})

add_library(qjpegplugin MODULE ${qjpegplugin_SOURCES})
target_link_libraries(qjpegplugin KtCore KtGui)
set_target_properties(qjpegplugin PROPERTIES OUTPUT_NAME qjpeg)

install(
    TARGETS qjpegplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
    COMPONENT Runtime
)
