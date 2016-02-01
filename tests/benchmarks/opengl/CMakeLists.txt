if(WITH_OPENGL AND OPENGL_FOUND)
    katie_test(tst_bench_opengl
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

    target_link_libraries(tst_bench_opengl KtOpenGL)
endif()
