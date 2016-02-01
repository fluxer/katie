# add_definitions(-DBEARER_MANAGEMENT_DEBUG)

set(ICDPLUGIN_HEADERS
    ${ICDPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/qicdengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/dbusdispatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/iapconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/iapmonitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/maemo_icd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/proxyconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/wlan-utils.h
)

set(ICDPLUGIN_SOURCES
    ${ICDPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/icdmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/qicdengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/qnetworksession_impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/dbusdispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/iapmonitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/iapconf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/maemo_icd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icde/proxyconf.cpp
)

katie_resources(${ICDPLUGIN_HEADERS} ${ICDPLUGIN_SOURCES})

add_library(qicdbearerplugin ${KATIE_TYPE} ${ICDPLUGIN_SOURCES} ${ICDPLUGIN_HEADERS})
set_target_properties(qicdbearerplugin PROPERTIES OUTPUT_NAME qicdbearer)
target_include_directories(qicdbearerplugin PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/libgq)

install(
    TARGETS qicdbearerplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/bearer
)
