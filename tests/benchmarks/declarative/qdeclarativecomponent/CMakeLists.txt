katie_test(tst_qdeclarativecomponent
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdeclarativecomponent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testtypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testtypes.h
)

target_link_libraries(tst_qdeclarativecomponent KtDeclarative)
