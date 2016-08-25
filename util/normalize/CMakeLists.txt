# add_definitions()
set(EXTRA_NORMALIZE_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(NORMALIZE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_target(normalize ${NORMALIZE_SOURCES})

add_executable(normalize EXCLUDE_FROM_ALL ${normalize_SOURCES})
target_link_libraries(normalize ${EXTRA_NORMALIZE_LIBS})
