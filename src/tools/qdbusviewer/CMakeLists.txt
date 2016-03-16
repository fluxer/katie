# add_definitions()
set(EXTRA_QDBUSVIEWER_LIBS KtCore KtGui KtXml KtDBus)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/privateinclude/QtDBus
)

set(QDBUSVIEWER_HEADERS
    ${QDBUSVIEWER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertydialog.h
)

set(QDBUSVIEWER_SOURCES
    ${QDBUSVIEWER_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertydialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewermain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.qrc
)

katie_resources(${QDBUSVIEWER_SOURCES} ${QDBUSVIEWER_HEADERS})
katie_setup_flags()

add_executable(qdbusviewer ${QDBUSVIEWER_SOURCES} ${QDBUSVIEWER_HEADERS})
target_link_libraries(qdbusviewer ${EXTRA_QDBUSVIEWER_LIBS})
set_target_properties(qdbusviewer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    EXPORT_NAME qdbusviewer
)

install(
    TARGETS qdbusviewer
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH_INST}
    COMPONENT Devel
)
