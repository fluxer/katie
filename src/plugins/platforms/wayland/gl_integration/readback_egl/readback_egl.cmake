set(EXTRA_WAYLANDPLUGIN_LIBS
    ${EXTRA_WAYLANDPLUGIN_LIBS}
    # TODO: unhardcode
    X11
    Xext
    EGL
)

set(WAYLANDPLUGIN_HEADERS
    ${WAYLANDPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl/qwaylandreadbackeglintegration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl/qwaylandreadbackeglcontext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl/qwaylandreadbackeglwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qeglconvenience.h
)

set(WAYLANDPLUGIN_SOURCES
    ${WAYLANDPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl/qwaylandreadbackeglintegration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl//qwaylandreadbackeglwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl//qwaylandreadbackeglcontext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qeglconvenience.cpp
)
