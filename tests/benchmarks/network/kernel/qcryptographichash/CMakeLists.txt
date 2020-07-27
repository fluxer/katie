katie_test(tst_bench_qcryptographichash
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(tst_bench_qcryptographichash KtNetwork)
