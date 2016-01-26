set(SHAREDHELP_DEFINITIONS)

set(SHAREDHELP_INCLUDES
    ${SHAREDHELP_INCLUDES}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtHelp
    ${CMAKE_BINARY_DIR}/privateinclude/QtHelp
    ${CMAKE_SOURCE_DIR}/src/shared/help
    ${CMAKE_BINARY_DIR}/src/shared/help
)

set(SHAREDHELP_SOURCES
    ${SHAREDHELP_SOURCES}
    ${CMAKE_SOURCE_DIR}/src/shared/help/collectionconfiguration.cpp
    ${CMAKE_SOURCE_DIR}/src/shared/help/helpgenerator.cpp
)

set(SHAREDHELP_HEADERS
    ${SHAREDHELP_HEADERS}
    ${CMAKE_SOURCE_DIR}/src/shared/help/collectionconfiguration.h
    ${CMAKE_SOURCE_DIR}/src/shared/help/helpgenerator.h
) 

katie_resources(${SHAREDHELP_SOURCES} ${SHAREDHELP_HEADERS})

add_library(sharedhelp OBJECT ${SHAREDHELP_SOURCES} ${SHAREDHELP_HEADERS})
target_compile_definitions(sharedhelp PRIVATE ${SHAREDHELP_DEFINITIONS})
target_include_directories(sharedhelp PRIVATE ${SHAREDHELP_INCLUDES})

katie_setup_flags(sharedhelp)
