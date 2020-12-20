if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusservicewatcher
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusservicewatcher.cpp
    )
endif()
