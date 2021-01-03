katie_test(tst_vector
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/outofline.cpp
)

katie_unity_exclude(
    # both set N as iterations variable
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/outofline.cpp
)
