# add_definitions()
set(EXTRA_FIXONLATIN1_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(FIXONLATIN1_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_target(fixonlatin1 ${FIXONLATIN1_SOURCES})

add_executable(fixonlatin1 ${fixonlatin1_SOURCES})
target_link_libraries(fixonlatin1 ${EXTRA_FIXONLATIN1_LIBS})
