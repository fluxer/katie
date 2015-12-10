set(QTGAPLUGIN_HEADERS
    ${QTGAPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgahandler.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgafile.h
)

set(QTGAPLUGIN_SOURCES
    ${QTGAPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgaplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgahandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/tga/qtgafile.cpp
)

add_library(qtgaplugin ${KATIE_TYPE} ${QTGAPLUGIN_SOURCES} ${QTGAPLUGIN_HEADERS})
target_link_libraries(qtgaplugin KtCore KtGui)
set_target_properties(qtgaplugin PROPERTIES OUTPUT_NAME qtga)

install(
    TARGETS qtgaplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/imageformats
)
