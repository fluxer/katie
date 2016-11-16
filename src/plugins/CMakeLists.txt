add_definitions(-DQT_PLUGIN)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/privateinclude/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/privateinclude/QtDBus
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/privateinclude/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtDesigner
    ${CMAKE_BINARY_DIR}/privateinclude/QtDesigner
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/privateinclude/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript
    ${CMAKE_BINARY_DIR}/include/QtSql
    ${CMAKE_BINARY_DIR}/privateinclude/QtSql
    ${CMAKE_BINARY_DIR}/include/QtSvg
    ${CMAKE_BINARY_DIR}/privateinclude/QtSvg
    ${CMAKE_CURRENT_SOURCE_DIR}/script
    ${CMAKE_CURRENT_SOURCE_DIR}/codecs
    ${CMAKE_CURRENT_SOURCE_DIR}/imageformats
    ${CMAKE_CURRENT_SOURCE_DIR}/iconengines
    ${CMAKE_CURRENT_SOURCE_DIR}/accessible
    ${CMAKE_CURRENT_SOURCE_DIR}/qmltooling
    ${CMAKE_SOURCE_DIR}/src/3rdparty/harfbuzz/src
)

if(NOT ${KATIE_TYPE} STREQUAL SHARED)
    add_definitions(-DQT_STATICPLUGIN)
endif()


if(WITH_ACCESSIBILITY)
    include(accessible/widgets/widgets.cmake)
endif()


set(SHAREDBEARER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.cpp
)
set(SHAREDBEARER_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qnetworksession_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer/qbearerengine_impl.h
)
katie_setup_target(sharedbearer ${SHAREDBEARER_SOURCES} ${SHAREDBEARER_HEADERS})
add_library(sharedbearer OBJECT ${sharedbearer_SOURCES})
target_include_directories(sharedbearer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bearer
)

if(KATIE_PLATFORM MATCHES "(linux|freebsd|openbsd)")
    include(bearer/generic/generic.cmake)
    if(WITH_DBUS AND DBUS_FOUND)
        include(bearer/connman/connman.cmake)
        include(bearer/networkmanager/networkmanager.cmake)
    endif()
else()
    include(bearer/generic/generic.cmake)
endif()


include(iconengines/svgiconengine/svgiconengine.cmake)


include(imageformats/gif/gif.cmake)
include(imageformats/ico/ico.cmake)
if(WITH_JPEG AND JPEG_FOUND)
    include(imageformats/jpeg/jpeg.cmake)
endif()
if(WITH_MNG AND MNG_FOUND)
    include(imageformats/mng/mng.cmake)
endif()
include(imageformats/svg/svg.cmake)
include(imageformats/tga/tga.cmake)
if(WITH_TIFF AND TIFF_FOUND)
    include(imageformats/tiff/tiff.cmake)
endif()


if(WITH_DBUS AND DBUS_FOUND)
    include(script/qtdbus/qtdbus.cmake)
endif()


if(WITH_MYSQL AND MYSQL_FOUND)
    include(sqldrivers/mysql/mysql.cmake)
endif()
if(WITH_ODBC AND ODBC_FOUND)
    include(sqldrivers/odbc/odbc.cmake)
endif()
if(WITH_PSQL AND PostgreSQL_FOUND)
    include(sqldrivers/psql/psql.cmake)
endif()
if(WITH_SQLITE AND SQLITE_FOUND)
    include(sqldrivers/sqlite/sqlite.cmake)
endif()


include(designer/qdeclarativeview/qdeclarativeview.cmake)
