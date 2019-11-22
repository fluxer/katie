set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodec_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodec.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qicucodec_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodecplugin.h
)

set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodec.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qtextcodecplugin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs/qicucodec.cpp
)
