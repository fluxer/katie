add_definitions(${SHAREDHELP_DEFINITIONS})
set(EXTRA_QHELPGENERATOR_LIBS KtCore KtGui KtNetwork KtHelp KtSql)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtHelp
    ${CMAKE_BINARY_DIR}/privateinclude/QtHelp
    ${SHAREDHELP_INCLUDES}
)

set(QHELPGENERATOR_SOURCES
    ${QHELPGENERATOR_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/qhelpgenerator.cpp
)

set(QHELPGENERATOR_HEADERS
    ${QHELPGENERATOR_HEADERS}
    ${SHAREDHELP_HEADERS}
)

katie_resources("${QHELPGENERATOR_SOURCES}")
katie_resources("${QHELPGENERATOR_HEADERS}")
katie_setup_flags()

add_executable(qhelpgenerator $<TARGET_OBJECTS:sharedhelp> ${QHELPGENERATOR_SOURCES} ${QHELPGENERATOR_HEADERS})
target_link_libraries(qhelpgenerator ${EXTRA_QHELPGENERATOR_LIBS})
set_target_properties(qhelpgenerator PROPERTIES
    EXPORT_NAME qhelpgenerator
)

install(
    TARGETS qhelpgenerator
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH_INST}
    COMPONENT Devel
)
