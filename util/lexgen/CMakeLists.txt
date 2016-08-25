# add_definitions()
set(EXTRA_LEXGEN_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(LEXGEN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/nfa.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/configfile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/re2nfa.cpp
)

set(LEXGEN_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/configfile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/generator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/global.h
    ${CMAKE_CURRENT_SOURCE_DIR}/nfa.h
    ${CMAKE_CURRENT_SOURCE_DIR}/re2nfa.h
)

katie_setup_target(lexgen ${LEXGEN_SOURCES} ${LEXGEN_HEADERS})

add_executable(lexgen EXCLUDE_FROM_ALL ${lexgen_SOURCES})
target_link_libraries(lexgen ${EXTRA_LEXGEN_LIBS})
