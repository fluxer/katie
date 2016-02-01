katie_test(tst_bench_qobject
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/object.h
)

target_link_libraries(tst_bench_qobject KtGui)
