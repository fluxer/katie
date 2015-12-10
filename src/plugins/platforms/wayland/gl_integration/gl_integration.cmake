if(WITH_OPENGL AND OPENGL_FOUND)
    set(WAYLANDPLUGIN_DEFINITIONS
        ${WAYLANDPLUGIN_DEFINITIONS}
        -DQT_WAYLAND_GL_SUPPORT
    )
endif()

set(WAYLANDPLUGIN_HEADERS
    ${WAYLANDPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/qwaylandglintegration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/qwaylandglwindowsurface.h
)

set(WAYLANDPLUGIN_SOURCES
    ${WAYLANDPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/qwaylandglintegration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/qwaylandglwindowsurface.cpp
)


# TODO: automatically choose the best one for the platform
set(WAYLANDPLUGIN_INTEGRATION "xcomposite_glx")

if(WITH_OPENGLES2)
    if(${WAYLANDPLUGIN_INTEGRATION} STREQUAL "wayland_egl")
        include(wayland_egl/wayland_egl.cmake)
    elseif(${WAYLANDPLUGIN_INTEGRATION} STREQUAL "readback")
        include(readback_egl/readback_egl.cmake)
    else()
        include(xcomposite_egl/xcomposite_egl.cmake)
    endif()
else()
    if(${WAYLANDPLUGIN_INTEGRATION} STREQUAL "readback")
        include(readback_glx/readback_glx.cmake)
    else()
        include(xcomposite_glx/xcomposite_glx.cmake)
    endif()
endif()
