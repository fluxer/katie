katie_test(tst_qprocess
    ${CMAKE_CURRENT_SOURCE_DIR}/tst_qprocess.cpp
)

set(QPROCESS_TEST_EXECUTABLES
    fileWriterProcess
    testDetached
    testExitCodes
    testProcessCrash
    testProcessDeadWhileReading
    testProcessEcho
    testProcessEcho2
    testProcessEcho3
    testProcessEnvironment
    testProcessEOF
    testProcessLoopback
    testProcessNormal
    testProcessOutput
    testSetWorkingDirectory
    testSoftExit
    testProcessSpacesArgs
    testGuiProcess
)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
foreach(qprocess_test ${QPROCESS_TEST_EXECUTABLES})
    katie_setup_target(qprocess_${qprocess_test}
        ${CMAKE_CURRENT_SOURCE_DIR}/${qprocess_test}/main.cpp
    )
    add_executable(qprocess_${qprocess_test} ${qprocess_${qprocess_test}_SOURCES})
    target_link_libraries(qprocess_${qprocess_test} KtCore)
endforeach()

target_link_libraries(qprocess_testGuiProcess KtCore KtGui)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/test Space In Name")
katie_setup_target(qprocess_testSpaceInName
    ${CMAKE_CURRENT_SOURCE_DIR}/testSpaceInName/main.cpp
)
add_executable(qprocess_testSpaceInName ${qprocess_testProcessSpacesArgs_SOURCES})
target_link_libraries(qprocess_testSpaceInName KtCore)
