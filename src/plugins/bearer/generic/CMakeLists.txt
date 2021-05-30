set(QGENERICBEARERPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qgenericengine.h
)

set(QGENERICBEARERPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qgenericmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qgenericengine.cpp
)

katie_setup_target(qgenericbearerplugin ${QGENERICBEARERPLUGIN_SOURCES} ${QGENERICBEARERPLUGIN_HEADERS})

add_library(qgenericbearerplugin MODULE ${qgenericbearerplugin_SOURCES}
    $<TARGET_OBJECTS:sharedbearer>
)
target_link_libraries(qgenericbearerplugin KtCore KtNetwork)
set_target_properties(qgenericbearerplugin PROPERTIES OUTPUT_NAME qgenericbearer)
target_include_directories(qgenericbearerplugin PRIVATE
    ${CMAKE_SOURCE_DIR}/src/plugins/bearer
    ${CMAKE_BINARY_DIR}/src/plugins/bearer
)

katie_setup_object(qgenericbearerplugin sharedbearer)
katie_setup_plugin(qgenericbearerplugin)

install(
    TARGETS qgenericbearerplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/bearer
    COMPONENT Runtime
)
