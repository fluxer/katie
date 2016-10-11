include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/generic
    ${CMAKE_CURRENT_BINARY_DIR}/bearer
)

set(QGENERICBEARERPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/generic/qgenericengine.h
)

set(QGENERICBEARERPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/generic/qgenericmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/generic/qgenericengine.cpp
)

katie_setup_target(qgenericbearerplugin ${QGENERICBEARERPLUGIN_SOURCES} ${QGENERICBEARERPLUGIN_HEADERS})

add_library(qgenericbearerplugin MODULE ${qgenericbearerplugin_SOURCES}
    $<TARGET_OBJECTS:sharedbearer>
)
target_link_libraries(qgenericbearerplugin KtCore KtNetwork)
set_target_properties(qgenericbearerplugin PROPERTIES OUTPUT_NAME qgenericbearer)

katie_setup_object(qgenericbearerplugin sharedbearer)

install(
    TARGETS qgenericbearerplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/bearer
)
