set(XML_HEADERS
    ${XML_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/stream/qxmlstream.h
    # XXX: obsolete
    ${CMAKE_CURRENT_SOURCE_DIR}/stream/qxmlstreamreader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stream/qxmlstreamwriter.h
)

# TODO: !static, The platforms that require the symbol to be present in QtXml
if(${KATIE_PLATFORM} MATCHES "(win32|mac|aix)")
    set(XML_HEADERS
        ${XML_HEADERS}
        ${CMAKE_SOURCE_DIR}/src/core/xml/qxmlstream.cpp
    )
endif()

