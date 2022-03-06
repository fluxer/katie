# add_definitions()
set(EXTRA_IMGCONV_LIBS KtCore KtGui)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(IMGCONV_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_target(imgconv ${IMGCONV_SOURCES})

add_executable(imgconv ${imgconv_SOURCES})
target_link_libraries(imgconv ${EXTRA_IMGCONV_LIBS})
