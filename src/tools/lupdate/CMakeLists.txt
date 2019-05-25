add_definitions(${SHAREDLINGUIST_DEFINITIONS})
set(EXTRA_LUPDATE_LIBS KtCore KtXml)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${SHAREDLINGUIST_INCLUDES}
)

set(LUPDATE_SOURCES
    ${LUPDATE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/lupdate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/merge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qscript.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui.cpp
)

set(LUPDATE_HEADERS
    ${LUPDATE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/lupdate.h
    ${SHAREDLINGUIST_HEADERS}
)

katie_setup_target(lupdate ${LUPDATE_SOURCES} ${LUPDATE_HEADERS})

add_executable(lupdate ${lupdate_SOURCES}
    $<TARGET_OBJECTS:sharedlinguist>
)
target_link_libraries(lupdate ${EXTRA_LUPDATE_LIBS})

set_target_properties(lupdate PROPERTIES
    EXPORT_NAME lupdate
    OUTPUT_NAME lupdate${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS lupdate
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_RELATIVE}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/lupdate.1
    DESTINATION ${KATIE_MAN_RELATIVE}/man1
    RENAME lupdate${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
