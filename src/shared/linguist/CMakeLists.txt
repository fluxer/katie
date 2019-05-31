set(SHAREDLINGUIST_DEFINITIONS
)

set(SHAREDLINGUIST_INCLUDES
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_SOURCE_DIR}/src/shared/linguist
)

set(SHAREDLINGUIST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/simtexth.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/numerus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/translator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/translatormessage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/po.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ts.cpp
)

set(SHAREDLINGUIST_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/simtexth.h
    ${CMAKE_CURRENT_SOURCE_DIR}/translator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/translatormessage.h
)

katie_setup_target(sharedlinguist ${SHAREDLINGUIST_SOURCES} ${SHAREDLINGUIST_HEADERS})

add_library(sharedlinguist OBJECT ${sharedlinguist_SOURCES})
target_compile_definitions(sharedlinguist PUBLIC ${SHAREDLINGUIST_DEFINITIONS})
target_include_directories(sharedlinguist PUBLIC ${SHAREDLINGUIST_INCLUDES})
