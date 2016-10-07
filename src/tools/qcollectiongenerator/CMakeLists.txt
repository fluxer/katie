add_definitions(${SHAREDHELP_DEFINITIONS})
set(EXTRA_QCOLLECTIONGENERATOR_LIBS KtCore KtGui KtNetwork KtXml KtHelp)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_BINARY_DIR}/include/QtHelp
    ${CMAKE_BINARY_DIR}/privateinclude/QtHelp
    ${SHAREDHELP_INCLUDES}
)

set(QCOLLECTIONGENERATOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qcollectiongenerator.cpp
)

set(QCOLLECTIONGENERATOR_HEADERS
    ${SHAREDHELP_HEADERS}
)

katie_setup_target(qcollectiongenerator ${QCOLLECTIONGENERATOR_SOURCES} ${QCOLLECTIONGENERATOR_HEADERS})

add_executable(qcollectiongenerator ${qcollectiongenerator_SOURCES}
    $<TARGET_OBJECTS:sharedhelp>
)
target_link_libraries(qcollectiongenerator ${EXTRA_QCOLLECTIONGENERATOR_LIBS})

install(
    TARGETS qcollectiongenerator
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_RELATIVE}
    COMPONENT Devel
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qcollectiongenerator.1
    DESTINATION ${KATIE_MAN_RELATIVE}/man1
)
