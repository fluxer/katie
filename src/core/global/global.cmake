set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnamespace.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qendian.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnumeric.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qlibraryinfo.h
)

set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qglobal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qlibraryinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnumeric.cpp
)
