set(QGLGRAPHICSSYSTEMPLUGIN_SOURCES
    ${QGLGRAPHICSSYSTEMPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/opengl/openglplugin.cpp
)

include_directories(${OPENGL_INCLUDE_DIR})

add_library(qglgraphicssystemplugin MODULE ${QGLGRAPHICSSYSTEMPLUGIN_SOURCES})
target_link_libraries(qglgraphicssystemplugin KtCore KtGui KtOpenGL)
set_target_properties(qglgraphicssystemplugin PROPERTIES OUTPUT_NAME qglgraphicssystem)

katie_setup_target(qglgraphicssystemplugin)

install(
    TARGETS qglgraphicssystemplugin
    DESTINATION ${QT_PLUGINS_PATH}/graphicssystems
)
