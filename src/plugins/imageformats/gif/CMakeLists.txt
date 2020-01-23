set(QGIFPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qgifhandler_p.h
)

set(QGIFPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qgifplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qgifhandler.cpp
)

katie_setup_target(qgifplugin ${QGIFPLUGIN_SOURCES} ${QGIFPLUGIN_HEADERS})

add_library(qgifplugin MODULE ${qgifplugin_SOURCES})
target_link_libraries(qgifplugin KtCore KtGui)
set_target_properties(qgifplugin PROPERTIES OUTPUT_NAME qgif)

install(
    TARGETS qgifplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
    COMPONENT Runtime
)
