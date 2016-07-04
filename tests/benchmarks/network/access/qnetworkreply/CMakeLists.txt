katie_test(tst_bench_qnetworkreply
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qnetworkreply.cpp
)

target_link_libraries(tst_bench_qnetworkreply KtNetwork)

# FIXME: not an ideal sulution
set_target_properties(tst_bench_qnetworkreply PROPERTIES TIMEOUT 120)
