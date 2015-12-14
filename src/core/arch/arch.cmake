if(${KATIE_PLATFORM} STREQUAL "win32")
    set(CORE_HEADERS
        ${CORE_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_windows.h
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_generic.h
    )
    if(${KATIE_COMPILER} STREQUAL "gcc")
        set(CORE_HEADERS
            ${CORE_HEADERS}
            ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_i386.h
            ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_x86_64.h
        )
    endif()
elseif(${KATIE_PLATFORM} STREQUAL "mac")
    set(CORE_HEADERS
        ${CORE_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_macosx.h
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_generic.h
    )
elseif(${KATIE_PLATFORM} STREQUAL "vxworks")
    set(CORE_HEADERS
        ${CORE_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_vxworks.h
    )
elseif(${KATIE_PLATFORM} STREQUAL "integrity")
    set(CORE_HEADERS
        ${CORE_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_integrity.h
    )
endif()

set(CORE_HEADERS
    ${CORE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_avr32.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_ia64.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_parisc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_sparc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_arch.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_generic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_powerpc.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_arm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_armv5.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_armv6.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_armv7.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_i386.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_mips.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_s390.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_x86_64.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_sh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/qatomic_sh4a.h
)

# TODO: inline assembly check
set(CORE_SOURCES
    ${CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/i386/qatomic_i386.s
)

if(UNIX)
    set(CORE_SOURCES
        ${CORE_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/generic/qatomic_generic_unix.cpp
    )
elseif(${KATIE_PLATFORM} STREQUAL "win32")
    set(CORE_SOURCES
        ${CORE_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/generic/qatomic_generic_windows.cpp
    )
endif()
