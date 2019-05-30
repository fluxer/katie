set(QICOPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qicohandler.h
)

set(QICOPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qicoplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qicohandler.cpp
)

katie_setup_target(qicoplugin ${QICOPLUGIN_SOURCES} ${QICOPLUGIN_HEADERS})

add_library(qicoplugin MODULE ${qicoplugin_SOURCES})
target_link_libraries(qicoplugin KtCore KtGui)
set_target_properties(qicoplugin PROPERTIES OUTPUT_NAME qico)

install(
    TARGETS qicoplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/imageformats
    COMPONENT Runtime
)
