include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/script/qtdbus
)

set(QTDBUSPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/script/qtdbus/dbmain.h
)

set(QTDBUSPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/script/qtdbus/dbmain.cpp
)

katie_setup_target(qtscriptdbusplugin ${QTDBUSPLUGIN_SOURCES} ${QTDBUSPLUGIN_HEADERS})

add_library(qtscriptdbusplugin MODULE ${qtscriptdbusplugin_SOURCES})
target_link_libraries(qtscriptdbusplugin KtCore KtDBus KtScript)
set_target_properties(qtscriptdbusplugin PROPERTIES OUTPUT_NAME qtscriptdbus)
target_include_directories(qtscriptdbusplugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/script/qtdbus)

install(
    TARGETS qtscriptdbusplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/script
)
