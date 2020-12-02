if(WITH_DBUS AND DBUS_FOUND)
    include_directories(${DBUS_INCLUDES})

    katie_dbus_test(tst_qdbustype
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbustype.cpp
    )

    target_link_libraries(tst_qdbustype ${DBUS_LIBRARIES})
endif()
