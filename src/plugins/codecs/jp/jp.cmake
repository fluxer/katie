set(JPCODECSPLUGIN_HEADERS
    ${JPCODECSPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qjpunicode.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qeucjpcodec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qjiscodec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qsjiscodec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qfontjpcodec.h
)

set(JPCODECSPLUGIN_SOURCES
    ${JPCODECSPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qeucjpcodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qjiscodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qsjiscodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qjpunicode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/qfontjpcodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/jp/jpmain.cpp
)

add_library(qjpcodecsplugin MODULE ${JPCODECSPLUGIN_SOURCES} ${JPCODECSPLUGIN_HEADERS})
target_link_libraries(qjpcodecsplugin KtCore)
set_target_properties(qjpcodecsplugin PROPERTIES OUTPUT_NAME qjpcodecs)

install(
    TARGETS qjpcodecsplugin
    DESTINATION ${QT_PLUGINS_PATH}/codecs
)
