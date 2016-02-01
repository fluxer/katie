katie_test(tst_binding
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_binding.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testtypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testtypes.h
)

target_link_libraries(tst_binding KtDeclarative)
