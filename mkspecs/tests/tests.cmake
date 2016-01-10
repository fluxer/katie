################################ CPU TESTS ################################

macro(KATIE_CPU_TEST _test _define _flag)
    string(TOUPPER ${_test} uppertest)
    try_compile(
        _has_cpu_feature
        ${CMAKE_BINARY_DIR}/katie_tests/cpu
        ${KATIE_MKSPECS_DIR}/tests/cpu/${_test}.cpp
        COMPILE_DEFINITIONS -m${_flag}
        OUTPUT_VARIABLE _cpu_feature_log
    )
    if(_has_cpu_feature)
        add_definitions(-DQT_HAVE_${_define})
    endif()

    message(STATUS "Host CPU supports ${uppertest}: ${_has_cpu_feature}")
    file(WRITE ${CMAKE_BINARY_DIR}/katie_tests/cpu/${_test}.log "${_cpu_feature_log}")

    set(KATIE_${uppertest}_RESULT ${_has_cpu_feature} CACHE BOOL "Host CPU supports ${uppertest}")
endmacro()

katie_cpu_test(3dnow 3DNOW 3dnow)
katie_cpu_test(avx AVX avx)
katie_cpu_test(mmx MMX mmx)
katie_cpu_test(sse SSE sse)
katie_cpu_test(sse2 SSE2 sse2)
if(KATIE_SSE2_RESULT)
    katie_cpu_test(sse3 SSE3 sse3)
    if(KATIE_SSE3_RESULT)
        katie_cpu_test(ssse3 SSSE3 ssse3)
        if(KATIE_SSSE3_RESULT)
            katie_cpu_test(sse4_1 SSE4_1 sse4.1)
            if(KATIE_SSE4_1_RESULT)
                katie_cpu_test(sse4_2 SSE4_2 sse4.2)
            endif()
        endif()
    endif()
endif()
if("${KATIE_ARCHITECTURE}" STREQUAL "arm")
    katie_cpu_test(iwmmxt IWMMXT cpu=iwmmxt)
    if(NOT KATIE_IWMMXT_RESULT)
        message(SEND_ERROR "Please make sure your compiler supports iWMMXt intrinsics!")
    endif()
    katie_cpu_test(neon NEON -mfpu=neon)
endif()

################################ MISC TESTS ################################

macro(KATIE_MISC_TEST _test _define)
    string(TOUPPER ${_test} uppertest)
    try_compile(
        _has_misc_feature
        ${CMAKE_BINARY_DIR}/katie_tests/misc
        ${KATIE_MKSPECS_DIR}/tests/misc/${_test}.cpp
        OUTPUT_VARIABLE _misc_feature_log
    )
    if(_has_misc_feature)
        add_definitions(-DQT_${_define})
    else()
        add_definitions(-DQT_NO_${_define})
    endif()

    message(STATUS "Host supports ${uppertest}: ${_has_misc_feature}")
    file(WRITE ${CMAKE_BINARY_DIR}/katie_tests/misc/${_test}.log "${_misc_feature_log}")

    set(KATIE_${uppertest}_RESULT ${_has_misc_feature} CACHE BOOL "Host supports ${uppertest}")
endmacro()

katie_misc_test(stl STL)

include(CheckTypeSize)
check_type_size(size_t QT_POINTER_SIZE)