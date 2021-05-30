set(QTDBUSPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/dbmain.h
)

set(QTDBUSPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/dbmain.cpp
)

katie_setup_target(qtscriptdbusplugin ${QTDBUSPLUGIN_SOURCES} ${QTDBUSPLUGIN_HEADERS})

add_library(qtscriptdbusplugin MODULE ${qtscriptdbusplugin_SOURCES})
target_link_libraries(qtscriptdbusplugin KtCore KtDBus KtScript)
set_target_properties(qtscriptdbusplugin PROPERTIES OUTPUT_NAME qtscriptdbus)

katie_setup_plugin(qtscriptdbusplugin)

install(
    TARGETS qtscriptdbusplugin
    DESTINATION ${KATIE_PLUGINS_PATH}/script
    COMPONENT Runtime
)
