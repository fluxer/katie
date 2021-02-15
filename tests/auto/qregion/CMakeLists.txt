include_directories(${X11_INCLUDE_DIR})

katie_gui_test(tst_qregion
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qregion.cpp
)
