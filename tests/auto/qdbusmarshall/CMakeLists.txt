if(WITH_DBUS AND DBUS_FOUND)
    include_directories(${DBUS_INCLUDES})

    katie_dbus_test(tst_qdbusmarshall
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbusmarshall.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    )

    target_link_libraries(tst_qdbusmarshall KtDBus ${DBUS_LIBRARIES})

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    katie_setup_target(qpong
        ${CMAKE_CURRENT_SOURCE_DIR}/qpong/qpong.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/common.h
    )
    add_executable(qpong ${qpong_SOURCES})

    target_link_libraries(qpong KtCore KtDBus)
endif()