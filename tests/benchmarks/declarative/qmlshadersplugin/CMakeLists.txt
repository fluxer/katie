if(WITH_OPENGL AND OPENGL_FOUND)
    katie_test(tst_performance
        ${CMAKE_CURRENT_SOURCE_DIR}/tst_performance.cpp
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffectitem.cpp
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffectsource.cpp
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffect.cpp
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffectbuffer.cpp
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/scenegraph/qsggeometry.cpp
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffectitem.h
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffectsource.h
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffect.h
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/shadereffectbuffer.h
        ${CMAKE_SOURCE_DIR}/src/imports/shaders/scenegraph/qsggeometry.h
    )

    target_link_libraries(tst_performance KtDeclarative KtOpenGL)
    include_directories(${CMAKE_BINARY_DIR}/src/imports/shaders)
endif()
