add_definitions(-DQT_UIC)
set(EXTRA_UIC_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp
)

set(UIC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/uicmain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/customwidgetsinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/databaseinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/treewalker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui4.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppextractimages.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwritedeclaration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteicondata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteicondeclaration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteiconinitialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteincludes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/cppwriteinitialization.cpp
)

katie_setup_flags()

add_executable(uic ${UIC_SOURCES})
target_link_libraries(uic ${EXTRA_UIC_LIBS})

install(
    TARGETS uic
    EXPORT KatieBinaryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    RUNTIME DESTINATION ${QT_BINARIES_PATH}
    COMPONENT Devel
)
