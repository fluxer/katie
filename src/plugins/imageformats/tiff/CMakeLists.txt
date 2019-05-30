set(QTIFFPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qtiffplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qtiffhandler.cpp
)

katie_setup_target(qtiffplugin ${QTIFFPLUGIN_SOURCES})

add_library(qtiffplugin MODULE ${qtiffplugin_SOURCES})
target_link_libraries(qtiffplugin KtCore KtGui)
set_target_properties(qtiffplugin PROPERTIES OUTPUT_NAME qtiff)

install(
    TARGETS qtiffplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
    COMPONENT Runtime
)
