katie_test(tst_qeventloop
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qeventloop.cpp
)

target_link_libraries(tst_qeventloop KtNetwork)
