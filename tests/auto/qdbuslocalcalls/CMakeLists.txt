if(WITH_DBUS AND DBUS_FOUND)
    katie_test(tst_qdbuslocalcalls
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_qdbuslocalcalls.cpp
    )

    target_link_libraries(tst_qdbuslocalcalls KtDBus)
endif()