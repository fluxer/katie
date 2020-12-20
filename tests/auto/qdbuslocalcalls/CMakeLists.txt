if(WITH_DBUS AND DBUS_FOUND)
    katie_dbus_test(tst_qdbuslocalcalls
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbuslocalcalls.cpp
    )
endif()