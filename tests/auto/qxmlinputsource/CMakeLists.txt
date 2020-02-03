katie_test(tst_qxmlinputsource
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qxmlinputsource.cpp
)

target_link_libraries(tst_qxmlinputsource KtXml KtNetwork)
