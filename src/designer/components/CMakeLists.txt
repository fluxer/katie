set(EXTRA_DESIGNERCOMPONENTS_LIBS
    KtCore KtGui KtXml KtDesigner KtScript KtUiTools
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtDesignerComponents
)

set(DESIGNERCOMPONENTS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/buddyeditor/buddyeditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/buddyeditor/buddyeditor_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/buddyeditor/buddyeditor_tool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/buddyeditor/buddyeditor_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qdesigner_resource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qdesignerundostack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindow_widgetstack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindow_dnditem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindowcursor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/widgetselection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindowmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formeditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formeditor_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qlayoutwidget_propertysheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/layout_propertysheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/spacer_propertysheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/line_propertysheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/default_container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/default_actionprovider.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qmainwindow_container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qworkspace_container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qmdiarea_container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qwizard_container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/default_layoutdecoration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qtbrushmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/brushmanagerproxy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/iconcache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/tool_widgeteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formeditor_optionspage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/embeddedoptionspage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindowsettings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/deviceprofiledialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/dpi_chooser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/previewactiongroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/itemview_propertysheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/templateoptionspage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectinspector/objectinspector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectinspector/objectinspectormodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectinspector/objectinspector_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/propertyeditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/designerpropertymanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/paletteeditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/paletteeditorbutton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/stringlisteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/stringlisteditorbutton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/previewwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/previewframe.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/newdynamicpropertydialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/brushpropertymanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/fontpropertymanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/propertyeditor_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/qlonglongvalidator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalslot_utils_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/connectdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_tool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditorwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tabordereditor/tabordereditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tabordereditor/tabordereditor_plugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tabordereditor/tabordereditor_tool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tabordereditor/tabordereditor_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/button_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/groupbox_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/label_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/lineedit_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/listwidget_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/treewidget_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/tablewidget_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/combobox_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/textedit_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/toolbar_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/containerwidget_taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/inplace_editor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/taskmenu_component.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/itemlisteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/listwidgeteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/treewidgeteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/tablewidgeteditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/inplace_widget_helper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/menutaskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/layouttaskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetboxcategorylistview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetboxtreewidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetbox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetbox_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetbox_dnditem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesignercomponentscommon_p.h
)

set(DESIGNERCOMPONENTS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/buddyeditor/buddyeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buddyeditor/buddyeditor_tool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buddyeditor/buddyeditor_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/deviceprofiledialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindowsettings.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/templateoptionspage.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qdesigner_resource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qdesignerundostack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindow_widgetstack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindow_dnditem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindowcursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/widgetselection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindowmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qlayoutwidget_propertysheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/layout_propertysheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/spacer_propertysheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/line_propertysheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qmainwindow_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qworkspace_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qmdiarea_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qwizard_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/default_container.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/default_layoutdecoration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/default_actionprovider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/tool_widgeteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/qtbrushmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/brushmanagerproxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/iconcache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formeditor_optionspage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/embeddedoptionspage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formwindowsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/deviceprofiledialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/dpi_chooser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/previewactiongroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/itemview_propertysheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/templateoptionspage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formeditor/formeditor.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/objectinspector/objectinspector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/objectinspector/objectinspectormodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/propertyeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/designerpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/paletteeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/paletteeditorbutton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/stringlisteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/stringlisteditorbutton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/previewwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/previewframe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/newdynamicpropertydialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/brushpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/fontpropertymanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/qlonglongvalidator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/propertyeditor.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/paletteeditor.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/stringlisteditor.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/previewwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyeditor/newdynamicpropertydialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalslot_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/connectdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_tool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditor_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/signalsloteditorwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalsloteditor/connectdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/tabordereditor/tabordereditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tabordereditor/tabordereditor_tool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tabordereditor/tabordereditor_plugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/button_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/groupbox_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/label_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/lineedit_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/listwidget_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/treewidget_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/tablewidget_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/combobox_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/textedit_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/toolbar_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/containerwidget_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/inplace_editor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/taskmenu_component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/itemlisteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/listwidgeteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/treewidgeteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/tablewidgeteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/inplace_widget_helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/menutaskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/layouttaskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/itemlisteditor.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/treewidgeteditor.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/taskmenu/tablewidgeteditor.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetboxcategorylistview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetboxtreewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetbox_dnditem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/widgetbox/widgetbox.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_components.cpp
)

katie_generate_misc("${DESIGNERCOMPONENTS_HEADERS}" QtDesignerComponents)
katie_generate_public("${DESIGNERCOMPONENTS_PUBLIC_HEADERS}" QtDesignerComponents)
katie_generate_package(KtDesignerComponents "KtCore KtGui KtXml KtDesigner KtScript KtUiTools")
katie_setup_target(KtDesignerComponents ${DESIGNERCOMPONENTS_SOURCES} ${DESIGNERCOMPONENTS_HEADERS})

add_library(KtDesignerComponents ${KATIE_TYPE} ${KtDesignerComponents_SOURCES}
    $<TARGET_OBJECTS:sharedfindwidget>
    $<TARGET_OBJECTS:sharedqtpropertybrowser>
    $<TARGET_OBJECTS:sharedqtgradienteditor>
)
target_link_libraries(KtDesignerComponents PRIVATE ${EXTRA_DESIGNERCOMPONENTS_LIBS})
set_target_properties(KtDesignerComponents PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME DesignerComponents
)

katie_setup_object(KtDesignerComponents sharedfindwidget sharedqtpropertybrowser sharedqtgradienteditor)

install(
    TARGETS KtDesignerComponents
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtDesignerComponents
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtDesignerComponents)
