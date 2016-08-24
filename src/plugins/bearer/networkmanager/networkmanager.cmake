include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/networkmanager
)

set(QNMBEARERPLUGIN_HEADERS
    ${QNMBEARERPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnmdbushelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerservice.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QNMBEARERPLUGIN_SOURCES
    ${QNMBEARERPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/nmmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnmdbushelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerservice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/networkmanager/qnetworkmanagerengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

add_library(qnmbearerplugin MODULE ${QNMBEARERPLUGIN_SOURCES} ${QNMBEARERPLUGIN_HEADERS})
target_link_libraries(qnmbearerplugin KtCore KtNetwork KtDBus)
set_target_properties(qnmbearerplugin PROPERTIES OUTPUT_NAME qnmbearer)

katie_setup_target(qnmbearerplugin)

install(
    TARGETS qnmbearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
