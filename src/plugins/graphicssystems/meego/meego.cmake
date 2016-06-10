set(QMEEGOGRAPHICSSYSTEMPLUGIN_HEADERS
    ${QMEEGOGRAPHICSSYSTEMPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegographicssystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegopixmapdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegoextensions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegorasterpixmapdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegolivepixmapdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegolivepixmapdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegopixmapdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegoextensions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegorasterpixmapdata.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegographicssystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegographicssystemplugin.h
)

set(QMEEGOGRAPHICSSYSTEMPLUGIN_SOURCES
    ${QMEEGOGRAPHICSSYSTEMPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegographicssystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegographicssystemplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegopixmapdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegoextensions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegorasterpixmapdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/qmeegolivepixmapdata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphicssystems/meego/dithering.cpp
)

katie_resources("${QMEEGOGRAPHICSSYSTEMPLUGIN_HEADERS}")
katie_resources("${QMEEGOGRAPHICSSYSTEMPLUGIN_SOURCES}")

add_library(qmeegographicssystemplugin ${KATIE_TYPE} ${QMEEGOGRAPHICSSYSTEMPLUGIN_SOURCES} ${QMEEGOGRAPHICSSYSTEMPLUGIN_HEADERS})
set_target_properties(qmeegographicssystemplugin PROPERTIES OUTPUT_NAME qmeegographicssystem)

install(
    TARGETS qmeegographicssystemplugin
    DESTINATION ${QT_PLUGINS_PATH}/graphicssystems
)
