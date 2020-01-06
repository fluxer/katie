katie_test(tst_qiodevice
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qiodevice.cpp
)

target_link_libraries(tst_qiodevice KtNetwork)
