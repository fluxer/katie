katie_test(tst_qnetworkinterface
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qnetworkinterface.cpp
)

target_link_libraries(tst_qnetworkinterface KtNetwork)
