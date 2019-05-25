add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    ${SHAREDLINGUIST_DEFINITIONS}
)
set(EXTRA_LCONVERT_LIBS KtCore KtXml)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${SHAREDLINGUIST_INCLUDES}
)

set(LCONVERT_SOURCES
    ${LCONVERT_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/lconvert.cpp
)

set(LCONVERT_HEADERS
    ${LCONVERT_HEADERS}
    ${SHAREDLINGUIST_HEADERS}
)

katie_setup_target(lconvert ${LCONVERT_SOURCES} ${LCONVERT_HEADERS})

add_executable(lconvert ${lconvert_SOURCES}
    $<TARGET_OBJECTS:sharedlinguist>
)
target_link_libraries(lconvert ${EXTRA_LCONVERT_LIBS})

set_target_properties(lconvert PROPERTIES
    EXPORT_NAME lconvert
    OUTPUT_NAME lconvert${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS lconvert
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_RELATIVE}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lconvert.1
    DESTINATION ${KATIE_MAN_RELATIVE}/man1
    RENAME lconvert${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
