if(WITH_DBUS AND DBUS_FOUND)
    include_directories(${DBUS_INCLUDES})

    katie_test(tst_bench_qdbustype
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

    target_link_libraries(tst_bench_qdbustype KtDBus ${DBUS_LIBRARIES})
endif()
