set(QICOPLUGIN_HEADERS
    ${QICOPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/ico/qicohandler.h
)

set(QICOPLUGIN_SOURCES
    ${QICOPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/ico/qicoplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats/ico/qicohandler.cpp
)

add_library(qicoplugin MODULE ${QICOPLUGIN_SOURCES} ${QICOPLUGIN_HEADERS})
target_link_libraries(qicoplugin KtCore KtGui)
set_target_properties(qicoplugin PROPERTIES OUTPUT_NAME qico)

install(
    TARGETS qicoplugin
    DESTINATION ${QT_PLUGINS_PATH}/imageformats
)
