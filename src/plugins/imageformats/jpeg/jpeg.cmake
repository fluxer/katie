set(QJPEGPLUGIN_SOURCES
    ${QJPEGPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/jpeg/qjpegplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qjpeghandler.cpp
)

add_library(qjpegplugin MODULE ${QJPEGPLUGIN_SOURCES})
target_link_libraries(qjpegplugin KtCore KtGui)
set_target_properties(qjpegplugin PROPERTIES OUTPUT_NAME qjpeg)

install(
    TARGETS qjpegplugin
    DESTINATION ${QT_PLUGINS_PATH}/imageformats
)
