if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusreply
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusreply.cpp
    )

    target_link_libraries(tst_qdbusreply KtDBus)
endif()
