if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusperformance
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusperformance.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/serverobject.h
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    katie_setup_target(performance_server
        ${CMAKE_CURRENT_SOURCE_DIR}/server/server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/serverobject.h
    )
    add_executable(performance_server ${performance_server_SOURCES})

    target_link_libraries(performance_server KtCore KtDBus)
endif()