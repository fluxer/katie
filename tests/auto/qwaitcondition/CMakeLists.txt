if(WITH_THREADS AND THREADS_FOUND)
    katie_test(tst_qwaitcondition
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qwaitcondition.cpp
    )
endif()
