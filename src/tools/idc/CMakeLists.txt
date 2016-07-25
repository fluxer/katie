# add_definitions()
set(EXTRA_IDC_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(IDC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_flags()

add_executable(idc ${IDC_SOURCES})
target_link_libraries(idc ${EXTRA_IDC_LIBS})

install(
    TARGETS idc
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)
