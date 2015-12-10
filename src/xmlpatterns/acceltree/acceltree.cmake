set(XMLPATTERNS_HEADERS
    ${XMLPATTERNS_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qacceltreebuilder_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qacceltree_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qacceltreeresourceloader_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qacceliterators_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qcompressedwhitespace_p.h
)

set(XMLPATTERNS_SOURCES
    ${XMLPATTERNS_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qacceltree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qacceltreeresourceloader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qacceliterators.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree/qcompressedwhitespace.cpp
)

