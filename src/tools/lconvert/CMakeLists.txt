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
katie_setup_paths()

add_executable(lconvert ${lconvert_SOURCES}
    $<TARGET_OBJECTS:sharedlinguist>
)
target_link_libraries(lconvert ${EXTRA_LCONVERT_LIBS})

install(
    TARGETS lconvert
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

if(NOT KATIE_PLATFORM MATCHES "(win32|wince|mac)")
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/lconvert.1
        DESTINATION ${MAN_INSTALL_PATH}/man1
    )
endif()
