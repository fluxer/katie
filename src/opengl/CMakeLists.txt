add_definitions(-DQT_BUILD_OPENGL_LIB)
set(EXTRA_OPENGL_LIBS KtCore KtGui ${OPENGL_LIBRARIES})

set(OPENGL_PUBLIC_HEADERS
    QGL
    QGLBuffer
    QGLColormap
    QGLContext
    QGLFormat
    QGLFramebufferObject
    QGLFramebufferObjectFormat
    QGLFunctions
    QGLFunctionsPrivate
    QGLPixelBuffer
    QGLScreen
    QGLScreenSurfaceFunctions
    QGLShader
    QGLShaderProgram
    QGLWidget
)

set(OPENGL_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qglcolormap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qglpixelbuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qglpixelbuffer_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qglframebufferobject.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/qglframebufferobject_p.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/qglextensions_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qglpaintdevice_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qglbuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qglfunctions.h
)

set(OPENGL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qgl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qglcolormap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qglpixelbuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qglframebufferobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qglextensions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qglpaintdevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qglbuffer.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtOpenGL
    ${CMAKE_BINARY_DIR}/privateinclude/QtOpenGL
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex
    ${CMAKE_CURRENT_SOURCE_DIR}/util
    ${CMAKE_CURRENT_BINARY_DIR}/gl2paintengineex
    ${CMAKE_CURRENT_BINARY_DIR}/util
    ${CMAKE_SOURCE_DIR}/src/core/tools
    ${CMAKE_SOURCE_DIR}/src/gui/painting
    ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
    ${OPENGL_INCLUDE_DIR}
)

if(NOT WITH_OPENGLES1)
    set(OPENGL_HEADERS
        ${OPENGL_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/qglshaderprogram.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qglpixmapfilter_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qgraphicssystem_gl_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qwindowsurface_gl_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/qpixmapdata_gl_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglgradientcache_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglengineshadermanager_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qgl2pexvertexarray_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qpaintengineex_opengl2_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglengineshadersource_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglcustomshaderstage_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qtriangulatingstroker_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qtriangulator_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qtextureglyphcache_gl_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglshadercache_p.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglshadercache_meego_p.h
    )
    set(OPENGL_SOURCES
        ${OPENGL_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/qglshaderprogram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qglpixmapfilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qgraphicssystem_gl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qwindowsurface_gl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/qpixmapdata_gl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglgradientcache.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglengineshadermanager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qgl2pexvertexarray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qpaintengineex_opengl2.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qglcustomshaderstage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qtriangulatingstroker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qtriangulator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl2paintengineex/qtextureglyphcache_gl.cpp
    )
endif()

if(NOT WITH_OPENGLES2)
    set(OPENGL_HEADERS
        ${OPENGL_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/qpaintengine_opengl_p.h
    )
    set(OPENGL_SOURCES
        ${OPENGL_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/qpaintengine_opengl.cpp
    )
endif()

if(WITH_X11 AND X11_FOUND)
    if(WITH_EGL AND EGL_FOUND)
        set(OPENGL_HEADERS
            ${OPENGL_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/qgl_egl_p.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qpixmapdata_x11gl_p.h
            ${CMAKE_CURRENT_SOURCE_DIR}/qwindowsurface_x11gl_p.h
        )
        set(OPENGL_SOURCES
            ${OPENGL_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/qgl_x11egl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qglpixelbuffer_egl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qgl_egl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qpixmapdata_x11gl_egl.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qwindowsurface_x11gl.cpp
        )
        set(EXTRA_OPENGL_LIBS
            ${EXTRA_OPENGL_LIBS}
            ${EGL_LIBRARIES}
        )
        include_directories(${EGL_INCLUDES})
    else()
        set(OPENGL_SOURCES
            ${OPENGL_SOURCES}
            ${CMAKE_CURRENT_SOURCE_DIR}/qgl_x11.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/qglpixelbuffer_x11.cpp
        )
    endif()
endif()

if(WITH_FREETYPE AND FREETYPE_FOUND)
    set(EXTRA_OPENGL_LIBS
        ${EXTRA_OPENGL_LIBS}
        ${FREETYPE_LIBRARIES}
    )
    include_directories(${FREETYPE_INCLUDE_DIRS})
endif()

set(OPENGL_HEADERS
    ${OPENGL_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qopenglcommon_p.h
)

# qglfunctions.cpp must be last for all-in-one build because qglfunctions.h
# undefines from the global namespace and then defines pretty much all OpenGL
# functions into a class
set(OPENGL_SOURCES
    ${OPENGL_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/qglfunctions.cpp
)

katie_generate_misc("${OPENGL_HEADERS}" QtOpenGL)
katie_generate_public("${OPENGL_PUBLIC_HEADERS}" QtOpenGL)
katie_generate_package(KtOpenGL "KtCore KtGui")
katie_setup_target(KtOpenGL ${OPENGL_SOURCES} ${OPENGL_HEADERS})

add_library(KtOpenGL ${KATIE_TYPE} ${KtOpenGL_SOURCES})
target_link_libraries(KtOpenGL ${EXTRA_OPENGL_LIBS})
set_target_properties(KtOpenGL PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME OpenGL
)

install(
    TARGETS KtOpenGL
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtOpenGL
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtOpenGL)
