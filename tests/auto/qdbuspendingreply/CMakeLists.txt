if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbuspendingreply
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbuspendingreply.cpp
    )
endif()
