# add_definitions()
set(EXTRA_UITOOLS_LIBS KtCore KtGui KtXml KtScript)

set(UITOOLS_PUBLIC_HEADERS
    QUiLoader
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    # for widgets.table
    ${CMAKE_SOURCE_DIR}/src/shared
    # for ui4.h
    ${CMAKE_SOURCE_DIR}/src/tools/uic
)

set(UITOOLS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractformbuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/customwidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilderextra_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/formscriptrunner_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/properties_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/quiloader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/quiloader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/resourcebuilder_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/textbuilder_p.h
)

set(UITOOLS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/quiloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/abstractformbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formbuilderextra.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/formscriptrunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/textbuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resourcebuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/properties.cpp
    ${CMAKE_SOURCE_DIR}/src/tools/uic/ui4.cpp
)

katie_generate_misc("${UITOOLS_HEADERS}" QtUiTools)
katie_generate_public("${UITOOLS_PUBLIC_HEADERS}" QtUiTools)
katie_generate_package(KtUiTools "KtCore KtGui KtXml KtScript")
katie_setup_target(KtUiTools ${UITOOLS_SOURCES} ${UITOOLS_HEADERS})

add_library(KtUiTools ${KATIE_TYPE} ${KtUiTools_SOURCES})
target_link_libraries(KtUiTools PRIVATE ${EXTRA_UITOOLS_LIBS})
set_target_properties(KtUiTools PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME UiTools
)

install(
    TARGETS KtUiTools
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtUiTools
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtUiTools)
