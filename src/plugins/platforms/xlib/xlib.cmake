set(EXTRA_QXLIB_LIBS
    ${EXTRA_QXLIB_LIBS}
    ${X11_LIBRARIES}
    ${X11_Xext_LIBRARIES}
)

set(QXLIBPLUGIN_INCLUDES
    ${QXLIBPLUGIN_INCLUDES}
    ${CMAKE_SOURCE_DIR}/src/gui/kernel
)

set(QXLIBPLUGIN_HEADERS
    ${QXLIBPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibintegration.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibwindowsurface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibcursor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibscreen.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibkeyboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibclipboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibmime.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibstatic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibdisplay.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibnativeinterface.h
)

set(QXLIBPLUGIN_SOURCES
    ${QXLIBPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/xlibplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibintegration.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibwindowsurface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibcursor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibscreen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibkeyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibclipboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibmime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibstatic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibdisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qxlibnativeinterface.cpp
)

if(WITH_OPENGL AND OPENGL_FOUND)
    if(NOT WITH_OPENGLES2)
        # include (../glxconvenience/glxconvenience.pri)
        set(QXLIBPLUGIN_HEADERS
            ${QXLIBPLUGIN_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qglxintegration.h
        )
        set(QXLIBPLUGIN_SOURCES
            ${QXLIBPLUGIN_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/xlib/qglxintegration.cpp
        )
     else()
        # TODO: use shared target objects
        # There is no easy way to detect if we'r suppose to use glx or not
        set(QXLIBPLUGIN_HEADERS
            ${QXLIBPLUGIN_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qeglplatformcontext.h
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qeglconvenience.h
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qxlibeglintegration.h
        )
        set(QXLIBPLUGIN_SOURCES
            ${QXLIBPLUGIN_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qeglplatformcontext.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qeglconvenience.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/platforms/eglconvenience/qxlibeglintegration.cpp
        )
        set(EXTRA_QXLIB_LIBS
            ${EXTRA_QXLIB_LIBS}
            ${EGL_LIBRARIES}
        )
        set(QXLIBPLUGIN_INCLUDES
            ${QXLIBPLUGIN_INCLUDES}
            ${EGL_INCLUDES}
        )
    endif()
endif()

if(${KATIE_PLATFORM} STREQUAL "mac")
    set(EXTRA_QXLIB_LIBS
        ${EXTRA_QXLIB_LIBS}
        -L/usr/X11/lib
        ${ZLIB_LIBRARIES}
        -framework Carbon
    )
endif()

katie_resources("${QXLIBPLUGIN_SOURCES}")
katie_resources("${QXLIBPLUGIN_HEADERS}")

add_library(qxlibplugin ${KATIE_TYPE} ${QXLIBPLUGIN_SOURCES} ${QXLIBPLUGIN_HEADERS})
set_target_properties(qxlibplugin PROPERTIES OUTPUT_NAME qxlib)
target_link_libraries(qxlibplugin ${EXTRA_QXLIB_LIBS})
target_include_directories(qxlibplugin PRIVATE ${QXLIBPLUGIN_INCLUDES})

install(
    TARGETS qxlibplugin
    DESTINATION ${QT_PLUGINS_PATH}/platforms
)
