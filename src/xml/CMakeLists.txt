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
)

include(dom/dom.cmake)
include(sax/sax.cmake)
include(stream/stream.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/privateinclude/QtXml
    ${CMAKE_CURRENT_SOURCE_DIR}/dom
    ${CMAKE_CURRENT_SOURCE_DIR}/sax
    ${CMAKE_CURRENT_SOURCE_DIR}/stream
)

if(${KATIE_PLATFORM} MATCHES "win32" AND MSVC)
    set(KATIE_CXXFLAGS
        ${KATIE_CXXFLAGS}
        -w-use
    )
endif()

katie_generate_misc("${XML_HEADERS}" QtXml)
katie_generate_public("${XML_PUBLIC_HEADERS}" QtXml)
katie_generate_map(QtXml Q_XML_EXPORT)
katie_resources("${XML_SOURCES}")
katie_resources("${XML_HEADERS}")
katie_setup_flags()

add_library(KtXml ${KATIE_TYPE} ${XML_SOURCES} ${XML_HEADERS})
target_link_libraries(KtXml ${EXTRA_XML_LIBS})
set_target_properties(KtXml PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Xml
)

katie_generate_package(KtXml "" "${EXTRA_XML_LIBS}" "QtCore")

install(
    TARGETS KtXml
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH_INST}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtXml
    DESTINATION ${QT_HEADERS_PATH_INST}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtXml)
