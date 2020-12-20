# add_definitions()
set(EXTRA_QDBUS_LIBS
    KtCore KtXml KtDBus
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/include/QtDBus
)

add_executable(qdbus ${CMAKE_CURRENT_SOURCE_DIR}/qdbus.cpp)
target_link_libraries(qdbus ${EXTRA_QDBUS_LIBS})

set_target_properties(qdbus PROPERTIES
    EXPORT_NAME qdbus
    OUTPUT_NAME qdbus${KATIE_TOOLS_SUFFIX}
)

add_executable(qdbuscpp2xml ${CMAKE_CURRENT_SOURCE_DIR}/qdbuscpp2xml.cpp)
target_link_libraries(qdbuscpp2xml ${EXTRA_QDBUS_LIBS})

set_target_properties(qdbuscpp2xml PROPERTIES
    EXPORT_NAME qdbuscpp2xml
    OUTPUT_NAME qdbuscpp2xml${KATIE_TOOLS_SUFFIX}
)

add_executable(qdbusxml2cpp ${CMAKE_CURRENT_SOURCE_DIR}/qdbusxml2cpp.cpp)
target_link_libraries(qdbusxml2cpp ${EXTRA_QDBUS_LIBS})

set_target_properties(qdbusxml2cpp PROPERTIES
    EXPORT_NAME qdbusxml2cpp
    OUTPUT_NAME qdbusxml2cpp${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS qdbus qdbuscpp2xml qdbusxml2cpp
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qdbus.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME qdbus${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qdbuscpp2xml.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME qdbuscpp2xml${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/qdbusxml2cpp.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME qdbusxml2cpp${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
