if(WITH_THREADS AND THREADS_FOUND)
    katie_test(tst_qmutex
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qmutex.cpp
    )
endif()
