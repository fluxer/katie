katie_test(tst_bench_qgraphicsview
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qgraphicsview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chiptester/chiptester.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chiptester/chip.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/chiptester/chiptester.h
    ${CMAKE_CURRENT_SOURCE_DIR}/chiptester/chip.h
)

set(test_resources
    ${CMAKE_CURRENT_SOURCE_DIR}/chiptester/images.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/qgraphicsview.qrc
)
katie_resource(
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qgraphicsview.cpp
    "${test_resources}"
    tst_qgraphicsview
)

if(WITH_OPENGL AND OPENGL_FOUND)
    target_link_libraries(tst_bench_qgraphicsview KtOpenGL)
endif()