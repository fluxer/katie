include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/bearer/connman
)

set(QTCONNMANBEARERPLUGIN_HEADERS
    ${QTCONNMANBEARERPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanservice_linux_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qofonoservice_linux_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanengine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)

set(QTCONNMANBEARERPLUGIN_SOURCES
    ${QTCONNMANBEARERPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/cmmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanservice_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qofonoservice_linux.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman/qconnmanengine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)

add_library(qconnmanbearerplugin MODULE ${QTCONNMANBEARERPLUGIN_SOURCES} ${QTCONNMANBEARERPLUGIN_HEADERS})
target_link_libraries(qconnmanbearerplugin KtCore KtNetwork KtDBus)
set_target_properties(qconnmanbearerplugin PROPERTIES OUTPUT_NAME qconnmanbearer)
target_include_directories(qconnmanbearerplugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/connman
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer
)

katie_setup_target(qconnmanbearerplugin)

install(
    TARGETS qconnmanbearerplugin
    DESTINATION ${QT_PLUGINS_PATH}/bearer
)
