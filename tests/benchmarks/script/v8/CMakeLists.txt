katie_test(tst_bench_v8
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_v8.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/v8.qrc
)

target_link_libraries(tst_bench_v8 KtScript)
