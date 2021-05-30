set(QNMBEARERPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qnmdbushelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qnetworkmanagerservice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qnetworkmanagerengine.h
)

set(QNMBEARERPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/nmmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qnmdbushelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qnetworkmanagerservice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qnetworkmanagerengine.cpp
)

katie_setup_target(qnmbearerplugin ${QNMBEARERPLUGIN_SOURCES} ${QNMBEARERPLUGIN_HEADERS})

add_library(qnmbearerplugin MODULE ${qnmbearerplugin_SOURCES}
    $<TARGET_OBJECTS:sharedbearer>
)
target_link_libraries(qnmbearerplugin KtCore KtNetwork KtDBus)
set_target_properties(qnmbearerplugin PROPERTIES OUTPUT_NAME qnmbearer)
target_include_directories(qnmbearerplugin PRIVATE
    ${CMAKE_SOURCE_DIR}/src/plugins/bearer
    ${CMAKE_BINARY_DIR}/src/plugins/bearer
)

katie_setup_object(qnmbearerplugin sharedbearer)
katie_setup_plugin(qnmbearerplugin)

install(
    TARGETS qnmbearerplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/bearer
    COMPONENT Runtime
)
