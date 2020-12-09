if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbusabstractinterface
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusabstractinterface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/interface.h
        ${CMAKE_CURRENT_SOURCE_DIR}/pinger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/pinger.h
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    katie_setup_target(qdbusabstractinterface_qpinger
        ${CMAKE_CURRENT_SOURCE_DIR}/qpinger/qpinger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/interface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/interface.h
    )
    add_executable(qdbusabstractinterface_qpinger ${qdbusabstractinterface_qpinger_SOURCES})

    target_link_libraries(qdbusabstractinterface_qpinger KtCore KtDBus)
endif()