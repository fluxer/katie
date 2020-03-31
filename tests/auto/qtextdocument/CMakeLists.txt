katie_gui_test(tst_qtextdocument
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qtextdocument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h
)

target_link_libraries(tst_qtextdocument KtXml)
