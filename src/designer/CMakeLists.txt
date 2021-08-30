add_definitions(
    -DKATIE_TOOLS_SUFFIX="${KATIE_TOOLS_SUFFIX}"
)
set(EXTRA_DESIGNER_LIBS
    KtCore KtGui KtXml KtUiTools KtScript
)

set(DESIGNER_PUBLIC_HEADERS
    QExtensionManager
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
    # for widgets.table
    ${CMAKE_SOURCE_DIR}/src/shared
    # for rcc.h
    ${CMAKE_SOURCE_DIR}/src/tools/rcc
    # for ui4.h
    ${CMAKE_SOURCE_DIR}/src/tools/uic
)

set(DESIGNER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/extension/default_extensionfactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/extension/extension.h
    ${CMAKE_CURRENT_SOURCE_DIR}/extension/qextensionmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformeditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractintrospection_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractsettings_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformeditorplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractresourcebrowser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractintegration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractpropertyeditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindowtool.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindowcursor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindowmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractwidgetdatabase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractmetadatabase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractwidgetfactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractobjectinspector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractactioneditor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractbrushmanager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstracticoncache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractlanguage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractoptionspage_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/propertysheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/dynamicpropertysheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/membersheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/taskmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/extrainfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractwidgetbox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/layoutdecoration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractdnditem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractpromotioninterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractdialoggui_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/script_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractnewformwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/spacer_widget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/layoutinfo_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/layout_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/connectionedit_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/pluginmanager_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/metadatabase_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formeditorcommand_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formwindowcommand_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_command_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/morphmenu_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_command2_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_propertycommand_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formbuilder_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_taskmenu_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/formlayoutmenu_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_widget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_propertysheet_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_membersheet_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_propertyeditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_objectinspector_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_integration_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/invisible_widget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qlayout_widget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/sheet_delegate_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_stackedbox_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_tabwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_dockwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_toolbox_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_dnditem_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qsimpleresource_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/widgetfactory_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/widgetdatabase_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_promotion_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_introspection_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/promotionmodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_promotiondialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/iconloader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/richtexteditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/plaintexteditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/actioneditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/actionrepository_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_toolbar_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_menubar_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_menu_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/actionprovider_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/orderdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/newactiondialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/stylesheeteditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/csshighlighter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/shared_enums_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/textpropertyeditor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/propertylineedit_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/promotiontaskmenu_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/scripterrordialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/scriptcommand_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/scriptdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qscripthighlighter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/gridpanel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/grid_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/formwindowbase_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_utils_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_widgetbox_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/signalslotdialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/extensionfactory_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/dialoggui_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/deviceprofile_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/zoomwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/previewmanager_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/previewconfigurationwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/codedialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qtresourceeditordialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qtresourcemodel_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qtresourceview_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/iconselector_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/htmlhighlighter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_widgetitem_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_qsettings_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formwindowmanager_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/shared_settings_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/newformwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/filterwidget_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/plugindialog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesignerexportwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_components_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_components.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesignercommon_p.h
)

set(DESIGNER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/extension/default_extensionfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extension/extension.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extension/qextensionmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractintrospection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformeditorplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractresourcebrowser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractintegration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractpropertyeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindowtool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindowcursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractformwindowmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractwidgetdatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractmetadatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractwidgetfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractobjectinspector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractactioneditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractwidgetbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/extrainfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractpromotioninterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractdialoggui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/script.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/abstractnewformwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/spacer_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/layoutinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/connectionedit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/pluginmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formwindowcommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formeditorcommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_command.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/morphmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_command2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_propertycommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_taskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/formlayoutmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_dockwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_propertysheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_membersheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_propertyeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_objectinspector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_integration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_dnditem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qsimpleresource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/invisible_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qlayout_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/sheet_delegate.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/metadatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_stackedbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_tabwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_toolbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/widgetfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/widgetdatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_promotion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_introspection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/promotionmodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_promotiondialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/richtexteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/plaintexteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/actioneditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/actionrepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_toolbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_menubar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/orderdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/newactiondialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/stylesheeteditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/csshighlighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/textpropertyeditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/propertylineedit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/promotiontaskmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/scripterrordialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/scriptcommand.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/scriptdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qscripthighlighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/gridpanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/grid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/formwindowbase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_widgetbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/iconloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/signalslotdialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/dialoggui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/deviceprofile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/zoomwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/previewmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/previewconfigurationwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/codedialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qtresourceeditordialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qtresourcemodel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qtresourceview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/iconselector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/htmlhighlighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_widgetitem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_qsettings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qdesigner_formwindowmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/shared_settings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/newformwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/filterwidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/plugindialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/shared.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/addlinkdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/orderdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/newactiondialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/gridpanel.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/signalslotdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/previewconfigurationwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/qtresourceeditordialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/newformwidget.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/selectsignaldialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/formlayoutrowdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/plugindialog.ui
    ${CMAKE_SOURCE_DIR}/src/tools/rcc/rcc.cpp
)

add_subdirectory(components)

katie_generate_misc("${DESIGNER_HEADERS}" QtDesigner)
katie_generate_public("${DESIGNER_PUBLIC_HEADERS}" QtDesigner)
katie_generate_package(KtDesigner "KtCore KtGui KtXml KtUiTools KtScript")
katie_setup_target(KtDesigner ${DESIGNER_SOURCES} ${DESIGNER_HEADERS})

add_library(KtDesigner ${KATIE_TYPE} ${KtDesigner_SOURCES}
    $<TARGET_OBJECTS:sharedqtgradienteditor>
    $<TARGET_OBJECTS:sharedqtpropertybrowser>
    $<TARGET_OBJECTS:sharedfindwidget>
)
target_link_libraries(KtDesigner PRIVATE ${EXTRA_DESIGNER_LIBS})
set_target_properties(KtDesigner PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Designer
)

katie_setup_object(KtDesigner sharedqtgradienteditor sharedqtpropertybrowser sharedfindwidget)

install(
    TARGETS KtDesigner
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtDesigner
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtDesigner)
