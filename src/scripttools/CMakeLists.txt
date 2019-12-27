# add_definitions()
set(EXTRA_SCRIPTTOOLS_LIBS
    KtCore KtGui KtScript
)

set(SCRIPTTOOLS_PUBLIC_HEADERS
    QScriptEngineDebugger
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtScriptTools
    ${CMAKE_BINARY_DIR}/privateinclude/QtScriptTools
)

include(debugging/debugging.cmake)

set(SCRIPTTOOLS_HEADERS
    ${SCRIPTTOOLS_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qscripttoolscommon_p.h
)

katie_generate_misc("${SCRIPTTOOLS_HEADERS}" QtScriptTools)
katie_generate_public("${SCRIPTTOOLS_PUBLIC_HEADERS}" QtScriptTools)
katie_generate_package(KtScriptTools "KtCore KtGui KtScript")
katie_setup_target(KtScriptTools ${SCRIPTTOOLS_SOURCES} ${SCRIPTTOOLS_HEADERS})

add_library(KtScriptTools ${KATIE_TYPE} ${KtScriptTools_SOURCES})
target_link_libraries(KtScriptTools PRIVATE ${EXTRA_SCRIPTTOOLS_LIBS})
set_target_properties(KtScriptTools PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME ScriptTools
)

install(
    TARGETS KtScriptTools
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtScriptTools
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtScriptTools)
