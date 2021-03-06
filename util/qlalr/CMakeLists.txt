# add_definitions()
set(EXTRA_QLALR_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(QLALR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/compress.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cppgenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dotgraph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lalr.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parsetable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/recognizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/grammar.cpp
)

set(QLALR_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/compress.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cppgenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dotgraph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/lalr.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parsetable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/grammar_p.h
)

katie_setup_target(qlalr ${QLALR_SOURCES} ${QLALR_HEADERS})

add_executable(qlalr ${qlalr_SOURCES})
target_link_libraries(qlalr ${EXTRA_QLALR_LIBS})
