add_definitions(-DQT_BUILD_XML_LIB)
set(EXTRA_XML_LIBS KtCore)

set(XML_PUBLIC_HEADERS
    ${XML_PUBLIC_HEADERS}
    # mixed
    QDomDocument
    QDomElement
    QDomNode
    QDomNodeList
    QDomCDATASection
    QDomCharacterData
    QDomComment
    QDomDocumentFragment
    QDomDocumentType
    QDomEntity
    QDomEntityReference
    QDomImplementation
    QDomNamedNodeMap
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
    # both in core and xml
    QXmlStreamAttribute
    QXmlStreamAttributes
    QXmlStreamStringRef
    QXmlStreamNotationDeclaration
    QXmlStreamNotationDeclarations
    QXmlStreamEntityDeclaration
    QXmlStreamEntityDeclarations
    QXmlStreamNamespaceDeclaration
    QXmlStreamNamespaceDeclarations
    QXmlStreamEntityResolver
    QXmlStreamReader
    QXmlStreamWriter
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

katie_generate_misc("${XML_HEADERS}" QtXml)
katie_generate_public("${XML_PUBLIC_HEADERS}" QtXml)
katie_generate_package(KtXml "KtCore")
katie_setup_target(KtXml ${XML_SOURCES} ${XML_HEADERS})
katie_setup_paths()

add_library(KtXml ${KATIE_TYPE} ${KtXml_SOURCES})
target_link_libraries(KtXml ${EXTRA_XML_LIBS})
set_target_properties(KtXml PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Xml
)

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
