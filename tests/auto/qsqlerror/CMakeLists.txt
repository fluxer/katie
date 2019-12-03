katie_test(tst_qsqlerror
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqlerror.cpp
)

target_link_libraries(tst_qsqlerror KtSql KtNetwork)
