set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qsystemtrayicon.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qcompleter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qcompleter_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qdesktopservices.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qsystemtrayicon_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qundogroup.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qundostack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qundostack_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qundoview.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qsystemtrayicon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qcompleter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qdesktopservices.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qundogroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qundostack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util/qundoview.cpp
)

if(UNIX AND WITH_X11 AND X11_FOUND)
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/util/qsystemtrayicon_x11.cpp
    )
endif()

if(KATIE_PLATFORM STREQUAL "wince")
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/util/qsystemtrayicon_wince.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "win32")
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/util/qsystemtrayicon_win.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "mac")
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/util/qsystemtrayicon_mac.mm
    )
endif()
