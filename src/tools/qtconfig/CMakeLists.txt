# add_definitions()
set(EXTRA_QTCONFIG_LIBS KtCore KtGui)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
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
katie_setup_paths()

add_executable(qtconfig
    WIN32 # TODO: MACOSX_BUNDLE
    ${qtconfig_SOURCES}
)
target_link_libraries(qtconfig ${EXTRA_QTCONFIG_LIBS})

install(
    TARGETS qtconfig
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/qtconfig.desktop
    DESTINATION ${APPLICATIONS_INSTALL_PATH}
    COMPONENT Devel
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/qtconfig.png
    DESTINATION ${PIXMAPS_INSTALL_PATH}
    COMPONENT Devel
)
