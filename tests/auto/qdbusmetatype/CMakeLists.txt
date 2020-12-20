if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusmetatype
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusmetatype.cpp
    )
endif()
