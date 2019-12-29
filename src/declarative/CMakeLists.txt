add_definitions(
    -DQT_NO_URL_CAST_FROM_STRING
)
set(EXTRA_DECLARATIVE_LIBS
    KtCore KtGui KtNetwork KtScript
)

set(DECLARATIVE_PUBLIC_HEADERS
    QDeclarativeAttachedPropertiesFunc
    QDeclarativeComponent
    QDeclarativeContext
    QDeclarativeEngine
    QDeclarativeError
    QDeclarativeExpression
    QDeclarativeExtensionInterface
    QDeclarativeExtensionPlugin
    QDeclarativeImageProvider
    QDeclarativeInfo
    QDeclarativeItem
    QDeclarativeListReference
    QDeclarativeNetworkAccessManagerFactory
    QDeclarativeParserStatus
    QDeclarativeProperties
    QDeclarativeProperty
    QDeclarativePropertyMap
    QDeclarativePropertyValueInterceptor
    QDeclarativePropertyValueSource
    QDeclarativeScriptString
    QDeclarativeTypeInfo
    QDeclarativeView
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/privateinclude/QtDeclarative
    ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
)

include(util/util.cmake)
include(graphicsitems/graphicsitems.cmake)
include(qml/qml.cmake)

set(DECLARATIVE_HEADERS
    ${DECLARATIVE_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qdeclarativecommon_p.h
)
katie_generate_misc("${DECLARATIVE_HEADERS}" QtDeclarative)
katie_generate_public("${DECLARATIVE_PUBLIC_HEADERS}" QtDeclarative)
katie_generate_package(KtDeclarative "KtCore KtGui KtNetwork KtScript")
katie_setup_target(KtDeclarative ${DECLARATIVE_SOURCES} ${DECLARATIVE_HEADERS})

add_library(KtDeclarative ${KATIE_TYPE} ${KtDeclarative_SOURCES})
target_link_libraries(KtDeclarative PRIVATE ${EXTRA_DECLARATIVE_LIBS})
set_target_properties(KtDeclarative PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Declarative
)

install(
    TARGETS KtDeclarative
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtDeclarative
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtDeclarative)
