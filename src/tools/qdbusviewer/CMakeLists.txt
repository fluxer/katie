# add_definitions()
set(EXTRA_QDBUSVIEWER_LIBS
    KtCore KtGui KtXml KtDBus
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/include/QtDBus
)

set(QDBUSVIEWER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertydialog.h
)

set(QDBUSVIEWER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertydialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewermain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.qrc
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.desktop.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/qdbusviewer.desktop
    @ONLY
)

katie_setup_target(qdbusviewer ${QDBUSVIEWER_SOURCES} ${QDBUSVIEWER_HEADERS})

add_executable(qdbusviewer ${qdbusviewer_SOURCES})
target_link_libraries(qdbusviewer ${EXTRA_QDBUSVIEWER_LIBS})

set_target_properties(qdbusviewer PROPERTIES
    EXPORT_NAME qdbusviewer
    OUTPUT_NAME qdbusviewer${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS qdbusviewer
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/qdbusviewer.desktop
    DESTINATION ${KATIE_APPLICATIONS_PATH}
    RENAME qdbusviewer${KATIE_TOOLS_SUFFIX}.desktop
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.png
    DESTINATION ${KATIE_PIXMAPS_PATH}
    RENAME qdbusviewer${KATIE_TOOLS_SUFFIX}.png
    COMPONENT Tools
)
