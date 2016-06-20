# genericunix

set(WAYLANDPLUGIN_DEFINITIONS
    ${WAYLANDPLUGIN_DEFINITIONS}
    -DQ_PLATFORM_WAYLAND
)

set(WAYLANDPLUGIN_INCLUDES
    ${WAYLANDPLUGIN_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/3rdparty/wayland
)

set(WAYLANDPLUGIN_HEADERS
    ${WAYLANDPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandintegration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandnativeinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandcursor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylanddisplay.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandscreen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandshmsurface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandbuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandshmwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandclipboard.h
)

set(WAYLANDPLUGIN_SOURCES
    ${WAYLANDPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandintegration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandnativeinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandshmsurface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandinputdevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandcursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylanddisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandscreen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandshmwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/wayland/qwaylandclipboard.cpp
)

include(gl_integration/gl_integration.cmake)
include(windowmanager_integration/windowmanager_integration.cmake)

katie_resources(${WAYLANDPLUGIN_SOURCES} ${WAYLANDPLUGIN_HEADERS})

add_library(qwaylandplugin MODULE ${WAYLANDPLUGIN_SOURCES} ${WAYLANDPLUGIN_HEADERS})
set_target_properties(qwaylandplugin PROPERTIES OUTPUT_NAME qwayland)
target_compile_definitions(qwaylandplugin ${WAYLANDPLUGIN_DEFINITIONS})
target_include_directories(qwaylandplugin ${WAYLANDPLUGIN_INCLUDES})

install(
    TARGETS qwaylandplugin
    DESTINATION ${QT_PLUGINS_PATH}/platforms
)
