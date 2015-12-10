set(QGLGRAPHICSSYSTEMPLUGIN_SOURCES
    ${QGLGRAPHICSSYSTEMPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/opengl/openglplugin.cpp
)

katie_resources("${QGLGRAPHICSSYSTEMPLUGIN_SOURCES}")

add_library(qglgraphicssystemplugin ${KATIE_TYPE} ${QGLGRAPHICSSYSTEMPLUGIN_SOURCES})
target_link_libraries(qglgraphicssystemplugin KtCore KtGui KtOpenGL)
set_target_properties(qglgraphicssystemplugin PROPERTIES OUTPUT_NAME qglgraphicssystem)

install(
    TARGETS qglgraphicssystemplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/graphicssystems
)
