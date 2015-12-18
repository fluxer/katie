# add_definitions()
set(EXTRA_XKBDATAGEN_LIBS KtCore KtXml)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(XKBDATAGEN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_resources(${XKBDATAGEN_SOURCES})
katie_setup_flags()

add_executable(xkbdatagen ${XKBDATAGEN_SOURCES})
target_link_libraries(xkbdatagen ${EXTRA_XKBDATAGEN_LIBS})
