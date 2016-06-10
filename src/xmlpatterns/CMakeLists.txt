add_definitions(-DQT_BUILD_XMLPATTERNS_LIB)
set(EXTRA_XMLPATTERNS_LIBS KtCore KtNetwork)

set(XMLPATTERNS_PUBLIC_HEADERS
    ${XMLPATTERNS_PUBLIC_HEADERS}
    QAbstractMessageHandler
    QAbstractUriResolver
    QAbstractXmlNodeModel
    QAbstractXmlReceiver
    QSimpleXmlNodeModel
    QSourceLocation
    QXmlFormatter
    QXmlItem
    QXmlName
    QXmlNamePool
    QXmlNodeModelIndex
    QXmlQuery
    QXmlResultItems
    QXmlSchema
    QXmlSchemaValidator
    QXmlSerializer
)

include(acceltree/acceltree.cmake)
include(api/api.cmake)
include(data/data.cmake)
include(environment/environment.cmake)
include(expr/expr.cmake)
include(functions/functions.cmake)
include(iterators/iterators.cmake)
include(janitors/janitors.cmake)
include(parser/parser.cmake)
include(projection/projection.cmake)
include(schema/schema.cmake)
include(type/type.cmake)
include(utils/utils.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtXmlPatterns
    ${CMAKE_BINARY_DIR}/privateinclude/QtXmlPatterns
    ${CMAKE_CURRENT_SOURCE_DIR}/acceltree
    ${CMAKE_CURRENT_SOURCE_DIR}/api
    ${CMAKE_CURRENT_SOURCE_DIR}/data
    ${CMAKE_CURRENT_SOURCE_DIR}/environment
    ${CMAKE_CURRENT_SOURCE_DIR}/expr
    ${CMAKE_CURRENT_SOURCE_DIR}/functions
    ${CMAKE_CURRENT_SOURCE_DIR}/iterators
    ${CMAKE_CURRENT_SOURCE_DIR}/janitors
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/projection
    ${CMAKE_CURRENT_SOURCE_DIR}/schema
    ${CMAKE_CURRENT_SOURCE_DIR}/type
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_BINARY_DIR}/acceltree
    ${CMAKE_CURRENT_BINARY_DIR}/api
    ${CMAKE_CURRENT_BINARY_DIR}/data
    ${CMAKE_CURRENT_BINARY_DIR}/environment
    ${CMAKE_CURRENT_BINARY_DIR}/expr
    ${CMAKE_CURRENT_BINARY_DIR}/functions
    ${CMAKE_CURRENT_BINARY_DIR}/iterators
    ${CMAKE_CURRENT_BINARY_DIR}/janitors
    ${CMAKE_CURRENT_BINARY_DIR}/parser
    ${CMAKE_CURRENT_BINARY_DIR}/projection
    ${CMAKE_CURRENT_BINARY_DIR}/schema
    ${CMAKE_CURRENT_BINARY_DIR}/type
    ${CMAKE_CURRENT_BINARY_DIR}/utils
)

katie_generate_misc("${XMLPATTERNS_HEADERS}" QtXmlPatterns)
katie_generate_public("${XMLPATTERNS_PUBLIC_HEADERS}" QtXmlPatterns)
katie_generate_map(QtXmlPatterns Q_XMLPATTERNS_EXPORT)
katie_resources(${XMLPATTERNS_SOURCES} ${XMLPATTERNS_HEADERS})
katie_setup_flags()

add_library(KtXmlPatterns ${KATIE_TYPE} ${XMLPATTERNS_SOURCES} ${XMLPATTERNS_HEADERS})
target_link_libraries(KtXmlPatterns ${EXTRA_XMLPATTERNS_LIBS})
set_target_properties(KtXmlPatterns PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME XmlPatterns
)

katie_generate_package(KtXmlPatterns "KtCore KtNetwork")

install(
    TARGETS KtXmlPatterns
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtXmlPatterns
    DESTINATION ${QT_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtXmlPatterns)
