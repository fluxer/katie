if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusservicewatcher
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusservicewatcher.cpp
    )

    target_link_libraries(tst_qdbusservicewatcher KtDBus)
endif()
