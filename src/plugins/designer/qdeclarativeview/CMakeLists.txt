set(QDECLARATIVEVIEWPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativeview_plugin.h
)

set(QDECLARATIVEVIEWPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativeview_plugin.cpp
)

katie_setup_target(qdeclarativeviewplugin ${QDECLARATIVEVIEWPLUGIN_SOURCES} ${QDECLARATIVEVIEWPLUGIN_HEADERS})

add_library(qdeclarativeviewplugin MODULE ${qdeclarativeviewplugin_SOURCES})
target_link_libraries(qdeclarativeviewplugin KtCore KtDeclarative KtDesigner)
set_target_properties(qdeclarativeviewplugin PROPERTIES OUTPUT_NAME qdeclarativeview)
target_compile_definitions(qdeclarativeviewplugin PRIVATE -DQDESIGNER_SDK_LIBRARY)

install(
    TARGETS qdeclarativeviewplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/designer
    COMPONENT Runtime
)
