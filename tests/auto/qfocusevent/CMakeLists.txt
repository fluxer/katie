include_directories(${X11_INCLUDE_DIR})

katie_gui_test(tst_qfocusevent
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qfocusevent.cpp
)

target_link_libraries(tst_qfocusevent ${X11_X11_LIB})
