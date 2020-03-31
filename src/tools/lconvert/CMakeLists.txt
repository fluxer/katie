add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
)
set(EXTRA_LCONVERT_LIBS
    KtCore KtXml
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
)

add_executable(lconvert
    ${CMAKE_CURRENT_SOURCE_DIR}/lconvert.cpp
    $<TARGET_OBJECTS:sharedlinguist>
)
target_link_libraries(lconvert ${EXTRA_LCONVERT_LIBS})

set_target_properties(lconvert PROPERTIES
    EXPORT_NAME lconvert
    OUTPUT_NAME lconvert${KATIE_TOOLS_SUFFIX}
)

katie_setup_object(lconvert sharedlinguist)

install(
    TARGETS lconvert
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lconvert.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME lconvert${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
