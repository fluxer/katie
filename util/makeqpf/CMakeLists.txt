add_definitions(-DQT_NO_FREETYPE)
set(EXTRA_MAKEQPF_LIBS KtCore KtGui)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
    ${CMAKE_SOURCE_DIR}/src/gui/text
)

set(MAKEQPF_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qpf2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/makeqpf.qrc
)

set(MAKEQPF_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qpf2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
)

katie_resources(${MAKEQPF_SOURCES} ${MAKEQPF_HEADERS})
katie_setup_flags()

add_executable(makeqpf ${MAKEQPF_SOURCES} ${MAKEQPF_HEADERS})
target_link_libraries(makeqpf ${EXTRA_MAKEQPF_LIBS})
