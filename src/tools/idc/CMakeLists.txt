# add_definitions()
set(EXTRA_IDC_LIBS "")

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOTSTRAP_INCLUDES}
)

set(IDC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(IDC_HEADERS
    ${BOOTSTRAP_HEADERS}
)

# katie_resources("${IDC_SOURCES}")
katie_setup_flags()

add_executable(idc $<TARGET_OBJECTS:bootstrap> ${IDC_SOURCES} ${IDC_HEADERS})
target_link_libraries(idc ${EXTRA_IDC_LIBS})
set_target_properties(idc PROPERTIES
    EXPORT_NAME idc
)

target_compile_definitions(idc PRIVATE ${BOOTSTRAP_DEFINITIONS})

install(
    TARGETS idc
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH_INST}
    COMPONENT Devel
)
