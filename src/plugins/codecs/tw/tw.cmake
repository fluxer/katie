set(TWCODECSPLUGIN_HEADERS
    ${TWCODECSPLUGIN_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/tw/qbig5codec.h
)

set(TWCODECSPLUGIN_SOURCES
    ${TWCODECSPLUGIN_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/tw/qbig5codec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/tw/twmain.cpp
)

add_library(qtwcodecsplugin ${KATIE_TYPE} ${TWCODECSPLUGIN_SOURCES} ${TWCODECSPLUGIN_HEADERS})
target_link_libraries(qtwcodecsplugin KtCore)
set_target_properties(qtwcodecsplugin PROPERTIES OUTPUT_NAME qtwcodecs)

install(
    TARGETS qtwcodecsplugin
    DESTINATION ${QT_PLUGINS_PATH_INST}/codecs
)
