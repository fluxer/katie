include_directories(${MNG_INCLUDES})

set(QMNGPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qmnghandler_p.h
)

set(QMNGPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qmngplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qmnghandler.cpp
)

katie_setup_target(qmngplugin ${QMNGPLUGIN_SOURCES} ${QMNGPLUGIN_HEADERS})

add_library(qmngplugin MODULE ${qmngplugin_SOURCES})
target_link_libraries(qmngplugin KtCore KtGui ${MNG_LIBRARIES})
set_target_properties(qmngplugin PROPERTIES OUTPUT_NAME qmng)

install(
    TARGETS qmngplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/imageformats
    COMPONENT Runtime
)
