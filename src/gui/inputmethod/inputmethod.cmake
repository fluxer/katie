set(GUI_HEADERS
    ${GUI_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qinputcontextfactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qinputcontextplugin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qinputcontext_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qinputcontext.h
)

set(GUI_SOURCES
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qinputcontextfactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qinputcontextplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qinputcontext.cpp
)

if(WITH_X11 AND X11_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qximinputcontext_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qximinputcontext_x11.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "win32")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qwininputcontext_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qwininputcontext_win.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        imm32
    )
elseif(KATIE_PLATFORM STREQUAL "mac")
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qmacinputcontext_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/inputmethod/qmacinputcontext_mac.cpp
    )
endif()
