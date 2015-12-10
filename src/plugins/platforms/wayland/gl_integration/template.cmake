set(EXTRA_WAYLANDPLUGIN_LIBS
    ${EXTRA_WAYLANDPLUGIN_LIBS}
    # TODO: unhardcode
    X11
    Xext
    EGL
)

set(WAYLANDPLUGIN_HEADERS
    ${WAYLANDPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl/
)

set(WAYLANDPLUGIN_SOURCES
    ${WAYLANDPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/gl_integration/readback_egl/
)
