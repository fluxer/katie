katie_test(tst_qfile
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qfile.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
katie_setup_target(qfile_stdinprocess
    ${CMAKE_CURRENT_SOURCE_DIR}/stdinprocess/main.cpp
)
add_executable(qfile_stdinprocess ${qfile_stdinprocess_SOURCES})

target_link_libraries(qfile_stdinprocess KtCore)