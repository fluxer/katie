# add_definitions()
set(EXTRA_IMPORTS_LIBS KtCore)

katie_setup_flags()

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/privateinclude/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtOpenGL
    ${CMAKE_BINARY_DIR}/privateinclude/QtOpenGL
    ${CMAKE_CURRENT_SOURCE_DIR}/folderlistmodel
    ${CMAKE_CURRENT_SOURCE_DIR}/particles
    ${CMAKE_CURRENT_SOURCE_DIR}/gestures
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/scenegraph
    ${CMAKE_CURRENT_BINARY_DIR}/folderlistmodel
    ${CMAKE_CURRENT_BINARY_DIR}/particles
    ${CMAKE_CURRENT_BINARY_DIR}/gestures
    ${CMAKE_CURRENT_BINARY_DIR}/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders/scenegraph
)

include(folderlistmodel/folderlistmodel.cmake)
include(particles/particles.cmake)
include(gestures/gestures.cmake)
if(NOT WITH_OPENGLES1 AND OPENGL_FOUND)
    include(shaders/shaders.cmake)
endif()
