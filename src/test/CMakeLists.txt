add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_DATASTREAM
)
set(EXTRA_TEST_LIBS KtCore)

set(TEST_PUBLIC_HEADERS
    ${TEST_PUBLIC_HEADERS}
    QSignalSpy
    QTest
    QTestBasicStreamer
    QTestCoreElement
    QTestCoreList
    QTestData
    QTestElement
    QTestElementAttribute
    QTestEvent
    QTestEventLoop
    QTestFileLogger
    QTestLightXmlStreamer
    QTestXmlStreamer
    QTestXunitStreamer
    # those are aliases
    QtTestGui
)

set(TEST_HEADERS
    ${TEST_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmark.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsignalspy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestaccessible.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestassert.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestbasicstreamer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestcase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestcoreelement.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestcorelist.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestelementattribute.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestelement.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtesteventloop.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestfilelogger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtest_global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtest_gui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestkeyboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestlightxmlstreamer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestmouse.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestspontaneevent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestsystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtesttouch.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestxmlstreamer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestxunitstreamer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkmetric.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestlog_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtesttable_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestresult_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qsignaldumper_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmark_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkmeasurement_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkvalgrind_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkmetric_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkevent_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qabstracttestlogger_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qplaintestlogger_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qxmltestlogger_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestcommon_p.h
)

set(TEST_SOURCES
    ${TEST_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestcase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestlog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtesttable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestresult.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qasciikey.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qplaintestlogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qxmltestlogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qsignaldumper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qabstracttestlogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmark.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkmeasurement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkvalgrind.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkevent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qbenchmarkmetric.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestelement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestelementattribute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestbasicstreamer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestxunitstreamer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestxmlstreamer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestlightxmlstreamer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestlogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtestfilelogger.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtTest
    ${CMAKE_BINARY_DIR}/privateinclude/QtTest
    # for fftw and valgrind
    ${CMAKE_SOURCE_DIR}/src
)

katie_generate_misc("${TEST_HEADERS}" QtTest)
katie_generate_public("${TEST_PUBLIC_HEADERS}" QtTest)
katie_generate_package(KtTest "KtCore")
katie_setup_target(KtTest ${TEST_SOURCES} ${TEST_HEADERS})

# for 3rd party source
if(HAVE_SYS_TIME_H)
    add_definitions(-DTIME_WITH_SYS_TIME=1 -DHAVE_SYS_TIME_H)
else()
    add_definitions(-DTIME_WITH_SYS_TIME=0)
endif()

add_library(KtTest ${KATIE_TYPE} ${KtTest_SOURCES})
target_link_libraries(KtTest ${EXTRA_TEST_LIBS})
set_target_properties(KtTest PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Test
)

install(
    TARGETS KtTest
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtTest
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtTest)
