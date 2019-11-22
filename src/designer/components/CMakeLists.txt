add_definitions(-DQT_STATICPLUGIN)
set(EXTRA_DESIGNERCOMPONENTS_LIBS KtCore KtGui KtXml KtDesigner KtScript)

include(buddyeditor/buddyeditor.cmake)
include(formeditor/formeditor.cmake)
include(objectinspector/objectinspector.cmake)
include(propertyeditor/propertyeditor.cmake)
include(signalsloteditor/signalsloteditor.cmake)
include(tabordereditor/tabordereditor.cmake)
include(taskmenu/taskmenu.cmake)
include(widgetbox/widgetbox.cmake)

set(DESIGNERCOMPONENTS_SOURCES
    ${DESIGNERCOMPONENTS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesigner_components.cpp
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
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    ${CMAKE_BINARY_DIR}/privateinclude/QtUiTools
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtDesignerComponents
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesignerComponents
)

if(KATIE_TYPE STREQUAL SHARED)
    add_definitions(
        -DQDESIGNER_SDK_LIBRARY
        -DQDESIGNER_EXTENSION_LIBRARY
        -DQDESIGNER_SHARED_LIBRARY
        -DQDESIGNER_COMPONENTS_LIBRARY
    )
else()
    add_definitions(-DQT_DESIGNER_STATIC)
endif()

set(DESIGNERCOMPONENTS_HEADERS
    ${DESIGNERCOMPONENTS_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qdesignercomponentscommon_p.h
)

katie_generate_misc("${DESIGNERCOMPONENTS_HEADERS}" QtDesignerComponents)
katie_generate_public("${DESIGNERCOMPONENTS_PUBLIC_HEADERS}" QtDesignerComponents)
katie_generate_package(KtDesignerComponents "KtCore KtGui KtXml KtDesigner KtScript")
katie_setup_target(KtDesignerComponents ${DESIGNERCOMPONENTS_SOURCES} ${DESIGNERCOMPONENTS_HEADERS})

add_library(KtDesignerComponents ${KATIE_TYPE} ${KtDesignerComponents_SOURCES}
    $<TARGET_OBJECTS:sharedfindwidget>
    $<TARGET_OBJECTS:sharedqtpropertybrowser>
    $<TARGET_OBJECTS:sharedqtgradienteditor>
)
target_link_libraries(KtDesignerComponents ${EXTRA_DESIGNERCOMPONENTS_LIBS})
set_target_properties(KtDesignerComponents PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME DesignerComponents
)

katie_setup_object(KtDesignerComponents sharedfindwidget sharedqtpropertybrowser sharedqtgradienteditor)

install(
    TARGETS KtDesignerComponents
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtDesignerComponents
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtDesignerComponents)
