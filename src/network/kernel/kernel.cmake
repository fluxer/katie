set(NETWORK_HEADERS
    ${NETWORK_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qauthenticator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qauthenticator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostaddress.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostinfo_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qurlinfo.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkproxy.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkinterface_p.h
)

set(NETWORK_SOURCES
    ${NETWORK_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qauthenticator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostaddress.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qurlinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkproxy.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkinterface.cpp
)

if(UNIX)
    set(NETWORK_SOURCES
        ${NETWORK_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostinfo_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkinterface_unix.cpp
    )
endif()

if(KATIE_PLATFORM STREQUAL "win32")
    set(NETWORK_SOURCES
        ${NETWORK_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostinfo_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkinterface_win.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkproxy_win.cpp
    )
elseif(KATIE_PLATFORM STREQUAL "mac")
    set(NETWORK_SOURCES
        ${NETWORK_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkproxy_mac.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkproxy_mac.cpp
    )
    set(CMAKE_SHARED_LINK_FLAGS
        ${CMAKE_SHARED_LINK_FLAGS}
        -framework SystemConfiguration
        -framework CoreFoundation
        -framework CoreServices
    )
elseif(KATIE_PLATFORM STREQUAL "blackberry")
    set(NETWORK_SOURCES
        ${NETWORK_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkproxy_blackberry.cpp
    )
    set(EXTRA_NETWORK_LIBS
        ${EXTRA_NETWORK_LIBS}
        bps
    )
elseif(KATIE_PLATFORM STREQUAL "integrity")
    set(NETWORK_SOURCES
        ${NETWORK_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qhostinfo_unix.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkinterface_unix.cpp
    )
else()
    set(NETWORK_SOURCES
        ${NETWORK_SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel/qnetworkproxy_generic.cpp
    )
endif()