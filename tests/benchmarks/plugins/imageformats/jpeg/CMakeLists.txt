if(WITH_JPEG AND JPEG_FOUND)
    katie_test(tst_bench_jpeg
        ${CMAKE_CURRENT_SOURCE_DIR}/jpeg.cpp
    )

    target_link_libraries(tst_bench_jpeg KtGui)
endif()
