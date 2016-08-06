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

katie_setup_flags()
katie_setup_paths()

add_executable(lupdate $<TARGET_OBJECTS:sharedlinguist> ${LUPDATE_SOURCES} ${LUPDATE_HEADERS})
target_link_libraries(lupdate ${EXTRA_LUPDATE_LIBS})

install(
    TARGETS lupdate
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

if(NOT KATIE_PLATFORM MATCHES "(win32|wince|mac)")
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/lupdate.1
        DESTINATION ${MAN_INSTALL_PATH}/man1
    )
endif()
