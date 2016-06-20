set(QTIFFPLUGIN_SOURCES
    ${QTIFFPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tiff/qtiffplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qtiffhandler.cpp
)

add_library(qtiffplugin MODULE ${QTIFFPLUGIN_SOURCES})
target_link_libraries(qtiffplugin KtCore KtGui)
set_target_properties(qtiffplugin PROPERTIES OUTPUT_NAME qtiff)

install(
    TARGETS qtiffplugin
    DESTINATION ${QT_PLUGINS_PATH}/imageformats
)
