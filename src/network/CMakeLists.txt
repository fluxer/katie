add_definitions(-DQT_BUILD_NETWORK_LIB)
set(EXTRA_NETWORK_LIBS KtCore)

set(NETWORK_PUBLIC_HEADERS
    ${NETWORK_PUBLIC_HEADERS}
    QAbstractNetworkCache
    QAbstractSocket
    QAuthenticator
    QFtp
    QHostAddress
    QHostInfo
    QHttp
    QHttpHeader
    QHttpMultiPart
    QHttpPart
    QHttpRequestHeader
    QHttpResponseHeader
    Q_IPV6ADDR
    QIPv6Address
    QLocalServer
    QLocalSocket
    QNetworkAccessManager
    QNetworkAddressEntry
    QNetworkCacheMetaData
    QNetworkConfiguration
    QNetworkConfigurationManager
    QNetworkCookie
    QNetworkCookieJar
    QNetworkDiskCache
    QNetworkInterface
    QNetworkProxy
    QNetworkProxyFactory
    QNetworkProxyQuery
    QNetworkReply
    QNetworkRequest
    QNetworkSession
    QSsl
    QSslCertificate
    QSslCipher
    QSslConfiguration
    QSslError
    QSslKey
    QSslSocket
    QTcpServer
    QTcpSocket
    QUdpSocket
    QUrlInfo
)

include(access/access.cmake)
include(bearer/bearer.cmake)
include(kernel/kernel.cmake)
include(socket/socket.cmake)
include(ssl/ssl.cmake)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_CURRENT_SOURCE_DIR}/access
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer
    ${CMAKE_CURRENT_SOURCE_DIR}/kernel
    ${CMAKE_CURRENT_SOURCE_DIR}/socket
    ${CMAKE_CURRENT_SOURCE_DIR}/ssl
    ${CMAKE_CURRENT_BINARY_DIR}/access
    ${CMAKE_CURRENT_BINARY_DIR}/bearer
    ${CMAKE_CURRENT_BINARY_DIR}/kernel
    ${CMAKE_CURRENT_BINARY_DIR}/socket
    ${CMAKE_CURRENT_BINARY_DIR}/ssl
)

katie_generate_misc("${NETWORK_HEADERS}" QtNetwork)
katie_generate_public("${NETWORK_PUBLIC_HEADERS}" QtNetwork)
katie_generate_map(QtNetwork Q_NETWORK_EXPORT)
katie_resources(${NETWORK_SOURCES} ${NETWORK_HEADERS})
katie_setup_flags()

add_library(KtNetwork ${KATIE_TYPE} ${NETWORK_SOURCES} ${NETWORK_HEADERS})
target_link_libraries(KtNetwork ${EXTRA_NETWORK_LIBS})
set_target_properties(KtNetwork PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Network
)

katie_generate_package(KtNetwork "" "${EXTRA_NETWORK_LIBS}" "KtCore")

install(
    TARGETS KtNetwork
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${QT_LIBRARIES_PATH_INST}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtNetwork
    DESTINATION ${QT_HEADERS_PATH_INST}
    COMPONENT Devel
)

katie_optimize_headers(${QT_HEADERS_PATH}/QtNetwork)
