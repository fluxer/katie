# TODO: move QT_NEEDS_QMAIN to global scope?

add_definitions(-DQT_NEEDS_QMAIN)
set(EXTRA_QTMAIN_LIBS)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${PNG_INCLUDE_DIRS}
)

set(QTMAIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qtmain_win.cpp
)

katie_resources("${QTMAIN_SOURCES}")
katie_setup_flags()

add_library(ktmain ${QTMAIN_SOURCES} ${QTMAIN_HEADERS})
target_link_libraries(ktmain ${EXTRA_QTMAIN_LIBS})

install(
    TARGETS ktmain
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH_INST}
)
