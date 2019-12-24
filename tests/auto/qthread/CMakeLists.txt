katie_test(tst_qthread
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qthread.cpp
)

target_link_libraries(tst_qthread ${CMAKE_THREAD_LIBS_INIT})
