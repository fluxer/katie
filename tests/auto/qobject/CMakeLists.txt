katie_test(tst_qobject
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qobject.cpp
)

target_link_libraries(tst_qobject KtNetwork)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
katie_setup_target(signalbug
    ${CMAKE_CURRENT_SOURCE_DIR}/signalbug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/signalbug.h
)
add_executable(signalbug ${signalbug_SOURCES})

target_link_libraries(signalbug KtCore)
