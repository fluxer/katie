set(TWCODECSPLUGIN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/tw/qbig5codec.h
)

set(TWCODECSPLUGIN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/tw/qbig5codec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/tw/twmain.cpp
)

katie_setup_target(qtwcodecsplugin ${TWCODECSPLUGIN_SOURCES} ${TWCODECSPLUGIN_HEADERS})

add_library(qtwcodecsplugin MODULE ${qtwcodecsplugin_SOURCES})
target_link_libraries(qtwcodecsplugin KtCore)
set_target_properties(qtwcodecsplugin PROPERTIES OUTPUT_NAME qtwcodecs)

install(
    TARGETS qtwcodecsplugin
    DESTINATION ${KATIE_PLUGINS_RELATIVE}/codecs
)
