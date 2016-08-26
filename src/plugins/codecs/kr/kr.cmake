set(KRCODECSPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/qeuckrcodec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/cp949codetbl.h
)

set(KRCODECSPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/qeuckrcodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/kr/krmain.cpp
)

katie_setup_target(qkrcodecsplugin ${KRCODECSPLUGIN_SOURCES} ${KRCODECSPLUGIN_HEADERS})

add_library(qkrcodecsplugin MODULE ${qkrcodecsplugin_SOURCES})
target_link_libraries(qkrcodecsplugin KtCore)
set_target_properties(qkrcodecsplugin PROPERTIES OUTPUT_NAME qkrcodecs)

install(
    TARGETS qkrcodecsplugin
    DESTINATION ${QT_PLUGINS_PATH}/codecs
)
