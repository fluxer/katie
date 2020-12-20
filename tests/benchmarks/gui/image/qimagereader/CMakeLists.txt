katie_gui_test(tst_bench_qimagereader
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qimagereader.cpp
)

if(WITH_JPEG AND JPEG_FOUND)
    target_compile_definitions(tst_bench_qimagereader PRIVATE -DQTEST_HAVE_JPEG)
endif()
if(WITH_TIFF AND TIFF_FOUND)
    target_compile_definitions(tst_bench_qimagereader PRIVATE -DQTEST_HAVE_TIFF)
endif()
