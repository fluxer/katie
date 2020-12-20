# add_definitions()
set(EXTRA_QTSCRIPT_LIBS
    KtCore KtScript KtGui KtScriptTools
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtScriptTools
)

add_executable(qscript ${CMAKE_CURRENT_SOURCE_DIR}/qscript.cpp)
target_link_libraries(qscript ${EXTRA_QTSCRIPT_LIBS})

set_target_properties(qscript PROPERTIES
    EXPORT_NAME qscript
    OUTPUT_NAME qscript${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS qscript
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/qscript.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME qscript${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
