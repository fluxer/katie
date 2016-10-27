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
        ${CMAKE_CURRENT_SOURCE_DIR}/egl/qegl_x11.cpp
    )
    set(EXTRA_GUI_LIBS
        ${EXTRA_GUI_LIBS}
        ${EGL_LIBRARIES}
    )
    include_directories(${EGL_INCLUDES})
endif()

