katie_test(tst_qsocketnotifier
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qsocketnotifier.cpp
)

target_link_libraries(tst_qsocketnotifier ${SOCKET_AND_NSL_LIBRARIES})
