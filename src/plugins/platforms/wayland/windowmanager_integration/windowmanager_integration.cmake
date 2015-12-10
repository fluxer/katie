set(WAYLANDPLUGIN_DEFINITIONS
    ${WAYLANDPLUGIN_DEFINITIONS}
    -DQT_WAYLAND_WINDOWMANAGER_SUPPORT
)

set(WAYLANDPLUGIN_HEADERS
    ${WAYLANDPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/windowmanager_integration/qwaylandwindowmanagerintegration.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/wayland/qwaylandwindowmanager-client-protocol.h
)

set(WAYLANDPLUGIN_SOURCES
    ${WAYLANDPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/windowmanager_integration/qwaylandwindowmanagerintegration.cpp
    ${CMAKE_SOURCE_DIR}/src/3rdparty/wayland/wayland-windowmanager-protocol.c
)
