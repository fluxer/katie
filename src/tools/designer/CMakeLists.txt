# add_definitions()
set(EXTRA_DESIGNERBIN_LIBS
    KtCore KtGui KtXml KtDesigner KtDesignerComponents
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtDesignerComponents
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    ${CMAKE_CURRENT_SOURCE_DIR}/fontpanel
    ${CMAKE_CURRENT_SOURCE_DIR}/qttoolbardialog
    ${CMAKE_CURRENT_BINARY_DIR}/fontpanel
    ${CMAKE_CURRENT_BINARY_DIR}/qttoolbardialog
)

set(DESIGNERBIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/fontpanel/fontpanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qttoolbardialog/qttoolbardialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_toolwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_formwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_workbench.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_settings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_actions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_appearanceoptions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/saveformastemplate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/newform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/versiondialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/designer_enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/appfontdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencesdialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
)

set(DESIGNERBIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/fontpanel/fontpanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qttoolbardialog/qttoolbardialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qttoolbardialog/qttoolbardialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qttoolbardialog/qttoolbardialog.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_toolwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_formwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_workbench.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_settings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_actions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_appearanceoptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/saveformastemplate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/newform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/versiondialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/appfontdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencesdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/saveformastemplate.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/preferencesdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_appearanceoptions.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/designer.qrc
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/designer.desktop.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/designer.desktop
    @ONLY
)

katie_setup_target(designer ${DESIGNERBIN_SOURCES} ${DESIGNERBIN_HEADERS})

add_executable(designer ${designer_SOURCES})
target_link_libraries(designer ${EXTRA_DESIGNERBIN_LIBS})

set_target_properties(designer PROPERTIES
    EXPORT_NAME designer
    OUTPUT_NAME designer${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS designer
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/designer.desktop
    DESTINATION ${KATIE_APPLICATIONS_PATH}
    RENAME designer${KATIE_TOOLS_SUFFIX}.desktop
    COMPONENT Tools
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/designer.png
    DESTINATION ${KATIE_PIXMAPS_PATH}
    RENAME designer${KATIE_TOOLS_SUFFIX}.png
    COMPONENT Tools
)
