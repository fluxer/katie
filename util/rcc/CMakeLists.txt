# add_definitions()
set(EXTRA_RCC_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(RCC_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_target(rcc ${RCC_SOURCES})

add_executable(rcc ${rcc_SOURCES})
target_link_libraries(rcc ${EXTRA_RCC_LIBS})