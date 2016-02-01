katie_test(tst_bench_qsvgrenderer
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsvgrenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsvgrenderer.qrc
)

target_link_libraries(tst_bench_qsvgrenderer KtSvg)
