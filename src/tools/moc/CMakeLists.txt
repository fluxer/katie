add_definitions(-DQT_MOC)
set(EXTRA_MOC_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOTSTRAP_INCLUDES}
)

set(MOC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mocmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/moc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/token.cpp
)

set(MOC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/moc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/preprocessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/symbols.h
    ${CMAKE_CURRENT_SOURCE_DIR}/token.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/outputrevision.h
    ${BOOTSTRAP_HEADERS}
)

# katie_resources("${MOC_SOURCES}")
katie_setup_flags()

add_executable(bootstrap_moc $<TARGET_OBJECTS:bootstrap> ${MOC_SOURCES} ${MOC_HEADERS})
target_compile_definitions(bootstrap_moc PRIVATE ${BOOTSTRAP_DEFINITIONS})

add_executable(moc ${MOC_SOURCES} ${MOC_HEADERS})
target_link_libraries(moc ${EXTRA_MOC_LIBS})
set_target_properties(moc PROPERTIES
    EXPORT_NAME moc
)

install(
    TARGETS moc
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH_INST}
    COMPONENT Devel
)

if(NOT "${KATIE_PLATFORM}" MATCHES "(win32|wince|mac)")
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/moc.1
        DESTINATION ${MAN_INSTALL_PATH_INST}/man1
    )
endif()
