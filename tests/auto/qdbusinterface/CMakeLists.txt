if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbusinterface
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusinterface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/myobject.h
    )

    target_link_libraries(tst_qdbusinterface KtDBus)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    katie_setup_target(qmyserver2
        ${CMAKE_CURRENT_SOURCE_DIR}/qmyserver/qmyserver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/myobject.h
    )
    add_executable(qmyserver2 ${qmyserver2_SOURCES})

    target_link_libraries(qmyserver2 KtCore KtDBus)
endif()