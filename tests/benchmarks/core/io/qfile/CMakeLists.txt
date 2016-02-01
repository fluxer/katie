katie_test(tst_bench_qfile
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

if(KATIE_PLATFORM STREQUAL "wince")
    set_target_properties(tst_bench_qfile PROPERTIES PRIVATE
        COMPILE_DEFINITIONS -D_CRT_SECURE_NO_WARNINGS
    )
endif()
