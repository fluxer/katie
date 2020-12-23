katie_test(tst_qxmlsimplereader
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qxmlsimplereader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/parser.h
)

target_link_libraries(tst_qxmlsimplereader KtXml KtNetwork)
