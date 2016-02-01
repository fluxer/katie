if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusperformance
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusperformance.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/serverobject.h
    )

    target_link_libraries(tst_qdbusperformance KtDBus)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_executable(server
        ${CMAKE_CURRENT_SOURCE_DIR}/server/server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/serverobject.h
    )

    target_link_libraries(server KtDBus)
endif()