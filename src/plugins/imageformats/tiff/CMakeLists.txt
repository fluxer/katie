include_directories(${TIFF_INCLUDE_DIR})

set(QTIFFPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qtiffhandler_p.h
)

set(QTIFFPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qtiffplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtiffhandler.cpp
)

katie_setup_target(qtiffplugin ${QTIFFPLUGIN_SOURCES} ${QTIFFPLUGIN_HEADERS})

add_library(qtiffplugin MODULE ${qtiffplugin_SOURCES})
target_link_libraries(qtiffplugin KtCore KtGui ${TIFF_LIBRARIES})
set_target_properties(qtiffplugin PROPERTIES OUTPUT_NAME qtiff)

install(
    TARGETS qtiffplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
    COMPONENT Runtime
)
