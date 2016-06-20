set(BLACKBERRYPLUGIN_DEFINITIONS
    ${BLACKBERRYPLUGIN_DEFINITIONS}
    # Unomment this to enable screen event handling through a dedicated thread.
    # -DQQNX_SCREENEVENTTHREAD
    -DQBB_SCREENEVENTTHREAD
)

set(BLACKBERRYPLUGIN_INCLUDES
    ${BLACKBERRYPLUGIN_INCLUDES}
    ${EGL_INCLUDES}
)

set(BLACKBERRYPLUGIN_HEADERS
    ${QTRACEPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbbuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbscreeneventthread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbinputcontext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbintegration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbnavigatoreventhandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbnavigatoreventnotifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbglcontext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbglwindowsurface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbscreen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbrasterwindowsurface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbvirtualkeyboardpps.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbclipboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbblocalethread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbrootwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbscreeneventhandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbabstractvirtualkeyboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbnativeinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbbuttoneventnotifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbcursor.h
)

set(BLACKBERRYPLUGIN_SOURCES
    ${BLACKBERRYPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/blackberryplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbbuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbscreeneventthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbglcontext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbglwindowsurface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbinputcontext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbintegration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbnavigatoreventhandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbnavigatoreventnotifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbscreen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbrasterwindowsurface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbvirtualkeyboardpps.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbclipboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbblocalethread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbrootwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbscreeneventhandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbabstractvirtualkeyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbnativeinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbbuttoneventnotifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbcursor.cpp
)

if(${KATIE_PLATFORM} STREQUAL "blackberry")
    set(BLACKBERRYPLUGIN_HEADERS
        ${BLACKBERRYPLUGIN_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbbpseventfilter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbvirtualkeyboardbps.h
    )
    set(BLACKBERRYPLUGIN_SOURCES
        ${BLACKBERRYPLUGIN_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbbpseventfilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/platforms/blackberry/qbbvirtualkeyboardbps.cpp
    )
endif()

katie_resources(${BLACKBERRYPLUGIN_SOURCES} ${BLACKBERRYPLUGIN_HEADERS})

add_library(blackberryplugin MODULE ${BLACKBERRYPLUGIN_SOURCES} ${BLACKBERRYPLUGIN_HEADERS})
set_target_properties(blackberryplugin PROPERTIES OUTPUT_NAME blackberry)
target_link_libraries(blackberryplugin pps screen clipboard ${EGL_LIBRARIES})
target_compile_definitions(blackberryplugin ${BLACKBERRYPLUGIN_DEFINITIONS})
target_include_directories(blackberryplugin PRIVATE ${BLACKBERRYPLUGIN_INCLUDES})

install(
    TARGETS blackberryplugin
    DESTINATION ${QT_PLUGINS_PATH}/platforms
)
