# add_definitions(-DBEARER_MANAGEMENT_DEBUG)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/nativewifi
)

set(QNATIVEWIFIPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/platformdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QNATIVEWIFIPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

katie_setup_target(qnativewifibearerplugin ${QNATIVEWIFIPLUGIN_SOURCES} ${QNATIVEWIFIPLUGIN_HEADERS})

add_library(qnativewifibearerplugin MODULE ${qnativewifibearerplugin_SOURCES})
target_link_libraries(qnativewifibearerplugin KtCore KtNetwork)
set_target_properties(qnativewifibearerplugin PROPERTIES OUTPUT_NAME qnativewifibearer)

install(
    TARGETS qnativewifibearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
