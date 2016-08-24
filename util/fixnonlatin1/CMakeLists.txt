# add_definitions()
set(EXTRA_FIXONLATIN1_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(FIXONLATIN1_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(fixonlatin1 EXCLUDE_FROM_ALL ${FIXONLATIN1_SOURCES})
target_link_libraries(fixonlatin1 ${EXTRA_FIXONLATIN1_LIBS})

katie_setup_target(fixonlatin1)
