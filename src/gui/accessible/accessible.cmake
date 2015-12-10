if(WITH_ACCESSIBILITY)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible.h
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible2.h
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessibleobject.h
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessiblewidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessibleplugin.h
    )

    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessibleobject.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessiblewidget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessibleplugin.cpp
    )

    if(KATIE_PLATFORM STREQUAL "mac")
        set(GUI_HEADERS
            ${GUI_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible_mac_p.h
        )
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible_mac.mm
            ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible_mac_cocoa.mm
        )
    elseif(KATIE_PLATFORM STREQUAL "win32")
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible_win.cpp
        )
    else()
        set(GUI_HEADERS
            ${GUI_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessiblebridge.h
        )
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessible_unix.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/accessible/qaccessiblebridge.cpp
        )
    endif()
else()
    # FIXME: move to global scope
    add_definitions(-DQT_NO_ACCESSIBILITY)
endif()