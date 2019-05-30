set(QMNGPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qmngplugin.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/image/qmnghandler.cpp
)

katie_setup_target(qmngplugin ${QMNGPLUGIN_SOURCES})

add_library(qmngplugin MODULE ${qmngplugin_SOURCES})
target_link_libraries(qmngplugin KtCore KtGui)
set_target_properties(qmngplugin PROPERTIES OUTPUT_NAME qmng)

install(
    TARGETS qmngplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
    COMPONENT Runtime
)
