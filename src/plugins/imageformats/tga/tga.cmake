set(QTGAPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgahandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgafile.h
)

set(QTGAPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgaplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgahandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgafile.cpp
)

katie_setup_target(qtgaplugin ${QTGAPLUGIN_SOURCES} ${QTGAPLUGIN_HEADERS})

add_library(qtgaplugin MODULE ${qtgaplugin_SOURCES})
target_link_libraries(qtgaplugin KtCore KtGui)
set_target_properties(qtgaplugin PROPERTIES OUTPUT_NAME qtga)

install(
    TARGETS qtgaplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
)
