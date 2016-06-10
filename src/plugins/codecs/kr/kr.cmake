set(KRCODECSPLUGIN_HEADERS
    ${KRCODECSPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/qeuckrcodec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/cp949codetbl.h
)

set(KRCODECSPLUGIN_SOURCES
    ${KRCODECSPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/qeuckrcodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/krmain.cpp
)

add_library(qkrcodecsplugin ${KATIE_TYPE} ${KRCODECSPLUGIN_SOURCES} ${KRCODECSPLUGIN_HEADERS})
target_link_libraries(qkrcodecsplugin KtCore)
set_target_properties(qkrcodecsplugin PROPERTIES OUTPUT_NAME qkrcodecs)

install(
    TARGETS qkrcodecsplugin
    DESTINATION ${QT_PLUGINS_PATH}/codecs
)
