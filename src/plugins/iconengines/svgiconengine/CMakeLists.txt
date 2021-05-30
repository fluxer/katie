set(QSVGICONPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgiconengine.h
)

set(QSVGICONPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/svgiconplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgiconengine.cpp
)

katie_setup_target(qsvgiconplugin ${QSVGICONPLUGIN_SOURCES} ${QSVGICONPLUGIN_HEADERS})

add_library(qsvgiconplugin MODULE ${qsvgiconplugin_SOURCES})
target_link_libraries(qsvgiconplugin KtCore KtGui KtSvg)
set_target_properties(qsvgiconplugin PROPERTIES OUTPUT_NAME qsvgicon)

katie_setup_plugin(qsvgiconplugin)

install(
    TARGETS qsvgiconplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/iconengines
    COMPONENT Runtime
)
