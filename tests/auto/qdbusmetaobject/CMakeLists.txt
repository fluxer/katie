if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusmetaobject
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusmetaobject.cpp
    )
endif()
