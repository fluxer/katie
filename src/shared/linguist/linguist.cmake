set(SHAREDLINGUIST_DEFINITIONS
    ${SHAREDLINGUIST_DEFINITIONS}
    -DPROEVALUATOR_CUMULATIVE
    -DPROEVALUATOR_INIT_PROPS
)

set(SHAREDLINGUIST_INCLUDES
    ${SHAREDLINGUIST_INCLUDES}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_SOURCE_DIR}/src/shared/linguist
)

set(SHAREDLINGUIST_SOURCES
    ${SHAREDLINGUIST_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/simtexth.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/numerus.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translator.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translatormessage.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/qm.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/qph.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/po.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/ts.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/xliff.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/ioutils.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/proitems.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/profileparser.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/profileevaluator.cpp
)

set(SHAREDLINGUIST_HEADERS
    ${SHAREDLINGUIST_HEADERS}
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/simtexth.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translator.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/translatormessage.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/proparser_global.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/ioutils.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/proitems.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/profileparser.h
    ${CMAKE_SOURCE_DIR}/src/shared/linguist/profileevaluator.h
) 

add_library(sharedlinguist OBJECT ${SHAREDLINGUIST_SOURCES})
target_compile_definitions(sharedlinguist PRIVATE ${SHAREDLINGUIST_DEFINITIONS})
target_include_directories(sharedlinguist PRIVATE ${SHAREDLINGUIST_INCLUDES})
