if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusinterface
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusinterface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/myobject.h
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    katie_setup_target(qdbusinterface_qmyserver
        ${CMAKE_CURRENT_SOURCE_DIR}/qmyserver/qmyserver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/myobject.h
    )
    add_executable(qdbusinterface_qmyserver ${qdbusinterface_qmyserver_SOURCES})

    target_link_libraries(qdbusinterface_qmyserver KtCore KtDBus)
endif()