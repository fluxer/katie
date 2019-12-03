katie_test(tst_qsqlrelationaltablemodel
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqlrelationaltablemodel.cpp
)

target_link_libraries(tst_qsqlrelationaltablemodel KtSql KtNetwork)
