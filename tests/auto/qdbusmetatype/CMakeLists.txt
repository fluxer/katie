if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusmetatype
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusmetatype.cpp
    )

    target_link_libraries(tst_qdbusmetatype KtDBus)
endif()
