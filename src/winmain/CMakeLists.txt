set(EXTRA_QTMAIN_LIBS)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${PNG_INCLUDE_DIRS}
)

set(MAIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qtmain_win.cpp
)

katie_resources(${MAIN_SOURCES})
katie_setup_flags()

add_library(KtMain ${MAIN_SOURCES} ${MAIN_HEADERS})
target_link_libraries(KtMain ${EXTRA_MAIN_LIBS})
set_target_properties(KtMain PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Main
)

install(
    TARGETS KtMain
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH_INST}
)
