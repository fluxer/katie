add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    ${SHAREDLINGUIST_DEFINITIONS}
)
set(EXTRA_LRELEASE_LIBS KtCore KtXml)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${SHAREDLINGUIST_INCLUDES}
)

set(LRELEASE_SOURCES
    ${LRELEASE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/lrelease.cpp
)

set(LRELEASE_HEADERS
    ${LRELEASE_HEADERS}
    ${SHAREDLINGUIST_HEADERS}
    ${BOOTSTRAP_HEADERS}
)

katie_setup_flags()

add_executable(lrelease $<TARGET_OBJECTS:sharedlinguist> ${LRELEASE_SOURCES} ${LRELEASE_HEADERS})
target_link_libraries(lrelease ${EXTRA_LRELEASE_LIBS})
set_target_properties(lrelease PROPERTIES
    EXPORT_NAME lrelease
)

install(
    TARGETS lrelease
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH_INST}
    COMPONENT Devel
)

if(NOT "${KATIE_PLATFORM}" MATCHES "(win32|wince|mac)")
    install(
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/lrelease.1
        DESTINATION
        ${MAN_INSTALL_DIR}/man1
    )
endif()
