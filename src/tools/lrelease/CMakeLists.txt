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
    ${BOOTSTRAP_INCLUDES}
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

# can not reuse sharedlinguist objects since this is a bootstrap target and
# they must take the bootstrap definitions into account
add_executable(bootstrap_lrelease $<TARGET_OBJECTS:bootstrap>
    ${SHAREDLINGUIST_SOURCES} ${LRELEASE_SOURCES} ${LRELEASE_HEADERS})
target_compile_definitions(bootstrap_lrelease PRIVATE ${BOOTSTRAP_DEFINITIONS})

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
