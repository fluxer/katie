katie_gui_test(tst_qsvggenerator
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsvggenerator.cpp
)

target_link_libraries(tst_qsvggenerator KtXml KtSvg)
