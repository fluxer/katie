katie_gui_test(tst_qsqlquerymodel
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsqlquerymodel.cpp
)

target_link_libraries(tst_qsqlquerymodel KtSql KtNetwork)
