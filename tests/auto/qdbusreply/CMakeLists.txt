if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusreply
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusreply.cpp
    )
endif()
