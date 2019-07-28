if(WITH_THREADS AND THREADS_FOUND)
    katie_test(tst_qthread
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qthread.cpp
    )
endif()
