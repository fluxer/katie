add_definitions(-DQT_BUILD_SCRIPTTOOLS_LIB)
set(EXTRA_SCRIPTTOOLS_LIBS KtCore KtGui KtScript)

set(SCRIPTTOOLS_PUBLIC_HEADERS
    ${SSCRIPTTOOLS_PUBLIC_HEADERS}
    QScriptEngineDebugger
)

include(debugging/debugging.cmake)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/debugging
    ${CMAKE_CURRENT_BINARY_DIR}/debugging
)

katie_generate_misc("${SCRIPTTOOLS_HEADERS}" QtScriptTools)
katie_generate_public("${SCRIPTTOOLS_PUBLIC_HEADERS}" QtScriptTools)
katie_generate_map(QtScriptTools Q_SCRIPTTOOLS_EXPORT)
katie_resources(${SCRIPTTOOLS_SOURCES})
katie_resources(${SCRIPTTOOLS_HEADERS})
katie_setup_flags()

add_library(KtScriptTools ${KATIE_TYPE} ${SCRIPTTOOLS_SOURCES} ${SCRIPTTOOLS_HEADERS})
target_link_libraries(KtScriptTools ${EXTRA_SCRIPTTOOLS_LIBS})
set_target_properties(KtScriptTools PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME ScriptTools
)

katie_generate_package(
    KtScriptTools
    ""
    "${EXTRA_SCRIPTTOOLS_LIBS}"
    "KtCore KtGui KtScript"
)

install(
    TARGETS KtScriptTools
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH_INST}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtScriptTools
    DESTINATION ${QT_HEADERS_PATH_INST}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtScriptTools)
