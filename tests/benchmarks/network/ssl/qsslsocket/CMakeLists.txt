katie_test(tst_bench_qsslsocket
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsslsocket.cpp
)

target_link_libraries(tst_bench_qsslsocket KtNetwork)
