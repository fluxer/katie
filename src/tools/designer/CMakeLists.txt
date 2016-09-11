add_definitions(${SHAREDDEVICESKIN_DEFINITIONS})
set(EXTRA_DESIGNERBIN_LIBS KtCore KtGui KtNetwork KtDesigner KtDesignerComponents m)

include(fontpanel/fontpanel.cmake)
include(qttoolbardialog/qttoolbardialog.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtDesignerComponents
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesignerComponents
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    ${CMAKE_BINARY_DIR}/privateinclude/QtUiTools
    ${CMAKE_CURRENT_SOURCE_DIR}/fontpanel
    ${CMAKE_CURRENT_SOURCE_DIR}/qttoolbardialog
    ${CMAKE_CURRENT_BINARY_DIR}/fontpanel
    ${CMAKE_CURRENT_BINARY_DIR}/qttoolbardialog
    ${CMAKE_SOURCE_DIR}/src/designer/sdk
    ${CMAKE_SOURCE_DIR}/src/designer/extension
    ${CMAKE_SOURCE_DIR}/src/designer/shared
    ${CMAKE_SOURCE_DIR}/src/designer
    ${SHAREDDEVICESKIN_INCLUDES}
)

set(DESIGNERBIN_HEADERS
    ${DESIGNERBIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_toolwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_formwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_workbench.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_settings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_actions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_appearanceoptions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/saveformastemplate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/newform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/versiondialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/designer_enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/appfontdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencesdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/assistantclient.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
    ${SHAREDDEVICESKIN_HEADERS}
)

set(DESIGNERBIN_SOURCES
    ${DESIGNERBIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_toolwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_formwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_workbench.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_settings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_actions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_appearanceoptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/saveformastemplate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/newform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/versiondialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appfontdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencesdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/assistantclient.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/saveformastemplate.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencesdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_appearanceoptions.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/designer.qrc
)

if(NOT KATIE_TYPE STREQUAL SHARED)
    add_definitions(-DQT_DESIGNER_STATIC)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/designer.desktop.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/designer.desktop
    @ONLY
)


katie_setup_target(designer ${DESIGNERBIN_SOURCES} ${DESIGNERBIN_HEADERS})
katie_setup_paths()

add_executable(designer
    ${designer_SOURCES}
    $<TARGET_OBJECTS:shareddeviceskin>
)
target_link_libraries(designer ${EXTRA_DESIGNERBIN_LIBS})

install(
    TARGETS designer
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/designer.desktop
    DESTINATION ${APPLICATIONS_INSTALL_PATH}
    COMPONENT Devel
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/designer.png
    DESTINATION ${PIXMAPS_INSTALL_PATH}
    COMPONENT Devel
)
