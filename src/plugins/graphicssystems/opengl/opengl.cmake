set(QGLGRAPHICSSYSTEMPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/opengl/openglplugin.cpp
)

include_directories(${OPENGL_INCLUDE_DIR})

katie_setup_target(qglgraphicssystemplugin ${QGLGRAPHICSSYSTEMPLUGIN_SOURCES})

add_library(qglgraphicssystemplugin MODULE ${qglgraphicssystemplugin_SOURCES})
target_link_libraries(qglgraphicssystemplugin KtCore KtGui KtOpenGL)
set_target_properties(qglgraphicssystemplugin PROPERTIES OUTPUT_NAME qglgraphicssystem)

install(
    TARGETS qglgraphicssystemplugin
    DESTINATION ${QT_PLUGINS_PATH}/graphicssystems
)
