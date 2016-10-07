set(QGIFPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/gif/qgifplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qgifhandler.cpp
)

katie_setup_target(qgifplugin ${QGIFPLUGIN_SOURCES})

add_library(qgifplugin MODULE ${qgifplugin_SOURCES})
target_link_libraries(qgifplugin KtCore KtGui)
set_target_properties(qgifplugin PROPERTIES OUTPUT_NAME qgif)

install(
    TARGETS qgifplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
)
