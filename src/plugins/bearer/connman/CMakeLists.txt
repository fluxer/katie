set(QTCONNMANBEARERPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qconnmanservice_linux_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qofonoservice_linux_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qconnmanengine.h
)

set(QTCONNMANBEARERPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qconnmanservice_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qofonoservice_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qconnmanengine.cpp
)

katie_setup_target(qconnmanbearerplugin ${QTCONNMANBEARERPLUGIN_SOURCES} ${QTCONNMANBEARERPLUGIN_HEADERS})

add_library(qconnmanbearerplugin MODULE ${qconnmanbearerplugin_SOURCES}
    $<TARGET_OBJECTS:sharedbearer>
)
target_link_libraries(qconnmanbearerplugin KtCore KtNetwork KtDBus)
set_target_properties(qconnmanbearerplugin PROPERTIES OUTPUT_NAME qconnmanbearer)
target_include_directories(qconnmanbearerplugin PRIVATE
    ${CMAKE_SOURCE_DIR}/src/plugins/bearer
    ${CMAKE_BINARY_DIR}/src/plugins/bearer
)

katie_setup_object(qconnmanbearerplugin sharedbearer)
katie_setup_plugin(qconnmanbearerplugin)

install(
    TARGETS qconnmanbearerplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/bearer
    COMPONENT Runtime
)
