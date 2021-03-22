katie_test(tst_qcryptographichash
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qcryptographichash.cpp
)

target_link_libraries(tst_qcryptographichash KtNetwork)
