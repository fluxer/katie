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

if(KATIE_PLATFORM MATCHES "(win32|wince)")
    set(QDBUSVIEWER_SOURCES
        ${QDBUSVIEWER_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.rc
    )
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusviewer.desktop.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/qdbusviewer.desktop
    @ONLY
)


katie_setup_target(qdbusviewer ${QDBUSVIEWER_SOURCES} ${QDBUSVIEWER_HEADERS})
katie_setup_paths()

add_executable(qdbusviewer
    WIN32 # TODO: MACOSX_BUNDLE
    ${qdbusviewer_SOURCES}
)
target_link_libraries(qdbusviewer ${EXTRA_QDBUSVIEWER_LIBS})

install(
    TARGETS qdbusviewer
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

if(NOT KATIE_PLATFORM MATCHES "(win32|wince|mac)")
    install(
        PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/qdbusviewer.desktop
        DESTINATION ${APPLICATIONS_INSTALL_PATH}
        COMPONENT Devel
    )
    install(
        FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/qdbusviewer.png
        DESTINATION ${PIXMAPS_INSTALL_PATH}
        COMPONENT Devel
    )
endif()
