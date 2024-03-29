include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDBus
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtUiTools
    ${CMAKE_BINARY_DIR}/include/QtNetwork
    ${CMAKE_BINARY_DIR}/include/QtScript
    ${CMAKE_BINARY_DIR}/include/QtSvg
)

# place the plugins in a sub-directory the leading of which can be used as plugin path,
# this makes testing without installation possible and is relied upon in the exec script
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/imageformats")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/imageformats")
add_subdirectory(imageformats/svg)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/script")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/script")
if(WITH_DBUS AND DBUS_FOUND)
    add_subdirectory(script/qtdbus)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/designer")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/designer")
add_subdirectory(designer/qdeclarativeview)
