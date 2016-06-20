set(QGIFPLUGIN_SOURCES
    ${QGIFPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/gif/qgifplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qgifhandler.cpp
)

add_library(qgifplugin MODULE ${QGIFPLUGIN_SOURCES})
target_link_libraries(qgifplugin KtCore KtGui)
set_target_properties(qgifplugin PROPERTIES OUTPUT_NAME qgif)

install(
    TARGETS qgifplugin
    DESTINATION ${QT_PLUGINS_PATH}/imageformats
)
