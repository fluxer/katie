# add_definitions()
set(EXTRA_IMPORTS_LIBS KtCore)

include_directories(
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/privateinclude
    ${CMAKE_BINARY_DIR}/include/QtCore
    ${CMAKE_BINARY_DIR}/include/QtGui
    ${CMAKE_BINARY_DIR}/include/QtDeclarative
    ${CMAKE_BINARY_DIR}/include/QtScript
)

# place the imports in a sub-directory the leading of which can be used as imports path,
# this makes testing without installation possible and is relied upon in the test script
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/imports/Qt/labs/folderlistmodel")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/imports/Qt/labs/folderlistmodel")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/folderlistmodel/qmldir"
    "${CMAKE_BINARY_DIR}/imports/Qt/labs/folderlistmodel/qmldir"
)
add_subdirectory(folderlistmodel)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/imports/Qt/labs/particles")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/imports/Qt/labs/particles")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/particles/qmldir"
    "${CMAKE_BINARY_DIR}/imports/Qt/labs/particles/qmldir"
)
add_subdirectory(particles)
