katie_test(tst_qsql
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsql.cpp
)

target_link_libraries(tst_qsql KtSql KtNetwork)
