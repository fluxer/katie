katie_test(tst_bench_sunspider
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_sunspider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sunspider.qrc
)

target_link_libraries(tst_bench_sunspider KtScript)
