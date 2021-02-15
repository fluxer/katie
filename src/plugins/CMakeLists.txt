include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/include/QtSvg
)

# place the plugins in a sub-directory the leading of which can be used as plugin path,
# this makes testing without installation possible and is relied upon in the exec script
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/accessible")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/accessible")
if(WITH_ACCESSIBILITY)
    add_subdirectory(accessible/widgets)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/bearer")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/bearer")

set(SHAREDBEARER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)
set(SHAREDBEARER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)
katie_setup_target(sharedbearer ${SHAREDBEARER_SOURCES} ${SHAREDBEARER_HEADERS})
add_library(sharedbearer OBJECT ${sharedbearer_SOURCES})
target_include_directories(sharedbearer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer
    ${CMAKE_CURRENT_BINARY_DIR}/bearer
)

add_subdirectory(bearer/generic)
if(CMAKE_SYSTEM_NAME MATCHES "Linux" AND WITH_DBUS AND DBUS_FOUND)
    add_subdirectory(bearer/connman)
    add_subdirectory(bearer/networkmanager)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/iconengines")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/iconengines")
add_subdirectory(iconengines/svgiconengine)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/imageformats")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/imageformats")
add_subdirectory(imageformats/gif)
add_subdirectory(imageformats/ico)
add_subdirectory(imageformats/svg)
add_subdirectory(imageformats/tga)
if(WITH_JPEG AND JPEG_FOUND)
    add_subdirectory(imageformats/jpeg)
endif()
if(WITH_TIFF AND TIFF_FOUND)
    add_subdirectory(imageformats/tiff)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/script")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/script")
if(WITH_DBUS AND DBUS_FOUND)
    add_subdirectory(script/qtdbus)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/sqldrivers")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/sqldrivers")
if(WITH_MYSQL AND MYSQL_FOUND)
    add_subdirectory(sqldrivers/mysql)
endif()
if(WITH_ODBC AND ODBC_FOUND)
    add_subdirectory(sqldrivers/odbc)
endif()
if(WITH_POSTGRESQL AND POSTGRESQL_FOUND)
    add_subdirectory(sqldrivers/psql)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/designer")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/designer")
add_subdirectory(designer/qdeclarativeview)
