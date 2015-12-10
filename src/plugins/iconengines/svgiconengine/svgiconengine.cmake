set(QSVGICONPLUGIN_HEADERS
    ${QSVGICONPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines/svgiconengine/qsvgiconengine.h
)

set(QSVGICONPLUGIN_SOURCES
    ${QSVGICONPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines/svgiconengine/svgiconplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines/svgiconengine/qsvgiconengine.cpp
)

katie_resources("${QSVGICONPLUGIN_HEADERS}")
katie_resources("${QSVGICONPLUGIN_SOURCES}")

add_library(qsvgiconplugin ${KATIE_TYPE} ${QSVGICONPLUGIN_SOURCES} ${QSVGICONPLUGIN_HEADERS})
target_link_libraries(qsvgiconplugin KtCore KtGui KtSvg)
set_target_properties(qsvgiconplugin PROPERTIES OUTPUT_NAME qsvgicon)

install(
    TARGETS qsvgiconplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/iconengines
)
