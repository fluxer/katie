if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusabstractadaptor
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusabstractadaptor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/myobject.h
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    katie_setup_target(qdbusabstractadaptor_qmyserver
        ${CMAKE_CURRENT_SOURCE_DIR}/qmyserver/qmyserver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/myobject.h
    )
    add_executable(qdbusabstractadaptor_qmyserver ${qdbusabstractadaptor_qmyserver_SOURCES})

    target_link_libraries(qdbusabstractadaptor_qmyserver KtCore KtDBus)
endif()