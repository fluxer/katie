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

set(ATOMIC_FILES)
if(${KATIE_ARCHITECTURE} STREQUAL "alpha" AND NOT ${KATIE_COMPILER} STREQUAL "gcc")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/alpha/qatomic_alpha.s
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "arm")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/arm/qatomic_arm.cpp
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "i386" AND NOT ${KATIE_COMPILER} MATCHES "(gcc|clang|icc)")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/i386/qatomic_i386.s
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "ia64" AND NOT ${KATIE_COMPILER} MATCHES "(gcc|icc|hpuxi)")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/ia64/qatomic_ia64.s
    )
# note: even though we use inline assembler with gcc, we always
# include the compiled version to keep binary compatibility
elseif(${KATIE_ARCHITECTURE} MATCHES "(mips64|mips32)")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/mips/qatomic_${KATIE_ARCHITECTURE}.s
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "parisc")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/parisc/q_ldcw.s
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/parisc/qatomic_parisc.cpp
    )
elseif(${KATIE_ARCHITECTURE} MATCHES "(powerpc32|powerpc64)" AND NOT ${KATIE_COMPILER} STREQUAL "gcc")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/powerpc/qatomic_${KATIE_ARCHITECTURE}.s
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "sh")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/sh/qatomic_sh.cpp
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "sparc64")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/sparc/qatomic_sparc64.s
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "sparc32")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/sparc/qatomic_sparc32.s
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/sparc/qatomic_sparc.cpp
    )
elseif(${KATIE_ARCHITECTURE} STREQUAL "x86_64" AND ${KATIE_PLATFORM} STREQUAL "solaris")
    set(ATOMIC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/x86_64/qatomic_sun.s
    )
endif()
if(ATOMIC_FILES)
    set(CORE_SOURCES
        ${CORE_SOURCES}
        ${ATOMIC_FILES}
    )
endif()

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
