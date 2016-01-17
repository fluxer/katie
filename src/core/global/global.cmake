set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qglobal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnamespace.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qendian.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnumeric.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnumeric_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qfeatures.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qlibraryinfo.h
)

set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qglobal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qlibraryinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/global/qnumeric.cpp
)

# TODO: QNX slog2 check
if(${KATIE_PLATFORM} STREQUAL "qnx")
    set(EXTRA_CORE_LIBS
        ${EXTRA_CORE_LIBS}
        slog2
    )
    add_definitions(-DQT_USE_SLOG2)
elseif(${KATIE_PLATFORM} STREQUAL "win32")
    set(CORE_HEADERS
        ${CORE_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/global/qt_windows.h
    )
endif()
