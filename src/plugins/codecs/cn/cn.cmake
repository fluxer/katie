set(CNCODECSPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/cn/qgb18030codec.h
)

set(CNCODECSPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/cn/qgb18030codec.cpp
)

katie_setup_target(qcncodecsplugin ${CNCODECSPLUGIN_SOURCES} ${CNCODECSPLUGIN_HEADERS})

add_library(qcncodecsplugin MODULE ${qcncodecsplugin_SOURCES})
target_link_libraries(qcncodecsplugin KtCore)
set_target_properties(qcncodecsplugin PROPERTIES OUTPUT_NAME qcncodecs)

install(
    TARGETS qcncodecsplugin
    DESTINATION ${QT_PLUGINS_PATH}/codecs
)
