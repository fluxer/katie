if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusconnection
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusconnection.cpp
    )

    target_link_libraries(tst_qdbusconnection KtDBus)
endif()