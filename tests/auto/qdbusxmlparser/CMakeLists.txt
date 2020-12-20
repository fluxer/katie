if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusxmlparser
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusxmlparser.cpp
    )
endif()
