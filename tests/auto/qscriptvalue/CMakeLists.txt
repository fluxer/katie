katie_gui_test(tst_qscriptvalue
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qscriptvalue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qscriptvalue.h
)

target_link_libraries(tst_qscriptvalue KtScript)
