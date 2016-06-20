include_directories(${CMAKE_CURRENT_BINARY_DIR}/designer/qdeclarativeview)

set(QDECLARATIVEVIEWPLUGIN_HEADERS
    ${QDECLARATIVEVIEWPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/qdeclarativeview/qdeclarativeview_plugin.h
)

set(QDECLARATIVEVIEWPLUGIN_SOURCES
    ${QDECLARATIVEVIEWPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/designer/qdeclarativeview/qdeclarativeview_plugin.cpp
)

katie_resources(${QDECLARATIVEVIEWPLUGIN_HEADERS} ${QDECLARATIVEVIEWPLUGIN_SOURCES})

add_library(qdeclarativeviewplugin MODULE ${QDECLARATIVEVIEWPLUGIN_SOURCES} ${QDECLARATIVEVIEWPLUGIN_HEADERS})
target_link_libraries(qdeclarativeviewplugin KtCore KtDeclarative KtDesigner)
set_target_properties(qdeclarativeviewplugin PROPERTIES OUTPUT_NAME qdeclarativeview)
target_compile_definitions(qdeclarativeviewplugin PRIVATE -DQDESIGNER_SDK_LIBRARY)

install(
    TARGETS qdeclarativeviewplugin
    DESTINATION ${QT_PLUGINS_PATH}/designer
)
