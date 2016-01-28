# add_definitions()
set(EXTRA_GENERATERUN_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GENERATERUN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codegenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codegenerator.h
)

katie_resources(${GENERATERUN_SOURCES})
katie_setup_flags()

add_executable(generaterun EXCLUDE_FROM_ALL ${GENERATERUN_SOURCES})
target_link_libraries(generaterun ${EXTRA_GENERATERUN_LIBS})
