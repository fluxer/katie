katie_test(tst_qlocale
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qlocale.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_executable(qlocale_syslocaleapp
    ${CMAKE_CURRENT_SOURCE_DIR}/syslocaleapp/syslocaleapp.cpp
)
target_link_libraries(qlocale_syslocaleapp KtCore)