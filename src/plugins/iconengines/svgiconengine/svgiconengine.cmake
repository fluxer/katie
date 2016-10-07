set(QSVGICONPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines/svgiconengine/qsvgiconengine.h
)

set(QSVGICONPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines/svgiconengine/svgiconplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines/svgiconengine/qsvgiconengine.cpp
)

katie_setup_target(qsvgiconplugin ${QSVGICONPLUGIN_SOURCES} ${QSVGICONPLUGIN_HEADERS})

add_library(qsvgiconplugin MODULE ${qsvgiconplugin_SOURCES})
target_link_libraries(qsvgiconplugin KtCore KtGui KtSvg)
set_target_properties(qsvgiconplugin PROPERTIES OUTPUT_NAME qsvgicon)

install(
    TARGETS qsvgiconplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/iconengines
)
