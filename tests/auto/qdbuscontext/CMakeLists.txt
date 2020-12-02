if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbuscontext
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbuscontext.cpp
    )
endif()