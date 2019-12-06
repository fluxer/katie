katie_test(tst_qsqlfield
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqlfield.cpp
)

target_link_libraries(tst_qsqlfield KtSql KtNetwork)
