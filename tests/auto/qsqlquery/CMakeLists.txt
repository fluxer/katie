katie_test(tst_qsqlquery
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqlquery.cpp
)

target_link_libraries(tst_qsqlquery KtSql KtNetwork)
