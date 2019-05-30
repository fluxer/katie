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
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/simtexth.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/numerus.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translator.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translatormessage.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/qm.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/qph.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/po.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/ts.cpp
)

set(SHAREDLINGUIST_HEADERS
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/simtexth.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translator.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translatormessage.h
)

katie_setup_target(sharedlinguist ${SHAREDLINGUIST_SOURCES} ${SHAREDLINGUIST_HEADERS})

add_library(sharedlinguist OBJECT ${sharedlinguist_SOURCES})
target_compile_definitions(sharedlinguist PRIVATE ${SHAREDLINGUIST_DEFINITIONS})
target_include_directories(sharedlinguist PRIVATE ${SHAREDLINGUIST_INCLUDES})
