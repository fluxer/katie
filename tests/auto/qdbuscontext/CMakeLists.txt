if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbuscontext
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbuscontext.cpp
    )

    target_link_libraries(tst_qdbuscontext KtDBus)
endif()