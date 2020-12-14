if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusthreading
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusthreading.cpp
    )
endif()
