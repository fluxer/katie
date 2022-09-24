if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusmetaobject
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusmetaobject.cpp
    )

    target_link_libraries(tst_qdbusmetaobject KtDBus)
endif()
