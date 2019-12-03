katie_test(tst_qsqltablemodel
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqltablemodel.cpp
)

target_link_libraries(tst_qsqltablemodel KtSql KtNetwork)
