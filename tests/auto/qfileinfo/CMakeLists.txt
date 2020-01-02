katie_test(tst_qfileinfo
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qfileinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qfileinfo.qrc
)

target_link_libraries(tst_qfileinfo KtNetwork)
