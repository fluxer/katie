if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbuspendingcall
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbuspendingcall.cpp
    )

    target_link_libraries(tst_qdbuspendingcall KtDBus)
endif()
