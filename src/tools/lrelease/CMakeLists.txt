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
)

katie_setup_target(lrelease ${LRELEASE_SOURCES} ${LRELEASE_HEADERS})

add_executable(lrelease ${lrelease_SOURCES}
    $<TARGET_OBJECTS:sharedlinguist>
)
target_link_libraries(lrelease ${EXTRA_LRELEASE_LIBS})

set_target_properties(lrelease PROPERTIES
    EXPORT_NAME lrelease
    OUTPUT_NAME lrelease${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS lrelease
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_RELATIVE}
    COMPONENT Devel
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lrelease.1
    DESTINATION ${KATIE_MAN_RELATIVE}/man1
    RENAME lrelease${KATIE_TOOLS_SUFFIX}.1
)
