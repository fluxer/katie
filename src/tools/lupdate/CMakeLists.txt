add_definitions(${SHAREDLINGUIST_DEFINITIONS})
set(EXTRA_LUPDATE_LIBS KtCore KtXml KtDeclarative)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/privateinclude/QtDeclarative
    ${SHAREDLINGUIST_INCLUDES}
)

set(LUPDATE_SOURCES
    ${LUPDATE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/lupdate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/merge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/java.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qscript.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui.cpp
)

set(LUPDATE_HEADERS
    ${LUPDATE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/lupdate.h
    ${SHAREDLINGUIST_HEADERS}
)

katie_setup_flags()

add_executable(lupdate $<TARGET_OBJECTS:sharedlinguist> ${LUPDATE_SOURCES} ${LUPDATE_HEADERS})
target_link_libraries(lupdate ${EXTRA_LUPDATE_LIBS})
set_target_properties(lupdate PROPERTIES
    EXPORT_NAME lupdate
)

install(
    TARGETS lupdate
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH_INST}
    COMPONENT Devel
)
