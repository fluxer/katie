add_definitions(
    -DDBUS_API_SUBJECT_TO_CHANGE
)
set(EXTRA_DBUS_LIBS
    KtCore KtXml
    ${DBUS_LIBRARIES}
)

set(DBUS_PUBLIC_HEADERS
    QDBusAbstractAdaptor
    QDBusAbstractInterface
    QDBusArgument
    QDBusConnection
    QDBusConnectionInterface
    QDBusContext
    QDBusError
    QDBusInterface
    QDBusMessage
    QDBusMetaType
    QDBusPendingCall
    QDBusPendingReply
    QDBusReply
    QDBusServer
    QDBusServiceWatcher
    QDBusUnixFileDescriptor
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtXml
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${DBUS_INCLUDES}
)

set(DBUS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusargument.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusconnectioninterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuserror.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusextratypes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusserver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusconnection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusabstractinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusinterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusabstractadaptor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusreply.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmetatype.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuspendingcall.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuspendingreply.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuscontext.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusconnection_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmessage_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusxmlparser_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusabstractadaptor_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusargument_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusutil_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusabstractinterface_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuscontext_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusthreaddebug_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusintegrator_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuspendingcall_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmetaobject_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusintrospection_p.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusservicewatcher.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusunixfiledescriptor.h
)

set(DBUS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusconnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusconnectioninterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuserror.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusintegrator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmessage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusserver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusabstractinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusinterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusxmlparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusutil.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusintrospection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusabstractadaptor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusinternalfilters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmetaobject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusxmlgenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmisc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusargument.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusreply.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusmetatype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusextratypes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuscontext.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuspendingcall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbuspendingreply.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusservicewatcher.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qdbusunixfiledescriptor.cpp
)

katie_generate_misc("${DBUS_HEADERS}" QtDBus)
katie_generate_public("${DBUS_PUBLIC_HEADERS}" QtDBus)
katie_generate_package(KtDBus "KtCore KtXml")
katie_setup_target(KtDBus ${DBUS_SOURCES} ${DBUS_HEADERS})

add_library(KtDBus ${KATIE_TYPE} ${KtDBus_SOURCES})
target_link_libraries(KtDBus PRIVATE ${EXTRA_DBUS_LIBS})
set_target_properties(KtDBus PROPERTIES
    VERSION ${KATIE_MAJOR}.${KATIE_MINOR}
    SOVERSION ${KATIE_VERSION}
    EXPORT_NAME DBus
)

install(
    TARGETS KtDBus
    EXPORT KatieTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
    DESTINATION ${KATIE_LIBRARIES_PATH}
    COMPONENT Runtime
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/include/QtDBus
    DESTINATION ${KATIE_HEADERS_PATH}
    COMPONENT Devel
)

katie_optimize_headers(${KATIE_HEADERS_PATH}/QtDBus)
