katie_test(tst_qsqlthread
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqlthread.cpp
)

target_link_libraries(tst_qsqlthread KtSql KtNetwork)
