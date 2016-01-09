# add_definitions()
set(EXTRA_UNICODE_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(UNICODE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_resources(${UNICODE_SOURCES})
katie_setup_flags()

add_executable(unicode EXCLUDE_FROM_ALL ${UNICODE_SOURCES})
target_link_libraries(unicode ${EXTRA_UNICODE_LIBS})
