add_definitions(
    ${SHAREDGRADIENDEDITOR_DEFINITIONS}
    ${SHAREDPROPERTYBROWSER_DEFINITIONS}
)
set(EXTRA_DESIGNER_LIBS KtCore KtGui KtXml KtUiTools)

include(extension/extension.cmake)
include(sdk/sdk.cmake)
include(shared/shared.cmake)

add_subdirectory(components)

set(DESIGNER_PUBLIC_HEADERS
    ${DESIGNER_PUBLIC_HEADERS}
    QAbstractExtensionFactory
    QAbstractExtensionManager
    QAbstractFormBuilder
    QDesignerActionEditorInterface
    QDesignerBrushManagerInterface
    QDesignerComponents
    QDesignerContainerExtension
    QDesignerCustomWidgetCollectionInterface
    QDesignerCustomWidgetInterface
    QDesignerDnDItemInterface
    QDesignerDynamicPropertySheetExtension
    QDesignerExportWidget
    QDesignerExtraInfoExtension
    QDesignerFormEditorInterface
    QDesignerFormEditorPluginInterface
    QDesignerFormWindowCursorInterface
    QDesignerFormWindowInterface
    QDesignerFormWindowManagerInterface
    QDesignerFormWindowToolInterface
    QDesignerIconCacheInterface
    QDesignerIntegrationInterface
    QDesignerLanguageExtension
    QDesignerLayoutDecorationExtension
    QDesignerMemberSheetExtension
    QDesignerMetaDataBaseInterface
    QDesignerMetaDataBaseItemInterface
    QDesignerObjectInspectorInterface
    QDesignerPromotionInterface
    QDesignerPropertyEditorInterface
    QDesignerPropertySheetExtension
    QDesignerResourceBrowserInterface
    QDesignerTaskMenuExtension
    QDesignerWidgetBoxInterface
    QDesignerWidgetDataBaseInterface
    QDesignerWidgetDataBaseItemInterface
    QDesignerWidgetFactoryInterface
    QExtensionFactory
    QExtensionManager
    QFormBuilder
)

set(DESIGNER_HEADERS
    ${DESIGNER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/container.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesignerexportwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_components_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_components.h
    ${SHAREDGRADIENDEDITOR_HEADERS}
    ${SHAREDPROPERTYBROWSER_HEADERS}
    ${SHAREDDEVICESKIN_HEADERS}
    ${SHAREDFINDWIDGET_HEADERS}
)

set(DESIGNER_SOURCES
    ${DESIGNER_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/tools/rcc/rcc.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    ${CMAKE_BINARY_DIR}/privateinclude/QtUiTools
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesigner
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/components
    ${CMAKE_CURRENT_SOURCE_DIR}/extension
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk
    ${CMAKE_CURRENT_SOURCE_DIR}/shared
    ${CMAKE_CURRENT_BINARY_DIR}/components
    ${CMAKE_CURRENT_BINARY_DIR}/extension
    ${CMAKE_CURRENT_BINARY_DIR}/sdk
    ${CMAKE_CURRENT_BINARY_DIR}/shared
    ${SHAREDGRADIENDEDITOR_INCLUDES}
    ${SHAREDPROPERTYBROWSER_INCLUDES}
    ${SHAREDDEVICESKIN_INCLUDES}
    ${SHAREDFINDWIDGET_INCLUDES}
    # for widgets.table
    ${CMAKE_SOURCE_DIR}/src/shared
    # for rcc.h
    ${CMAKE_SOURCE_DIR}/src/tools/rcc
)

if(${KATIE_TYPE} STREQUAL SHARED)
    add_definitions(
        -DQDESIGNER_SDK_LIBRARY
        -DQDESIGNER_EXTENSION_LIBRARY
        -DQDESIGNER_SHARED_LIBRARY
    )
else()
    add_definitions(-DQT_DESIGNER_STATIC)
endif()

set(DESIGNER_HEADERS
    ${DESIGNER_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesignercommon_p.h
)

katie_generate_misc("${DESIGNER_HEADERS}" QtDesigner)
katie_generate_public("${DESIGNER_PUBLIC_HEADERS}" QtDesigner)
katie_generate_package(KtDesigner "KtCore KtGui KtXml KtUiTools")
katie_setup_target(KtDesigner ${DESIGNER_SOURCES} ${DESIGNER_HEADERS})

add_library(KtDesigner ${KATIE_TYPE} ${KtDesigner_SOURCES}
    $<TARGET_OBJECTS:sharedqtgradienteditor>
    $<TARGET_OBJECTS:sharedqtpropertybrowser>
    $<TARGET_OBJECTS:shareddeviceskin>
    $<TARGET_OBJECTS:sharedfindwidget>
)
target_link_libraries(KtDesigner ${EXTRA_DESIGNER_LIBS})
set_target_properties(KtDesigner PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Designer
)

katie_setup_object(KtDesigner sharedqtgradienteditor sharedqtpropertybrowser shareddeviceskin sharedfindwidget)

install(
    TARGETS KtDesigner
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtDesigner
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtDesigner)
