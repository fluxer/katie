if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbuspendingcall
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbuspendingcall.cpp
    )
endif()
