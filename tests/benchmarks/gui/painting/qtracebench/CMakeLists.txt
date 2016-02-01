katie_test(tst_qtracebench
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qtracebench.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtracebench.qrc
)

if(WITH_HARFBUZZ AND HARFBUZZ_FOUND)
    target_include_directories(tst_qtracebench PRIVATE ${HARFBUZZ_INCLUDE_DIRS})
else()
    target_include_directories(tst_qtracebench PRIVATE ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src)
endif()