if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_bench_qdbustype
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

    target_link_libraries(tst_bench_qdbustype ${DBUS_LIBRARIES})
    include_directories(${DBUS_INCLUDES})
endif()
