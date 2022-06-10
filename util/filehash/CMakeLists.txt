# add_definitions()
set(EXTRA_FILEHASH_LIBS KtCore KtNetwork)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(FILEHASH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_target(filehash ${FILEHASH_SOURCES})

add_executable(filehash ${filehash_SOURCES})
target_link_libraries(filehash ${EXTRA_FILEHASH_LIBS})
