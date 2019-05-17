katie_test(tst_bench_qnetworkdiskcache
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qnetworkdiskcache.cpp
)

target_link_libraries(tst_bench_qnetworkdiskcache KtNetwork KtGui)
