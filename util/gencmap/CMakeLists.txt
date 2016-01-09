# add_definitions()
set(EXTRA_GENCMAP_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GENCMAP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gencmap.cpp
)

katie_resources(${GENCMAP_SOURCES})
katie_setup_flags()

add_executable(gencmap EXCLUDE_FROM_ALL ${GENCMAP_SOURCES})
target_link_libraries(gencmap ${EXTRA_GENCMAP_LIBS})
