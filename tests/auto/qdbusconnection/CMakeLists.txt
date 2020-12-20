if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusconnection
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusconnection.cpp
    )
endif()