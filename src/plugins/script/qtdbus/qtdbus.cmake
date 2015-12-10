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

katie_resources(${QTDBUSPLUGIN_HEADERS})

add_library(qtscriptdbusplugin ${KATIE_TYPE} ${QTDBUSPLUGIN_SOURCES} ${QTDBUSPLUGIN_HEADERS})
target_link_libraries(qtscriptdbusplugin KtCore KtDBus KtScript)
set_target_properties(qtscriptdbusplugin PROPERTIES OUTPUT_NAME qtscriptdbus)
target_include_directories(qtscriptdbusplugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/script/qtdbus)

install(
    TARGETS qtscriptdbusplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/script
)
