# add_definitions(-DBEARER_MANAGEMENT_DEBUG)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/nativewifi
)

set(QNATIVEWIFIPLUGIN_HEADERS
    ${QNATIVEWIFIPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/platformdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QNATIVEWIFIPLUGIN_SOURCES
    ${QNATIVEWIFIPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

add_library(qnativewifibearerplugin MODULE ${QNATIVEWIFIPLUGIN_SOURCES} ${QNATIVEWIFIPLUGIN_HEADERS})
target_link_libraries(qnativewifibearerplugin KtCore KtNetwork)
set_target_properties(qnativewifibearerplugin PROPERTIES OUTPUT_NAME qnativewifibearer)

katie_setup_target(qnativewifibearerplugin)

install(
    TARGETS qnativewifibearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
