katie_test(tst_quuid
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_quuid.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_executable(quuid_testProcessUniqueness
    ${CMAKE_CURRENT_SOURCE_DIR}/testProcessUniqueness/main.cpp
)
target_link_libraries(quuid_testProcessUniqueness KtCore)
