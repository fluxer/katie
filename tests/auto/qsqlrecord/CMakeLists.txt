katie_test(tst_qsqlrecord
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqlrecord.cpp
)

target_link_libraries(tst_qsqlrecord KtSql KtNetwork)
