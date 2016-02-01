katie_test(tst_bench_qsqlquery
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(tst_bench_qsqlquery KtSql)
if(KATIE_PLATFORM STREQUAL "win32")
    target_link_libraries(tst_bench_qsqlquery Ws2_32)
endif()
