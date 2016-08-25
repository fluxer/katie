include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/connman
    ${CMAKE_CURRENT_BINARY_DIR}/bearer
)

set(QTCONNMANBEARERPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanservice_linux_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qofonoservice_linux_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QTCONNMANBEARERPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/cmmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanservice_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qofonoservice_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

katie_setup_target(qconnmanbearerplugin ${QTCONNMANBEARERPLUGIN_SOURCES} ${QTCONNMANBEARERPLUGIN_HEADERS})

add_library(qconnmanbearerplugin MODULE ${qconnmanbearerplugin_SOURCES})
target_link_libraries(qconnmanbearerplugin KtCore KtNetwork KtDBus)
set_target_properties(qconnmanbearerplugin PROPERTIES OUTPUT_NAME qconnmanbearer)
target_include_directories(qconnmanbearerplugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer
)

install(
    TARGETS qconnmanbearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
