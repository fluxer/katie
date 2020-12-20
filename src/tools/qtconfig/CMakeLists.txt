# add_definitions()
set(EXTRA_QTCONFIG_LIBS
    KtCore KtGui
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    # for UI file includes
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(QTCONFIG_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/colorbutton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/previewframe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/previewwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/paletteeditoradvanced.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/paletteeditoradvanced.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/previewwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qtconfig.qrc
)

set(QTCONFIG_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/colorbutton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/previewframe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/previewwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/paletteeditoradvanced.h
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/qtconfig.desktop.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/qtconfig.desktop
    @ONLY
)

katie_setup_target(qtconfig ${QTCONFIG_SOURCES} ${QTCONFIG_HEADERS})

add_executable(qtconfig ${qtconfig_SOURCES})
target_link_libraries(qtconfig ${EXTRA_QTCONFIG_LIBS})

set_target_properties(qtconfig PROPERTIES
    EXPORT_NAME qtconfig
    OUTPUT_NAME qtconfig${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS qtconfig
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/qtconfig.desktop
    DESTINATION ${KATIE_APPLICATIONS_PATH}
    RENAME qtconfig${KATIE_TOOLS_SUFFIX}.desktop
    COMPONENT Tools
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qtconfig.png
    DESTINATION ${KATIE_PIXMAPS_PATH}
    RENAME qtconfig${KATIE_TOOLS_SUFFIX}.png
    COMPONENT Tools
)
