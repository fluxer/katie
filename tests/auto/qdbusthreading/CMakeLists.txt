if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusthreading
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusthreading.cpp
    )

    target_link_libraries(tst_qdbusthreading KtDBus)
endif()
