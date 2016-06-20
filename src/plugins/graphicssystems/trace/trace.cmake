set(QTRACEGRAPHICSSYSTEMPLUGIN_SOURCES
    ${QTRACEGRAPHICSSYSTEMPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/trace/traceplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/trace/qgraphicssystem_trace.cpp
)

katie_resources(${QTRACEGRAPHICSSYSTEMPLUGIN_SOURCES})

add_library(qtracegraphicssystemplugin MODULE ${QTRACEGRAPHICSSYSTEMPLUGIN_SOURCES})
target_link_libraries(qtracegraphicssystemplugin KtCore KtGui)
set_target_properties(qtracegraphicssystemplugin PROPERTIES OUTPUT_NAME qtracegraphicssystem)

install(
    TARGETS qtracegraphicssystemplugin
    DESTINATION ${QT_PLUGINS_PATH}/graphicssystems
)
