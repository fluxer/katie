add_definitions(-DQT_BUILD_NETWORK_LIB -DQT_USE_SYSTEM_PROXIES)
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
    Q_IPV6ADDR
    QIPv6Address
    QLocalServer
    QLocalSocket
    QNetworkAccessManager
    QNetworkConfiguration
    QNetworkCookie
    QNetworkCookieJar
    QNetworkDiskCache
    QNetworkInterface
    QNetworkProxy
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
    QHttpMultiPart
    # mixed
    QNetworkAddressEntry
    QNetworkConfigurationManager
    QNetworkCacheMetaData
    QHttpHeader
    QHttpPart
    QHttpRequestHeader
    QHttpResponseHeader
    QNetworkProxyFactory
    QNetworkProxyQuery
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
    ${OPENSSL_INCLUDE_DIR}
)

if(WITH_RESOLV AND RESOLV_FOUND)
    set(EXTRA_NETWORK_LIBS
        ${EXTRA_NETWORK_LIBS}
        ${RESOLV_LIBRARIES}
    )
    include_directories(${RESOLV_INCLUDES})
endif()

set(NETWORK_HEADERS
    ${NETWORK_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/qnetworkcommon_p.h
)

katie_generate_misc("${NETWORK_HEADERS}" QtNetwork)
katie_generate_public("${NETWORK_PUBLIC_HEADERS}" QtNetwork)
katie_generate_package(KtNetwork "KtCore")
katie_setup_target(KtNetwork ${NETWORK_SOURCES} ${NETWORK_HEADERS})

add_library(KtNetwork ${KATIE_TYPE} ${KtNetwork_SOURCES})
target_link_libraries(KtNetwork ${EXTRA_NETWORK_LIBS})
set_target_properties(KtNetwork PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME Network
)

install(
    TARGETS KtNetwork
    EXPORT KatieLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_RELATIVE}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtNetwork
    DESTINATION ${KATIE_HEADERS_RELATIVE}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_FULL}/QtNetwork)
