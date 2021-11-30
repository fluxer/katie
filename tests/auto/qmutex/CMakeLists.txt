katie_test(tst_qmutex
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qmutex.cpp
)

# possible pthread reference from the global static mutex
target_link_libraries(tst_qmutex ${CMAKE_THREAD_LIBS_INIT})
