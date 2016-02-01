if(WITH_OPENGL AND OPENGL_FOUND)
    katie_test(paintbenchmark
        ${CMAKE_CURRENT_SOURCE_DIR}/paintbenchmark.cpp
    )

    target_link_libraries(paintbenchmark KtDeclarative KtOpenGL)
endif()