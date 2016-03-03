if(WITH_EGL AND EGL_FOUND)
    set(GUI_HEADERS
        ${GUI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qegl_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qeglcontext_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qeglproperties_p.h
    )
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qegl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qeglproperties.cpp
    )
    if(UNIX)
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/egl/qegl_x11.cpp
        )
    elseif(KATIE_PLATFORM STREQUAL "wince")
        set(GUI_SOURCES
            ${GUI_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/egl/qegl_wince.cpp
        )
    endif()
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${EGL_LIBRARIES}
    )
    include_directories(${EGL_INCLUDES})
else()
    set(GUI_SOURCES
        ${GUI_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qegl_stub.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qeglproperties_stub.cpp
    )
    add_definitions(-DQT_NO_EGL)
endif()

