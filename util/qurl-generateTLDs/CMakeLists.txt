# add_definitions()
set(EXTRA_QURLGENERATE_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(QURLGENERATE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

katie_setup_target(qurl-generateTLDs ${QURLGENERATE_SOURCES})

add_executable(qurl-generateTLDs ${qurl-generateTLDs_SOURCES})
target_link_libraries(qurl-generateTLDs ${EXTRA_QURLGENERATE_LIBS})
