if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusxmlparser
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusxmlparser.cpp
    )

    target_link_libraries(tst_qdbusxmlparser KtDBus)
endif()
