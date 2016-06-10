set(QMNGPLUGIN_SOURCES
    ${QMNGPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/mng/qmngplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qmnghandler.cpp
)

add_library(qmngplugin ${KATIE_TYPE} ${QMNGPLUGIN_SOURCES})
target_link_libraries(qmngplugin KtCore KtGui)
set_target_properties(qmngplugin PROPERTIES OUTPUT_NAME qmng)

install(
    TARGETS qmngplugin
    DESTINATION ${QT_PLUGINS_PATH}/imageformats
)
