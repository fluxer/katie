set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_generic.h
)

set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_unix.cpp
)
