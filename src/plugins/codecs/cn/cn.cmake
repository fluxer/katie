set(CNCODECSPLUGIN_HEADERS
    ${CNCODECSPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/cn/qgb18030codec.h
)

set(CNCODECSPLUGIN_SOURCES
    ${CNCODECSPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/cn/qgb18030codec.cpp
)

add_library(qcncodecsplugin ${KATIE_TYPE} ${CNCODECSPLUGIN_SOURCES} ${CNCODECSPLUGIN_HEADERS})
target_link_libraries(qcncodecsplugin KtCore)
set_target_properties(qcncodecsplugin PROPERTIES OUTPUT_NAME qcncodecs)

install(
    TARGETS qcncodecsplugin
    DESTINATION ${QT_PLUGINS_PATH}/codecs
)
