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

# this is done because of bootstrap_moc
if(KATIE_PLATFORM STREQUAL "mac")
    set(CMAKE_SHARED_LINK_FLAGS
        ${CMAKE_SHARED_LINK_FLAGS}
        -framework CoreFoundation
        -framework Security
        -framework ApplicationServices
        -lObjC
    )
endif()

katie_setup_paths()

add_executable(bootstrap_moc $<TARGET_OBJECTS:bootstrap> ${MOC_SOURCES})
target_compile_definitions(bootstrap_moc PRIVATE ${BOOTSTRAP_DEFINITIONS})

add_executable(moc ${MOC_SOURCES})
target_link_libraries(moc ${EXTRA_MOC_LIBS})

install(
    TARGETS moc
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

if(NOT "${KATIE_PLATFORM}" MATCHES "(win32|wince|mac)")
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/moc.1
        DESTINATION ${MAN_INSTALL_PATH}/man1
    )
endif()
