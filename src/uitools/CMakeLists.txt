add_definitions(-DQT_BUILD_UITOOLS_LIB -DQDESIGNER_UILIB_LIBRARY)
set(EXTRA_UITOOLS_LIBS KtCore KtGui KtXml KtScript)

set(UITOOLS_PUBLIC_HEADERS
    ${UITOOLS_PUBLIC_HEADERS}
    QUiLoader
)

set(UITOOLS_HEADERS
    ${UITOOLS_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractformbuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/customwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/extension.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilderextra_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formscriptrunner_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/properties_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/quiloader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/quiloader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/resourcebuilder_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textbuilder_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ui4_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/uilib_global.h
)

set(UITOOLS_SOURCES
    ${UITOOLS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/quiloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractformbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extension.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilderextra.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formscriptrunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/textbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui4.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resourcebuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/properties.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}
    # for widgets.table
    ${CMAKE_SOURCE_DIR}/src/shared
)

katie_generate_misc("${UITOOLS_HEADERS}" QtUiTools)
katie_generate_public("${UITOOLS_PUBLIC_HEADERS}" QtUiTools)
katie_generate_package(KtUiTools "KtCore KtGui KtXml KtScript")
katie_setup_target(KtUiTools ${UITOOLS_SOURCES} ${UITOOLS_HEADERS})
katie_setup_paths()

add_library(KtUiTools ${KATIE_TYPE} ${KtUiTools_SOURCES})
target_link_libraries(KtUiTools ${EXTRA_UITOOLS_LIBS})
set_target_properties(KtUiTools PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME UiTools
)

install(
    TARGETS KtUiTools
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtUiTools
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtUiTools)
