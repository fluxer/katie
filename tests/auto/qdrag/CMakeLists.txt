if(X11_Xext_FOUND AND X11_Xshape_FOUND)
    include_directories(${X11_INCLUDE_DIR})

    katie_gui_test(tst_qdrag
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdrag.cpp
    )

    target_link_libraries(tst_qdrag ${X11_Xshape_LIB} ${X11_Xext_LIB})
endif()

