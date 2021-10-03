set(EXTRA_UIC_LIBS
    KtCore KtXml
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
)

set(UIC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/uicmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/customwidgetsinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/treewalker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui4.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwritedeclaration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteicondata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteicondeclaration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteiconinitialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteincludes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteinitialization.cpp
)

katie_setup_target(uic ${UIC_SOURCES})

add_executable(uic ${uic_SOURCES})
target_link_libraries(uic ${EXTRA_UIC_LIBS})

set_target_properties(uic PROPERTIES
    EXPORT_NAME uic
    OUTPUT_NAME uic${KATIE_TOOLS_SUFFIX}
)

install(
    TARGETS uic
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${KATIE_BINARIES_PATH}
    COMPONENT Tools
)

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/uic.1
    DESTINATION ${KATIE_MAN_PATH}/man1
    RENAME uic${KATIE_TOOLS_SUFFIX}.1
    COMPONENT Doc
)
