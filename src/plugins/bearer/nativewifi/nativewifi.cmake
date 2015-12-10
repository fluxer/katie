# add_definitions(-DBEARER_MANAGEMENT_DEBUG)

set(NATIVEWIFIPLUGIN_HEADERS
    ${NATIVEWIFIPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/platformdefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(NATIVEWIFIPLUGIN_SOURCES
    ${NATIVEWIFIPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiemain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/nativewifi/qnativewifiengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

katie_resources("${NATIVEWIFIPLUGIN_HEADERS}")
katie_resources("${NATIVEWIFIPLUGIN_SOURCES}")

add_library(qnativewifibearerplugin ${KATIE_TYPE} ${NATIVEWIFIPLUGIN_SOURCES} ${NATIVEWIFIPLUGIN_HEADERS})
set_target_properties(qnativewifibearerplugin PROPERTIES OUTPUT_NAME qnativewifibearer)

install(TARGETS qnativewifibearerplugin DESTINATION ${QT_PLUGINS_PATH_INST}/bearer/)
