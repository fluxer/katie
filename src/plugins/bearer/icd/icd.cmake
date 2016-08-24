# add_definitions(-DBEARER_MANAGEMENT_DEBUG)

set(ICDPLUGIN_HEADERS
    ${ICDPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/qicdengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/dbusdispatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/iapconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/iapmonitor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/maemo_icd.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/proxyconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/wlan-utils.h
)

set(ICDPLUGIN_SOURCES
    ${ICDPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/icdmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/qicdengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/qnetworksession_impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/dbusdispatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/iapmonitor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/iapconf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/maemo_icd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/icd/proxyconf.cpp
)


add_library(qicdbearerplugin MODULE ${ICDPLUGIN_SOURCES} ${ICDPLUGIN_HEADERS})
set_target_properties(qicdbearerplugin PROPERTIES OUTPUT_NAME qicdbearer)
target_include_directories(qicdbearerplugin PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/libgq)

katie_setup_target(qicdbearerplugin)

install(
    TARGETS qicdbearerplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/bearer
)
