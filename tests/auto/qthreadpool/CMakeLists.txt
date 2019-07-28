if(WITH_THREADS AND THREADS_FOUND)
    katie_test(tst_qthreadpool
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qthreadpool.cpp
    )
endif()
