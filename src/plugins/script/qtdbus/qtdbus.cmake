include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/script/qtdbus
)

set(QTDBUSPLUGIN_HEADERS
    ${QTDBUSPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/script/qtdbus/dbmain.h
)

set(QTDBUSPLUGIN_SOURCES
    ${QTDBUSPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/script/qtdbus/dbmain.cpp
)

add_library(qtscriptdbusplugin MODULE ${QTDBUSPLUGIN_SOURCES} ${QTDBUSPLUGIN_HEADERS})
target_link_libraries(qtscriptdbusplugin KtCore KtDBus KtScript)
set_target_properties(qtscriptdbusplugin PROPERTIES OUTPUT_NAME qtscriptdbus)
target_include_directories(qtscriptdbusplugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/script/qtdbus)

katie_setup_target(qtscriptdbusplugin)

install(
    TARGETS qtscriptdbusplugin
    DESTINATION ${QT_PLUGINS_PATH}/script
)
