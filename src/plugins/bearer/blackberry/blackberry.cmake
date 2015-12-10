# Uncomment this to enable debugging output for the plugin
# add_definitions(-DQBBENGINE_DEBUG)

set(QBBBEARER_HEADERS
    ${QBBBEARER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/blackberry/qbbengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QBBBEARERPLUGIN_SOURCES
    ${QBBBEARERPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/blackberry/bbmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/blackberry/qbbengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

katie_resources("${QBBBEARER_HEADERS}")
katie_resources("${QBBBEARERPLUGIN_SOURCES}")

add_library(qbbbearerplugin ${KATIE_TYPE} ${QBBBEARERPLUGIN_SOURCES} ${QBBBEARER_HEADERS})
set_target_properties(qbbbearerplugin PROPERTIES OUTPUT_NAME qbbbearer)

install(TARGETS qbbbearerplugin DESTINATION ${QT_PLUGINS_PATH_INST}/bearer/)
