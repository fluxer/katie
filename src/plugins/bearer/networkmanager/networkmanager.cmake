include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/networkmanager
)

set(QNMBEARERPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnmdbushelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerservice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerengine.h
)

set(QNMBEARERPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/nmmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnmdbushelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerservice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerengine.cpp
)

katie_setup_target(qnmbearerplugin ${QNMBEARERPLUGIN_SOURCES} ${QNMBEARERPLUGIN_HEADERS})

add_library(qnmbearerplugin MODULE ${qnmbearerplugin_SOURCES}
    $<TARGET_OBJECTS:sharedbearer>
)
target_link_libraries(qnmbearerplugin KtCore KtNetwork KtDBus)
set_target_properties(qnmbearerplugin PROPERTIES OUTPUT_NAME qnmbearer)

katie_setup_object(qnmbearerplugin sharedbearer)

install(
    TARGETS qnmbearerplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/bearer
)
