# add_definitions()
set(EXTRA_PLUGINTEST_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PLUGINTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_target(plugintest ${PLUGINTEST_SOURCES})

add_executable(plugintest ${plugintest_SOURCES})
target_link_libraries(plugintest ${EXTRA_PLUGINTEST_LIBS})
