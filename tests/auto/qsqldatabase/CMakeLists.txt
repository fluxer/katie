katie_test(tst_qsqldatabase
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqldatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_databases.h
)

target_link_libraries(tst_qsqldatabase KtSql KtNetwork)