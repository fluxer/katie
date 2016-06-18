add_definitions(-DQT_BUILD_XML_LIB)
set(EXTRA_XML_LIBS KtCore)

set(XML_PUBLIC_HEADERS
    ${XML_PUBLIC_HEADERS}
    QDomCDATASection
    QDomCharacterData
    QDomComment
    QDomDocumentFragment
    QDomDocumentType
    QDomEntity
    QDomEntityReference
    QDomImplementation
    QDomNamedNodeMap
    QDomNodeList
    QDomNotation
    QDomProcessingInstruction
    QDomText
    QXmlDeclHandler
    QXmlDTDHandler
    QXmlEntityResolver
    QXmlErrorHandler
    QXmlLexicalHandler
    QXmlLocator
    QXmlNamespaceSupport
    QXmlReader
    QXmlStreamAttributes
    QXmlStreamEntityDeclaration
    QXmlStreamEntityDeclarations
    QXmlStreamEntityResolver
    QXmlStreamNamespaceDeclaration
    QXmlStreamNamespaceDeclarations
    QXmlStreamNotationDeclaration
    QXmlStreamNotationDeclarations
    QXmlStreamReader
    QXmlStreamStringRef
    QXmlStreamWriter
    # mixed
    QDomDocument
    QDomElement
    QDomNode
)

include(dom/dom.cmake)
include(sax/sax.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_CURRENT_SOURCE_DIR}/dom
    ${CMAKE_CURRENT_SOURCE_DIR}/sax
)

if(${KATIE_PLATFORM} MATCHES "win32" AND MSVC)
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -w-use
    )
endif()

# The platforms that require the symbol to be present in QtXml
if(${KATIE_TYPE} STREQUAL SHARED AND ${KATIE_PLATFORM} MATCHES "(win32|mac|aix)")
    set(XML_SOURCES
        ${XML_SOURCES}
        ${CMAKE_SOURCE_DIR}/src/core/xml/qxmlstream.cpp
    )
endif()

katie_generate_misc("${XML_HEADERS}" QtXml)
katie_generate_public("${XML_PUBLIC_HEADERS}" QtXml)
katie_resources(${XML_SOURCES} ${XML_HEADERS})
katie_setup_flags()

add_library(KtXml ${KATIE_TYPE} ${XML_SOURCES} ${XML_HEADERS})
target_link_libraries(KtXml ${EXTRA_XML_LIBS})
set_target_properties(KtXml PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Xml
)

katie_generate_package(KtXml "KtCore")

install(
    TARGETS KtXml
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtXml
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtXml)
