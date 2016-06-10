set(QSVGPLUGIN_HEADERS
    ${QSVGPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/svg/qsvgiohandler.h
)

set(QSVGPLUGIN_SOURCES
    ${QSVGPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/svg/qsvgplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/svg/qsvgiohandler.cpp
)

add_library(qsvgplugin ${KATIE_TYPE} ${QSVGPLUGIN_SOURCES} ${QSVGPLUGIN_HEADERS})
target_link_libraries(qsvgplugin KtCore KtGui KtSvg)
set_target_properties(qsvgplugin PROPERTIES OUTPUT_NAME qsvg)

install(
    TARGETS qsvgplugin
    DESTINATION ${QT_PLUGINS_PATH}/imageformats
)
