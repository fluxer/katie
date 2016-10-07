# add_definitions()
set(EXTRA_SHADERS_LIBS KtOpenGL KtDeclarative)

set(SHADERS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/qmlshadersplugin_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffectitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffectsource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/scenegraph/qsggeometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffectbuffer.cpp
)

set(SHADERS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/qmlshadersplugin_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/glfunctions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffect.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffectitem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffectsource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/scenegraph/qsggeometry.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shadereffectbuffer.h
)

include_directories(${OPENGL_INCLUDE_DIR})

katie_setup_target(qmlshadersplugin ${SHADERS_SOURCES} ${SHADERS_HEADERS})

add_library(qmlshadersplugin ${KATIE_TYPE} ${qmlshadersplugin_SOURCES})
target_link_libraries(qmlshadersplugin ${EXTRA_SHADERS_LIBS})

install(
    TARGETS qmlshadersplugin
    DESTINATION ${KATIE_IMPORTS_RELATIVE}/Qt/labs/shaders
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/qmldir
    DESTINATION ${KATIE_IMPORTS_RELATIVE}/Qt/labs/shaders
)
