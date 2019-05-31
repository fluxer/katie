add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
)
set(EXTRA_LRELEASE_LIBS KtCore KtXml)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
)

add_executable(lrelease ${CMAKE_CURRENT_SOURCE_DIR}/lrelease.cpp
    $<TARGET_OBJECTS:sharedlinguist>
)
target_link_libraries(lrelease ${EXTRA_LRELEASE_LIBS})

set_target_properties(lrelease PROPERTIES
    EXPORT_NAME lrelease
    OUTPUT_NAME lrelease${KATIE_TOOLS_SUFFIX}
)

katie_setup_object(lrelease sharedlinguist)

install(
    TARGETS lrelease
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_RELATIVE}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lrelease.1
    DESTINATION ${KATIE_MAN_RELATIVE}/man1
    RENAME lrelease${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
