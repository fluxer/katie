# add_definitions()
set(EXTRA_QDBUS_LIBS KtCore KtXml KtDBus)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/privateinclude/QtDBus
)

katie_setup_paths()

add_executable(qdbus ${CMAKE_CURRENT_SOURCE_DIR}/qdbus.cpp)
target_link_libraries(qdbus ${EXTRA_QDBUS_LIBS})

add_executable(qdbuscpp2xml ${CMAKE_CURRENT_SOURCE_DIR}/qdbuscpp2xml.cpp)
target_link_libraries(qdbuscpp2xml ${EXTRA_QDBUS_LIBS})

add_executable(qdbusxml2cpp ${CMAKE_CURRENT_SOURCE_DIR}/qdbusxml2cpp.cpp)
target_link_libraries(qdbusxml2cpp ${EXTRA_QDBUS_LIBS})

install(
    TARGETS qdbus qdbuscpp2xml qdbusxml2cpp
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)

install(
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbus.1
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuscpp2xml.1
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusxml2cpp.1
    DESTINATION ${MAN_INSTALL_PATH}/man1
)
